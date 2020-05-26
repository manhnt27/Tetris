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

        if(piece[i].x<0 || piece[i].x>=BOARD_WIDTH || piece[i].y>=BOARD_HEIGHT)
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

bool Game::isValid(int cx,int cy) const{
    if(cx<24 && cx>=0 && cy<10 && cy>=0 && !mark[cx][cy])
        return true;
    return false;
}

void Game::searchPiece(int cx,int cy,int targetColor){
    for(pair<int,int> dir: directions)
    {   int tmpx=cx+dir.first,tmpy=cy+dir.second;
        if(isValid(tmpx,tmpy) && board[tmpx][tmpy]==targetColor)
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

void Game::searchAlonePiece(int x,int y){
    for(pair<int,int> dir: directions)
    {
        mp[{x,y}]=board[x][y];
        if(x==23) {canMove=true;}
        int tmpx=x+dir.first,tmpy=y+dir.second;
        if(isValid(tmpx,tmpy) && board[tmpx][tmpy]!=0)
        {
            mark[tmpx][tmpy]=true;
            searchAlonePiece(tmpx,tmpy);
        }
    }

}

void Game::deleteAlonePiece(){
    mp.clear();
    for(int i=0;i<24;i++)
        for(int j=0;j<10;j++){
            tmpBoard[i][j]=board[i][j];
            mark[i][j]=false;
        }
    for(int i=0;i<24;i++)
        for(int j=0;j<10;j++){
        canMove=false;
        if(board[i][j]!=0 && !mark[i][j]){
            searchAlonePiece(i,j);
        //cout<<"h"<<i<<" "<<j<<"\n";
        if(!canMove){
            map<pair<int,int>,int> tmpMap=mp;
            bool collision=false;
            int len=mp.size();
            int prev[len+1],next[len+1],coorx[len+1],id=0;

            for(auto it=tmpMap.begin();it!=tmpMap.end();it++)
                {
                    pair<int,int> pr=it->first;
                    int cx=pr.first,cy=pr.second;
                    tmpBoard[cx][cy]=0;
                    next[id]=cx;
                    coorx[id]=cy;
                    id++;

                }

            while(!collision){

                for(int i=0;i<len;i++)
                {
                    prev[i]=next[i];
                    next[i]++;

                    if(tmpBoard[next[i]][coorx[i]]!=0 || next[i]==24) {collision=true;}
                }

            }
            id=0;
            for(auto it=tmpMap.begin();it!=tmpMap.end();it++)
                {
                    tmpBoard[prev[id]][coorx[id]]=it->second;
                    id++;
                }
        }

        mp.clear();
        }
    }
    for(int i=0;i<24;i++)
        for(int j=0;j<10;j++)
            board[i][j]=tmpBoard[i][j];

}

