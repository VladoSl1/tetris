#include "Board.h"
#include "Orientation.h"

#include <algorithm>
#include <iostream>
#include <set>

template<typename Type>
inline void PushBack(std::vector<Type>* baseVec, const std::vector<Type>& vec)
{
	baseVec->insert(baseVec->end(), vec.begin(), vec.end());
}


Board::Board(uint width, uint height)
	: board(width * height, 0), Width(width), Height(height),
	  score(0), gameOver(false), level(1), cleanedLines(0)
{
	pieceOrder = Pieces::GenerateRandomSequence(1, 8);
	PushBack(&pieceOrder, Pieces::GenerateRandomSequence(1, 8));

	SpawnNewPiece();
}

std::vector<unsigned char> Board::PiecePreview() const
{
	std::vector<uchar> result;

	for (uchar i = 0; i < 4; ++i)
		result.push_back(pieceOrder[currentPiece.Index + i + 1]);

	return result;
}

void Board::Update(int operation, bool userInteration)
{
	switch (operation)
	{
	case 1:																// DOWN			S / DOWN_ARROW
		if (userInteration)
			score += GameConstants::ForceDown;
		if (!PieceDown(&currentPiece))
		{
			CleanLines();
			if (!SpawnNewPiece())
				gameOver = true;
		}
		break;

	case 2: PieceLeft();		break;				// LEFT			A / RIGHT_ARROW
	case 3: PieceRight();		break;				// RIGHT		D / LEFT_ARROW
	case 4: RotatePiece();		break;				// ROTATE		W / UP_ARROW / SPACE
	}
}

bool Board::MakeMove(int move, std::vector<uint>* piece, std::function<bool(int)> moveCondition)
{
	std::vector<uint> pieceCopy(*piece);
	if (piece == &currentPiece && move != Width)
		DeleteShadow();

	for (uchar i = 0; i < pieceCopy.size(); ++i)
	{
		uint newPos = pieceCopy[i] + move;
		if (newPos >= Size() || !(moveCondition(newPos)) || !IsFreeSpace(*piece, newPos))
		{
			if (piece == &currentPiece && move != Width)
				CreateShadow();

			DrawCurrentPiece();
			return false;
		}

		pieceCopy[i] = newPos;
	}
	
	OverwritePiece(piece, pieceCopy, currentPiece.Color);

	if (piece == &currentPiece)
	{
		currentPiece.mapOriginPoint += move;
		if (move != Width)
			CreateShadow();
	}

	return true;
}

void Board::OverwritePiece(std::vector<uint>* oldPiece, const std::vector<uint>& newPiece, uchar color)
{
	for (uchar i : *oldPiece)
		board[i] = 0;

	for (uchar i : newPiece)
		board[i] = color;

	*oldPiece = std::vector<uint>(newPiece);
}

bool Board::PieceDown(std::vector<uint>* piece)
{
	return MakeMove(Width, piece, [&](uint position) -> bool { return position < Size(); });;
}

bool Board::PieceLeft()
{
	return MakeMove(-1, &currentPiece, [&](uint position) -> bool { return position % Width < Width - 1; });
}

bool Board::PieceRight()
{
	return MakeMove(1, &currentPiece, [&](uint position) -> bool { return position % Width > 0; });
}

bool Board::MakeRotation(std::function<uchar(uchar)> rotation, int correction)
{
	std::vector<uchar> pieceMap = Pieces::GetPieceMap(pieceOrder[currentPiece.Index]);
	std::vector<uint> rotatedPiece;

	DeleteShadow();

	for (uchar i : pieceMap)
	{
			uchar newSquarePos = rotation(i);
			uint newPos = (uint)(currentPiece.mapOriginPoint + newSquarePos % 4 + Width * floor(newSquarePos / 4)) - correction;
			uint oldPos = (uint)(currentPiece.mapOriginPoint + i % 4 + Width * floor(i / 4));

			bool sideOverflow = abs(int(newPos % Width) - int(oldPos % Width)) > 3;
			if (sideOverflow || newPos >= Size() || !IsFreeSpace(currentPiece.parts, newPos))
			{
				CreateShadow();
				return false;
			}

			rotatedPiece.push_back(newPos);
	}

	OverwritePiece(&currentPiece, rotatedPiece, currentPiece.Color);
	CreateShadow();

	return true;
}

bool Board::RotatePiece()
{
	bool succes = true;
	currentPiece.Rotation = (currentPiece.Rotation + 1) % 4;
	
	switch (currentPiece.Rotation)
	{
	case 0:
		succes = MakeRotation([](uchar i) -> uchar { return (uchar)(i); }, 0);
		break;
	case 1:
		succes = MakeRotation([](uchar i) -> uchar { return (uchar)(3 + 4 * (i % 4) - floor(i / 4)); }, Width + 1);
		break;
	case 2:
		succes = MakeRotation([](uchar i) -> uchar { return (uchar)(15 - i); }, 2 * Width);
		break;
	case 3:
		succes = MakeRotation([](uchar i) -> uchar { return (uchar)(12 - 4 * (i % 4) + floor(i / 4)); }, Width - 1);
		break;
	}

	if (!succes)
		currentPiece.Rotation = (currentPiece.Rotation - 1) % 4;

	return true;
}

bool Board::SpawnNewPiece()
{
	Piece oldPiece = Piece(currentPiece);
	currentPiece = Piece();

	/* Index */
	if (oldPiece.Index + 1 > 6)
	{
		pieceOrder = std::vector<uchar>(pieceOrder.begin() + 7, pieceOrder.end());
		PushBack(&pieceOrder, Pieces::GenerateRandomSequence(1, 8));
	}
	else
		currentPiece.Index = oldPiece.Index + 1;

	/* Map Origin */
	std::vector<uchar> pieceMap = Pieces::GetPieceMap(pieceOrder[currentPiece.Index]);
	currentPiece.mapOriginPoint = (uint)(floor(Width / 2) - 2);
	
	/* Color */
	currentPiece.Color = pieceOrder[currentPiece.Index];

	/* Parts */
	for (uchar i : pieceMap)
	{
		uint pos = (uint)(currentPiece.mapOriginPoint + i % 4 + Width * floor(i / 4));
		if (board[pos])
			return false;

		board[pos] = currentPiece.Color;
		currentPiece.parts.push_back(pos);
	}

	pieceShadow = std::vector<uint>();
	CreateShadow();

	return true;
}

void Board::CreateShadow()
{
	std::vector<uint> currentPieceShadow = std::vector<uint>(currentPiece.parts);

	while (PieceDown(&currentPieceShadow));
	OverwritePiece(&pieceShadow, currentPieceShadow, currentPiece.Color | Pieces::ShadowMask);

	DrawCurrentPiece();
}

inline void Board::DeleteShadow()
{
	OverwritePiece(&pieceShadow, std::vector<uint>(), 0);
}

inline void Board::DrawCurrentPiece()
{
	for (uint i : currentPiece.parts)
		board[i] = currentPiece.Color;
}

void Board::CleanLines()
{
	std::set<uint> reviewed_lines;
	std::vector<uint> filled_lines;

	for (uint i : currentPiece.parts)
	{
		uint line = (uint)(floor(i / Width));

		/* if line was already reviewed */
		if (reviewed_lines.find(line) != reviewed_lines.end())
			continue;
		else
			reviewed_lines.insert(line);

		bool filled_line = true;

		/* Check every element in line (if 0 break) */
		for (uint x = line * Width; x < Width * (line + 1); ++x)
		{
			if (!board[x])
			{
				filled_line = false;
				break;
			}
		}

		if (filled_line)
			filled_lines.push_back(line);
	}

	if (filled_lines.size() > 0)
		DeleteLines(filled_lines);
}

void Board::DeleteLine(uint line)
{
	/* Delete Line */
	for (uint x = 0; x < Width; ++x)
	{
		board[line * Width + x] = board[(line - 1) * Width + x];
		board[(line - 1) * Width + x] = 0;
	}

	/* Move lines downward */
	bool empty_line = true;
	uint currentLine = line - 2;

	do
	{
		empty_line = true;
		for (uint i = currentLine * Width; i < Width * (currentLine + 1); ++i)
		{
			if (board[i])
			{
				empty_line = false;
				board[i + Width] = board[i];
				board[i] = 0;
			}
		}
		--currentLine;
	} while (!empty_line);
}

void Board::DeleteLines(const std::vector<uint>& filled_lines)
{
	if (level < 16)
	{
		cleanedLines += (uchar)filled_lines.size();
		if (cleanedLines > GameConstants::LevelUpLineNum)
		{
			level += 1;
			cleanedLines %= GameConstants::LevelUpLineNum;
		}
	}

	std::vector<uint> deleted_lines(filled_lines);
	std::sort(deleted_lines.begin(), deleted_lines.end(), std::greater<int>());

	switch (deleted_lines.size())
	{
	case 1:
		score += level * GameConstants::OneLine;
		break;
	case 2:
		score += level * GameConstants::TwoLines;
		break;
	case 3:
		score += level * GameConstants::ThreeLines;
		break;
	case 4:
		score += level * GameConstants::FourLines;
		break;
	}
	
	for (uchar i = 0; i < deleted_lines.size(); ++i)
		DeleteLine(deleted_lines[i] + i);
}