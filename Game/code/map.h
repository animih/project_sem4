#pragma once

#include <SFML/Graphics.hpp>
#include <list>
//#include "Entity.h"

#define num_in_x (list->Final[index].x_right+list->Final[index].x_left)/2
#define num_in_y (list->Final[index].y_top+list->Final[index].y_bottom)/2

#define num_u_x (list->Final[u].x_right+list->Final[u].x_left)/2
#define num_u_y (list->Final[u].y_top+list->Final[u].y_bottom)/2

#define WIDTH 4*640
#define HEIGHT 4*480

using namespace sf;

class Entity;

// Это класс комнаты с базовыми функциями движения и прорисовки

// Коориданты комнаты я храню как крайние точки прямоугольников

/* Нужно опнимать, что сокрости здесь - чисто номинальные переменные для движения,
писать полноценные дифуры на ускорения - затратная задача*/

class RoomList;

class Room{

	double x_left, x_right, y_top, y_bottom;
	double speedx, speedy;

	int num_neighbors; // Просто для нормировки скорсоти при расталкивании

	friend class RoomList; // Ну а куда в мире без друзей?
	friend class Player;

	friend void map_check(Entity *mob, RoomList * list);

	int color = 0;

	public:
	bool outline = 0;
	Room(double x_left, double x_right, double y_top, double y_bottom);
	void move(float time);
	void upd(RenderWindow * window);

};

// Функции, исопльзуемые для генерации комнат

void pyaniy_polyarnik(int radius, double* coord);
void random_size(int average, int* size);

// В чём я храню граф:

class Graph{
	int V;
	public:
		std::vector < std::list < int > > a;
		Graph(int V);
		void addEdge(int u, int w);
};

class RoomList{

	std::list<Room> list; // Первоначальный двусвязанный список, хранящий комнаты (последовательный перебор)
	std::vector<Room> Final; // Окончательный вектор из комнат (быстрый досутп по индексу)
	Graph * graph; // Граф, в котором комнаты - узлы, их связи - рёбра

	int average;
	
	friend void map_check(Entity *mob, RoomList * list);
	friend class Player;

	public:
		int help = -1;
		double * make_map(int , int , int, RenderWindow * window);
		void generate_rooms(int radius, int average, int total_number);
		bool push_rooms(float time);
		void update(RenderWindow * window);
		bool is_collide(std::_List_iterator<Room> room1, std::_List_iterator<Room>room2);
		void TriEdges();
		void DrawEdges(RenderWindow * window);
		void BuildTree();
		void DrawTree(RenderWindow * window);
		void AddWalkRooms();
		//void Graph_colouring();

};