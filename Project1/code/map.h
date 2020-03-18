#pragma once
#include <SFML\Graphics.hpp>
#include "Entity.h"

const int HEIGHT_MAP = 25;
const int WIDTH_MAP = 40;

sf::Image map_image;
sf::Texture map;
sf::Sprite s_map;

void map_init(std::string name) { //загрузка тайлов для карты. Переводим картинку в текстуру, а текстуру в спрайт
	map_image.loadFromFile(name);
	map.loadFromImage(map_image);
	s_map.setTexture(map);
};

sf::String TileMap[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000",
	"0                                      0",
	"0   s                                  0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0000000000000000000000000000000000000000",
};

void map_draw(Entity *mob, sf::RenderWindow *window) {
	for (int i = 0; i < HEIGHT_MAP; i++) {
		for (int j = 0; j < WIDTH_MAP; j++) {
			if (TileMap[i][j] == ' ')  s_map.setTextureRect(sf::IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
			if (TileMap[i][j] == 's')  s_map.setTextureRect(sf::IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
			if ((TileMap[i][j] == '0')) s_map.setTextureRect(sf::IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик

			s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту
			sf::FloatRect MapCollision = s_map.getGlobalBounds();

			mob->setCollision(mob->x, mob->y - mob->dy, 16, 16);
			if (MapCollision.intersects(mob->collisionbox) && ((TileMap[i][j] == 's') || (TileMap[i][j] == '0'))) {
				mob->x -= mob->dx;
			};
			mob->setCollision(mob->x, mob->y, 16, 16);
			if (MapCollision.intersects(mob->collisionbox) && ((TileMap[i][j] == 's') || (TileMap[i][j] == '0'))) {
				mob->y -= mob->dy;
			};
			//границы карты
			window->draw(s_map);
			
		};
	};
};