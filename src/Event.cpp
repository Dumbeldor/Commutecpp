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

#include <SDL_events.h>
#include <SDL_mixer.h>
#include "Event.h"
#include "Game.h"
#include "Car.h"

Event::Event(Game *game, Car *car) : m_game(game), m_car(car), m_horn(Mix_LoadWAV("/home/vincent/CLionProjects/commutecpp/data/sound/horn.mp3"))
{
}

Event::~Event()
{
	Mix_FreeChunk(m_horn);
}

void Event::getEvent()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			m_game->stop();
			return;
		}

		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					m_game->stop();
					return;
				case SDLK_LEFT:
					m_car->set_direction(m_car->get_direction() - m_car->get_steering());
					break;
				case SDLK_RIGHT:
					m_car->set_direction(m_car->get_direction() + m_car->get_steering());
					break;
				case SDLK_k:
					Mix_PlayChannel(1, m_horn, 0);
					break;
				case SDLK_p:
					Game::pause();
					break;
				case SDLK_r:
					Game::win(false);
					Game::loose(false);
					Game::set_arrest(false);
					break;

				case SDLK_UP:
					Game::set_volume(Game::get_volume() + 1);
					Mix_VolumeMusic(Game::get_volume());
					break;
				case SDLK_DOWN:
					Game::set_volume(Game::get_volume() - 1);
					Mix_VolumeMusic(Game::get_volume());
					break;

				case SDLK_m:
					std::cout << "PAUSED" << std::endl;
					if (Mix_PausedMusic())
						Mix_ResumeMusic();
					else
						Mix_PauseMusic();
					break;

				default:;
			}
		}
	}
}
