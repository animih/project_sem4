#ifndef GAME_H
#define GAME_H

#include "../States/MainMenu.h"
#include <stack>


/*

	Как устроен класс игры:
	Это главный класс, в котором хранится
	указатель на окно, что передаётся всем дургим классам

	Кроме того, в этом классе хранится стек состояний
	Обращаясь к верхнему элементу стека, я выполняю актвиное
	состояния игры, делаю для него метод рендера и апдейта

	Это очень удобно, так я могу из главного меню проипхнуть
	В стек состояния игры, а потом при выходе из игры сделать поп
	и вернуться снова в главное меню, точно также при паузе 
	(такая будет написано позже) я могу сделать поп и венуться в состояние игры
	причём все объекты остаются на месте.
	Для таких трюков мне нужно при инициализации класса состояния передать стек состояний
	чтобы я мог пропихивать состояния из других состояний.

	также в классе игры происходит единственный метод чтения времени
	переменная временная передаётся потом всем классам как референс (&)


*/

class Game{

	sf::RenderWindow * window;
	sf::Event sfEvent;

	float dt;
	sf::Clock dtClock;

	std::stack<State *> states;

	public:
		void run();
		void updateDt();
		void render();
		void update();
		void updateSFMLEvents();
		Game();
		~Game();

};

Game::Game(){

	this->window = new sf::RenderWindow(sf::VideoMode(640*2, 480*2), "RGlike v 1.0.0");
	this->states.push(new MainMenuState(this->window, &states));

}

Game::~Game(){

	while(!this->states.empty()){
		delete this->states.top();
		this->states.pop();
	}

	delete this->window;

}

void Game::run(){

	while(this->window->isOpen()){
		this->updateSFMLEvents();
		this->updateDt();
		this->render();
		this->update();

	}

}

void Game::updateSFMLEvents(){


	while(this->window->pollEvent(this->sfEvent))
	{
		if(this->sfEvent.type == sf::Event::Closed)
			this->window->close();

	}

}

void Game::update(){

	if(!this->states.empty()){
		this->states.top()->update();
		this->updateDt();
		this->states.top()->update(this->dt);

		if(this->states.top()->getQuit()){
			this->states.top()->endState();
			delete this->states.top();
			this->states.pop();
		}
	}

}

void Game::render(){

	this->window->clear();

	if(!this->states.empty()){
		this->states.top()->render(this->window);
	}


	this->window->display();

}

void Game::updateDt(){
	this->dt = this->dtClock.getElapsedTime().asSeconds();
	this->dtClock.restart();
	//printf("%f \n", dt);
}

#endif