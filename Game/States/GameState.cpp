
#include "GameState.h"


GameState::GameState(sf::RenderWindow * window, std::stack<State *> * states) : State(window, states){

	map = new Map(16);

	double * a = new double[2];
	int * b = new int[2];
	
	a = map->make_map(b, 32, 70, 355, 70, window);

	std::string hero = "heroForRotate .png";

	player = new Player(hero, a[0], a[1], 136, 74, map);
	player->setSpeed(550);


	view = new View();
	view->reset(sf::FloatRect(0, 0, 640, 480));

}

GameState::~GameState()
{

	delete this->player;
	delete this->map;

	for(int i = 0; i < mobs.size(); i++){
		delete mobs[i];
	}

	mobs.clear();
	buf.clear();

}

void GameState::update(const float & dt){

	this->updateKeybinds(dt);
	this->updateMousePositions();

	this->player->update(dt);

	this->map->check_Spawn(buf, player->x, player->y, window->getSize().x/1.5);

	for(int i = 0; i < buf.size(); i+=2){
		printf("Zombie added \n");
		mobs.push_back(new Zombie("zombieForRotate .png", buf[i], buf[i+1], 136, 74, map));
	}

	buf.clear();
	int n = mobs.size();

	for(int i = 0; i < n; i++){
		//printf("%d ", mobs.size())
		mobs[i]->React_on(player);

		if(fabs(mobs[i]->x - player->x) > window->getSize().x/1.2 && fabs(mobs[i]->y - player->y) > window->getSize().x/1.2){
			this->map->push_mob_back(-4, mobs[i]->x, mobs[i]->y);
			delete mobs[i];
			printf("deleted mob \n");
			n--;
			mobs.erase(mobs.begin()+i);

		}
	}



}



void GameState::render(sf::RenderWindow * window){

	view->setCenter(player->x, player->y);
	window->setView(*view);

	this->map->render_region(window, player->x-window->getSize().x/2, player->x+window->getSize().x/2, player->y-window->getSize().y/2, player->y+window->getSize().y/2);
	this->player->render(window);

	for(int i = 0; i < mobs.size(); i++){
		mobs[i]->render(window);
	}

}


void GameState::updateKeybinds(const float & dt){
	this->checkForQuit();

}

void GameState::endState(){


	view->reset(sf::FloatRect(0, 0, 640, 480));
	window->setView(*view);
	std::cout << "Ending Gamestate!" << "\n";

}