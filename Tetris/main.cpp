#include <SDL2/SDL.h>
#include<bits/stdc++.h>
using namespace std;
SDL_Window* window = NULL;
SDL_Surface* surface=NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH=410,SCREEN_HEIGHT=555,PIECE_SIZE=24.5;
const char* WINDOW_TITLE="Tetris";
SDL_Event event;
int randomColor=0;
bool pause=false,done=false,isRotated,fast,isPressed,beginGame;
Uint32 moveTime=0;
struct Point
{
    int x;
    int y;
} piece[4], tmpPiece[4];
void createRect(SDL_Rect& rect,const int& cx,const int& cy,const int& cw,const int& ch)
    {
        rect.x=cx;
        rect.y=cy;
        rect.w=cw;
        rect.h=ch;
    }
int board[24][10]={0};
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
        if(piece[i].x<0 || piece[i].x>=10 || piece[i].y>23)
            return true;
        else
            if(board[piece[i].y][piece[i].x]!=0)
                return true;
    return false;
}
bool checkGameOver()
{
    for(int i=0;i<10;i++)
        if(board[3][i]!=0)
            return true;
    return false;
}

void colorSelect()
{
    switch(randomColor)
    {
        case 0:
            SDL_SetRenderDrawColor(renderer,3, 65, 174,SDL_ALPHA_OPAQUE);
        break;
        case 1:
            SDL_SetRenderDrawColor(renderer,114, 203, 59,SDL_ALPHA_OPAQUE);
        break;
        case 2:
            SDL_SetRenderDrawColor(renderer,255, 213, 0,SDL_ALPHA_OPAQUE);
        break;
        case 3:
            SDL_SetRenderDrawColor(renderer,255, 151, 28,SDL_ALPHA_OPAQUE);
        break;
        case 4:
            SDL_SetRenderDrawColor(renderer,255, 50, 19,SDL_ALPHA_OPAQUE);
        break;
    }
}

void init()
{
    for(int i=0;i<=23;i++)
        for(int j=0;j<10;j++)
            board[i][j]=0;
    isRotated=false,fast=false,isPressed=false,beginGame=true;
    moveTime=0;
    pause=false;
    done=false;
}

void showMessGameOver(){
    const SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Exit" },
        { 0, 1, "Continue" },
    };
    const SDL_MessageBoxColorScheme colorScheme = {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
            { 255,   0,   0 },
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
            {   255, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
            { 255, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
            {   0,   0, 255 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
            { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_WARNING, /* .flags */
        NULL, /* .window */
        "Tetris", /* .title */
        "     Game Over!!!    ", /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");
        return;
    }
    switch(buttonid)
    {
        case -1: SDL_Log("No button selected");
        break;
        case 0: done=true;
        break;
        case 1: {
            init();
            pause=false;
            moveTime=SDL_GetTicks();
        }
        break;
    }

}

int main(int argc, char *argv[])
{

     srand(time(0));

     if (SDL_Init(SDL_INIT_VIDEO) !=0)
        cout<<"Error on SDL initialization \n"<<SDL_GetError();
        else
        {

        window= SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_Rect rectBound;
        createRect(rectBound,25,25,350,500);

        int dx=0,speed=0,randomPiece=-1;
        init();
        while (!done) {
            SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            while (SDL_PollEvent(&event)) {
                switch (event.type){
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_LEFT:
                            {
                                isPressed=true;
                                dx=-1;
                            }
                        break;
                        case SDLK_RIGHT:
                            {
                                isPressed=true;
                                dx=1;
                            }
                        break;
                        case SDLK_UP: isRotated=true;
                        break;
                        case SDLK_DOWN: fast=true;
                        break;
                        case SDLK_p:
                            {
                                if(pause) pause=false;
                                else pause=true;
                            }
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
            SDL_RenderDrawLine(renderer,275,30,275,530);
            if(pause) moveTime=SDL_GetTicks();
            if(!pause)
            {
                    for(int i=0;i<4;i++)
                        {
                            tmpPiece[i]=piece[i];
                            piece[i].x+=dx;
                        }
                    if(isCollision())
                        for(int i=0;i<4;i++)
                            piece[i]=tmpPiece[i];
                    if(isRotated && randomPiece!=6)
                    {
                        Point rotatePiece=piece[1];
                        for(int i=0;i<4;i++)
                        {
                            int cx=piece[i].y-rotatePiece.y;
                            int cy=piece[i].x-rotatePiece.x;
                            piece[i].x=rotatePiece.x-cx;
                            piece[i].y=rotatePiece.y+cy;
                        }
                        if(isCollision())
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


                        if(isCollision() || beginGame)
                        {   if(isCollision())
                            for(int i=0;i<4;i++)
                                board[tmpPiece[i].y][tmpPiece[i].x]=1;
                            randomPiece=rand()%7;
                            randomColor=rand()%5;
                            for (int i=0;i<4;i++)
                            {
                                piece[i].x = figures[randomPiece][i]%2+4;
                                piece[i].y = figures[randomPiece][i]/2;
                            }
                            for(int i=0;i<=23;i++){
                                for(int j=0;j<10;j++)
                                    cout<<board[i][j]<<" ";
                                cout<<"\n";}
                            cout<<"\n";
                            if(randomPiece!=0)
                                for (int i=0;i<4;i++)
                                    piece[i].y --;


                        }
                        if(fast)
                        {
                            moveTime+=100;
                            fast=false;
                        }
                        else
                            moveTime+=900;
                        beginGame=false;
                    }
            }
            dx=0;isRotated=false,isPressed=false;


            for(int i=4;i<=23;i++)
                for(int j=0;j<10;j++)
                    {
                        SDL_Rect idlePiece;
                        if(board[i][j]==0) continue;
                        //<<"n";
                        createRect(idlePiece,25+j*25,25+(i-4)*25,PIECE_SIZE,PIECE_SIZE);
                        SDL_SetRenderDrawColor(renderer,116,41,215,SDL_ALPHA_OPAQUE);
                        SDL_RenderFillRect(renderer,&idlePiece);
                    }

               for(int i=0;i<4;i++)
                if(piece[i].y>3)
                {

                    SDL_Rect activePiece;
                    createRect(activePiece,25+piece[i].x*25,(piece[i].y-3)*25,PIECE_SIZE,PIECE_SIZE);
                    colorSelect();
                    SDL_RenderFillRect(renderer,&activePiece);
                }
            if(checkGameOver())
            {

                done=true;
                showMessGameOver();
            }
            SDL_RenderPresent(renderer);
        }

        if(renderer) SDL_DestroyRenderer(renderer);
        if(window) SDL_DestroyWindow(window);

     }
     SDL_Quit();
}
