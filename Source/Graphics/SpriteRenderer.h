#pragma once

#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

#include "Mathematics.h"

namespace graphics
{
	struct GraphicQueue
	{
		Texture2D* texture;
		math::vec3<float> color;
		std::vector<math::mat4<float>> models;
		VertexBuffer* instance_vbo;
		
		~GraphicQueue() {
			delete texture;
			delete instance_vbo;
		}

		inline unsigned int size() const { return models.size(); }
		inline void AddModel(math::mat4<float> model) {	models.push_back(model); }
	};

	class SpriteRenderer
	{
	public:
		SpriteRenderer(Shader* shader);
		~SpriteRenderer();

		void DrawSprite(Texture2D* texture, math::vec2<float> position,
			math::vec2<float> size = math::vec2(15.0f, 15.0f), float rotation = 0.0f,
			math::vec3<float> color = math::vec3(1.0f));

		/* NOT COMPLETED QUEUE */
		inline unsigned int CreateQueue(Texture2D* texture, math::vec3<float> color = math::vec3(1.0f)) {
			queues.push_back(GraphicQueue({ texture, color }));
			return queues.size() - 1;
		}

		inline void AddToQueue(unsigned int ID, math::vec2<float> position, math::vec2<float> size = math::vec2(15.0f, 15.0f),
			float rotation = 0.0f) {
			queues[ID].AddModel(CreateModel(position, size, rotation));
		}

		inline void FinishStaticQueue(unsigned int ID)
		{

		}

		void DrawQueue(unsigned int queueID);

	private:
		Shader* shader;
		VertexArray* vao;
		VertexBuffer* vbo;
		IndexBuffer* ibo;

		std::vector<GraphicQueue> queues;

		void InitData();

		math::mat4<float> CreateModel(math::vec2<float> position, math::vec2<float> size, float rotation) const;
	};
}

