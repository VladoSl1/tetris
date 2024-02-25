#include "Graphics/Renderer.h"
#include "Graphics/Shapes.h"
#include "Graphics/Text.h"
#include "Graphics/SoundDevice.h"

#include "Graphics/Shader.h"
#include "Graphics/SpriteRenderer.h"

#include "Mathematics.h"

#include "Board.h"
#include "Pieces.h"
#include "Orientation.h"

#include <iostream>

const unsigned int BoardWidth = 10;
const unsigned int BoardHeight = 24;

const float BaseGameDelayMS = 500.0f;
const float InputDelayMS = 85.0f;

unsigned short ProcessKeyboardInput(GLFWwindow* window);

inline void DrawText(graphics::Text* textRenderer, const Board& board);
inline void DrawBoard(graphics::SpriteRenderer* spriteRenderer, const Board& board);
inline void DrawPreview(graphics::SpriteRenderer* spriteRenderer, const Board& board);

bool GamePause = false;

std::vector<graphics::Texture2D*> textures(16);

int main(void)
{
    Board board(BoardWidth, BoardHeight);
    Orientation::Init(board);
    graphics::Renderer renderer(Orientation::ScreenWidth, Orientation::ScreenHeight, "Tetris v1", false);

    /* Game Scope */
    {
        graphics::Shader* shader = new graphics::Shader("Resources/Shaders/vertex_shader.shader", "Resources/Shaders/fragment_shader.shader");
        graphics::SpriteRenderer spriteRenderer(shader);

        graphics::Text textRenderer(renderer, "Resources/Fonts/OLD_SCHOOL.ttf", 27);

        graphics::SoundDevice soundDevice;
        // soundDevice.Play("Resources/Audio/TetrisTheme.mp3", true);

        /* Set window coordinates to left-top corner */
        shader->Use();
        math::mat4 projection = math::Ortho<float>(0.0f, (float)Orientation::ScreenWidth, (float)Orientation::ScreenHeight, 0.0f, -1.0f, 1.0f);
        shader->SetUniformMatrix4f("projection", projection);

        /* Load textures */
        textures[0] = new graphics::Texture2D(Pieces::GreyBorderTexturePath.c_str());

        for (unsigned char i = 1; i < 8; ++i)
        {
            std::string basic_path = Pieces::ColorTexture(i);
            textures[i] = new graphics::Texture2D(basic_path.c_str());

            std::string shadow_path = Pieces::ColorTexture(i + 8);
            textures[i + 8] = new graphics::Texture2D(shadow_path.c_str());
        }
        
        /* Setup game timers */
        graphics::Timer gameTimer;
        graphics::Timer inputTimer;

        /* Loop until the user closes the window */
        while (!renderer.WindowShouldClose())
        {
            unsigned short pressedKey = ProcessKeyboardInput(renderer.GetWindow());

            /* Clear screen and set color of background */
            renderer.ClearScreen({ 0.2f, 0.2f, 0.2f, 1.0f });

            /* Adjust delay between each reaction to keyboard input (uncontrollable behavior prevention) */
            if (inputTimer.DeltaTime() >= InputDelayMS / 1000.0f)
            {
                if (!(board.GameOver() || GamePause))
                    board.Update(pressedKey, true);

                inputTimer.UpdateTime();
            }

            /* Delay after each move */
            float delay = ((BaseGameDelayMS * (15 / (28 * (float)(board.Level()))) + 1625 / 7) / 1000.0f);
            if (gameTimer.DeltaTime() >= delay)
            {
                if (!(board.GameOver() || GamePause))
                    board.Update(1);                        // Move down

                gameTimer.UpdateTime();
            }

            DrawBoard(&spriteRenderer, board);
            DrawPreview(&spriteRenderer, board);
            DrawText(&textRenderer, board);

            renderer.ShowFPS();
            renderer.Update();
        }
    }

    /* Delete pointers */
    for (graphics::Texture2D* tex : textures)
        delete tex;

    /* Destroy window and clear memory */
    renderer.CloseWindow();

    return 0;
}

inline void DrawText(graphics::Text* textRenderer, const Board& board)
{
    float x = (board.Width + 3) * Orientation::BlockSize + Orientation::ScreenBorder;

    /* Score */
    float score_y = 30.0f;
    textRenderer->CreateText("Score:" + std::to_string(board.Score()), x, score_y, 1.0f, math::vec3(1.0f, 1.0f, 1.0f));

    /* Level */
    float level_y = 70.0f;
    textRenderer->CreateText("Level:" + std::to_string(board.Level()), x, level_y, 0.75f, math::vec3(1.0f, 1.0f, 1.0f));

    /* Preview description */
    float preview_y = Orientation::ScreenHeight - 3 * Orientation::ScreenBorder;
    textRenderer->CreateText("Next:", x, preview_y, 0.9f, math::vec3(1.0f, 1.0f, 1.0f));

    /* "Game Over" state */
    if (board.GameOver())
    {
        float y = Orientation::ScreenHeight / 2 - 20;
        float x = Orientation::ScreenWidth / 2 - textRenderer->TextLength("Game Over", 2.00f) / 2;
        textRenderer->CreateText("Game Over", x, y, 2.00f, math::vec3(0.8f, 0.0f, 0.0f));
    }
}

inline void DrawBoard(graphics::SpriteRenderer* spriteRenderer, const Board& board)
{
    math::vec2 blockSize = math::vec2(Orientation::BlockSize, Orientation::BlockSize);

    /* Draw whole board */
    for (unsigned int pos = 0; pos < board.Size(); ++pos)
    {
        math::vec2 board_coords = Orientation::CoordinatesFromPos(board, pos);

        /* Borders - Vertical */
        if (board_coords.x == 0 || board_coords.x == board.Width - 1)
        {
            math::vec2 border_coords = Orientation::BorderVScreenCoordsFromPos(board, pos);
            spriteRenderer->DrawSprite(textures[0], border_coords, blockSize);
        }

        /* Borders - Horizontal */
        if (board_coords.y == 0 || board_coords.y == board.Height - 1)
        {
            math::vec2 border_coords = Orientation::BorderHScreenCoordsFromPos(board, pos);
            spriteRenderer->DrawSprite(textures[0], border_coords, blockSize);
        }

        /* Pieces */
        if (board[pos])
        {
            math::vec2 coords = Orientation::PieceScreenCoordsFromPosition(board, pos);
            spriteRenderer->DrawSprite(textures[board[pos]], coords, blockSize);
        }
    }
    return;
}

/* Draw upcoming piece order (4 pieces) */
inline void DrawPreview(graphics::SpriteRenderer* spriteRenderer, const Board& board)
{
    math::vec2 previewBlockSize = math::vec2(Orientation::PreviewBlockSize, Orientation::PreviewBlockSize);

    PreviewBoard preview(board.PiecePreview());

    for (unsigned char i = 0; i < preview.length(); ++i)
    {
        math::vec2 coords = Orientation::PiecePreviewScreenCoordsFromPos(board, preview.piecePositions[i]);
        spriteRenderer->DrawSprite(textures[preview.pieceColors[i]], coords, previewBlockSize);
    }
    return;
}

/* Input processing */
unsigned short ProcessKeyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        return 1;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        return 2;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        return 3;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        return 4;

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        GamePause = !GamePause;

    return 0;
}