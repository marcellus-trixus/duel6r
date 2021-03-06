/*
* Copyright (c) 2006, Ondrej Danek (www.ondrej-danek.net)
* All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Ondrej Danek nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_keycode.h>
#include "TextBox.h"

namespace Duel6
{
	namespace Gui
	{
		Textbox::Textbox(Desktop& desk)
				: Control(desk)
		{
		}

		Textbox::~Textbox()
		{
		}

		void Textbox::setPosition(int X, int Y, int W, int M, const std::string& allowed)
		{
			x = X + 2;
			y = Y - 2;
			max = M;
			width = W;

			allowedCharacters = allowed;
			text.clear();
		}

		void Textbox::keyEvent(SDL_Keycode keyCode)
		{
			if (!text.empty() && keyCode == SDLK_BACKSPACE)
			{
				text.pop_back();
			}
		}

		void Textbox::textInputEvent(const char* newText)
		{
			while (*newText != 0)
			{
				if ((int)text.length() < max && allowedCharacters.find(*newText) != std::string::npos)
				{
					text.push_back(*newText);
				}

				++newText;
			}
		}

		const std::string& Textbox::getText() const
		{
			return text;
		}

		void Textbox::flush()
		{
			text.clear();
		}

		void Textbox::check(const GuiContext& context)
		{
			const MouseState& ms = context.getMouseState();

			if (ms.isPressed() && ms.isInside(x, y, (width + 1) << 3, 18))
			{
				// EventType::Active;
			}
		}

		void Textbox::draw(const Font& font) const
		{
			int     w = (width << 3) + 8;

			drawFrame(x - 2, y + 2, w + 4, 22, true);
			glBegin(GL_QUADS);
			glColor3ub(255, 255, 255);
			glVertex2i(x, y);
			glVertex2i(x + w, y);
			glVertex2i(x + w, y - 17);
			glVertex2i(x, y - 17);
			glEnd();

			font.print(x, y - 16, Color(0), text + "_");
		}
	}
}
