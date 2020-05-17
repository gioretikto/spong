#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include "ball.h"

void renderText(const std::string& message, SDL_Renderer *renderer, SDL_Texture* Message, int x, int y);

class Paddle;
class Ball;

// Game class
class Game
{
public:
	Game();
	// Initialize the game
	bool Initialize(int argc, char *argv[]);
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();
	
	static constexpr int wallThickness = 15;
	static const int WIND_WIDTH = 1024;
	static const int WIND_HEIGHT = 720;
private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	SDL_Texture* renderText(const std::string& message);
 	void reset();

	// Window created by SDL
	SDL_Window* window;
	// Renderer for 2D drawing
	SDL_Renderer* renderer;
	// Number of ticks since start of game
	
	Uint32 ticksCount;
	// Game should continue to run
	bool isRunning;
	
	bool isPaused;
	
    Paddle left_paddle = {SDL_Rect{WIND_WIDTH * 4 / 10, WIND_HEIGHT / 12, 30, 30}};
    Paddle right_paddle = {SDL_Rect{WIND_WIDTH * 6 / 10 - 12, WIND_HEIGHT/ 12, 30, 30}};
    
    Ball ball;
    
    // Controllers.
    enum Controllers {mouse, keyboard};
    Controllers controller;
    
    int mouse_x, mouse_y;  // Mouse coordinates.
    
    // Sounds
        
    Mix_Chunk* score_sound;
    
    // Indicates when rendering new score is necessary
    bool left_score_changed;

    // Indicates when rendering new score is necessary
    bool right_score_changed;
    
    // Scores
    int left_score;
    int right_score;
    
    // Holds text indicating player score (left)
    SDL_Texture* font_image_left_score;

    // Holds text indicating CPU score (right)
    SDL_Texture* font_image_right_score;
};
