#include <SFML/Graphics.hpp>


const int WIDTH = 840;
const int HEIGHT = 640; 

using namespace sf;

class GameMap{
	char map[HEIGHT/10][WIDTH/10];

	public:
	GameMap();
	void Dunkyard_Walk(int num);
	void update(RenderWindow * window);

};