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

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "Desktop.h"

namespace Duel6
{
	namespace Gui
	{
		namespace
		{
			Color bcgColor(192, 192, 192);
		}

		Desktop::Desktop()
		{
		}

		Desktop::~Desktop()
		{
		}

		void Desktop::addControl(Control* control)
		{
			controls.push_back(std::unique_ptr<Control>(control));
		}

		void Desktop::screenSize(Int32 scrWidth, Int32 scrHeight, Int32 trX, Int32 trY)
		{
			screenWidth = scrWidth;
			screenHeight = scrHeight;
			this->trX = trX;
			this->trY = trY;
		}

		void Desktop::update()
		{
			Int32 mx, my, mb;
			mb = SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT);
			mx -= trX;
			my = screenHeight - my - trY;
			GuiContext context(MouseState(mx, my, mb != 0));

			for (auto& control : controls)
			{
				control->check(context);
			}
		}

		void Desktop::draw(const Font& font) const
		{
			glBegin(GL_QUADS);
			glColor3ub(bcgColor.getRed(), bcgColor.getGreen(), bcgColor.getBlue());
			glVertex2i(0, 0);
			glVertex2i(screenWidth, 0);
			glVertex2i(screenWidth, screenHeight);
			glVertex2i(0, screenHeight);
			glEnd();

			glPushMatrix();
			glTranslatef((GLfloat)trX, (GLfloat)trY, 0);

			for (auto& control : controls)
			{
				control->draw(font);
			}

			glPopMatrix();
		}

		void Desktop::keyEvent(SDL_Keycode keyCode)
		{
			for (auto& control : controls)
			{
				control->keyEvent(keyCode);
			}
		}

		void Desktop::textInputEvent(const char* text)
		{
			for (auto& control : controls)
			{
				control->textInputEvent(text);
			}
		}
	}
}