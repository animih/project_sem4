#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../States/PauseState.h"


class GameState : public State{

	Map * map;
	sf::View * view;

	Player * player;
	Tactics * tactics;

	std::list<Entity *> active_mobs;
	std::list<Entity *> active_characters;
	std::list<Entity *> objects;

	bool next_level = 0;

	std::map<std::string, std::vector<double>> buf;

	sf::Font font;

	public:
		GameState(sf::RenderWindow * window, std::stack<State *> * states, bool test = 0);
		~GameState();

		void update(const float & dt);
		void update();
		void render(sf::RenderWindow * window = NULL);

		void updateKeybinds(const float & dt);
		void endState();
		bool collision_check(double x, double y);
};

#endif