#include "table.h"
/*
 * king 6, queen 5, rook 4, knight 3, bishop 2, pawn 1, null 0
 * - white, + black
 */
table::table() {
    tableMatrix = {
            {4,3,2,5,6,2,3,4},
            {1,1,1,1,1,1,1,1},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {-1,-1,-1,-1,-1,-1,-1,-1},
            {-4,-3,-2,-5,-6,-2,-3,-4}
    };


    sf::Image img;
    sf::Texture texture;
    sf::Sprite sprite;

    ///loading board textures
    mBoard_tiles.emplace_back(sprite,texture);
    mBoard_tiles.emplace_back(sprite,texture);

    std::string* path;
    path = new std::string("square_gray_dark_1x_ns.png");
    *path = PATH_R + *path;
    if (!(img.loadFromFile(*path)))
        std::cout << "Cannot load image";
    mBoard_tiles[0].second.loadFromImage(img);
    mBoard_tiles[0].first.setTexture(mBoard_tiles[0].second);
    mBoard_tiles[0].first.setScale(0.25,0.25);

    path = new std::string("square_gray_light_1x_ns.png");
    *path = PATH_R + *path;
    if (!(img.loadFromFile(*path)))
        std::cout << "Cannot load image";
    mBoard_tiles[1].second.loadFromImage(img);
    mBoard_tiles[1].first.setTexture(mBoard_tiles[1].second);
    mBoard_tiles[1].first.setScale(0.25,0.25);

    ///loading pieces texture

    for(int i = 0 ;i<12;++i){
        mPieces.emplace_back(sprite,texture);
    }
    for(int i = 0 ;i<12;++i){
        if(i < 6){
            path = new std::string("w_");
        }else path = new std::string("b_");
        if( i % 6 == 0){
            *path = *path + "pawn_1x_ns.png";
        }else if( i % 6 == 1){
            *path = *path + "bishop_1x_ns.png";
        }else if( i % 6 == 2){
            *path = *path + "knight_1x_ns.png";
        }else if( i % 6 == 3){
            *path = *path + "rook_1x_ns.png";
        }else if( i % 6 == 4){
            *path = *path + "queen_1x_ns.png";
        }else{
            *path = *path + "king_1x_ns.png";
        }
        *path = PATH_R + *path;

        if (!(img.loadFromFile(*path)))
            std::cout << "Cannot load image";

        mPieces[i].second.loadFromImage(img);
        mPieces[i].first.setTexture(mPieces[i].second);
        mPieces[i].first.setScale(0.20,0.20);

    }

    square_size = {mBoard_tiles.front().first.getGlobalBounds().width, mBoard_tiles.front().first.getGlobalBounds().height};
    drawPieces = false;
}

std::vector<bool> table::getTablePackage() {
    std::vector<bool> tableData;

    return tableData;
}

void table::draw(sf::RenderWindow *window) {
    int offset_y = (WINDOW_Y - 8 * square_size.y * 1.004) / 2;
    int offset_x = offset_y;
    int piece;

    for(int i = 0; i<8 ;++i) {
        for (int j = 0; j < 8; ++j) {
            if ((i + j) % 2) {
                mBoard_tiles[0].first.setPosition(offset_x + square_size.x * i * (1.004),
                                                  offset_y + square_size.y * j * (1.004));
                window->draw(mBoard_tiles[0].first);
            } else {
                mBoard_tiles[1].first.setPosition(offset_x + square_size.x * i * (1.004),
                                                  offset_y + square_size.y * j * (1.004));
                window->draw(mBoard_tiles[1].first);
            }
        }
    }
        for(int i = 0; i<8 ;++i){
            for(int j = 0; j<8 ;++j) {
            if(drawPieces){
                piece = tableMatrix[i][j];
                if(piece!=0){
                    piece = abs(piece);
                    if(tableMatrix[i][j] > 0)piece+=6;
                    piece-=1;

                    mPieces[piece].first.setPosition(offset_x + square_size.x * j*(1.004) + (square_size.x- mPieces[piece].first.getGlobalBounds().width)/2,offset_y + square_size.y * i*(1.004) + (square_size.y- mPieces[piece].first.getGlobalBounds().height)/2);
                    window->draw(mPieces[piece].first);
                }
            }
        }
    }
}

sf::Vector2f table::getSquareSize() {
    return square_size;
}

std::vector<std::vector<int>> table::getTableMatrix() {
    return tableMatrix;
}

void table::setTableMatrix(std::vector<std::vector<int>> matrix) {
    tableMatrix = matrix;
}

void table::setTableMatrix(std::vector<int> matrix) {

    for(int i = 0;i<8;++i)
        for(int j = 0;j<8;++j)
        {
            tableMatrix[i][j]=matrix[i*8+j];
        }
}

void table::drawPiecesSet() {
    drawPieces = true;
}