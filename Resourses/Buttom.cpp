#include "Buttom.h"


Buttom::Buttom(float x, float y, float width, float height, sf::Font* font, std::string text,
		 sf::Color idleColor, sf::Color hoverColor,
		  sf::Color activeColor){

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));

	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(sf::Color::White);
	this->text.setCharacterSize(24);
	this->text.setPosition(
		x + width/2- 
		this->text.getGlobalBounds().width /2.f,
		y - 5 + height/2 -
		this->text.getGlobalBounds().height /2.f
	);

	this->idleColor = idleColor;
	this->hoverColor = hoverColor;
	this->activeColor = activeColor;

	this->shape.setFillColor(this->idleColor);

	this->button_state = 0;



}

Buttom::~Buttom(){

	
}

void Buttom::render(sf::RenderWindow * window){

	switch(button_state){
		case 1:
			this->shape.setFillColor(this->hoverColor);
			break;
		case 2:
			this->shape.setFillColor(this->activeColor);
		default:
			this->shape.setFillColor(this->idleColor);
	}

	window->draw(this->shape);
	window->draw(this->text);

}

const bool Buttom::isPressed() const{
	if(this->button_state == 2)
		return true;

	return false;

};

void Buttom::update(const sf::Vector2f mousePos){

	this->button_state = 0;

	if(this->shape.getGlobalBounds().contains(mousePos)){

		this->button_state = 1;

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			this->button_state = 2;
		}

	}

}