#include "map.h"
#include <functional>
#include <math.h>
#include "A_star.h"
#include <stdio.h>
#include <stdlib.h>


int * RoomList::transpose_to_matrix(){

	int * a = new int[HEIGHT/tile_size*WIDTH/tile_size];

	for(int j = 0; j < HEIGHT/tile_size*WIDTH/tile_size; j++){
		a[j] = -1;
	}

	int u1, v1, u2, v2;

	for(int i = 0; i < Final.size(); i ++){

		u1 = int(round(Final[i].x_left/tile_size));
		v1 = int(round(Final[i].y_top/tile_size));

		u2 = int(round(Final[i].x_right/tile_size));
		v2 = int(round(Final[i].y_bottom/tile_size));

		for(int k = u1; k <= u2; k++){
			a[v1+HEIGHT/tile_size*k] = 1;
			a[v2+HEIGHT/tile_size*k] = 1;
		}

		for(int k = v1; k <= v2; k++){
			a[k+HEIGHT/tile_size*u1] = 1;
			a[k+HEIGHT/tile_size*u2] = 1;
		}

		for(int j = v1+1; j < v2; j++){
			for(int k = u1+1; k < u2; k++)
				a[j+HEIGHT/tile_size*k] = 0;
		}


	}

	return a;

}

Map::Map(int tile_size){
	this->tile_size = tile_size;
}

int Map::intersec(double x, double y){

	int i = int(round(y/tile_size))+int(round(x/tile_size))*WIDTH/tile_size;

	return a[i];
}

void Map::update_all(RenderWindow * window){

	double x;
	double y;

	for(int i = 0; i < HEIGHT/tile_size; i ++){
		for(int j = 0; j < WIDTH/tile_size; j ++){

			x = j*tile_size;
			y = i*tile_size;

			RectangleShape tile(sf::Vector2f(tile_size, tile_size));
			tile.setPosition(x-tile_size/2, y-tile_size/2);	

			switch(a[i+HEIGHT/tile_size*j]){
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


				default:
					tile.setFillColor(sf::Color(0, 0, 0));

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
				if(a[res[j+1]+res[j]*HEIGHT/tile_size] == 1 ){
					if( a[res[j+1+2]+res[j+2]*HEIGHT/tile_size] != 1)
						a[res[j+1]+res[j]*HEIGHT/tile_size] = 2;
					else
						a[res[j+1]+res[j]*HEIGHT/tile_size] = 0;
				}
				else if(a[res[j+1]+res[j]*HEIGHT/tile_size] == -1){

					a[res[j+1]+res[j]*HEIGHT/tile_size] = 0;

				}

			}

			for(int j = 0; j < res.size(); j+=2){

				for(int k = 0; k <3; k++){
					for(int h = 0; h <3; h++){
						if(h==0 & k == 0){
							continue;
						}

						if(a[res[j+1]-1+k+(res[j]-1+h) * HEIGHT/tile_size] == -1){
							a[res[j+1]-1+k+(res[j]-1+h) * HEIGHT/tile_size] = 1;
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

	Я надеюсь, что у нас получится вместе писать генерацию тайлов окружения,
	потому просбюа подробно расписывать метод генерации и собсвтенно сам объект,
	что кидается в комнату. Желаю весело провести время =)

*/



void Map::Gen_Sur(){

	for(int i = 0; i< Rooms->Final.size(); i ++){

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

			// Случайная точка в исхожной комнате
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
				if(a[res[j+1]+res[j]*HEIGHT/tile_size] == 0){
					a[res[j+1]+res[j]*HEIGHT/tile_size] = 3;
				}
				counter += 2;
				if(a[res[j+1]+res[j]*HEIGHT/tile_size] == 2){
					break;
				}

			}

			// Обрисовываю оборванный путь в 3-окрестности. НА выход за пределы комнаты - пофиг (это типо двери протикают)

			for(int j = 0; j < counter; j+=2){

				for(int k = 0; k <5; k++){
					for(int h = 0; h <5; h++){
						if(h==0 & k == 0 || abs(k-2)+abs(h-2) > 4){
							continue;
						}

						if(a[res[j+1]-3+k+(res[j]-3+h) * HEIGHT/tile_size] == 0){
							a[res[j+1]-3+k+(res[j]-3+h) * HEIGHT/tile_size] = 3;
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

				for(int k = 0; k < 7; k++){
					for(int h = 0; h < 7; h++){
					if(rand()%2){ // Успех = кидаю травку

						if(a[y-4+k+(x-4+h) * HEIGHT/tile_size] == 0 || a[y-4+k+(x-4+h) * HEIGHT/tile_size] == 3){
							a[y-4+k+(x-4+h) * HEIGHT/tile_size] = 4;
						}

					}
					}
				}


			}

		}

		/*
	
			В зелённые и синие комнаты с вероятность 1/2 пихаю колонны 
			(объект размером 3x3, по бокам стенки, в центре - пусто)

			Объязательно проверю, чтобы моя колонна не загородила проход или примкунла к стене
			Если такое происходит - просто заканчиваю попытку (поэтому реально на карте оюычно больше 2 колонн не увидишь)


		*/

		if(Rooms->Final[i].color == 3 || Rooms->Final[i].color == 2){

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
						if(a[y-4+k+(x-4+h) * HEIGHT/tile_size] == 2 || a[y-4+k+(x-4+h) * HEIGHT/tile_size] == 1){
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
							a[y-1+k+(x-1+h) * HEIGHT/tile_size] = 1;
						}
						
					}

				}

				a[x+y * HEIGHT/tile_size] = -1;

			}

		}




	}

	/*

		С веротяностью 1/6 в каждой комнате прокладываю дороги от дверей.
		Дороги строются через A* из них отбираются самые длинные (просто жадный алгоритм, тут я не считаю общую длину путей)

		дороги нужны просто для декора и другие тайлы не перекрашивают.

	*/

	for(int i = 0; i< Rooms->Final.size(); i ++){

		if(rand()%6==0){

			if(Rooms->graph->a[i].size() < 2){
				continue;
			}

			int x_left = int(round(((Rooms->Final[i].x_left)/tile_size)));
			int x_right = int(round(((Rooms->Final[i].x_right)/tile_size)));

			int y_top = int(round(((Rooms->Final[i].y_top)/tile_size)));
			int y_bottom = int(round(((Rooms->Final[i].y_bottom)/tile_size)));

			std::vector<int> exits;

			for(int k = 0; k < y_bottom-y_top; k++){
				if(a[y_top+k+x_left* HEIGHT/tile_size] == 0 || a[y_top+k+x_left* HEIGHT/tile_size] == 2){

					exits.push_back(x_left);
					exits.push_back(y_top+k);

				}

				if(a[y_top+k+x_right* HEIGHT/tile_size] == 0 || a[y_top+k+x_right* HEIGHT/tile_size] == 2){

					exits.push_back(x_right);
					exits.push_back(y_top+k);

				}

			}

			for(int k = 0; k < x_right-x_left; k++){
				if(a[y_top+(x_left+k)* HEIGHT/tile_size] == 0 || a[y_top+(x_left+k)* HEIGHT/tile_size] == 2){

					exits.push_back(x_left+k);
					exits.push_back(y_top);

				}

				if(a[y_bottom+(x_left+k)* HEIGHT/tile_size] == 0 || a[y_bottom+(x_left+k)* HEIGHT/tile_size] == 2){

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

					if(a[buf[u+1]+buf[u]*HEIGHT/tile_size] == 0){
						a[buf[u+1]+buf[u]*HEIGHT/tile_size] = 5;
					}

				}

				exits[best_u] = -1;

			}
			

		}


	}


}