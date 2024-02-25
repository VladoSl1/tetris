#pragma once

#include <map>
#include <string>

#include "Renderer.h"
#include "Mathematics.h"

#include "ft2build.h"
#include FT_FREETYPE_H


namespace graphics
{
	struct Character
	{
		unsigned int ID;
		math::vec2<int> Size;
		math::vec2<int> Bearing;
		unsigned int Advance;
	};

	enum class CharacterSets
	{
		ASCII
	};

	class Text
	{

	public:
		Text(const Renderer& renderer, const char* fontPath, unsigned int fontSize = 48, CharacterSets characterSetName = CharacterSets::ASCII);
		~Text();

		void CreateText(std::string text, float x, float y, float scale, math::vec3<float> color);
		bool LoadCharacterSet(CharacterSets characterSetName);

		inline float TextLength(std::string text, float scale = 1.0f) const
		{
			float length = 0;

			for (char letter : text)
			{
				Character ch = characters.at(letter);
				length += ch.Size.x * scale;
			}

			return length;
		}
		
	private:
		std::map<char, Character> characters;
		FT_Library ft;
		FT_Face face;
		
		Shader* shader;
		VertexArray* vao;
		VertexBuffer* vbo;

		void InitShader();
	};
}


