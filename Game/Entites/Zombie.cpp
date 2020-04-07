#include "Entity.h"


Zombie::Zombie(std::string F, float X, float Y, float W, float H, Map * map):Entity(F, X, Y, W, H){
	this->map = map;
	this->speed = 0.10;
	this->setAngle(rand(), rand());
};

Zombie::~Zombie(){
	delete sprite;
	delete texture;
	delete image;
	//path.clear();
};

void Zombie::update(const float &dt)
{
	//this->move(dt, *((const double *) &dX), *((const double *) &dY));
}

void Zombie::render(sf::RenderWindow * window){

	sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(16, 16));
    rect.setOutlineColor(sf::Color::Red);
    rect.setOrigin(8, 8);

	this->sprite->setRotation(this->getAngleD());
	this->sprite->setPosition(this->x, this->y);
    window->draw(*this->sprite);

   
    rect.setPosition(sf::Vector2f(this->x, this->y));

   	window->draw(rect);



}

void Zombie::React_on(Entity * entity){
	if(fabs(this->x - entity->x) > 420 && fabs(this->y - entity->y) > 420){
			printf("I don't see you \n");
			this->seen = 0;
			return;
	}
	else if(map->if_visible(this->x, this->y, entity->x, entity->y)){
		this->setAngle(int(entity->x-this->x), int(entity->y-this->y));
		printf("I see you \n");
		this->seen = 1;
	}
	else if(this->seen){

		printf("I gonna start looking for ya! \n");

	}


};

