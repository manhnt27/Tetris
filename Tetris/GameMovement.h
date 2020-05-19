#ifndef _GAMEMOVEMENT_H_
#define _GAMEMOVEMENT_H_
#include<SDL2/SDL.h>
#include"GameGraphics.h"

struct Point
{
    int x;
    int y;
};

class Game
{
    Point tmpPiece[4],piece[4];
    ////////Tham khao: https://www.youtube.com/watch?v=zH_omFPqMO4
    const int figures[7][4] =
    {
        {1,3,5,7},
        {2,4,5,7},
        {3,5,4,6},
        {3,5,4,7},
        {2,3,5,7},
        {3,5,7,6},
        {2,3,4,5},
    };
    int board[24][10],countPiece;
    const int PIECE_SIZE=24.5;
    bool mark[25][10];
    const int BOARD_WIDTH=10;
    const int BOARD_HEIGHT=24;
    const std::vector<std::pair<int,int> > directions{{1,0},{0,1},{-1,0},{0,-1}};
public:


    void moveLeftRight(int dx);
    void generatePiece(int randomPiece);
    void moveDown();
    void rotate_();
    bool isCollision() const;
    void initBoard();
    void restorePieces();
    void getColorLanded(int randomColor);
    bool checkGameOver() const;
    void initPiece();
    void showIdlePiece(SDL_Renderer* renderer);
    void showActivePiece(SDL_Renderer* renderer,int randomColor);
    bool checkDelete() const;
    void initMark();
    bool isValid(int cx,int cy,int targetColor) const;
    void searchPiece(int cx,int cy,int targetColor);
    void deletePiece(int targetColor,int& score);
};

#endif // _GAMEMOVEMENT_H_
