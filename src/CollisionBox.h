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

#include "Map.h"

struct Box
{
	int x;
	int y;
	int w;
	int h;
	Box(int x = 0, int y = 0, int w = 0, int h = 0) : x(x), y(y), w(w), h(h){};
	bool operator==(const Box &box) const {
		return (x == box.x && y == box.y && w == box.w && h == box.h);
	}
	bool operator<(const Box &box) const {
		return (x < box.x + box.w && y < box.y + box.h);
	}
	bool operator>(const Box &box) const {
		return (x + w > box.x && y + h > box.y);
	}
	void operator=(const Box &box) {
		x = box.x;
		y = box.y;
	}
};


class CollisionBox {
public:
	CollisionBox(int x, int y, int w, int h) : m_box(x, y, w, h) {};
	~CollisionBox() {};
	bool is_collision(const CollisionBox *collision) const;
	void set_pos(const int x, const int y) { m_box.x = x; m_box.y = y;}
	const Box &get_box() const { return m_box; };

private:
	Box m_box;
};
