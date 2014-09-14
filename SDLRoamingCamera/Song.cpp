#include "Song.h"


Song::Song(std::string t, std::string a, std::wstring f, SDL_Texture* tex)
{
	title = t;
	artist = a;
	file = f;
	texture = tex;
}

Song::~Song()
{

}
