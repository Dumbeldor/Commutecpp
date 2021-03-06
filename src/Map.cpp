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
#include <SDL_mixer.h>
#include "Map.h"
#include "Graphics.h"
#include "Car.h"
#include "Cop.h"

//Mix_Chunk *Map::s_train = nullptr;

Map::~Map()
{
	delete m_car;
	for (int i = 0; i < CAR_MAX; i++) {
		SDL_free(Car::s_tile[i]);
	}
	SDL_FreeSurface(m_s);
	SDL_free(m_t);
}

void Map::remove_cars()
{
	/*
	for (Car *car : m_cars) {
		if (car && car != m_car)
			delete car;
	}
	 */
	m_cars.clear();
	m_cars = {};
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

	m_s_batman = SDL_LoadBMP("/home/vincent/CLionProjects/commutecpp/data/batman.bmp");
	if (!m_s_batman) {
		std::cerr << "Image batman doesnt exist !" << std::endl;
		return;
	}

	m_rect.w = w;
	m_rect.h = h;
	m_rect.x = 0;
	m_rect.y = 0;
}

void Map::loadTiles()
{
	m_batman = SDL_CreateTextureFromSurface(m_graphics->get_renderer(), m_s_batman);
	if (!m_batman) {
		std::cerr << "erreur batman" << std::endl;
		return;
	}
	SDL_FreeSurface(m_s_batman);
}

void Map::loadCollision()
{
	int pixel = 0;
	m_types_maps = new TypeMap*[1280];
	for (int l = 0; l < get_w(); l++) {
		m_types_maps[l] = new TypeMap[800];
		for (int h = 0; h < get_h(); h++) {
			pixel = Graphics::getpixel(m_s_collision, l, h);
			if (pixel == 0xf00) {
				TypeMap type(BUILDING);
				m_types_maps[l][h] = type;
			}
			else if (pixel == 0x0f0) {
				TypeMap type(GRASS);
				m_types_maps[l][h] = type;
			}
			else if (pixel == 0x00f) {
				TypeMap type(TRAIN);
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

void Map::loadBonus()
{
	m_bonus.clear();
	int bonus;
	for (int h = 0; h < get_h()-1; h++) {
		for (int l = 0; l < get_w()-1; l++) {
			if (m_types_maps[l][h] == ROAD) {
				bonus = rand() % 100000;
				if (bonus > 99998) {
					int t = rand() % 3;
					TypeBonus type((TypeBonus) t);
					SDL_Rect rect;
					rect.x = l-20;
					rect.y = l-20;
					rect.h = 40;
					rect.w = 40;
					Bonus b(type, rect, 1);
					m_bonus.push_back(b);
				}
			}
		}
	}
}

void Map::loadEndPoint()
{
	Point point;
	do {
		point = m_spawn_point[rand() % m_spawn_point.size()];
		m_end_point.x = point.x;
		m_end_point.y = point.y;
	} while (m_car->get_spawn().x == m_end_point.x && m_car->get_spawn().y == m_end_point.y);

	m_end_point.x -= 25;
	m_end_point.y -= 25;
}

void Map::paint(SDL_Renderer *sdl_render)
{
	// Map
	SDL_RenderCopy(sdl_render, m_t, NULL, &m_rect);

	// End Point
	SDL_SetRenderDrawColor(sdl_render, 0, 255, 255, 255);
	SDL_RenderFillRect(sdl_render, &m_end_point);
	SDL_RenderDrawRect(sdl_render, &m_end_point);

	// Bonus
	SDL_SetRenderDrawColor(sdl_render, 255, 0, 0, 255);
	for (const Bonus &b : m_bonus) {
		SDL_RenderCopyEx(sdl_render, m_batman, NULL, &b.rect, 0, NULL, SDL_FLIP_NONE);
		//SDL_RenderDrawRect(sdl_render, &b.rect);
	}
}

void Map::loadSound()
{
	//s_train = Mix_LoadWAV("/home/vincent/CLionProjects/commutecpp/data/sound/punch.mp3");
}