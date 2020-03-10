#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include <utility>

typedef std::pair<int, int> point;

const int WIDTH = 840;
const int HEIGHT = 680;

class space{

	int width;
	int height;
	std::list <point> out;
	std::list <point> in;

	public:
		space(int height, int width);
		void gen_bnd();
		void update_out(sf::RenderWindow * window);
		void update_in(sf::RenderWindow * window);
	

};