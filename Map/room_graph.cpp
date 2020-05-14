#include "map.h"



Room::Room(double x_left, double x_right, double y_top, double y_bottom, int tile_size){
	this->x_left = x_left;
	this->x_right = x_right;
	this->y_top = y_top;
	this->y_bottom = y_bottom;

	this->tile_size = tile_size;
}
void Room::move(float time){

	if(x_left < 0 || x_right > WIDTH){
		speedx = - speedx; // Упругий удар о стенки
	}
	if(y_top < 0 || y_bottom > HEIGHT){
		speedy = - speedy; // Упругий Удар о стенки
	}

	x_left += 0.05*time*speedx/num_neighbors;
	x_right += 0.05*time*speedx/num_neighbors;

	y_top += 0.05*time*speedy/num_neighbors;
	y_bottom += 0.05*time*speedy/num_neighbors;

	speedx = 0;
	speedy = 0;

	num_neighbors = 1;

}

/*
	Генерация окружения.

	Тут идея очень проста, у каждой комнаты есть свой цвет-маркер,
	который раздавался так, чтобы не было двух соседних комнат
	одного цвета (для разнообразия игрового процесса)

	Изначально было такое распределение:

	1- Синий цвет. Генерация водоёмов, колон и, возможно, 
	мобов  в засаде
	2 - Зелёный цвет. Генерация травы, колон и, возможно, мобов  в засаде
	3 - Жёлтый цвет - абсолютно рандомная генерация.
	4 - Комнаты, представляющие наибольшую опасность:
		варды/мобы
	5 - Комнаты с числом соседних > 3. Важные узлы
	6 - комнаты с числом соседних = 1. Потенциальные тайные/закрытые комнаты.

*/

void Room::upd(RenderWindow * window){

	RectangleShape room(sf::Vector2f(x_right-x_left, y_bottom-y_top));
	room.setPosition(x_left, y_top);	
	room.setOutlineThickness(2);
	switch(color){
		case 1:
			room.setFillColor(sf::Color::Blue);
			break;
		case 2:
			room.setFillColor(sf::Color::Green);
			break;
		case 3:
			room.setFillColor(sf::Color::Yellow);
			break;
		case 4:
			room.setFillColor(sf::Color::Red);
			break;
		case 5:
			room.setFillColor(sf::Color(177, 177, 177));
			break;
		case 6:
			room.setFillColor(sf::Color(77, 77, 77));
			break;
	}
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

void random_size(int average, int* size, int tile_size){

	static std::default_random_engine generator;
	std::normal_distribution<double> distribution((float)average, (float)average/3.2);

	size[0] = int(distribution(generator));
	size[1] = 0;

	while(size[1]*3.5<size[0] || size[0]*3.5 < size[1])
		size[1] = int(distribution(generator));

	size[0] = int(round(size[0]))/tile_size*tile_size*1.;
	size[1] = int(round(size[1]))/tile_size*tile_size*1.;

	if(size[0] == 0){
		size[0] = tile_size;
	}

	if(size[1] == 0){
		size[1] = tile_size;
	}


}

Graph::Graph(int V){
	this->V = V;
	for(int i =0; i < V; i++)
	a.push_back({});
}
void Graph::addEdge(int u, int v){
	a[u].push_back(v);
	a[v].push_back(u);
}
