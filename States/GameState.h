#ifndef GAMESTATE_H
#define GAMESTATE_H

# include "State.h"


class GameState : public State{

	Map * map;
	sf::View * view;

	Player * player;
	Tactics * tactics;

	std::list<Entity *> active_mobs;
	std::list<Entity *> active_characters;
	std::list<Entity *> objects;

	std::map<std::string, std::vector<double>> buf;

	public:
		GameState(sf::RenderWindow * window, std::stack<State *> * states, bool test = 0);
		~GameState();

		void update(const float & dt);
		void render(sf::RenderWindow * window = NULL);

		void updateKeybinds(const float & dt);
		void endState();
		bool collision_check(double x, double y);
};

#endif