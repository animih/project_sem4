#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <list>

class Entity
{
protected:
	float width, height, initspeed;
	float angle;
	float dX, dY;
public:
	int w, h;
	float speed;//òåêóùàÿ ñêîðîñòü
	sf::FloatRect collisionbox;//ãðàíèöû ïåðñîíàæà (êâàäðàò)
	float x, y, dx ,dy;//êîîðäèíàòû è ïîñëåäíèå èõ èçìåíåíèå
	std::string file;//ôàéë ñ êàðòèíêîé
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	Entity(std::string F, float X, float Y, float W, float H);
	Entity();
	void update(float time, sf::RenderWindow window);//ìåíÿåò õ è ó, íî íå ïåðåìåùàåò ñïðàéò è íå ðèñóåò 
	void setAngle(int dx, int dy);//çàäàåò óãîë ïîâîðîòà
	float getAngleR();//âîçâðàùàåò óãîë ïîâîðîòà â ðàäèàíàõ
	float getAngleD();//âîçâðàùàåò óãîë ïîâîðîòà â ãðàäóñàõ
	void setSpeed(float speed);//ìåíÿåò 
	void setCollision(int x, int y, int w, int h);
	int index; // переменная для номера комнаты
	bool cheat = 0; // чит-код для ноклипа
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




