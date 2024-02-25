#pragma once

#include "IrrKlang/irrKlang.h"

#include <iostream>

#pragma comment(lib, "irrKlang.lib")

namespace graphics
{
	class SoundDevice
	{
	public:
		SoundDevice()
		{
			SoundEngine = irrklang::createIrrKlangDevice();
		}

		void Play(const char* soundPath, bool loop = false)
		{
			SoundEngine->play2D(soundPath, true);
		}

	private:

		irrklang::ISoundEngine* SoundEngine;
	};
}

