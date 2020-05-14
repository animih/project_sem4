#ifndef PAUSE_H
#define PAUSE_H

#include "../States/State.h"
#include "../Resourses/Buttom.h"

class PauseState : public State{

	sf::Font font;
	sf::Text text;

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

class EndMessage : public State{

	sf::Font font;
	sf::Text text;

	Buttom * exit_btm;

	State * anc;

	public:
		EndMessage(sf::RenderWindow * window, State * anc, std::stack<State *> * states, double x_coord, double y_coord, std::string text);
		~EndMessage();

		void update(const float & dt);
		void update();
		void render(sf::RenderWindow * window = NULL);

		void updateKeybinds(const float & dt);
		void endState();

};

#endif