#ifndef SFMLAPP_INTERFACE_H
#define SFMLAPP_INTERFACE_H
#define WINDOW_X 996
#define WINDOW_Y 1000
#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
#include "utilities.h"

class button{
private:
    sf::RectangleShape mRectangle;
    position mPosition;
    sf::Vector2f mSize;
    sf::Text mText;
public:
    button(bool type,position popupPos,sf::Text text,sf::Vector2f size);
    void draw(sf::RenderWindow *window);
    bool isWithin(position searchedPos);
};

class popup{
private:
    sf::RectangleShape mRectangle;
    bool connPopup;
    button* exitB;
    button* newB;
    position mPosition;
    sf::Vector2f mSize;
    sf::Text mText;
public:
    popup(sf::Text text);
    void draw(sf::RenderWindow *window);
    void drawButtons(sf::RenderWindow *window);
    bool buttonNewPressed(position searchedPos);
    bool buttonExitPressed(position searchedPos);
};

class interface{
private:
    popup* waitConn;
    popup* gameWin;
    popup* gameLose;
    popup* disconn;
    int choice;
    sf::Font mFont;
    int gameState;/// -1 - conn, 0 - null, 1 - win, 2 - lose, 3 - disconn
public:
    interface();
    int getState();
    void draw(sf::RenderWindow *window);
    void setState(int state);
    int checkE_N(position mousePosition);///Check exit or new game choice
};

#endif //SFMLAPP_INTERFACE_H
