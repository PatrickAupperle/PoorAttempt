#pragma once

#include <string>
#include <Windows.h>
#include <bass.h>


class MusicPlayer
{
public:
	MusicPlayer(HWND win);
	~MusicPlayer();
	void playPause();
	void loadFile(std::wstring file);

private:
	HSTREAM str;
	bool playing;

	std::string playErrorString();
};

