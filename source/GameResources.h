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

#ifndef DUEL6_GAMERESOURCES_H
#define DUEL6_GAMERESOURCES_H

#include <unordered_map>
#include "Water.h"
#include "Block.h"
#include "AppService.h"

namespace Duel6
{
	class GameResources
	{
	private:
		Water::WaterSet waterSet;
		Block::Meta blockMeta;
		Sound::Sample gameOverSound;
		Sound::Sample roundStartSound;
		TextureManager::Texture blockTextures;
		TextureManager::Texture bcgTextures;
		TextureManager::Texture explosionTextures;
		std::unordered_map<Size, TextureManager::Texture> fireTextures;

	public:
		GameResources(AppService& appService);

		const Water::WaterSet& getWaterSet() const
		{
			return waterSet;
		}

		const Block::Meta& getBlockMeta() const
		{
			return blockMeta;
		}

		const Sound::Sample& getGameOverSound() const
		{
			return gameOverSound;
		}

		const Sound::Sample& getRoundStartSound() const
		{
			return roundStartSound;
		}

		const TextureManager::Texture& getBlockTextures() const
		{
			return blockTextures;
		}

		const TextureManager::Texture& getBcgTextures() const
		{
			return bcgTextures;
		}

		const TextureManager::Texture& getExplosionTextures() const
		{
			return explosionTextures;
		}

		const std::unordered_map<Size, TextureManager::Texture>& getFireTextures() const
		{
			return fireTextures;
		}
	};
}

#endif
