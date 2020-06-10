#include "spong.h"

Game::Game()
:window(nullptr)
,renderer(nullptr)
,ticksCount(0)
,isRunning(true)
,isPaused(true)
,number_collisions(0)
,score_sound(nullptr)
,left_score_update(false)
,right_score_update(false)
,font(nullptr)
{
	
}

bool Game::Initialize()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL :%s", SDL_GetError());
		return false;
	}
	
    // Don't show cursor
    SDL_ShowCursor(0);
	
	// Create an SDL Window
	window = SDL_CreateWindow(
		"spong", // Window title
        SDL_WINDOWPOS_UNDEFINED,  // Centered window
        SDL_WINDOWPOS_UNDEFINED,  // Centered window
		WIND_WIDTH,				 // Width of window
		WIND_HEIGHT,			// Height of window
	    SDL_WINDOW_SHOWN
	);

	if (!window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	// Sounds
	// Initialize SDL_mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024)) {    
	    std::cout << "SDL_mixer: Msx_OpenAudio " << Mix_GetError() << std::endl;
        return false;
    }
    
    ball.paddle_sound = Mix_LoadWAV("resources/audio/ball_paddle_hit.ogg");
    
    // Load score sound
    score_sound = Mix_LoadWAV("resources/audio/score.ogg");
	
	// Create SDL renderer
	renderer = SDL_CreateRenderer(window,-1, // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!renderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	
	if (TTF_Init() == -1) {  // Initialize font
	    std::cout << "TTF_Init: " << TTF_GetError();
		return false;
	}
	
	else {
		font = TTF_OpenFont("resources/fonts/unifont.ttf", 24); //this opens a font style and sets a size
		font_color = {255, 255, 255, 255};  // this is the white color in rgb format
	}
	
	/* Initialize scores messages*/
	
	textRender (left_paddle);

    textRender (right_paddle);
        	
	// Get state of keyboard
	state = SDL_GetKeyboardState(NULL);
	
	reset();

	return true;
}

void Game::RunLoop()
{
	while (isRunning)
	{
		ProcessInput();
		
		if (isPaused != true) {
			SDL_Delay(16); 	// Wait until 16ms has elapsed since last frame
			UpdateGame();	
		}
			
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	
	while (SDL_PollEvent(&event))
	{
		// Track mouse movement
		if (event.type == SDL_MOUSEMOTION) {
	        SDL_GetMouseState(&mouse_x, &mouse_y);
        }
        
		if (event.type == SDL_QUIT) {        // If we get an SDL_QUIT event, end loop
			isRunning = false;
		}
        
        if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
                    isRunning = false;
                    break;
                    				
				case SDLK_RETURN:
				// If return is pressed, pause the game
					if (isPaused == false)
						{
							isPaused = true;
						}
					
						else
							isPaused = false;
					
					break;
					
				 // Pressing F11 toggles fullscreen
		         case SDLK_F11:
		         
		        	 int flags = SDL_GetWindowFlags(window);
		        	 
		             if (flags & SDL_WINDOW_FULLSCREEN) {
		        	     if (SDL_SetWindowFullscreen(window, 0) != 0)
		        	     	  std::cout << "Unable to switch window to fullscreen mode :" << SDL_GetError() <<std::endl;
		             }
		             
		             else
		             {
		        	 	if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
		        	 	  	std::cout << "Unable to switch window to fullscreen mode :" << SDL_GetError() <<std::endl;
		        	 }
		        	 
		             break;
			}
		} /* end of if (event.type == SDL_KEYDOWN) */
	}
		
	// Update paddle direction based on Up/Down arrow keys
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
	if (controller == mouse) {
        left_paddle.y = mouse_y;
    }
    
	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - ticksCount) / 1000.0f;
	
	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
		deltaTime = 0.05f;

	// Update tick counts (for next frame)
	ticksCount = SDL_GetTicks();
	
	// Update paddle position based on direction
	if (left_paddle.direction != 0)
	{
		left_paddle.y += left_paddle.direction * 500.0f * deltaTime;
		
		// Make sure paddle doesn't move off screen!
		if (left_paddle.y < (Paddle::HEIGHT/2.0f + wallThickness))
		{
			left_paddle.y = Paddle::HEIGHT/2.0f + wallThickness;
		}
		
		if (left_paddle.y > (float(WIND_HEIGHT) - Paddle::HEIGHT/2.0f - wallThickness))
		{
			left_paddle.y = float(WIND_HEIGHT) - Paddle::HEIGHT/2.0f - wallThickness;
		}
	}
	
	// Update ball position based on ball velocity
	ball.x += ball.vel_x * deltaTime;
	ball.y += ball.vel_y * deltaTime;
	
	// Did the ball go off the screen? (if so, end game)
	if (ball.x <= 0.0f)
	{
		right_paddle.score++;
		right_score_update = true;
		reset();		
	}
	
	else if (ball.x >= float(WIND_WIDTH))
	{
		left_paddle.score++;
        left_score_update = true;
		reset();
	}
	
	else {	
	
		if (ball.vel_x < 0) {
		
			if (ball.collides_with(left_paddle)) {
				number_collisions++;
				right_paddle.move_status = true;
			}
		}
		
		else {
			if (ball.collides_with(right_paddle)) {
				number_collisions++;
				right_paddle.move_status = false;
			}
		}
		
		// Did the ball collide with the top wall?
		if (ball.y <= wallThickness && ball.vel_y < 0.0f)
		{
			ball.vel_y *= -1;
		}
		
		// Did the ball collide with the bottom wall?
		if (ball.y >= (float(WIND_HEIGHT) - wallThickness) && ball.vel_y > 0.0f)
		{
			ball.vel_y *= -1;
		}
  		
  		if (ball.x > float(WIND_WIDTH)*3/5 && right_paddle.move_status) {
  		
	  		if (ball.y > right_paddle.y + right_paddle.HEIGHT/2.0f) {  // If the ball is below the center of the paddle
				right_paddle.y = right_paddle.y + ball.vel_x/(40-number_collisions/180);       // Move downwards
	  		}
	  		
	  		else if (ball.y < right_paddle.y + right_paddle.HEIGHT/2.0f) {  				// If the ball is above the center of the paddle
				right_paddle.y  = right_paddle.y - ball.vel_x/(40-number_collisions/180);   // Move upwards
			}	
		}
 		
  	} /* End of else */
}

void Game::GenerateOutput()
{
	// Set draw color to white used for drawing operations (Rect, Line and Clear)
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
		0,				// Top left x
		0,				// Top left y
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
		static_cast<int>(left_paddle.y - left_paddle.HEIGHT/2),
		static_cast<int>(left_paddle.WIDTH),
		static_cast<int>(left_paddle.HEIGHT)
	};
	SDL_RenderFillRect(renderer, &paddle);	
	
	// Draw CPU paddle
	SDL_Rect paddle_cpu{
		static_cast<int>(right_paddle.x),
		static_cast<int>(right_paddle.y - right_paddle.HEIGHT/2),
		static_cast<int>(right_paddle.WIDTH),
		static_cast<int>(right_paddle.HEIGHT)
	};
	SDL_RenderFillRect(renderer, &paddle_cpu);
	
	// Draw ball
	SDL_Rect spong_ball {static_cast<int>(ball.x), static_cast<int>(ball.y), ball.DIAMETER, ball.DIAMETER};
	SDL_RenderFillRect(renderer, &spong_ball);
	
    // Render scores
    if (left_score_update)
    {
       	textRender (left_paddle);
        left_score_update = false;
    }
    
   SDL_RenderCopy(renderer, left_paddle.Message, NULL, &left_paddle.Message_rect);
    
    if (right_score_update) {
    
        textRender (right_paddle);   
        right_score_update = false;
        
    }
    
   SDL_RenderCopy(renderer, right_paddle.Message, NULL, &right_paddle.Message_rect);
	
	// Swap front buffer and back buffer
	SDL_RenderPresent(renderer);
}

// Reset game to initial state
void Game::reset() {

	Mix_PlayChannel(-1, score_sound, 0);
	
	isPaused = true;

    ball.x = WIND_WIDTH/2 - ball.DIAMETER/2;
    ball.y = WIND_HEIGHT/2;
    
    ball.vel_x = -416.0f;
	ball.vel_y = 416.0f;
    
	left_paddle.x = 0.0 + left_paddle.WIDTH;
	left_paddle.y = float(WIND_HEIGHT)/2.0f;
	
	right_paddle.x = float(WIND_WIDTH) - 2 * right_paddle.WIDTH;
	right_paddle.y = float(WIND_HEIGHT)/2.0f;
	
	number_collisions = 0;	
}

void Game::textRender(Paddle& paddle) {

	std::string message = std::to_string(paddle.score);
	
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, message.c_str(), font_color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	if (!surfaceMessage) {
		SDL_Log("Failed to create surfaceMessage: %s", SDL_GetError());
	}

	if (paddle.Message != nullptr) {
		SDL_DestroyTexture(paddle.Message);
		paddle.Message = nullptr;
	}

	paddle.Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
	
	if (paddle.Message == nullptr) {
		SDL_Log("Failed to create Texture Message: %s", SDL_GetError());
	}		

	//Don't forget to free your surface
	SDL_FreeSurface(surfaceMessage);
}

Game::~Game()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
    TTF_CloseFont(font);
    TTF_Quit();
	
	// Quit SDL_mixer
    Mix_CloseAudio();

    Mix_FreeChunk(score_sound);
	SDL_Quit();
}

Paddle::~Paddle()
{
	SDL_DestroyTexture(Message);
}
