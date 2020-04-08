#include "Entity.h"

Lantern::Lantern(std::string F, float X, float Y, float W, float H, Map * map):Entity(F, X, Y, W, H){
	this->map = map;
	this->is_on = rand()%2;
	this->map->update_env_lighting_mask(this->x, this->y, 690, is_on);
	this->bad_cond = rand()%2;

};
Lantern::~Lantern(){
	//delete sprite;
	//delete texture;
	//delete image;
	//path.clear();
};

void Lantern::React_on(Entity * entity){

	static bool is_key_pressed = 0;
	static int i = 0;

	if(fabs(entity->x-x) < 50 && fabs(entity->x-x) < 50){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && not is_key_pressed) {
			is_key_pressed = 1;
			i = 0;
		}
		else if(is_key_pressed){
			i += 1;
			if(bad_cond){
			this->is_on = not is_on;
			this->map->update_env_lighting_mask(this->x, this->y, 690, is_on);
			}
			else if(i%10 == 0){
			this->is_on = not is_on;
			this->map->update_env_lighting_mask(this->x, this->y, 690, is_on);
			}
			if(i == 102){
				this->is_on = not is_on;
				this->map->update_env_lighting_mask(this->x, this->y, 690, is_on);
				is_key_pressed = 0;
			}
		}
	}
};

void Lantern::update(const float &dt){

};
void Lantern::render(sf::RenderWindow * window){

	this->sprite->setPosition(this->x, this->y);
    window->draw(*this->sprite);

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(16, 16));
    rect.setOutlineColor(sf::Color::Red);
    rect.setOrigin(8, 8);
    rect.setPosition(sf::Vector2f(this->x, this->y));

   	window->draw(rect);

};

