#include <iostream>
#include <malloc.h>
#include <list>
#include <math.h>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "lib.h"


using namespace sf;


int main(int argc, char*argv[])
{


    RoomList Rooms;

    Rooms.generate_rooms(100, 30, 90); //rad & avr & numb

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "It's dark again");

    View view;

    view.reset(sf::FloatRect(0, 0, 840, 680));

    float CurrentFrame = 0;
    Clock clock;
    Event event;

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

        if(Keyboard::isKeyPressed(Keyboard::Escape)){
            break;
        }

        Rooms.push_rooms(time);

        window.setView(view);
        window.clear();



        Rooms.update(&window);



        window.display();

    }

    Rooms.TriEdges();

    while(window.isOpen())
    {

        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
        }

        if(Keyboard::isKeyPressed(Keyboard::W)){
            break;
        }



        window.clear();

        Rooms.DrawEdges(&window);

        window.display();
    }

    Rooms.BuildTree();

    while(window.isOpen())
    {

        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
        }

        if(Keyboard::isKeyPressed(Keyboard::Escape)){
            break;
        }



        window.clear();

        Rooms.DrawEdges(&window);

        window.display();
    }

    Rooms.AddWalkRooms();

    while(window.isOpen())
    {

        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
        }

        if(Keyboard::isKeyPressed(Keyboard::W)){
            break;
        }



        window.clear();

        Rooms.DrawEdges(&window);

        window.display();
    }

    while(window.isOpen())
    {

        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
        }



        window.clear();

        Rooms.update(&window);
        Rooms.DrawEdges(&window);

        window.display();
    }


    return 0;
}