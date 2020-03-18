#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "code/Entity.h"
#include "code/Player.h"
#include "code/map.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "RGlike v.1.0"); // создаем окно 640х480

    sf::Clock clock;

    sf::View view;//объявили sfml объект "вид", который и является камерой
    view.reset(sf::FloatRect(0, 0, 640, 480));//размер видимой камерой области


    std::string hero = "resources/images/heroForRotate.png";//загрузка изображения стрелки и инициализация
    Player arrow(hero, 200, 200, 136, 74);
    arrow.setSpeed(0.25);


    map_init("resources/images/map.png");//инициализация карты


    while (window.isOpen())
    {
        sf::Event event;
        float time = clock.getElapsedTime().asMicroseconds();//поправка на системное время, чтобы не лагало
        clock.restart();
        time = time / 800;
        arrow.update(time, &window);//передвижение стрелки и её поворот



        window.clear(sf::Color(128, 106, 89));//фоновый цвет

        map_draw(&arrow, &window);//отрисовка карты с поправкой координаты 
        //map_check(&arrow);

        view.setCenter(arrow.x, arrow.y); //следим за игроком
        window.setView(view);

        arrow.sprite.setPosition(arrow.x, arrow.y);
        window.draw(arrow.sprite);

        sf::RectangleShape rect; //область столкновения стрелки
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