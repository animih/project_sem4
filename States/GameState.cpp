
#include "GameState.h"


GameState::GameState(sf::RenderWindow * window, std::stack<State *> * states, bool test) : State(window, states){

	map = new Map(32);

	double * a = new double[2];
	int * b = new int[2];
	
	if(!test){
		a = map->make_map(b, 32, 90, 385, 65, window, buf);
		tactics = new Tactics(map);
		std::string hero = "./Resourses/Player.png";
		player = new Player(hero, a[0], a[1], 64, 64, map, &mousePosView, &active_mobs, this);
		active_characters.push_back(player);
	}
	else{
		map->make_test_map(32);
		tactics = new Tactics(map);
		a[1] = 264;
		a[2] = 264;
		std::string hero = "./Resourses/Player.png";
		player = new Player(hero, a[0], a[1], 64, 64, map, &mousePosView, &active_mobs, this);
		active_characters.push_back(player);
		active_mobs.push_back(new Zombie("./Resourses/Zombie.png", 364, 364, 64, 64, map, &active_characters, this, tactics));
		active_mobs.push_back(new Zombie("./Resourses/Zombie.png", 664, 664, 64, 64, map, &active_characters, this, tactics));
	}

	view = new View();
	view->reset(sf::FloatRect(0, 0, 640*2, 480*2));

}

GameState::~GameState()
{

	delete this->map;

	for(auto chr : active_characters)
		delete chr;

	for(auto mob : active_mobs)
		delete mob;
	active_mobs.clear();
	
	for(auto obj : objects)
		delete obj;

	objects.clear();

}

void GameState::update(const float & dt){

	this->updateKeybinds(dt);
	this->updateMousePositions();

	// Обновляем объекты

	//zprintf("%f \n", dt);

	for(auto chr : active_characters){
		chr->update(dt);
	}

	for(auto mob : active_mobs){
		mob->update(dt);
	}

	for(auto obj : objects){
		obj->update(dt);
		obj->respawn();
	}

	// Удаляем недействительные

	auto it_end = active_mobs.end();

	for(auto it = active_mobs.begin(); it != it_end;){
		if((*it)->if_exists()){
			++it;
			continue;
		}
		auto it_del = it;
		++it;
		delete (*it_del);
		objects.erase(it_del);
		it_end = active_mobs.end();
	}

	it_end = objects.end();

	for(auto it = objects.begin(); it != it_end;){
		if((*it)->if_exists()){
			++it;
			continue;
		}
		auto it_del = it;
		++it;
		delete (*it_del);
		objects.erase(it_del);
		it_end = active_mobs.end();
	}

}



void GameState::render(sf::RenderWindow * window){


	for(auto chr : active_characters){
		for(auto mob : active_mobs){
			mob->react(chr);
		}
	}
	tactics->refresh();

	view->setCenter(player->x, player->y);
	window->setView(*view);

	this->map->render_region(window, player->x-window->getSize().x, player->x+window->getSize().x, player->y-window->getSize().y, player->y+window->getSize().y, true);
	this->player->render(window);

	for(auto mob : active_mobs){
		mob->render(window);
	}

	for(auto obj : objects){
		obj->render(window);
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

bool GameState::collision_check(double x, double y){

	for(auto chr : active_characters){
		if(chr->collisionbox->contains(x, y)){
			return 1;
		}
	}

	for(auto mob : active_mobs){
		if(mob->collisionbox->contains(x, y))
			return 1;
	}

	for(auto obj : objects){
		if(obj->collisionbox->contains(x, y))
			return 1;
	}


	return 0;
}