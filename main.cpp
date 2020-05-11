#include "./Game/Game.h"

int main(){

	omp_set_num_threads(8);
	Game * game = new Game();
	game->run();

	delete game;

}