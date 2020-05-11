#include "Entity.h"

Entity::Entity(std::string F, double X, double Y, int W, int H) {
	this->image = new sf::Image();
	this->texture = new sf::Texture();
	this->sprite = new sf::Sprite();
	this->name = &F;
	this->w = W; this->h = H;
	image->loadFromFile(F);
	texture->loadFromImage(*image);
	sprite->setTexture(*texture);
	this->x = X; this->y = Y;
	sprite->setTextureRect(sf::IntRect(0, 0, w, h));
	sprite->setOrigin(this->w / 2, this->h / 2);
	collisionbox = new sf::FloatRect(x - 32 / 2, y - 32 / 2, 32, 32);
};
Entity::Entity() {
};

Entity::~Entity(){

}

void Entity::react(Entity * someone){
	return;
}

const bool & Entity::if_exists() const{
	return this->exists;
}