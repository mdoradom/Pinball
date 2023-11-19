#pragma once
#include "Module.h"
#include "SDL_ttf/include/SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

class ModuleFonts : public Module
{
public:
	ModuleFonts(Application* app, bool start_enabled = true);
	~ModuleFonts();

	bool Init();
	bool CleanUp();

	bool drawText(const char* string, SDL_Color color, int x, int y);

public:
	/*typedef struct text {
		int x;
		int y;
		SDL_Texture* texture;
	}text;

	p2List<text*> text_list;*/

	TTF_Font* font;
	SDL_Texture* texture;
	SDL_Texture* LoadText(const char* text, SDL_Color color);
};