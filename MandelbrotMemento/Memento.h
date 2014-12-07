#ifndef MEMENTO_H
#define MEMENTO_H
#include <SDL.h>

#include "Mandelbrot.h"
#include "Dimensions.h"

class Mandelbrot;

class Memento
{
public:
	~Memento();

private:
	friend class Mandelbrot;

	uint32_t* pixels;
	Dimensions bounds;

	Memento();
	void setPixels(uint32_t* oldPixels, int size);
	void setBounds(Dimensions bounds);

	uint32_t* getPixels();
	Dimensions getBounds();
};

#endif
