#include "interface.h"

button::button(bool type,position popupPos,sf::Text text,sf::Vector2f size) {
    mSize = size;
    mPosition = popupPos;

    mRectangle.setSize({mSize.x,mSize.y});
    mRectangle.setFillColor(sf::Color(0xC24914ff));
    mRectangle.setPosition(mPosition.x,mPosition.y);
    mRectangle.setOutlineThickness(1);
    mRectangle.setOutlineColor(sf::Color::White);

    mText = std::move(text);
    mText.setCharacterSize(20);
    if(type){
        mText.setString("New");
    }else{
        mText.setString("Exit");
    }
    mText.setPosition(mPosition.x+(mSize.x-mText.getGlobalBounds().width)/2,mPosition.y+(mSize.y-mText.getGlobalBounds().height)/2.5);
}

void button::draw(sf::RenderWindow *window) {
    window->draw(mRectangle);
    window->draw(mText);
}

bool button::isWithin(position searchedPos) {
    if(searchedPos.x > mPosition.x && searchedPos.x < (mPosition.x + mSize.x) &&
       searchedPos.y > mPosition.y && searchedPos.y < (mPosition.y + mSize.y))return true;
    return false;
}

popup::popup(sf::Text text) {
    mSize = {1400 / 4,800 / 7};
    mPosition = {int((WINDOW_X - mSize.x)/2),int((WINDOW_Y - mSize.y)/2)};
    mRectangle.setSize({mSize.x,mSize.y});
    mRectangle.setFillColor(sf::Color(0xC24914ff));
    mRectangle.setPosition(mPosition.x,mPosition.y);
    mRectangle.setOutlineThickness(1);
    mRectangle.setOutlineColor(sf::Color::White);

    mText = std::move(text);
    if(mText.getString() == "Waiting for connection..." ||
        mText.getString() == "Black!" ||
        mText.getString() == "White!"){
        connPopup = true;
        mText.setPosition(int((WINDOW_X - mSize.x)/2) + (mSize.x - text.getGlobalBounds().width)/2,int((WINDOW_Y - mSize.y)/2 + (mSize.y - text.getGlobalBounds().height)/2.2));
    }else{
        mText.setPosition(int((WINDOW_X - mSize.x)/2) + (mSize.x - text.getGlobalBounds().width)/2,int((WINDOW_Y - mSize.y)/2 + (mSize.y - text.getGlobalBounds().height)/4));
        exitB = new button(false,{int(mPosition.x+ 1*mSize.x/5),int(mPosition.y + 2 * mSize.y/3.5)},text,{mSize.x/5,float(mSize.y/2.8)});
        newB = new button(true,{int(mPosition.x+ 3*mSize.x/5),int(mPosition.y + 2 * mSize.y/3.5)},text,{mSize.x/5,float(mSize.y/2.8)});
        connPopup = false;
    }
}

void popup::drawButtons(sf::RenderWindow *window) {
    exitB->draw(window);
    newB->draw(window);
}

void popup::draw(sf::RenderWindow *window) {
    window->draw(mRectangle);
    window->draw(mText);
    if(!connPopup){
        drawButtons(window);
    }
}

bool popup::buttonExitPressed(position searchedPos) {
    return exitB->isWithin(searchedPos);
}

bool popup::buttonNewPressed(position searchedPos) {
    return newB->isWithin(searchedPos);
}

interface::interface() {
    mFont.loadFromFile("../OpenSans-Regular.ttf");
    gameState = -1;
    sf::Text text;
    text.setString("Waiting for connection...");
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(24);
    text.setFont(mFont);
    waitConn = new popup(text);

    text.setString("Win!");
    gameWin = new popup(text);

    text.setString("Lost!");
    gameLose = new popup(text);


}

void interface::draw(sf::RenderWindow *window) {
    if(gameState==-1)waitConn->draw(window);
    else if(gameState==1)gameWin->draw(window);
    else if(gameState==2)gameLose->draw(window);
}

void interface::setState(int state) {
    gameState = state;
}

int interface::checkE_N(position mousePosition) {
    if(gameWin->buttonNewPressed(mousePosition) || gameLose->buttonNewPressed(mousePosition))return 2;
    if(gameWin->buttonExitPressed(mousePosition) || gameLose->buttonExitPressed(mousePosition))return 1;
    return 0;
}