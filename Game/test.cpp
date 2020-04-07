#include "./Game/Game.h"

int main(){

	Game * gage = new Game();
	gage->run();

	delete gage;

}