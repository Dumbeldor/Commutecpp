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
#include "Game.h"
#include "CollisionBox.h"

#define PI 3.14159265

const uint16_t Car::size = 64;

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

Car::Car(Map *map, bool drive, TypeCar type, Position pos, int speed, int sterring, float direction) : m_map(map), m_drive(drive), m_type(type), m_pos(pos), m_speed(speed),
																				 m_steering(sterring), m_direction(direction)
{
	load_collision();
}

Car::Car(Car *car)
{
	m_spawn = car->get_spawn();
	m_directions = car->get_directions();
	m_map = car->get_map();
	m_pos = car->get_pos();
	m_speed = car->get_speed();
	m_type = car->get_type();
	m_steering = car->get_steering();
	m_drive = car->get_drive();
	//m_collision_box = new CollisionBox(24, 72, 214, 93);
	m_box.w = 214;
	m_box.h = 93;
}

Car::~Car()
{
}

const SDL_Rect Car::get_rect() const
{
	SDL_Rect rect = {int(m_pos.x), int(m_pos.y), size, size};
	return rect;
}

void Car::load_collision()
{
	// A refaire
	//m_collision_box = new CollisionBox(24, 72, 214, 93);
	m_box.w = 214;
	m_box.h = 93;
}

bool Car::check_collision_with_car(Car *car)
{

	m_box.x = m_pos.x + 6;
	m_box.y = m_pos.y + 18;
	m_box.w = 27;
	m_box.h = 24;

	Box car_box = car->get_box();
	car_box.x = car->get_pos().x + 6;
	car_box.y = car->get_pos().y + 18;
	car_box.w = 27;
	car_box.h = 24;

	if (m_box.x < car_box.x + car_box.w &&
			m_box.x + m_box.w > car_box.x &&
			m_box.y < car_box.y + car_box.h &&
			m_box.y + m_box.h > car_box.y)
	{
		return true;
	}
	return false;

	float SIZEX=27;
	float SIZEY=24;
	float ca=cos(m_direction - car->get_direction());
	float sa=sin(m_direction-car->get_direction());
	float cx=m_pos.x-car->get_pos().x;
	float cy=m_pos.y-car->get_pos().y;
	float xx=cx*cos(-car->get_direction())-cy*sin(-car->get_direction());
	float yy=cx*sin(-car->get_direction())+cy*cos(-car->get_direction());
	float tx[]= {-SIZEX/2, SIZEX/2,-SIZEX/2,SIZEX/2};
	float ty[]={-SIZEY/2, SIZEY/2,SIZEY/2,-SIZEY/2};
	for (int i=0; i<4; i++) {
		float dx=xx+ca*tx[i]-sa*ty[i];
		float dy=yy+sa*tx[i]+ca*ty[i];
		if (dx>=-SIZEX/2&&dx<=SIZEX/2&&dy>=-SIZEY/2&&dy<=SIZEY/2)
			return true;
	}
	return false;
}

void Car::move()
{
	double val;
	val = PI / 180.0;
	if (!m_drive) {
		if (m_directions.size() > Game::get_time()-1)
			m_direction = m_directions[Game::get_time()];
		else
			m_direction = 0;
	}
	float dep_x = (m_speed + m_override_speed) * cos(val * m_direction);
	float dep_y = (m_speed + m_override_speed) * sin(val * m_direction);
	float x = m_pos.x + dep_x;
	float y = m_pos.y + dep_y;

	//m_collision_box->set_pos(static_cast<int>(x) + 24, static_cast<int>(y) + 72);
	m_box.x = static_cast<int>(x) + 24;
	m_box.y = static_cast<int>(y) + 24;

	// Check collision with other car
	const std::vector<Car *> &cars = m_map->get_cars();
	for (Car *car : cars) {
		Position pos = car->get_pos();

		if (car != this && check_collision_with_car(car)) {
			pos.x = pos.x + ((m_speed + m_override_speed) * cos(val * m_direction) * 2);
			pos.y = pos.y + ((m_speed + m_override_speed) * sin(val * m_direction) * 2);
			car->check_collision(pos.x, pos.y);
			x = m_pos.x;
			y = m_pos.y;
			car->set_pos(pos);
		}
	}

	check_collision(x, y);

	m_pos.x = x;
	m_pos.y = y;
}

bool Car::check_collision(float &x, float &y)
{
	m_override_speed = 0;
	TypeMap **types_maps;
	types_maps = m_map->get_types_maps();
	int case_x = static_cast<int>(x+size/2);
	int case_y = static_cast<int>(y+size/2);
	bool is_collision;


	if (x > m_map->get_w() - size / 2 || x < -size / 2) {
		x = m_pos.x;
		is_collision = true;
	}
	if (y > m_map->get_h() - size / 2 || y < -size / 2) {
		y = m_pos.y;
		is_collision = true;
	}
	if (case_x < 0 || case_y < 0 || case_x > m_map->get_w()-1 || case_y > m_map->get_h()-1) {}
	else if (types_maps[case_x][case_y] == BUILDING) {
		x = m_pos.x;
		y = m_pos.y;
		return true;
	}
	else if (types_maps[case_x][case_y] == GRASS){
		m_override_speed = -2;
	}
	else {

	}
	return is_collision;
}

void Car::save()
{
	m_directions.push_back(m_direction);
}

void Car::paint(SDL_Renderer *sdl_render)
{
	SDL_Rect rect = get_rect();
	SDL_RenderCopyEx(sdl_render, s_tile[m_type], NULL, &rect, m_direction, NULL, SDL_FLIP_NONE);

	if (m_drive) {
		SDL_SetRenderDrawColor(sdl_render, 255, 0, 0, 255);
	} else {
		SDL_SetRenderDrawColor(sdl_render, 0, 255, 0, 255);
	}
	SDL_RenderDrawRect(sdl_render, &rect);
}

void Car::spawn()
{
	srand(time(nullptr));
	std::vector<Point> spawn_point = m_map->get_spawn_point();
	Point point = spawn_point[rand() % spawn_point.size()];
	m_pos.x = point.x - size/2;
	m_pos.y = point.y - size/2;
	m_spawn = m_pos;
}

void Car::spawn_begin()
{
	m_pos = m_spawn;
}
