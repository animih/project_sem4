#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "code/Entity.h"
#include "code/Player.h"
#include "code/map.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "RGlike v.1.0"); // ������� ���� 640�480

    sf::Clock clock;

    sf::View view;//�������� sfml ������ "���", ������� � �������� �������
    view.reset(sf::FloatRect(0, 0, 640, 480));//������ ������� ������� �������


    std::string hero = "resources/images/heroForRotate.png";//�������� ����������� ������� � �������������
    Player arrow(hero, 200, 200, 136, 74);
    arrow.setSpeed(0.25);


    map_init("resources/images/map.png");//������������� �����


    while (window.isOpen())
    {
        sf::Event event;
        float time = clock.getElapsedTime().asMicroseconds();//�������� �� ��������� �����, ����� �� ������
        clock.restart();
        time = time / 800;
        arrow.update(time, &window);//������������ ������� � � �������



        window.clear(sf::Color(128, 106, 89));//������� ����

        map_draw(&arrow, &window);//��������� ����� � ��������� ���������� 
        //map_check(&arrow);

        view.setCenter(arrow.x, arrow.y); //������ �� �������
        window.setView(view);

        arrow.sprite.setPosition(arrow.x, arrow.y);
        window.draw(arrow.sprite);

        sf::RectangleShape rect; //������� ������������ �������
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