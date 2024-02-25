#pragma once

#include "Texture.h"

#include "SpriteRenderer.h"

#include "Mathematics.h"


namespace graphics
{
	struct GameObject
	{
		math::vec2<float> position, size, velocity;
		math::vec3<float> color;
		float rotation;

		Texture2D* texture;

		GameObject();
		GameObject(math::vec2<float> pos, math::vec2<float> size, Texture2D* texture,
			math::vec3<float> color = math::vec3<float>(1.0f), math::vec2<float> velocity = math::vec2(0.0f, 0.0f));

		virtual void Draw(SpriteRenderer& renderer);
	};
}

