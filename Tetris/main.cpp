#include <SDL2/SDL.h>
#include<bits/stdc++.h>
#include<SDL2/SDL_ttf.h>
#include "GameMovement.h"
using namespace std;
SDL_Window* window = NULL;
SDL_Surface* surface=NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* fontMedium=NULL;
TTF_Font* fontSmall=NULL;
const SDL_Color textColor = { 243, 156, 18 };
const int SCREEN_WIDTH=410,SCREEN_HEIGHT=555;
const char* WINDOW_TITLE="Tetris";
SDL_Event event;
Game game;
int randomColor=0,gameSpeed,tmpGameSpeed,point,prePoint;
bool pause=false,done=false,isRotated,fast,isPressed,beginGame,chooseSpeed;
Uint32 moveTime=0;

void init()
{

    isRotated=false,fast=false,isPressed=false,beginGame=true;
    moveTime=0;
    pause=false;
    done=false;
    point=20;
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
        SDL_MESSAGEBOX_ERROR, /* .flags */
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
            game.initPiece();
            pause=false;
            moveTime=SDL_GetTicks();
        }
        break;
    }

}

void showMessHelp(){
    const SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Got it->" },
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
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        "Tetris", /* .title */
        "Move Left: Left\n Move Right: Right\n Rotate: Up\n Move Faster: Down\n Pause: P", /* .message */
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
        case 0: ;
        break;

    }

}

void showMessSpeed(){
    const SDL_MessageBoxButtonData buttons[] = {
        { /* .flags, .buttonid, .text */        0, 0, "Slow" },
        { 0, 1, "Normal" },
        { 0, 2, "Fast" },

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
        "  Choose your speed  ", /* .message */
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
        case 0: {cout<<"Slow";tmpGameSpeed=2000;cout<<gameSpeed<<"v";}
        break;
        case 1: {cout<<"Normal";tmpGameSpeed=1000;cout<<gameSpeed<<"v";}
        break;
        case 2: {cout<<"Fast";tmpGameSpeed=100;cout<<gameSpeed<<"v";}
        break;
    }
    cout<<gameSpeed<<"m";

}


int main(int argc, char *argv[])
{

     srand(time(0));

     LTexture LSpeed,LHelp,Lscore;
     if (SDL_Init(SDL_INIT_VIDEO) !=0)
        cout<<"Error on SDL initialization \n"<<SDL_GetError();
        else
        {

        window= SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_Rect rectBound;
        createRect(rectBound,25,25,350,500);

        int dx=0,speed=0,randomPiece=-1,gameSpeed=1000;
        bool first=true;
        game.initBoard();
        init();
        game.initPiece();

        if( TTF_Init() == -1 ) SDL_Log(TTF_GetError());

        fontMedium = TTF_OpenFont("font/sans_serif.ttf", 25);
        fontSmall = TTF_OpenFont("font/sans_serif.ttf", 20);
        if(fontSmall==NULL) SDL_Log("Font not found");
        if(fontMedium==NULL) SDL_Log("Font not found");
        if(!LSpeed.loadFromRenderedText(renderer,fontMedium,"Speed",textColor))
            printf( "Failed to render text texture!\n" );
        if(!LHelp.loadFromRenderedText(renderer,fontMedium,"Help",textColor))
            printf( "Failed to render text texture!\n" );

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
                        case SDLK_UP: {isRotated=true;point-=4;}
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

                case SDL_MOUSEBUTTONDOWN:
                    {
                        int cx=event.button.x;
                        int cy=event.button.y;

                        if(cx>=290 && cx<=359 && cy>=399 && cy<=416)
                            {
                                chooseSpeed=true;
                                showMessSpeed();
                            }

                        if(cx>=300 && cx<=342 && cy>=439 && cy<=456)
                            {
                                chooseSpeed=false;
                                showMessHelp();
                            }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    {
                        if(chooseSpeed)
                        gameSpeed=tmpGameSpeed;
                    }
                    break;
            }
            }

            SDL_SetRenderDrawColor(renderer,100,30,215,SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(renderer,&rectBound);
            SDL_RenderDrawLine(renderer,275,25,275,530);
            SDL_RenderDrawLine(renderer,25,250,375,250);


            LSpeed.render(renderer,290,390);
            LHelp.render(renderer,297,430);
            if(pause) moveTime=SDL_GetTicks();

            if(!pause)
            {
                    game.moveLeftRight(dx);
                    if(game.isCollision())
                        game.restorePieces();

                    if(isRotated && randomPiece!=6)
                    {
                        game.rotate_();
                        if(game.isCollision())
                            game.restorePieces();
                    }

                    if(SDL_GetTicks()>moveTime || beginGame)
                    {
                        if(!isPressed)
                           game.moveDown();
                        bool check=game.isCollision();
                        if(check|| beginGame)
                        {

                            if(check){
                                game.getColorLanded(randomColor);
                                game.initMark();
                                game.deletePiece(randomColor,point);
                            }
                            randomPiece=rand()%7;
                            randomColor=rand()%6+1;
                            game.generatePiece(randomPiece);
                        }
                        if(fast){
                            switch(gameSpeed)
                            {
                                case 2000: moveTime+=(gameSpeed-1000);
                                break;
                                case 1000: moveTime+=(gameSpeed-900);
                                break;
                                case 100: moveTime+=(gameSpeed-10);
                                break;
                            }

                            fast=false;
                        }
                        else
                            moveTime+=gameSpeed;
                        beginGame=false;

                    }
            }

            dx=0;isRotated=false,isPressed=false;
            if(point<0)
            {
                done=true;
                showMessGameOver();
            }
            string score="Score: "+to_string(point);
            if(!Lscore.loadFromRenderedText(renderer,fontSmall,score,textColor))
                printf( "Failed to render text texture!\n" );
            Lscore.render(renderer,283,120);

            game.showIdlePiece(renderer);
            game.showActivePiece(renderer,randomColor);

            if(game.checkGameOver())
            {
                done=true;
                showMessGameOver();
            }
            SDL_RenderPresent(renderer);
        }

        if(renderer) SDL_DestroyRenderer(renderer);
        if(window) SDL_DestroyWindow(window);

     }
      TTF_CloseFont(fontMedium);
      TTF_CloseFont(fontSmall);
      TTF_Quit();
      SDL_Quit();
}
