///////////////Copy///////////
#ifndef _GAMEGRAPHICS_H_
#define _GAMEGRAPHICSH_
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile( std::string path );

        //Creates image from font string
        bool loadFromRenderedText(SDL_Renderer* gRenderer,TTF_Font* gFont,std::string textureText, SDL_Color textColor );

        //Deallocates texture
        void free();

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        //Set blending
        void setBlendMode( SDL_BlendMode blending );

        //Set alpha modulation
        void setAlpha( Uint8 alpha );

        //Renders texture at given point
        void render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;
};
void createRect(SDL_Rect& rect,const int& cx,const int& cy,const int& cw,const int& ch);
void colorSelect(SDL_Renderer* renderer,const int& colorKey);
#endif
