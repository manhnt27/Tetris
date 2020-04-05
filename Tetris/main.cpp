#include <SDL2/SDL.h>
#include<bits/stdc++.h>
using namespace std;
SDL_Window* window = NULL;
SDL_Surface* surf=NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;
struct Point
{int x,y;} a[4], b[4];
void createRect( SDL_Rect&  rect,int cx,int cy,int cw,int ch)
    {
        rect.x=cx;
        rect.y=cy;
        rect.w=cw;
        rect.h=ch;
    }
int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

int main(int argc, char *argv[])
{

    srand(time(0));
     if (SDL_Init(SDL_INIT_VIDEO) == 0) {

        window= SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 500, SDL_WINDOW_SHOWN);
        renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        int n=rand()%7;
        for (int i=0;i<4;i++)
           {
            a[i].x = figures[n][i] % 2;
            a[i].y = figures[n][i] / 2;
           }
        SDL_Rect rect[4];
        for(int i=0;i<4;i++)
           {
           //
            rect[i].x=a[i].x*18;
            rect[i].y=a[i].y*18;
            rect[i].w=17.6;
            rect[i].h=17.6;
            }
        bool done=false;
        while (!done) {

            SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    done = true;}
            SDL_SetRenderDrawColor(renderer,116,41,215,0xff);

            SDL_RenderFillRects(renderer, rect,4);

            SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
            //SDL_RenderDrawRects(renderer, rect,4);
            SDL_RenderPresent(renderer);

         }
        cerr<<SDL_GetError();
        if(renderer) SDL_DestroyRenderer(renderer);
        if(window) SDL_DestroyWindow(window);

        }
     SDL_Quit();
}
