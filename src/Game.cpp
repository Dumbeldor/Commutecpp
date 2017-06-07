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

#include "Game.h"
#include "Graphics.h"
#include "Event.h"
#include "Car.h"
#include "Map.h"
#include "Hud.h"
#include <SDL.h>
#include <iostream>

uint32_t Game::s_time = 0;
//uint32_t Game::s_time_max = 500;

Game::~Game()
{
	delete m_graphics;
}

void Game::start()
{
	float ntime = SDL_GetTicks();

	m_graphics = new Graphics();
	Map *map = new Map(m_graphics, "/home/vincent/CLionProjects/commutecpp/data/circuit", 1280, 800);

	Car *car = new Car(map, true);
	cars_t cars = {};
	map->set_car(car);
	map->set_cars(cars);

	m_graphics->openWindow(map);
	m_graphics->loadTiles();

	map->loadSpawnPoint();
	std::cout << "Chargement des collision" << std::endl;
	map->loadCollision();
	std::cout << "Fin chargement collision" << std::endl;
	car->spawn();

	Event *event = new Event(this, car);

	while (m_start) {
		if (s_time == s_time_max) {
			car->set_drive(false);
			car->spawn_begin();

			map->get_cars().push_back(new Car(car));
			car->set_drive(true);
			car->set_directions();
			car->spawn();
			s_time = 0;

			for (Car *c: map->get_cars()) {
				c->spawn_begin();
			}
		}

		if (car->get_drive()) {
			event->getEvent();
			car->move();
			car->save();
		}
		else {
			car->move();
		}

		const std::vector<Car *> &cars = map->get_cars();
		for (Car *c: cars) {
			if (c)
				c->move();
		}

		m_graphics->paint();

		m_graphics->render();

		s_time++;
		std::cout << s_time << std::endl;
		SDL_Delay(( 1000 / 30));
	}

	delete map;
	delete event;

	return;
}

void Game::update()
{

}

void Game::stop()
{
	m_start = false;
}

void Game::getEvent()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			m_start = false;
			return;
		}

		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				m_start = false;
				return;
			}
		}
	}
}
