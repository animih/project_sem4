#include "Entity.h"

Zombie::Zombie(std::string F, float X, float Y, float W, float H, Map * map, std::list<Entity *> * targets, GameState * gamestate, Tactics * tactics)
 : Entity(F, X, Y, W, H){
 	this->angle = rand()%180*M_PI/180;
	this->map = map;
	this->Max_Rotate_speed = 5;
	movement = new Movement(140, map, gamestate, this);
	hp_armor = new Health(100, 1, this); // hit_points and armor class
	animation = new AnimationComponent(&x, &y, & angle);

	damage = new Stab(this, map, targets, 78, 10);

	this->tactics = tactics;

	dest.first = X;
	dest.second = Y;

	dir.first = 0;
	dir.second = 0;

}

Zombie::~Zombie(){

	delete movement;
	delete hp_armor;
	delete animation;
	delete damage;

	delete sprite;
	delete texture;
	delete image;
}

void Zombie::render(sf::RenderWindow * window){
	animation->render(window);

	this->sprite->setPosition(this->x, this->y);
    window->draw(*this->sprite);
   
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(16, 16));
    rect.setFillColor(sf::Color::Red);
    rect.setOrigin(8, 8);
    rect.setPosition(sf::Vector2f(this->x, this->y));

   	window->draw(rect);

   	rect.setPosition(this->x+dir.first*32, this->y+dir.second*32);
   	window->draw(rect);

   	rect.setPosition(dest.first, dest.second);
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

void Zombie::setAngle(int dx, int dy){
	this->angle = (atan2(dy, dx));
};

void Zombie::update(const float &dt){

	movement->update(dt);
	hp_armor->update(dt);
	animation->play(dt);
	damage->update(dt);

	this->angle += this->Rotate_speed*dt;

	if(this->angle > M_PI){
		this->angle -= 2*M_PI;
	}
	else if(this->angle < -M_PI){
		this->angle += 2*M_PI;
	}

	if(hit == 1 && animation->animations.count("./Resourses/Zombie_bite.png") == 0){
		hit = 0;
		damage->Hit(cos(angle), sin(angle));
	}

	this->sprite->setRotation(this->angle * 180 / 3.14159265);
	
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

	if(dest.first != x && dest.second!= y ){
	dir = tactics->lead_mob(this->x, this->y, dest.first, dest.second, 1);
	}
	movement->Set_dir(dir.first, dir.second);

	if(fabs(this->x - entity->x) > 620 && fabs(this->y - entity->y) > 620){
			this->seen = 0;
			this->Rotate_speed = 0;
			return;
	}
	else if(map->if_visible(this->x, this->y, entity->x, entity->y)){


		double rot_angle = atan2(-this->y + entity->y, -this->x + entity->x)-this->angle;
		if(fabs(rot_angle) > M_PI){
			rot_angle = -sign_sec(rot_angle)*(2*M_PI - fabs(rot_angle));
		}
		if(fabs(rot_angle) > 2 && !seen){
			return;
		}

		if(fabs(this->x - entity->x) < 55 && fabs(this->y - entity->y) < 55 && (!hit)){
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
		if(fabs(this->x - entity->x) > 420 && fabs(this->y - entity->y) > 420){
			this->seen = 0;
			this->Rotate_speed = 0;
			return;
		}
		//this->Rotate_speed = 0;
		double rot_angle = atan2(dir.first, dir.second)-this->angle;
		if(fabs(rot_angle) > M_PI){
			rot_angle = -sign_sec(rot_angle)*(2*M_PI - fabs(rot_angle));
		}

		this->Rotate_speed = Max_Rotate_speed*sign_sec(rot_angle);

		if(timer >= 5){
			timer = 0;

			dest.first = entity->x;
			dest.second =  entity->y;
		}


	}
};


