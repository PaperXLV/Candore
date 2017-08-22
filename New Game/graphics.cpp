#include "graphics.h"
#include <SDL_Image.h>
#include "globals.h"

/* graphics.cpp
* Holds all info for handling graphics ;)
*/
Graphics::Graphics() {
	this->_window = SDL_CreateWindow("Candore (pre alpha)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, globalsettings.SCREEN_WIDTH, globalsettings.SCREEN_HEIGHT, 0);
	this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
}

Graphics::~Graphics() {
	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);
}

SDL_Surface *Graphics::loadImage(const std::string &filepath) {
	if (this->_spritesheets.count(filepath) == 0) {
		this->_spritesheets[filepath] = IMG_Load(filepath.c_str());
	}
	return this->_spritesheets[filepath];
}

void Graphics::blitSurfacer(SDL_Texture *source, SDL_Rect *sourceRect, SDL_Rect *destinationRect) {
	SDL_RenderCopy(this->_renderer, source, sourceRect, destinationRect);
}

void Graphics::blitSurfaceRotate(SDL_Texture *source, SDL_Rect *sourceRect, SDL_Rect *destinationRect, double angle, SDL_Point rotationPoint, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(this->_renderer, source, sourceRect, destinationRect, angle, &rotationPoint, flip);
}

void Graphics::blitSurfaceAlphaRectangle(SDL_Texture *source, const SDL_Rect *sourceRect, SDL_Texture *dest, SDL_Rect *destRect) {
	//SDL_SetRenderTarget
	//SDL_RenderCopy(this->_renderer, )
}

//Thought, create array of polygons, put on blank texture, make everything outside black, or find a way to create alpha on black texture
void Graphics::blitSurfaceAlphaPolygon(Polygon polygon, Uint8 alpha, SDL_Texture *dest) {	
	SDL_SetRenderTarget(this->_renderer, dest);
	polygonRGBA(this->_renderer, polygon.x, polygon.y, polygon.points, 255, 0, 255, alpha);
	SDL_SetRenderTarget(this->_renderer, NULL);

	//SDL_Texture *temp = SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, globalsettings.SCREEN_WIDTH, globalsettings.SCREEN_HEIGHT);
	//SDL_SetTextureBlendMode(temp, SDL_BLENDMODE_BLEND);
	//SDL_SetTextureAlphaMod(temp, 50);
	//SDL_SetRenderTarget(this->_renderer, temp);
	//SDL_RenderCopy(this->_renderer, dest, NULL, NULL);

	//SDL_SetRenderTarget(this->_renderer, dest);
	//filledPolygonRGBA(this->_renderer, vx, vy, numberOfPoints, 255, 0, 255, 0);
	//SDL_SetRenderTarget(this->_renderer, dest);
	//SDL_RenderCopy(this->_renderer, temp, NULL, NULL);
	//SDL_SetRenderTarget(this->_renderer, NULL);
	//SDL_DestroyTexture(dest);
	//dest = temp;
	//SDL_DestroyTexture(temp);


}

//TODO::lighting
//create alpha filled polygon and SDL_BlitSurface it onto the image to add alpha channel, I think that works?
void Graphics::blitSurfacePolygon(Polygon polygon, Uint8 alpha) {
	filledPolygonRGBA(this->_renderer, polygon.x, polygon.y, polygon.points, 0, 0, 255, alpha);
	//SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
}

void Graphics::flip() {
	SDL_RenderPresent(this->_renderer);
}

void Graphics::clear() {
	SDL_RenderClear(this->_renderer);
}

SDL_Renderer *Graphics::getRenderer() const {
	return this->_renderer;
}