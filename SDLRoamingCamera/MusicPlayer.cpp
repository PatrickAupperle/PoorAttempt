#include "MusicPlayer.h"

#include <stdexcept>
#include <iostream>
using std::cout;
using std::endl;

MusicPlayer::MusicPlayer(HWND win)
{
	if (!BASS_Init(-1, 44100, 0, win, NULL))
		throw new std::runtime_error("Can't initialize device");
	playing = false;
	cout << "init" << endl;
}

void MusicPlayer::loadFile(std::string file)
{
	if (!(str = BASS_StreamCreateFile(FALSE, file.c_str(), 0, 0, 0))) 
		throw new std::runtime_error("Can't Open File");
	cout << "load" << endl;
}

void MusicPlayer::playPause()
{
	if (playing)
	{
		if (!BASS_ChannelPause(str))
			cout << "Couldn't Pause" << endl;
		else
			playing = false;
		cout << "pause" << endl;
	}
	else
	{
		if (!BASS_ChannelPlay(str, FALSE))
			cout << "Couldn't Play " << playErrorString() << endl;
		else
			playing = true;
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
	BASS_Free();
}
