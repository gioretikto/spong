#include "game.h"

int main(int argc, char* argv[])
{
	Game game;
	
	bool success = game.Initialize(argc,argv);
	
	if (success)
	{
		game.RunLoop();
	}
	
	game.Shutdown();
	
	return 0;
}
