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

#include <SDL_surface.h>
#include <SDL_render.h>
#include "Map.h"
#include "Graphics.h"
#include "Car.h"

Map::~Map()
{
	delete m_car;
	for (const auto &car : m_cars) {
		delete car;
	}
	for (int i = 0; i < CAR_MAX; i++) {
		SDL_free(Car::s_tile[i]);
	}
	SDL_FreeSurface(m_s);
	SDL_free(m_t);
	delete m_s;
}

Map::Map(Graphics *graphics, const std::string &map, int w, int h) : m_graphics(graphics)
{
	std::string m = map + ".bmp";
	m_s = SDL_LoadBMP(m.c_str());
	if (!m_s) {
		std::cerr << "The map " << m << " doesnt exist !" << std::endl;
		return;
	}
	std::string col = map + "_collision.bmp";
	m_s_collision = SDL_LoadBMP(col.c_str());
	if (!m_s_collision) {
		std::cerr << "The map " << col << " doesnt exist !" << std::endl;
		return;
	}
	if (m_graphics) {
		m_t = SDL_CreateTextureFromSurface(m_graphics->get_renderer(), m_s);
	}


	m_rect.w = w;
	m_rect.h = h;
	m_rect.x = 0;
	m_rect.y = 0;
}

void Map::loadCollision()
{
	int pixel = 0;
	m_types_maps = new TypeMap*[1280];
	for (int l = 0; l < get_w()-1; l++) {
		m_types_maps[l] = new TypeMap[800];
		for (int h = 0; h < get_h()-1; h++) {
			pixel = Graphics::getpixel(m_s_collision, l, h);
			if (pixel == 0xf00) {
				TypeMap type(BUILDING);
				m_types_maps[l][h] = type;
			}
			else if (pixel == 0x0f0) {
				TypeMap type(GRASS);
				m_types_maps[l][h] = type;
			}
			else {
				TypeMap type(ROAD);
				m_types_maps[l][h] = type;
			}
		}
	}
	SDL_FreeSurface(m_s_collision);
}

void Map::loadSpawnPoint()
{
	int pixel = 0;
	for (int h = 0; h < get_h()-1; h++) {
		for (int l = 0; l < get_w(); l++) {
			pixel = Graphics::getpixel(m_s, l, h);
			if (pixel == 0x00f) {
				Point point(l, h);
				m_spawn_point.push_back(point);
			}
		}
	}
}