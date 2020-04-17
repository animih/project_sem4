#ifndef MAINMENU_H
#define MAINMENU_H

#include "../States/GameState.h"
#include "../Resourses/Buttom.h"

/*

	Это главное меню с милой кнопкой
	Больше ничего и не требутся.

*/

class MainMenuState : public State{

	sf::RectangleShape background;
	sf::Font font;

	Buttom * gamestate_btm;
	Buttom * teststate_btm;

	public:
		MainMenuState(sf::RenderWindow * window, std::stack<State *> * states);
		~MainMenuState();

		void update(const float & dt);
		void render(sf::RenderWindow * window = NULL);

		void updateKeybinds(const float & dt);
		void endState();
};

#endif