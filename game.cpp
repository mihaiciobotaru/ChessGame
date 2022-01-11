#include "game.h"

game::game(): mTable() {
    piece_selected = {-1,0,{-1,-1}};
    playing = false;
}

bool operator==(const pieceInfo& lhs, const pieceInfo& rhs){
    if(lhs.team != rhs.team)return false;
    if(lhs.type != rhs.type)return false;
    if(lhs.index != rhs.index)return false;
    return true;

}

bool operator!=(const pieceInfo& lhs, const pieceInfo& rhs){
    return !(lhs==rhs);
}

void pieceInfo::getInfo(std::pair<int, int> indext, std::vector<std::vector<int>> matrix) {
    if(indext.first < 0 or indext.first > 7 or
            indext.second < 0 or indext.second > 7){
        this->index = {-1,-1};
        this->team = 0;
        this->type = 0;
    }else{
        int typet = abs(matrix[indext.first][indext.second]);
        bool teamt = 1;
        if(matrix[indext.first][indext.second] < 0)teamt = 0;
        this->index = indext;
        this->type = typet;
        this->team = teamt;
    }
}

void pieceInfo::highlightPiece(sf::RenderWindow *window,sf::Vector2f size) {
    sf::RectangleShape box;
    box.setSize(size);
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color::Blue);
    box.setOutlineThickness(-2);
    box.setPosition((WINDOW_Y - 8 * size.y * 1.004) / 2 + size.x * index.first * (1.004),
                    (WINDOW_Y - 8 * size.y * 1.004) / 2 + size.y * index.second * (1.004));
    window->draw(box);
}

void game::highlightSquare(sf::RenderWindow *window,std::pair<int, int> index) {
    sf::RectangleShape box;
    box.setSize(mTable.getSquareSize());
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color::Green);
    box.setOutlineThickness(-2);
    box.setPosition((WINDOW_Y - 8 * mTable.getSquareSize().y * 1.004) / 2 + mTable.getSquareSize().x * index.first * (1.004),
                    (WINDOW_Y - 8 * mTable.getSquareSize().y * 1.004) / 2 + mTable.getSquareSize().y * index.second * (1.004));
    window->draw(box);
}

void game::highlightAttack(sf::RenderWindow *window,std::pair<int, int> index) {
    sf::RectangleShape box;
    box.setSize(mTable.getSquareSize());
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color::Red);
    box.setOutlineThickness(-2);
    box.setPosition((WINDOW_Y - 8 * mTable.getSquareSize().y * 1.004) / 2 + mTable.getSquareSize().x * index.first * (1.004),
                    (WINDOW_Y - 8 * mTable.getSquareSize().y * 1.004) / 2 + mTable.getSquareSize().y * index.second * (1.004));
    window->draw(box);
}

pieceInfo game::selectSquare(position searchedPos) {
    int x = -1,y = -1;
    bool team = 1;

    for(int i = 0 ; i<8 ;++i){
        if(searchedPos.x >= (WINDOW_Y - 8 * mTable.getSquareSize().y * 1.004) / 2 + mTable.getSquareSize().x * i* (1.004)) x = i;
    }

    for(int i = 0 ; i<8 ;++i){
        if(searchedPos.y >= (WINDOW_Y - 8 * mTable.getSquareSize().y * 1.004) / 2 + mTable.getSquareSize().y * i* (1.004)) y = i;

    }
    if(searchedPos.x >= (WINDOW_Y - 8 * mTable.getSquareSize().y * 1.004) / 2 + mTable.getSquareSize().x * 8* (1.004)) x = -1;
    if(searchedPos.y >= (WINDOW_Y - 8 * mTable.getSquareSize().y * 1.004) / 2 + mTable.getSquareSize().y * 8* (1.004)) y = -1;

    if( x == -1 || y == -1)return {-1,0,{-1,-1}};

    if(mTable.getTableMatrix()[y][x] < 0)team = 0;
    return {mTable.getTableMatrix()[y][x], team,{x,y}};
}

void game::computeAvailableMoves() {
    ///team 0 is white and 1 is black
    available_moves.clear();
    bool team = 1;
    pieceInfo tempPiece;
    int temp_team;
    int type = abs(piece_selected.type);
    std::pair<int,int> index = piece_selected.index;
    if(piece_selected.type < 0)team = 0;

    if(type == 1 ){
        if(team){
            if(index.second==1){
                if(mTable.getTableMatrix()[index.second+1][index.first]==0) {
                    available_moves.push_back({index.first, index.second + 1});
                    if (mTable.getTableMatrix()[index.second + 2][index.first] == 0)
                        available_moves.push_back({index.first, index.second + 2});
                }
            }
            if(index.second != 7) {
                if (mTable.getTableMatrix()[index.second + 1][index.first] == 0)
                    available_moves.push_back({index.first, index.second + 1});

                if (index.first != 7) {
                    tempPiece.getInfo({index.second + 1, index.first + 1}, mTable.getTableMatrix());
                    if (tempPiece.type != 0 and piece_selected.team != tempPiece.team)
                        available_moves.push_back({index.first + 1, index.second + 1});
                }

                if (index.first != 0) {
                    tempPiece.getInfo({index.second + 1, index.first - 1}, mTable.getTableMatrix());
                    if (tempPiece.type != 0 and piece_selected.team != tempPiece.team)
                        available_moves.push_back({index.first - 1, index.second + 1});
                }
            }
        }else{
            if(index.second==6){
                if(mTable.getTableMatrix()[index.second-1][index.first]==0) {
                    available_moves.push_back({index.first, index.second - 1});
                    if (mTable.getTableMatrix()[index.second - 2][index.first] == 0)
                        available_moves.push_back({index.first, index.second - 2});
                }
            }
            else if(index.second != 0){
                if(mTable.getTableMatrix()[index.second-1][index.first]==0)available_moves.push_back({index.first,index.second-1});

                if (index.first != 7) {
                    tempPiece.getInfo({index.second - 1, index.first + 1}, mTable.getTableMatrix());
                    if (tempPiece.type != 0 and piece_selected.team != tempPiece.team)
                        available_moves.push_back({index.first + 1, index.second - 1});
                }

                if (index.first != 0) {
                    tempPiece.getInfo({index.second - 1, index.first - 1}, mTable.getTableMatrix());
                    if (tempPiece.type != 0 and piece_selected.team != tempPiece.team)
                        available_moves.push_back({index.first - 1, index.second - 1});
                }
            }
        }
    }if(type == 2 or type==5){
        index = {piece_selected.index.second,piece_selected.index.first};
        for(int i = 1 ; i< 8 ;++i){
            tempPiece.getInfo({index.first + i,index.second + i},mTable.getTableMatrix());
            if(tempPiece.index.second == -1)break;
            else if(tempPiece.type==0)available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
            else{
                if(tempPiece.team!=piece_selected.team){
                    available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
                }
                break;
            }
        }

        for(int i = 1 ; i< 8 ;++i){
            tempPiece.getInfo({index.first - i,index.second - i},mTable.getTableMatrix());
            if(tempPiece.index.second == -1)break;
            else if(tempPiece.type==0)available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
            else{
                if(tempPiece.team!=piece_selected.team){
                    available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
                }
                break;
            }
        }

        for(int i = 1 ; i< 8 ;++i){
            tempPiece.getInfo({index.first - i,index.second + i},mTable.getTableMatrix());
            if(tempPiece.index.second == -1)break;
            else if(tempPiece.type==0)available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
            else{
                if(tempPiece.team!=piece_selected.team){
                    available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
                }
                break;
            }
        }

        for(int i = 1 ; i< 8 ;++i){
            tempPiece.getInfo({index.first + i,index.second - i},mTable.getTableMatrix());
            if(tempPiece.index.second == -1)break;
            if(tempPiece.type==0)available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
            else{
                if(tempPiece.team!=piece_selected.team){
                    available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
                }
                break;
            }
        }
    }if(type == 3){
        index = {piece_selected.index.second,piece_selected.index.first};

        tempPiece.getInfo({index.first+1,index.second+2},mTable.getTableMatrix());
        if(tempPiece.index.first!=-1) {
            if ((tempPiece.type != 0 && tempPiece.team != piece_selected.team) || (tempPiece.type == 0))
        available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
        }

        tempPiece.getInfo({index.first+2,index.second+1},mTable.getTableMatrix());
        if(tempPiece.index.first!=-1) {
            if ((tempPiece.type != 0 && tempPiece.team != piece_selected.team) || (tempPiece.type == 0))
                available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
        }

        tempPiece.getInfo({index.first-1,index.second+2},mTable.getTableMatrix());
        if(tempPiece.index.first!=-1) {
            if ((tempPiece.type != 0 && tempPiece.team != piece_selected.team) || (tempPiece.type == 0))
                available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
        }

        tempPiece.getInfo({index.first-2,index.second+1},mTable.getTableMatrix());
        if(tempPiece.index.first!=-1) {
            if ((tempPiece.type != 0 && tempPiece.team != piece_selected.team) || (tempPiece.type == 0))
                available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
        }

        tempPiece.getInfo({index.first+1,index.second-2},mTable.getTableMatrix());
        if(tempPiece.index.first!=-1) {
            if ((tempPiece.type != 0 && tempPiece.team != piece_selected.team) || (tempPiece.type == 0))
                available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
        }

        tempPiece.getInfo({index.first+2,index.second-1},mTable.getTableMatrix());
        if(tempPiece.index.first!=-1) {
            if ((tempPiece.type != 0 && tempPiece.team != piece_selected.team) || (tempPiece.type == 0))
                available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
        }

        tempPiece.getInfo({index.first-1,index.second-2},mTable.getTableMatrix());
        if(tempPiece.index.first!=-1) {
            if ((tempPiece.type != 0 && tempPiece.team != piece_selected.team) || (tempPiece.type == 0))
                available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
        }

        tempPiece.getInfo({index.first-2,index.second-1},mTable.getTableMatrix());
        if(tempPiece.index.first!=-1) {
            if ((tempPiece.type != 0 && tempPiece.team != piece_selected.team) || (tempPiece.type == 0))
                available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
        }


    }if(type == 4 or type ==5){
        index = {piece_selected.index.second,piece_selected.index.first};
        for(int i = index.second+1 ; i<8;++i){
            tempPiece.getInfo({index.first,i},mTable.getTableMatrix());
            if(tempPiece.type==0)available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
            else {
                if(tempPiece.team!=piece_selected.team)available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
                break;
            }
        }

        for(int i = index.second-1 ; i>=0;--i){
            tempPiece.getInfo({index.first,i},mTable.getTableMatrix());
            if(tempPiece.type==0)available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
            else {
                if(tempPiece.team!=piece_selected.team)available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
                break;
            }
        }

        for(int i = index.first-1 ; i>=0;--i){
            tempPiece.getInfo({i,index.second},mTable.getTableMatrix());
            if(tempPiece.type==0)available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
            else {
                if(tempPiece.team!=piece_selected.team)available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
                break;
            }
        }

        for(int i = index.first+1 ; i<8;++i){
            tempPiece.getInfo({i,index.second},mTable.getTableMatrix());
            if(tempPiece.type==0)available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
            else {
                if(tempPiece.team!=piece_selected.team)available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
                break;
            }
        }


    }if(type == 6){
        index = {piece_selected.index.second,piece_selected.index.first};

        for(int i = 0; i<3;++i){
            tempPiece.getInfo({index.first-1,index.second+i-1},mTable.getTableMatrix());
            if(tempPiece.index.first!=-1){
                if ((tempPiece.type != 0 && tempPiece.team != piece_selected.team) || (tempPiece.type == 0))
                    available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
            }
        }

        for(int i = 0; i<3;++i){
            tempPiece.getInfo({index.first+1,index.second+i-1},mTable.getTableMatrix());
            if(tempPiece.index.first!=-1){
                if ((tempPiece.type != 0 && tempPiece.team != piece_selected.team) || (tempPiece.type == 0))
                    available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
            }
        }

        for(int i = 0; i<3;++i){
            tempPiece.getInfo({index.first,index.second+i-1},mTable.getTableMatrix());
            if(tempPiece.index.first!=-1){
                if ((tempPiece.type != 0 && tempPiece.team != piece_selected.team) || (tempPiece.type == 0))
                    available_moves.push_back({tempPiece.index.second,tempPiece.index.first});
            }
        }
    }
}

int game::doAction(position mousePosition) {
    move_finished = false;
    if(playing){
        pieceInfo temp_piece;
        temp_piece = selectSquare(mousePosition);
        if(piece_selected.index.first == -1 || (temp_piece.team==piece_selected.team && temp_piece.type!=0)){
            if(team != temp_piece.team)piece_selected = temp_piece;
        }
        if (piece_selected.index.first != -1 and piece_selected.type != 0) {
            computeAvailableMoves();
            if(temp_piece != piece_selected){
                for(auto move : available_moves){
                    if(temp_piece.index == move){
                        int pc = piece_selected.type;
                        if(piece_selected.team < 0)pc*=-1;
                        std::vector<std::vector<int>> matrix = mTable.getTableMatrix();
                        matrix[piece_selected.index.second][piece_selected.index.first] = 0;
                        matrix[temp_piece.index.second][temp_piece.index.first] = pc;
                        mTable.setTableMatrix(matrix);
                        move_finished = true;
                    }
                }
                piece_selected = {-1,0,{-1,-1}};
            }
        }else piece_selected = {-1,0,{-1,-1}};
        if(checkState()==1)mInterface.setState(1);
        if(checkState()==-1)mInterface.setState(2);
        return 0;
    }else {
        piece_selected = {-1,0,{-1,-1}};
        return mInterface.checkE_N(mousePosition);
    }
}

void game::highlightMoves(sf::RenderWindow *window) {
    for(int i = 0; i < available_moves.size();++i){
        if(mTable.getTableMatrix()[available_moves[i].second][available_moves[i].first]!=0)highlightAttack(window,available_moves[i]);
        else highlightSquare(window,available_moves[i]);
    }
}

void game::draw(sf::RenderWindow *window) {
    mTable.draw(window);
    if(piece_selected.index.first!=-1)
    {
        piece_selected.highlightPiece(window, mTable.getSquareSize());
        highlightMoves(window);
    }
    mInterface.draw(window);
}

int game::checkState() {
    if(mInterface.getState() == 3){
        playing == false;
        return 2;
    }
    int enemyK=-6,allyK=6;
    bool found_enemyK=false,found_allyK=false;
    if(team) {
        enemyK*=-1;
        allyK*=-1;
    }
    for(const auto& row : mTable.getTableMatrix()){
        for(auto piece : row){
            if(enemyK==piece)found_enemyK = true;
            if(allyK==piece)found_allyK = true;
        }
    }
    if(found_enemyK && found_allyK)return 0;
    playing = false;
    if(found_allyK) return 1;
    return -1;

}

std::vector<std::vector<int>> game::getTableMatrix() {
    return mTable.getTableMatrix();
}

void game::setTableMatrix(std::vector<int> matrix) {
    mTable.setTableMatrix(matrix);
}

void game::setTeam(bool mTeam) {
    team = mTeam;
}

bool game::getTeam() {
    return team;
}

void game::connSucces() {
    mInterface.setState(0);
    playing = true;
    mTable.drawPiecesSet();
}

bool game::getMoveFinish(){
    return move_finished;
}

int game::getState() {
    int temp = checkState();
    if(temp == 1)mInterface.setState(1);
    if(temp == -1)mInterface.setState(2);
    return temp;
}

void game::disconn() {
    playing = false;
    mInterface.setState(3);
}
