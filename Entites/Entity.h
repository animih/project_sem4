#ifndef ENTITY_H
#define ENTITY_H

#include "../Components/Component.h"
#include <iostream>
#include <string>
#include <cmath>


class GameState;

class Entity
{
friend class GameState;
protected:
	int w, h;
	std::string * name;
	sf::Image * image;
	sf::Texture * texture;
	sf::Sprite * sprite;
public:
	bool exists = 1;
	double x, y;
	int damage_taken = 0;
	sf::FloatRect * collisionbox;
	virtual void update(const float &dt) = 0;
	virtual void render(sf::RenderWindow * window) = 0;
	virtual void react(Entity *);
	virtual void respawn();

	Entity(std::string F, double X, double Y, int W, int H);
	Entity();
	~Entity();

	const bool & if_exists() const;

};

/*

Это класс игрока

*/

class Player: public Entity{
protected:
	Map * map;
	sf::Vector2f * mousePosView;
	sf::Font font;
	double angle;

	//Components
	Movement * movement;
	Health * hp_armor;
	Stab * damage_1;
	Slash * damage_2;
	AnimationComponent * animation;

public:
	~Player();
	Player(std::string F, float X, float Y, float W, float H, Map * map, sf::Vector2f * mousePosView, std::list<Entity *> * mobs, GameState * gamestate);
	void update(const float &dt);
	void render(sf::RenderWindow * window);
	void setAngle(int dx, int dy);
	void control();

};
/*

Это класс зомби.

*/


class Zombie:public Entity{
protected:
	Map * map;
	Movement * movement;
	Health * hp_armor;
	AnimationComponent * animation;

	Tactics * tactics;

	double angle;
	bool seen = 0;
	double Rotate_speed = 0;
	double Max_Rotate_speed;

	bool hit = 0;

	double timer = 0;

	std::pair<double, double> dest;
	std::pair<int, int> dir;

	Stab * damage;

public:
	~Zombie();
	Zombie(std::string F, float X, float Y, float W, float H, Map * map, std::list<Entity *> * targets, GameState * gamestate, Tactics *);
	void react(Entity * entity);
	void update(const float &dt);
	void render(sf::RenderWindow * window);
	void setAngle(int dx, int dy);

};

/*
class Ward:public Entity{
protected:
	std::list<Entity *> * target_list;
	std::list<Entity *> mobs;

	Health * hp_armor;
	AnimationComponent * animation;

	double angle;
	bool shoot;

public:
	~Ward();
	Ward(std::string F, float X, float Y, float W, float H, Map * map, Entity * player, GameState * gamestate);
	void react(Entity * entity);
	void update(const float &dt);
	void render(sf::RenderWindow * window);
	void setAngle(int dx, int dy);


}

class Missle:public Entity{
protected:
	std::list<Entity *> * target_list;

	double dir_x;
	double dir_y;
	double speed;

	Movement * movement;
public:
	Missle(double dir_x, double dir_y, double speed, );
	~Missle();
	void update(const float &dt);
	void render(sf::RenderWindow * window);
}
*/
/*

	Это лампа, переменная is_on - отвечает за состояния (включена/выключена)

	а перменная bad_cond... Ну... В рогалике и лампы случайные )
	Некоторые из них много мерцают при переключении (bad_cond = true)

*/
/*
class Lantern:public Entity{
	protected:
		Map * map;
	public:
		~Lantern();
		Lantern(std::string F, float X, float Y, float W, float H, Map *);
		void update(const float &dt);
		void render(sf::RenderWindow * window);

};


*/

#endif