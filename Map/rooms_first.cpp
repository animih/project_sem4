
#include "map.h"
#include "delauney.h"

#define INF 9999999999

//Просто костыль для облегчения написания кода

#define num1_top Final[i].y_top
#define num1_bottom  Final[i].y_bottom
#define num1_left Final[i].x_left
#define num1_right Final[i].x_right
#define num1_x (Final[i].x_left+Final[i].x_right)/2
#define num1_y (Final[i].y_bottom+Final[i].y_top)/2

#define num2_top Final[*it].y_top
#define num2_bottom Final[*it].y_bottom
#define num2_left Final[*it].x_left
#define num2_right Final[*it].x_right
#define num2_x (Final[*it].x_left+Final[*it].x_right)/2
#define num2_y (Final[*it].y_bottom+Final[*it].y_top)/2

RoomList::RoomList(){}

// 0 - base, 1 - Все комнаты раскиданы
bool RoomList::is_collide(std::_List_iterator<Room> room1, std::_List_iterator<Room>room2){

	float * coord1 = (float*)malloc(2*sizeof(float));
	int * size1 = (int*)malloc(2*sizeof(int));

	coord1[0] = (room1->x_left + room1->x_right)/2;
	coord1[1] = (room1->y_bottom + room1->y_top)/2;

	size1[0] = (room1->x_right - room1->x_left)/2;
	size1[1] = (room1->y_bottom - room1->y_top)/2;

	float * coord2 = (float*)malloc(2*sizeof(float));
	int * size2 = (int*)malloc(2*sizeof(int));

	coord2[0] = (room2->x_left + room2->x_right)/2;
	coord2[1] = (room2->y_bottom + room2->y_top)/2;

	size2[0] = (room2->x_right - room2->x_left)/2;
	size2[1] = (room2->y_bottom - room2->y_top)/2;

	return fabs(coord1[0]-coord2[0]) <= (size1[0]+ size2[0]+1)*1.01 &&
		fabs(coord1[1]-coord2[1]) <= (size1[1]+ size2[1]+1)*1.01;

}

bool RoomList::push_rooms(float time){

	bool flag = 1;

	float speedx;
	float speedy;

	for(auto it1 = list.begin(); it1 != list.end(); ){

		for(auto it2 = it1; it2 != list.end();){
			
			
			if(is_collide(it1, it2)){
				flag =0;
				speedx = (it1->x_right + it1->x_left - it2->x_right - it2->x_left)/2;
				speedy = (it1->y_top + it1->y_bottom - it2->y_top - it2->y_bottom)/2;

				it1->speedx += speedx;
				it1->speedy += speedy;

				it2->speedx -= speedx;
				it2->speedy -= speedy;

				it1->num_neighbors++;
				it2->num_neighbors++;

			}
			++it2;

		}
		++it1;
	}

	for(auto it = list.begin(); it != list.end(); ++it){
		it->move(time);
	}

	return flag;
}

void RoomList::update(RenderWindow * window){
	for(Room dung : list){
		dung.upd(window);
	}

}

/* 
	Генерируем комнаты. 

	Размеры - Гауссовское распр
	Координаты - равномерное по кругу

*/

void RoomList::generate_rooms(int radius, int average, int total_number){

	double * coord = (double*)malloc(2*sizeof(double));
	int * size = (int*)malloc(2*sizeof(int));

	this->average = average;

	for(int i =0; i<total_number; i++){

		pyaniy_polyarnik(radius, coord);

		coord[0] += WIDTH/2.;
		coord[1] += HEIGHT/2.;

		random_size(average, size, tile_size);



		Room room_member(coord[0]-size[0]/2., coord[0]+size[0]/2.,
			coord[1]-size[1]/2., coord[1]+size[1]/2., tile_size);
		list.push_front(room_member);
	}

	

}

/*

	Здесь происходит триангуляция Делоне.
	За библиотеку спасибо пользователю delfrrr

*/


void RoomList::TriEdges(){

	std::vector <double> coords = {};

	for(auto it = list.begin(); it != list.end();){

		if((it->x_right-it->x_left)*(it->y_bottom - it->y_top) > average*average*1.525){
		Final.push_back(*it);
		
		coords.push_back((it->x_right + it->x_left)/2);
		coords.push_back((it->y_bottom + it->y_top)/2);
		auto it_del = it;
		++it;
		list.erase(it_del);
		continue;
		}
		 ++it;
	}
	

	delaunator::Delaunator d(coords);


	graph = new Graph(Final.size());

	for(std::size_t i = 0; i < d.triangles.size(); i+=3){

		graph->addEdge(d.triangles[i], d.triangles[i+1]);
		graph->addEdge(d.triangles[i+1], d.triangles[i+2]);
		graph->addEdge(d.triangles[i+2], d.triangles[i]);
		

	}

	for(int i = 0; i < graph->a.size(); i++){
		graph->a[i].sort();
		graph->a[i].unique();
	}



}

/*

	Обновлённая функция прорисовки

*/

void RoomList::DrawEdges(RenderWindow * window){

	sf::VertexArray line(sf::LinesStrip, 2);

	auto color = sf::Color::White;

	for(int i = 0; i < Final.size(); i++){
		Final[i].upd(window);
	}

	
	for(int i = 0; i < Final.size()-1; i++){
		line[0].position =  sf::Vector2f((Final[i].x_left+Final[i].x_right)/2, 
			(Final[i].y_bottom+Final[i].y_top)/2);
		for(int u : graph->a[i]){
			if(u == i){
				printf("error. Cycle detected \n"); //Просто проверка на случай если мы сделали цикл
				exit(0);
			}
			if(u > i){
				
				line[1].position =  sf::Vector2f((Final[u].x_left+Final[u].x_right)/2, 
				(Final[u].y_bottom+Final[u].y_top)/2);

				window->draw(line);
			}

			
		}
	}
	
	
}


/* 
	Дальше на подходе алгоритм Прима.
*/

int pop_min(double * weight, int number){
	int k = 0;
	for(int i =0; i < number; i++){
		if(weight[k] == -1. || weight[i] < weight[k] && weight[i] >= 0){
			k = i;
		}
	}

	weight[k] = -1.;
	return k;

}

double square(double a){
	return a*a;
}

void RoomList::BuildTree(){
	
	int number = Final.size();

	double * weight = new double[number];
	int * parrent = new int[number];


	std::vector<int> res = {};


	for(int i = 0; i< number; i++){
		weight[i] = INF; // Весь весовой массив - бесконечности
		parrent[i] = -1; // Изначально родителей нет
	}

	weight[0] = 0;
	int v = pop_min(weight, number);

	int flag = 1;
	float ran;

	while(flag!= number){
		for(int u : graph->a[v]){

			if(weight[u] < 0){
				continue;
			}

			ran = square((Final[u].x_left+Final[u].x_right) - (Final[v].x_left+Final[v].x_right))
			+ square((Final[u].y_bottom+Final[u].y_top) - (Final[v].y_bottom+Final[v].y_top));
			if(ran < weight[u]){
				weight[u] = ran;
				parrent[u] = v;
			}
		}



		v = pop_min(weight, number);
		
		
		
		res.push_back(parrent[v]);
		res.push_back(v);
		flag++;
		
	}

	
	int n = rand()%number;

	/* Здесь происходит возвращение части рёбер из триангуляции
		По-просту говоря, это нужно для наличия проходных путей.
	*/

	
	std::vector <int> visited = {};

	for(int i = 0; i<number; i+= 7){
		n = rand()%number;
		for(int k: graph->a[n]){
			if(parrent[k] == n || parrent[n] == k){
				continue;
			}
			res.push_back(n);
			res.push_back(k);

			visited.push_back(n);
			visited.push_back(k);
		}

	}

	free(weight);
	free(parrent);
	
	
	
	free(graph);
	graph = new Graph(number);

	for(int i = 0; i <res.size(); i+=2){
		graph->addEdge(res[i], res[i+1]);
	}


}

//Костылииии

double min(double a, double b){
	return a < b ? a : b;
}
double max(double a, double b){
	return a > b ? a : b;
}

void RoomList::AddWalkRooms(){
	
	double k;

	int num = Final.size();

	int n = 0;
	int flag = 0;

	for(int i = 0; i < num; i++){
		graph->a[i].sort();
		graph->a[i].unique();
	}
	
	
	for(Room room: list){
		flag = 1;
		for(int i = 0; i < num; i++){
			auto it_end =  graph->a[i].end();
			for(auto it=graph->a[i].begin(); it != it_end; it++){

				k = (num1_y - num2_y)/(num1_x - num2_x);

				if( ( (k*(room.x_left - num1_x) + num1_y - room.y_top)*(k*(room.x_right - num1_x) + num1_y- room.y_bottom) < 0 
					&& (room.x_left - min(num1_left, num2_left) )*(room.x_left - max(num1_right, num2_right)) < 0 && ((room.y_bottom+room.y_top)/2 - num1_y) * ((room.y_bottom+room.y_top)/2 - num2_y) < 0 ) || 
					( (k*(room.x_right - num1_x) + num1_y - room.y_top)*(k*(room.x_left - num1_x) + num1_y- room.y_bottom) < 0 
					&& (room.x_right - min(num1_left, num2_left) )*(room.x_right - max(num1_right, num2_right)) < 0 && ((room.y_bottom+room.y_top)/2 - num1_y) * ((room.y_bottom+room.y_top)/2 - num2_y) < 0 ) ||

					( (1/k*(room.y_top - num1_y) + num1_x - room.x_left)*(1/k*(room.y_bottom - num1_y) + num1_x - room.x_right) < 0 
					&& (room.y_top - min(num1_top, num2_top))*(room.y_top - max(num1_bottom, num2_bottom)) < 0 && ((room.x_left+room.x_right)/2 - num1_x) * ((room.x_left+room.x_right)/2 - num2_x) < 0 ) || 
					( (1/k*(room.y_bottom - num1_y) + num1_x - room.x_left)*(1/k*(room.y_top - num1_y) + num1_x - room.x_right) < 0 
					&& (room.y_bottom - min(num1_top, num2_top))*(room.y_bottom - max(num1_bottom, num2_bottom)) < 0 && ((room.x_left+room.x_right)/2 - num1_x) * ((room.x_left+room.x_right)/2 - num2_x) < 0 ) ){

					if(flag){
						graph->a.push_back({});
						Final.push_back(room);
						flag = 0;
						n++;
						num++;
					}

					graph->addEdge(*it, (int)num-1);
					graph->a[*it].remove(i);
					graph->a[*it].unique();

					graph->a[(int)num-1].push_back(i);
					*it = (int)num-1;


					it_end = graph->a[i].end();

				}
				
			}
			graph->a[i].unique();


		}
	}
	
	graph->a[num-1].unique();
	num = Final.size();

	for(int i = 0; i < graph->a.size(); i++){
		graph->a[i].sort();
		graph->a[i].unique();
	}

	int color;
	bool flag2;

	// красим комнаты

	int counter = 0;
	int counter2 = 0;

	printf("total: %d \n", num);

	for(int i = 0; i < num; i++){
		graph->a[i].sort();
		graph->a[i].unique();
		color = rand()%4+1;
		if(graph->a[i].size() > 3){

			counter2 += 1;
			if(counter2 < 3)
				Final[i].color = 5;
			else
				Final[i].color = rand()%4+1;
			printf("%d : %d \n", i, Final[i].color);
			continue;
		}
		if(graph->a[i].size() == 1 && counter < 2){
			Final[i].color = 6;
			counter += 1;
			printf("%d : %d \n", i, Final[i].color);
			continue;
		}
		else
			while(1){
				flag2 = 1;
				for(int u : graph->a[i]){
					if(Final[u].color == color){
						color = rand()%4+1;
						flag2 = 0;
						break;
					}
				}
				if(flag2){
					break;
				}
			}
		//printf("%d ", color);
		Final[i].color = color;
		printf("%d : %d \n", i, Final[i].color);

	}
	list.clear();
}