/*

Библиотека для построения пути от точки А в точку Б.
Здесь представлен известный алгоритм A* в моём исполнении.

Ф-ция A_star_inside может быть использована для построения пути
моба из одной комнаты в другую.

входные данные одни и те же:
1) индекс x первой точки (в матрице)
2) индекс y первой точки
3) -..- тоже самое для второй точки
4) матрица тайлов
5) размеры матрицы (n по y и m по x)

Если потребуется переписать ф-цию со специфичными приоритетами тайлов
(например избегать замедляющих клеток) - обращайтесь. (делается это несложно)

*/

#include <stdlib.h>

struct Node{

	int x;
	int y;

	int cost = 0;

};

struct cmp{
bool operator()(const Node &nd1, const Node &nd2){

	return nd1.cost > nd2.cost;

	}
};


//это для коридоров

std::vector<int> A_star(int x1, int y1, int x2, int y2, int * a, int n, int m){

	std::vector<Node> open = {};
	std::map <int, int> cost_so_far;
	std::map <int, int> came_from;
	came_from[y1+n*x1] = -1;
	cost_so_far[y1+n*x1] = 0;

	Node start;
	start.x = x1;
	start.y = y1;

	open.push_back(start);

	int x;
	int y;
	Node current;

	while(!open.empty()){


		current = *(open.begin());
		open.erase(open.begin());

		
		if(current.x == x2 && current.y == y2){
			//printf("found");
			break;
		}

		//printf("%d, %d \n", current.x, current.y);

		for(int i = 0; i < 4; i ++){
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
			}
			if(x < 0 || x > m-1 || y < 0 || y > n-1){
				continue;
			}

			if(y+n*x == came_from[current.y+n*current.x]){
				continue;
			}

			int add = abs(x-current.x)+abs(y-current.y);

			if(a[y+n*x] == -1){
				add += 1;
			}
			if(a[y+n*x] == 1){
				add += 10;
			}

			add += cost_so_far[current.y+n*current.x];
			//printf("%d, %d, %d \n", add, x, y);

			if(cost_so_far.count(y+n*x) == 0 || add < cost_so_far[y+n*x]){
				cost_so_far[y+n*x] = add;
				Node append;
				append.x = x;
				append.y = y;
				append.cost = add + abs(x2-x)+abs(y2-y);

				came_from[y+n*x] = current.y+n*current.x;
				open.push_back(append);

			}

		}

		std::make_heap(open.begin(), open.end(), cmp());
	}

	std::vector<int> result = {};

	x = current.x;
	y = current.y;

	result.insert(result.begin(), y);
	result.insert(result.begin(), x);

	int buf;

	while(came_from[y+n*x] != -1){

		buf = came_from[y+n*x];

		x = buf/n;
		y = buf%n;

		result.insert(result.begin(), y);
		result.insert(result.begin(), x);

	}


	return result;

}

// это для путей внутри

std::vector<int> A_star_inside(int x1, int y1, int x2, int y2, int * a, int n, int m){

	std::vector<Node> open = {};
	std::map <int, int> cost_so_far;
	std::map <int, int> came_from;
	came_from[y1+n*x1] = -1;
	cost_so_far[y1+n*x1] = 0;

	Node start;
	start.x = x1;
	start.y = y1;

	open.push_back(start);

	int x;
	int y;
	Node current;

	while(!open.empty()){


		current = *(open.begin());
		open.erase(open.begin());

		
		if(current.x == x2 && current.y == y2){
			//printf("found");
			break;
		}

		//printf("%d, %d \n", current.x, current.y);

		for(int i = 0; i < 4; i ++){
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
			}
			if(x < 0 || x > m-1 || y < 0 || y > n-1){
				continue;
			}

			if(y+n*x == came_from[current.y+n*current.x]){
				continue;
			}

			int add = abs(x-current.x)+abs(y-current.y);

			if(a[y+n*x] == -1){
				add += 666;
			}
			if(a[y+n*x] == 1){
				add += 6666;
			}

			add += cost_so_far[current.y+n*current.x];
			//printf("%d, %d, %d \n", add, x, y);

			if(cost_so_far.count(y+n*x) == 0 || add < cost_so_far[y+n*x]){
				cost_so_far[y+n*x] = add;
				Node append;
				append.x = x;
				append.y = y;
				append.cost = add + abs(x2-x)+abs(y2-y);

				came_from[y+n*x] = current.y+n*current.x;
				open.push_back(append);

			}

		}

		std::make_heap(open.begin(), open.end(), cmp());
	}

	std::vector<int> result = {};

	x = current.x;
	y = current.y;

	result.insert(result.begin(), y);
	result.insert(result.begin(), x);

	int buf;

	while(came_from[y+n*x] != -1){

		buf = came_from[y+n*x];

		x = buf/n;
		y = buf%n;

		result.insert(result.begin(), y);
		result.insert(result.begin(), x);

	}


	return result;

}