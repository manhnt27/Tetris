#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <algorithm>
#include"GameGraphics.h"

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}




bool LTexture::loadFromRenderedText(SDL_Renderer* gRenderer,TTF_Font* gFont,std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}

	//Return success
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render(SDL_Renderer* gRenderer,int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}


void createRect(SDL_Rect& rect,const int& cx,const int& cy,const int& cw,const int& ch)
{
    rect.x=cx;
    rect.y=cy;
    rect.w=cw;
    rect.h=ch;
}
void colorSelect(SDL_Renderer* renderer,const int& colorKey)
{
    switch(colorKey)
    {
        case 1:
            SDL_SetRenderDrawColor(renderer,3, 65, 174,SDL_ALPHA_OPAQUE);
        break;
        case 2:
            SDL_SetRenderDrawColor(renderer,114, 203, 59,SDL_ALPHA_OPAQUE);
        break;
        case 3:
            SDL_SetRenderDrawColor(renderer,255, 213, 0,SDL_ALPHA_OPAQUE);
        break;
        case 4:
            SDL_SetRenderDrawColor(renderer,255, 151, 28,SDL_ALPHA_OPAQUE);
        break;
        case 5:
            SDL_SetRenderDrawColor(renderer,255, 50, 19,SDL_ALPHA_OPAQUE);
        break;
        case 6:
            SDL_SetRenderDrawColor(renderer,140, 25, 255,SDL_ALPHA_OPAQUE);
        break;

    }
}
