#pragma once

#include <string>


/*
Texture can be assign to any object.
It's coordinates are coordinates in the object (everything is happening inside).
Needs to be programmed in the fragment shader.
*/

namespace graphics
{
	class Texture2D
	{
	public:
		int width, height, nrChannels;

		Texture2D() : width(0), height(0), nrChannels(0), m_ID(0) { }
		Texture2D(const char* path);
		~Texture2D();

		/* Set texture as active */
		void Activate(unsigned int slot = 0) const;

		inline unsigned int ID() const { return m_ID; }

	private:
		unsigned int m_ID;

		/* Load image */
		unsigned char* LoadData(const char* path);

		/* Free memory */
		void DeleteData(unsigned char* data);
	};

}
