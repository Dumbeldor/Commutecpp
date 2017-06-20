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

#pragma once

#include <iostream>
#include <vector>
#include <SDL_rect.h>
#include <unordered_map>
#include "Graphics.h"

class SDL_Texture;
class SDL_Surface;
class Car;
class Graphics;
class Cop;

typedef std::vector<Car *> cars_t;

struct Point {
	int x;
	int y;
	Point(int x = 0, int y = 0) : x(x), y(y) {};
	bool operator==(const Point &point) const {
		return (x == point.x && y == point.y);
	}
};

enum TypeMap : uint8_t {
	ROAD=0,
	GRASS,
	BUILDING,
};


class Map {
public:
	Map(Graphics *graphics, const std::string &map, int w, int h);
	~Map();
	void loadCollision();
	void loadSpawnPoint();
	void loadEndPoint();
	void paint(SDL_Renderer *sdl_render);

	const int get_w() const { return m_rect.w; };
	const int get_h() const { return m_rect.h; };

	SDL_Texture *get_texture() const { return m_t; };
	void set_texture(SDL_Texture *t) { m_t = t; };

	Car *get_car() const { return m_car; }
	void set_car(Car *car) { m_car = car; };

	std::vector<Car *> &get_cars() { return m_cars; };
	void set_cars(std::vector<Car *> &cars) { m_cars = cars; };
	void remove_cars();

	SDL_Rect *get_rect() { return &m_rect; };
	SDL_Surface *get_surface() const { return m_s; };

	const std::vector<Point> &get_spawn_point() const { return m_spawn_point; };

	TypeMap **get_types_maps() const { return m_types_maps;}

	const SDL_Rect &get_end_point() const { return m_end_point; };

	void set_cop(Cop *cop) { m_cop = cop; };
	Cop *get_cop() const { return m_cop; };

private:
	SDL_Surface *m_s = nullptr;
	SDL_Surface *m_s_collision = nullptr;
	SDL_Texture *m_t = nullptr;
	SDL_Rect m_rect;
	Car *m_car = nullptr;
	Cop *m_cop = nullptr;
	std::vector<Car *> m_cars = {};
	std::vector<Point> m_spawn_point = {};
	Graphics *m_graphics = nullptr;
	TypeMap **m_types_maps = nullptr;
	//Point m_end_point;
	SDL_Rect m_end_point = {0, 0, 50, 50};
};
