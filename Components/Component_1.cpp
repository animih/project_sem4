#include "../States/GameState.h"

Movement::Movement(double speed, Map * map, GameState * gamestate, Entity * owner, bool option){

	this->gamestate = gamestate;
	this->map = map;
	this->owner = owner;
	this->option = option;

	this->max_speed = speed;

}

void Movement::Set_dir(double dir_x, double dir_y){
	this->dir_x = dir_x;
	this->dir_y = dir_y;
	if(option){
	int check = map->intersec(owner->x, owner->y+owner->collisionbox->height/2);
	if(check == 1 || check == -1 || check == 6){
		owner->y = int(round(owner->y/32))*32;

		//this->dir_y = -fabs(this->dir_y);
	}
	else{
	check = map->intersec(owner->x, owner->y-owner->collisionbox->height/2);
	if(check == 1 || check == -1 || check == 6){
		owner->y = int(round(owner->y/32))*32;
		//this->dir_y = fabs(this->dir_y);
	}
	}

	check = map->intersec(owner->x+owner->collisionbox->width/2, owner->y);
	if(check == 1 || check == -1 || check == 6){
		owner->x = int(round(owner->x/32))*32;
		//this->dir_x = - fabs(this->dir_x);
	}
	else{
	check = map->intersec(owner->x-owner->collisionbox->width/2, owner->y);
	if(check == 1 || check == -1 || check == 6){
		owner->x = int(round(owner->x/32))*32;
		//this->dir_x = fabs(this->dir_x);
	}
	}
	}

	double norm = sqrt(dir_x*dir_x+dir_y*dir_y);
	
	if(this->dir_x != 0){
		this->dir_x /= norm;
	}
	if(this->dir_y != 0){
		this->dir_y /= norm;
	}

	owner->collisionbox->left = owner->x-32/2;
	owner->collisionbox->top = owner->y-32/2;

}

void Movement::update(){

}

void Movement::update(const float &dt){

	double add_x;
	double add_y;

	owner->x += dir_x * max_speed * dt;
	owner->y += dir_y * max_speed * dt;

	dir_x > 0 ? add_x = owner->collisionbox->width/2 : dir_x < 0 ? add_x = -owner->collisionbox->width/2 : add_x = 0;
	dir_y > 0 ? add_y = owner->collisionbox->height/2 : dir_y < 0 ? add_y = -owner->collisionbox->height/2 : add_y = 0;

	int check = map->intersec(owner->x, owner->y+add_y);
	bool checkout = this->gamestate->collision_check(owner->x, owner->y+add_y); 

	if(check == 1 || check == 6 || check == -1 || checkout)
		owner->y -= dir_y * max_speed * dt;

	check = map->intersec(owner->x+add_x, owner->y);
	checkout = this->gamestate->collision_check(owner->x+add_x, owner->y); 

	if(check == 1 || check == 6 || check == -1 || checkout)
		owner->x -= dir_x * max_speed * dt;

}

Health::Health(int hp, int armor, Entity * owner){

	this->max_hp = hp;
	this->hp = hp;

	this->armor = armor;
	this->owner = owner;

}

void Health::update(){

	if(owner->damage_taken != 0){
		if(owner->damage_taken < armor){
			owner->damage_taken = 0;
			return;
		}

		hp -= owner->damage_taken-armor;
		owner->damage_taken = 0;

		if(hp < 0){
			owner->exists = 0;
		}
	}

}

void Health::update(const float &dt){

};

double Health::Get_hp(){
	return this->hp/(this->max_hp*1.);
}

Damage::Damage(Entity * owner, Map * map, std::list<Entity *> * target, double damage){
	this->owner = owner;
	this->map = map;
	this->target = target;

	this->damage = damage;

}

void Stab::Set_dir(double dir_x, double dir_y){

	this->dir_x = dir_x;
	this->dir_y = dir_y;

	if(this->dir_x != 0){
		this->dir_x /= sqrt(dir_x*dir_x+dir_y*dir_y);
	}
	if(this->dir_y != 0){
		this->dir_y /= sqrt(dir_x*dir_x+dir_y*dir_y);
	}

	this->hit = 1;
}


void Stab::update(){

}

void Stab::update(const float &dt){

	if(hit){
		timer += dt;
		if(timer <= 0){
			return;
		}

		hit = 0;
		timer = 0;

		for(auto tar : *target){
			if(!map->if_visible(owner->x, owner->y, tar->x, tar->y))
				continue;
			if( (tar->x-owner->x)*(tar->x-owner->x) + (tar->y-owner->y)*(tar->y-owner->y) < range*range){
				double dot = (tar->x-owner->x)*dir_x + (tar->y-owner->y)*dir_y;
				double det = (tar->x-owner->x)*dir_y - (tar->y-owner->y)*dir_x;

				if(fabs(atan2(det, dot)) < 0.3){
					tar->damage_taken += damage;
				}


			}


		}


	}

};

void Slash::Set_dir(double dir_x, double dir_y){

	this->dir_x = dir_x;
	this->dir_y = dir_y;

	if(this->dir_x != 0){
		this->dir_x /= sqrt(dir_x*dir_x+dir_y*dir_y);
	}
	if(this->dir_y != 0){
		this->dir_y /= sqrt(dir_x*dir_x+dir_y*dir_y);
	}

	this->hit = 1;
}

void Slash::update(){
	
}

void Slash::update(const float &dt){

	if(hit){
		timer += dt;
		if(timer <= 0){
			return;
		}
		hit = 0;
		timer = 0;

		for(auto tar : *target){
			if(!map->if_visible(owner->x, owner->y, tar->x, tar->y))
				continue;
			if( (tar->x-owner->x)*(tar->x-owner->x) + (tar->y-owner->y)*(tar->y-owner->y) < range*range){
				double dot = (tar->x-owner->x)*dir_x + (tar->y-owner->y)*dir_y;
				double det = (tar->x-owner->x)*dir_y - (tar->y-owner->y)*dir_x;

				if(fabs(atan2(det, dot)) < 1.8){
					tar->damage_taken += damage;
				}


			}


		}


	}

};

MovementDamage::MovementDamage(Entity * owner, Map * map, std::list<Entity *> * target, double range, double damage):Damage(owner, map, target, damage){

	this->range = range;
	this->copy_list.clear();

	for(auto u: *this->target){
		copy_list.push_back(u);
	}
}

MovementDamage::~MovementDamage(){
	this->copy_list.clear();
}

void MovementDamage::reload(){
	this->copy_list.clear();

	for(auto u: *this->target){
		copy_list.push_back(u);
	}
	printf("%d \n", copy_list.size());
	printf("%d \n", target->size());
}

void MovementDamage::update(){
	//printf("ARGH! \n");
	auto it_end = copy_list.end();
	for(auto it = copy_list.begin(); it != it_end;){
		if(((*it)->x-owner->x)*((*it)->x-owner->x) + ((*it)->y-owner->y)*((*it)->y-owner->y) < range*range){
			(*it)->damage_taken += damage;
			auto it_del = it;
			++it;
			copy_list.erase(it_del);
			it_end = copy_list.end();
		}
		++it;
	}

}

void MovementDamage::update(const float &dt){

}



AnimationComponent::AnimationComponent(double * x, double * y, double * angle){
	this->x = x;
	this->y = y;
	this->angle = angle;
}

AnimationComponent::~AnimationComponent(){ 

	for(auto &i : this->animations){
		delete i.second;
	}

}

void AnimationComponent::addAnimation(std::string F, float animationTimer, int start_frame_x, int start_frame_y, int frames_x, int frames_y, int width, int height, bool continous){


	if(this->animations.count(F) != 0){
		delete this->animations[F];
	}

	this->animations[F] = new Animation(F, animationTimer, start_frame_x, start_frame_y, frames_x, frames_y, width, height, continous);

}

void AnimationComponent::play(const std::string key, const float &dt){

	this->animations[key]->play(dt);

	if(animations[key]->stop == 1){
		delete animations[key];
		printf("the end \n");
		animations.erase(key);
	}

}
void AnimationComponent::reset(const std::string key){

	this->animations[key]->reset();

}

void AnimationComponent::play(const float &dt){
	for(auto &i : this->animations){
		i.second->play(dt);
		//printf("Component: %f \n", dt);

		if(i.second->stop == 1){
		delete i.second;
		//printf("the end \n");
		animations.erase(i.first);
	}
	}
}

void AnimationComponent::render(sf::RenderWindow * window){

	for(auto &i : this->animations){
		i.second->sprite->setPosition(*this->x, *this->y);
		i.second->sprite->setOrigin(i.second->width/2, i.second->height/2);
		i.second->sprite->setRotation(*(this->angle) * 180 / 3.14159265);
		window->draw(*(i.second->sprite));
	}

}


Spell::Spell(Entity * owner, Map * map, std::list<Entity *> * target, double range){
	this->owner = owner;
	this->map = map;
	this->target = target;

	this->range = range;
};
Spell::~Spell(){

};


OnFlame::OnFlame(Entity * owner, Map * map, std::list<Entity *> * target, double duration, double jmp_bound, double damage):Damage(owner, map, target, damage){

	this->duration = duration;
	this->jmp_bound = jmp_bound;

	this->range = 64;

};

OnFlame::~OnFlame(){
}

void OnFlame::update(){

	if(timer >= duration){
		this->exists = 0;
	}

	if(map->get_tile(owner->x, owner->y) == 3)
		this->exists = 0;

	map->update_player_lighting_mask(owner->x, owner->y, 120);

}

void OnFlame::update(const float & dt){

	timer += dt;

	if(round(timer) > comp){
		owner->damage_taken += damage;
		comp = round(timer);
	}


	for(auto tar : *target){
			if(!map->if_visible(owner->x, owner->y, tar->x, tar->y))
				continue;
			if( (tar->x-owner->x)*(tar->x-owner->x) + (tar->y-owner->y)*(tar->y-owner->y) < range*range){
				if(tar->components.count("OnFlame") == 0){
					tar->components["OnFlame"] = new OnFlame(tar, map, target, duration, jmp_bound, damage);
				}
			}
	}

}

FireCone::FireCone(Entity * owner, Map * map, std::list<Entity *> * target, double range):Spell(owner, map, target, range){

}

FireCone::~FireCone(){
}

void FireCone::Set_dir(double dir_x, double dir_y){

	this->dir_x = dir_x;
	this->dir_y = dir_y;

	if(this->dir_x != 0){
		this->dir_x /= sqrt(dir_x*dir_x+dir_y*dir_y);
	}
	if(this->dir_y != 0){
		this->dir_y /= sqrt(dir_x*dir_x+dir_y*dir_y);
	}

	this->hit = 1;
}

void FireCone::update(){

	if(hit){

		hit = 0;

		for(auto tar : *target){
			if(!map->if_visible(owner->x, owner->y, tar->x, tar->y))
				continue;
			if((tar->x-owner->x)*(tar->x-owner->x) + (tar->y-owner->y)*(tar->y-owner->y) < 320*320){
				double dot = (tar->x-owner->x)*dir_x + (tar->y-owner->y)*dir_y;
				double det = (tar->x-owner->x)*dir_y - (tar->y-owner->y)*dir_x;

				if(fabs(atan2(det, dot)) < 0.71){
					tar->damage_taken += 15;
					tar->components["OnFlame"] = new OnFlame(tar, map, target, 5, 0.5, 10);
				}


			}


		}


	}

};

void FireCone::update(const float &dt){};
