#include "MusicPlayer.h"

#include <locale>
#include <codecvt>

#include <stdexcept>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <tags.h>

#include <SDL_ttf.h>

//Taglib
#include <tag.h>
#include <fileref.h>

using std::cout;
using std::wcout;
using std::endl;

MusicPlayer::MusicPlayer(HWND win, std::vector<Song>* p, SDL_Renderer* r)
{
	if (!BASS_Init(-1, 44100, 0, win, NULL))
		throw new std::runtime_error("Can't initialize device");
	if (!BASS_PluginLoad("bassflac.dll", 0))
		cout << "Plugin load failed" << endl;

	playlist = p;
	currSong = 0;
	
	ren = r;

	font = TTF_OpenFont("MSMINCHO.TTF", 50);
	trackFont = TTF_OpenFont("MSMINCHO.TTF", 16);
	cout << "init" << endl;
}

void CALLBACK nextCallback(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	((MusicPlayer*)user)->next();
}

void MusicPlayer::loadFile(Song* song)
{
	if (str)
	{
		BASS_StreamFree(str);
	}
	if (!(str = BASS_StreamCreateFile(FALSE, song->file.c_str(), 0, 0, BASS_UNICODE)))
		throw new std::runtime_error("Can't Open File");

	BASS_ChannelSetSync(str, BASS_SYNC_END, NULL, nextCallback, this);
	cout << "load" << endl;

	fileName = song->file;
	TagLib::FileRef f(song->file.c_str());
	_setmode(_fileno(stdout), _O_U16TEXT);
	wcout << f.tag()->artist().toCWString() << " - " << f.tag()->title().toCWString() << endl;
	_setmode(_fileno(stdout), _O_TEXT);

	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, f.tag()->title().toCString(true), { 46, 139, 87 });
	song->titleTex = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	surface = TTF_RenderUTF8_Blended(font, f.tag()->artist().toCString(true), { 46, 139, 87 });
	song->ArtistTex = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	int texW = 0;
	int texH = 0;
}

void MusicPlayer::enqueue(std::wstring file)
{
	TagLib::FileRef f(file.c_str());
	SDL_Surface* surface = TTF_RenderUTF8_Blended(trackFont, (f.tag()->artist() + " - " + f.tag()->title()).toCString(true), { 46, 139, 87 });
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	playlist->push_back(Song(f.tag()->title().toCString(true), f.tag()->artist().toCString(true), file, tex));
}

void MusicPlayer::playPause()
{
	if (BASS_ChannelIsActive(str) == BASS_ACTIVE_PLAYING)
	{
		BASS_ChannelPause(str);
		cout << "pause" << endl;
	}
	else if (BASS_ChannelIsActive(str) == BASS_ACTIVE_STOPPED)
	{
		loadFile(&(playlist->at(currSong)));
		BASS_ChannelPlay(str, FALSE);
		cout << "play" << endl;
	}
	else
	{
		BASS_ChannelPlay(str, FALSE);
		cout << "play" << endl;
	}
}

std::string MusicPlayer::playErrorString()
{
	int code = BASS_ErrorGetCode();
	switch (code)
	{
	case BASS_ERROR_HANDLE:
		return "BASS_ERROR_HANDLE";
	case BASS_ERROR_START:
		return "Error Start";
	case BASS_ERROR_DECODE:
		return "BASS_ERROR_DECODE";
	case BASS_ERROR_BUFLOST:
		return "BASS_ERROR_BUFLOST";
	case BASS_ERROR_NOHW:
		return "BASS_ERROR_NOHW";
	}
	return "NOne of them";
}

MusicPlayer::~MusicPlayer()
{
	TTF_CloseFont(font);
	TTF_CloseFont(trackFont);
	BASS_Free();
}

HSTREAM* MusicPlayer::getStream()
{
	return &str;
}

std::string MusicPlayer::getTitle()
{
	TagLib::FileRef f(fileName.c_str());
	return f.tag()->title().toCString(true);
}

std::string MusicPlayer::getArtist()
{
	TagLib::FileRef f(fileName.c_str());
	return f.tag()->artist().toCString(true);
}

void MusicPlayer::next()
{
	if (currSong + 1 < playlist->size())
	{
		loadFile(&playlist->at(currSong + 1));
		++currSong;
		SDL_DestroyTexture(playlist->at(currSong - 1).ArtistTex);
		SDL_DestroyTexture(playlist->at(currSong - 1).titleTex);
		BASS_ChannelPlay(str, FALSE);
	}
}

void MusicPlayer::prev()
{
	if (currSong - 1 > 0)
	{
		loadFile(&playlist->at(currSong - 1));
		--currSong;
		SDL_DestroyTexture(playlist->at(currSong + 1).ArtistTex);
		SDL_DestroyTexture(playlist->at(currSong + 1).titleTex);
		BASS_ChannelPlay(str, FALSE);
	}
}

Song* MusicPlayer::getCurrSong()
{
	return &playlist->at(currSong);
}
