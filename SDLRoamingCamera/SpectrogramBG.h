#pragma once

#include <SDL.h>
#include <bass.h>
class SpectrogramBG
{
public:
	SpectrogramBG(SDL_Window* window, SDL_Renderer* renderer, HSTREAM* stream);
	void update();
	void draw();
	void free();
	~SpectrogramBG();
private:
	HSTREAM* music;
	SDL_Texture* tex1;
	SDL_Texture* tex2;
	SDL_Renderer* ren;
	SDL_Window* win;
	SDL_Texture* curTex;
	int winWidth;
	int winHeight;
	SDL_Rect tex1Pos;
	SDL_Rect tex2Pos;
	SDL_Rect* curPos;
	int highest;
};

