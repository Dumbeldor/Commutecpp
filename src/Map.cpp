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
		delete Car::s_tile[i];
	}
	delete m_s;
	delete m_t;
}

Map::Map(Graphics *graphics, const std::string &map, int w, int h, Car *car, cars_t &cars) : m_graphics(graphics)
{
	m_s = SDL_LoadBMP(map.c_str());
	if (m_graphics) {
		m_t = SDL_CreateTextureFromSurface(m_graphics->get_renderer(), m_s);
	}

	m_rect.w = w;
	m_rect.h = h;
	m_rect.x = 0;
	m_rect.y = 0;

	if (car)
		m_car = car;

	if (!cars.empty())
		m_cars = cars;
}
