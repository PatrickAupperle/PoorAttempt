#include <iostream>
#include <SDL.h>
#include <Windows.h>
#include <tuple>
using namespace std;
#include "Background.h"
#include "MusicPlayer.h"
#include <SDL_syswm.h>

#define _UNICODE

int main(int argc, char** argv)
{

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* win = SDL_CreateWindow("Roaming Camera", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_BORDERLESS);
	if (!win)
	{
		SDL_Quit();
		return 1;
	}
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!ren)
	{
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 2;
	}
	{ // Destroy objects declared within before cleaning up SDL
		Background bg(win, ren);
		if (!bg.setImage("image.bmp"))
		{
			bg.free();
			SDL_DestroyRenderer(ren);
			SDL_DestroyWindow(win);
			SDL_Quit();
			return 3;
		}
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		SDL_GetWindowWMInfo(win, &info);
		MusicPlayer music(info.info.win.window);
		music.loadFile(LR"(トランスルーセント.flac)");
		music.playPause();

		std::tuple<int, int> downPos;
		bool quit = false;
		bool moving = false;
		SDL_Rect r;
		r.x = 10;
		r.y = 10;
		r.w = 100;
		r.h = 100;

		while (!quit)
		{
			SDL_Event e;
			SDL_PumpEvents();
			while (SDL_PollEvent(&e))
			{
				//If user closes the window
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				if (e.type == SDL_KEYDOWN)
				{
					if (e.key.keysym.sym == SDLK_ESCAPE)
						quit = true;
				}
				else if (e.type == SDL_DROPFILE)
				{
					std::string imagePath = e.drop.file;
					bg.setImage(imagePath);
				}
				else if (e.type == SDL_MOUSEBUTTONDOWN)
				{

					if (e.button.button == SDL_BUTTON_LEFT)
					{
						cout << "Mouse Button Down " << e.button.y << endl;
						downPos = std::make_tuple(e.button.x, e.button.y);
						/*GetCursorPos(&down);
						int wx, wy;
						SDL_GetWindowPosition(win, &wx, &wy);
						downPos = std::make_tuple(down.x - wx, down.y - wy);*/
						//SDL_SetWindowGrab(win, SDL_TRUE);
					}
					else if (e.button.button == SDL_BUTTON_RIGHT)
						music.playPause();
				}
				
			}
			/*
			if ((GetKeyState(VK_LBUTTON) & 0x80) != 0)
			{
			int wx, wy;
			POINT currPos;
			GetCursorPos(&currPos);
			SDL_GetWindowPosition(win, &wx, &wy);
			SDL_SetWindowPosition(win, currPos.x - std::get<0>(downPos), currPos.y - std::get<1>(downPos));
			cout << "downPos: " << std::get<0>(downPos) << " " << std::get<1>(downPos) << endl;
			cout << "currPos: " << currPos.x << " " << currPos.y << endl;
			}
			*/
			if ((GetKeyState(VK_LBUTTON) & 0x80) != 0)
			{
				int wx, wy;
				POINT currPos;
				GetCursorPos(&currPos);
				SDL_GetWindowPosition(win, &wx, &wy);
				SDL_SetWindowPosition(win, currPos.x - std::get<0>(downPos), currPos.y - std::get<1>(downPos));
				cout << "downPos: " << std::get<0>(downPos) << " " << std::get<1>(downPos) << endl;
				cout << "currPos: " << currPos.x << " " << currPos.y << endl;
			}
			bg.update();
			SDL_RenderClear(ren);
			bg.draw();
			SDL_SetRenderDrawColor(ren, 0, 0, 0, 55);
			SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
			SDL_RenderFillRect(ren, &r);
			SDL_RenderPresent(ren);
		}

	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}