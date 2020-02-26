#include <SFML/Graphics.hpp>
#include "lib.h"
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
		speedx = - speedx;
	}
	if(y_top < 0 || y_bottom > HEIGHT){
		speedy = - speedy;
	}

	x_left += 0.05*time*speedx/num_neighbors;
	x_right += 0.05*time*speedx/num_neighbors;

	y_top += 0.05*time*speedy/num_neighbors;
	y_bottom += 0.05*time*speedy/num_neighbors;

	speedx = 0;
	speedy = 0;

	num_neighbors = 1;

}

void Room::upd(RenderWindow * window){

	RectangleShape room(sf::Vector2f(x_right-x_left, y_bottom-y_top));

	/*
	
	if(x_right-x_left < 0 || y_bottom-y_top < 0)
		printf("%f, %f \n", x_right-x_left, y_bottom-y_top);

	*/

	room.setPosition(x_left, y_top);
 	room.setFillColor(sf::Color(23, 23, 23));
 	room.setOutlineThickness(2);
 	room.setOutlineColor(sf::Color(255, 255, 255));
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
	std::normal_distribution<double> distribution((float)average, (float)average/4);

	size[0] = int(distribution(generator));
	size[1] = 0;

	while(size[1]*5<size[0] || size[0]*5 < size[1])
		size[1] = int(distribution(generator));

}


Graph::Graph(char V){
	this->V = V;
	for(int i =0; i < V; i++)
	a.push_back({});
}
void Graph::addEdge(char u, char v){
	a[u].push_back(v);
	a[v].push_back(u);
}
