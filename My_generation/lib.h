#include <SFML/Graphics.hpp>
#include <list>



using namespace sf;

const int WIDTH = 840;
const int HEIGHT = 680;

// Это класс комнаты с базовыми функциями движения и прорисовки

// Коориданты комнаты я храню как крайние точки прямоугольников

/* Нужно опнимать, что сокрости здесь - чисто номинальные переменные для движения,
писать полноценные дифуры на ускорения - затратная задача*/

class Room{

	double x_left, x_right, y_top, y_bottom;
	double speedx, speedy;

	int num_neighbors; // Просто для нормировки скорсоти при расталкивании

	friend class RoomList; // Ну а куда в мире без друзей?

	public:
	Room(double x_left, double x_right, double y_top, double y_bottom);
	void move(float time);
	void upd(RenderWindow * window);

};

// Функции, исопльзуемые для генерации комнат

void pyaniy_polyarnik(int radius, double* coord);
void random_size(int average, int* size);

// В чём я храню граф:

class Graph{
	char V;
	public:
		std::vector < std::list < char > > a;
		Graph(char V);
		void addEdge(char u, char w);
};

class RoomList{

	std::list<Room> list; // Первоначальный двусвязанный список, хранящий комнаты (последовательный перебор)
	std::vector<Room> Final; // Окончательный вектор из комнат (быстрый досутп по индексу)
	Graph * graph; // Граф, в котором комнаты - узлы, их связи - рёбра

	int average;

	public:

		void generate_rooms(int radius, int average, int total_number);
		bool push_rooms(float time);
		void update(RenderWindow * window);
		bool is_collide(std::_List_iterator<Room> room1, std::_List_iterator<Room>room2);
		void TriEdges();
		void DrawEdges(RenderWindow * window);
		void BuildTree();
		void DrawTree(RenderWindow * window);
		void AddWalkRooms();

};