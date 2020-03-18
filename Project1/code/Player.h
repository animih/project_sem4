#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include "Entity.h"

class Player: public Entity{
protected:
public:
	Player(std::string F, float X, float Y, float W, float H);
	void update(float time, sf::RenderWindow *window);
};

Player::Player(std::string F, float X, float Y, float W, float H) : Entity::Entity(F, X, Y, W, H) {
};

void Player::update(float time, sf::RenderWindow *window)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);//считывает координаты курсора
	sf::Vector2f pos = window->mapPixelToCoords(pixelPos);//переводит в абсолютную систему координат
	this->setAngle((pos.x - this->x), (pos.y - this->y));
	this->sprite.setRotation(this->getAngleD());//поворот стрелки

	speed = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		this->speed = this->initspeed;
		this->dY = -1;
	};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->speed = this->initspeed;
		this->dX = -1;
	};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		this->speed = this->initspeed;
		this->dY = 1;
	};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->speed = this->initspeed;
		this->dX = 1;
	};
	if ((dX!=0)&&(dY!=0)) { this->speed = this->speed / sqrt(2); };
	this->dx = this->dX * speed * time;
	this->dy = this->dY * speed * time;
	this->dX = this->dx;
	this->dY = this->dy;
	this->x += this->dX;
	this->y += this->dY;
	this->dX = 0; this->dY = 0;
};
