#pragma once

#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <random>
#include <iostream>
#include <algorithm>
#include <list>
#include <functional>
#include <SFML/Graphics.hpp>


#define num_in_x (list->Final[index].x_right+list->Final[index].x_left)/2
#define num_in_y (list->Final[index].y_top+list->Final[index].y_bottom)/2

#define num_u_x (list->Final[u].x_right+list->Final[u].x_left)/2
#define num_u_y (list->Final[u].y_top+list->Final[u].y_bottom)/2


// Вот сюда нужно писать граничные размеры карты

#define WIDTH 15*640
#define HEIGHT 15*480

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
		void transpose_to_matrix(std::vector<std::list <char>> & a); // Переводим наши комнаты в матрицу
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

	render_all - прорисовка всей карты сразу

	Gen_sur отвечает за генерацию окружения в карте.
	Подробное описание работы читай в rooms_second.cpp

*/

/*

	Новый коммит, новые 500 000 методов.
	На самом деле я их писал для удобства, хоть с виду не скажешь.
	Если вам нужно убдет потребовать какую-нибудь зеротень от карты,
	почти с большей веротяностью нужный метод уже есть.


	render_region - отвечает за прорисовку карты ТОЛЬКО в указанном диапозоне (например в размерах окна)
	там есть один парамтер, который по дфефлоту отмечен false, т.к. вот:
	если передать true, о прорисовка будет происходить с учётом масок обзора и света

	маска обзора представляет из себя отображение из инт в бул (инт - это две координаты записанные в виде x+y*n)
	очень быстро могу проверить в ф-ции рендера видима ли клетка (и не рендерить её, если что)

	маска света - это тоже самое, что маска обзора, но вместо булевских значений там параметры прозрачности
	очень эффектная штука!

	update_view_mask() - обновляет маску игрока для обзора

	update_player_lighting_mask() - обновляет маску игрока для света (собственное поле зрение)
	должна вызываться в цикле, т.к. игрок перемезается постоянно

	update_env_lighting_mask() - обновляет маску света окружения. Вызывается только при изменении этого самого окружения
	Простыми словами, я у экономлю ресурс вызываю метод только при включении/выключении лампы.
	параметр is_on - переменная обозначающая тип операции
	is_on = true - рисую область света видимый свет для источника
	is_on = true - убираю из области видимый свет для источника
	Совсем простыми словами - я передаю сюда переменную обозначающую включена или выключена лампа. Чтобы быстро можно было посчитать изменение в текущей маске.


*/

class Tactics;

class Map{

	friend class Tactics;

	int tile_size; // размер единичного тайла
	std::vector<std::list <char>> a = {}; 
	RoomList * Rooms;

	std::map<int, bool> render_mask;
	std::map<int, int> player_lighting_mask;
	std::map<int, int> env_lighting_mask;

	std::map<int, bool> tactics_for;


	friend class RoomList;
	friend void lead_path(Map * map, const double & start_x, const double & start_y, const double & end_x, const double & end_y, std::vector<int> & path);

	public:
		Map(int);
		int intersec(const double & x, const double & y);
		void render_all(RenderWindow * window);
		void render_region(RenderWindow * window, double x_left, double x_right, double y_top, double y_bottom, bool mask = false);
		void build_Hallways();
		void Gen_Sur(std::map<std::string, std::vector<double>> & buf);
		double * make_map(int * size, int tile_size, int radius, int average, int number, RenderWindow *, std::map<std::string, std::vector<double>> & buf);
		void check_Spawn(std::map<std::string, std::vector<double>> & buf, const double & x, const double & y, double range);
		~Map(){
			std::vector<std::list <char>>().swap(a);
			//delete Rooms;
		};
		bool if_visible(double x1, double y1, double x2, double y2, bool seen = 0);
		void push_mob_back(int id, const double & x, const double & y);
		void update_view_mask(const double & x_coord, const double & y_coord, double radius_coord);
		void update_player_lighting_mask(const double & x_coord, const double & y_coord, double radius_coord);
		void update_env_lighting_mask(const double & x_coord, const double & y_coord, double radius_coord, bool is_on);
		void cast_light_for_view(int x, int y, int radius, int row,
		double start_slope, double end_slope, int xx, int xy, int yx,int yy);
		void cast_light_for_player(int x, int y, int radius, int row,
		double start_slope, double end_slope, int xx, int xy, int yx,int yy);
		void cast_light_for_env(int x, int y, int radius, int row,
		double start_slope, double end_slope, int xx, int xy, int yx,int yy, bool is_on);
		const bool getMask(double x, double y);
		int getLum(double x, double y);
		void reset_lighting_mask();
		void make_test_map(int tile_size);


};


class Tactics{

	Map * map;
	std::list<int> allies = {};

	public:
		Tactics(Map * map);
		~Tactics();
		void refresh();
		std::pair<int, int> lead_mob(double x1, double y1, double x2, double y2, bool walk);


};






#endif