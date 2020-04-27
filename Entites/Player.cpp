#include "Entity.h"

Player::Player(std::string F, float X, float Y, float W, float H, Map * map, sf::Vector2f * mousePosView, std::list<Entity *> * mobs, GameState * gamestate)
 : Entity(F, X, Y, W, H){
	this->map = map;
	this->mousePosView = mousePosView;
	movement = new Movement(550, map, gamestate, this, 0);
	dash_movement = new Movement(950, map, gamestate, this, 0);
	hp_armor = new Health(100, 5, this); // hit_points and armor class
	animation = new AnimationComponent(&x, &y, & angle);
	damage_1 = new Stab(this, map, mobs, 145, 25); // указатель на владельца и указатель на карту, traget, range, damage
	damage_2 = new Slash(this, map, mobs, 110, 20);

	dash = new MovementDamage(this, map, mobs, 70, 5);
	timer = 0;
	dash_duration = 0.65;
	dash_kd = 1.8;


}

Player::~Player(){

	delete movement;
	delete hp_armor;
	delete damage_1;
	delete damage_2;
	delete animation;

	delete sprite;
	delete texture;
	delete image;
}

void Player::control(){
	this->setAngle((mousePosView->x - this->x), (mousePosView->y - this->y));
	this->sprite->setRotation(this->angle * 180 / 3.14159265);

	double dX = 0;
	double dY = 0;

	static bool hit1 = 0;
	static bool hit2 = 0;

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && !is_dashing && timer >= dash_kd) {
		is_dashing = 1;
		dash->reload();
		animation->addAnimation("./Resourses/Dash.png", dash_duration, 0, 0, 0, 5, 120, 64);
		timer = 0;
	};
	if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
		hit1 = 1;
		animation->addAnimation("./Resourses/Player_stab.png", 0.2, 0, 0, 0, 3, 240, 64);
	}
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		hit2 = 1;
		animation->addAnimation("./Resourses/Player_slash.png", 0.2, 0, 0, 0, 6, 200, 100);
	}
	if(hit1 == 1 && animation->animations.count("./Resourses/Player_stab.png") == 0){
		hit1 = 0;
		damage_1->Hit(mousePosView->x-this->x, mousePosView->y-this->y);
		printf("Hit1 \n");
	}
	if(hit2 == 1 && animation->animations.count("./Resourses/Player_slash.png") == 0){
		hit2 = 0;
		damage_2->Hit(mousePosView->x-this->x, mousePosView->y-this->y);
		printf("Hit2 \n");
	}

	if(!is_dashing){
		movement->Set_dir(dX, dY);
		dash_movement->Set_dir(0, 0);
	}
	else{
		damage_taken = 0;
		dash_movement->Set_dir((mousePosView->x - this->x), (mousePosView->y - this->y));
	}
}

void Player::render(sf::RenderWindow * window){
	animation->render(window);

	this->sprite->setPosition(this->x, this->y);
    window->draw(*this->sprite);

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(16, 16));
    rect.setOutlineColor(sf::Color::Red);
    rect.setOrigin(8, 8);
    rect.setPosition(sf::Vector2f(this->x, this->y));

   	window->draw(rect);

   	double hp = hp_armor->Get_hp();

   	rect.setSize(sf::Vector2f(64*hp, 16));
   	rect.setFillColor(sf::Color(14, 220, 60, 82));
   	rect.setPosition(sf::Vector2f(this->x-64/2+16/2, this->y-32));

   	window->draw(rect);

   	rect.setSize(sf::Vector2f(64-64*hp, 16));
	rect.setFillColor(sf::Color(224, 20, 10, 82));
   	rect.setPosition(sf::Vector2f(this->x-64/2+64*hp+16/2, this->y-32));

   	window->draw(rect);
   	

}

void Player::renderUI(sf::RenderWindow * window){
	
	sf::RectangleShape rect;

    if(!is_dashing){
    	if(timer <= dash_kd){
    		rect.setFillColor(sf::Color(155, 155, 155, 80));
    		double a = timer/dash_kd;
    		rect.setPosition(sf::Vector2f(this->x-window->getSize().x/2+20, this->y+window->getSize().y/2-20-a*120));
    		rect.setSize(sf::Vector2f(120, a*120));
    	}
    	else{
    		rect.setFillColor(sf::Color(155, 155, 155, 80));
    		rect.setPosition(sf::Vector2f(this->x-window->getSize().x/2+20, this->y+window->getSize().y/2-20-120));
			rect.setSize(sf::Vector2f(120, 120));
    	}
	}
	else{
		rect.setFillColor(sf::Color(0, 155, 255, 80));
		rect.setPosition(sf::Vector2f(this->x-window->getSize().x/2+20, this->y+window->getSize().y/2-20-120));
		rect.setSize(sf::Vector2f(120, 120));
	}
   	
   	window->draw(rect);

}

void Player::setAngle(int dx, int dy){
	this->angle = (atan2(dy, dx));
};

void Player::update(){
	this->control();
	movement->update();
	dash_movement->update();

	hp_armor->update();
	damage_1->update();
	damage_2->update();

	this->map->update_view_mask(this->x, this->y, 650);
	this->map->update_player_lighting_mask(this->x, this->y, 250);
	if(is_dashing){
		dash->update();
	}

}


void Player::update(const float &dt){

	if(is_dashing){
		timer += dt;
		//printf("%f \n", timer);


		if(timer >= dash_duration){
			timer = 0;
			is_dashing = 0;
		}
	}
	else if(timer < dash_kd){
		timer += dt;
	}

	movement->update(dt);
	dash_movement->update(dt);
	hp_armor->update(dt);
	damage_1->update(dt);
	damage_2->update(dt);
	animation->play(dt);
	
}


