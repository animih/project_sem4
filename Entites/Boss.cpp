#include "Entity.h"

Boss::Boss(std::string F, float X, float Y, float W, float H, Map * map, std::list<Entity *> * targets, std::list<Entity *> * mobs, GameState * gamestate, Entity * player, Tactics * tactics):
Entity(F, X, Y, W, H){

	this->map = map;
	this->gamestate = gamestate;
	this->player = player;
	this->target_list = targets;
	this->mobs = mobs;

	this->tactics = tactics;

	components["Hp"] = new Health(195, 2, this);
	this->animation = new AnimationComponent(&x, &y, &angle);

	components["Movement"] = new Movement(170, map, gamestate, this);
	components["Dash_Movement"] = new Movement(240, map, gamestate, this);

	components["Hit"] = new Slash(this, map, targets, 115, 25);
	components["Hit_Dash"] = new Slash(this, map, targets, 115, 40);

	angle = 0;

	this->Max_Rotate_speed = 12;

	dest.first = X;
	dest.second = Y;

	this->start_x = X;
	this->start_y = Y;

	dir.first = 0;
	dir.second = 0;

	this->second_image = new sf::Image();
	this->second_texture = new sf::Texture();
	this->second_sprite = new sf::Sprite();

	second_image->loadFromFile("./Resourses/Boss1.png");
	second_texture->loadFromImage(*second_image);
	second_sprite->setTexture(*second_texture);
	second_sprite->setOrigin(this->w / 2, this->h / 2);


}

Boss::~Boss(){

	for(auto obj : components){
		delete obj.second;
	}
	components.clear();

	delete animation;
	delete sprite;
	delete texture;
	delete image;

}

void Boss::setAngle(int dx, int dy){
	this->angle = (atan2(dy, dx));
}

int sign_fr(float a){
	if(a > 0){
		return 1;
	}
	else if(a < 0){
		return -1;
	}
	else
		return 0;

}

void Boss::react(Entity * entity){
	
	if(mode){
		dest.first = start_x;
		dest.second =  start_y;
		this->setAngle(entity->x-this->x, entity->y - this->y);

		if(!shoot){
			shoot = 1;

			this->mobs->push_back(new Missle("./Resourses/Missle.png", x, y, angle, 300, 32, 32, map, target_list, gamestate, player, mobs));

		}

	}
	else{
		if(fabs(timer-timer_kd)>1){
			double rot_angle = atan2(-this->y + entity->y, -this->x + entity->x)-this->angle;
			if(fabs(rot_angle) > M_PI){
				rot_angle = -sign_fr(rot_angle)*(2*M_PI - fabs(rot_angle));
			}
			this->Rotate_speed = Max_Rotate_speed*sign_fr(rot_angle);
		}
		else{
			Rotate_speed = 0;
			this->setAngle(entity->x-this->x, entity->y - this->y);
		}


		dest.first = entity->x;
		dest.second =  entity->y;

		if(fabs(this->x - entity->x) < 120 && fabs(this->y - entity->y) < 120 && (!hit)){
			hit = 1;
			animation->addAnimation("./Resourses/Zombie_bite.png", 0.6, 0, 0, 0, 4, 120, 64);
		}
	}

	
}

void Boss::update(){
	
	if(components.count("OnFlame") != 0 && animation->animations.count("./Resourses/OnFire.png") == 0){
		animation->addAnimation("./Resourses/OnFire.png", 0.3, 0, 0, 0, 5, 64, 64, 1);
	}
	else if(components.count("OnFlame") == 0 && animation->animations.count("./Resourses/OnFire.png") != 0){
		animation->animations["./Resourses/OnFire.png"]->stop = 1;
	}

	for(auto obj : components)
		obj.second->update();

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

	if(dest.first != x && dest.second!= y ){
		dir = tactics->lead_mob(this->x, this->y, dest.first, dest.second, 1);
	}

	if(!mode){
		if(hit == 1 && animation->animations.count("./Resourses/Zombie_bite.png") == 0){
			hit = 0;
			if(fabs(timer-timer_kd) > 1)
				components["Hit"]->Set_dir(cos(angle), sin(angle));
			else
				components["Hit_Dash"]->Set_dir(cos(angle), sin(angle));
		}
		if(fabs(timer-timer_kd) < 1){
			components["Dash_Movement"]->Set_dir(dir.first, dir.second);
			components["Movement"]->Set_dir(0, 0);
		}
		else{
			components["Movement"]->Set_dir(dir.first, dir.second);
			components["Dash_Movement"]->Set_dir(0, 0);
		}


	}
	else{
		components["Dash_Movement"]->Set_dir(dir.first, dir.second);
	}
	
}

void Boss::update(const float & dt){
	
	timer += dt;
	this->angle += this->Rotate_speed*dt;

	if(timer >= timer_kd){
		mode = not mode;
		timer = 0;
	}

	if(shoot){
		shoot_timer += dt;

		if(shoot_timer >= 0.35){
			shoot_timer = 0;
			shoot = 0;
		}

	}

	for(auto obj : components)
		obj.second->update(dt);

	animation->play(dt);
	
}

void Boss::render(sf::RenderWindow * window){

	if(not map->getMask(x, y)){
		return;
	}

	animation->render(window);

	if((shoot && shoot_timer < 0.17) || hit){
		this->second_sprite->setColor(sf::Color(255, 255, 255, 255));
		this->second_sprite->setRotation(this->angle * 180 / 3.14159265);
		this->second_sprite->setPosition(this->x, this->y);
    	window->draw(*this->second_sprite);
	}
	else{
		this->sprite->setColor(sf::Color(255, 255, 255, 255));
		this->sprite->setRotation(this->angle * 180 / 3.14159265);
		this->sprite->setPosition(this->x, this->y);
    	window->draw(*this->sprite);
	}
   	
    sf::RectangleShape rect;
    rect.setOrigin(8, 8);
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
