

#include "map.h"
#include "A_star.h"


// ф-ция для записи конмат в матрицу.

void RoomList::transpose_to_matrix(std::vector<std::list <char>>& a){

	a = {};

	for(int j = 0; j < HEIGHT/tile_size*WIDTH/tile_size; j++){
		a.push_back(std::list<char>());
	}

	a.push_back(std::list<char>());

	for(int j = 0; j < HEIGHT/tile_size*WIDTH/tile_size; j++){
		a[j].push_back(-1);
	}
	


	int u1, v1, u2, v2;

	for(int i = 0; i < Final.size(); i ++){

		u1 = int(round(Final[i].x_left/tile_size));
		v1 = int(round(Final[i].y_top/tile_size));

		u2 = int(round(Final[i].x_right/tile_size));
		v2 = int(round(Final[i].y_bottom/tile_size));

		for(int k = u1; k <= u2; k++){
			a[v1+HEIGHT/tile_size*k].back() = 1;
			a[v2+HEIGHT/tile_size*k].back() = 1;
		}

		for(int k = v1; k <= v2; k++){
			a[k+HEIGHT/tile_size*u1].back() = 1;
			a[k+HEIGHT/tile_size*u2].back() = 1;
		}

		for(int j = v1+1; j < v2; j++){
			for(int k = u1+1; k < u2; k++)
				a[j+HEIGHT/tile_size*k].back() = 0;
		}

	}
}

Map::Map(int tile_size){
	this->tile_size = tile_size;
}

int Map::intersec(const double & x,const double & y){

	int i = int(round(y/tile_size))+int(round(x/tile_size))*HEIGHT/tile_size;
	return a[i].back();
}

void Map::render_all(RenderWindow * window){

	double x;
	double y;

	for(int i = 0; i < HEIGHT/tile_size; i ++){
		for(int j = 0; j < WIDTH/tile_size; j ++){

			x = j*tile_size;
			y = i*tile_size;

			RectangleShape tile(sf::Vector2f(tile_size, tile_size));
			tile.setPosition(x-tile_size/2, y-tile_size/2);	

			switch(a[i+HEIGHT/tile_size*j].back()){
				// 1 - стена (но не та, что альбом или книга Сартра)
				case 1:
					tile.setFillColor(sf::Color(255, 255, 255));
					window->draw(tile);
					break;
				// 0 - пустое пространство свободное для пересечения
				case 0:
					tile.setFillColor(sf::Color(55, 45, 65));
					window->draw(tile);
					break;
				// 2 - дверь
				case 2:
					tile.setFillColor(sf::Color(0, 139, 139));
					window->draw(tile);
					break;
				// 3 - вода
				case 3:
					tile.setFillColor(sf::Color(0, 255, 255));
					window->draw(tile);
					break;
				// 4 - трава/зелень. А вообще я не определился. Может быть это и плесень...
				case 4:
					tile.setFillColor(sf::Color(0, 128, 0));
					window->draw(tile);
					break;
				// 5 - обыкновенная тропинка / каменная плита
				case 5:
					tile.setFillColor(sf::Color(105, 105, 105));
					window->draw(tile);
					break;
				// 6 - лестница (в рай)
				case 6:
					tile.setFillColor(sf::Color(0, 0, 0));
					window->draw(tile);
					break;
				// факел (светильник)
				case 7:
					tile.setFillColor(sf::Color(255, 177, 0));
					window->draw(tile);
					break;
				// потухший факел (светильник)
				case 8:
					tile.setFillColor(sf::Color(155, 150, 0));
					window->draw(tile);
					break;
				// закрытая дверь
				case 9:
					tile.setFillColor(sf::Color(125, 255, 238));
					window->draw(tile);
					break;
				// какая-то ловушка
				case -2:
					tile.setFillColor(sf::Color(255, 25, 0));
					window->draw(tile);
					break;
				// спавн мобов/потенциальыне точки для их патрулирования
				case -3:
					tile.setFillColor(sf::Color(204, 115, 0));
					window->draw(tile);
					break;
				// моб в засаде (наелся в траве и спит.)
				case -4:
					tile.setFillColor(sf::Color(178, 34, 34));
					window->draw(tile);
					break;


				default:
					tile.setFillColor(sf::Color(0, 0, 0));

			}

		}
	}



}

void Map::render_region(RenderWindow * window, double x_left, double x_right, double y_top, double y_bottom, bool mask){

	double x;
	double y;

	int j1 = int(round(x_left/tile_size));
	int j2 = int(round(x_right/tile_size));


	int i1 = int(round(y_top/tile_size));
	int i2 = int(round(y_bottom/tile_size));

	if(j1 < 0)
		j1 = 0;
	if(i1 < 0)
		i1 = 0;

	if(i2 > HEIGHT/tile_size-1)
		i2 = HEIGHT/tile_size-1;
	if(i1 > WIDTH/tile_size-1)
		i1 = WIDTH/tile_size-1;

	int a = 255;

	bool liten = 0;

	RectangleShape tile(sf::Vector2f(tile_size, tile_size));

	for(int i = i1; i <= i2; i ++){
		for(int j = j1; j <= j2; j ++){

			if(mask){

				if(render_mask.count(i+HEIGHT/tile_size*j) == 0)
					continue;
				else if (env_lighting_mask.count(i+HEIGHT/tile_size*j) == 0){
					if(player_lighting_mask.count(i+HEIGHT/tile_size*j) == 0){
						continue;
					}
					else
						a = player_lighting_mask[i+HEIGHT/tile_size*j];

					liten = 0;
				}
				else{
					a = env_lighting_mask[i+HEIGHT/tile_size*j];
					liten = 1;
				}
			}

			x = j*tile_size;
			y = i*tile_size;
			tile.setPosition(x-tile_size/2, y-tile_size/2);	

			switch(this->a[i+HEIGHT/tile_size*j].back()){
				// 1 - стена (но не та, что альбом или книга Сартра)
				case 1:
					if(env_lighting_mask.count((i1+i2)/2+HEIGHT/tile_size*(j1+j2)/2) == 0){
						liten = 0;
						if(player_lighting_mask.count(i+HEIGHT/tile_size*j) == 0){
							continue;
						}
						else
							a = player_lighting_mask[i+HEIGHT/tile_size*j];
					}
					tile.setFillColor(sf::Color(155, 155, 155, a));
					window->draw(tile);

					break;
				
				// 0 - пустое пространство свободное для пересечения
				case 0:
					tile.setFillColor(sf::Color(55, 45, 65, a));
					window->draw(tile);
					break;
				// 2 - дверь
				case 2:
					if(env_lighting_mask.count((i1+i2)/2+HEIGHT/tile_size*(j1+j2)/2) == 0){
						liten = 0;
						if(player_lighting_mask.count(i+HEIGHT/tile_size*j) == 0){
							continue;
						}
						else
							a = player_lighting_mask[i+HEIGHT/tile_size*j];
					}
					tile.setFillColor(sf::Color(0, 139, 139, a));
					window->draw(tile);
					break;
				// 3 - вода
				case 3:
					tile.setFillColor(sf::Color(0, 255, 255, a));
					window->draw(tile);
					break;
				// 4 - трава/зелень. А вообще я не определился. Может быть это и плесень...
				case 4:
					tile.setFillColor(sf::Color(0, 128, 0, a));
					window->draw(tile);
					break;
				// 5 - обыкновенная тропинка / каменная плита
				case 5:
					tile.setFillColor(sf::Color(105, 105, 105, a));
					window->draw(tile);
					break;
				// 6 - лестница (в рай)
				case 6:
					tile.setFillColor(sf::Color(0, 0, 0, a));
					window->draw(tile);
					break;
				// факел (светильник)
				case 7:
					tile.setFillColor(sf::Color(255, 177, 0, a));
					window->draw(tile);
					break;
				// потухший факел (светильник)
				case 8:
					tile.setFillColor(sf::Color(155, 150, 0, a));
					window->draw(tile);
					break;
				// закрытая дверь
				case 9:
					tile.setFillColor(sf::Color(125, 255, 238, a));
					window->draw(tile);
					break;
				// какая-то ловушка
				case -2:
					tile.setFillColor(sf::Color(255, 25, 0, a));
					window->draw(tile);
					break;
				// спавн мобов/потенциальыне точки для их патрулирования
				case -3:
					tile.setFillColor(sf::Color(204, 115, 0, a));
					window->draw(tile);
					break;
				// моб в засаде (наелся в траве и спит.)
				case -4:
					tile.setFillColor(sf::Color(55, 45, 65, a));
					window->draw(tile);
					break;


				default:
					tile.setFillColor(sf::Color(0, 0, 0));
					window->draw(tile);
					if(env_lighting_mask.count((i1+i2)/2+HEIGHT/tile_size*(j1+j2)/2) == 0)
						liten = 0;

			}

			if(liten){

				tile.setFillColor(sf::Color(225, 205, 55, a/3));
				window->draw(tile);
			}

		}
	}



}

// Прокладываем кориоры

void Map::build_Hallways(){


	for(int i = 0; i < Rooms->Final.size(); i++){

		for(int u : Rooms->graph->a[i]){

			if(u <= i){
				continue;
			}
			int x1 = int(round(((Rooms->Final[i].x_left+Rooms->Final[i].x_right)/2)/tile_size));
			int y1 = int(round(((Rooms->Final[i].y_top+Rooms->Final[i].y_bottom)/2)/tile_size));

			int x2 = int(round(((Rooms->Final[u].x_left+Rooms->Final[u].x_right)/2)/tile_size));
			int y2 = int(round(((Rooms->Final[u].y_top+Rooms->Final[u].y_bottom)/2)/tile_size));

			std::vector<int> res = A_star(x1, y1, x2, y2, a, HEIGHT/tile_size, WIDTH/tile_size); // Могучий A* решает всё.

			for(int j = 0; j < res.size(); j+=2){
				if(a[res[j+1]+res[j]*HEIGHT/tile_size].back() == 1 ){
					if( a[res[j+1+2]+res[j+2]*HEIGHT/tile_size].back() != 1)
						if(Rooms->Final[i].color == 6){
							a[res[j+1]+res[j]*HEIGHT/tile_size].back() = 9; // закрытая комната
						}
						else{
							a[res[j+1]+res[j]*HEIGHT/tile_size].back() = 2;
						}
					else
						a[res[j+1]+res[j]*HEIGHT/tile_size].back() = 0;
				}
				else if(a[res[j+1]+res[j]*HEIGHT/tile_size].back() == -1){

					a[res[j+1]+res[j]*HEIGHT/tile_size].back() = 0;

				}

			}

			for(int j = 0; j < res.size(); j+=2){

				for(int k = 0; k <3; k++){
					for(int h = 0; h <3; h++){
						if(h==0 & k == 0){
							continue;
						}

						if(a[res[j+1]-1+k+(res[j]-1+h) * HEIGHT/tile_size].back() == -1){
							a[res[j+1]-1+k+(res[j]-1+h) * HEIGHT/tile_size].back() = 1;
						}

					}

				}

			}


		}
		
	}
}

/*
	Генерация окружения.

	Тут идея очень проста, у каждой комнаты - цвет маркер,
	который раздавался так, чтоыб не было двух соседних комнат
	одного цвета (чтобы игрвоой процесс был разнообразней =) )

	Изначально я предплагал такое распределние:

	1- Синий цвет. Генерация водоёмов, колон и, возможно, 
	каких-нибдуь речных мобов, если такие будут (крабы?)
	2 - Зелёный цвет. Генерация травы и каких-нибудь растений. Ещё тут есть колоны (зачем хз)
	сюда можно засунуть (при редком случае) каких-нибдуь мобов в засаде
	3 - Жёлтый цвет - абсолютно рандомная генерация. Возможны сундуки с лутом.
	4 - Комнаты, представляющие наибольшую опасность:
		ловушки/мобы
	5 - Комнаты с числом соседних > 3. Одна из таких комнат
	должна иметь лестницу, другие можно приспосбить под какую-нибдуь
	сокровищницу/декоративную площадку
	6 - комнаты с числом соседних = 1. Потенциальные тайные/закрытые комнаты.

	потому просбюа подробно расписывать метод генерации и собсвтенно сам объект,
	что кидается в комнату. Желаю весело провести время =)

*/



void Map::Gen_Sur(std::map<std::string, std::vector<double>> & buf){

	auto rng = std::default_random_engine {};

	bool flag_for_locked = 0;
	int counter_of_traps = Rooms->Final.size()/2;

	for(int i = 1; i< Rooms->Final.size(); i ++){

		printf("%d ", i);

		/*

			В "синих" комнатах гененрирую воду:

			Вероятность - 50 %
			выбираю радномную точку в пределах комнаты
			и A* строю из неё путь в соседние комнаты.
			Далее этот путь обрываю до дверей, а там
			даю воде совсем чуть-чуть растечься.

			Весь путь я позднее обриосвываю толще

		*/

		if(Rooms->Final[i].color == 1){

			if(rand()%2){
				continue; // 50 % - вероятность провала
			}

			for(int u : Rooms->graph->a[i]){

			int x1_left = int(round(((Rooms->Final[i].x_left)/tile_size)));
			int x1_right = int(round(((Rooms->Final[i].x_right)/tile_size)));

			int y1_top = int(round(((Rooms->Final[i].y_top)/tile_size)));
			int y1_bottom = int(round(((Rooms->Final[i].y_bottom)/tile_size)));

			// Случайная точка в исходной комнате
			int x1 = rand()%(x1_right-x1_left)+x1_left+1;
			int y1 = rand()%(y1_bottom-y1_top)+y1_top+1;
			// центр комнат соседней
			int x2 = int(round(((Rooms->Final[u].x_left+Rooms->Final[u].x_right)/2)/tile_size));
			int y2 = int(round(((Rooms->Final[u].y_top+Rooms->Final[u].y_bottom)/2)/tile_size));

			std::vector<int> res = A_star_inside(x1, y1, x2, y2, a, HEIGHT/tile_size, WIDTH/tile_size); // A_star но уже внутренний, который не идёт по стенам
			int counter = 0;

			// Сперва рисую на пустых клетках прямо на пути.
			// Если встречаю дверь - останавливаюсь

			for(int j = 0; j < res.size(); j+=2){
				if(a[res[j+1]+res[j]*HEIGHT/tile_size].back() == 0){
					a[res[j+1]+res[j]*HEIGHT/tile_size].push_back(3);
				}
				counter += 2;
				if(a[res[j+1]+res[j]*HEIGHT/tile_size].back() == 2 || a[res[j+1]+res[j]*HEIGHT/tile_size].back() == 9){
					break;
				}

			}

			// Обрисовываю оборванный путь в 3-окрестности. НА выход за пределы комнаты - пофиг (это типо двери протекают)

			for(int j = 0; j < counter; j+=2){

				for(int k = 0; k <5; k++){
					for(int h = 0; h <5; h++){
						if(h==0 & k == 0 || abs(k-2)+abs(h-2) > 4){
							continue;
						}

						if(a[res[j+1]-3+k+(res[j]-3+h) * HEIGHT/tile_size].back() == 0){
							a[res[j+1]-3+k+(res[j]-3+h) * HEIGHT/tile_size].push_back(3);
						}

					}

				}

			}


			if(rand()%3 == 0 && counter_of_traps > 0){

					bool flag_exit_mob = 0;
					for(int k = 0; k < 3; k++){
						if(flag_exit_mob){
							break;
						}
						for(int v = 0; v < 3; v++){
							if(abs(v-1)+abs(k-1) == 2)
								continue;
							if(a[y1+x1*HEIGHT/tile_size].back() != 1 && a[y1+x1*HEIGHT/tile_size].back() != 2){
								buf["Zombie"].push_back(x1*tile_size);
								buf["Zombie"].push_back(y1*tile_size);

								printf("Zombie added to map! \n");
								flag_exit_mob = 1;
								break;
							}

						}
					}
				}
			}



		}

		/*

			В зелённых комнатах генерирую траву. 
			Выбираю рандомно 0-3 "очага" генерации.
			Затем в прямоугольной окрестности с вероятностью 1/2 генерю тайл травы
			Генерация происходит либов  пустом месте, либо на воде (я подумал, что так логично)
			На выход за пределы комнаты - забиваю, т.к. это выглядит даже реалистичней

		*/

		if(Rooms->Final[i].color == 2){

			int counter2 = rand()%4; // число "очагов"

			int x_left = int(round(((Rooms->Final[i].x_left)/tile_size)));
			int x_right = int(round(((Rooms->Final[i].x_right)/tile_size)));

			int y_top = int(round(((Rooms->Final[i].y_top)/tile_size)));
			int y_bottom = int(round(((Rooms->Final[i].y_bottom)/tile_size)));

			for(int l = 0; l < counter2; l++){

				int x = rand()%(x_right-x_left)+x_left+1; // Координата центра
				int y = rand()%(y_bottom-y_top)+y_top+1; // Координата центра

				if(x+3 > WIDTH || y+3 > HEIGHT || x-3 < 0 || y -3 < 0)
					continue;

				for(int k = 0; k < 7; k++){
					for(int h = 0; h < 7; h++){
					if(rand()%2){ // Успех = кидаю травку

						if(a[y-4+k+(x-4+h) * HEIGHT/tile_size].back() == 0 || a[y-4+k+(x-4+h) * HEIGHT/tile_size].back() == 3){
							a[y-4+k+(x-4+h) * HEIGHT/tile_size].push_back(4);
						}

					}
					}
				}

				if(rand()%2 == 0 && counter_of_traps > 0){

					bool flag_exit_hid_mob = 0;
					for(int k = 0; k < 3; k++){
						if(flag_exit_hid_mob){
							break;
						}
						for(int v = 0; v < 3; v++){
							if(abs(v-1)+abs(k-1) == 2)
								continue;
							if(a[y+x*HEIGHT/tile_size].back() != 1 && a[y+x*HEIGHT/tile_size].back() != 2){
								if(rand()%3 == 0){
									buf["Ward"].push_back(x*tile_size);
									buf["Ward"].push_back(y*tile_size);
									printf("Ward added to map! \n");
								}
								else{
								buf["Zombie"].push_back(x*tile_size);
								buf["Zombie"].push_back(y*tile_size);
								printf("Zombie added to map! \n");
								}
								counter_of_traps -= 1;
								flag_exit_hid_mob = 1;
								break;
							}

						}
					}
				}


			}

		}

		/*
	
			В зелённые и синие и жёлтые комнаты с вероятность 1/2 пихаю колонны 
			(объект размером 3x3, по бокам стенки, в центре - пусто)

			Объязательно проверю, чтобы моя колонна не загородила проход или примкунла к стене
			Если такое происходит - просто заканчиваю попытку (поэтому реально на карте оюычно больше 2 колонн не увидишь)


		*/

		if(Rooms->Final[i].color == 3 || Rooms->Final[i].color == 2 || Rooms->Final[i].color == 1){

			if(rand()%2)
				continue;

			int value = rand()%3;

			int x_left = int(round(((Rooms->Final[i].x_left)/tile_size)));
			int x_right = int(round(((Rooms->Final[i].x_right)/tile_size)));

			int y_top = int(round(((Rooms->Final[i].y_top)/tile_size)));
			int y_bottom = int(round(((Rooms->Final[i].y_bottom)/tile_size)));

			for(int l = 0; l < value; l++){

				int x = rand()%(x_right-x_left-3)+x_left+3;
				int y = rand()%(y_bottom-y_top-3)+y_top+3;

				bool flag1 = 0;

				for(int k = 0; k < 7; k++){
					for(int h = 0; h < 7; h++){
						if(a[y-4+k+(x-4+h) * HEIGHT/tile_size].back() == 2 || a[y-4+k+(x-4+h) * HEIGHT/tile_size].back() == 1){
							flag1 = 1;
							break;
						}
					}
					if(flag1)
						break;
				}

				if(flag1){
					continue;
				}

			

				for(int k = 0; k < 3; k++){
					for(int h = 0; h < 3; h++){
						if(k != 1 || h!= 1){
							a[y-1+k+(x-1+h) * HEIGHT/tile_size].back() = 1;
						}
						
					}

				}

				a[x+y * HEIGHT/tile_size].back() = -1;

			}

		}

		if(Rooms->Final[i].color != 1){


			int x_left = int(round(((Rooms->Final[i].x_left)/tile_size)));
			int x_right = int(round(((Rooms->Final[i].x_right)/tile_size)));

			int y_top = int(round(((Rooms->Final[i].y_top)/tile_size)));
			int y_bottom = int(round(((Rooms->Final[i].y_bottom)/tile_size)));

			if(abs(x_left-x_right) < 5 || abs(y_top-y_bottom) < 5)
					continue;

			if(rand()%3){

				int x;
				int y;

				std::vector<int> b = {0, 1, 2, 3};

				std::shuffle(b.begin(), b.end(), rng);

				for(int i = 0; i < 4; i++){
					switch(b[i]){
						case 0:
							x = x_left+2;
							y = y_top+2;
							break;
						case 1:
							x = x_left+2;
							y = y_bottom-2;
							break;
						case 2:
							x = x_right-2;
							y = y_bottom-2;
							break;
						case 3:
							x = x_right-2;
							y = y_top-2;
							break;

					}

					if(a[y+x*HEIGHT/tile_size].back() == 0 || a[y+x*HEIGHT/tile_size].back() == 3 || a[y+x*HEIGHT/tile_size].back() == 4){
						buf["Lantern"].push_back(x*tile_size);
						buf["Lantern"].push_back(y*tile_size);

						printf("Lantern added to map! \n");
						break;
					}


				}

			}
			else if(Rooms->Final[i].color == 3){

				int x_left = int(round(((Rooms->Final[i].x_left)/tile_size)));
				int x_right = int(round(((Rooms->Final[i].x_right)/tile_size)));

				int y_top = int(round(((Rooms->Final[i].y_top)/tile_size)));
				int y_bottom = int(round(((Rooms->Final[i].y_bottom)/tile_size)));

				if(abs(x_left-x_right) < 5 || abs(y_top-y_bottom) < 5)
					continue;

				if(rand()%3 == 0){
					int x1 = (x_right+x_left)/2;
					int y1 = (y_top+y_bottom)/2;
					int x, y;

					std::vector<int> b = {0, 1, 2, 3};

					std::shuffle(b.begin(), b.end(), rng);

					for(int i = 0; i < 6; i++){
						switch(b[i]){
						case 0:
							x = x1+2;
							y = y1+2;
							break;
						case 1:
							x = x1+2;
							y = y1-2;
							break;
						case 2:
							x = x1-2;
							y = y1-2;
							break;
						case 3:
							x = x1-2;
							y = y1-2;
							break;

					}

					

					if(a[y+x*HEIGHT/tile_size].back() == 0 || a[y+x*HEIGHT/tile_size].back() == 3 || a[y+x*HEIGHT/tile_size].back() == 4){
						buf["Lantern"].push_back(x*tile_size);
						buf["Lantern"].push_back(y*tile_size);

						printf("Lantern added to map! \n");
						break;
					}
					}

				}

			}
		}
		
		if(Rooms->Final[i].color == 6 && !flag_for_locked){

			int u1 = int(round(Rooms->Final[i].x_left/tile_size));
			int v1 = int(round(Rooms->Final[i].y_top/tile_size));

			int u2 = int(round(Rooms->Final[i].x_right/tile_size));
			int v2 = int(round(Rooms->Final[i].y_bottom/tile_size));

			for(int k = u1+1; k <= u2-1; k++){
				for(int l = v1+1; l <= v2-1; l++){
				a[l+HEIGHT/tile_size*k].back() = 5;
				a[l+HEIGHT/tile_size*k].back() = 5;
				}
			}

		}

		if(Rooms->Final[i].color == 4){

			int u1 = int(round(Rooms->Final[i].x_left/tile_size));
			int v1 = int(round(Rooms->Final[i].y_top/tile_size));

			int u2 = int(round(Rooms->Final[i].x_right/tile_size));
			int v2 = int(round(Rooms->Final[i].y_bottom/tile_size));


			if(abs(u2-u1) < 5 || abs(v1-v2) < 5)
				continue;

			int x_w;
			int y_w;

			switch(rand()%4){
				case 0:
					x_w = u1+2;
					y_w = v1+2;
					break;
				case 1:
					x_w = u1+2;
					y_w = v2-2;
					break;
				case 2:
					x_w = u2-2;
					y_w = v1+2;
					break;
				case 3:
					x_w = u2-2;
					y_w = v2-2;
					break;

			}
			if(a[y_w+x_w*HEIGHT/tile_size].back() == 0){
				buf["Ward"].push_back(x_w*tile_size);
				buf["Ward"].push_back(y_w*tile_size);
			}
			printf("Ward added to map! \n");


			for(int i = 0; i < rand()%3+1; i++){

				int x = rand()%(u2-u1)+u1;
				int y = rand()%(v2-v1)+v1;

				while(x == x_w && y == y_w){
					x = rand()%(u2-u1)+u1;
					y = rand()%(v2-v1)+v1;
				}	


				if(a[y+x*HEIGHT/tile_size].back() == 0){
					buf["Zombie"].push_back(x*tile_size);
					buf["Zombie"].push_back(y*tile_size);

					a[y+x*HEIGHT/tile_size].push_back(-4);

					printf("Zombie added to map! \n");
				}

			}




		}


	}

	/*

		С веротяностью 1/6 в каждой комнате прокладываю дороги от дверей.
		Дороги строются через A* из них отбираются самые длинные (просто жадный алгоритм, тут я не считаю общую длину путей)

		дороги нужны просто для декора и другие тайлы не перекрашивают.

	*/

	for(int i = 0; i< Rooms->Final.size(); i ++){

		if(rand()%3==0){

			if(Rooms->graph->a[i].size() < 2){
				continue;
			}

			int x_left = int(round(((Rooms->Final[i].x_left)/tile_size)));
			int x_right = int(round(((Rooms->Final[i].x_right)/tile_size)));

			int y_top = int(round(((Rooms->Final[i].y_top)/tile_size)));
			int y_bottom = int(round(((Rooms->Final[i].y_bottom)/tile_size)));

			std::vector<int> exits;

			for(int k = 0; k < y_bottom-y_top; k++){
				if(a[y_top+k+x_left* HEIGHT/tile_size].back() == 0 || a[y_top+k+x_left* HEIGHT/tile_size].back() == 2){

					exits.push_back(x_left);
					exits.push_back(y_top+k);

				}

				if(a[y_top+k+x_right* HEIGHT/tile_size].back() == 0 || a[y_top+k+x_right* HEIGHT/tile_size].back() == 2){

					exits.push_back(x_right);
					exits.push_back(y_top+k);

				}

			}

			for(int k = 0; k < x_right-x_left; k++){
				if(a[y_top+(x_left+k)* HEIGHT/tile_size].back() == 0 || a[y_top+(x_left+k)* HEIGHT/tile_size].back() == 2){

					exits.push_back(x_left+k);
					exits.push_back(y_top);

				}

				if(a[y_bottom+(x_left+k)* HEIGHT/tile_size].back() == 0 || a[y_bottom+(x_left+k)* HEIGHT/tile_size].back() == 2){

					exits.push_back(x_left+k);
					exits.push_back(y_bottom);

				}

			}

			std::vector <int> buf = {};
			std::vector <int> cur;

			int end = exits.size();

			//printf("%d \n", end);
			
			for(int k = 0; k <exits.size(); k+=2){
				if(exits[k] == -1){
						continue;
				}
				int best_size = 0;
				int best_u = 0;
				for(int u = k+2; u <exits.size(); u+=2){

					if(exits[u] == -1){
						continue;
					}

					cur = A_star_inside(exits[k], exits[k+1], exits[u], exits[u+1], a, HEIGHT/tile_size, WIDTH/tile_size);

					if(cur.size() > best_size){
						buf = cur;
						best_size = cur.size();
						best_u = u;
					}

					break;
				}

				for(int u = 0; u < buf.size(); u+=2){

					if(a[buf[u+1]+buf[u]*HEIGHT/tile_size].back() == 0){
						a[buf[u+1]+buf[u]*HEIGHT/tile_size].back() = 5;
					}

				}

				exits[best_u] = -1;

			}
			

		}


	}


}
/*

Респавн мобов у меня работает буквально в границе экрана (т.е. в армке ширинйо один тайл за обзором). 
Это очень класс решает проблему оптимищации, но если точка спавна окажется сразу же в пределах видимости игрока при начале игры, то
она србаотет только если от неё отдалится =(
Хорошим решением была бы единичная респа всех мобов в пределе видимотси, но мне лень это писать.

*/

void Map::check_Spawn(std::map<std::string , std::vector<double>> & buf, const double & x, const double & y, double range){

	int j1 = int(round(x-range)/tile_size);
	int i1 = int(round(y-range)/tile_size);

	int j2 = int(round(x+range)/tile_size);
	int i2 = int(round(y+range)/tile_size);


	if(i1 < 0)
		i1 = 0;
	if(j1 < 0)
		j1 = 0;

	if(i2 > HEIGHT/tile_size-1)
		i2 = HEIGHT/tile_size-1;
	if(i2 > WIDTH/tile_size-1)
		j2 = WIDTH/tile_size-1;

	

		for(int j = j1; j < j2; j++){

			if(a[i1+j*HEIGHT/tile_size].back()==-4){
			a[i1+j*HEIGHT/tile_size].pop_back();
			buf["Zombie"].push_back(j*tile_size);
			buf["Zombie"].push_back(i1*tile_size);
			printf("%d %d \n", i1, j);
			}

		}

		for(int j = j1; j < j2; j++){

			if(a[i2+j*HEIGHT/tile_size].back()==-4){
			a[i2+j*HEIGHT/tile_size].pop_back();
			buf["Zombie"].push_back(j*tile_size);
			buf["Zombie"].push_back(i2*tile_size);
			printf("%d %d \n", i2, j);
			}

		}

		for(int i = i1+1; i < i2-1; i++){

			if(a[i+j1*HEIGHT/tile_size].back()==-4){
			a[i+j1*HEIGHT/tile_size].pop_back();
			buf["Zombie"].push_back(j1*tile_size);
			buf["Zombie"].push_back(i*tile_size);
			printf("%d %d \n", i, j1);
			}

		}

		for(int i = i1+1; i < i2-1; i++){

			if(a[i+j2*HEIGHT/tile_size].back()==-4){
			a[i+j2*HEIGHT/tile_size].pop_back();
			buf["Zombie"].push_back(j2*tile_size);
			buf["Zombie"].push_back(i*tile_size);
			printf("%d %d \n", i, j2);
			}

		}



}

// просто мне нужна ф-ция для определния знака

int sign(int a){
	if(a > 0){
		return 1;
	}
	else if(a < 0){
		return -1;
	}
	else
		return 0;

}
/*

 так, ну вот этот метод определяет пересекает ли прямая,
  соединяющая две произвольные точки какой-нибдуь непрозрачный тайл (стена или дверь)

  простым словами, тут обрисовывается "клетками прямая (причём точно, я намернно тестил алгос)

 */

bool Map::if_visible(double x1, double y1, double x2, double y2, bool seen){

	

	int u1 = int((x1/tile_size));
	int u2 = int((x2/tile_size));


	int v1 = int(y1/tile_size);
	int v2 = int(y2/tile_size);

	int counter = abs(u1-u2)+abs(v1-v2)+1;

	if(counter == 1){
		return true;
	}

	int tileStepX;
	int tileStepY;

	tileStepX = sign(u2-u1);
	tileStepY = sign(v2-v1);

	double intersectXstep, nextIntersectX;
	double intersectYstep, nextIntersectY;

	if(u1-u2 == 0){
		intersectXstep = 6666;
		nextIntersectX = 0;
	}
	else{
		double startXFraction = x1 - floor(x1);
		if(u1-u2>0){
			intersectXstep = 1./(u1-u2);
			nextIntersectX =(1-startXFraction )*intersectXstep;}
		else{
			intersectXstep = -1./(u1-u2);
			nextIntersectX =(startXFraction )*intersectXstep;}
	}

	if(v1-v2 == 0){
		intersectYstep = 6666;
		nextIntersectY = 0;
	}
	else{
		double startYFraction = y1 - floor(y1);
		if(v1-v2>0){
			intersectYstep = 1./(v1-v2);
			nextIntersectY =(1-startYFraction )*intersectYstep;}
		else{
			intersectYstep = -1./(v1-v2);
			nextIntersectY =(startYFraction)*intersectYstep;}
	}

	for(int i = 0; i < counter; i++){
		if(nextIntersectX < nextIntersectY){
			u1 += tileStepX;
			nextIntersectX += intersectXstep;}
		else{
			v1 += tileStepY;
			nextIntersectY += intersectYstep;}

		if(a[v1+u1*HEIGHT/tile_size].back() == 1 || (!seen && a[v1+u1*HEIGHT/tile_size].back() == 2)){
			return false;
		}

	}

	return true;

}

// Просто костыль для возврата моба на карту в качестве точки спавна

void Map::push_mob_back(int id, const double & x, const double & y){


	this->a[int(round(y/tile_size))+int(round(x/tile_size))*HEIGHT/tile_size].push_back(id);


}
