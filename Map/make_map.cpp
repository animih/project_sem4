#include <iostream>
#include <malloc.h>
#include <list>
#include <math.h>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "map.h"

using namespace sf;

double * Map::make_map(int * size, int tile_size, int radius, int average, int number, RenderWindow * window, std::map<std::string, std::vector<double>> & buf){

	this->tile_size = tile_size;

	Rooms = new RoomList();

    printf("HI! \n");

	Rooms->tile_size = tile_size;
	Rooms->generate_rooms(radius, average, number);

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
        if(Rooms->push_rooms(time/2)){
			break;
		}

		timer += time/1000;

        if(timer >= 4.5){
        	timer = 0;
        	break;
        	
        }
		
		
        window->clear(); 
		Rooms->update(window);
		window->display();
	}

	Rooms->TriEdges();

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
        	timer = 0;
        	break;
        }

        window->clear();
        
        
        Rooms->DrawEdges(window);

        window->display();
    }

    Rooms->BuildTree();


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
        	timer = 0;
        	break;
        }

        window->clear();  
        Rooms->DrawEdges(window);
        window->display();
    }

    Rooms->AddWalkRooms();

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
        	timer = 0;
        	break;
        }

        window->clear();  
        Rooms->DrawEdges(window);
        window->display();
    }

    Rooms->transpose_to_matrix(a);

   	this->build_Hallways();



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
        	timer = 0;
        	break;
        }

        window->clear();
        
        this->render_all(window);

        window->display();
    }

    double * coords = new double[2];

    coords[0] = (Rooms->Final[0].x_left+Rooms->Final[0].x_right)/2;
    coords[1] = (Rooms->Final[0].y_top+Rooms->Final[0].y_bottom)/2;

    // После гены листы удаляются

    this->Gen_Sur(buf);
    delete Rooms;

    while(window->isOpen()){
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
        	timer = 0;
        	break;
        }

        window->clear();
        
        this->render_all(window);

        window->display();


    }

    //size = new int[2];

    size[0] = WIDTH/tile_size;
    size[1] = HEIGHT/tile_size;
    
    return coords;

}

void Map::make_test_map(int tile_size){
    this->tile_size = tile_size;

    a.clear();

    for(int j = 0; j < HEIGHT/tile_size*WIDTH/tile_size; j++){
        a.push_back(std::list<char>());
    }

    for(int i = 0; i < WIDTH/tile_size; i++){
        for(int j = 0; j < HEIGHT/tile_size; j++){
            if(i == 0 || j == 0|| i == WIDTH/tile_size-1 || j == HEIGHT/tile_size-1 )
                a[j+HEIGHT/tile_size*i].push_back(1);
            else
                a[j+HEIGHT/tile_size*i].push_back(0);
        }
    }


    a[3+HEIGHT/tile_size*2].push_back(1);
    a[3+HEIGHT/tile_size*3].push_back(1);
    a[4+HEIGHT/tile_size*2].push_back(1);

    a[6+HEIGHT/tile_size*2].push_back(1);
    a[7+HEIGHT/tile_size*3].push_back(1);
    a[7+HEIGHT/tile_size*2].push_back(1);


    a[3+HEIGHT/tile_size*5].push_back(1);
    a[3+HEIGHT/tile_size*6].push_back(1);
    a[4+HEIGHT/tile_size*6].push_back(1);

    a[6+HEIGHT/tile_size*6].push_back(1);
    a[7+HEIGHT/tile_size*6].push_back(1);
    a[7+HEIGHT/tile_size*5].push_back(1);


    a[20+HEIGHT/tile_size*20].push_back(6);

}