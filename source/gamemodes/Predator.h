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

#ifndef DUEL6_INVISIBLEMAN_H
#define DUEL6_INVISIBLEMAN_H

#include "../GameMode.h"

namespace Duel6
{

    /**
     * PLOT:
     * Each round one player is randomly picked as an invisible man. Only his gun is visible.
     * All other people has to hunt him down as a team.
     * Invisible man: - invisible
     *                - fast
     *                - has submachine gun
     *
     * Others: - shotguns
     */
    class Predator : public GameMode
    {

    private:
        int predatorIndex;
        std::unique_ptr<PlayerEventListener> eventListener;

    public:
        static std::string PREDATOR_TEAM()
        {
            return "Predator";
        }

        static std::string HUNTERS_TEAM()
        {
            return "Marines";
        }

    public:
        Predator()
		{}

        std::string getName()
        {
            return "Predator";
        }
        void initialize(World& world, Game& game);
        void preparePlayer(Player& player, Int32 playerIndex, std::vector<Player>& allPlayers);
        bool checkRoundOver(World& world, std::vector<Player*>& alivePlayers);
    };
}
#endif
