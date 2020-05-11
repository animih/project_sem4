#include "Entity.h"

Player::Player(std::string F, float X, float Y, float W, float H, Map * map, sf::Vector2f * mousePosView, std::list<Entity *> * mobs, GameState * gamestate)
 : Entity(F, X, Y, W, H){
	this->map = map;
	this->mousePosView = mousePosView;
	components["Movement"] = new Movement(550, map, gamestate, this, 0);
	components["Dash_mov"] = new Movement(1050, map, gamestate, this, 0);
	components["Hp"] = new Health(100, 5, this); // hit_points and armor class
	animation = new AnimationComponent(&x, &y, & angle);
	components["hit1"] = new Stab(this, map, mobs, 145, 25); // указатель на владельца и указатель на карту, traget, range, damage
	components["hit2"] = new Slash(this, map, mobs, 110, 20);

	components["Dash_dmg"] = new MovementDamage(this, map, mobs, 70, 5);
	components["FireCast"] = new FireCone(this, map, mobs, 298);
	dash_duration = 0.25;
	dash_kd = 1.8;
	cast_kd = 7;
	timer_2 = cast_kd;

}

Player::~Player(){

	for(auto obj : components){
		delete obj.second;
	}
	components.clear();

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && !is_dashing && timer_1 >= dash_kd) {
		is_dashing = 1;
		components["Dash_dmg"]->reload();
		animation->addAnimation("./Resourses/Dash.png", dash_duration, 0, 0, 0, 5, 120, 64);
		timer_1 = 0;
	};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && !is_dashing && timer_2 >= cast_kd) {
		is_casting = 1;
		timer_2 = 0;
		animation->addAnimation("./Resourses/FireAnimation.png", 0.5, 0, 0, 0, 5, 560, 220);
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
		components["hit1"]->Set_dir(mousePosView->x-this->x, mousePosView->y-this->y);
		printf("Hit1 \n");
	}
	if(hit2 == 1 && animation->animations.count("./Resourses/Player_slash.png") == 0){
		hit2 = 0;
		components["hit2"]->Set_dir(mousePosView->x-this->x, mousePosView->y-this->y);
		printf("Hit2 \n");
	}
	if(is_casting == 1 && animation->animations.count("./Resourses/Player_slash.png") == 0){
		is_casting = 0;
		components["FireCast"]->Set_dir(mousePosView->x-this->x, mousePosView->y-this->y);
		printf("Hit2 \n");
	}

	if(!is_dashing){
		components["Movement"]->Set_dir(dX, dY);
		components["Dash_mov"]->Set_dir(0, 0);
	}
	else{
		damage_taken = 0;
		components["Dash_mov"]->Set_dir((mousePosView->x - this->x), (mousePosView->y - this->y));
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

   	double hp = components["Hp"]->Get_hp();

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
    	if(timer_1 <= dash_kd){
    		rect.setFillColor(sf::Color(155, 155, 155, 80));
    		double a = timer_1/dash_kd;
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

	if(!is_casting){
    	if(timer_2 <= cast_kd){
    		rect.setFillColor(sf::Color(155, 155, 155, 80));
    		double a = timer_2/cast_kd;
    		rect.setPosition(sf::Vector2f(this->x-window->getSize().x/2+40+155, this->y+window->getSize().y/2-20-a*120));
    		rect.setSize(sf::Vector2f(120, a*120));
    	}
    	else{
    		rect.setFillColor(sf::Color(155, 155, 155, 80));
    		rect.setPosition(sf::Vector2f(this->x-window->getSize().x/2+40+155, this->y+window->getSize().y/2-20-120));
			rect.setSize(sf::Vector2f(120, 120));
    	}
	}
	else{
		rect.setFillColor(sf::Color(195, 85, 55, 80));
		rect.setPosition(sf::Vector2f(this->x-window->getSize().x/2+40+155, this->y+window->getSize().y/2-20-120));
		rect.setSize(sf::Vector2f(120, 120));
	}
   	
   	window->draw(rect);

}

void Player::setAngle(int dx, int dy){
	this->angle = (atan2(dy, dx));
};

void Player::update(){
	this->control();

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

	for(auto obj: components)
		obj.second->update();

	this->map->update_view_mask(this->x, this->y, 650);
	this->map->update_player_lighting_mask(this->x, this->y, 250);

}


void Player::update(const float &dt){

	if(is_dashing){
		timer_1 += dt;
		//printf("%f \n", timer);


		if(timer_1 >= dash_duration){
			timer_1 = 0;
			is_dashing = 0;
		}
	}
	else if(timer_1 < dash_kd){
		timer_1 += dt;
	}

	if(!is_casting && timer_2 <= cast_kd){
		timer_2 += dt;
	}

	for(auto obj: components)
		obj.second->update(dt);

	animation->play(dt);
	
}


