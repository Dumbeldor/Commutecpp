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
#include "Car.h"
#include "Map.h"
#include "Graphics.h"

#define PI 3.14159265

const char *Car::s_tilenames[] = {
		"/home/vincent/CLionProjects/commutecpp/data/ambulance.bmp",
		"/home/vincent/CLionProjects/commutecpp/data/minitruck.bmp",
		"/home/vincent/CLionProjects/commutecpp/data/taxi.bmp",
		"/home/vincent/CLionProjects/commutecpp/data/sport.bmp",
		"/home/vincent/CLionProjects/commutecpp/data/minivan.bmp",
		"/home/vincent/CLionProjects/commutecpp/data/truck.bmp",
		"/home/vincent/CLionProjects/commutecpp/data/car.bmp",
		"/home/vincent/CLionProjects/commutecpp/data/police.bmp",
		"/home/vincent/CLionProjects/commutecpp/data/viper.bmp"
};
SDL_Texture *Car::s_tile[CAR_MAX] = {};

Car::Car(Map *map, TypeCar type, Position pos, int speed, int sterring, float direction) : m_map(map), m_type(type), m_pos(pos), m_speed(speed),
																				 m_steering(sterring), m_direction(direction)
{
}

Car::~Car()
{
}

const SDL_Rect Car::get_rect() const
{
	SDL_Rect rect = {int(m_pos.x), int(m_pos.y), 32, 32};
	return rect;
}

void Car::move()
{
	double val;
	val = PI / 180.0;
	float x = m_pos.x + m_speed * cos(val * m_direction);
	float y = m_pos.y + m_speed * sin(val * m_direction);

	int pixel = Graphics::getpixel(m_map->get_surface_collision(), x+16, y+16);

	if (pixel == 0xf00) {
		//std::cout << "ROUUUUUUUGE" << std::endl;
		//x = m_pos.x - (m_speed * cos(val * m_direction) * 2);
		//y = m_pos.y - (m_speed * sin(val * m_direction) * 2);
		x = m_pos.x;
		y = m_pos.y;
	}
	else {
		if (x > m_map->get_w() - 16 || x < -16)
			x = m_pos.x;
		if (y > m_map->get_h() - 16 || y < -16)
			y = m_pos.y;
	}

	m_pos.x = x;
	m_pos.y = y;
}

void Car::paint(SDL_Renderer *sdl_render)
{
	SDL_Rect rect = get_rect();
	SDL_RenderCopyEx(sdl_render, s_tile[m_type], NULL, &rect, m_direction, NULL, SDL_FLIP_NONE);

}

void Car::spawn()
{
	srand(time(nullptr));
	std::vector<Point> spawn_point = m_map->get_spawn_point();
	Point point = spawn_point[rand() % spawn_point.size()];
	m_pos.x = point.x;
	m_pos.y = point.y;
}
