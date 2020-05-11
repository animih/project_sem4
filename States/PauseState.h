#ifndef PAUSE_H
#define PAUSE_H

#include "../States/State.h"
#include "../Resourses/Buttom.h"

class PauseState : public State{

	sf::Font font;

	Buttom * continue_btm;
	Buttom * exit_btm;

	State * anc;


	public:
		PauseState(sf::RenderWindow * window, State * anc, std::stack<State *> * states, double x_coord, double y_coord);
		~PauseState();

		void update(const float & dt);
		void update();
		void render(sf::RenderWindow * window = NULL);

		void updateKeybinds(const float & dt);
		void endState();

};

#endif