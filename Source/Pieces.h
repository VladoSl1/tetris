#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>


class Pieces
{
public:
	static const unsigned int ShadowMask	= 0b1000;
	static const unsigned int PieceMask		= 0b0111;

	inline static const std::string TextureDefaultPath = "Resources/BlockTextures/";
	inline static const std::string GreyBorderTexturePath = TextureDefaultPath + "grey.png";

	/* Piece type ID is the same as its color ID */
	static std::string ColorTexture(unsigned char color)
	{
		std::string path = TextureDefaultPath;

		unsigned char type = color & PieceMask;
		unsigned char texType = color & ShadowMask;

		switch (type)
		{
		case 0:					    break;			// NONE
		case 1: path += "cyan";		break;
		case 2: path += "red";		break;
		case 3: path += "blue";		break;
		case 4: path += "orange";	break;
		case 5: path += "yellow";	break;
		case 6: path += "magenta";	break;
		case 7: path += "green";	break;
		}
		
		if (texType)
			path += "_outline";

		if (color)
			path += ".png";

		return path;
	}

	/* Piece map for each piece */
	static std::vector<unsigned char> GetPieceMap(unsigned char type)
	{
		std::vector<unsigned char> result;

		switch (type)
		{
		case 0:
			result = { };
			break;
		case 1:
			result = { 2, 6, 10, 14 };
			break;
		case 2:
			result = { 2, 5, 6, 9 };
			break;
		case 3:
			result = { 1, 2, 5, 9 };
			break;
		case 4:
			result = { 1, 5, 9, 10 };
			break;
		case 5:
			result = { 1, 2, 5, 6 };
			break;
		case 6:
			result = { 1, 5, 6, 9 };
			break;
		case 7:
			result = { 1, 5, 6, 10 };
			break;
		}

		return result;
	}

	/* Generates list with shuffled number from _start (including) to _end (excluding) */
	static std::vector<unsigned char> GenerateRandomSequence(unsigned int _start, unsigned int _end)
	{
		using namespace std;

		// fill result list with numbers from _start (including) to _end (excluding)
		vector<unsigned char> result(_end - _start);
		iota(begin(result), end(result), _start);						

		// Create random number generator
		random_device rd;
		mt19937::result_type seed = rd() ^
			(
			(mt19937::result_type)
			chrono::duration_cast<std::chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count() +
			(mt19937::result_type)
			 chrono::duration_cast<std::chrono::microseconds>( chrono::high_resolution_clock::now().time_since_epoch()).count()
			);
		std::mt19937 gen(seed);

		// Shuffle list
		std::shuffle(std::begin(result), std::end(result), gen);

		return result;
	}
};

