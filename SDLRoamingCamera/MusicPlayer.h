#pragma once

#include <string>
#include <Windows.h>
#include <bass.h>
#include "Song.h"
#include <vector>
#include <SDL_ttf.h>

class MusicPlayer
{
public:
	MusicPlayer(HWND win, std::vector<Song>* p, SDL_Renderer* ren);
	~MusicPlayer();
	void playPause();
	void enqueue(std::wstring file);
	HSTREAM* getStream();
	std::string getArtist();
	std::string getTitle();
	void next();
	void prev();
	Song* getCurrSong();

private:
	HSTREAM str;
	SDL_Renderer* ren;
	void loadFile(Song* song);
	std::wstring fileName;
	std::string playErrorString();
	std::vector<Song>* playlist;
	int currSong;
	TTF_Font* font;
	TTF_Font* trackFont;
};

