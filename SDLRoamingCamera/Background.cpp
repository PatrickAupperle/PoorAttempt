#include "Background.h"

#include <SDL_image.h>
#include <cstdlib>
#include <ctime>

Background::Background(SDL_Window* window, SDL_Renderer* renderer)
{
	win = window;
	ren = renderer;
	bg = NULL;
	fadeSpeed = 5;
	fade = 0;
	speed = 1;
	srand(time(NULL));
}

void Background::setDirection(Direction d)
{
	dir = d;
	up = false;
	down = false;
	left = false;
	right = false;
	switch (d)
	{
	case Direction::UP_LEFT:
		left = true;
	case Direction::UP:
		up = true;
		break;
	case Direction::DOWN_RIGHT:
		right = true;
	case Direction::DOWN:
		down = true;
		break;
	case Direction::DOWN_LEFT:
		down = true;
	case Direction::LEFT:
		left = true;
		break;
	case Direction::UP_RIGHT:
		up = true;
	case Direction::RIGHT:
		right = true;
		break;
	}
}

bool Background::setImage(std::string path)
{
	if (bg)
		SDL_DestroyTexture(bg);
	bg = IMG_LoadTexture(ren, path.c_str());
	if (!bg)
		return false;
	resetDir();
	return true;
}

void Background::resetDir()
{
	Direction d = Direction(rand() % 8);
	while (dir == d)
		d = Direction(rand() % 8);
	setDirection(d);
	resetPos();


}

void Background::resetPos()
{
	fadeIn = true;
	fadeOut = false;

	int ww, wh;
	SDL_QueryTexture(bg, NULL, NULL, &iw, &ih);
	SDL_GetWindowSize(win, &ww, &wh);

	if (iw > ww * 2 && ih > wh * 2)
	{
		pos.h = wh;
		pos.w = ww;
	}
	else
	{
		if ((wh * 2 / ih) > (ww * 2 / iw))
		{
			pos.h = wh * ih / (wh * 2);
			pos.w = ww * ih / (wh * 2);
		}
		else
		{
			pos.h = wh * iw / (ww * 2);
			pos.w = ww * iw / (ww * 2);
		}

	}
	pos.x = pos.y = 0;
	
	if (up || down)
		pos.x = rand() % (iw - ww);
	if (left || right)
		pos.y = rand() % (ih - wh);
	if (up)
		pos.y = ih - wh;
	if (left)
		pos.x = iw - ww;
	if (down && right)
		pos.x = pos.y = 0;

}

void Background::update()
{
	updateFade();
	updatePos();
}

void Background::updateFade()
{
	if ((right && (iw - (pos.x + pos.w)) / speed < 255 / fadeSpeed) ||
		(down && (ih - (pos.y + pos.h)) / speed < 255 / fadeSpeed) ||
		(up && pos.y < 255 / fadeSpeed) ||
		(left && pos.x < 255 / fadeSpeed))
		fadeOut = true;


	if (fadeIn)
	{
		fade += fadeSpeed;
		if (fade >= 255)
		{
			fade = 255;
			fadeIn = false;
		}
	}
	else if (fadeOut)
	{
		fade -= fadeSpeed;
		if (fade <= 0)
		{
			fade = 0;
			fadeOut = false;
		}
	}
	SDL_SetTextureColorMod(bg, fade, fade, fade);
}

int framesBetweenMoves = 0;

void Background::updatePos()
{
	framesBetweenMoves++;
	if (framesBetweenMoves == 5)
	{
		framesBetweenMoves = 0;
		int dx = 0, dy = 0;
		if (down)
			dy = speed;
		if (up)
			dy = -speed;
		if (left)
			dx = -speed;
		if (right)
			dx = speed;
		pos.x += dx;
		pos.y += dy;

		if (iw <= (pos.x + pos.w) || ih <= (pos.y + pos.h) || pos.x < 0 || pos.y < 0)
			resetDir();
	}

}

void Background::draw()
{
	SDL_RenderCopy(ren, bg, &pos, NULL);
}

void Background::free()
{
	if (bg)
		SDL_DestroyTexture(bg);
}

Background::~Background()
{
	if (bg)
		SDL_DestroyTexture(bg);
}
