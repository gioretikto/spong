class Paddle {
 public:
 	Paddle();
	// Paddle position
    float x;
    float y;
    
	// Direction of paddle
	int direction;
	
	int score;
	
	// Paddle dimensions
    static constexpr float HEIGHT = 95.0f;
    static constexpr float WIDTH = 8.0f;
    
    SDL_Rect Message_rect;  //a rect to hold scores	
};
