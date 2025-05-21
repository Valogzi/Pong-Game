#include <iostream>
#include <raylib.h>

using namespace std;

bool cancelled = false;
bool isGameEnding = false;
string winner;

class Game
{

public:
    void Ending()
    {
        string text = winner + " wins!";
        DrawText(text.c_str(), GetScreenWidth() / 2 - 100, GetScreenHeight() / 2, 20, WHITE);
        DrawText("Press ESC to exit", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 30, 20, WHITE);
    }
};

class Ball
{

public:
    int x;
    int y;
    int speed_x;
    int speed_y;
    int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update()
    {
        if (cancelled)
        {
            return;
        }

        x += speed_x;
        y += speed_y;
    }

    void Bounce()
    {
        if (x + radius >= GetScreenWidth() || x - radius <= 0)
        {
            // Game Over
            if (x + radius >= GetScreenWidth())
            {
                winner = "Left player";
                cancelled = true;
                isGameEnding = true;
            }
            else
            {
                winner = "Right player";
                cancelled = true;
                isGameEnding = true;
            }
        }

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1; // Change to -1 for full bounce
            // *=   <=>   ball_speed_x = ball_speed_x * -1;
        }
    }
};

class Paddle
{

public:
    int x;
    int y;
    int width;
    int height;

    void Draw()
    {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update(KeyboardKey up, KeyboardKey down)
    {
        if (cancelled)
        {
            return;
        }

        if (IsKeyDown(up) && y > 10)
        {
            y -= 5;
        }
        if (IsKeyDown(down) && y < GetScreenHeight() - height - 10)
        {
            y += 5;
        }
    }

    void Colision(Ball &ball)
    {
        if (ball.x + ball.radius >= x && ball.x - ball.radius <= x + width)
        {
            if (ball.y + ball.radius >= y && ball.y - ball.radius <= y + height)
            {
                ball.speed_x *= -1;
            }
        }
    }
};

Ball ball;
Paddle left_paddle;
Paddle right_paddle;
Game game;

int main()
{

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    cout << "Hello World" << endl;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My first RAYLIB program!");
    SetTargetFPS(60);

    ball.x = 100;
    ball.y = 100;
    ball.speed_x = 5;
    ball.speed_y = 5;
    ball.radius = 15;

    left_paddle.x = 10;
    left_paddle.y = SCREEN_HEIGHT / 2 - 60;
    left_paddle.width = 20;
    left_paddle.height = 120;

    right_paddle.x = SCREEN_WIDTH - 35;
    right_paddle.y = SCREEN_HEIGHT / 2 - 60;
    right_paddle.width = 20;
    right_paddle.height = 120;

    while (WindowShouldClose() == false)
    {

        ball.Update();
        ball.Bounce();

        left_paddle.Update(KEY_W, KEY_S);
        right_paddle.Update(KEY_UP, KEY_DOWN);

        left_paddle.Colision(ball);
        right_paddle.Colision(ball);

        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(SCREEN_WIDTH / 2, 0, 5, SCREEN_HEIGHT, GRAY); // Draw the center line

        ball.Draw();
        left_paddle.Draw();
        right_paddle.Draw();

        if (isGameEnding)
        {
            game.Ending();
        }

        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE))
        {
            break;
        }
        // Check for game over
    }

    CloseWindow(); // Close window and OpenGL context
}