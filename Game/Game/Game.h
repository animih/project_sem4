#ifndef GAME_H
#define GAME_H

#include "../States/MainMenu.h"
#include <stack>

class Game{

	sf::RenderWindow * window;
	sf::Event sfEvent;

	float dt;
	sf::Clock dtClock;

	std::stack<State *> states;

	public:
		void updateDt();
		void run();
		void render();
		void update();
		void updateSFMLEvents();
		Game();
		~Game();



};

Game::Game(){

	this->window = new sf::RenderWindow(sf::VideoMode(640, 480), "RGlike v 1.0.0");
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