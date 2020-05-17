class Paddle {
 public:
    const SDL_Rect Message_rect;  //a rect to hold scores	
    
	// Direction of paddle
	int direction = 0;
	
	int score = 0;
	
	// Paddle position
    float x = 0;
    float y = 0;
	
	// Paddle dimensions
    static constexpr float HEIGHT = 95.0f;
    static constexpr float WIDTH = 8.0f;
};
