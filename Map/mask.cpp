#include "map.h"

// Короче. Это ф-ция для рекурсивного алгоса шадоу-каста. Работает.



void Map::cast_light_for_view(int x, int y, int radius, int row, double start_slope, double end_slope, int xx, int xy, int yx, int yy){

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

			int sax = dx * xx + dy *xy;
			int say = dx*yx + dy*yy;

			if((sax < 0 && abs(sax) > x) || (say < 0 && abs(say) > y)){
				continue;
			}

			int ax = x + sax;
			int ay = y + say;

			if(ax>=WIDTH/tile_size || ay>=HEIGHT/tile_size){
				continue;
			}

			if(dx*dx+dy*dy < radius*radius){
				//printf("%d, %d ", ax, ay);
				render_mask[ay+HEIGHT/tile_size*ax] = 1;
			}

			if(blocked){
				if(this->a[ay+ax*HEIGHT/tile_size].back() == 1 || this->a[ay+ax*HEIGHT/tile_size].back() == -1 || this->a[ay+ax*HEIGHT/tile_size].back() == 2){
					next_start_slope = start_slope;
					continue;
				}
				else{
					blocked = false;
					start_slope = next_start_slope;
				}
			}
			else if(this->a[ay+ax*HEIGHT/tile_size].back() == 1 || this->a[ay+ax*HEIGHT/tile_size].back() == -1){
				blocked = true;
				next_start_slope = r_slope;
				this->cast_light_for_view(x, y, radius, i+1, start_slope, l_slope, xx, xy, yx, yy);
			}
		}

	if(blocked){
		break;
	}



	}


};

// Тоже самое, но тут для маски света поле зрения игрока

void Map::cast_light_for_player(int x, int y, int radius, int row, double start_slope, double end_slope, int xx, int xy, int yx, int yy){

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

			int sax = dx * xx + dy *xy;
			int say = dx*yx + dy*yy;

			if((sax < 0 && abs(sax) > x) || (say < 0 && abs(say) > y)){
				continue;
			}

			int ax = x + sax;
			int ay = y + say;

			if(ax>=WIDTH/tile_size || ay>=HEIGHT/tile_size || ax < 0 || ay < 0){
				continue;
			}

			if(dx*dx+dy*dy < radius*radius){
				player_lighting_mask[ay+HEIGHT/tile_size*ax] = 255*(radius*radius-(ax-x)*(ax-x)-(ay-y)*(ay-y))/(radius*radius);
			}

			if(blocked){
				if(this->a[ay+ax*HEIGHT/tile_size].back() == 1 || this->a[ay+ax*HEIGHT/tile_size].back() == -1){
					next_start_slope = start_slope;
					continue;
				}
				else{
					blocked = false;
					start_slope = next_start_slope;
				}
			}
			else if(this->a[ay+ax*HEIGHT/tile_size].back() == 1 || this->a[ay+ax*HEIGHT/tile_size].back() == -1){
				blocked = true;
				next_start_slope = r_slope;
				this->cast_light_for_player(x, y, radius, i+1, start_slope, l_slope, xx, xy, yx, yy);
			}
		}

	if(blocked){
		break;
	}



	}


};

// Это просто костыль, который я подсмотрел у чувака на гитхабе (он там писал шадоу-каст, но чуть иначе)

static int multipliers[4][8] = {
    {1, 0, 0, -1, -1, 0, 0, 1},
    {0, 1, -1, 0, 0, -1, 1, 0},
    {0, 1, 1, 0, 0, -1, -1, 0},
    {1, 0, 0, 1, -1, 0, 0, -1}
};

/*
	Краткое описание того, что делает апдейт.
	По факту он вызывает шадоу-каст для каждого октета:

	собстенно для каждого октета в цикле я и считаю видимые клетки (делаю рекурсивный шадоу-каст)

				
	   \   |   /
	    \  |  /
	     \ | /
	______\|/_____
		  /|\
		 / | \
		/  |  \
	   /   |   \

	Чтобы переписать вычисления хдесь паралленльно, надо каждую операцию в цикле запускать
	в своей ветке (всего получится 8 -штук)

	первая ветка, полседние 4 коэфицента - 1, 0, 0, 1
	вторая ветка, последние 4 коэфицента - 0 , 1, 1, 0
	и т.д.

	(просто смотри соответсвующий n-элемент каждого столбца в multipliers)


*/

void Map::update_view_mask(const double & x_coord, const double & y_coord, double radius_coord){

	int x = int(round(x_coord/tile_size));
	int y = int(round(y_coord/tile_size));

	int radius = int(round(radius_coord/tile_size));

	render_mask.clear();

	render_mask[y+HEIGHT/tile_size*x] = 1;

	for(int i = 0; i < 8; i++){
		this->cast_light_for_view(x, y, radius, 1, 1.0, 0.0, multipliers[0][i],
			multipliers[1][i], multipliers[2][i], multipliers[3][i]);
	}


}

/*

	Тот же самый алгос, но дял маски игрока света.
	Также нужно переписать через параллельное программирование

*/

void Map::update_player_lighting_mask(const double & x_coord, const double & y_coord, double radius_coord){

	int x = int(round(x_coord/tile_size));
	int y = int(round(y_coord/tile_size));

	int radius = int(round(radius_coord/tile_size));

	player_lighting_mask.clear();

	player_lighting_mask[y+HEIGHT/tile_size*x] = 255;

	for(int i = 0; i < 8; i++){
		this->cast_light_for_player(x, y, radius, 1, 1.0, 0.0, multipliers[0][i],
			multipliers[1][i], multipliers[2][i], multipliers[3][i]);
	}


}

/*

	Этот метод возвращает элемент маски видимости, на котором находится точка с произвольными кооридантами
	простыми слвоами:

	true - в поле зрения
	false - не-а

*/

const bool Map::getMask(double x, double y){

	return (render_mask.count(int(round(y/tile_size))+int(round(x/tile_size))*HEIGHT/tile_size)) != 0;

}

/*

	Возварщает коэф прозрачности для элемента макси, на котором находится точка с произвольынми коордами.
	255 - не прозрачный
	0 - абсолютно прозрачный

	Если элемента нет в маске, то возвращается 0

*/

int Map::getLum(double x, double y){

	if(env_lighting_mask.count(int(round(y/tile_size))+int(round(x/tile_size))*HEIGHT/tile_size)==0){
		if(player_lighting_mask.count(int(round(y/tile_size))+int(round(x/tile_size))*HEIGHT/tile_size)==0){
			return 0;
		}
		return player_lighting_mask[int(round(y/tile_size))+int(round(x/tile_size))*HEIGHT/tile_size];
	}

	return env_lighting_mask[int(round(y/tile_size))+int(round(x/tile_size))*HEIGHT/tile_size];

}

/*

 Это шадоу-каст для окружения

*/

void Map::cast_light_for_env(int x, int y, int radius, int row, double start_slope, double end_slope, int xx, int xy, int yx, int yy, bool is_on){


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

			int sax = dx * xx + dy *xy;
			int say = dx*yx + dy*yy;

			if((sax < 0 && abs(sax) > x) || (say < 0 && abs(say) > y)){
				continue;
			}

			int ax = x + sax;
			int ay = y + say;

			if(ax>=WIDTH/tile_size || ay>=HEIGHT/tile_size || ax < 0 || ay < 0){
				continue;
			}

			if(dx*dx+dy*dy < radius*radius){
				if(is_on){
					env_lighting_mask[ay+HEIGHT/tile_size*ax] = 255-30+30*(-abs(ax-x)-abs(ay-y)+radius)/radius;
				}
				else{
					env_lighting_mask.erase(ay+HEIGHT/tile_size*ax);
				}
			}

			if(blocked){
				if(this->a[ay+ax*HEIGHT/tile_size].back() == 1 || this->a[ay+ax*HEIGHT/tile_size].back() == -1 || this->a[ay+ax*HEIGHT/tile_size].back() == 2){
					next_start_slope = start_slope;
					continue;
				}
				else{
					blocked = false;
					start_slope = next_start_slope;
				}
			}
			else if(this->a[ay+ax*HEIGHT/tile_size].back() == 1 || this->a[ay+ax*HEIGHT/tile_size].back() == -1 || this->a[ay+ax*HEIGHT/tile_size].back() == 2){
				blocked = true;
				next_start_slope = r_slope;
				this->cast_light_for_env(x, y, radius, i+1, start_slope, l_slope, xx, xy, yx, yy, is_on);
			}
		}

	if(blocked){
		break;
	}



	}


};

/*

 Снова тот же самый метод, снова требуется переписать параллельно.

*/


void Map::update_env_lighting_mask(const double & x_coord, const double & y_coord, double radius_coord, bool is_on){

	int x = int(round(x_coord/tile_size));
	int y = int(round(y_coord/tile_size));

	int radius = int(round(radius_coord/tile_size));

	env_lighting_mask[y+HEIGHT/tile_size*x] = 255;

	for(int i = 0; i < 8; i++){
		this->cast_light_for_env(x, y, radius, 1, 1.0, 0.0, multipliers[0][i],
			multipliers[1][i], multipliers[2][i], multipliers[3][i], is_on);
	}


}