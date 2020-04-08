#include "./Game/Game.h"

// вот так выглядит main

int main(){

	Game * gage = new Game();
	gage->run();

	delete gage;

}