/**
 * Copyright (c) 2017, Vincent Glize <vincent.glize@live.fr>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Hud.h"
#include "Graphics.h"
#include "Game.h"
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <iostream>

Hud::Hud()
{

}

void Hud::paint(SDL_Renderer *renderer)
{
	SDL_Rect rect = {0, 0, Graphics::get_w(), 20};
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect rect_finish = {0, 0,((float) Game::get_time() / (float) Game::get_time_max()) * Graphics::get_w()
			, 20};
	SDL_RenderFillRect(renderer, &rect_finish);

	if (Game::is_pause()) {
		TTF_Font *font = TTF_OpenFont("/home/vincent/CLionProjects/commutecpp/data/font/rainbow_bridge.ttf",300);
		if (!font) {
			std::cerr << "Error charge font" << std::endl;
			exit(1);
		}
		SDL_Color color = {255, 255, 255};
		SDL_Surface *surface_message = TTF_RenderText_Solid(font, "Pause", color);
		SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, surface_message);
		rect = {Graphics::get_w() / 2 - 600/2, Graphics::get_h() / 2 - 300/2, 600, 300};
		SDL_RenderCopy(renderer, text, NULL, &rect);
	}
	else if (Game::is_win()) {
		TTF_Font *font = TTF_OpenFont("/home/vincent/CLionProjects/commutecpp/data/font/rainbow_bridge.ttf", 100);
		if (!font) {
			std::cerr << "Error charge font" << std::endl;
			exit(1);
		}
		SDL_Color color = {255, 255, 255};
		SDL_Surface *surface_message = TTF_RenderText_Solid(font, "YOU WIN !", color);
		SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, surface_message);
		rect = {Graphics::get_w() / 2 - 600/2, Graphics::get_h() / 2 - 300 / 2, 600, 300};
		SDL_RenderCopy(renderer, text, NULL, &rect);
	}
	else if (Game::is_loose()) {
		TTF_Font *font = TTF_OpenFont("/home/vincent/CLionProjects/commutecpp/data/font/rainbow_bridge.ttf", 100);
		if (!font) {
			std::cerr << "Error charge font" << std::endl;
			exit(1);
		}
		SDL_Color color = {255, 255, 255};
		SDL_Surface *surface_message = TTF_RenderText_Solid(font, "YOU LOOSE !", color);
		SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, surface_message);
		rect = {Graphics::get_w() / 2 - 600/2, Graphics::get_h() / 2 - 300 / 2, 600, 300};
		SDL_RenderCopy(renderer, text, NULL, &rect);
	}
}