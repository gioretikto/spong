#include "game.h"
#include <SDL2/SDL_ttf.h>                   // SDL font library

Game::Game()
:window(nullptr)
,renderer(nullptr)
,ticksCount(0)
,isRunning(true)
,isPaused(true)
,controller(keyboard)
,left_score_changed(false)
,right_score_changed(false)
,font_image_left_score(nullptr)
,font_image_right_score(nullptr)
{
	
}

bool Game::Initialize(int argc, char *argv[])
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
    
    ball.paddle_sound = Mix_LoadWAV("resources/sounds/ball_paddle_hit.wav");
    
    // Load score sound
    score_sound = Mix_LoadWAV("resources/sounds/score.wav");
	
	// Create SDL renderer
	renderer = SDL_CreateRenderer(
		window, // Window to create renderer for
		-1,		// Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!renderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	
	if (TTF_Init() == -1) {  // Initialize font
	    std::cout << "TTF_Init: " << TTF_GetError();
		return false;
	}
	
	font_image_left_score = renderText (std::to_string(left_paddle.score));
    SDL_RenderCopy(renderer, font_image_left_score, NULL, &left_paddle.Message_rect);
    
    font_image_right_score = renderText (std::to_string(right_paddle.score));
    SDL_RenderCopy(renderer, font_image_right_score, NULL, &right_paddle.Message_rect);
	
	// Controllers
    if (argc == 2) {
    
        if (strcmp(argv[1], "mouse") == 0 ) {
            controller = mouse;
        }
        
    	if (strcmp(argv[1], "keyboard") == 0 ) {
            controller = keyboard;
        }
    }
    
	reset();

	return true;
}

void Game::RunLoop()
{
	while (isRunning)
	{
		ProcessInput();
		
		if (isPaused != true)
			UpdateGame();
			
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_MOUSEMOTION) {
	        SDL_GetMouseState(&mouse_x, &mouse_y);
        }
        
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
				
			 // Pressing F11 toggles fullscreen
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
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16))
		;

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
	if (ball.x <= 0.0f) {
		right_paddle.score++;
		right_score_changed = true;
		std::cout<<right_paddle.score<<std::endl;
		reset();		
	}
	
	else if (ball.x >= float(WIND_WIDTH))
	{
		left_paddle.score++;
        left_score_changed = true;
		reset();
	}
	
	else {	
	
		if (ball.vel_x < 0) {
		
			if (ball.collides_with(left_paddle)) {
				;
			}
		}
		
		else {
			if (ball.collides_with(right_paddle)) {
				;
			}
		}
		
		// Did the ball collide with the right wall?
		if (ball.x >= (float(WIND_WIDTH) - wallThickness) && ball.vel_x > 0.0f)
		{
			ball.vel_x *= -1.0f;
		}
		
		// Did the ball collide with the top wall?
		if (ball.y <= wallThickness && ball.vel_y < 0.0f)
		{
			ball.vel_y *= -1;
		}
		
		// Did the ball collide with the bottom wall?
		if (ball.y >= (float(WIND_HEIGHT) - wallThickness) &&
			ball.vel_y > 0.0f)
		{
			ball.vel_y *= -1;
		}
		
		/* AI move for CPU paddle */
  		if (ball.x > WIND_WIDTH * 3/5 && ball.vel_x > 0) {
		  
			if (ball.y > right_paddle.y + right_paddle.HEIGHT/2) {	// If the ball is below the center of the paddle
		  		right_paddle.y = right_paddle.y + ball.vel_x/40;    // Move downwards
		  	}
				
		  	else {  						// If the ball is above the center of the paddle
		  		right_paddle.y = right_paddle.y - ball.vel_x/40;     // Move upwards
		  	}
  		}
  	} /* End of else */
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
    if (left_score_changed) {
    
    	font_image_left_score = renderText (std::to_string(left_paddle.score));
        left_score_changed = false;
        
    }
    
   	SDL_RenderCopy(renderer, font_image_left_score, NULL, &left_paddle.Message_rect);

    if (right_score_changed) {
    
        font_image_right_score = renderText (std::to_string(right_paddle.score));
        right_score_changed = false;
        
    }
	
	SDL_RenderCopy(renderer, font_image_right_score, NULL, &right_paddle.Message_rect);
	
	// Swap front buffer and back buffer
	SDL_RenderPresent(renderer);
}

// Reset game to initial state
void Game::reset() {

	Mix_PlayChannel(-1, score_sound, 0);
	
	isPaused = true;

    ball.x = WIND_WIDTH/2 - ball.DIAMETER/2;
    ball.y = WIND_HEIGHT/2;
    
    ball.vel_x = -415.0f;
	ball.vel_y = 415.0f;
    
	left_paddle.x = 0.0 + left_paddle.WIDTH;
	left_paddle.y = float(WIND_HEIGHT)/2.0f;
	
	right_paddle.x = float(WIND_WIDTH) - 2 * right_paddle.WIDTH;
	right_paddle.y = float(WIND_HEIGHT)/2.0f;
	
}

SDL_Texture* Game::renderText(const std::string& message) {

	static TTF_Font* font = nullptr;
	
	if (!font) {
		
		font = TTF_OpenFont("resources/fonts/NES-Chimera.ttf", 24); //this opens a font style and sets a size
	
	}
	
	if (font == nullptr ) {
		std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
		return nullptr;
	}
	
	SDL_Color White = {255, 255, 255, 255};  // this is the color in rgb format

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, message.c_str(), White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	if (!surfaceMessage) {
		SDL_Log("Failed to create surfaceMessage: %s", SDL_GetError());
		return nullptr;
	}

	SDL_Texture* Message = SDL_CreateTextureFromSurface(Game::renderer, surfaceMessage); //now you can convert it into a texture
	
	if (Message == nullptr) {
		SDL_Log("Failed to create Texture Message: %s", SDL_GetError());
		return nullptr;
	}		

	//Don't forget to free your surface
	SDL_FreeSurface(surfaceMessage);
	
	return Message;
}

void Game::Shutdown()
{     
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	// Destroy textures
    SDL_DestroyTexture(font_image_left_score);
    SDL_DestroyTexture(font_image_right_score);
	
	// Quit SDL_mixer
    Mix_CloseAudio();
    
    Mix_FreeChunk(ball.paddle_sound);
    Mix_FreeChunk(score_sound);
	SDL_Quit();
}
