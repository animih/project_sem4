#include "PauseState.h"

PauseState::PauseState(sf::RenderWindow * window, State * anc, std::stack<State *> * states, double x_coord, double y_coord) : State(window, states){

	if(!this->font.loadFromFile("Resourses/Old-Japanese.ttf")){
		throw("Could not load font");
	}

	double x = window->getSize().x/2-150/2+x_coord;
	double y = window->getSize().y/5*2-50/2+y_coord;

	this->continue_btm = new Buttom(x, y+60, 190, 50, &this->font, "Continue",
		sf::Color(65, 65, 65, 150), sf::Color(65, 65, 65, 206), 
		sf::Color(95, 85, 75, 250));

	this->exit_btm = new Buttom(x, y+240, 190, 50, &this->font, "Main Menu",
		sf::Color(65, 65, 65, 150), sf::Color(65, 65, 65, 206), 
		sf::Color(95, 85, 75, 250));

	this->anc = anc;

}
PauseState::~PauseState(){

	delete this->continue_btm;
	delete this->exit_btm;

};

void PauseState::update(const float & dt){

	this->continue_btm->update(this->mousePosView);
	this->exit_btm->update(this->mousePosView);

	this->updateKeybinds(dt);
	this->updateMousePositions();

}
void PauseState::update(){

}
void PauseState::render(sf::RenderWindow * window){

	anc->render(window);
	this->continue_btm->render(window);
	this->exit_btm->render(window);

}

void PauseState::updateKeybinds(const float & dt){
	if(continue_btm->isPressed()){
		this->quit = true;
	}
	if(exit_btm->isPressed()){
		this->anc->quit = true;
		this->quit = true;
	}
}

void PauseState::endState(){
	//Nope.
};