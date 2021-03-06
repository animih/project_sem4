
#include "GameState.h"


GameState::GameState(sf::RenderWindow * window, std::stack<State *> * states, bool test) : State(window, states){
	map = new Map(32);

	double * a = new double[2];
	int * b = new int[2];

	if(!this->font.loadFromFile("Resourses/Old-Japanese.ttf")){
		throw("Could not load font");
	}
	
	if(!test){
		a = map->make_map(b, 32, 90, 385, 85, window, buf); // buf , tile_size, rad, avr_size, number
		tactics = new Tactics(map);
		std::string hero = "./Resourses/Player.png";
		player = new Player(hero, a[0], a[1], 64, 64, map, &mousePosView, &active_mobs, this);
		active_characters.push_back(player);
		if(buf.count("Zombie") != 0){
			for(int i = 0; i < buf["Zombie"].size(); i += 2){
				active_mobs.push_back(new Zombie("./Resourses/Zombie.png", buf["Zombie"][i], buf["Zombie"][i+1], 64, 64, map, &active_characters, this, tactics));
			}
		}
		if(buf.count("Lantern") != 0){
			for(int i = 0; i < buf["Lantern"].size(); i += 2){
				active_mobs.push_back(new Lantern("./Resourses/Lantern.png", buf["Lantern"][i], buf["Lantern"][i+1], 64, 64, map));
			}
		}
		if(buf.count("Ward") != 0){
			for(int i = 0; i < buf["Ward"].size(); i += 2){
				active_mobs.push_back(new Ward("./Resourses/Ward.png", buf["Ward"][i], buf["Ward"][i+1], 64, 64, map, &active_characters, &active_mobs, this, player));
			}
		}
		if(buf.count("Ladder") != 0){
			for(int i = 0; i < buf["Ladder"].size(); i += 2){
				active_mobs.push_back(new Table("./Resourses/Lader.png", buf["Ladder"][i], buf["Ladder"][i+1], 64, 64, map, &this->font, "EXIT", &next_level));
			}
		}

	}
	else{
		map->make_test_map(32);
		tactics = new Tactics(map);
		a[0] = 264;
		a[1] = 264;
		std::string hero = "./Resourses/Player.png";
		player = new Player(hero, a[0], a[1], 64, 64, map, &mousePosView, &active_mobs, this);
		active_characters.push_back(player);
		active_mobs.push_back(new Zombie("./Resourses/Zombie.png", 364, 364, 64, 64, map, &active_characters, this, tactics));
		active_mobs.push_back(new Zombie("./Resourses/Zombie.png", 664, 664, 62, 62, map, &active_characters, this, tactics));
		active_mobs.push_back(new Lantern("./Resourses/Lantern.png", 64*10, 64*10, 64, 65, map));
		active_mobs.push_back(new Ward("./Resourses/Ward.png", 64*9, 64*9, 64, 64, map, &active_characters, &active_mobs, this, player));
		active_mobs.push_back(new Table("./Resourses/Lader.png", 64*5, 64*10, 64, 64, map, &this->font, "EXIT", &next_level));
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

	delete tactics;
	buf.clear();

	objects.clear();

}

void GameState::update(const float & dt){

	this->updateKeybinds(dt);
	this->updateMousePositions();

	// Обновляем объекты

	for(auto chr : active_characters){
		chr->update(dt);
	}

	for(auto mob : active_mobs){
		mob->update(dt);
	}

	for(auto obj : objects){
		obj->update(dt);
	}

}

void GameState::update(){

	if(!player->if_exists() && !this->quit){
		this->states->push(new EndMessage(this->window, this, states, player->x-window->getSize().x/2, player->y-window->getSize().y/2, "You died"));
		this->quit = true;
	}

	if(buf.count("Boss")!= 0 && this->active_mobs.size() == 0 && !this->quit){
		this->states->push(new EndMessage(this->window, this, states, player->x-window->getSize().x/2, player->y-window->getSize().y/2, "You won"));
		this->quit = true;
	}

	map->reset_player_lighting_mask();
	for(auto chr : active_characters){
		for(auto mob : active_mobs){
			mob->react(chr);
		}
	}
	tactics->refresh();

	for(auto chr : active_characters){
		chr->update();
	}

	for(auto mob : active_mobs){
		mob->update();
	}

	for(auto obj : objects){
		obj->update();
	}

	// Удаляем отжившее

	auto it_end = active_mobs.end();

	for(auto it = active_mobs.begin(); it != it_end;){
		if((*it)->if_exists()){
			++it;
			continue;
		}
		auto it_del = it;
		++it;
		delete (*it_del);
		active_mobs.erase(it_del);
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

	// Переход на след уровень = босс файт

	if(next_level){
			printf("New level !\n");
			next_level = 0;
			double * a = new double[2];
			a = this->map->make_boss_battle(32, buf);
			this->player->x = a[0];
			this->player->y = a[1];
			for(auto mob : active_mobs)
				delete mob;
			active_mobs.clear();
	
			for(auto obj : objects)
				delete obj;
			objects.clear();

			if(buf.count("Boss")!= 0){
				for(int i = 0; i < buf["Boss"].size(); i += 2){
					active_mobs.push_back(new Boss("./Resourses/Boss2.png", buf["Boss"][i], buf["Boss"][i+1], 64, 64, map, &active_characters, &active_mobs, this, player, tactics));
				}
			}
	}

}



void GameState::render(sf::RenderWindow * window){

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

	this->player->renderUI(window);
}


void GameState::updateKeybinds(const float & dt){
	static bool exit = 0;

	int i = 0;

	// Выход в паузу
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		exit =1;
	}
	else if(exit){
		while(i < 100)
			i+= 1;
		printf("\n PauseMenu");
		this->states->push(new PauseState(this->window, this, states, player->x-window->getSize().x/2, player->y-window->getSize().y/2));
		exit = 0;
	}

}

void GameState::endState(){

	// Последнее слово...
	view->reset(sf::FloatRect(0, 0, 640*2, 480*2));
	window->setView(*view);
	std::cout << "Ending Gamestate!" << "\n";

}
/*

	Тут просто осущетсвляется проверка коллизии между объектами.

*/
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