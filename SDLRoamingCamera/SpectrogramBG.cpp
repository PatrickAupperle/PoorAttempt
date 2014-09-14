#include "SpectrogramBG.h"


SpectrogramBG::SpectrogramBG(SDL_Window* window, SDL_Renderer* renderer, HSTREAM* stream)
{
	music = stream;
	ren = renderer;
	win = window;

	SDL_GetWindowSize(window, &winWidth, &winHeight); 

	tex1 = SDL_CreateTexture(ren, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, winWidth, winHeight);
	tex2 = SDL_CreateTexture(ren, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, winWidth, winHeight);

	tex1Pos.x = 0;
	tex1Pos.y = 0;
	tex1Pos.h = winHeight;
	tex1Pos.w = winWidth;

	tex2Pos.x = winWidth;
	tex2Pos.y = 0;
	tex2Pos.h = winHeight;
	tex2Pos.w = winWidth;

	curTex = tex2;
	curPos = &tex2Pos;

	highest = 100;
}



SpectrogramBG::~SpectrogramBG()
{
}

#include <iostream>
void SpectrogramBG::update()
{
	if (BASS_ChannelIsActive(*music) == BASS_ACTIVE_PLAYING)
	{
		float data[2048] = {};
		BASS_ChannelGetData(*music, data, BASS_DATA_FFT2048);
		//std::cout << "update" << tex2Pos.x << std::endl;
		SDL_Texture* oldTarget = SDL_GetRenderTarget(ren);
		Uint8 oldColor[4];
		SDL_GetRenderDrawColor(ren, oldColor, oldColor + 1, oldColor + 2, oldColor + 3);
		SDL_SetRenderTarget(ren, curTex);

		for (int i = 0; i < 2048; ++i)
		{
			int color = sqrt(data[i + 1]) * 3 * 255;
			int r = 0, g = 0, b = 0;
			if (color < 64)
			{
				b = color * 4;
			}
			else if (color >= 64 && color < 128)
			{
				b = 255;
				r = (color - 64) * 4;
			}
			else if (color >= 128 && color < 192)
			{
				b = 255 - (color - 128) * 4;
				r = 255;
				g = color * 4;
			}
			else
			{
				b = (color - 192) * 4;
				r = 255;
				g = 255;
			}

			/*if (color > highest)
			{
			highest = color;
			std::cout << highest << std::endl;
			}
			color = color * 255 / highest; */
			if (color > 255) color = 255;
			SDL_SetRenderDrawColor(ren, r, g, b, 255);
			SDL_RenderDrawPoint(ren, winWidth - curPos->x, winHeight - i);
		}

		SDL_SetRenderDrawColor(ren, *oldColor, oldColor[1], oldColor[2], oldColor[3]);
		SDL_SetRenderTarget(ren, oldTarget);

		tex2Pos.x -= 1;
		tex1Pos.x -= 1;

		if (tex1Pos.x <= -winWidth)
		{
			tex1Pos.x = winWidth;
			curPos = &tex1Pos;
			curTex = tex1;
		}
		if (tex2Pos.x <= -winWidth)
		{
			tex2Pos.x = winWidth;
			curPos = &tex2Pos;
			curTex = tex2;
		}
	}
}

void SpectrogramBG::draw()
{
	SDL_RenderCopy(ren, tex1, NULL, &tex1Pos);
	SDL_RenderCopy(ren, tex2, NULL, &tex2Pos);
}

