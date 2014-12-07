#include <stdio.h>

#include "Mandelbrot.h"
#include <string>


Mandelbrot::Mandelbrot(int iters, int screenWidth, int screenHeight)
{
	iterations = iters;
	width = screenWidth;
	height = screenHeight;
	pixels = new uint32_t[width * height];
	bounds = { -2, -1, 3, 2 };
	doCalc = true;
}

Mandelbrot::~Mandelbrot()
{
	delete[] pixels;
}

void Mandelbrot::changeIterations(int num)
{
	doCalc = true;

	iterations += num;

	if (iterations < 0) {
		iterations = 0;
	}
}

void Mandelbrot::zoomIn(SDL_Rect zoomRect)
{
	doCalc = true;

	bounds.x += ((double)zoomRect.x / width) * bounds.w;
	bounds.w *= ((double)zoomRect.w) / width;
	bounds.y += ((double)zoomRect.y / height) * bounds.h;
	bounds.h *= ((double)zoomRect.h) / height;

	printf("bounds.x = %.30f\n"
		"bounds.y = %.30f\n"
		"bounds.w = %.30f\n"
		"bounds.h = %.30f\n\n",
		bounds.x, bounds.y, bounds.w, bounds.h);
}

void Mandelbrot::render(SDL_Renderer* renderer, SDL_Texture* texture)
{
	printf("%s", "Rendering mandelbrot...\n");
	if (!doCalc) {
		SDL_UpdateTexture(texture, NULL, pixels,
		width * sizeof(uint32_t));
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		return;
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			pixels[i * width + j] = calculatePoint(j, i);
		}
		// if (i % 10 == 0 || i == SCREEN_HEIGHT - 1) {
		SDL_UpdateTexture(texture, NULL, pixels,
			width * sizeof(uint32_t));
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		// }
	}

	doCalc = false;
}

int Mandelbrot::calculatePoint(double ix, double iy) {
	double x0 = bounds.x + bounds.w * ix / width;
	double y0 = bounds.y + bounds.h * iy / height;
	double x = 0;
	double y = 0;
	double xtemp, ytemp;
	int iters;

	for (iters = 0; iters < iterations && x*x + y*y < 4; iters++) {
		xtemp = x*x - y*y + x0;
		ytemp = 2 * x*y + y0;

		// Periodicity check
		if (x == xtemp && y == ytemp) {
			iters = iterations;
			break;
		}

		x = xtemp;
		y = ytemp;
	}

	iters = iters == iterations ? 0 : iters;

	// Normalize iterations to 255.
	iters *= 255. / iterations;
	// Shift to get more interesting colors.
	return iters | (iters << 10);
}

Memento* Mandelbrot::createMemento()
{
	Memento* m = new Memento();
	m->setPixels(pixels, width * height);
	m->setBounds(bounds);
	return m;
}

void Mandelbrot::setMemento(Memento* memento)
{
	memcpy(pixels,memento->getPixels(), width * height * sizeof(uint32_t));
	bounds = memento->getBounds();
	doCalc = false;
}