#include "map.h"



void cast_light_for_view(Map * map, int x, int y, int radius, int row, double start_slope, double end_slope, int xx, int xy, int yx, int yy){

	if(start_slope < end_slope){
		return;
	}

	double next_start_slope = start_slope;
	for(int i = row; i <= radius; i++){
		bool blocked = false;
		for(int dx = -i, dy = -i; dx <= 0; dx++){
			float l_slope = (dx - 0.5)/(dy + 0.5);
			float r_slope = (dx + 0.5)/(dy - 0.5);
			if (start_slope < r_slope){
				continue;
			}
			else if(end_slope > l_slope){
				break;
			}

			int sax = dx*xx + dy*xy;
			int say = dx*yx + dy*yy;

			if((sax < 0 && abs(sax) > x) || (say < 0 && abs(say) > y)){
				continue;
			}

			int ax = x + sax;
			int ay = y + say;

			bool exit = 0;

			#pragma omp critical
			if(ax>=WIDTH/map->tile_size || ay>=HEIGHT/map->tile_size){
				exit = 1;
			}

			if(exit){
				continue;
			}

			if(dx*dx+dy*dy < radius*radius){
				//printf("%d, %d ", ax, ay);
				#pragma omp critical
				map->render_mask[ay+HEIGHT/map->tile_size*ax] = 1;
			}
			//#pragma omp critical
			//{
			if(blocked){
				if(map->a[ay+ax*HEIGHT/map->tile_size].back() == 1 || map->a[ay+ax*HEIGHT/map->tile_size].back() == -1 || map->a[ay+ax*HEIGHT/map->tile_size].back() == 2){
					next_start_slope = r_slope;
					//continue;
				}
				else{
					blocked = false;
					start_slope = next_start_slope;
				}
			}
			else if(map->a[ay+ax*HEIGHT/map->tile_size].back() == 1 || map->a[ay+ax*HEIGHT/map->tile_size].back() == -1){
				blocked = true;
				next_start_slope = r_slope;
				cast_light_for_view(map, x, y, radius, i+1, start_slope, l_slope, xx, xy, yx, yy);
			}
			//}
		}

	if(blocked){
		break;
	}



	}


};

// Тоже самое, но тут - поле зрения игрока

void cast_light_for_player(Map * map, int x, int y, int radius, int row, double start_slope, double end_slope, int xx, int xy, int yx, int yy){

	if(start_slope < end_slope){
		return;
	}

	double next_start_slope = start_slope;
	for(int i = row; i <= radius; i++){
		bool blocked = false;
		for(int dx = -i, dy = -i; dx <= 0; dx++){
			float l_slope = (dx - 0.5)/(dy + 0.5);
			float r_slope = (dx + 0.5) / (dy - 0.5);
			if (start_slope < r_slope){
				continue;
			}
			else if(end_slope > l_slope){
				break;
			}

			int sax = dx*xx + dy*xy;
			int say = dx*yx + dy*yy;

			if((sax < 0 && abs(sax) > x) || (say < 0 && abs(say) > y)){
				continue;
			}

			int ax = x + sax;
			int ay = y + say;

			bool exit = 0;

			#pragma omp critical
			if(ax>=WIDTH/map->tile_size || ay>=HEIGHT/map->tile_size || ax < 0 || ay < 0){
				exit = 1;
			}

			if(exit){
				continue;
			}

			if(dx*dx+dy*dy < radius*radius){
				#pragma omp critical
				{
				if(map->player_lighting_mask.count(ay+HEIGHT/map->tile_size*ax) == 0){
					map->player_lighting_mask[ay+HEIGHT/map->tile_size*ax] = 255*(radius*radius-(ax-x)*(ax-x)-(ay-y)*(ay-y))/(radius*radius);
				}
				else{
					if(255*(radius*radius-(ax-x)*(ax-x)-(ay-y)*(ay-y))/(radius*radius) > map->player_lighting_mask[ay+HEIGHT/map->tile_size*ax])
						map->player_lighting_mask[ay+HEIGHT/map->tile_size*ax] = 255*(radius*radius-(ax-x)*(ax-x)-(ay-y)*(ay-y))/(radius*radius);
				}
				}
			}
			//#pragma omp critical
			//{
			if(blocked){
				if(map->a[ay+ax*HEIGHT/map->tile_size].back() == 1 || map->a[ay+ax*HEIGHT/map->tile_size].back() == -1){
					next_start_slope = r_slope;
					//continue;
				}
				else{
					blocked = false;
					start_slope = next_start_slope;
				}
			}
			else if(map->a[ay+ax*HEIGHT/map->tile_size].back() == 1 || map->a[ay+ax*HEIGHT/map->tile_size].back() == -1){
				blocked = true;
				next_start_slope = r_slope;
				cast_light_for_player(map, x, y, radius, i+1, start_slope, l_slope, xx, xy, yx, yy);
			}
			//}
		}

	if(blocked){
		break;
	}



	}


};

// Это просто костыль

static int multipliers[4][8] = {
    {1, 0, 0, -1, -1, 0, 0, 1},
    {0, 1, -1, 0, 0, -1, 1, 0},
    {0, 1, 1, 0, 0, -1, -1, 0},
    {1, 0, 0, 1, -1, 0, 0, -1}
};

/*
	Краткое описание того, что делает update.
	По факту он вызывает shadow-cast для каждого октета

	собстенно для каждого октета в цикле я и считаю видимые клетки (делаю рекурсивный shadow-cast)

				
	   \   |   /
	    \  |  /
	     \ | /
	______\|/_____
		  /|\
		 / | \
		/  |  \
	   /   |   \


*/

void Map::update_view_mask(const double & x_coord, const double & y_coord, double radius_coord){

	int x = int(round(x_coord/tile_size));
	int y = int(round(y_coord/tile_size));

	int radius = int(round(radius_coord/tile_size));

	render_mask.clear();

	render_mask[y+HEIGHT/tile_size*x] = 1;

	#pragma omp parallel for
	for(int i = 0; i < 8; i++){
		cast_light_for_view(this, x, y, radius, 1, 1.0, 0.0, multipliers[0][i],
			multipliers[1][i], multipliers[2][i], multipliers[3][i]);
	}


}


void Map::update_player_lighting_mask(const double & x_coord, const double & y_coord, double radius_coord){

	int x = int(round(x_coord/tile_size));
	int y = int(round(y_coord/tile_size));

	int radius = int(round(radius_coord/tile_size));

	player_lighting_mask[y+HEIGHT/tile_size*x] = 255;

	#pragma omp parallel for
	for(int i = 0; i < 8; i++){
		cast_light_for_player(this, x, y, radius, 1, 1.0, 0.0, multipliers[0][i],
			multipliers[1][i], multipliers[2][i], multipliers[3][i]);
	}


}

void Map::reset_player_lighting_mask(){
	player_lighting_mask.clear();
}

/*

	Этот метод возвращает элемент маски видимости, на котором находится точка с произвольными кооридантами
	простыми слвоами:

	1 - в поле зрения, 0 - иначе

*/

const bool Map::getMask(double x, double y){

	return (render_mask.count(int(round(y/tile_size))+int(round(x/tile_size))*HEIGHT/tile_size)) != 0;

}

int Map::getLum(double x, double y){

	if(env_lighting_mask.count(int(round(y/tile_size))+int(round(x/tile_size))*HEIGHT/tile_size)==0){
		if(player_lighting_mask.count(int(round(y/tile_size))+int(round(x/tile_size))*HEIGHT/tile_size)==0){
			return 0;
		}
		return player_lighting_mask[int(round(y/tile_size))+int(round(x/tile_size))*HEIGHT/tile_size];
	}

	return env_lighting_mask[int(round(y/tile_size))+int(round(x/tile_size))*HEIGHT/tile_size];

}

void cast_light_for_env(Map * map, int x, int y, int radius, int row, double start_slope, double end_slope, int xx, int xy, int yx, int yy, bool is_on){


	if(start_slope < end_slope){
		return;
	}

	double next_start_slope = start_slope;
	for(int i = row; i <= radius; i++){
		bool blocked = false;
		for(int dx = -i, dy = -i; dx <= 0; dx++){
			float l_slope = (dx - 0.5)/(dy + 0.5);
			float r_slope = (dx + 0.5)/(dy - 0.5);
			if (start_slope < r_slope){
				continue;
			}
			else if(end_slope > l_slope){
				break;
			}

			int sax = dx*xx + dy*xy;
			int say = dx*yx + dy*yy;

			if((sax < 0 && abs(sax) > x) || (say < 0 && abs(say) > y)){
				continue;
			}

			int ax = x + sax;
			int ay = y + say;

			bool exit = 0;

			#pragma omp critical
			if(ax>=WIDTH/map->tile_size || ay>=HEIGHT/map->tile_size || ax < 0 || ay < 0){
				exit = 1;
			}

			if(exit){
				continue;
			}

			if(dx*dx+dy*dy < radius*radius){
				if(is_on){
					 #pragma omp critical
					map->env_lighting_mask[ay+HEIGHT/map->tile_size*ax] = 255-30+30*(-abs(ax-x)-abs(ay-y)+radius)/radius;
				}
				else{
					 #pragma omp critical
					map->env_lighting_mask.erase(ay+HEIGHT/map->tile_size*ax);
				}
			}
			//#pragma omp critical
			//{
			if(blocked){
				if(map->a[ay+ax*HEIGHT/map->tile_size].back() == 1 || map->a[ay+ax*HEIGHT/map->tile_size].back() == -1 || map->a[ay+ax*HEIGHT/map->tile_size].back() == 2){
					next_start_slope = r_slope;
					//continue;
				}
				else{
					blocked = false;
					start_slope = next_start_slope;
				}
			}
			else if(map->a[ay+ax*HEIGHT/map->tile_size].back() == 1 || map->a[ay+ax*HEIGHT/map->tile_size].back() == -1 || map->a[ay+ax*HEIGHT/map->tile_size].back() == 2){
				blocked = true;
				next_start_slope = r_slope;
				cast_light_for_env(map, x, y, radius, i+1, start_slope, l_slope, xx, xy, yx, yy, is_on);
			}
			//}
		}

	if(blocked){
		break;
	}



	}


};



void Map::update_env_lighting_mask(const double & x_coord, const double & y_coord, double radius_coord, bool is_on){

	int x = int(round(x_coord/tile_size));
	int y = int(round(y_coord/tile_size));

	int radius = int(round(radius_coord/tile_size));

	env_lighting_mask[y+HEIGHT/tile_size*x] = 255;

	#pragma omp parallel for
	for(int i = 0; i < 8; i++){
		cast_light_for_env(this, x, y, radius, 1, 1.0, 0.0, multipliers[0][i],
			multipliers[1][i], multipliers[2][i], multipliers[3][i], is_on);
	}


}