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

#include <stdlib.h>
#include "Sound.h"
#include "BonusList.h"
#include "Weapon.h"
#include "World.h"
#include "InfoMessageQueue.h"
#include "collision/Collision.h"

namespace Duel6
{
	namespace
	{
		std::list<Bonus> d6Bonuses;
		TextureManager::Texture textures;
	}
	
	void BONUS_Init(TextureManager& textureManager)
	{
		textures = textureManager.load(D6_TEXTURE_BONUS_PATH, GL_LINEAR, true);
	}

	GLuint BONUS_GetTexture(Size type)
	{
		return textures.getGlTextures()[type];
	}

	void BONUS_Clear()
	{
		d6Bonuses.clear();
	}

	void BONUS_DrawAll()
	{
		glEnable(GL_ALPHA_TEST);

		for (const Bonus& bonus : d6Bonuses)
		{
			bonus.render();
		}

		glDisable(GL_ALPHA_TEST);
	}

	void BONUS_AddNew(const Level& level)
	{
		bool weapon = ((rand() % 2) == 1);
		Int32 x = rand() % level.getWidth();
		Int32 y = rand() % level.getHeight();

		if (!level.isWall(x, y, true))
		{
			if (weapon)
			{
				if (!level.isWall(x, y - 1, true)) // Weapons must be on the ground
				{
					return;
				}
				d6Bonuses.push_back(Bonus(Vector(x, y), WPN_GetRandomWeapon(), rand() % 10 + 10));
			}
			else
			{
				Size type = rand() % D6_BONUS_COUNT;
				d6Bonuses.push_back(Bonus(Vector(x + 0.2f, y + 0.2f), type, textures.getGlTextures()[type]));
			}
		}
	}

	void BONUS_AddDeadManGun(const Vector& position, Player& player)
	{
		d6Bonuses.push_back(Bonus(position, player.getWeapon(), player.getAmmo()));
	}

	static bool BONUS_IsApplicable(const Bonus& bonus, const Player& player, bool weapon)
	{
		if (weapon != bonus.isWeapon())
		{
			return false;
		}

		if (!weapon && (bonus.getType() == D6_BONUS_LIFEM && player.isInvulnerable()))
		{
			return false;
		}

		return Collision::rectangles(bonus.getCollisionRect(), player.getCollisionRect());
	}

	static void BONUS_Apply(const Bonus& bonus, Player& player, InfoMessageQueue& messageQueue)
	{
		Int32 duration = 13 + rand() % 17;
		Int32 hit = (Int32(D6_MAX_LIFE) / 7) + rand() % (Int32(D6_MAX_LIFE) / 2);
		Size type = (bonus.getType() == D6_BONUS_GUESS) ? rand() % (D6_BONUS_COUNT - 1) : bonus.getType();

		switch (type)
		{
		case D6_BONUS_INVIS:
			player.setBonus(type, duration).setAlpha(0.2f);
			messageQueue.add(player, Format(D6_L("Invisibility for {0} seconds")) << duration);
			break;

		case D6_BONUS_SPEED:
			player.setBonus(type, duration).setAlpha(1.0f);
			messageQueue.add(player, Format(D6_L("Fast movement for {0} seconds")) << duration);
			break;

		case D6_BONUS_LIFEP:
			player.addLife(Float32(hit));
			messageQueue.add(player, Format(D6_L("Life +{0}")) << hit);
			break;

		case D6_BONUS_LIFEM:
			if (player.hit(Float32(hit)))
			{
				player.playSound(PlayerSounds::Type::WasKilled);
			}
			messageQueue.add(player, Format(D6_L("Life -{0}")) << hit);
			break;

		case D6_BONUS_LIFEF:
			player.setFullLife();
			messageQueue.add(player, D6_L("Full life"));
			break;

		case D6_BONUS_SHOTS:
			player.setBonus(type, duration).setAlpha(1.0f);
			messageQueue.add(player, Format(D6_L("Fast reload for {0} seconds")) << duration);
			break;

		case D6_BONUS_SHOTP:
			player.setBonus(type, duration).setAlpha(1.0f);
			messageQueue.add(player, Format(D6_L("Powerful shots for {0} seconds")) << duration);
			break;

		case D6_BONUS_INVUL:
			player.setBonus(type, duration).setAlpha(1.0f);
			messageQueue.add(player, Format(D6_L("Invulnerability for {0} seconds")) << duration);
			break;

		case D6_BONUS_BULLT:
			hit = 5 + rand() % 12;
			player.pickAmmo(hit);
			messageQueue.add(player, Format(D6_L("Bullets +{0}")) << hit);
			break;

		case D6_BONUS_SPLITFIRE:
			player.setBonus(type, duration).setAlpha(1.0f);
			messageQueue.add(player, Format(D6_L("Splitfire for {0} seconds")) << duration);
			break;

		case D6_BONUS_VAMPIRESHOTS:
			player.setBonus(type, duration).setAlpha(1.0f);
			messageQueue.add(player, Format(D6_L("Vampire shots for {0} seconds")) << duration);
			break;
		}
	}

	void BONUS_Check(Player& player, InfoMessageQueue& messageQueue)
	{
		auto bonusIter = d6Bonuses.begin();
		while (bonusIter != d6Bonuses.end())
		{
			if (BONUS_IsApplicable(*bonusIter, player, false))
			{
				BONUS_Apply(*bonusIter, player, messageQueue);
				player.playSound(PlayerSounds::Type::PickedBonus);
				bonusIter = d6Bonuses.erase(bonusIter);
			}
			else
			{
				++bonusIter;
			}
		}
	}

	static void BONUS_PickWeapon(const Bonus& bonus, Player& player, InfoMessageQueue& messageQueue)
	{
		if (player.getAmmo() > 0)
		{
			// Leave the current weapon at the same place
			d6Bonuses.push_back(Bonus(bonus.getPosition(), player.getWeapon(), player.getAmmo()));
		}

		player.pickWeapon(bonus.getWeaponType(), bonus.getBullets());
		messageQueue.add(player, Format(D6_L("You picked up gun {0}")) << D6_L(bonus.getWeaponType().name));
	}

	void BONUS_CheckPick(Player& player, InfoMessageQueue& messageQueue)
	{
		auto bonusIter = d6Bonuses.begin();
		while (bonusIter != d6Bonuses.end())
		{
			if (BONUS_IsApplicable(*bonusIter, player, true))
			{
				BONUS_PickWeapon(*bonusIter, player, messageQueue);
				d6Bonuses.erase(bonusIter);
				return;
			}
			else
			{
				++bonusIter;
			}
		}
	}
}