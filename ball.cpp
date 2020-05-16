#include "ball.h"
#include "game.h"

bool Ball::collides_with(const Paddle& paddle) {

	// Did the ball intersect with the paddle? Bounce if needed
	
	float diff = paddle.y - y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// Our y-difference is small enough
		diff <= paddle.HEIGHT / 2.0f &&
		// We are in the correct x-position
		x <= (paddle.x + paddle.WIDTH) && x >= paddle.x)
	{
		vel_x *= -1.0f;
		return true;
	}
	
	else
		return false;
}

// Reset ball to initial state.
void Ball::reset(Paddle& left_paddle, Paddle& right_paddle) {
    x = Game::WIND_WIDTH/2 - DIAMETER/2;
    y = Game::WIND_HEIGHT/2;
    
	left_paddle.x = 0.0 + left_paddle.MARGIN;
	left_paddle.y = float(Game::WIND_HEIGHT)/2.0f;
	
	right_paddle.x = float(Game::WIND_WIDTH) - right_paddle.MARGIN;
	right_paddle.y = float(Game::WIND_HEIGHT)/2.0f;   
}
