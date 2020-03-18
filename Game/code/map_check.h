#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include "Entity.h"


#define num_in_x (list->Final[index].x_right+list->Final[index].x_left)/2
#define num_in_y (list->Final[index].y_top+list->Final[index].y_bottom)/2

#define num_u_x (list->Final[u].x_right+list->Final[u].x_left)/2
#define num_u_y (list->Final[u].y_top+list->Final[u].y_bottom)/2


void map_check(Entity *mob, RoomList * list) {

	double vect_x;
	double vect_y;

	int index = mob->index;


	for (char u : list->graph->a[index]){
		
		if(mob->x <= list->Final[u].x_right+2 &&  mob->x >= list->Final[u].x_left-2
		&&  mob->y <= list->Final[u].y_bottom+2 &&  mob->y >= list->Final[u].y_top-2){
			mob->index = u;
			return;
		}
		if(u < list->help && index < list->help && index){
			vect_x = mob->x - num_in_x;
			vect_y = mob->y - num_in_y;

			vect_x -= (vect_x*(num_u_x-num_in_x) + vect_y*(num_u_y-num_in_y))/
				((num_u_x-num_in_x)*(num_u_x-num_in_x) + (num_u_y-num_in_y)* (num_u_y-num_in_y))*(num_u_x-num_in_x);
			vect_y -= (vect_x*(num_u_x-num_in_x) + vect_y*(num_u_y-num_in_y))/
				((num_u_x-num_in_x)*(num_u_x-num_in_x) + (num_u_y-num_in_y)* (num_u_y-num_in_y))*(num_u_y-num_in_y);

			

			if(vect_x*vect_x + vect_y*vect_y < 64 && (mob->x-num_in_x)*(mob->x-num_u_x) < 0)			
				return;
		}
		
	}

	
	
	if(mob->x > list->Final[index].x_right || mob->x < list->Final[index].x_left){
		mob->x -= mob->dx;
	}
	
	if(mob->y > list->Final[index].y_bottom || mob->y < list->Final[index].y_top){
		mob->y -= mob->dy;
	}

};