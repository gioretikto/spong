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
		x <= (paddle.x + paddle.WIDTH) && x >= paddle.x &&
		// The ball is moving to the left
		vel_x < 0.0f)
	{
		vel_x *= -1.0f;
		return true;
	}
	
	else
		return false;
}
