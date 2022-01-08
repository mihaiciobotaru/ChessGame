#pragma once
#include "game.h"
#include "TCP_Client.h"

class app{
private:
    sf::RenderWindow* window;
    game* mGame;
    TCP_client* mClient;
public:
    app();
    void render();
    void pollEvent();
    void exit(sf::RenderWindow* window);
};


