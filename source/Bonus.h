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

#ifndef DUEL6_BONUS_H
#define DUEL6_BONUS_H

#include <SDL2/SDL_opengl.h>
#include "Type.h"
#include "TextureManager.h"
#include "Vector.h"
#include "Rectangle.h"

namespace Duel6
{
	struct Weapon; // Forward declaration, TODO: Remove

	/*
	class BonusType
	{
	public:
		virtual void apply(Player& player) const = 0;
		virtual void cancel(Player& player) const = 0;
		virtual void render() const = 0;
	};
	*/

	enum BonusType
	{
		D6_BONUS_LIFEP = 0,
		D6_BONUS_LIFEM,
		D6_BONUS_LIFEF,
		D6_BONUS_SHOTS,
		D6_BONUS_SHOTP,
		D6_BONUS_INVUL,
		D6_BONUS_BULLT,
		D6_BONUS_SPEED,
		D6_BONUS_INVIS,
		D6_BONUS_SPLITFIRE,
		D6_BONUS_VAMPIRESHOTS,
		D6_BONUS_GUESS // GUESS must be the last type
	};

	// TODO: Split into Bonus and LyingWeapon
	class Bonus
	{
	private:
		Vector position;
		Size type;
		bool weapon;
		const Weapon* weaponType;
		Int32 bullets; // Number of bullets for weapon bonuses
		GLuint texture;

	public:
		Bonus(const Vector& position, Size type, GLuint texture);

		Bonus(const Vector& position, const Weapon& weaponType, Int32 bullets);

		Size getType() const
		{
			return type;
		}

		bool isWeapon() const
		{
			return weapon;
		}

		const Weapon& getWeaponType() const
		{
			return *weaponType;
		}

		Int32 getBullets() const
		{
			return bullets;
		}

		const Vector& getPosition() const
		{
			return position;
		}

		Vector getDimensions() const
		{
			return isWeapon() ? Vector(1.0f, 1.0f) : Vector(0.6f, 0.6f);
		}

		Rectangle getCollisionRect() const
		{
			return Rectangle::fromCornerAndSize(getPosition(), getDimensions());
		}

		Vector getSpritePosition() const
		{
			return isWeapon() ? position : position - Vector(0.2f, 0.2f);
		}

		void render() const;
	};
}

#endif