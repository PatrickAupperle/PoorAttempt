#include "MusicPlayer.h"

#include <stdexcept>
#include <iostream>
using std::cout;
using std::wcout;
using std::endl;

MusicPlayer::MusicPlayer(HWND win)
{
	if (!BASS_Init(-1, 44100, 0, win, NULL))
		throw new std::runtime_error("Can't initialize device");
	if (!BASS_PluginLoad("bassflac.dll", 0))
		cout << "Plugin load failed" << endl;
	playing = false;
	cout << "init" << endl;
}

void MusicPlayer::loadFile(std::wstring file)
{
	wcout << file.c_str() << endl;
	if (!(str = BASS_StreamCreateFile(FALSE, file.c_str(), 0, 0, BASS_UNICODE)))
		throw new std::runtime_error("Can't Open File");
	cout << "load" << endl;
}

void MusicPlayer::playPause()
{
	if (playing)
	{
		if (BASS_ChannelPause(str))
			playing = false;
		else
			cout << "Couldn't Pause" << endl;

		cout << "pause" << endl;
	}
	else
	{
		if (BASS_ChannelPlay(str, FALSE))
			playing = true;
		else
			cout << "Couldn't Play " << playErrorString() << endl;
			
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
