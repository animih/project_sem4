#include "lib.h"
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <utility>

using namespace sf;

GameMap::GameMap(){

	for(int i =0; i< HEIGHT/10; i++){
		for(int j =0; j<WIDTH/10; j++){

			map[i][j] = 0;
		}
	}

}



void GameMap::Dunkyard_Walk(int num){

	int random;



	static int x = 0;
	static int y = 0;

	int flag = 0;

	int var[8];

	map[x][y] = 1;


	for(int i = 0; i < num; i++){

		random = rand()%4;
		flag = 0;

		if(x+1 < WIDTH/10){
			var[2*flag] = y;
			var[2*flag+1] = x+1;
			flag ++;

		}
		if(x-1 > 0){
			var[2*flag] = y;
			var[2*flag+1] = x-1;
			flag ++;


		}

		if(y+1 < HEIGHT/10){
			var[2*flag] = y+1;
			var[2*flag+1] = x;
			flag ++;

		}
		if(y-1 > 0){
			var[2*flag] = y-1;
			var[2*flag+1] = x;
			flag ++;

		}



		for(int j = 0; j <= random; j++){
			y = var[2*(j%flag)];
			x = var[2*(j%flag)+1];	
		}

		map[y][x] = 1;


	}


}

void GameMap::update(RenderWindow * window){

	RectangleShape space(sf::Vector2f(10, 10));
	space.setFillColor(sf::Color(150, 153, 123));

	for(int i =0; i< HEIGHT/10; i++){
		for(int j =0; j<WIDTH/10; j++){
			if(map[i][j] == 0){
				space.setPosition(j*10, i*10);
				window->draw(space);
			}
			
		}
	}


}