/*
Copyright (C) 2003 Parallel Realities
Copyright (C) 2011, 2012, 2013 Guus Sliepen
Copyright (C) 2015 Julian Marchant

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Starfighter.h"

static char saveSlot[10][25];

/*
Reads in each save game that it finds and gives it an appropriate
description using the area variable contained in the game binary
data. It returns the slot number (1 - 10) of the most recently
used file. On the title screen, this is used to determine whether
a player can "Continue Current Game" and "Load Saved Game".
*/
int initSaveSlots()
{
	char fileName[PATH_MAX];
	int imagePos = 350;
	Game tempGame;
	struct stat fileInfo;
	int modTime = 0;
	int continueSaveIndex = 0;

	FILE *fp;

	//READ SAVE GAME DATA
	for (int i = 0 ; i <= 5 ; i++)
	{
		sprintf(fileName, "%ssave%.2d.dat", engine.userHomeDirectory, i);

		fp = fopen(fileName, "rb");
		if (fp == NULL)
		{
			sprintf(saveSlot[i], (i == 0 ? "�����ȥ����� (��)" : "��"));
			if (engine.gameSection == SECTION_TITLE)
				textSurface(TS_SAVESLOT_0 + i, saveSlot[i], -1, imagePos,
					FONT_WHITE);
		}
		else
		{
			if (i == 0)
			{
				sprintf(saveSlot[i], "�����ȥ�����");
			}
			else
			{
				if (fread(&tempGame, sizeof(Game), 1, fp) != 1)
				{
					sprintf(saveSlot[i], "���줿������ǡ���");
				}
				else
				{
					sprintf(saveSlot[i], "%s�� %s", systemNames[tempGame.system],
						tempGame.stationedName);
				}
			}

			if (engine.gameSection == SECTION_TITLE)
				textSurface(TS_SAVESLOT_0 + i, saveSlot[i], -1,
					imagePos, FONT_WHITE);

			if (stat(fileName, &fileInfo) != -1)
			{
				if (fileInfo.st_mtime > modTime)
					{modTime = fileInfo.st_mtime; continueSaveIndex = i;}
			}

			fclose(fp);
		}
		imagePos += 20;
	}

	return continueSaveIndex;
}

/*
Fill in later...
*/
bool loadGame(int slot)
{
	char filename[PATH_MAX];
	FILE *fp;
	sprintf(filename, "%ssave%.2d.dat", engine.userHomeDirectory, slot);

	fp = fopen(filename, "rb");

	if (fp == NULL)
		return false;

	if (fread(&game, sizeof(Game), 1, fp) != 1)
	{
		printf("Save game error. The file was not of the expected format.\n");
		fclose(fp);
		return false;
	}

	fclose(fp);

	if (game.saveFormat < 2)
		game.difficulty = DIFFICULTY_NORMAL;

	weapon[W_PLAYER_WEAPON] = game.playerWeapon;
	player = game.thePlayer;

	// Re-init all the planets in this system...
	initPlanetMissions(game.system);

	// ... and then override with completition status
	for (int i = 0 ; i < 10 ; i++)
		systemPlanet[i].missionCompleted = game.missionCompleted[i];

	return true;
}

void saveGame(int slot)
{
	FILE *fp;
	char fileName[PATH_MAX];

	if ((slot < 0) || (slot > 5))
	{
		printf("Error - Saves may only be 0 to 5\n");
		return;
	}

	sprintf(fileName, "%ssave%.2d.dat", engine.userHomeDirectory, slot);
	fp = fopen(fileName, "wb");

	game.saveFormat = 3;
	game.playerWeapon = weapon[W_PLAYER_WEAPON];
	game.thePlayer = player;
	for (int i = 0 ; i < 10 ; i++)
		game.missionCompleted[i] = systemPlanet[i].missionCompleted;

	if (fp != NULL)
	{
		if (fwrite(&game, sizeof(Game), 1, fp) != 1)
		{
			printf("Error Saving Game to Slot %d\n", slot);
		}
		fclose(fp);
	}
	else
	{
		printf("Error Saving Game to Slot %d\n", slot);
	}

	// Recall to update the save slots... (lazy, yes)
	initSaveSlots();
	engine.keyState[KEY_FIRE] = 0;
}

void createSavesSurface(SDL_Surface *savesSurface, signed char clickedSlot)
{
	int y = 10;

	blevelRect(savesSurface, 0, 0, 348, 298, 0x00, 0x00, 0x00);

	for (int i = 1 ; i <= 5 ; i++)
	{
		if (clickedSlot == i)
			blevelRect(savesSurface, 5, y, 338, 25, 0x99, 0x00, 0x00);
		else
			blevelRect(savesSurface, 5, y, 338, 25, 0x00, 0x00, 0x99);
		drawString(saveSlot[i], 70, y + 5, FONT_WHITE, savesSurface);
		y += 30;
	}

	drawString("*** �إ�� ***", 120, 170, FONT_WHITE, savesSurface);

	switch (clickedSlot)
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			blevelRect(savesSurface, 5, 265, 100, 25, 0x00, 0x99, 0x00);
			blevelRect(savesSurface, 125, 265, 100, 25, 0x99, 0x99, 0x00);
			blevelRect(savesSurface, 243, 265, 100, 25, 0x99, 0x00, 0x00);
			drawString("��¸", 40, 270, FONT_WHITE, savesSurface);
			drawString("���ä�", 150, 270, FONT_WHITE, savesSurface);
			drawString("���", 270, 270, FONT_WHITE, savesSurface);

			drawString("[��¸] ������ξ��֤���¸����", 17, 200, FONT_WHITE, savesSurface);
			drawString("[���ä�] ����åȤ������������", 17, 220, FONT_WHITE,
				savesSurface);
			drawString("[���] ��¸�������֤�������", 17, 240, FONT_WHITE,
				savesSurface);
			break;
		case -1:
			drawString("����åȤ����򤷤Ƥ�������", 17, 200,
				FONT_WHITE, savesSurface);
			break;
		case -10:
			drawString("��¸����", 130, 200, FONT_WHITE, savesSurface);
			break;
		case -11:
			drawString("�������", 130, 200, FONT_WHITE, savesSurface);
			break;
	}

	engine.keyState[KEY_FIRE] = 0;
}

/*
Displays the save slot available. For use with an interface that
has the cursor enabled. It returns the index number of the slot clicked
so that the function invoking it can perform a load or save on that slot.
*/
int showSaveSlots(SDL_Surface *savesSurface, signed char saveSlot)
{
	int clickedSlot = -1;

	SDL_Rect r;
	r.x = 201;
	r.y = 115;
	r.w = 348;
	r.h = 25;

	if ((engine.keyState[KEY_FIRE]))
	{
		for (int i = 1 ; i <= 5 ; i++)
		{
			if (collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6,
				r.x, r.y, r.w, r.h))
			{
				clickedSlot = i;
				createSavesSurface(savesSurface, i);
			}
			r.y += 30;
		}

		if (collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 215,
			365, 100, 25))
		{
			saveGame(saveSlot);
			createSavesSurface(savesSurface, -10);
		}

		if (collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 335,
				365, 100, 25))
			createSavesSurface(savesSurface, -1);

		if (collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 453,
			365, 100, 25))
		{
			char filename[PATH_MAX];
			sprintf(filename, "%ssave%.2d.dat", engine.userHomeDirectory,
				saveSlot);
			remove(filename);
			initSaveSlots();
			createSavesSurface(savesSurface, -11);
		}
	}

	if (clickedSlot > -1)
		saveSlot = clickedSlot;

	return saveSlot;
}
