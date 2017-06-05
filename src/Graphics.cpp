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

#include <SDL_render.h>
#include <SDL.h>
#include "Graphics.h"
#include "Map.h"
#include "Car.h"

Graphics::~Graphics()
{
	delete m_r;
}

void Graphics::openWindow(Map *map)
{
	m_map = map;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Initialization error:%s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);
	if (SDL_CreateWindowAndRenderer(m_map->get_w(), m_map->get_h(), SDL_WINDOW_RESIZABLE, &m_window, &m_r)) {
		fprintf(stderr, "Couldn't create window and renderer: %s", SDL_GetError());
		exit(1);
	}
	SDL_RenderSetLogicalSize(m_r, m_map->get_w(), m_map->get_h());
}

void Graphics::loadTiles()
{
	m_map->set_texture(SDL_CreateTextureFromSurface(m_r, m_map->get_surface()));

	for (int i = 0; i < CAR_MAX; i++) {
		SDL_Surface *loadedImage = SDL_LoadBMP(Car::s_tilenames[i]);
		if (loadedImage) {
			uint32_t colorkey = SDL_MapRGB(loadedImage->format, 0xff, 0xff, 0xff);
			SDL_SetColorKey(loadedImage, SDL_TRUE, colorkey);
			Car::s_tile[i] = SDL_CreateTextureFromSurface(m_r, loadedImage);
			SDL_FreeSurface(loadedImage);
		}
		else {
			std::cout << stderr << "Missing file " << Car::s_tilenames[i] << " : " << SDL_GetError() << std::endl;
		}
	}
}

void Graphics::paint()
{
	SDL_RenderClear(m_r);
	//SDL_SetRenderDrawColor(m_r, 0, 0, 0, 255);
	//SDL_Rect
	SDL_RenderCopy(m_r, m_map->get_texture(), NULL, m_map->get_rect());

	SDL_RenderPresent(m_r);
}
