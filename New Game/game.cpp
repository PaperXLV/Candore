#include "game.h"

Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_ShowCursor(0);

	//load data from files or whatever
	this->_camera = Camera(Vector2(globalsettings.SCREEN_WIDTH / 2, globalsettings.SCREEN_HEIGHT / 2));

	Graphics graphics;
	this->_graphics = graphics;

	Input input;
	this->_input = input;

	Audio audio;
	this->_audio = audio;

	//fix loading of these sprites
	Background background = Background(this->_graphics, "Assets/backgrounds/Blue.png");
	this->_background = background;

	//Background foreground = Background(this->_graphics, "Assets/backgrounds/Black.png");
	//this->_foreground = foreground;

	//init more stuff

	srand(time(NULL));

	this->gameLoop();
}

Game::~Game() {

}

namespace {
	const int FPS = 60;
	const int maxFrameTimeMs = 1000 / FPS;
}

void Game::gameLoop() {

	int lastUpdateTimeMs = SDL_GetTicks();

	this->_level = Level(this->_graphics);

	this->_player = Player(this->_graphics, this->_level.getSpawnPosition());

	
	//Enemy Testing
	//Slime newSlime = Slime(this->_graphics, 0, 0, 40, 40, Vector2(this->_level.getSpawnPosition().x + 10, this->_level.getSpawnPosition().y + 10), 3);
	//this->addItem(newSlime);
	

	while (true) {
		this->_input.beginNewFrame();

		if (SDL_PollEvent(&this->_event)) {
			if (this->_event.type == SDL_KEYDOWN) {
				if (this->_event.key.repeat == 0) {
					this->_input.keyDownEvent(this->_event);
				}
			}
			else if (this->_event.type == SDL_KEYUP) {
				this->_input.keyUpEvent(this->_event);
			}
			else if (this->_event.type == SDL_QUIT) {
				return;
			}
		}
		if (this->_input.wasKeyPressed(std::vector<SDL_Scancode> {SDL_SCANCODE_ESCAPE}) == true) {
			return;
		}


		const int currentTimeMs = SDL_GetTicks();
		int elapsedTimeMs = currentTimeMs - lastUpdateTimeMs;

		this->update(std::min(elapsedTimeMs, maxFrameTimeMs));
		lastUpdateTimeMs = currentTimeMs;
		this->draw(this->_graphics);
	}
}

void Game::update(int elapsedTime) {
	//update objects
	this->_player.update(elapsedTime, this->_level, this->_input);
	this->_level.update(elapsedTime);
	//this->_fps.update(elapsedTime);
	
	for (int i = 0; i < this->_enemies.size(); i++) {
		//gonna need to add in some checks before this here cast so we don't get errors on different enemy types
		dynamic_cast<Slime*>(this->_enemies[i].get())->update(elapsedTime, this->_level);
	}

	//whatnot

	this->_camera.centerOnObject(this->_player.getCenter());
}

void Game::draw(Graphics &graphics) {
	graphics.clear();
	//draw objects from back to front
	//apply camera offset
	this->_background.draw(graphics);
	this->_level.draw(graphics, this->_camera.getOffset());
	this->_player.draw(graphics, this->_camera.getOffset());

	std::vector<Polygon> lights = this->_level.getLightPolygons(this->_camera.getOffset());
	SDL_Texture *texture = SDL_CreateTexture(graphics.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_RENDERER_TARGETTEXTURE, globalsettings.SCREEN_WIDTH, globalsettings.SCREEN_HEIGHT);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	for (int i = 0; i < lights.size(); i++) {
		graphics.blitSurfaceAlphaPolygon(lights[i], 100, texture);
	}

	SDL_RenderCopy(graphics.getRenderer(), texture, NULL, NULL);

	//this->_foreground.setTexture(texture);


	/*
	SDL_Rect rect{ 50, 50, 100, 100 };
	const SDL_Rect* rectp = &rect;


	SDL_SetRenderTarget(graphics.getRenderer(), this->_foreground.getTexture());
	SDL_Surface *surface = SDL_CreateRGBSurface(0, 200, 200, 32, 0, 0, 0, 0);
	SDL_FillRect(surface, rectp, SDL_MapRGBA(surface->format, 255, 0, 0, 255));
	SDL_Texture *texture = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(texture, 50);
	SDL_RenderCopy(graphics.getRenderer(), texture, NULL, NULL);
	SDL_SetRenderTarget(graphics.getRenderer(), NULL);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	*/

	//this->_foreground.draw(graphics);
	SDL_DestroyTexture(texture);

	//this->_fps.displayFPS();

	for (int i = 0; i < this->_enemies.size(); i++) {
		dynamic_cast<Slime*>(this->_enemies[i].get())->draw(graphics, this->_camera.getOffset());
	}

	graphics.flip();
}