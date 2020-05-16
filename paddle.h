class Paddle {
 public:
 	Paddle();
	// Paddle position
    float x;
    float y;
    
	// Direction of paddle
	int direction;
	
	// Paddle dimensions
    static constexpr float HEIGHT = 100.0f;
    static constexpr float WIDTH = 8.0f;
    static constexpr float MARGIN = 10.0f;
};
