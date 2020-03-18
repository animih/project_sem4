#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>

class Entity
{
protected:
	float width, height, initspeed;
	float angle;
	float dX, dY;
public:
	int w, h;
	float speed;//текущая скорость
	sf::FloatRect collisionbox;//границы персонажа (квадрат)
	float x, y, dx ,dy;//координаты и последние их изменение
	std::string file;//файл с картинкой
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	Entity(std::string F, float X, float Y, float W, float H);
	Entity();
	void update(float time, sf::RenderWindow window);//меняет х и у, но не перемещает спрайт и не рисует 
	void setAngle(int dx, int dy);//задает угол поворота
	float getAngleR();//возвращает угол поворота в радианах
	float getAngleD();//возвращает угол поворота в градусах
	void setSpeed(float speed);//меняет 
	void setCollision(int x, int y, int w, int h);
};

Entity::Entity(std::string F, float X, float Y, float W, float H) {
	this->speed = 0; this->initspeed = 0;
	this->angle = 0;
	this->dx = 0; this->dy = 0;
	this->file = F;
	this->width = W; this->height = H;
	this->w = 16; this->h = 16;
	image.loadFromFile(file);
	image.createMaskFromColor(sf::Color(0, 0, 255));
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	this->x = X; this->y = Y;
	sprite.setTextureRect(sf::IntRect(0, 0, width, height));
	sprite.setOrigin(this->width / 2, this->height / 2);
};
Entity::Entity() {
	Entity(" ", 0, 0, 0, 0);
};
void Entity::update(float time, sf::RenderWindow window)
{
};
void Entity::setAngle(int dx, int dy) {
	this->angle = (atan2(dy, dx));
};
float Entity::getAngleR() {
	return (this->angle);
};
float Entity::getAngleD() {
	return (this->angle * 180 / 3.14159265);
};
void Entity::setSpeed(float speed) {
	this->speed = speed;
	this->initspeed = speed;
};
void Entity::setCollision(int x, int y, int w, int h) {
	collisionbox = sf::FloatRect(x - w / 2, y - h / 2, w, h);
};