
#include "MainMenu.h"


MainMenuState::MainMenuState(sf::RenderWindow * window, std::stack<State *> * states) : State(window, states){

	this->background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	this->background.setFillColor(sf::Color::Black);
	if(!this->font.loadFromFile("Resourses/Old-Japanese.ttf")){
		throw("Could not load font");
	}

	double x = window->getSize().x/2-150/2;
	double y = window->getSize().y/5*2-50/2;

	this->gamestate_btm = new Buttom(x, y, 190, 50, &this->font, "Start the Game",
		sf::Color(65, 65, 65, 150), sf::Color(65, 65, 65, 206), 
		sf::Color(95, 85, 75, 250));

	this->teststate_btm = new Buttom(x, y+120, 190, 50, &this->font, "TestGround",
		sf::Color(65, 65, 65, 150), sf::Color(65, 65, 65, 206), 
		sf::Color(95, 85, 75, 250));



}

MainMenuState::~MainMenuState()
{


	delete this->gamestate_btm;

}

void MainMenuState::update(const float & dt){

	this->gamestate_btm->update(this->mousePosView);
	this->teststate_btm->update(this->mousePosView);

	this->updateKeybinds(dt);
	this->updateMousePositions();

}



void MainMenuState::render(sf::RenderWindow * window){

	window->draw(this->background);
	this->gamestate_btm->render(window);
	this->teststate_btm->render(window);

}


void MainMenuState::updateKeybinds(const float & dt){
	this->checkForQuit();

	static int i = 0;
	i = (i+1)%1000;

	srand(i);
	if(gamestate_btm->isPressed()){
		this->states->push(new GameState(this->window, states)); // New game
	}
	if(teststate_btm->isPressed()){
		this->states->push(new GameState(this->window, states, true)); // New TestGround
	}

}

void MainMenuState::endState(){

	std::cout << "Ending MainMenuState!" << "\n";

}