#include "lib.h"
#include <math.h>
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace sf;

#define num_in_x (list->Final[index].x_right+list->Final[index].x_left)/2
#define num_in_y (list->Final[index].y_top+list->Final[index].y_bottom)/2

#define num_u_x (list->Final[u].x_right+list->Final[u].x_left)/2
#define num_u_y (list->Final[u].y_top+list->Final[u].y_bottom)/2

Player::Player(RoomList * Rooms){

	x = (Rooms->Final[0].x_left + Rooms->Final[0].x_right)/2;
	y = (Rooms->Final[0].y_top + Rooms->Final[0].y_bottom)/2;

	
	list = Rooms;

	index = 0;


}

void Player::add_dir(double x, double y){

	dir_x += x;
	dir_y += y;


	if(dir_x != 0 && dir_y !=0){
		dir_x /= sqrt(dir_x*dir_x + dir_y * dir_y);
		dir_y /= sqrt(dir_x*dir_x + dir_y * dir_y);
	}

	double vect_x;
	double vect_y;


	for (char u : list->graph->a[index]){
		
		if(this->x+dir_x <= list->Final[u].x_right+2 &&  this->x >= list->Final[u].x_left-2
		&&  this->y+dir_y <= list->Final[u].y_bottom+2 &&  this->y >= list->Final[u].y_top-2){
			index = u;
			printf("this: %d neghbors: ", u);
			for(char v: list->graph->a[index]){
				printf("%d ", v);
			}
			printf("\n");
			return;
		}
		if(u < list->help && index < list->help && index){
			vect_x = this->x+dir_x- num_in_x;
			vect_y = this->y+dir_y - num_in_y;

			vect_x -= (vect_x*(num_u_x-num_in_x) + vect_y*(num_u_y-num_in_y))/
				((num_u_x-num_in_x)*(num_u_x-num_in_x) + (num_u_y-num_in_y)* (num_u_y-num_in_y))*(num_u_x-num_in_x);
			vect_y -= (vect_x*(num_u_x-num_in_x) + vect_y*(num_u_y-num_in_y))/
				((num_u_x-num_in_x)*(num_u_x-num_in_x) + (num_u_y-num_in_y)* (num_u_y-num_in_y))*(num_u_y-num_in_y);

			//printf("%f %f \n", vect_x, vect_y);

			if(vect_x*vect_x + vect_y*vect_y < 64) //&& (this->x-num_in_x)*(this->x-num_u_x) < 0)			
				return;
		}
		
	}
	




	if(cheet){
		return;
	}

	
	
	if((this->x +dir_x > list->Final[index].x_right && dir_x > 0 )|| (this->x +dir_x < list->Final[index].x_left && dir_x < 0)){
		dir_x = 0;
	}
	
	if((this->y +dir_y > list->Final[index].y_bottom && dir_y > 0) || (this->y +dir_y < list->Final[index].y_top && dir_y < 0)){
		dir_y = 0;
	}
	
	
}


void Player::update(RenderWindow * window, float time){

	RectangleShape player(sf::Vector2f(10, 10));

	list->Final[index].upd(window);

	for(char u : list->graph->a[index]){
		list->Final[u].upd(window);
	}

	player.setPosition(x-5, y-5);
	player.setFillColor(sf::Color(15, 164, 120));

	x += speed*dir_x*time;
	y += speed*dir_y*time;

	dir_x = 0;
	dir_y = 0;

	window->draw(player);

}