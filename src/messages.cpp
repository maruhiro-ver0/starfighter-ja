/*
Copyright (C) 2003 Parallel Realities
Copyright (C) 2011, 2012 Guus Sliepen
Copyright (C) 2015, 2016 onpon4 <onpon4@riseup.net>

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

static const char *deathMessage[6] = {
	"������ ����!",
	"���率�������������������å�!!!!",
	"�����ȸ��äƤ����",
	"���ꥹ����������!!",
	"���Ȥ������Ȥ�?!",
	"���ꥹ��������������������������!!!!",
};

static const char *killMessage[10] = {
	"1���ɲ�!",
	"�פ��Τä���!",
	"�����1���ɲ�!",
	"��ɸ���˲�!",
	"�夤��!",
	"��ä���!",

	"�����%d���ܤ���",

	"����%s�����Ƥ���Τ�?!",
	"������ɤ��Ĥ�������%s!",

	"�ޤ���1����!",
};

static const char *missFireMessage[5] = {
	"Ũ����ʤ���!",
	"�������褯����!",
	"�����Ƥ���?! �ۤ����!",
	"���ä�!!! �ߥ����ȴ�äƤ��!",
	"�ܤ򳫤���!!",
};

static const char *playerHitMessage[3] = {
	"���ä�! ���ޤʤ�!",
	"������ɤ�!",
	"�ߥ�������������֤�!",
};

void getKillMessage(object *ally)
{
	char in[50], otherName[30];
	int kills, difference;
	bool firstPlace = false;
	int faceToUse = FS_PHOEBE;

	if (ally == &aliens[ALIEN_PHOEBE])
	{
		strcpy(otherName, "���������");
		kills = game.wingMate1Kills;
		difference = game.wingMate1Kills - game.wingMate2Kills;
		if ((game.wingMate1Kills > game.wingMate2Kills) &&
				(game.wingMate1Kills > game.totalKills))
			firstPlace = true;
		faceToUse = FS_PHOEBE;
	}
	else
	{
		strcpy(otherName, "�ե�����");
		kills = game.wingMate2Kills;
		difference = game.wingMate2Kills - game.wingMate1Kills;
		if ((game.wingMate2Kills > game.wingMate1Kills) &&
				(game.wingMate2Kills > game.totalKills))
			firstPlace = true;
		faceToUse = FS_URSULA;
	}

	int r = rand() % 10;
	
	if (game.hasWingMate2 == 0)
		r = rand() % 7;

	switch(r)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			strcpy(in, killMessage[rand() % 6]);
			break;

		case 6:
		case 7:
			sprintf(in, killMessage[6], kills);
			break;

		case 8:
			if (difference > 0)
			{
				sprintf(in, killMessage[7], otherName);
			}
			else
			{
				sprintf(in, killMessage[8], otherName);
			}
			break;

		case 9:
			if (firstPlace)
			{
				strcpy(in, killMessage[9]);
			}
			else
			{
				strcpy(in, killMessage[rand() % 6]);
			}
			break;
	}

	setRadioMessage(faceToUse, in, 0);
}

const char *getKlineInsult()
{
	static const char *insult[] = {
		"�ߤ�����ʡ�", "���ˤϼ�˾�������", "ͦ���������ƶ򤫤���", "�Ф��Ф�������",
		"���Ϥ�Ϥ���α�ͺ�ˤ����ʤ���"
	};

	if (game.area != MISN_VENUS)
		return (insult[rand() % 3]);
	else
		return (insult[3 + (rand() % 2)]);
}

void getPlayerDeathMessage()
{
	if (aliens[ALIEN_KLINE].active)
	{
		setRadioMessage(FS_KLINE, getKlineInsult(), 1);
		return;
	}
	
	if ((aliens[ALIEN_BOSS].active) && (aliens[ALIEN_BOSS].classDef == CD_KRASS))
	{
		setRadioMessage(FS_KRASS, "�����$90,000,000�Ȥϥ��������! ���ϥϥϥϥϥ�!!", 1);
		return;
	}

	int faceToUse = FS_PHOEBE;
	

	if ((game.area == MISN_URUSOR) || (game.area == MISN_POSWIC) ||
		(game.area == MISN_EARTH))
	{
		faceToUse = FS_SID;
	}
	else if (game.hasWingMate2)
	{
		if ((rand() % 2) == 0)
			faceToUse = FS_URSULA;
	}

	setRadioMessage(faceToUse, deathMessage[rand() % 6], 1);
}

void getMissFireMessage(object *ally)
{
	int faceToUse = FS_PHOEBE;

	if (ally == &aliens[ALIEN_PHOEBE])
		faceToUse = FS_PHOEBE;
	else
		faceToUse = FS_URSULA;

	setRadioMessage(faceToUse, missFireMessage[rand() % 5], 0);
}

void getPlayerHitMessage(object *ally)
{
	int faceToUse = FS_PHOEBE;

	if (ally == &aliens[ALIEN_PHOEBE])
		faceToUse = FS_PHOEBE;
	else
		faceToUse = FS_URSULA;

	setRadioMessage(faceToUse, playerHitMessage[rand() % 3], 0);
}
