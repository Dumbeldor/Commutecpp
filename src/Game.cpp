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
#include "Cop.h"
#include <SDL.h>
#include <iostream>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

uint32_t Game::s_time = 0;
bool Game::s_pause = false;
bool Game::s_win = false;
bool Game::s_loose = false;
bool Game::s_arrest = false;
uint32_t Game::s_volume = MIX_MAX_VOLUME / 4;
//uint32_t Game::s_time_max = 500;

Game::~Game()
{
	delete m_graphics;
	delete m_car;
	delete m_map;
	delete m_event;
}

void Game::start()
{
	TTF_Init();
	float ntime = SDL_GetTicks();

	m_graphics = new Graphics();
	m_map = new Map(m_graphics, "/home/vincent/CLionProjects/commutecpp/data/circuit", 1280, 800);

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		std::cout << "Error to charge mix : " << Mix_GetError();
	}

	Mix_AllocateChannels(5);

	Mix_Music *music = nullptr;
	music = Mix_LoadMUS("/home/vincent/CLionProjects/commutecpp/data/sound/musique.mp3");

	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(s_volume);

	m_car = new Car(m_map, true);
	Car::load_punch();
	cars_t cars = {};
	m_map->set_car(m_car);
	m_map->set_cars(cars);

	m_graphics->openWindow(m_map);
	m_graphics->loadTiles();

	m_map->loadSpawnPoint();
	std::cout << "Chargement des collision" << std::endl;
	m_map->loadCollision();
	std::cout << "TEST : " << m_map->get_types_maps()[10][10] << std::endl;
	std::cout << "Fin chargement collision" << std::endl;
	m_car->spawn();
	m_map->loadEndPoint();
	Cop *cop = nullptr;

	m_event = new Event(this, m_car);

	while (m_start) {
		if (s_loose || s_arrest) {
			while (s_loose || s_arrest) {
				m_event->getEvent();
				SDL_Delay((1000 / 30));
			}
			s_time = 0;
			std::vector<Car *> &cars = m_map->get_cars();
			for (const auto &car : cars) {
				delete car;
			}
			m_map->remove_cars();
			if (cop)
				delete cop;
			respawn();
		}

		if (s_time == s_time_max) {
			s_loose = true;
		}

		if (s_win) {
			while (s_win) {
				m_event->getEvent();
				SDL_Delay((1000 / 30));
			}
			s_time = 0;
			add_car_to_cars();
			respawn();
			if (cop)
				delete cop;
			cop = new Cop(m_car);
			cop->set_type(POLICE);
			cop->set_drive(false);
			m_map->set_cop(cop);
		}

		if (m_car->get_drive()) {
			m_event->getEvent();
			m_car->move();
			m_car->save();
		}
		else {
			m_car->move();
		}

		const std::vector<Car *> &cars = m_map->get_cars();
		for (Car *c: cars) {
			if (c)
				c->move();
		}

		if (cop) {
			cop->set_directions_(m_car->get_directions());
			cop->move();
		}

		m_graphics->paint();

		m_graphics->render();

		s_time++;
		std::cout << s_time << std::endl;
		while (s_pause) {
			m_event->getEvent();
			SDL_Delay((1000 / 30));
		}
		SDL_Delay(( 1000 / 30));
	}
	delete cop;

	Mix_FreeMusic(music);
	Mix_FreeChunk(Car::get_punch());
	Mix_CloseAudio();
	return;
}

void Game::update()
{

}

void Game::stop()
{
	m_start = false;
}

void Game::add_car_to_cars()
{
	m_car->set_drive(false);
	m_car->spawn_begin();

	m_map->get_cars().push_back(new Car(m_car));
	m_car->set_direction(0.0f);

	for (Car *c: m_map->get_cars()) {
		c->spawn_begin();
	}
}

void Game::respawn()
{
	m_car->set_drive(true);
	m_car->set_directions();
	m_car->spawn();
	m_map->loadEndPoint();
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
