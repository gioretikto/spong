#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>                 // SDL sound library.
#include "ball.h"

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
	static constexpr int WIND_WIDTH = 1024;
	static constexpr int WIND_HEIGHT = 720;
private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
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
	
    Paddle left_paddle;
    Paddle right_paddle;
    
    Ball ball;
    
    // Controllers.
    enum Controllers {mouse, keyboard, joystick};
    Controllers controller;
    SDL_Joystick *gamepad;  // Holds joystick information.
    
    // Sounds.
    // Holds sound produced after ball collides with paddle.
    Mix_Chunk* paddle_sound;
    
    Mix_Chunk* score_sound;
    
    // Holds text indicating player 1 score (left).
    SDL_Texture* font_image_left_score;

    // Holds text indicating palyer 2 score (right).
    SDL_Texture* font_image_right_score;
    
    // Scores.
    int left_score;
    int right_score;
};
