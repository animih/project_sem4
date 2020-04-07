#ifndef GAMESTATE_H
#define GAMESTATE_H

# include "State.h"

class GameState : public State{

	Entity * player;
	Map * map;
	sf::View * view;

	std::vector<Entity *> mobs = {};
	std::vector<double> buf = {};


	public:
		GameState(sf::RenderWindow * window, std::stack<State *> * states);
		~GameState();

		void update(const float & dt);
		void render(sf::RenderWindow * window = NULL);

		void updateKeybinds(const float & dt);
		void endState();
};

#endif