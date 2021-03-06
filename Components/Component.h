#ifndef COMPONENT_H
#define COMPONENT_H

#include "../Map/map.h"

class Entity;
class GameState;

class Component{
	protected:
	Entity * owner;
	public:
		bool exists = 1;
		virtual ~Component(){};
		virtual void update(const float & dt) = 0;
		virtual void update() = 0;
		virtual double Get_hp(){};
		virtual void reload(){};
		virtual void Set_dir(double dir_x, double dir_y){};

};

class Movement:public Component{

	double max_speed;

	Map * map;
	GameState * gamestate;
	double dir_x = 0;
	double dir_y = 0;
	bool option;

	public:
		Movement(double speed, Map * map, GameState * gamestate, Entity * owner, bool option = 1);
		~Movement(){};
		void update(const float & dt);
		void update();
		void Set_dir(double dir_x, double dir_y);

};

class Health:public Component{

	int max_hp;
	int hp;
	int armor;

	public:
		Health(int hp, int armor, Entity * owner);
		~Health(){};
		void update(const float & dt);
		void update();
		double Get_hp();
};

class Damage:public Component{
	protected:
	std::list<Entity *> * target; 
	double damage;

	Map * map;

	public:
		~Damage(){};
		Damage(Entity * owner, Map * map, std::list<Entity *> * target, double damage);
		Damage(){};
		virtual void update(const float &dt) = 0;
		virtual void update() = 0;
};

class Stab:public Damage{
	double range;

	bool hit = 0;
	double dir_x;
	double dir_y;

	double timer = 0;

	public:
		Stab(Entity * owner, Map * map, std::list<Entity *> * target, double range, double damage):Damage(owner, map, target, damage){
			this->range = range;
		};
		~Stab(){};
		void update(const float &dt);
		void update();
		void Set_dir(double dir_x, double dir_y);
};

class Slash:public Damage{
	double range;

	bool hit = 0;
	double dir_x;
	double dir_y;

	double timer = 0;

	public:
		Slash(Entity * owner, Map * map, std::list<Entity *> * target, double range, double damage):Damage(owner, map, target, damage){
			this->range = range;
		};
		~Slash(){};
		void update(const float &dt);
		void update();
		void Set_dir(double dir_x, double dir_y);
};

class MovementDamage:public Damage{

	double range;

	std::list<Entity *> copy_list;

	public:
		MovementDamage(Entity * owner, Map * map, std::list<Entity *> * target, double range, double damage);
		~MovementDamage();
		void update(const float &dt);
		void update();
		void reload();

};

class Spell:public Component{
	protected:
	double range;

	double dir_x;
	double dir_y;

	std::list<Entity *> * target;
	Map * map;

	public:
		Spell(Entity * owner, Map * map, std::list<Entity *> * target, double range);
		~Spell();
		virtual void update(const float &dt) = 0;
		virtual void update() = 0;	
};

class OnFlame:public Damage{

	double timer = 0;
	double duration;

	double range;

	double jmp_timer = 0;
	double jmp_bound;

	int comp = 0;

	public:
		OnFlame(Entity * owner, Map * map, std::list<Entity *> * target, double duration, double jmp_bound, double damage);
		~OnFlame();
		void update(const float &dt);
		void update();
};

class FireCone:public Spell{

	bool hit = 0;

	public:
		FireCone(Entity * owner, Map * map, std::list<Entity *> * traget, double range);
		~FireCone();
		void update(const float & dt);
		void update();
		void Set_dir(double dir_x, double dir_y);
};


class Player;


class AnimationComponent{
	double * x;
	double * y;
	double * angle;

	friend class Player;

	class Animation{
		public:
			sf::Image * image;
			sf::Texture * texture;
			sf::Sprite * sprite;

			double animationTimer;
			double timer = 0;

			int width;
			int height;

			bool stop = 0;
			bool continous = 0;

			sf::Rect<int> startRect;
			sf::Rect<int> endRect;
			sf::Rect<int> currentRect;

			Animation(std::string F, float animationTimer, int start_frame_x, int start_frame_y, int frames_x, int frames_y, int width, int height, bool continous = 0){
				image = new sf::Image();
				texture = new sf::Texture();
				sprite = new sf::Sprite();

				this->animationTimer = animationTimer;

				image->loadFromFile(F);
				texture->loadFromImage(*image);
				sprite->setTexture(*texture, true);

				this->startRect = sf::IntRect(start_frame_x*width, start_frame_y*height, width, height);
				this->endRect = sf::IntRect(frames_x*width, frames_y*height, width, height);
				this->currentRect = this->startRect;

				sprite->setTextureRect(currentRect);

				this->continous = continous;

				this->height = height;
				this->width = width;

			};
			~Animation(){
				delete sprite;
				delete texture;
				delete image;
			};
			void play(const float & dt){
				this->timer += 10*dt;
				if(this->timer >= this->animationTimer){
					this->timer = 0;

					//Animate
					if(this->currentRect != this->endRect){
						this->currentRect.top += this->height;

					}
					else{
						this->currentRect.top = this->endRect.top;
						if(!continous){
							stop = 1;
						}
						else{
							reset();
						}
					}

				}

				sprite->setTextureRect(currentRect);
			};
			void reset(){
				this->currentRect = this->startRect;
				this->timer = 0;
			};
	};

	public:
		std::map<std::string, Animation *> animations;
		AnimationComponent(double * x, double * y, double * angle);
		~AnimationComponent();
		void play(const std::string, const float &dt);
		void render(sf::RenderWindow * window);
		void reset(const std::string animation);
		void addAnimation(std::string F, float animationTimer, int start_frame_x, int start_frame_y, int frames_x, int frames_y, int width, int height, bool continous = 0);
		void play(const float &dt);
};

#endif