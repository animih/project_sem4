#ifndef STATE_H
#define STATE_H

#include "../Entites/Entity.h"
#include <stack>


/*

	Это абстрактный класс состояний.
	Общие методы для всех состояний - это
	рендер, апдейт (стандартная зрень вообще для всех классов)
	проверка на выход и последняя опреация (что делает состояния при выходе)

*/

class State{

	protected:
		sf::RenderWindow * window;
		std::vector <sf::Texture> textures;
		bool quit = false;

		sf::Vector2i mousePosScreen;
		sf::Vector2i mousePosWindow;
		sf::Vector2f mousePosView;

		std::stack<State *> * states;

	public:
		State(sf::RenderWindow * window, std::stack<State *> * states);
		virtual ~State();

		const bool & getQuit() const;

		virtual void checkForQuit();
		virtual void endState() = 0;
		virtual void updateMousePositions();
		virtual void updateKeybinds(const float & dt) = 0;
		virtual void update(const float & dt) = 0;
		virtual void render(sf::RenderWindow * window = NULL) = 0;
};

#endif