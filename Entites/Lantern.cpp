#include "Entity.h"

Lantern::Lantern(std::string F, float X, float Y, float W, float H, Map * map):Entity(F, X, Y, W, H){
	this->map = map;
	this->is_on = rand()%2;
	this->map->update_env_lighting_mask(this->x, this->y, 540, is_on);
	this->bad_cond = rand()%2;

};


Lantern::~Lantern(){
};

void Lantern::react(Entity * entity){

	if(fabs(entity->x-this->x) < 70 && fabs(entity->y-this->y) < 70){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && not is_key_pressed) {
			is_key_pressed = 1;
		}
	}
};

void Lantern::update(){

	if(is_key_pressed){
			i += 1;
			if(bad_cond){
			this->is_on = not is_on;
			this->map->update_env_lighting_mask(this->x, this->y, 540, is_on);
			}
			else if(i%10 == 0){
			this->is_on = not is_on;
			this->map->update_env_lighting_mask(this->x, this->y, 540, is_on);
			}
			if(i == 80){
				this->is_on = not is_on;
				this->map->update_env_lighting_mask(this->x, this->y, 540, is_on);
				is_key_pressed = 0;
				i = 0;
			}
	}

}

void Lantern::update(const float &dt){

};
void Lantern::render(sf::RenderWindow * window){

	if(not map->getMask(x, y)){
		return;
	}

	int a = map->getLum(x, y);
	this->sprite->setColor(sf::Color(255, 255, 255, a));

	this->sprite->setPosition(this->x, this->y);
    window->draw(*this->sprite);

};