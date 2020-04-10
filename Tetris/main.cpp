#include <SDL2/SDL.h>
#include<bits/stdc++.h>
using namespace std;
SDL_Window* window = NULL;
SDL_Surface* surf=NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;
struct Point
{int x,y;} piece[4], tmpPiece[4];
void createRect(SDL_Rect& rect,int cx,int cy,int cw,int ch)
    {
        rect.x=cx;
        rect.y=cy;
        rect.w=cw;
        rect.h=ch;
    }
int board[21][10]={0};
const int figures[7][4] =
{
    1,3,5,7,
    2,4,5,7,
    3,5,4,6,
    3,5,4,7,
    2,3,5,7,
    3,5,7,6,
    2,3,4,5,
};
bool isCollision()
{
    for(int i=0;i<4;i++)
        if(piece[i].x<0 || piece[i].x>=10 || piece[i].y>20)
            return 0;
        else {  //cout<<board[a[i].y][a[i].x];
                if(board[piece[i].y][piece[i].x])

            return 0;}
    return 1;
}
bool check()
{
    for(int i=0;i<4;i++)
        if(piece[i].y>0)
            return true;
    return false;
}
int main(int argc, char *argv[])
{

     srand(time(0));

     if (SDL_Init(SDL_INIT_VIDEO) == 0) {

        window= SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 410, 555, SDL_WINDOW_SHOWN);
        renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_Rect rectBound;
        createRect(rectBound,25,25,350,500);

        int dx=0,speed=0;
        bool done=false,isRotated=false,fast=false,isPressed=false,beginGame=true;
        Uint32 moveTime=0;

        while (!done) {
            SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            while (SDL_PollEvent(&event)) {
                switch (event.type){
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_LEFT:{isPressed=true; dx=-1;}
                        break;
                        case SDLK_RIGHT:{isPressed=true; dx=1;}
                        break;
                        case SDLK_UP: isRotated=true;
                        break;
                        case SDLK_DOWN: fast=true;
                        break;
                    }
                    break;
                }
                case SDL_QUIT:
                    done=true;
                    break;
                }
            }
            SDL_SetRenderDrawColor(renderer,100,30,215,SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(renderer,&rectBound);
            SDL_RenderDrawLine(renderer,280,30,280,530);
            for(int i=0;i<4;i++)
                {
                    tmpPiece[i]=piece[i];
                    piece[i].x+=dx;
                }
            if(!isCollision())
                for(int i=0;i<4;i++)
                    piece[i]=tmpPiece[i];
            if(isRotated)
            {
                Point p=piece[1];
                for(int i=0;i<4;i++)
                {
                    int cx=piece[i].y-p.y;
                    int cy=piece[i].x-p.x;
                    piece[i].x=p.x-cx;
                    piece[i].y=p.y+cy;
                }
                if(!isCollision())
                    for(int i=0;i<4;i++)
                        piece[i]=tmpPiece[i];
            }
            if(SDL_GetTicks()>moveTime || beginGame)
            {
                if(!isPressed)
                    for(int i=0;i<4;i++)
                    {
                        tmpPiece[i]=piece[i];
                        piece[i].y++;
                    }


                if(!isCollision() || beginGame)
                {   if(!isCollision())
                    for(int i=0;i<4;i++)
                        board[tmpPiece[i].y][tmpPiece[i].x]=1;
                    int randomPiece=rand()%7;

                    for (int i=0;i<4;i++)
                    {
                        piece[i].x = figures[randomPiece][i]%2+4;
                        piece[i].y = figures[randomPiece][i]/2;
                    }
                    if(randomPiece==0)
                        for (int i=0;i<4;i++)
                        {

                            piece[i].y ++;
                        }
                }
                if(fast)
                {
                    moveTime+=200;
                    fast=false;
                }
                else
                    moveTime+=1000;
                beginGame=false;
            }
            dx=0;isRotated=false,isPressed=false;

            for(int i=0;i<21;i++)
                for(int j=0;j<10;j++)
                    {
                        SDL_Rect idlePiece;
                        if(board[i][j]==0) continue;
                        //<<"n";
                        createRect(idlePiece,25+j*25,25+(i-1)*25,24.5,24.5);
                        SDL_SetRenderDrawColor(renderer,116,41,215,SDL_ALPHA_OPAQUE);
                        SDL_RenderFillRect(renderer,&idlePiece);
                    }

               for(int i=0;i<4;i++)
                {

                    SDL_Rect rectPiece;
                    createRect(rectPiece,25+piece[i].x*25,(piece[i].y)*25,24.5,24.5);
                    SDL_SetRenderDrawColor(renderer,116,41,215,SDL_ALPHA_OPAQUE);
                    SDL_RenderFillRect(renderer,&rectPiece);
                }
            SDL_RenderPresent(renderer);
        }

        if(renderer) SDL_DestroyRenderer(renderer);
        if(window) SDL_DestroyWindow(window);

     }
     SDL_Quit();
}
