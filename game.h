#ifndef SFMLAPP_GAME_H
#define SFMLAPP_GAME_H

#include "table.h"
#include "utilities.h"
#include "interface.h"


struct pieceInfo{
    int type;
    bool team;
    std::pair<int,int> index;
    void highlightPiece(sf::RenderWindow *window,sf::Vector2f size);
    void getInfo(std::pair<int,int> index,std::vector<std::vector<int>> matrix);
};

class game{
private:
    interface mInterface;
    table mTable;
    bool team;///1 is white 0 is black
    bool playing;
    bool move_finished;

    pieceInfo piece_selected;
    std::vector<std::pair<int,int>>available_moves;

    pieceInfo selectSquare(position searchedPos);
    void computeAvailableMoves();
    void highlightMoves(sf::RenderWindow *window);
    void highlightSquare(sf::RenderWindow *window,std::pair<int, int> index);
    void highlightAttack(sf::RenderWindow *window,std::pair<int, int> index);
    int checkState();/// 0 nobody won, -1 lost 1 win
public:
    game();
    void draw(sf::RenderWindow *window);
    int doAction(position mousePosition);
    int getState();
    std::vector<std::vector<int>> getTableMatrix();
    void setTableMatrix(std::vector<int> matrix);
    void setTeam(bool mTeam);
    bool getTeam();
    void connSucces();
    bool getMoveFinish();
    void setMoveFinish(bool x);
};

#endif //SFMLAPP_GAME_H
