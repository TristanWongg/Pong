#include <iostream>
#include "raylib.h"

using namespace std;

int player_score = 0, cpu_score = 0;

class Ball
{
public:
    float x, y;
    int speed_x, speed_y, radius;
    
    void Draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }
    
    void Update()
    {
        x += speed_x;
        y += speed_y;
        
        if (x + radius >= GetScreenWidth())
        {
            speed_x *= -1;
            player_score++;
            ResetBall();
        }
        
        if (x - radius <= 0)
        {
            speed_x *= -1;
            cpu_score++;
            ResetBall();
        }
        
        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
    }
    
    void ResetBall()
    {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;
    
        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
    
};

class Paddle
{
protected:
    void LimitMovement()
    {
        if (y <= 0)
        {
            y = 0;
        }
        
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }
    
public:
    float x, y, width, height;
    int speed;
    
    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 1, 0, WHITE);
    }
    
    void Update()
    {
        if (IsKeyDown(KEY_UP))
        {
            y -= speed;
        }
        
        if (IsKeyDown(KEY_DOWN))
        {
            y += speed;
        }
        
        LimitMovement();
    }
};

class CpuPaddle: public Paddle
{
public:
    void Update(int ball_y)
    {
        if (y + height/2 > ball_y)
        {
            y -= speed;
        }
        
        if (y + height/2 <= ball_y)
        {
            y += speed;
        }
        
        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main(void)
{
    const int screen_width = 1080;
    const int screen_height = 680;
    InitWindow(screen_width, screen_height, "Pong!");
    SetTargetFPS(60);
    
    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;
    
    player.width = 25;
    player.height = 120;
    player.x = 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 5;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = screen_width - cpu.width - 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 5;
    
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        
        if (player_score !=3 && cpu_score != 3)
        {
            //Updating
            ball.Update();
            player.Update();
            cpu.Update(ball.y);
        }
        
        else if (player_score == 3)
        {
            DrawText("Player Wins!", screen_width/2 - MeasureText("Player Wins!", 80)/2, screen_height/4, 80, GOLD);
            DrawText("Press Space to Play Again", screen_width/2 - MeasureText("Press Space to Play Again", 40)/2, 3 * screen_height/4, 40, GOLD);
            if (IsKeyDown(KEY_SPACE))
            {
                player_score = cpu_score = 0;
            }
        }
        else
        {
            DrawText("CPU Wins!", screen_width/2 - MeasureText("CPU Wins!", 80)/2, screen_height/4, 80, GOLD);
            DrawText("Press Space to Play Again", screen_width/2 - MeasureText("Press Space to Play Again", 40)/2, 3 * screen_height/4, 40, GOLD);
            if (IsKeyDown(KEY_SPACE))
            {
                player_score = cpu_score = 0;
            }
        }
        
        //Check for Collisions
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}) || CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            ball.speed_x *= -1;
        }
        
        //Drawing
        ClearBackground(DARKGREEN);
        DrawCircleLines(screen_width/2, screen_height/2, 80, WHITE);
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", player_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", cpu_score), 3 * screen_width/4 - 20, 20, 80, WHITE);
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
