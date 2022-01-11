#include "app.h"
#include <vector>
app::app(){
    window = new  sf::RenderWindow(sf::VideoMode(WINDOW_X, WINDOW_Y), "Chess",sf::Style::Default,sf::ContextSettings(0,0,8,1,1,0,false));///open window with antialiasing x8 for smoother edges
    window->setFramerateLimit(60);    ///Limit CPU usage
    mClient = new TCP_client;
    mGame = new game;

    init();
}

void app::init() {
    window->clear(sf::Color(0x261C2Cff)); ///Clear old frame
    mGame->draw(window);
    window->display();

    package info;
    mClient->retrive_package(&info);
    if(info.type == 0){
        window->close();
    } else if(info.type == 1){
        mGame->setTeam(info.data[0]);
        mGame->connSucces();
    }

    window->clear();
    mGame->draw(window);
    sf::Font mFont;
    mFont.loadFromFile("../OpenSans-Regular.ttf");
    sf::Text text;
    text.setString("Waiting for connection...");
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(24);
    text.setFont(mFont);

    popup* color;
    if(mGame->getTeam()){
        text.setString("White!");
        color = new popup(text);
    }else{
        text.setString("Black!");
        color = new popup(text);
    }

    color->draw(window);
    window->display();
    sleep(3);
}

void app::pollEvent() {

    sf::Event event;
    package info;
    if(mGame->getState() and mClient->getConn()){
        mClient->send_exit();
    }
    if(!mClient->getTurn() and mClient->getConn() and !mGame->getState()){
        mClient->retrive_package(&info);
        if(info.type==2)mGame->setTableMatrix(info.data);
        else if(info.type == 0)
        {
            mGame->disconn();
        }
    }
    ///Event polling
    while (window->pollEvent(event))
    {
        position mousePosition;
        mousePosition.x = sf::Mouse::getPosition(*window).x;
        mousePosition.y = sf::Mouse::getPosition(*window).y;

        if(event.type == sf::Event::KeyPressed ){
            if(event.key.code == sf::Keyboard::Escape){
                event.type = sf::Event::Closed;
            }
        }else if(event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                int choice = mGame->doAction(mousePosition);

                if(mClient->getTurn() and mClient->getConn()  and choice == 0 and mGame->getMoveFinish()) {
                    mClient->send_board(mGame->getTableMatrix());
                }
                if (choice == 1) {
                    event.type = sf::Event::Closed;
                }else if (choice == 2) {
                    mGame = new game;
                    mClient = new TCP_client;
                    init();
                }
            }
        }

        if(event.type == sf::Event::Closed){
            exit(window);
            break;
        }
    }
}

void app::render()
{
    ///App loop
    while (window->isOpen()) {
        window->clear(sf::Color(0x261C2Cff)); ///Clear old frame
        ///draw here
        mGame->draw(window);
        window->display();///Tell the App that the window is done drawing
        pollEvent();
    }
}

void app::exit(sf::RenderWindow *window) {
    if(mClient->getConn())mClient->send_exit();
    window->close();
}
