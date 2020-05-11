#include "Entity.h"

Ward::Ward(std::string F, float X, float Y, float W, float H, Map * map, std::list<Entity *> * targets, std::list<Entity *> * mobs, GameState * gamestate, Entity * player):
Entity(F, X, Y, W, H){

	this->map = map;
	this->gamestate = gamestate;
	this->player = player;

	components["Hp"] = new Health(45, 0, this);

	this->animation = new AnimationComponent(&x, &y, &angle);

	this->target_list = targets;
	this->mobs = mobs;

	angle = rand()%180*M_PI/180;

}

Ward::~Ward(){

	delete components["Hp"];
	components.clear();
	delete animation;

}

void Ward::setAngle(int dx, int dy){
	this->angle = (atan2(dy, dx));
};


void Ward::react(Entity * entity){

	if(fabs(this->x - entity->x) < 550 && fabs(this->y - entity->y) < 550){

		if(map->if_visible(this->x, this->y, entity->x, entity->y)){

			this->setAngle(entity->x-this->x, entity->y - this->y);

			if(!shoot){

				shoot = 1;

				this->mobs->push_back(new Missle("./Resourses/Missle.png", x, y, angle, 220, 32, 32, map, target_list, gamestate, player, mobs));

			}

		}

	}


}
void Ward::update(const float &dt){

	animation->play(dt);
	
	components["Hp"]->update(dt);

	if(shoot){
		timer += dt;

		if(timer >= 0.5){
			timer = 0;
			shoot = 0;
		}

	}
	auto it_end = components.end();

	for(auto it = components.begin(); it != it_end;){
		if((*it).second->exists){
			++it;
			continue;
		}
		auto it_del = it;
		++it;
		delete (*it_del).second;
		components.erase(it_del);
		it_end = components.end();
	}

}

void Ward::update(){

	components["Hp"]->update();

}

void Ward::render(sf::RenderWindow * window){
	animation->render(window);

	if(not map->getMask(x, y)){
		return;
	}

	int a = map->getLum(x, y);
	this->sprite->setColor(sf::Color(255, 255, 255, 255));
	this->sprite->setRotation(this->angle * 180 / 3.14159265);
	this->sprite->setPosition(this->x, this->y);
    window->draw(*this->sprite);
   	
    sf::RectangleShape rect;
    rect.setOrigin(8, 8);
   	double hp = components["Hp"]->Get_hp();
   	rect.setSize(sf::Vector2f(64*hp, 16));
   	rect.setFillColor(sf::Color(14, 220, 60, 82*a/255));
   	rect.setPosition(sf::Vector2f(this->x-64/2+16/2, this->y-32));

   	window->draw(rect);

   	rect.setSize(sf::Vector2f(64-64*hp, 16));
	rect.setFillColor(sf::Color(224, 20, 10, 82*a/255));
   	rect.setPosition(sf::Vector2f(this->x-64/2+64*hp+16/2, this->y-32));

   	window->draw(rect);


};

Missle::Missle(std::string F, float X, float Y, double angle, double speed, float W, float H, Map * map, std::list<Entity *> * targets, GameState * gamestate, Entity * player, std::list<Entity *> * mobs):
Entity(F, X, Y, W, H){

	this->player = player;
	this->mobs = mobs;

	this->target_list = targets;
	this->map = map;

	this->dir_x = cos(angle);
	this->dir_y = sin(angle);
	//this->sprite->setRotation(this->angle * 180 / 3.14159265);

	this->angle = angle;
	this->speed = speed;

	this->collisionbox->width = 0;
	this->collisionbox->height = 0;

	this->player = player;

	

};
Missle::~Missle(){

	delete movement;

}

void Missle::update(){

	for(auto tar : *(target_list)){

		if(tar->collisionbox->contains(this->x, this->y)){

			tar->damage_taken += 15;
			exists = 0;

		}

	}

	for(auto tar : *(mobs)){

		if(tar->collisionbox->contains(this->x, this->y)){
			if(deflected){
				tar->damage_taken += 20;
				exists = 0;
			}

		}

	}

	if(damage_taken == 20){
		deflected = 1;
		damage_taken = 0;
		this->speed /= 1.2;

		if(this->speed <= 100){
			exists = 0;
		}

		dir_x = this->y-player->y - sin(player->angle);
		dir_y = player->x - this->x + cos(player->angle);
		if(dir_x != 0 || dir_y != 0){
			double norm = sqrt(dir_x*dir_x+dir_y*dir_y);
			dir_x /= norm;
			dir_y /= norm;
			this->angle = (atan2(dir_y, dir_x));
		}

	}

}

void Missle::update(const float &dt){
	this->x += dir_x*speed*dt;
	this->y += dir_y*speed*dt;

	int check = map->intersec(x, y);
	if(check == 1 || check == -1)
		this->exists = 0;

}

void Missle::render(sf::RenderWindow * window){

	if(not map->getMask(x, y)){
		return;
	}

	//int a = map->getLum(x, y);
	this->sprite->setRotation(this->angle * 180 / 3.14159265);
	this->sprite->setColor(sf::Color(255, 255, 255));
	this->sprite->setPosition(this->x, this->y);
    window->draw(*this->sprite);

}