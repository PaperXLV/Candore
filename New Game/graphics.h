#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <map>
#include <string>
#include "polygon.h"

class Graphics {
public:
	Graphics();
	~Graphics();

	/* SDL_Surface *loadImage
	* loads an image into the _spriteSheets map if it doesnt exist
	* result, each image only loaded once
	* returns the image from map regardless if it was just loaded
	*/

	SDL_Surface *loadImage(const std::string &filepath);

	/* void blitSurfacer
	* Draws a texture to a part of the screen
	*/

	void blitSurfacer(SDL_Texture *source, SDL_Rect *sourceRect, SDL_Rect *destinationRect);

	/* void blitSurfaceRotate
	* Draws a texture to a part of the screen, enables ability to rotate and flip texture
	*/

	void blitSurfaceRotate(SDL_Texture *source, SDL_Rect *sourceRect, SDL_Rect *destinationRect, double angle, SDL_Point rotationPoint, SDL_RendererFlip flip);

	/* void blitSurfaceAlphaRectangle
	* Draws an alphachanneled rectangle to the screen, vital to lighting
	*/

	void blitSurfaceAlphaRectangle(SDL_Texture *source, const SDL_Rect *sourceRect, SDL_Texture *dest, SDL_Rect *destRect);

	/* void blitSurfaceAlphaPolgon
	 * Blits an alphachanneled polygon to another surface
	 */

	void blitSurfaceAlphaPolygon(Polygon polygon, Uint8 alpha, SDL_Texture *dest);

	/* void blitSurfacePolygon
	 * Draws an alphachanneled polygon to the screen, vital to lighting
	 */

	void blitSurfacePolygon(Polygon polygon, Uint8 alpha);

	/* void flip
	* Renders everything to the screen
	*/

	void flip();

	/* void clear
	* Clears the screen
	*/

	void clear();

	SDL_Renderer *getRenderer() const;

private:
	SDL_Window *_window;
	SDL_Renderer *_renderer;

	std::map<std::string, SDL_Surface*> _spritesheets;
};









#endif GRAPHICS_H