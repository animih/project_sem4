#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#include "code/map.h"
#include "code/Entity.h"
#include "code/Player.h"

void map_check(Entity *mob, RoomList * list) {

    double vect_x;
    double vect_y;

    int index = mob->index;

    bool helper = 0;

    // Кусок просто для отладки.

    
    printf("\n This: %d Nein: ", index);
                for(int v: list->graph->a[index])
                    printf("%d ", v);
    

    for (int u : list->graph->a[index]){

        list->Final[u].outline = 1;

        if( mob->y <= list->Final[u].y_bottom &&  mob->y >= list->Final[u].y_top && 
                mob->x <= list->Final[u].x_right &&  mob->x >= list->Final[u].x_left){
                mob->index = u;
            }
    }

    if(mob->cheat){
        return;
    }
    
    if(mob->x > list->Final[index].x_right || mob->x < list->Final[index].x_left){
       mob->x -= mob->dx;
    }
    
    if(mob->y > list->Final[index].y_bottom || mob->y < list->Final[index].y_top){
        mob->y -= mob->dy;
    }

};


int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "RGlike v. 0.0.7"); 

    sf::Clock clock;

    sf::View view;
    view.reset(sf::FloatRect(0, 0, 640, 480));

    /*

    Следующие 3 строчки отвечают за генерацию карты

    */
    double * a = new double[2];
    RoomList Rooms;
                        // Радиус распредления, средний размер и число комнат
    a = Rooms.make_map(50, 125, 95, &window); // Возвращает координаты комнаты 

    
    std::string hero = "resources/images/heroForRotate.png";
    Player arrow(hero, a[0], a[1], 136, 74);

    printf("%f, %f", a[0], a[1]);

    arrow.setSpeed(0.25);

    while (window.isOpen())
    {
        sf::Event event;
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;
        arrow.update(time, &window);

        window.clear(sf::Color(128, 106, 89));

        //Проверка перемещения
        map_check(&arrow, &Rooms);

        view.setCenter(arrow.x, arrow.y); 
        window.setView(view);

        //Прорисовка карты
        Rooms.DrawEdges(&window);

        arrow.sprite.setPosition(arrow.x, arrow.y);
        window.draw(arrow.sprite);

        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(16, 16));
        rect.setOutlineColor(sf::Color::Red);
        rect.setOrigin(8, 8);
        rect.setPosition(sf::Vector2f(arrow.x, arrow.y));

        window.draw(rect);
        window.display();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    };

    return 0;
};