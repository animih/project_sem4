#include <iostream>
#include <malloc.h>
#include <list>
#include <math.h>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "map.h"

using namespace sf;

double * RoomList::make_map(int radius, int average, int total_number, RenderWindow * window){

	this->generate_rooms(radius, average, total_number);

	Clock clock;
	Event event;

    sf::View view;
    view.reset(sf::FloatRect(0, 0, WIDTH, HEIGHT));

    window->setView(view);

	double timer = 0;

	while(window->isOpen()){
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time/800;

		while(window->pollEvent(event))
        {
            if(event.type == Event::Closed)
                window->close();
            	
        }
        
        //Пихаем комнаты в стороны
        if(this->push_rooms(time)){
			break;
		}

		timer += time/1000;

        if(timer >= 3.5){
        	break;
        	
        }
		
		
        window->clear();
        
		this->update(window);

		window->display();
	}

	//Строим триангуляцию
	this->TriEdges();

	timer = 0;

	while(window->isOpen())
    {
    	float time = clock.getElapsedTime().asMicroseconds();
    	clock.restart();
    	time = time/800;
        while(window->pollEvent(event))
        {
            if(event.type == Event::Closed)
                window->close();
        }

        timer += time/1000;

        if(timer >= 1.2){
        	break;
        }

        window->clear();
        
        
        this->DrawEdges(window);

        window->display();
    }

    this->BuildTree();

    timer = 0;

    while(window->isOpen())
    {
    	float time = clock.getElapsedTime().asMicroseconds();
    	clock.restart();
    	time = time/800;
        while(window->pollEvent(event))
        {
            if(event.type == Event::Closed)
                window->close();
        }

        timer += time/1000;

        if(timer >= 1.2){
        	break;
        }

        window->clear();
        
        this->DrawEdges(window);

        window->display();
    }

    this->AddWalkRooms();

    while(window->isOpen())
    {
    	float time = clock.getElapsedTime().asMicroseconds();
    	clock.restart();
    	time = time/800;
        while(window->pollEvent(event))
        {
            if(event.type == Event::Closed)
                window->close();
        }

        timer += time/1000;

        if(timer >= 2.5){
        	break;
        }

        window->clear();
        

        this->DrawEdges(window);

        window->display();
    }

    double * a = new double[2];

	a[0] = (this->Final[0].x_left + this->Final[0].x_right)/2;
	a[1] = (this->Final[0].y_top + this->Final[0].y_bottom)/2;
    
	return a;


};