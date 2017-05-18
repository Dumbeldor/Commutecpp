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
#include <SDL.h>
#include "Graphics.h"

void Graphics::openWindow()
{
	SDL_Window *sdlWindow;
	SDL_Renderer *sdlRenderer;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Initialization error:%s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);
	if (SDL_CreateWindowAndRenderer(m_w, m_h, SDL_WINDOW_RESIZABLE, &sdlWindow, &sdlRenderer)) {
		fprintf(stderr, "Couldn't create window and renderer: %s", SDL_GetError());
		exit(1);
	}
	SDL_RenderSetLogicalSize(sdlRenderer, m_w, m_h);
	m_r = sdlRenderer;
}

void Graphics::paint()
{
	SDL_RenderClear(m_r);
	SDL_SetRenderDrawColor(m_r, 0, 0, 0, 255);

	SDL_RenderPresent(m_r);
}
