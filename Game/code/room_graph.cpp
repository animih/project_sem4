#include <SFML/Graphics.hpp>
#include "map.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <random>
#include <iostream>
#include <algorithm>
#include <list>
#include <functional>


Room::Room(double x_left, double x_right, double y_top, double y_bottom){
	this->x_left = x_left;
	this->x_right = x_right;
	this->y_top = y_top;
	this->y_bottom = y_bottom;

}

void Room::move(float time){

	if(x_left < 0 || x_right > WIDTH){
		speedx = - speedx; // Упругий удар о стенки
	}
	if(y_top < 0 || y_bottom > HEIGHT){
		speedy = - speedy; // Упругий Удар о стенки
	}

	x_left += 0.05*time*speedx/num_neighbors;
	x_right += 0.05*time*speedx/num_neighbors;

	y_top += 0.05*time*speedy/num_neighbors;
	y_bottom += 0.05*time*speedy/num_neighbors;

	speedx = 0;
	speedy = 0;

	num_neighbors = 1;

}


/*Рисуем комнату. Тут нужно понимать, что "Outline" рисуется поверх 
прямоугольнкиа, потому может возникнуть илюзия "наслоения" между непересекающимися прямоугольнкиами */


void Room::upd(RenderWindow * window){

	RectangleShape room(sf::Vector2f(x_right-x_left, y_bottom-y_top));

	room.setPosition(x_left, y_top);


	switch(color){
		case 0:
			room.setFillColor(sf::Color(23, 23, 23));
			break;
		case 1:
			room.setFillColor(sf::Color::Blue);
			break;
		case 2:
			room.setFillColor(sf::Color::Green);
			break;
		case 3:
			room.setFillColor(sf::Color::Yellow);
			break;
		case 4:
			room.setFillColor(sf::Color::Cyan);
			break;
		case 5:
			room.setFillColor(sf::Color::White);
			break;
		case 6:
			room.setFillColor(sf::Color(101, 63, 65));

	}
 	room.setOutlineThickness(2);
 	room.setOutlineColor(sf::Color(255, 255, 255));

 	if(outline){
 		room.setOutlineColor(sf::Color::Red);
 		outline = 0;
 	}

 	window->draw(room);

}



void pyaniy_polyarnik(int radius, double* coord){


	float r = (float)rand()/(float)(RAND_MAX)+(float)rand()/(float)(RAND_MAX);

	if(r>1){
		r -= 1;
	}

	float t = (float)rand()/(float)(RAND_MAX)*2*M_PI;

	coord[0] = radius*r*cos(t);
	coord[1] = radius*r*sin(t);


}

void random_size(int average, int* size){

	static std::default_random_engine generator;
	std::normal_distribution<double> distribution((float)average, (float)average/3.1);

	size[0] = int(distribution(generator));
	size[1] = 0;

	while(size[1]*5<size[0] || size[0]*5 < size[1])
		size[1] = int(distribution(generator));

}


Graph::Graph(int V){
	this->V = V;
	for(int i =0; i < V; i++)
	a.push_back({});
}
void Graph::addEdge(int u, int v){
	a[u].push_back(v);
	a[v].push_back(u);
}
