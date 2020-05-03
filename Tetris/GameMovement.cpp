#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <algorithm>
#include"GameMovement.h"
using namespace std;
 void Game::moveLeftRight(int dx)
{
    for(int i=0;i<4;i++)
    {
        tmpPiece[i]=piece[i];
        piece[i].x+=dx;
   }

}
 void Game::generatePiece(int randomPiece)
{

    for (int i=0;i<4;i++)
    {
        piece[i].x = figures[randomPiece][i]%2+4;
        piece[i].y = figures[randomPiece][i]/2;

    }

}
 void Game::moveDown()
 {
     for(int i=0;i<4;i++)
        {
            tmpPiece[i]=piece[i];
            piece[i].y++;
        }
}
void Game::rotate_()
{
    Point pivot=piece[1];
    for(int i=0;i<4;i++)
    {
        int xNewRotate=-piece[i].y;
        int yNewRotate=piece[i].x;
        piece[i].x=xNewRotate+(pivot.x+pivot.y);
        piece[i].y=yNewRotate+(pivot.y-pivot.x);
    }

}
bool Game::isCollision() const{

    for(int i=0;i<4;i++){

        if(piece[i].x<0 || piece[i].x>=10 || piece[i].y>23)
            return true;
        else{

            if(board[piece[i].y][piece[i].x]!=0)
                return true;
            }
        }
    return false;
}
void Game::initBoard(){
    for(int i=0;i<BOARD_HEIGHT;i++)
        for(int j=0;j<BOARD_WIDTH;j++)
            board[i][j]=0;
}
void Game::restorePieces(){
    for(int i=0;i<4;i++)
        piece[i]=tmpPiece[i];
}

void Game::getColorLanded(int randomColor)
{
    for(int i=0;i<4;i++)
        board[tmpPiece[i].y][tmpPiece[i].x]=randomColor;
}

bool Game::checkGameOver() const{
    for(int i=0;i<10;i++)
        if(board[3][i]!=0)
            return true;
    return false;
}

void Game::initPiece(){
    for(int i=0;i<4;i++)
    {
        piece[i].x=piece[i].y=0;
        tmpPiece[i].x=tmpPiece[i].y=0;
    }
}

void Game::showIdlePiece(SDL_Renderer* renderer){
    for(int i=4;i<=23;i++)
        for(int j=0;j<10;j++)
            {
                SDL_Rect idlePiece;
                if(board[i][j]==0) continue;
                createRect(idlePiece,25+j*25,25+(i-4)*25,PIECE_SIZE,PIECE_SIZE);
                colorSelect(renderer,board[i][j]);
                SDL_RenderFillRect(renderer,&idlePiece);
            }
}

void Game::showActivePiece(SDL_Renderer* renderer,int randomColor){
    for(int i=0;i<4;i++)
        if(piece[i].y>3)
            {
                SDL_Rect activePiece;
                createRect(activePiece,25+piece[i].x*25,(piece[i].y-3)*25,PIECE_SIZE,PIECE_SIZE);
                colorSelect(renderer,randomColor);
                SDL_RenderFillRect(renderer,&activePiece);
            }
}

bool Game::checkDelete() const{
    for(int i=0;i<4;i++)
        if(tmpPiece[i].y<=12)
            return true;
    return false;
}

void Game::initMark(){

    for(int i=0;i<=23;i++)
        for(int j=0;j<10;j++)
            mark[i][j]=false;
    countPiece=0;
}

bool Game::isValid(int cx,int cy,int targetColor) const{
    if(cx<=24 && cx>=0 && cy<=10 && cy>=0 && !mark[cx][cy] && board[cx][cy]==targetColor)
        return true;
    return false;
}

void Game::searchPiece(int cx,int cy,int targetColor){
    for(pair<int,int> dir: directions)
    {   int tmpx=cx+dir.first,tmpy=cy+dir.second;
        if(isValid(tmpx,tmpy,targetColor))
        {
            mark[tmpx][tmpy]=true;
            countPiece++;
            searchPiece(tmpx,tmpy,targetColor);
        }
    }
}

void Game::deletePiece(int targetColor,int& score){

    if(Game::checkDelete())
        Game::searchPiece(tmpPiece[3].y,tmpPiece[3].x,targetColor);

    if(countPiece>4){
        for(int i=0;i<=23;i++)
            for(int j=0;j<10;j++)
                if(mark[i][j])
                    board[i][j]=0;
        score+=4;

    }
}

