#include "Entity.h"

Player::Player(std::string F, float X, float Y, float W, float H, Map * map) : Entity(F, X, Y, W, H) {
	this->map = map;
}

Player::~Player(){
	delete sprite;
	delete texture;
	delete image;
}

void Player::update(const float &dt)
{

	double dX = 0;
	double dY = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		dY = -1;
	};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		dX = -1;
	};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		dY = 1;
	};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		dX = 1;
	};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)){
		this->anti_cheat = 0;
	}
	else
		this->anti_cheat = 1;


	if ((dX!=0)&&(dY!=0)) { 
		dX /= sqrt(2); 
		dY /= sqrt(2);
	};
	this->move(dt, *((const double *) &dX), *((const double *) &dY));
}

void Player::render(sf::RenderWindow * window){

	sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
	sf::Vector2f pos = window->mapPixelToCoords(pixelPos);
	this->setAngle((pos.x - this->x), (pos.y - this->y));
	this->sprite->setRotation(this->getAngleD());


	this->sprite->setPosition(this->x, this->y);
    window->draw(*this->sprite);

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(16, 16));
    rect.setOutlineColor(sf::Color::Red);
    rect.setOrigin(8, 8);
    rect.setPosition(sf::Vector2f(this->x, this->y));

   	window->draw(rect);



}

void Player::move(const float& dt, const float & dir_x, const float & dir_y){

	int check;

	double add_x;
	double add_y;

	if(this->sprite){

		x += dir_x * speed * dt;
		y += dir_y * speed * dt;

		dir_x > 0 ? add_x = 16 : dir_x < 0 ? add_x = -16 : add_x = 0;
		dir_y > 0 ? add_y = 16 : dir_y < 0 ? add_y = -16 : add_y = 0;


		check = map->intersec(x, y+add_y);

		if(check == 1 || check == -1)
			y -= dir_y * speed * dt;

		check = map->intersec(x+add_x, y);

		if(check == 1 || check == -1)
			x -= dir_x * speed * dt;


	}	


}