#include "GameObject.h"

namespace graphics
{

	GameObject::GameObject()
		: position(0.0f, 0.0f), size(1.0f, 1.0f), texture(nullptr), color(1.0f), velocity(0.0f, 0.0f), rotation(0.0f) { }

	GameObject::GameObject(math::vec2<float> pos, math::vec2<float> size, Texture2D* sprite, math::vec3<float> color,
		math::vec2<float> velocity)
		: position(pos), size(size), texture(sprite), color(color), velocity(velocity), rotation(0.0f) { }

	void GameObject::Draw(SpriteRenderer& renderer)
	{
		renderer.DrawSprite(texture, position, size, rotation, color);
	}
}
