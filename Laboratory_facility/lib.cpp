#include <SFML/Graphics.hpp>
#include "lib.h"
#include <random>
#include <algorithm>
#include <list>
#include <functional>
#include <math.h>

space::space(int height, int width){

	this->height = height;
	this->width = width;

}

void space::gen_bnd(){

	out = {};
	in = {};

	int x, y;

	for(float a = 0.; a<2.0*M_PI;){
		x = width/2 + round((width/2.2*cos(a)));
		y = height/2 + round((height/2.2*sin(a)));
		out.push_front(point(x, y));
		a += (10.0 + 25.0*rand()/RAND_MAX)*M_PI/180.0;
	}



	for(float a = 0.; a<2.0*M_PI;){
		x = width/2 + round((width/10*cos(a)));
		y = height/2 + round((height/10*sin(a)));
		in.push_front(point(x, y));
		a += (20.0 + 40.0*rand()/RAND_MAX)*M_PI/180.0;
	}

	int k;

	

	for(auto u = std::next(out.begin()); u != out.end();){

		k = rand()%2;
		

		switch(k){
			case 0:
				out.insert(u, point(u->first, std::prev(u)->second));
				break;
			case 1:
				out.insert(u, point(std::prev(u)->first, u->second));
				break;
		}

		++u;
		//u = out.end();

	}


	k = rand()%2;
		

		switch(k){
			case 0:
				out.insert(out.begin(), point(out.begin()->first, std::prev(out.end())->second));
				break;
			case 1:
				out.insert(out.begin(), point(std::prev(out.end())->first, out.begin()->second));
				break;
		}




	
}

void space::update_out(sf::RenderWindow * window){

	sf::VertexArray line(sf::LinesStrip, out.size()+1);
	auto color = sf::Color::White;



	int i = 0;
	for(auto obj : out){
		line[i].position = sf::Vector2f(obj.first, obj.second);
		i++;
	}

	line[out.size()].position = line[0].position;

	window->draw(line);
}

void space::update_in(sf::RenderWindow * window){

	sf::VertexArray line(sf::LinesStrip, in.size()+1);
	auto color = sf::Color::White;



	int i = 0;
	for(auto obj : in){
		line[i].position = sf::Vector2f(obj.first, obj.second);
		i++;
	}

	line[in.size()].position = line[0].position;

	window->draw(line);
}