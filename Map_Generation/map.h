#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include "time.h"
#include <stdlib.h>

#define num_in_x (list->Final[index].x_right+list->Final[index].x_left)/2
#define num_in_y (list->Final[index].y_top+list->Final[index].y_bottom)/2

#define num_u_x (list->Final[u].x_right+list->Final[u].x_left)/2
#define num_u_y (list->Final[u].y_top+list->Final[u].y_bottom)/2


// Вот сюда нужно писать граничные размеры карты

#define WIDTH 6*640
#define HEIGHT 6*480

using namespace sf;

// Класс для графа

class Graph{
	int V;
	public:
		std::vector < std::list < int > > a;
		Graph(int V);
		void addEdge(int u, int w);
};

class RoomList;

// Класс для комнаты

class Room{

	double x_left, x_right, y_top, y_bottom;
	double speedx, speedy;

	int num_neighbors;
	int tile_size;

	friend class RoomList;
	friend class Map;
	friend class Player;

	int color = 0; // Маркер от 1 до 5 по котором происходит генерация окружения в комнате

	public:
	Room(double x_left, double x_right, double y_top, double y_bottom, int tile_size);
	void move(float time); // ф-ция для расталкивания комнат
	void upd(RenderWindow * window); // прорисовка

};

void pyaniy_polyarnik(int radius, double* coord);
void random_size(int average, int* size, int tile_size);


class RoomList{

	std::list<Room> list; // Первоначальный двусвязанный список, хранящий комнаты (последовательный перебор)
	std::vector<Room> Final; // Окончательный вектор из комнат (быстрый досутп по индексу)
	Graph * graph; // Граф, в котором комнаты - узлы, их связи - рёбра

	int tile_size;
	int average;

	friend class Player;
	friend class Map;

	public:
		RoomList();
		RoomList(int tile_size);
		int help = -1; // Просто полезная переменная
		void generate_rooms(int radius, int average, int total_number); // Генерация комнат в окружности
		bool push_rooms(float time); // Расталкивание комнат
		void update(RenderWindow * window); // Прорисовка всез комнат
		bool is_collide(std::_List_iterator<Room> room1, std::_List_iterator<Room>room2);
		void TriEdges(); // Триангуляция Делоне
		void DrawEdges(RenderWindow * window); // Прорисовка Триангуляции Делоне
		void BuildTree(); // Алгоритм построения остового дерева
		void DrawTree(RenderWindow * window); // Прорисовка дерева
		int * transpose_to_matrix(); // Переводим наши комнаты в матрицу
		void AddWalkRooms(); // Добавляем комнаты на пересечении с рёрами
		~RoomList(){
			list.clear();
			Final.clear();
			free(graph);
		};


};


//Класс для карты:

/*

	Тайлы хранятся в одномерном массиве
	доступ к элементу с координатами x, y :
	a[y+SIZE_y*X] (SIZE_y = HEIGHT/tile_size)
	Нумерация идёт вправо-вниз (как коорды в сфмл)

	метод intersec возвращает тайл на пересечении
	с координатами x, y
	(окргуление присутсвует)

	make_map запускает все методы в нужном порядке
	на вход:
	1)пустой массив (размера 2) куда будут
	записаны размеры тайловой матрицы
	2)Размер одного тайла (я тести с 16)
	3)Радиус распределения комнат(~100-150)
	4)Средний размер однйо комнаты(125 норм) 
	5)Число комнат
	6)Экран для прорисовки анимации
	на выходе получается массив double (размера 2)
	с координатами игрока

	update_all - прорисовка всей карты сразу

	Gen_sur отвечает за генерацию окружения в карте.
	Подробное описание работы читай в rooms_second.cpp

*/

class Map{

	int tile_size; // размер единичного тайла
	int * a;
	RoomList * Rooms;

	friend class RoomList;

	public:
		Map(int);
		int intersec(double x, double y);
		void update_all(RenderWindow * window);
		void build_Hallways();
		void Gen_Sur();
		double * make_map(int * size, int tile_size, int radius, int average, int number, RenderWindow *);
		~Map(){
			free(this->a);
			//delete Rooms;
		};

};