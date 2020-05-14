#include "game.h"

const int wallThickness = 15;
const int WIND_WIDTH = 1024;
const int WIND_HEIGHT = 720;

Game::Game()
:window(nullptr)
,renderer(nullptr)
,ticksCount(0)
,isRunning(true)
,isPaused(false)
{
	
}

Paddle::Paddle()
:direction(0)
{

}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
    // Don't show cursor.
    SDL_ShowCursor(0);
	
	// Create an SDL Window
	window = SDL_CreateWindow(
		"SPong", // Window title
        SDL_WINDOWPOS_UNDEFINED,  // Centered window.
        SDL_WINDOWPOS_UNDEFINED,  // Centered window.
		WIND_WIDTH,				 // Width of window
		WIND_HEIGHT,			// Height of window
	    SDL_WINDOW_SHOWN
	);

	if (!window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	//// Create SDL renderer
	renderer = SDL_CreateRenderer(
		window, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!renderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	left_score = 0;
    right_score = 0;

	left_paddle.x = 0.0 + float(wallThickness);
	left_paddle.y = float(WIND_HEIGHT)/2.0f;
	
	right_paddle.x = float(WIND_WIDTH) - float(wallThickness);
	right_paddle.y = float(WIND_HEIGHT)/2.0f;
	
	ballPos.x = float(WIND_WIDTH)/2.0f;
	ballPos.y = float(WIND_HEIGHT)/2.0f;
	ballVel.x = -415.0f;
	ballVel.y = 415.0f;
	return true;
}

void Game::RunLoop()
{
	while (isRunning)
	{
		ProcessInput();
		
		if (isPaused == true)
			UpdateGame();
			
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				isRunning = false;
				break;
				
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					// If return is pressed, pause the game
					if(isPaused == false)
					{
						isPaused = true;
					}
				
					else
						isPaused = false;
				}
				break;
				
			 // Pressing F11 toggles fullscreen.
             case SDLK_F11:             
            	 int flags = SDL_GetWindowFlags(window);
            	 
                 if (flags & SDL_WINDOW_FULLSCREEN) {
            	     SDL_SetWindowFullscreen(window, 0);
                 } else {
            	       SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            	   }
                 break;
		}
	}
	
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
		
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		isRunning = false;
	}
	
	// Update paddle direction based on W/S keys
	left_paddle.direction = 0;
	
	if (state[SDL_SCANCODE_UP])
	{
		left_paddle.direction -= 1;
	}
	
	if (state[SDL_SCANCODE_DOWN])
	{
		left_paddle.direction += 1;
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - ticksCount) / 1000.0f;
	
	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	ticksCount = SDL_GetTicks();
	
	// Update paddle position based on direction
	if (left_paddle.direction != 0)
	{
		left_paddle.y += left_paddle.direction * 500.0f * deltaTime;
		
		// Make sure paddle doesn't move off screen!
		if (left_paddle.y < (paddleHeight/2.0f + wallThickness))
		{
			left_paddle.y = paddleHeight/2.0f + wallThickness;
		}
		
		else if (left_paddle.y > (float(WIND_HEIGHT) - paddleHeight/2.0f - wallThickness))
		{
			left_paddle.y = float(WIND_HEIGHT) - paddleHeight/2.0f - wallThickness;
		}
	}
	
	// Update ball position based on ball velocity
	ballPos.x += ballVel.x * deltaTime;
	ballPos.y += ballVel.y * deltaTime;
	
	// Bounce if needed
	// Did we intersect with the paddle?
	float diff = left_paddle.y - ballPos.y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	
	if (
		// Our y-difference is small enough
		diff <= paddleHeight / 2.0f &&
		// We are in the correct x-position
		ballPos.x <= 25.0f && ballPos.x >= 20.0f &&
		// The ball is moving to the left
		ballVel.x < 0.0f)
	{
		ballVel.x *= -1.0f;
	}
	// Did the ball go off the screen? (if so, end game)
	else if (ballPos.x <= 0.0f)
	{
		isRunning = false;
	}
	// Did the ball collide with the right wall?
	else if (ballPos.x >= (float(WIND_WIDTH) - wallThickness) && ballVel.x > 0.0f)
	{
		ballVel.x *= -1.0f;
	}
	
	// Did the ball collide with the top wall?
	if (ballPos.y <= wallThickness && ballVel.y < 0.0f)
	{
		ballVel.y *= -1;
	}
	// Did the ball collide with the bottom wall?
	else if (ballPos.y >= (WIND_HEIGHT - wallThickness) && ballVel.y > 0.0f)
	{
		ballVel.y *= -1;
	}
}

void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		renderer,
		0,		// R
		0,		// G 
		0,		// B
		0		// A
	);
	
	// Clear back buffer
	SDL_RenderClear(renderer);

	// Draw walls
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		WIND_WIDTH,		// Width
		wallThickness	// Height
	};
	SDL_RenderFillRect(renderer, &wall);
	
	// Draw middle line
  	SDL_RenderDrawLine(renderer, WIND_WIDTH/2, 0, WIND_WIDTH/2, WIND_HEIGHT); 
	
	// Draw bottom wall
	wall.y = WIND_HEIGHT - wallThickness;
	SDL_RenderFillRect(renderer, &wall);
	
	// Draw Player paddle
	SDL_Rect paddle{
		static_cast<int>(left_paddle.x),
		static_cast<int>(left_paddle.y - paddleHeight/2),
		wallThickness,
		static_cast<int>(paddleHeight)
	};
	SDL_RenderFillRect(renderer, &paddle);	
	
	// Draw CPU paddle
	SDL_Rect paddle_cpu{
		static_cast<int>(right_paddle.x),
		static_cast<int>(right_paddle.y - paddleHeight/2),
		wallThickness,
		static_cast<int>(paddleHeight)
	};
	SDL_RenderFillRect(renderer, &paddle_cpu);
	
	// Draw ball
	SDL_Rect ball{
		static_cast<int>(ballPos.x - wallThickness/2),
		static_cast<int>(ballPos.y - wallThickness/2),
		wallThickness,
		wallThickness
	};
	SDL_RenderFillRect(renderer, &ball);
	
	// Swap front buffer and back buffer
	SDL_RenderPresent(renderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}