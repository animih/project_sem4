#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "map.h"

using namespace sf;

// Это тестовый main-файл для карты

int main(){

	sf::RenderWindow window(sf::VideoMode(640, 480), "RGlike v. 1.0.0"); 

    sf::Clock clock;
    Event event;

    sf::View view;
    view.reset(sf::FloatRect(0, 0, 640, 480));

    double * a = new double[2];

    Map * karta = new Map(16); // размер тайла

    int * b = new int[2];

    a = karta->make_map(b, 16, 90, 225, 65, &window);

    printf("%d , %d \n", b[0], b[1]);

    printf("%f, %f", a[0], a[1]);

    while (window.isOpen())
     {

    	while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

    }

    delete karta;

    return 0;


}