
#include "Mathematics.h"

#include <iostream>

static const float space = 0.05f;

class Orientation
{
public:
	inline static float BlockSize = 0.0f;
	inline static float PreviewBlockSize = 0.0f;

	inline static const float ScreenBorder = 13.5f;

	inline static unsigned int ScreenWidth = 700;
	inline static unsigned int ScreenHeight = 800;

	inline static math::vec2<float> PieceScreenCoordsFromPosition(const Board& board, unsigned int position)
	{
		math::vec2<unsigned int> coords = CoordinatesFromPos(board, position);

		float x = ScreenBorder + BlockSize * (coords.x + 1);
		float y = ScreenBorder + BlockSize * (coords.y + 1);

		return math::vec2(x, y);
	}

	inline static math::vec2<float> PiecePreviewScreenCoordsFromPos(const Board& board, unsigned int position)
	{
		math::vec2<unsigned int> coords = { static_cast<unsigned int>(position % 4), static_cast<unsigned int>(floor(position / 4)) };

		float leftBoardSide = ScreenBorder + (board.Width + 3) * BlockSize;

		float x = coords.x * PreviewBlockSize + leftBoardSide + 40.0f;
		float y = ScreenBorder * 5 + coords.y * PreviewBlockSize;

		return math::vec2(x, y);
	}

	inline static math::vec2<float> BorderVScreenCoordsFromPos(const Board& board, unsigned int position)
	{
		math::vec2 coords = CoordinatesFromPos(board, position);

		float x = ScreenBorder + (coords.x == 0 ? 0 : (board.Width + 1) * BlockSize);
		float y = ScreenBorder + BlockSize * (coords.y + 1);

		return math::vec2(x, y);
	}

	inline static math::vec2<float> BorderHScreenCoordsFromPos(const Board& board, unsigned int position)
	{
		math::vec2 coords = CoordinatesFromPos(board, position);

		float x = ScreenBorder + BlockSize * (coords.x + 1);
		float y = ScreenBorder + (coords.y == 0 ? 0 : BlockSize * (coords.y + 2));

		return math::vec2(x, y);
	}

	inline static math::vec2<unsigned int> CoordinatesFromPos(const Board& board, unsigned int position)
	{
		return math::vec2<unsigned int>((position % board.Width), floor(position / board.Width));
	}

	inline static unsigned int PositionFromCoordinates(const Board& board, const math::vec2<int>& coordinates)
	{
		return static_cast<unsigned int>(coordinates.x + board.Width * coordinates.y);
	}

	inline static void Init(const Board& board)
	{
		BlockSize = 30.0f;
		PreviewBlockSize = BlockSize * 1.0f;
	}

	inline static float AspectRatio()
	{
		return static_cast<float>(ScreenWidth / ScreenHeight);
	}
};