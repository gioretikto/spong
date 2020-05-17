#include "ball.h"

bool Ball::collides_with(const Paddle& paddle) {

	// Did the ball intersect with the paddle? Bounce if needed
	
	float diff = paddle.y - y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	
	if (// Our y-difference is small enough
		diff <= paddle.HEIGHT / 1.5f &&
		// We are in the correct x-position
		x <= (paddle.x + paddle.WIDTH) && x >= paddle.x)
	{
		vel_x *= -1.0f;
		
		/*Increase speed of a 100th */
		vel_x += vel_x/60;
		vel_y += vel_y/60;
		
		Mix_PlayChannel(-1, paddle_sound, 0);  // Play collision sound
		return true;
	}
	
	else
		return false;
}
