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

Table::Table(std::string F, float X, float Y, float W, float H, Map * map, sf::Font * font, std::string text, bool * triger):Entity(F, X, Y, W, H){
	this->map = map;
	this->triger = triger;

	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(sf::Color::White);
	this->text.setCharacterSize(20);
	this->text.setPosition(
		x - 
		this->text.getGlobalBounds().width /2.f,
		y -
		this->text.getGlobalBounds().height /2.f
	);
};


Table::~Table(){
};

void Table::react(Entity * entity){

	if(fabs(entity->x-this->x) < 120 && fabs(entity->y-this->y) < 120){
		is_close = 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && not is_key_pressed && fabs(entity->x-this->x) < 80 && fabs(entity->y-this->y) < 80) {
			is_key_pressed = 1;
		}
	}
	else{
		is_close = 0;
	}
};

void Table::update(){

	if(is_key_pressed){
			i += 1;
			if(i == 80){
				if(triger != NULL){
					*triger = 1;
				}
				is_key_pressed = 0;
				i = 0;
			}
	}
	map->update_player_lighting_mask(this->x, this->y, 120);
}

void Table::update(const float &dt){

};
void Table::render(sf::RenderWindow * window){

	if(not map->getMask(x, y)){
		return;
	}

	int a = map->getLum(x, y);
	this->sprite->setColor(sf::Color(255, 255, 255, a));

	this->sprite->setPosition(this->x, this->y);
    window->draw(*this->sprite);


    if(is_close){
		window->draw(this->text);
    }

};