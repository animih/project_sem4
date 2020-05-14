#include "Entity.h"

Zombie::Zombie(std::string F, float X, float Y, float W, float H, Map * map, std::list<Entity *> * targets, GameState * gamestate, Tactics * tactics)
 : Entity(F, X, Y, W, H){
 	this->angle = rand()%180*M_PI/180;
	this->map = map;
	this->Max_Rotate_speed = 8;
	components["Movement"] = new Movement(140, map, gamestate, this);
	components["Hp"] = new Health(100, 1, this); // hit_points and armor class
	animation = new AnimationComponent(&x, &y, & angle);

	components["Hit"] = new Slash(this, map, targets, 85, 10);

	this->tactics = tactics;

	dest.first = X;
	dest.second = Y;

	dir.first = 0;
	dir.second = 0;

}

Zombie::~Zombie(){

	for(auto obj : components){
		delete obj.second;
	}
	components.clear();

	delete animation;

	delete sprite;
	delete texture;
	delete image;
}

void Zombie::render(sf::RenderWindow * window){

	if(not map->getMask(x, y)){
		return;
	}

	animation->render(window);
	int a = map->getLum(x, y);
	this->sprite->setColor(sf::Color(255, 255, 255, a));

	this->sprite->setRotation(this->angle * 180 / 3.14159265);

	this->sprite->setPosition(this->x, this->y);
    window->draw(*this->sprite);
   	
    sf::RectangleShape rect;
    rect.setOrigin(8, 8);
   	double hp = components["Hp"]->Get_hp();
   	rect.setSize(sf::Vector2f(64*hp, 16));
   	rect.setFillColor(sf::Color(14, 220, 60, 82*a/255));
   	rect.setPosition(sf::Vector2f(this->x-64/2+16/2, this->y-32));

   	window->draw(rect);

   	rect.setSize(sf::Vector2f(64-64*hp, 16));
	rect.setFillColor(sf::Color(224, 20, 10, 82*a/255));
   	rect.setPosition(sf::Vector2f(this->x-64/2+64*hp+16/2, this->y-32));

   	window->draw(rect);
   	

}

void Zombie::setAngle(int dx, int dy){
	this->angle = (atan2(dy, dx));
};

void Zombie::update(){

	if(components.count("OnFlame") != 0 && animation->animations.count("./Resourses/OnFire.png") == 0){
		animation->addAnimation("./Resourses/OnFire.png", 0.3, 0, 0, 0, 5, 64, 64, 1);
	}
	else if(components.count("OnFlame") == 0 && animation->animations.count("./Resourses/OnFire.png") != 0){
		animation->animations["./Resourses/OnFire.png"]->stop = 1;
	}

	if(damage_taken != 0){
		seen = 1;
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

	if(this->angle > M_PI){
		this->angle -= 2*M_PI;
	}
	else if(this->angle < -M_PI){
		this->angle += 2*M_PI;
	}

	if(hit == 1 && animation->animations.count("./Resourses/Zombie_bite.png") == 0){
		hit = 0;
		components["Hit"]->Set_dir(cos(angle), sin(angle));
	}

	if(dest.first != x && dest.second!= y ){

	dir = tactics->lead_mob(this->x, this->y, dest.first, dest.second, 1);

	}
	components["Movement"]->Set_dir(dir.first, dir.second);

}

void Zombie::update(const float &dt){

	timer += dt;

	for(auto obj : components)
		obj.second->update(dt);

	animation->play(dt);

	this->angle += this->Rotate_speed*dt;
	
}

int sign_sec(float a){
	if(a > 0){
		return 1;
	}
	else if(a < 0){
		return -1;
	}
	else
		return 0;

}

void Zombie::react(Entity * entity){

	if(fabs(this->x - entity->x) > 490 && fabs(this->y - entity->y) > 490){
			this->seen = 0;
			this->Rotate_speed = 0;
			return;
	}
	else if(map->if_visible(this->x, this->y, entity->x, entity->y, this->seen)){


		double rot_angle = atan2(-this->y + entity->y, -this->x + entity->x)-this->angle;
		if(fabs(rot_angle) > M_PI){
			rot_angle = -sign_sec(rot_angle)*(2*M_PI - fabs(rot_angle));
		}
		if(fabs(rot_angle) > 2 && !seen && fabs(entity->x - x) > 65 && fabs(entity->y - y) > 65){
			return;
		}

		if(fabs(this->x - entity->x) < 65 && fabs(this->y - entity->y) < 65 && (!hit)){
			hit = 1;
			animation->addAnimation("./Resourses/Zombie_bite.png", 0.6, 0, 0, 0, 4, 120, 64);
		}

		this->Rotate_speed = Max_Rotate_speed*sign_sec(rot_angle);
		this->seen = 1;
		dest.first = entity->x;
		dest.second =  entity->y;
		//printf("%f %f\n", dest.first, dest.second);
	}
	else if(this->seen){
		//this->Rotate_speed = 0;
		double rot_angle = atan2(dir.first, dir.second)-this->angle;
		if(fabs(rot_angle) > M_PI){
			rot_angle = -sign_sec(rot_angle)*(2*M_PI - fabs(rot_angle));
		}

		this->Rotate_speed = Max_Rotate_speed*sign_sec(rot_angle);

		if(timer >= 1.2){
			timer = 0;

			dest.first = entity->x;
			dest.second =  entity->y;
		}


	}
};


