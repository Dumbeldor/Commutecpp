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
#include <fstream>
#include <sstream>

Map::~Map()
{
	delete m_car;
	for (const auto &car : m_cars) {
		delete car;
	}
	for (int i = 0; i < CAR_MAX; i++) {
		SDL_free(Car::s_tile[i]);
	}
	SDL_FreeSurface(m_tileset);
	SDL_free(m_texture);
	//delete m_spawn_point;
}

Map::Map(Graphics *graphics, const std::string &map, const std::string &filename, int w, int h) : m_graphics(graphics)
{
	std::string m = map + ".bmp";
	m_tileset = SDL_LoadBMP(m.c_str());
	if (!m_tileset) {
		std::cerr << "The tileset " << m << " doesnt exist !" << std::endl;
		return;
	}
	if (m_graphics) {
		m_texture = SDL_CreateTextureFromSurface(m_graphics->get_renderer(), m_tileset);
	}

	// Charger fichier map
	std::fstream fin;
	int x = 0;
	int y = 0;
	std::vector<std::vector<int>> lignes;
	std::vector<int> vectData;
	std::string strBuf, strTmp;
	std::stringstream iostr;

	fin.open(filename, std::fstream::in);
	if (!fin.is_open()) {
		std::cerr << "Error to load : " << filename << " !" << std::endl;
		exit(1);
	}
	while(!fin.eof()) {
		std::getline(fin, strBuf);
		if (!strBuf.size())
			continue;

		iostr.clear();
		iostr.str(strBuf);
		vectData.clear();

		while(true) {
			getline(iostr, strTmp, ' ');
			vectData.push_back(atoi(strTmp.c_str()));
			if (!iostr.good())
				break;
		}

		if (vectData.size())
			lignes.push_back(vectData);
	}

	fin.close();

	for (y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			tile[y][x] = lignes[y][x];
		}
	}

	m_rect.w = w;
	m_rect.h = h;
	m_rect.x = 0;
	m_rect.y = 0;
}

void Map::paint(SDL_Renderer *sdl_render)
{
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			int a = tile[y][x];

			SDL_Rect rect;
			rect.h = 64;
			rect.w = 64;
			rect.y = a / 9 * TILE_SIZE;
			rect.x = a % 9 * TILE_SIZE;

			SDL_Rect rect_dest;
			rect_dest.y = y * TILE_SIZE;
			rect_dest.x = x * TILE_SIZE;
			rect_dest.h = TILE_SIZE;
			rect_dest.w = TILE_SIZE;

			SDL_RenderCopy(sdl_render, m_texture, &rect, &rect_dest);
		}
	}
}

void Map::loadSpawnPoint()
{
	/*
	int pixel = 0;
	for (int h = 0; h < get_h()-1; h++) {
		for (int l = 0; l < get_w(); l++) {
			pixel = Graphics::getpixel(m_tileset, l, h);
			if (pixel == 0x00f) {
				Point point(l, h);
				m_spawn_point.push_back(point);
			}
		}
	}
	 */
}