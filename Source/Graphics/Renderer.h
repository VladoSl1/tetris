#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mathematics.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#ifdef _DEBUG
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x;
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


namespace graphics
{
	class Timer
	{
	public:
		inline double GetTime() const
		{
			return glfwGetTime();
		}

		inline double DeltaTime() const
		{
			return GetTime() - lastTime;
		}

		inline void UpdateTime()
		{
			lastTime = GetTime();
		}

		Timer() { UpdateTime(); }

	private:
		double lastTime;
	};

	class Renderer
	{
	public:
		unsigned int Width, Height;

		Renderer(unsigned int width, unsigned int height, const char* name, bool VSync = true);
		~Renderer();

		/* Calls PollEvents() and SwapBuffers() */
		void Update();

		/* Swap front and back buffers */
		void SwapBuffers();

		/* FPS counter */
		void ShowFPS();

		/* Poll for and process events */
		void PollEvents();

		/* Clear screen with given color (DEFAULT: vec4(0.0f, 0.0f, 0.0f, 0.0f)) */
		void ClearScreen(math::vec4<float> color = math::vec4(0.0f, 0.0f, 0.0f, 0.0f));

		/* Draw image with given VertexArray, IndexBuffer and Shader, to screen */
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);

		/* Close window */
		void CloseWindow();

		inline int WindowShouldClose() const { return glfwWindowShouldClose(window); }
		inline GLFWwindow* GetWindow() const { return window; }

	private:
		GLFWwindow* window;
		const char* window_name;

		/* FPS counter variables */
		double lastTime = 0.0L;
		unsigned int FrameNumber = 0;
	};
}

