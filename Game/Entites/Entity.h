#ifndef ENTITY_H
#define ENTITY_H

# include "../Map/map.h"
/*
#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <cmath>
#include <list>
*/
#include <iostream>
#include <string>
#include <cmath>

class Entity
{
friend class state;
protected:
	float width, height;
	float angle;
	int index;
	bool anti_cheat = 1;
public:
	int w, h;
	float speed;
	sf::FloatRect * collisionbox;
	float x, y;
	std::string * name;
	sf::Image * image;
	sf::Texture * texture;
	sf::Sprite * sprite;
	Entity(std::string F, float X, float Y, float W, float H);
	Entity();
	virtual ~Entity();
	virtual void update(const float &dt) = 0;
	virtual void render(sf::RenderWindow * window) = 0;
	void setAngle(int dx, int dy);
	float getAngleR();
	float getAngleD();
	void setSpeed(float speed);
	void setCollision(int x, int y, int w, int h);
	virtual void move(const float& dt, const float & dir_x, const float & dir_y);
	virtual void React_on(Entity * entity);
};

class Player: public Entity{
protected:
	Map * map;
public:
	~Player();
	Player(std::string F, float X, float Y, float W, float H, Map*);
	void move(const float& dt, const float & dir_x, const float & dir_y);
	void update(const float &dt);
	void render(sf::RenderWindow * window);

};



class Zombie:public Entity{
protected:
	Map * map;
	bool seen = 0;
public:
	~Zombie();
	Zombie(std::string F, float X, float Y, float W, float H, Map *);
	void React_on(Entity * entity);
	void update(const float &dt);
	void render(sf::RenderWindow * window);

};




#endif