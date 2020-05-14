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
#include <omp.h>


#define num_in_x (list->Final[index].x_right+list->Final[index].x_left)/2
#define num_in_y (list->Final[index].y_top+list->Final[index].y_bottom)/2

#define num_u_x (list->Final[u].x_right+list->Final[u].x_left)/2
#define num_u_y (list->Final[u].y_top+list->Final[u].y_bottom)/2


// Здесь задаются глобальные размеры карты

#define WIDTH 15*640
#define HEIGHT 15*480

using namespace sf;

// Класс для графа

class Graph{
	int V;
	public:
		~Graph(){};
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

	int color = 0; // Маркер от 1 до 6, по которому происходит генерация окружения

	public:
	~Room(){}
	Room(double x_left, double x_right, double y_top, double y_bottom, int tile_size);
	void move(float time); // ф-ция перемещения отдельной комнаты
	void upd(RenderWindow * window); // прорисовка

};

void pyaniy_polyarnik(int radius, double* coord);
void random_size(int average, int* size, int tile_size);


class RoomList{

	std::list<Room *> list; // Первоначальный список, в котором хранятся вобще все комнаты
	std::vector<Room *> Final; // Вектор из отобранных комнат
	Graph * graph; // Граф, репрезнтирующий подземелье

	int tile_size;
	int average;

	friend class Player;
	friend class Map;

	public:
		RoomList();
		RoomList(int tile_size);
		int help = -1; // Просто полезная переменная
		void generate_rooms(int radius, int average, int total_number); // Генерация комнат равномерно по окружности
		bool push_rooms(float time); // Расталкивание комнат
		void update(RenderWindow * window); // Прорисовка всех комнат
		bool is_collide(std::_List_iterator<Room *> room1, std::_List_iterator<Room *>room2);
		void TriEdges(); // Триангуляция Делоне
		void DrawEdges(RenderWindow * window); // Прорисовка Триангуляции Делоне
		void BuildTree(); // Алгоритм построения остового дерева
		void DrawTree(RenderWindow * window); // Прорисовка дерева
		void transpose_to_matrix(std::vector<std::list <char>> & a); // Переводим наши комнаты в матрицу
		void AddWalkRooms(); // Добавляем комнаты на пересечении с рёрами
		~RoomList(){
			for(auto obj : list){
				free(obj);
			}
			for(auto obj : Final){
				free(obj);
			}
			list.clear();
			Final.clear();
			free(graph);
		};


};


//Класс для карты:

/*

	Тайлы хранятся в одномерном векторе из списков
	доступ к элементу с координатами x, y :
	a[y+SIZE_y*X] (SIZE_y = HEIGHT/tile_size)
	Нумерация идёт вправо-вниз.
	Такая струкутра необходима для задания слоёв карты (например, чтобы разместить траву над землёй)

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

*/

class Tactics;

class Map{

	friend class Tactics;

	int tile_size; // размер единичного тайла 
	RoomList * Rooms;

	std::map<int, bool> render_mask; // Маска поле видимости
	std::map<int, int> player_lighting_mask; // Маска освещённости для любых перемещающих объектов (в основном игрока)
	std::map<int, int> env_lighting_mask; // Маска освещённости для фонарей

	std::map<int, bool> tactics_for;

	std::vector<std::list <char>> a = {}; // А здесь хранятся тайлы карты

	friend class RoomList;

	// ф-ция для построения пути
	friend void lead_path(Map * map, const double & start_x, const double & start_y, const double & end_x, const double & end_y, std::vector<int> & path);

	// Следующие 3 ф-ции изначально были методами класса, но для эффективного распаралеливания их пришлось вынести в самостоятельные ф-ции
	friend void cast_light_for_view(Map * map, int x, int y, int radius, int row, double start_slope, double end_slope, int xx, int xy, int yx, int yy);
	friend void cast_light_for_player(Map * map, int x, int y, int radius, int row, double start_slope, double end_slope, int xx, int xy, int yx, int yy);
	friend void cast_light_for_env(Map * map, int x, int y, int radius, int row, double start_slope, double end_slope, int xx, int xy, int yx, int yy, bool is_on);

	public:
		Map(int);
		int intersec(const double & x, const double & y); // Получения тайла на пересечении с объектом
		void render_all(RenderWindow * window); // Прорисовка вообще всей карты
		void render_region(RenderWindow * window, double x_left, double x_right, double y_top, double y_bottom, bool mask = false); // Прорисовка карты в пределах экрана, параметр отвечает за учёт поля видимости игрока
		void build_Hallways(); // Ф-ция для построения кордиров между комнатами
		void Gen_Sur(std::map<std::string, std::vector<double>> & buf); // ф-ция для генерации окружения в комнатах
		double * make_map(int * size, int tile_size, int radius, int average, int number, RenderWindow *, std::map<std::string, std::vector<double>> & buf); // ф-ция собирающая всю карту (написана для удобства)
		~Map(){
			std::vector<std::list <char>>().swap(a);
			render_mask.clear();
			player_lighting_mask.clear();
			env_lighting_mask.clear();

			tactics_for.clear();
		};
		bool if_visible(double x1, double y1, double x2, double y2, bool seen = 0); // Алгоритм построения отрезка, возвращает 1 если все тайлы на пересечении с отрезком - свободные для перемещения, 0 в дргуом случае
		void update_view_mask(const double & x_coord, const double & y_coord, double radius_coord); // просчёт маски видимости
		void update_player_lighting_mask(const double & x_coord, const double & y_coord, double radius_coord); // просчёт маски освещённости
		void update_env_lighting_mask(const double & x_coord, const double & y_coord, double radius_coord, bool is_on); // просчёт маски освещённости для фонарей
		
		const bool getMask(double x, double y); // Видим ли объект?
		int getLum(double x, double y); // Насколько освещён объект?
		void reset_lighting_mask(); // сбрасывает маску для статического освещения
		void make_test_map(int tile_size);
		void reset_player_lighting_mask(); // сбрасывает маску для динамического освещения

		double * make_boss_battle(int tile_size, std::map<std::string, std::vector<double>> & buf);


};

// Это просто класс построения пути для группы мобов. Такое нужно, чтобы они в зависимости от положения союзников, окружали игрока, по возможности обходя по разные стороны.

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