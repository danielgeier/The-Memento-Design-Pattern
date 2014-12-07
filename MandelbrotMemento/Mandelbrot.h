#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <SDL.h>
#include "Memento.h"
#include "Dimensions.h"

class Memento;


class Mandelbrot
{

public:
	Mandelbrot(int iters, int screenWidth, int screenHeight);
	~Mandelbrot();

	void changeIterations(int num);
	void zoomIn(const SDL_Rect zoomRect);
	void render(SDL_Renderer* renderer, SDL_Texture* texture);

	Memento* createMemento();
	void setMemento(Memento* memento);
private:
	friend class Memento;

	// Logical bounds of the fractal
	Dimensions bounds;
	int iterations;
	int width, height;
	uint32_t* pixels;
	bool doCalc;

	int calculatePoint(double ix, double iy);
};

#endif
