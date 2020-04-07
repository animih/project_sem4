# include "State.h"

State::State(sf::RenderWindow * window, std::stack<State *> * states){

	this->window = window;
	this->states = states;

};

State::~State(){



};

// Борьба с дребезгом

void State::checkForQuit(){
	static bool exit = 0;

	int i = 0;


	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		exit =1;
	}
	else if(exit){
		while(i < 100)
			i+= 1;
		this->quit = true;
		exit = 0;
	}



}

const bool & State::getQuit() const{
	return this->quit;
}

void State::updateMousePositions(){

	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));


}