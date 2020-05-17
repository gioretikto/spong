#pragma once
#include <SDL2/SDL_mixer.h>                 // SDL sound library
#include "paddle.h"

class Ball {
public:
 	float x;
 	float y;
 	
 	float vel_x;
 	float vel_y;
 	
    float speed;    
    
 	// Ball dimensions
    static constexpr int DIAMETER = 10;
    
    // Holds sound produced after ball collides with paddle.
    Mix_Chunk* paddle_sound;
    
 	bool collides_with(const Paddle& paddle);
};
