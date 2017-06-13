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

#include <cstdint>
#include <SDL_rect.h>
#include <vector>
#include "CollisionBox.h"

#define SIZE_CAR 256

class SDL_Texture;
class Map;
class SDL_Renderer;
//class CollisionBox;
class Box;

enum TypeCar : int8_t {
	AMBULANCE=0,
	MINITRUCK,
	TAXI,
	SPORT,
	MINIVAN,
	TRUCK,
	CAR,
	POLICE,
	VIPER,
	CAR_MAX
};

struct Position {
	float x;
	float y;
	Position(int x, int y) : x(x), y(y) {};
};

class Car {
public:
	Car(Map *map, bool drive = false, TypeCar type = VIPER, Position pos = Position(0, 0), int speed = 4, int sterring = 3, float direction = 0.0f);
	Car(Car *car);
	~Car();
	void paint(SDL_Renderer *sdl_render);
	void move();
	void spawn();
	void spawn_begin();
	void load_collision();
	void save();

	const TypeCar &get_type() const { return m_type; };

	const SDL_Rect get_rect() const;

	void set_direction(float direction) { m_direction = direction;}
	const float get_direction() const { return m_direction; };

	const int get_speed() const { return m_speed; };
	void set_speed(int speed) { m_speed = speed; };

	const int get_override_speed() const { return m_speed; };
	void set_override_speed(int speed) { m_override_speed = speed; };

	const int get_steering() const { return m_steering; }
	void set_steering(int steering) { m_steering = steering; }

	Position get_pos() const { return m_pos; }
	void set_pos(Position &pos) { m_pos = pos; }

	static const char *s_tilenames[];
	static SDL_Texture *s_tile[CAR_MAX];

	void set_map(Map *map) { m_map = map; }

	void set_drive(bool drive) { m_drive = drive; }
	const bool get_drive() const { return m_drive; }

	Position get_spawn() const { return m_spawn; }

	std::vector<float> get_directions() const { return m_directions; }
	void set_directions(std::vector<float> directions = {}) { m_directions = directions;}

	Map *get_map() const { return m_map; }

	//CollisionBox *get_collision_box() const { return m_collision_box; };
	Box get_box() const { return m_box; };

private:
	bool m_drive = false;
	TypeCar m_type = VIPER;
	int m_speed = 2, m_steering = 2;
	int m_override_speed = 0;
	float m_direction = 0.0f;
	Position m_pos = Position(0, 0);
	Map *m_map = nullptr;
	Position m_spawn = Position(0, 0);
	static const uint16_t size;
	std::vector<float> m_directions = {};
	//CollisionBox m_collision_box = nullptr;
	Box m_box;
};
