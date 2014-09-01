#pragma once

#include <SDL.h>
#include <string>

enum class Direction {UP, DOWN, RIGHT, LEFT, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT};

class Background
{
public:
	Background(SDL_Window* window, SDL_Renderer* renderer);
	bool setImage(std::string path);
	void update();
	void draw();
	void free();
	~Background();
private:
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Texture* bg;
	
	int iw, ih;
	SDL_Rect pos;
	Direction dir;
	bool up, down, right, left;
	bool speed;

	bool fadeIn;
	bool fadeOut;
	int fade;
	int fadeSpeed;

	void updateFade();
	void updatePos();
	void resetPos();
	void resetDir();
	void setDirection(Direction d);
};

