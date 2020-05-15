class Paddle {
 public:
 	Paddle();
	// Paddle position
    float x;
    float y;
    
	// Direction of paddle
	int direction;
	
	// Paddle dimensions
    static const float HEIGHT = 100.0f;
    static const float WIDTH = 8.0f;
    static const float MARGIN = 10.0f;
};
