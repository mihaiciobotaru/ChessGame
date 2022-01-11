#ifndef SFMLAPP_TABLE_H
#define SFMLAPP_TABLE_H
#define WINDOW_X 996
#define WINDOW_Y 996
#define PATH_R "../Resources/PNGs/"
#include <SFML/Graphics.hpp>
#include <iostream>


class table{
private:
    std::vector<std::vector<int>> tableMatrix;

    sf::Vector2f square_size;
    std::vector<std::pair<sf::Sprite,sf::Texture>> mPieces;
    std::vector<std::pair<sf::Sprite,sf::Texture>> mBoard_tiles;

    bool drawPieces;
public:
    table();
    void draw(sf::RenderWindow *window);
    std::vector<bool> getTablePackage();
    std::vector<bool> setTablePackage();

    std::vector<std::vector<int>> getTableMatrix();
    void setTableMatrix(std::vector<std::vector<int>> matrix);
    void setTableMatrix(std::vector<int> matrix);


    sf::Vector2f getSquareSize();
    void drawPiecesSet();
};

#endif //SFMLAPP_TABLE_H
