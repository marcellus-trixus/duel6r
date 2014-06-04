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

/*
Projekt: Konzole
Popis: Hlavni funkce
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "console.h"

///////////////////////////////////////////////////////
//                Zakladni funkce                    //
///////////////////////////////////////////////////////

/*
==================================================
Inicializace/deinicializace konzole
==================================================
*/
Console::Console(int flags)
{
    visible = false;
    insert = false;
    curpos = 0;
    inputscroll = 0;
    this->flags = flags;

    histcnt = 0;
    histscroll = 0;
    font = nullptr;

    width = CON_DEF_WIDTH;
    show = CON_DEF_SHOWLAST;

	text.resize(CON_TEXT_SIZE);
    clear();

    print("==========================\n");
    print(CON_Lang("Console -- author O.Danek\n"));
	print(CON_Format(CON_Lang("Version {0}\n")) << CON_VERSION);
    print("==========================\n");
}

Console::~Console()
{
	vars.clear();
	cmds.clear();
	aliases.clear();
}

/*
==================================================
Vyprazdneni textoveho bufferu
==================================================
*/
void Console::clear()
{
    bufpos = 0;
    scroll = 0;
    buffull = false;
    memset(&text[0], '\n', CON_TEXT_SIZE);
}

/*
==================================================
Formatovany tisk do konzoly
V infoproc by se nemelo s konzolou nic delat
==================================================
*/
void Console::print(const std::string& str)
{
	for (size_t pos = 0; pos < str.length(); ++pos)
    {
		char tx = str[pos];

        switch (tx)
        {
        case '\n':
            text[bufpos++] = tx;
            break;
        case '\t':
			for (int i = 0; i < CON_TAB_WIDTH; i++, bufpos++)
			{
				text[bufpos % CON_TEXT_SIZE] = ' ';
			}
            break;
        default:
			if (tx >= ' ')
			{
				text[bufpos++] = tx;
			}
            break;
        }

        if (bufpos >= CON_TEXT_SIZE)
        {
            bufpos -= CON_TEXT_SIZE;
            buffull = true;
        }
    }

    scroll = 0;
}

///////////////////////////////////////////////////////
//                  Registrace                       //
///////////////////////////////////////////////////////

/*
==================================================
Kontrola pripustnosti jmena
==================================================
*/
int Console::isNameValid(const std::string& name, const std::string& proc, void *p)
{
	if (name.empty())
    {
		print(CON_Format(CON_Lang("{0} : cannot register empty name\n")) << proc);
        return CON_FAILED;
    }

    if (findCommand(name) != nullptr || findVar(name) != nullptr || findAlias(name) != nullptr)
    {
		print(CON_Format(CON_Lang("{0} : name \"{1}\" is already registered\n")) << proc << name);
        return CON_ALREADY;
    }

    if (p == nullptr)
    {
		print(CON_Format(CON_Lang("{0} : attempt to register \"{1}\" with null pointer\n")) << proc << name);
        return CON_FAILED;
    }

    return CON_SUCCES;
}

/*
==================================================
Registrovani prikazu
==================================================
*/
int Console::registerCommand(Callback callback, const std::string& name)
{
    int i = isNameValid(name, CON_Lang("Command registration"), (void *)callback);
    if (i != CON_SUCCES)
        return i;

	Command newCommand;
	newCommand.name = name;
	newCommand.callback = callback;

    // Zaradi novy prikaz tak, aby byly setridene podle abecedy
	size_t position = 0;
    for (const Command& command : cmds)
    {
		if (command.name.compare(newCommand.name) > 0)
		{
			break;
		}
		++position;
    }

	cmds.insert(cmds.begin() + position, newCommand);

	if (flags & CON_F_REG_INFO)
	{
		print(CON_Format(CON_Lang("Command registration: \"{0}\" at address {1} has been successful\n")) << name << callback);
	}

    return CON_SUCCES;
}

/*
==================================================
Registrace aliasu
==================================================
*/
int Console::registerAlias(const std::string& name, const std::string& cmd)
{
    Alias* a = findAlias(name);

    if (a == nullptr)
    {
        int i = isNameValid(name, CON_Lang("Alias registration"), (void *) (1));
        if (i != CON_SUCCES)
            return i;

		Alias newAlias;
		newAlias.name = name;
		newAlias.command = cmd;

        // Zaradi novy alias tak, aby byly setridene podle abecedy
		size_t position = 0;
		for (const Alias& alias : aliases)
        {
			if (alias.name.compare(name) > 0)
			{
				break;
			}
			++position;
        }

		aliases.insert(aliases.begin() + position, newAlias);
    }
	else
	{
		a->command = cmd;
	}

	if (flags & CON_F_REG_INFO)
	{
		print(CON_Format(CON_Lang("Alias registration: \"{0}\" as \"{1}\" has been successful\n")) << name << cmd);
	}

    return CON_SUCCES;
}

/*
==================================================
Vraceni ukazatele na prikaz
==================================================
*/
Console::Command* Console::findCommand(const std::string& name)
{
	for (Command& command : cmds)
	{
		if (command.name == name)
		{
			return &command;
		}
	}

    return nullptr;
}

/*
==================================================
Vraci ukazatel na alias s danym jmenem
==================================================
*/
Console::Alias* Console::findAlias(const std::string& name)
{
	for (Alias& alias : aliases)
	{
		if (alias.name == name)
		{
			return &alias;
		}
	}

    return nullptr;
}

/*
==================================================
Nastaveni ukazatele na font
==================================================
*/
void Console::setFont(const conBYTE *p)
{
    font = p;
}

/*
==================================================
Nastaveni kolik predchozich radku bude zobrazeno
==================================================
*/
void Console::setLast(int sl)
{
    show = sl > 1 ? sl : 2;
}

/*
==================================================
Vraci ukazatel na textovy buffer konzole
==================================================
*/
const conBYTE *Console::getText(unsigned long *buf_pos, bool *buf_full) const
{
    *buf_pos = bufpos;
    *buf_full = buffull;
    return &text[0];
}
