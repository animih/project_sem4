#include <iostream>
#include <malloc.h>
#include <list>
#include <math.h>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "lib.h"


using namespace sf;




int main(int argc, char*argv[]){

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "It's dark again");

    View view;

    view.reset(sf::FloatRect(0, 0, 840, 680));

    float CurrentFrame = 0;
    Clock clock;
    Event event;

    GameMap Map;

    Map.Dunkyard_Walk(7650);

   // Map.Dunkyard_Walk(440);

    

    while(window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/800;

        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
        }

        

        window.setView(view);
        window.clear();

        Map.update(&window);


        window.display();

    }

}