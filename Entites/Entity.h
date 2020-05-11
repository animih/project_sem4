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
friend class Movement;
protected:
	int w, h;
	std::string * name;
	sf::Image * image;
	sf::Texture * texture;
	sf::Sprite * sprite;
public:
	std::map<std::string, Component *> components;
	bool exists = 1;
	double x, y;
	double angle;
	int damage_taken = 0;
	sf::FloatRect * collisionbox;
	virtual void update(const float &dt) = 0;
	virtual void update() = 0;
	virtual void render(sf::RenderWindow * window) = 0;
	virtual void react(Entity *);

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

	double timer_1 = 0;
	double timer_2 = 0;
	double dash_duration;
	double dash_kd;
	double cast_kd;
	bool is_dashing = 0;
	bool is_casting = 0;

	AnimationComponent * animation;

public:
	~Player();
	Player(std::string F, float X, float Y, float W, float H, Map * map, sf::Vector2f * mousePosView, std::list<Entity *> * mobs, GameState * gamestate);
	void update(const float &dt);
	void update();
	void render(sf::RenderWindow * window);
	void renderUI(sf::RenderWindow * window);
	void setAngle(int dx, int dy);
	void control();

};
/*

Это класс зомби.

*/


class Zombie:public Entity{
protected:
	Map * map;

	AnimationComponent * animation;

	Tactics * tactics;

	bool seen = 0;
	double Rotate_speed = 0;
	double Max_Rotate_speed;

	bool hit = 0;


	double shock_timer = 0;

	double timer = 0;

	std::pair<double, double> dest;
	std::pair<int, int> dir;

public:
	~Zombie();
	Zombie(std::string F, float X, float Y, float W, float H, Map * map, std::list<Entity *> * targets, GameState * gamestate, Tactics *);
	void react(Entity * entity);
	void update(const float &dt);
	void update();
	void render(sf::RenderWindow * window);
	void setAngle(int dx, int dy);

};

class Lantern:public Entity{
	protected:
		Map * map;
		bool is_on;
		bool bad_cond;
		bool is_key_pressed = 0;
		int i = 0;
	public:
		~Lantern();
		Lantern(std::string F,float X, float Y, float W, float H, Map *);
		void react(Entity * entity);
		void update(const float &dt);
		void update();
		void render (sf::RenderWindow * window);


};

class Ward:public Entity{
protected:
	std::list<Entity *> * target_list;
	std::list<Entity *>  * mobs;

	AnimationComponent * animation;
	Map * map;
	GameState * gamestate;

	Entity * player;

	bool shoot = 0;
	double timer = 0;

public:
	~Ward();
	Ward(std::string F, float X, float Y, float W, float H, Map * map, std::list<Entity *> * targets, std::list<Entity *> * mobs, GameState * gamestate, Entity * player);
	void react(Entity * entity);
	void update(const float &dt);
	void update();
	void render(sf::RenderWindow * window);
	void setAngle(int dx, int dy);


};

class Missle:public Entity{
protected:
	std::list<Entity *> * target_list;
	std::list<Entity *>  * mobs;
	Entity * player;

	double dir_x;
	double dir_y;
	double speed;

	bool deflected = 0;

	Map * map;

	Movement * movement;
public:
	Missle(std::string F, float X, float Y, double angle, double speed, float W, float H, Map * map, std::list<Entity *> * targets, GameState * gamestate, Entity * player, std::list<Entity *>  * mobs);
	~Missle();
	void update(const float &dt);
	void update();
	void render(sf::RenderWindow * window);
};



#endif