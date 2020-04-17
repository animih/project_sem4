#include "map.h"

struct Node1{

	int x;
	int y;

	int cost = 0;

};

struct cmp1{
bool operator()(const Node1 &nd1, const Node1 &nd2){

	return nd1.cost > nd2.cost;

	}
};


Tactics::Tactics(Map * map){
	this->map = map;
}

Tactics::~Tactics(){

}

std::pair<int, int> Tactics::lead_mob(double x1, double y1, double x2, double y2, bool walk){

	int x_1 = int(round(x1/map->tile_size));
	int y_1 = int(round(y1/map->tile_size));

	if(map->a[y_1+HEIGHT/map->tile_size*x_1].back() == 1 || map->a[y_1+HEIGHT/map->tile_size*x_1].back() == -1){
		x_1 = int((x1/map->tile_size));
		if(map->a[y_1+HEIGHT/map->tile_size*x_1].back() == 1 || map->a[y_1+HEIGHT/map->tile_size*x_1].back() == -1){
			x_1 = int(round(x1/map->tile_size));
			y_1 = int((y1/map->tile_size));
		}
	}

	int x_2 = int(round(x2/map->tile_size));
	int y_2 = int(round(y2/map->tile_size));


	printf("me : %d %d \nmy crush: %d %d \n", x_1, y_1, x_2, y_2);

	if(x_2 == x_1 && y_2 == y_1){
		return std::make_pair(0, 0);
	}

	allies.push_back(x_1+y_1*HEIGHT/map->tile_size);

	std::vector<Node1> open = {};
	std::map <int, int> cost_so_far;
	std::map <int, int> came_from;
	came_from[y_1+HEIGHT/map->tile_size*x_1] = -1;
	cost_so_far[y_1+HEIGHT/map->tile_size*x_1] = 0;

	Node1 start;
	start.x = x_1;
	start.y = y_1;

	open.push_back(start);

	int x;
	int y;
	Node1 current;

	while(!open.empty()){


		current = *(open.begin());
		open.erase(open.begin());

		printf("%d %d \n", current.x, current.y);

		if(current.x == x_2 && current.y == y_2){
			x = current.x;
			y = current.y;
			break;
		}

		for(int i = 0; i < 8; i ++){
			switch(i){
				case 0:
					x = current.x-1;
					y = current.y;
					break;
				case 1:
					x = current.x+1;
					y = current.y;
					break;
				case 2:
					x = current.x;
					y = current.y-1;
					break;
				case 3:
					x = current.x;
					y = current.y+1;
					break;
				case 4:
					x = current.x+1;
					y = current.y+1;
					break;
				case 5:
					x = current.x-1;
					y = current.y-1;
					break;
				case 6:
					x = current.x+1;
					y = current.y-1;
					break;
				case 7:
					x = current.x-1;
					y = current.y+1;
					break;
			}
			if(x < 0 || x > WIDTH/map->tile_size-1 || y < 0 || y > HEIGHT/map->tile_size-1){
				continue;
			}

			if(y+HEIGHT/map->tile_size*x == came_from[current.y+HEIGHT/map->tile_size*current.x]){
				continue;
			}



			int add = abs(x-current.x)+abs(y-current.y);

			if(map->a[y+HEIGHT/map->tile_size*x].back() == -1){
				add += 6666;
			}
			if(map->a[y+HEIGHT/map->tile_size*x].back() == 1){
				add += 6666;
			}
			if(walk){
				for(int i : allies){
					if(abs(i%(HEIGHT/map->tile_size) - x) < 4 &&  abs(i/(HEIGHT/map->tile_size) - y) < 4)
						add += 2;
					if(abs(i%(HEIGHT/map->tile_size) - x) <= 2 &&  abs(i/(HEIGHT/map->tile_size) - y) <= 2)
						add += 2;
				}
			}

			add += cost_so_far[current.y+HEIGHT/map->tile_size*current.x];
			//printf("%d, %d, %d \n", add, x, y);

			if(cost_so_far.count(y+HEIGHT/map->tile_size*x) == 0 || add < cost_so_far[y+HEIGHT/map->tile_size*x]){
				cost_so_far[y+HEIGHT/map->tile_size*x] = add;
				Node1 append;
				append.x = x;
				append.y = y;
				append.cost = add + abs(x_2-x)+abs(y_2-y);

				came_from[y+HEIGHT/map->tile_size*x] = current.y+HEIGHT/map->tile_size*current.x;
				open.push_back(append);

			}

		}

		std::make_heap(open.begin(), open.end(), cmp1());
	}

	int x_buf;
	int y_buf;

	int buf;

	printf("Time to backtrack\n");

	while(came_from[y+HEIGHT/map->tile_size*x] != -1){

		printf("%d %d \n", x, y);

		x_buf = x;
		y_buf = y;

		buf = came_from[y+HEIGHT/map->tile_size*x];

		x = buf/(HEIGHT/map->tile_size);
		y = buf%(HEIGHT/map->tile_size);

	}

	printf("%d %d \n", x, y);

	came_from.clear();
	cost_so_far.clear();

	return std::make_pair(x_buf-x_1, y_buf-y_1);

}

void Tactics::refresh(){
	if(this->allies.size() != 0){
		this->allies.clear();
	}

}