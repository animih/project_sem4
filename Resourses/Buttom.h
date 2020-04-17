#ifndef BUTTON_H
#define BUTTON_H

#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <cmath>
#include <list>

// Свободна - 0, Наведён крусор - 1, Нажата - 0

class Buttom{

	unsigned short int button_state;

	sf::RectangleShape shape;
	sf::Font * font;
	sf::Text text;

	sf::Color idleColor;
	sf::Color hoverColor;
	sf::Color activeColor;

	public:
		Buttom(float x, float y, float width, float height, sf::Font* font, std::string text,
		 sf::Color idleColor = sf::Color(0, 0, 0, 255), sf::Color hoverColor = sf::Color(0, 0, 0, 255),
		  sf::Color activeColor = sf::Color(0, 0, 0, 255));
		Buttom();
		~Buttom();
		void update(const sf::Vector2f mousePos);
		const bool isPressed() const;

		void render(sf::RenderWindow * window);

};




# endif