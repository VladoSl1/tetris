#pragma once

/* NOT PART OF ENGINE */

#include "Mathematics.h"

#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

namespace graphics
{
	namespace shapes
	{
		class Rectangle
		{
		public:
			Rectangle(math::vec3<float> start, math::vec3<float> end)
				: vao(nullptr), ibo(nullptr), shader(nullptr), vbo(nullptr), layout(nullptr), texture(nullptr)
			{
				float verticies[] = {
					// positions				 // texture coords
					start.x, start.y, 0.0f,		 0.0f, 1.0f,	// top left 
					start.x, end.y, 0.0f,		 1.0f, 1.0f,	// bottom left
					end.x, end.y, 0.0f,			 1.0f, 0.0f,	// bottom right
					end.x, start.y, 0.0f,		 0.0f, 0.0f		// top right
				};

				/* assign coordinates to corresponding triangles */
				unsigned int indicies[] = {
					0, 1, 2,
					0, 2, 3
				};

				shader = new Shader("Resources/Shaders/vertex_shader.shader", "Resources/Shaders/fragment_shader.shader");
				shader->Use();
				//shader->SetUniformMatrix4f("projection", math::Ortho(0, 700, 0, 800));

				vao = new VertexArray();
				vao->Use();

				vbo = new VertexBuffer(verticies, sizeof(verticies));
				vbo->Use();

				ibo = new IndexBuffer(indicies, 6);
				ibo->Use();

				/* Divide data given in verticies[] into corresponding layout variables in shader */
				layout = new VertexBufferLayout();
				layout->Add<float>(3);
				layout->Add<float>(2);

				vao->AddBuffer(*vbo, *layout);
			}

			~Rectangle()
			{
				delete vao;
				delete ibo;
				delete shader;
				delete vbo;
				delete layout;
				delete texture;
			}
			
			void SetTexture(const char* path)
			{
				std::string temp(path);
				auto it = std::find(texturePaths.begin(), texturePaths.end(), temp);
				
				/* If the texture wasn't already loaded */
				if (it == texturePaths.end())
				{
					texturePaths.push_back(temp);
					textures.push_back(new Texture2D(path));
					texture = textures[textures.size() - 1];
				}
				else
					texture = textures[it - texturePaths.begin()];
			
			}

			/* Translate every coordinate into corresponding position and draw rectangle */
			void DrawRel(Renderer& renderer, math::vec3<float> pos)
			{
				if (shader == nullptr)
					throw "Shader not set";
				
				shader->Use();

				if (texture != nullptr)
					texture->Activate();

				math::mat4 model = math::IdentityMatrix<float>();
				model = math::Translate(model, pos);

				shader->SetUniformMatrix4f("model", model);
				
				renderer.Draw(*vao, *ibo, *shader);
			}
			
			void Draw(Renderer& renderer, math::vec3<float> position)
			{
				if (shader == nullptr)
					throw "Shader not set";

				shader->Use();

				if (texture != nullptr)
					texture->Activate();

				math::mat4 model = math::IdentityMatrix<float>();
				model = math::Translate(model, position);



			}

		private:
			Shader* shader;
			Texture2D* texture;
			VertexArray* vao;
			IndexBuffer* ibo;
			VertexBuffer* vbo;
			VertexBufferLayout* layout;

			std::vector<Texture2D*> textures;
			std::vector<std::string> texturePaths;
		};
	}
}