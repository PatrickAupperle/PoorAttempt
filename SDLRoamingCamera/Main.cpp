#include <iostream>
#include <SDL.h>
#include <Windows.h>
#include <tuple>
using namespace std;
#include "Background.h"
#include "MusicPlayer.h"
#include "SpectrogramBG.h"
#include <SDL_syswm.h>
#include <SDL_ttf.h>

#include <locale>
#include <codecvt>
#include <string>

#include <io.h>
#include <fcntl.h>
#include <vector>
#include "Song.h"

bool dragging = false;

string getExtension(string in)
{
	auto end = in.rbegin();
	string out;
	while (*end != '.')
	{
		out.insert(0, 1, *end);
		++end;
	}
	return out;
}
//TODO: Add playlist. Vector of objects that contain artist, title, and file;
int main(int argc, char** argv)
{
	std::vector<Song> playlist;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* win = SDL_CreateWindow("SpectrogramPlayer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_BORDERLESS);
	if (!win)
	{
		SDL_Quit();
		return 1;
	}
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (!ren)
	{
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 2;
	}
	TTF_Init();
	{ // Destroy objects declared within before cleaning up SDL
		/*
		Background bg(win, ren);
		if (!bg.setImage("image.bmp"))
		{
			bg.free();
			SDL_DestroyRenderer(ren);
			SDL_DestroyWindow(win);
			SDL_Quit();
			return 3;
		}
		*/
		SDL_Rect titleRect;
		SDL_Rect artistRect;

		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		SDL_GetWindowWMInfo(win, &info);
		MusicPlayer music(info.info.win.window, &playlist, ren);
		music.enqueue(LR"(トランスルーセント.flac)");
		music.playPause();
		SpectrogramBG bg(win, ren, music.getStream());

		int winWidth = 0;
		SDL_GetWindowSize(win, &winWidth, NULL);

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
					else if (e.key.keysym.sym == SDLK_RIGHT)
						music.next();
					else if (e.key.keysym.sym == SDLK_LEFT)
						music.prev();
				}
				else if (e.type == SDL_DROPFILE)
				{
					// This string contains the correct unicode path
					std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
					std::wstring path = converter.from_bytes(e.drop.file);

					string extension = getExtension(e.drop.file);
					cout << extension << endl;

					if (extension == "flac" || extension == "mp3")
					{
						music.enqueue(path);
					} 
					SDL_free(e.drop.file);
					/*
					else
					{ 
						cout << "image" << endl;
						//bg.setImage(e.drop.file);
					}
					*/
					// In the future, we will need to check for file type (image vs music).
					// We should be able to obtain the supported file types to check from both SDL and bass.
					// Also consider logic for drag & drop of multiple files in the future; for example:
					//
					// If the standard behavior is to automatically play a new dropped song, then we should only
					// play the last song dropped if the drag and drop operation involved multiple files at once.
				}
				else if (e.type == SDL_MOUSEBUTTONDOWN)
				{

					if (e.button.button == SDL_BUTTON_LEFT)
					{
						cout << "Mouse Button Down " << e.button.y << endl;
						downPos = std::make_tuple(e.button.x, e.button.y);
						dragging = true;
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
			if ((GetKeyState(VK_LBUTTON) & 0x80) != 0 && dragging)
			{
				int wx, wy;
				POINT currPos;
				GetCursorPos(&currPos);
				SDL_GetWindowPosition(win, &wx, &wy);
				SDL_SetWindowPosition(win, currPos.x - std::get<0>(downPos), currPos.y - std::get<1>(downPos));
			}
			if ((GetKeyState(VK_LBUTTON) & 0x80) == 0)
				dragging = false;
			bg.update();
			SDL_RenderClear(ren);
			bg.draw();
			/*
			SDL_SetRenderDrawColor(ren, 0, 0, 0, 55);
			SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
			SDL_RenderFillRect(ren, &r);
			*/
			int texW, texH;
			SDL_QueryTexture(music.getCurrSong()->titleTex, NULL, NULL, &texW, &texH);
			titleRect = { winWidth / 2 - texW / 2, 50, texW, texH };
			SDL_QueryTexture(music.getCurrSong()->ArtistTex, NULL, NULL, &texW, &texH);
			artistRect = { winWidth / 2 - texW / 2, 50 + texH + 10, texW, texH };
			for (int i = 0; i < playlist.size(); ++i)
			{
				int songW, songH;
				SDL_QueryTexture(playlist[i].texture, NULL, NULL, &songW, &songH);
				SDL_Rect songRect = { winWidth / 2 - songW / 2, 50 + 10 + 2 * texH + 10 + 100 + i * (songH + 10), songW, songH };
				SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawColor(ren, 0, 0, 255, 100);
				if (&playlist[i] == music.getCurrSong())
					SDL_RenderFillRect(ren, &songRect);
				SDL_RenderCopy(ren, playlist[i].texture, NULL, &songRect);
			}
			SDL_RenderCopy(ren, music.getCurrSong()->titleTex, NULL, &titleRect);
			SDL_RenderCopy(ren, music.getCurrSong()->ArtistTex, NULL, &artistRect);

			SDL_RenderPresent(ren);
		}

	}
	TTF_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}