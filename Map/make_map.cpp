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

    coords[0] = (Rooms->Final[0]->x_left+Rooms->Final[0]->x_right)/2;
    coords[1] = (Rooms->Final[0]->y_top+Rooms->Final[0]->y_bottom)/2;

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


    for(int i = 0; i< 4; i++){
        for(int j = 0; j < 4; j ++){
            a[10+i+HEIGHT/tile_size*(15+j)].push_back(3);
        }
    }


    a[20+HEIGHT/tile_size*20].push_back(6);

}

double * Map::make_boss_battle(int tile_size, std::map<std::string, std::vector<double>> & buf){

    this->tile_size = tile_size;

    render_mask.clear();
    env_lighting_mask.clear();





    a.clear();

    for(int j = 0; j < HEIGHT/tile_size*WIDTH/tile_size; j++){
        a.push_back(std::list<char>());
    }

    for(int i = 0; i < 30; i++){
        for(int j = 0; j < 30; j++){
            if(i == 0 || j == 0|| i == 29 || j == 29)
                a[j+HEIGHT/tile_size*i].push_back(1);
            else
                a[j+HEIGHT/tile_size*i].push_back(0);
        }
    }

    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            a[j+4+HEIGHT/tile_size*(i+4)].push_back(1);
        }
    }

    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            a[j+4+HEIGHT/tile_size*(i+25)].push_back(1);
        }
    }

    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            a[j+25+HEIGHT/tile_size*(i+25)].push_back(1);
        }
    }

    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            a[j+25+HEIGHT/tile_size*(i+4)].push_back(1);
        }
    }

    buf["Boss"].push_back(10*tile_size);
    buf["Boss"].push_back(5*tile_size);


    double * coords = new double[2];

    coords[0] = 10*tile_size;
    coords[1] = 10*tile_size;

    return coords;

}