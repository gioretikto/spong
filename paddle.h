#include <SDL2/SDL.h>

class Paddle {
 public:
	 ~Paddle();
	 
    const SDL_Rect Message_rect;  //a rect to hold font scores	
    
	// Direction of paddle
	int direction = 0;
	
	int score = 0;
	
	// Paddle position
    float x = 0;
    float y = 0;
    
    bool move_status = true;
	
	// Paddle dimensions
    static constexpr float HEIGHT = 95.0f;
    static constexpr float WIDTH = 8.0f;
    
    SDL_Texture* Message = nullptr;   /* To hold Player's score */
    
};
