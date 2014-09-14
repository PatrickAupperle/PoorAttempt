#pragma once

#include <string>
#include <SDL.h>
class Song
{
public:
	Song(std::string t, std::string a, std::wstring f, SDL_Texture* tex);
	~Song();

	std::string title;
	std::string artist;
	std::wstring file;

	SDL_Texture* texture;
	SDL_Texture* titleTex;
	SDL_Texture* ArtistTex;
};

