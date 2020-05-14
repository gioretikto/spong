#include <SDL2/SDL.h>
#include "paddle.h"

// Vector2 struct just stores x/y coordinates
// (for now)
struct Vector
{
	float x;
	float y;
};

class Paddle;

// Game class
class Game
{
public:
	Game();
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();
private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

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
	
    // Holds text indicating player 1 score (left).
    SDL_Texture* font_image_left_score;

    // Holds text indicating palyer 2 score (right).
    SDL_Texture* font_image_right_score;
    
    // Scores.
    int left_score;
    int right_score;
	
	// Position of ball
	Vector ballPos;
	// Velocity of ball
	Vector ballVel;
};
