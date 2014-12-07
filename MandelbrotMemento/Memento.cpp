#include "Memento.h"
#include <string>

Memento::Memento()
{
	printf("%s", "Created Memento\n");
}

void Memento::setPixels(uint32_t* oldPixels, int size)
{
	pixels = new uint32_t[size];
	memcpy(pixels, oldPixels, size * sizeof(uint32_t));
}

void Memento::setBounds(Dimensions oldBounds)
{
	bounds = oldBounds;
}

Memento::~Memento()
{
	printf("%s", "Destroyed Memento\n");
	delete[] pixels;
}

uint32_t* Memento::getPixels()
{
	return pixels;
}

Dimensions Memento::getBounds()
{
	return bounds;
}