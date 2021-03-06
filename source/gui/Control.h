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

#ifndef DUEL6_GUI_CONTROL_H
#define DUEL6_GUI_CONTROL_H

#include <SDL2/SDL_keycode.h>
#include "../Font.h"
#include "../Color.h"
#include "GuiContext.h"

namespace Duel6
{
	namespace Gui
	{
		class Desktop;

		class Control
		{
		public:
			enum class Type
			{
				Button,
				Label,
				Textbox,
				Listbox,
				Switchbox,
				Slider
			};

		protected:
			Int32 x, y;

		public:
			Control(Desktop& desk);
			virtual ~Control()
			{}

			virtual void check(const GuiContext& context) = 0;
			virtual void draw(const Font& font) const = 0;
			virtual Type getType() const = 0;

			virtual void keyEvent(SDL_Keycode keyCode)
			{}

			virtual void textInputEvent(const char* text)
			{}

			Int32 getX() const
			{
				return x;
			}

			Int32 getY() const
			{
				return y;
			}

		protected:
			void drawFrame(Int32 x, Int32 y, Int32 w, Int32 h, bool p) const;
		};
	}
}

#endif
