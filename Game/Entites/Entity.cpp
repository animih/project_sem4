#include "Entity.h"

Entity::Entity(std::string F, float X, float Y, float W, float H) {
	this->speed = 0;
	this->angle = 0;
	this-> name = new std::string();
	this->image = new sf::Image();
	this->texture = new sf::Texture();
	this->sprite = new sf::Sprite();
	this->name = &F;
	this->width = W; this->height = H;
	this->w = 16; this->h = 16;
	image->loadFromFile(F);
	image->createMaskFromColor(sf::Color(0, 0, 255));
	texture->loadFromImage(*image);
	sprite->setTexture(*texture);
	this->x = X; this->y = Y;
	sprite->setTextureRect(sf::IntRect(0, 0, width, height));
	sprite->setOrigin(this->width / 2, this->height / 2);
};
Entity::Entity() {
};

Entity::~Entity(){

}

void Entity::move(const float& dt, const float & dir_x, const float & dir_y){

	if(this->sprite){

		x += dir_x * speed * dt;
		y += dir_y * speed * dt;
	}


}


void Entity::setAngle(int dx, int dy) {
	this->angle = (atan2(dy, dx));
};
float Entity::getAngleR() {
	return (this->angle);
};
float Entity::getAngleD() {
	return (this->angle * 180 / 3.14159265);
};
void Entity::setSpeed(float speed) {
	this->speed = speed;
};
void Entity::setCollision(int x, int y, int w, int h) {
	collisionbox = new sf::FloatRect(x - w / 2, y - h / 2, w, h);
};

void Entity::React_on(Entity * entity){

};

