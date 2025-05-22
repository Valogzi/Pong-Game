#include <iostream>
#include <raylib.h>

using namespace std;

bool newRound = false;
int player_score[2] = {0, 0};

int random_vector = GetRandomValue(4, 6);

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int ball_data[5] = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 5, 5, 15};
int left_paddle_data[4] = {
    10,
    SCREEN_HEIGHT / 2 - 60,
    20,
    120};
int right_paddle_data[4] = {
    SCREEN_WIDTH - 35,
    SCREEN_HEIGHT / 2 - 60,
    20,
    120};

class Game
{

public:
    void IncreaseScore(int player)
    {
        player_score[player]++;
    }

    void DrawScore()
    {
        DrawText(TextFormat("%i", player_score[0]), SCREEN_WIDTH / 2 - SCREEN_WIDTH / 4, 15, 40, WHITE);
        DrawText(TextFormat("%i", player_score[1]), SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4, 15, 40, WHITE);
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

    Ball()
    {

        random_vector = GetRandomValue(4, 10);

        int ball_vector[2] = {
            (GetRandomValue(0, 1) == 0) ? -random_vector : random_vector,
            (GetRandomValue(0, 1) == 0) ? -random_vector : random_vector};

        x = ball_data[0];
        y = ball_data[1];
        speed_x = ball_vector[0];
        speed_y = ball_vector[1];
        radius = ball_data[4];
    }

    void Draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update()
    {
        x += speed_x;
        y += speed_y;
    }

    void Bounce(Game &game)
    {
        if (x + radius >= SCREEN_WIDTH || x - radius <= 0)
        {
            // Game Over
            if (x + radius >= SCREEN_WIDTH)
            {
                game.IncreaseScore(0);
                newRound = true;
            }
            else
            {
                game.IncreaseScore(1);
                newRound = true;
            }
        }

        if (y + radius >= SCREEN_HEIGHT || y - radius <= 0)
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

    Paddle(int left_or_right)
    {
        if (left_or_right == 0)
        {
            x = left_paddle_data[0];
            y = left_paddle_data[1];
            width = left_paddle_data[2];
            height = left_paddle_data[3];
        }
        else
        {
            x = right_paddle_data[0];
            y = right_paddle_data[1];
            width = right_paddle_data[2];
            height = right_paddle_data[3];
        }
    }

    void Draw()
    {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update(KeyboardKey up, KeyboardKey down)
    {
        if (IsKeyDown(up) && y > 10)
        {
            y -= random_vector;
        }
        if (IsKeyDown(down) && y < GetScreenHeight() - height - 10)
        {
            y += random_vector;
        }
    }

    void Colision(Ball &ball)
    {

        if (ball.x + ball.radius >= x && ball.x - ball.radius <= x + width)
        {
            if (ball.y + ball.radius >= y && ball.y - ball.radius <= y + height)
            {
                ball.speed_x *= -1;

                float topZone = y + height * 0.05f;
                float bottomZone = y + height * 0.95f;

                if (ball.y < topZone)
                {
                    // 5% haut
                    ball.speed_y = -abs(random_vector);
                }
                else if (ball.y > bottomZone)
                {
                    // 5% bas
                    ball.speed_y = abs(random_vector);
                }
                else
                {
                    // 90% milieu
                    // Optionnel : garder la même vitesse ou la réduire
                    // ball.speed_y = 0; // ou rien
                }
            }
        }
    }
};

Game game;

int main()
{

    cout << "Hello World" << endl;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My first RAYLIB program!");
    SetTargetFPS(60);

    Ball ball = Ball();
    Paddle left_paddle = Paddle(0);
    Paddle right_paddle = Paddle(1);

    while (WindowShouldClose() == false)
    {

        if (newRound)
        {
            ball = Ball();
            left_paddle = Paddle(0);
            right_paddle = Paddle(1);

            newRound = false;
        }

        ball.Update();
        ball.Bounce(game);

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

        game.DrawScore();

        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE))
        {
            break;
        }
        // Check for game over
    }

    CloseWindow(); // Close window and OpenGL context
}