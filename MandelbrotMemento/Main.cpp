#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

#include "Mandelbrot.h"
#include "Memento.h"


const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const double SCREEN_RATIO = ((double) SCREEN_WIDTH) / SCREEN_HEIGHT;

const int MAX_ITERS = 1500;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
// Pixel array for the mandelbrot drawing
uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
// Used for rendering the mandelbrot to the window
SDL_Texture *screenTexture = NULL;

// Save previous states of mandelbrot.
std::vector<Memento*> mementos;

int main(int argc, char *args[]) {
	//Initialize graphics system
	int result = SDL_Init(SDL_INIT_VIDEO);
	if (result < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return result;
	}

	window = SDL_CreateWindow("Mandelbrot Explorer",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (window == NULL || renderer == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	screenTexture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	Mandelbrot mandelbrot(MAX_ITERS, SCREEN_WIDTH, SCREEN_HEIGHT);

	mandelbrot.render(renderer, screenTexture);

	// Main loop
	SDL_Event event;
	SDL_Rect zoomRect = {1, 1, 1, 1};
	BOOL doDrawRect = FALSE;
	BOOL isRunning = TRUE;
	int dx, dy;
	while (isRunning && SDL_WaitEvent(&event)) {
		switch (event.type) {
		// Start drawing zooming rectangle
		case SDL_MOUSEBUTTONDOWN:
			zoomRect.x = event.motion.x;
			zoomRect.y = event.motion.y;
			doDrawRect = TRUE;
			break;

		// Draw zooming rectangle
		case SDL_MOUSEMOTION:
			dx = event.motion.x - zoomRect.x;
			dy = event.motion.y - zoomRect.y;

			// Cap zoom at window aspect ratio
			// TODO Don't cap at ratio but calculate biggest bounding box
			dx = abs(dx) > SCREEN_RATIO * abs(dy) ?
				copysign(1.0, dx) * SCREEN_RATIO * abs(dy) : dx;
			dy = abs(dy) > (1.0 / SCREEN_RATIO) * abs(dx) ?
				copysign(1.0, dy) * (1.0 / SCREEN_RATIO) * abs(dx) : dy;

			zoomRect.w = dx;
			zoomRect.h = dy;

			if (doDrawRect) {
				SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
				SDL_RenderDrawRect(renderer, &zoomRect);
				SDL_RenderPresent(renderer);
			}
			break;

		// Zoom in.
		case SDL_MOUSEBUTTONUP:
			doDrawRect = FALSE;
			mementos.push_back(mandelbrot.createMemento());
			mandelbrot.zoomIn(zoomRect);
			mandelbrot.render(renderer, screenTexture);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				mandelbrot.changeIterations(-1);
				break;

			case SDLK_RIGHT:
				mandelbrot.changeIterations(1);
				break;

			case SDLK_F5:
				mandelbrot.render(renderer, screenTexture);
				break;

			case SDLK_BACKSPACE:
				if (mementos.size() > 0) {
					Memento* m = mementos.back();
					mementos.pop_back();

					mandelbrot.setMemento(m);
					mandelbrot.render(renderer, screenTexture);

					delete m;
				}
				break;
			}
			break;

		case SDL_QUIT:
			isRunning = FALSE;
			break;
		}
	}

	// Clean up
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(screenTexture);

	SDL_Quit();

	return 0;
}

