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

class Graphics;
class Map;
class Event;
class Car;


class Game {
public:
	Game(){};
	~Game();
	void start();
	void stop();
	static void pause() { s_pause = !s_pause;}
	static bool is_pause() { return s_pause; }
	static bool win(bool win = true) { s_win = win; }
	static bool is_win() { return s_win; }
	static void loose(bool loose = true) { s_loose = loose;}
	static bool is_loose() { return s_loose;}

	static const uint32_t get_time() { return s_time; };
	static const uint32_t get_time_max() { return s_time_max; }
private:
	void getEvent();
	void update();
	void add_car_to_cars();
	void respawn();

	Map *m_map = nullptr;
	Car *m_car = nullptr;
	Event *m_event = nullptr;
	Graphics *m_graphics = nullptr;
	bool m_start = true;
	static bool s_pause;
	static bool s_win;
	static bool s_loose;
	static uint32_t s_time;
	static const uint32_t s_time_max = 500;
};
