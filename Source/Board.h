#pragma once

#include "Pieces.h"

#include <vector>
#include <functional>

template<typename Type>
inline bool inVec(const std::vector<Type>& vec, const Type& item)
{
	return std::find(vec.begin(), vec.end(), item) != vec.end();
}

namespace GameConstants
{
	static const unsigned int ForceDown			= 1;
	static const unsigned int LevelUpLineNum	= 10;

	static const unsigned int OneLine			= 40;
	static const unsigned int TwoLines			= 100;
	static const unsigned int ThreeLines		= 300;
	static const unsigned int FourLines			= 1200;
};

struct Piece
{
	unsigned char Index			= 0;
	unsigned char Color			= 0;
	unsigned char Rotation		= 0;
	unsigned int mapOriginPoint	= 0;

	std::vector<unsigned int> parts;
	inline std::vector<unsigned int>* operator&() { return &parts; }
};

struct PreviewBoard
{
	using uchar = unsigned char;

	std::vector<unsigned int> piecePositions;
	std::vector<uchar> pieceColors;

	PreviewBoard(std::vector<unsigned char> pieces)
	{
		std::vector<unsigned char> pieceMap;
		uchar shift = 0;

		for (uchar i = 0; i < pieces.size(); ++i)
		{
			shift += (uchar)(i > 0 ? (floor(pieceMap[3] / 4) + 2) * 4 : 0);
			pieceMap = Pieces::GetPieceMap(pieces[i]);

			for (uchar x : pieceMap)
			{
				piecePositions.push_back(x + shift);
				pieceColors.push_back(pieces[i]);
			}
		}
	}
	inline unsigned int length() const { return pieceColors.size(); }
};


class Board
{
	using uint = unsigned int;
	using uchar = unsigned char;

public:
	const uint Width, Height;
	std::vector<uchar> board;

	Board(uint width, uint height);

	void Update(int operation = 0, bool userInteration = false);

	/* Returns list of 4 pieces */
	std::vector<uchar> PiecePreview() const;

	/* Actuall gameOver state */
	inline bool GameOver() const { return gameOver; }
	/* Actuall score */
	inline unsigned long Score() const { return score; }
	/* Number of elements in board list (Width * Height) */
	inline uint Size() const { return board.size(); }
	/* Actuall level */
	inline uint Level() const { return static_cast<uint>(level); }

	inline uchar operator[](int index) const { return board[index]; }

private:
	bool gameOver;
	unsigned long score;
	uchar level;
	uchar cleanedLines;

	Piece currentPiece;
	std::vector<uint> pieceShadow;

	std::vector<uchar> pieceOrder;
	
	inline bool IsPartOfPiece(std::vector<uint> piece, uint pos) const {
		return (board[pos] & Pieces::ShadowMask) || inVec(piece, pos);
	}
	inline bool IsFreeSpace(std::vector<uint> piece, uint pos) const {
		return (IsPartOfPiece(piece, pos) || board[pos] == 0);
	}

	bool PieceLeft();
	bool PieceRight();
	bool RotatePiece();
	bool PieceDown(std::vector<uint>* piece);

	bool SpawnNewPiece();
	inline void DrawCurrentPiece();

	void CleanLines();
	void DeleteLine(uint line);
	void DeleteLines(const std::vector<uint>& filled_lines);

	void CreateShadow();
	inline void DeleteShadow();

	bool MakeMove(int move, std::vector<uint>* piece, std::function<bool(int)> moveCondition);
	bool MakeRotation(std::function<uchar(uchar)> a, int correction);

	/* Overwrite old piece's list with new piece's position and apply change to board */
	inline void OverwritePiece(std::vector<uint>* oldPiece, const std::vector<uint>& newPiece, uchar color);
};