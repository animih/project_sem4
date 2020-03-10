#include <iostream>
#include <malloc.h>
#include <list>
#include <math.h>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "lib.h"

using namespace sf;


int main(int argc, char * argv[]){

	space where(HEIGHT, WIDTH);

	where.gen_bnd();

	RenderWindow window(VideoMode(WIDTH, HEIGHT), "It's dark again");	

	Clock clock;
	Event event;

	while(window.isOpen()){
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time/800;

		while(window.pollEvent(event))
		{
			if(event.type == Event::Closed)
				window.close();
        }


        window.clear();
        where.update_out(&window);
        where.update_in(&window);


        window.display();
	}



}