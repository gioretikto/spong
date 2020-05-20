#include "spong.h"

int main(int argc, char* argv[])
{
	Game game;
	
	// Controllers
    if (argc == 2) {
    
        if (strcmp(argv[1], "mouse") == 0 ) {
            game.controller = mouse;
        }        
    }
    
    else
        	game.controller = keyboard;
	
	if (game.Initialize())
	{
		game.RunLoop();
	}

	return 0;
}
