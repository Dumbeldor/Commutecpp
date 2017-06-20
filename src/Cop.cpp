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

#include <SDL_mixer.h>
#include <SDL_render.h>
#include "Cop.h"
#include "Game.h"

Mix_Chunk *Cop::s_siren = nullptr;

Cop::Cop(Map *map, bool drive, TypeCar type, Position pos, int speed, int sterring, float direction) :
		Car(map, drive, POLICE, pos, speed, sterring, direction)
{
}

Cop::~Cop()
{
	Mix_Pause(3);
}

void Cop::load_siren()
{
	s_siren = Mix_LoadWAV("/home/vincent/CLionProjects/commutecpp/data/sound/police_siren.mp3");
	if (s_siren) {
		Mix_PlayChannel(3, s_siren, -1);
		Mix_Volume(3, MIX_MAX_VOLUME/5);
		Mix_Pause(3);
	}
}

void Cop::move()
{
	if (Game::get_time() < 100) {

	}
	else {
		Car::move();
		if (Mix_Paused(3))
			Mix_Resume(3);
	}
}

void Cop::paint(SDL_Renderer *sdl_render)
{
	SDL_Rect rect = get_rect();
	SDL_RenderCopyEx(sdl_render, s_tile[m_type], NULL, &rect, m_direction, NULL, SDL_FLIP_NONE);
}

