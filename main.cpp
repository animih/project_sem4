#include "./Game/Game.h"

int main(){

	Game * game = new Game();
	game->run();

	delete game;

}