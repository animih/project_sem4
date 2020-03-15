#include <iostream>
#include <malloc.h>
#include <list>
#include <math.h>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "lib.h"

using namespace sf;


int main(int argc, char * argv[]){

	RoomList Rooms;

	Rooms.generate_rooms(50, 40, 95); // РАдиус распредления, средний размер и число комнат

	RenderWindow window(VideoMode(WIDTH, HEIGHT), "It's dark again");

	View view; //Переменная обзора, пока не используется
	view.reset(sf::FloatRect(0, 0, 840, 680));


	Clock clock;
	Event event;

	double timer = 0;

	while(window.isOpen()){
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time/800;

		while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
            	
        }
        
        //Пихаем комнаты в стороны
        if(Rooms.push_rooms(time)){
			break;
		}

		timer += time/1000;

        if(timer >= 2){
        	break;
        	
        }
		
		window.setView(view);
        window.clear();
		Rooms.update(&window);

		window.display();
	}

	//Строим триангуляцию
	Rooms.TriEdges();

	timer = 0;

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

        timer += time/1000;

        if(timer >= 1.2){
        	break;
        }

        window.clear();
        window.setView(view);
        Rooms.DrawEdges(&window);

        window.display();
    }

    Rooms.BuildTree();

    timer = 0;

    bool draw = 1;

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

        timer += time/1000;

        if(timer >= 1.2){
        	break;
        }

        window.clear();
        window.setView(view);
        Rooms.DrawEdges(&window);

        window.display();
    }

    Rooms.AddWalkRooms();

    Player GG(&Rooms);

    
    int counter = 0;

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


        if (Keyboard::isKeyPressed(Keyboard::Left))
            GG.add_dir(-1, 0); 

        if (Keyboard::isKeyPressed(Keyboard::Right))
            GG.add_dir(1, 0); 

        if (Keyboard::isKeyPressed(Keyboard::Up))
            GG.add_dir(0, -1); 

        if (Keyboard::isKeyPressed(Keyboard::Down))
            GG.add_dir(0, 1); 


        

        window.clear();
        window.setView(view);

        GG.cheet = 0;

        if (Keyboard::isKeyPressed(Keyboard::Escape))
            GG.cheet = 1;
        
        
        if (Keyboard::isKeyPressed(Keyboard::K)){
            counter++;
            if(counter == 80){
                draw = (draw+1)%2;
            }
        }
        else{
            counter=0;
        }

        if(draw)
            Rooms.DrawEdges(&window);
            
        GG.update(&window, time);



        window.display();
 	}
	return 0;
}