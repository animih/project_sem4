#ifndef ENTITY_H
#define ENTITY_H

# include "../Map/map.h"
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

/*

Это класс игрока

*/

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

/*

Это класс зомби.
переменная seen нужно для записи информации, видел ли зомби недавно игркоа
Если он его видел и игрок пропал из поле зрения - зомби должен начать поиски.
Когда игрок отходит достаточно далеко переменная обнуляется и зомби перестаёт искать

*/


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

/*

	Это лампа, переменная is_on - отвечает за состояния (включена/выключена)

	а перменная bad_cond... Ну... В рогалике и лампы случайные )
	Некоторые из них много мерцают при переключении (bad_cond = true)

*/

class Lantern:public Entity{
	protected:
		Map * map;
		bool is_on;
		bool bad_cond;
	public:
		~Lantern();
		Lantern(std::string F, float X, float Y, float W, float H, Map *);
		void React_on(Entity * Entity);
		void update(const float &dt);
		void render(sf::RenderWindow * window);

};




#endif