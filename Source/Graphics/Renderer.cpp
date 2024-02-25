#include "Renderer.h"

#include <iostream>
#include <sstream>


void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

/* Resize window */
void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    GLCall(glViewport(0, 0, width, height));
}

namespace graphics
{
    Renderer::Renderer(unsigned int width, unsigned int height, const char* name, bool VSync)
        : Width(width), Height(height), window(nullptr)
    {
        /* Initialize the library */
        if (!glfwInit())
        {
            std::cout << "Failed to initialize GLFW library\n";
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(width, height, name, NULL, NULL);
        window_name = name;

        if (!window)
        {
            std::cout << "Failed to create GLFW window\n" << std::endl;
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        /* Cap frame rate to display max frame rate */
        if (!VSync)
            glfwSwapInterval(0);

        /* Resize window when user it resizing it */
        glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);

        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initialize GLEW\n";
            return;
        }
    }

    Renderer::~Renderer()
    {
        CloseWindow();
    }
    
    void Renderer::CloseWindow()
    {
        glfwTerminate();
    }
    
    void Renderer::ClearScreen(math::vec4<float> color)
    {
        GLCall(glClearColor(color.x, color.y, color.z, color.w));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void Renderer::Update()
    {
        SwapBuffers();
        PollEvents();
    }

    void Renderer::ShowFPS()
    {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        FrameNumber++;

        if (deltaTime >= 1.0f)
        {
            double fps = double(FrameNumber) / deltaTime;

            std::stringstream ss;
            ss << window_name << " [" << fps << "FPS]";

            glfwSetWindowTitle(window, ss.str().c_str());

            FrameNumber = 0;
            lastTime = currentTime;
        }
    }

    void Renderer::SwapBuffers()
    {
        glfwSwapBuffers(window);
    }

    void Renderer::PollEvents()
    {
        glfwPollEvents();
    }

    void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
    {
        shader.Use();
        va.Use();
        ib.Use();

        GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0));
    }
}