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

#include "Type.h"
#include "Shot.h"
#include "Player.h"
#include "Weapon.h"

namespace Duel6
{
	Shot::Shot(Player& player, SpriteList::Iterator sprite, Orientation shotOrientation)
		: player(player), weapon(player.getWeapon()), orientation(shotOrientation), sprite(sprite)
	{
		const Vector dim = getDimensions();
		const Rectangle playerRect = player.getCollisionRect();
		if (orientation == Orientation::Left)
		{
			position = Vector(playerRect.left.x - dim.x, player.getGunVerticalPosition() - dim.y / 2.0f);
			velocity = Vector(-1.0f, 0.0f);
		}
		else
		{
			position = Vector(playerRect.right.x, player.getGunVerticalPosition() - dim.y / 2.0f);
			velocity = Vector(1.0f, 0.0f);
		}
		this->sprite->setPosition(getSpritePosition(), 0.6f).setOrientation(this->orientation);
	}

	Shot& Shot::move(Float32 elapsedTime)
	{
		position += velocity * getWeapon().bulletSpeed * elapsedTime;
		sprite->setPosition(getSpritePosition());
		return *this;
	}

	Float32 Shot::getExplosionPower() const
	{
		Float32 coef = getPlayer().hasPowerfulShots() ? 2.0f : 1.0f;
		return coef * getWeapon().power;
	}

	Float32 Shot::getExplosionRange() const
	{
		Float32 coef = getPlayer().hasPowerfulShots() ? 2.0f : 1.0f;
		return coef * getWeapon().boom;
	}
}
