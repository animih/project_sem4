#include <SFML/Graphics.hpp>
#include "lib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <random>
#include <list>
#include <iostream>
#include "delauney.h"
#include <math.h>

// Условная "бесконечность" для алг Прима (ключ - растояние между центрами)

#define INF WIDTH*WIDTH+HEIGHT*HEIGHT

//Просто, чтобы не заебаться пока пишешь это

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

//Проверка комнат на пересечение

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

	return fabs(coord1[0]-coord2[0]) <= (size1[0]+ size2[0]+1)*1.02 &&
		fabs(coord1[1]-coord2[1]) <= (size1[1]+ size2[1]+1)*1.02;

}

// 0 - base, 1 - Все комнаты раскиданы

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

/* Генерируем лист комнат. 

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

		random_size(average, size);



		Room room_member(coord[0]-size[0]/2., coord[0]+size[0]/2.,
			coord[1]-size[1]/2., coord[1]+size[1]/2.);
		list.push_front(room_member);
	}

	

}

/*

	Здесь происходит триангуляция Делоне.
	По-хорошему, стоит перепписать библиотеку под свои нужды.
	Но в прицнипе, итак работает.
	Так что и пофиг.

*/


void RoomList::TriEdges(){

	std::vector <double> coords = {};

	for(auto it = list.begin(); it != list.end();){

		if((it->x_right-it->x_left)*(it->y_bottom - it->y_top) > average*average*1.725){
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
		for(char u : graph->a[i]){
			if(u == i){
				printf("erro \n"); //Просто проверка на случай если мы сделали цикл на один узел (я забыл как называется)
				exit(0);
			}
			if(u > i){

				if(u < help && i < help){

					int k = (Final[u].y_bottom + Final[u].y_top - Final[i].y_top - Final[i].y_bottom) / (Final[u].x_left + Final[u].x_right - Final[i].x_left - Final[i].x_right);

					// Собственно вектор номрали (ненормированный) это (-k, 1)

					std::vector<double> norm = {-k/sqrt(k*k+1*1), 1/sqrt(k*k+1*1)};

					sf::VertexArray line_new(sf::LineStrip, 5);
					line_new[0].position = sf::Vector2f((Final[u].x_left+Final[u].x_right)/2-norm[0]*4, 
				(Final[u].y_bottom+Final[u].y_top)/2-norm[1]*4);

					line_new[1].position = sf::Vector2f((Final[u].x_left+Final[u].x_right)/2+norm[0]*4, 
				(Final[u].y_bottom+Final[u].y_top)/2+norm[1]*4);
					
					line_new[2].position = sf::Vector2f((Final[i].x_left+Final[i].x_right)/2+norm[0]*4, 
				(Final[i].y_bottom+Final[i].y_top)/2+norm[1]*4);


					line_new[3].position = sf::Vector2f((Final[i].x_left+Final[i].x_right)/2-norm[0]*4, 
				(Final[i].y_bottom+Final[i].y_top)/2-norm[1]*4);

					line_new[4].position = sf::Vector2f((Final[u].x_left+Final[u].x_right)/2-norm[0]*4, 
				(Final[u].y_bottom+Final[u].y_top)/2-norm[1]*4);

					line_new[0].color = sf::Color::Green;
					line_new[1].color = sf::Color::Green;
					line_new[2].color = sf::Color::Green;
					line_new[3].color = sf::Color::Green;
					line_new[4].color = sf::Color::Green;


					window->draw(line_new);

					continue;
				}
				
				line[1].position =  sf::Vector2f((Final[u].x_left+Final[u].x_right)/2, 
				(Final[u].y_bottom+Final[u].y_top)/2);

				window->draw(line);
			}

			
		}
	}

	
}

/* 
	Короче, дальше на подходе алгоритм Прима
	Перовначально, я писал его через кучу (отсюда и название pop_min)
	Но, как оказалось, из-за одной микроштуки там всё равно скорость о(n)
	Так что я переписал всё под массив
	НУ а хуле нам, физикам.
*/

char pop_min(double * weight, int number){
	char k = 0;
	for(char i =0; i < number; i++){
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


	std::vector<char> res = {};


	for(int i = 0; i< number; i++){
		weight[i] = INF; // Весь весовой массив - бесконечности
		parrent[i] = -1; // Все дети - сироты. Мир - печлаьное место
	}

	weight[0] = 0;
	char v = pop_min(weight, number);

	char flag = 1;
	float ran;

	while(flag!= number){
		for(char u : graph->a[v]){

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

		//graph->a[parrent[v]] = {v};
		
		
		
		res.push_back(parrent[v]);
		res.push_back(v);
		flag++;
		
	}

	
	int n = rand()%number;

	/* Здесь происходит возварщение части рёбер из триангуляции
		По-просту говоря, я хоу иметь проодные пути в своём подземелье.
	
		Беда в том, что такой подход рушит всё то, ради чего минимальное дерево строилось
		Из-за этого, редко, происхожит наслаивание "коридоров". Мб получится решить эту проблему прорисовкой в ходе игры (один идёт сверху, другйо снизу)

		Если кто-то придумает более изящный сопосб возвращения проходных коридоров, я буду очень рад.
	*/

	
	std::vector <int> visited = {};

	for(int i = 0; i<number; i+=5){
		//while(std::find(visited.begin(), visited.end(), n) != visited.end()) // ТУт проблема - почему-то работает лучше без исключения повтора (?!). Скорей всего я - лох
		n = rand()%number;
		for(char k: graph->a[n]){
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

	for(char i = 0; i <res.size(); i+=2){
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


	// Переписать блять!
	

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
					else{
						//continue;
						printf("Got Ya! %d  \n", num-1);
					}

					graph->addEdge(*it, (char)num-1);
					graph->a[*it].remove(i);
					graph->a[*it].unique();

					graph->a[(char)num-1].push_back(i);
					*it = (char)num-1;


					it_end = graph->a[i].end();

				}
				
			}
			graph->a[i].unique();


		}
	}
	graph->a[num-1].unique();
	num = Final.size();

	/*
	for(Room room: list){
		flag = 1;
		for(int i = 0; i < num; i++){


			auto it_end =  graph->a[i].end();

		for(auto it = graph->a[i].begin(); it != it_end; ++it){
			

			k = (num1_y - num2_y)/(num1_x - num2_x);
			if( ((room.x_left+room.x_right)/2 - num1_x)*((room.x_left+room.x_right)/2 - num2_x) < 0 &&  
			((room.y_top+room.y_bottom)/2 - num1_y)*((room.y_top+room.y_bottom)/2 - num2_y) < 0  && 
			((k*(room.x_left - num1_x) + num1_bottom - room.y_top)*(k*(room.x_left - num1_x) + num1_top- room.y_bottom) < 0 ||
				(k*(room.x_right - num1_x) + num1_bottom - room.y_top)*(k*(room.x_right - num1_x) + num1_top - room.y_bottom) < 0 || 
				(1/k*(room.y_top - num1_y) + num1_right - room.x_left)*(1/k*(room.y_top - num1_y) + num1_left- room.x_right) < 0 ||
				(1/k*(room.y_bottom - num1_y) + num1_right - room.x_left)*(1/k*(room.y_bottom - num1_y) + num1_left - room.x_right) < 0 ) ){
				if(flag){
				graph->a.push_back({});
				Final.push_back(room);
				flag = 0;
				n++;
				num++;
				}
				else{
					printf("Got Ya! %d  \n", num-1);
				}

				
				graph->addEdge(*it, (char)num-1);
				graph->a[*it].remove(i);

				graph->a[(char)num-1].push_back(i);
				*it = (char)num-1;


				it_end = graph->a[i].end();
				
				
				

				}
		 	
			}
			graph->a[i].unique();

			
		}
	}
	graph->a[num-1].unique();
	num = Final.size();


	*/
	double y1;
	double y2;


	help = num; /* Для диагоналяных коридоров я исопльзую след костыль - 
	записываю куда-то значение числа комнат, док оридоров и потом, еслив стречаю ребро соединящее узлы
	менье этого размера, обращаюсь с ними как с прямой */
	
	num = Final.size();

	//return;
	
	// Здесь монтирую двери
	
	for(int i = 0; i < help; i++){
		auto it_end = graph->a[i].end();
		for(auto it = graph->a[i].begin(); it != it_end; ++it){
			if(*it >= help){
				continue;
			}
			if(num1_left - num2_right < 14 && num1_left - num2_right > 0){

				if(num1_y > num2_top+4 && num1_y < num2_bottom -4){
					y1 = num1_y;
				}
				else if(num1_top > num2_top+3 && num1_top < num2_bottom -3){

					y1 = (num1_top+num2_bottom)/2;

				}
				else if(num1_bottom > num2_top+3 && num1_bottom < num2_bottom -3){
					
					y1 = (num1_bottom+num2_top)/2;

				}
				else{
					continue;
				}


				Final.push_back(Room(num2_right, num1_left, y1 - 5, y1 + 5));
				graph->a.push_back({});

				graph->addEdge(*it, (char)Final.size()-1);
				graph->a[*it].remove(i);

				graph->a[(char)Final.size()-1].push_back(i);
				*it = (char)Final.size()-1;


				it_end = graph->a[i].end();

			}
			if(num1_top - num2_bottom < 14 && num1_top - num2_bottom > 0){

				if(num1_x > num2_left+4 && num1_x < num2_right -4){
					y1 = num1_x;
				}
				else if(num1_left > num2_left+3 && num1_left < num2_right -3){

					y1 = (num1_left+num2_right)/2;

				}
				else if(num1_right > num2_left+3 && num1_right < num2_right -3){
					
					y1 = (num1_right+num2_left)/2;

				}
				else{
					continue;
				}


				Final.push_back(Room(y1 - 5, y1 + 5, num2_bottom , num1_top));
				graph->a.push_back({});

				graph->addEdge(*it, (char)Final.size()-1);
				graph->a[*it].remove(i);

				graph->a[(char)Final.size()-1].push_back(i);
				*it = (char)Final.size()-1;


				it_end = graph->a[i].end();

			}

		}
	}

	
	for(int i = 0; i < help; i++){
		auto it_end = graph->a[i].end();
		for(auto it = graph->a[i].begin(); it != it_end; ++it){
			if(*it >= help){
				continue;
			}
			if(num1_left + 2 < num2_x && num2_x < num1_right - 2 ){
				if(fabs(num1_top - num2_bottom) < 6 || 
					fabs(num2_top - num1_bottom) < 6)
					continue;

				if(num2_top > num1_bottom){
					y1 = num1_bottom;
					y2 = num2_top;
				}
				else{
					y1 = num2_bottom;
					y2 = num1_top;
				}


				Final.push_back(Room(num2_x - 5, num2_x + 5, y1, y2));
				graph->a.push_back({});

				graph->addEdge(*it, (char)Final.size()-1);
				graph->a[*it].remove(i);

				graph->a[(char)Final.size()-1].push_back(i);
				*it = (char)Final.size()-1;


				it_end = graph->a[i].end();

			}
			else if(num1_top + 2 < num2_y && num2_y < num1_bottom - 2 ){
				if(fabs(num1_left - num2_right) < 6 || 
					fabs(num2_left - num1_right) < 6)
					continue;

				if(num2_left > num1_right){
					y1 = num1_right;
					y2 = num2_left;
				}
				else{
					y1 = num2_right;
					y2 = num1_left;
				}


				Final.push_back(Room(y1, y2, num2_y - 5, num2_y + 5));
				graph->a.push_back({});

				graph->addEdge(*it, (char)Final.size()-1);
				graph->a[*it].remove(i);

				graph->a[(char)Final.size()-1].push_back(i);
				*it = (char)Final.size()-1;


				it_end = graph->a[i].end();

			}
		}
	}

	//Этап 2 Соединяю сопряжённые по краям комнаты

	for(int i = 0; i < help; i++){
		auto it_end = graph->a[i].end();
		for(auto it = graph->a[i].begin(); it != it_end; ++it){
			if(*it >= help){
				continue;
			}
			if(num1_left + 3 < num2_left && num2_left < num1_right - 3 ){
				if(fabs(num1_top - num2_bottom) < 6 || 
					fabs(num2_top - num1_bottom) < 6)
					continue;

				if(num2_top > num1_bottom){
					y1 = num1_bottom;
					y2 = num2_top;
				}
				else{
					y1 = num2_bottom;
					y2 = num1_top;
				}


				Final.push_back(Room((num2_left+num1_right)/2 - 5, (num2_left+num1_right)/2 + 5, y1, y2));
				graph->a.push_back({});

				graph->addEdge(*it, (char)Final.size()-1);
				graph->a[*it].remove(i);

				graph->a[(char)Final.size()-1].push_back(i);
				*it = (char)Final.size()-1;


				it_end = graph->a[i].end();

			}
			else if(num1_top + 3 < num2_bottom && num2_bottom < num1_bottom - 3 ){
				if(fabs(num1_left - num2_right) < 6 || 
					fabs(num2_left - num1_right) < 6)
					continue;

				if(num2_left > num1_right){
					y1 = num1_right;
					y2 = num2_left;
				}
				else{
					y1 = num2_right;
					y2 = num1_left;
				}


				Final.push_back(Room(y1, y2, (num2_bottom+num1_top)/2 - 5, (num2_bottom+num1_top)/2 + 5));
				graph->a.push_back({});

				graph->addEdge(*it, (char)Final.size()-1);
				graph->a[*it].remove(i);

				graph->a[(char)Final.size()-1].push_back(i);
				*it = (char)Final.size()-1;


				it_end = graph->a[i].end();

			}
		}
	}
	
	// Финальный этап. Тут я ебусь с диагональными коридорами
	
	

printf("Done %d", n);

}