#include "SpriteRenderer.h"


namespace graphics
{
	SpriteRenderer::SpriteRenderer(Shader* shader)
		: shader(shader), vao(nullptr), vbo(nullptr), ibo(nullptr)
	{
		InitData();
	}

	SpriteRenderer::~SpriteRenderer()
	{
		delete shader;
		delete vao;
		delete vbo;
		delete ibo;
	}

	void SpriteRenderer::DrawSprite(Texture2D* texture, math::vec2<float> position,
		math::vec2<float> size, float rotation, math::vec3<float> color)
	{
		math::mat4 model = CreateModel(position, size, rotation);

		shader->Use();
		shader->SetUniformMatrix4f("model", model);
		shader->SetUniform3f("modelColor", color);

		texture->Activate();

		vao->Use();
		ibo->Use();
		GLCall(glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0));
	}

	void SpriteRenderer::DrawQueue(unsigned int queueID)
	{
		//instance_vbo = new VertexBuffer(&queues[queueID].models[0][0], sizeof(math::vec2<float>) * queues[queueID].size());

		VertexBufferLayout layout;
		layout.Add<float>(2, 1);

	}
	

	math::mat4<float> SpriteRenderer::CreateModel(math::vec2<float> position, math::vec2<float> size, float rotation) const
	{
		math::mat4 model = math::IdentityMatrix<float>();
		model = math::Translate(model, position);

		// Move rotation origin to the centre
		model = math::Translate(model, math::vec2(size.x / 2, size.y / 2));
		// Rotate
		model = math::Rotate(model, math::Radians(rotation), math::vec3(0.0f, 0.0f, 1.0f));
		// Return rotation origin back to top-left corner
		model = math::Translate(model, math::vec2(size.x / (-2), size.y / (-2)));
		// Scale to given size
		model = math::Scale(model, math::vec3(size.x, size.y, 1.0f));

		return model;
	}

	void SpriteRenderer::InitData()
	{
		float vertices[] = {
			// coordinates		// texture coords
			0.0f, 1.0f,			0.0f, 1.0f,
			1.0f, 0.0f,			1.0f, 0.0f,
			0.0f, 0.0f,			0.0f, 0.0f,
			1.0f, 1.0f,			1.0f, 1.0f,
		};

		unsigned int indexes[] = {
			0, 1, 2,
			0, 3, 1
		};

		vao = new VertexArray();
		vbo = new VertexBuffer(vertices, sizeof(vertices));
		ibo = new IndexBuffer(indexes, 6);

		VertexBufferLayout layout;
		layout.Add<float>(2);
		layout.Add<float>(2);

		vao->AddBuffer(*vbo, layout);
	}
}