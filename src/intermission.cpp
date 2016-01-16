/*
Copyright (C) 2003 Parallel Realities
Copyright (C) 2011, 2012, 2013 Guus Sliepen
Copyright (C) 2012, 2015, 2016 onpon4 <onpon4@riseup.net>

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

/*
Drives the cursor. Is used by some other screens too
*/
static void intermission_doCursor()
{
	getPlayerInput();

	LIMIT(engine.cursor_x, 10, screen->w - 10 - gfx_sprites[SP_CURSOR]->w);
	LIMIT(engine.cursor_y, 10, screen->h - 10 - gfx_sprites[SP_CURSOR]->h);
	screen_blit(gfx_sprites[SP_CURSOR], engine.cursor_x, engine.cursor_y);
}

/*
Sets the player's current status information lines. These are the lines
that are scrolled up the screen when the player clicks on Current Status
These are set only once.
*/
static void intermission_setStatusLines()
{
	char string[50];
	int timeTaken = game.timeTaken;
	int y;

	sprintf(string, "撃った弾の数 : %d", game.shots);
	gfx_createTextObject(TS_SHOTS_FIRED, string, 0, 0, FONT_WHITE);

	sprintf(string, "命中した弾の数 : %d", game.hits);
	gfx_createTextObject(TS_HITS_SCORED, string, 0, 0, FONT_WHITE);

	sprintf(string, "命中率 : %d%%", game.accuracy);
	gfx_createTextObject(TS_ACCURACY, string, 0, 0, FONT_WHITE);

	sprintf(string, "プレイヤー以外に撃墜された敵の数 : %d", game.totalOtherKills);
	gfx_createTextObject(TS_OTHER_KILLS, string, 0, 0, FONT_WHITE);

	sprintf(string, "稼いだ送金額 : %d", game.cashEarned);
	gfx_createTextObject(TS_CASH_EARNED, string, 0, 0, FONT_WHITE);

	gfx_createTextObject(TS_CHRIS_HEADER, "*** クリス ***", 0, 0, FONT_WHITE);

	sprintf(string, "撃墜数 : %d", game.totalKills);
	gfx_createTextObject(TS_CHRIS_KILLS, string, 0, 0, FONT_WHITE);

	sprintf(string, "獲得したシールド回復の数 : %d", game.shieldPickups);
	gfx_createTextObject(TS_CHRIS_SHIELD_PICKUPS, string, 0, 0, FONT_WHITE);

	sprintf(string, "獲得したプラズマセルの数 : %d", game.cellPickups);
	gfx_createTextObject(TS_CHRIS_PLASMA_PICKUPS, string, 0, 0, FONT_WHITE);

	sprintf(string, "獲得したロケット弾の数 : %d", game.rocketPickups);
	gfx_createTextObject(TS_CHRIS_ROCKET_PICKUPS, string, 0, 0, FONT_WHITE);

	sprintf(string, "獲得したパワーアップの数 : %d", game.powerups);
	gfx_createTextObject(TS_CHRIS_POWERUP_PICKUPS, string, 0, 0, FONT_WHITE);

	sprintf(string, "破壊した機雷の数 : %d", game.minesKilled);
	gfx_createTextObject(TS_CHRIS_MINES_KILLED, string, 0, 0, FONT_WHITE);

	sprintf(string, "救出した奴隷の人数 : %d", game.slavesRescued);
	gfx_createTextObject(TS_CHRIS_SLAVES_RESCUED, string, 0, 0, FONT_WHITE);

	if (game.hasWingMate1)
	{
		gfx_createTextObject(TS_PHOEBE_HEADER, "*** フェーベ ***", 0, 0, FONT_WHITE);

		sprintf(string, "撃墜数 : %d", game.wingMate1Kills);
		gfx_createTextObject(TS_PHOEBE_KILLS, string, 0, 0, FONT_WHITE);

		sprintf(string, "脱出した回数 : %d", game.wingMate1Ejects);
		gfx_createTextObject(TS_PHOEBE_DEATHS, string, 0, 0, FONT_WHITE);
	}
	else
	{
		gfx_createTextObject(TS_PHOEBE_HEADER, "", 0, 0, FONT_WHITE);
		gfx_createTextObject(TS_PHOEBE_KILLS, "", 0, 0, FONT_WHITE);
		gfx_createTextObject(TS_PHOEBE_DEATHS, "", 0, 0, FONT_WHITE);
	}

	if (game.hasWingMate2)
	{
		gfx_createTextObject(TS_URSULA_HEADER, "*** ウーシュラ ***", 0, 0, FONT_WHITE);

		sprintf(string, "撃墜数 : %d", game.wingMate2Kills);
		gfx_createTextObject(TS_URSULA_KILLS, string, 0, 0, FONT_WHITE);

		sprintf(string, "脱出した回数 : %d", game.wingMate2Ejects);
		gfx_createTextObject(TS_URSULA_DEATHS, string, 0, 0, FONT_WHITE);
	}
	else
	{
		gfx_createTextObject(TS_URSULA_HEADER, "", 0, 0, FONT_WHITE);
		gfx_createTextObject(TS_URSULA_KILLS, "", 0, 0, FONT_WHITE);
		gfx_createTextObject(TS_URSULA_DEATHS, "", 0, 0, FONT_WHITE);
	}

	gfx_createTextObject(TS_STATUS_HEADER, "現在の状態", -1, 83, FONT_WHITE);

	snprintf(string, sizeof string, "総時間 : %2d:%02d:%02d", timeTaken / 3600, (timeTaken / 60) % 60, timeTaken % 60);
	gfx_createTextObject(TS_STATUS_FOOTER, string, -1, 404, FONT_WHITE);

	y = 400;
	for (int i = TS_STATUS_HEADER + 1 ; i < TS_STATUS_FOOTER ; i++)
	{
		y += 20;
		if ((i == TS_CHRIS_HEADER) || (i == TS_PHOEBE_HEADER) ||
				(i == TS_URSULA_HEADER))
			y += 25;

		gfx_textSprites[i].x = 150;
		gfx_textSprites[i].y = y;
	}
}

/*
Sets the names and stats of the planets within the current system.
*/
static void intermission_setSystemPlanets()
{
	for (int i = 0 ; i < 10 ; i++)
	{
		systemPlanet[i].y = -1;
		strcpy(systemPlanet[i].name, "");
		systemPlanet[i].image = NULL;
		systemPlanet[i].messageMission = -1;
		systemPlanet[i].messageSlot = -1;
		systemPlanet[i].faceImage = -1;
		strcpy(systemPlanet[i].subject, "ERROR");
	}

	switch (game.system)
	{
		case SYSTEM_SPIRIT:
			systemPlanet[PLANET_HAIL].y = 15;
			strcpy(systemPlanet[PLANET_HAIL].name, "ハイル");
			systemPlanet[PLANET_HAIL].image = gfx_sprites[SP_PLANET_GREEN];

			systemPlanet[PLANET_CERADSE].y = 30;
			strcpy(systemPlanet[PLANET_CERADSE].name, "セラドセ");
			systemPlanet[PLANET_CERADSE].image = gfx_sprites[SP_PLANET_BLUE];

			systemPlanet[PLANET_HINSTAG].y = 30;
			strcpy(systemPlanet[PLANET_HINSTAG].name, "ヒンスタグ");
			systemPlanet[PLANET_HINSTAG].image = gfx_sprites[SP_PLANET_RED];

			systemPlanet[PLANET_JOLDAR].y = 20;
			strcpy(systemPlanet[PLANET_JOLDAR].name, "ジョルダー");
			systemPlanet[PLANET_JOLDAR].image = gfx_sprites[SP_PLANET_GREEN];

			systemPlanet[PLANET_MOEBO].y = 40;
			strcpy(systemPlanet[PLANET_MOEBO].name, "モエボ");
			systemPlanet[PLANET_MOEBO].image = gfx_sprites[SP_PLANET_ORANGE];

			systemPlanet[PLANET_HAIL].messageMission = MISN_HAIL;
			systemPlanet[PLANET_HAIL].messageSlot = 0;
			systemPlanet[PLANET_HAIL].faceImage = FS_KRASS;
			strcpy(systemPlanet[PLANET_HAIL].subject, "WEAPCOの訓練所の破壊");

			systemPlanet[PLANET_CERADSE].messageMission = MISN_CERADSE;
			systemPlanet[PLANET_CERADSE].messageSlot = 1;
			systemPlanet[PLANET_CERADSE].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_CERADSE].subject, "6つの積荷の回収");

			systemPlanet[PLANET_HINSTAG].messageMission = MISN_HINSTAG;
			systemPlanet[PLANET_HINSTAG].messageSlot = 2;
			systemPlanet[PLANET_HINSTAG].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_HINSTAG].subject, "5隻のWEAPCOミサイル艇の破壊");

			systemPlanet[PLANET_JOLDAR].messageMission = MISN_JOLDAR;
			systemPlanet[PLANET_JOLDAR].messageSlot = 3;
			systemPlanet[PLANET_JOLDAR].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_JOLDAR].subject, "ジョルダー周辺の機雷の除去");

			systemPlanet[PLANET_MOEBO].messageMission = MISN_MOEBO;
			systemPlanet[PLANET_MOEBO].messageSlot = 0;
			systemPlanet[PLANET_MOEBO].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_MOEBO].subject, "WEAPCOフリゲートの破壊");

			break;

		case SYSTEM_EYANANTH:
			strcpy(systemPlanet[PLANET_RESCUESLAVES].name, "WEAPCOの迎撃部隊");

			systemPlanet[PLANET_NEROD].y = 15;
			strcpy(systemPlanet[PLANET_NEROD].name, "ネロド");
			systemPlanet[PLANET_NEROD].image = gfx_sprites[SP_PLANET_GREEN];

			systemPlanet[PLANET_ALLEZ].y = 30;
			strcpy(systemPlanet[PLANET_ALLEZ].name, "アレズ");
			systemPlanet[PLANET_ALLEZ].image = gfx_sprites[SP_PLANET_BLUE];

			systemPlanet[PLANET_URUSOR].y = 30;
			strcpy(systemPlanet[PLANET_URUSOR].name, "ウルソル");
			systemPlanet[PLANET_URUSOR].image = gfx_sprites[SP_PLANET_RED];

			systemPlanet[PLANET_DORIM].y = 20;
			strcpy(systemPlanet[PLANET_DORIM].name, "ドリム");
			systemPlanet[PLANET_DORIM].image = gfx_sprites[SP_PLANET_GREEN];

			systemPlanet[PLANET_ELAMALE].y = 40;
			strcpy(systemPlanet[PLANET_ELAMALE].name, "エラメール");
			systemPlanet[PLANET_ELAMALE].image = gfx_sprites[SP_PLANET_ORANGE];

			systemPlanet[PLANET_RESCUESLAVES].messageMission = MISN_RESCUESLAVES;
			systemPlanet[PLANET_RESCUESLAVES].messageSlot = 0;
			systemPlanet[PLANET_RESCUESLAVES].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_RESCUESLAVES].subject, "奴隷の救出");

			systemPlanet[PLANET_NEROD].messageMission = MISN_NEROD;
			systemPlanet[PLANET_NEROD].messageSlot = 1;
			systemPlanet[PLANET_NEROD].faceImage = FS_PHOEBE;
			strcpy(systemPlanet[PLANET_NEROD].subject, "SOS");

			systemPlanet[PLANET_ALLEZ].messageMission = MISN_ALLEZ;
			systemPlanet[PLANET_ALLEZ].messageSlot = 2;
			systemPlanet[PLANET_ALLEZ].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_ALLEZ].subject, "医薬品の輸送船の救助");

			systemPlanet[PLANET_URUSOR].messageMission = MISN_URUSOR;
			systemPlanet[PLANET_URUSOR].messageSlot = 0;
			systemPlanet[PLANET_URUSOR].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_URUSOR].subject, "5隻のWEAPCO補給艦の拿捕");

			systemPlanet[PLANET_DORIM].messageMission = MISN_DORIM;
			systemPlanet[PLANET_DORIM].messageSlot = 1;
			systemPlanet[PLANET_DORIM].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_DORIM].subject, "WEAPCOの科学者の捜索");

			systemPlanet[PLANET_ELAMALE].messageMission = MISN_ELAMALE;
			systemPlanet[PLANET_ELAMALE].messageSlot = 0;
			systemPlanet[PLANET_ELAMALE].faceImage = FS_PHOEBE;
			strcpy(systemPlanet[PLANET_ELAMALE].subject, "WEAPCOの採掘船の破壊");

			break;

		case SYSTEM_MORDOR:
			strcpy(systemPlanet[PLANET_CLOAKFIGHTER].name, "WEAPCOの迎撃部隊");

			systemPlanet[PLANET_ODEON].y = 15;
			strcpy(systemPlanet[PLANET_ODEON].name, "オデオン");
			systemPlanet[PLANET_ODEON].image = gfx_sprites[SP_PLANET_GREEN];

			systemPlanet[PLANET_FELLON].y = 30;
			strcpy(systemPlanet[PLANET_FELLON].name, "フェロン");
			systemPlanet[PLANET_FELLON].image = gfx_sprites[SP_PLANET_BLUE];

			systemPlanet[PLANET_SIVEDI].y = 30;
			strcpy(systemPlanet[PLANET_SIVEDI].name, "サイベジ");
			systemPlanet[PLANET_SIVEDI].image = gfx_sprites[SP_PLANET_RED];

			systemPlanet[PLANET_ALMARTHA].y = 20;
			strcpy(systemPlanet[PLANET_ALMARTHA].name, "アルマサ");
			systemPlanet[PLANET_ALMARTHA].image = gfx_sprites[SP_PLANET_GREEN];

			systemPlanet[PLANET_POSWIC].y = 20;
			strcpy(systemPlanet[PLANET_POSWIC].name, "ポスイック");
			systemPlanet[PLANET_POSWIC].image = gfx_sprites[SP_PLANET_ORANGE];

			systemPlanet[PLANET_ELLESH].y = 40;
			strcpy(systemPlanet[PLANET_ELLESH].name, "エリッシュ");
			systemPlanet[PLANET_ELLESH].image = gfx_sprites[SP_PLANET_GREEN];

			systemPlanet[PLANET_CLOAKFIGHTER].messageMission = MISN_CLOAKFIGHTER;
			systemPlanet[PLANET_CLOAKFIGHTER].messageSlot = 0;
			systemPlanet[PLANET_CLOAKFIGHTER].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_CLOAKFIGHTER].subject, "試作戦闘機の破壊");

			systemPlanet[PLANET_ODEON].messageMission = MISN_ODEON;
			systemPlanet[PLANET_ODEON].messageSlot = 1;
			systemPlanet[PLANET_ODEON].faceImage = FS_PHOEBE;
			strcpy(systemPlanet[PLANET_ODEON].subject, "ウーシュラの救出");

			systemPlanet[PLANET_FELLON].messageMission = MISN_FELLON;
			systemPlanet[PLANET_FELLON].messageSlot = 2;
			systemPlanet[PLANET_FELLON].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_FELLON].subject, "反乱軍の支援");

			systemPlanet[PLANET_SIVEDI].messageMission = MISN_SIVEDI;
			systemPlanet[PLANET_SIVEDI].messageSlot = 0;
			systemPlanet[PLANET_SIVEDI].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_SIVEDI].subject, "アステロイドベルトでの採掘");

			systemPlanet[PLANET_ALMARTHA].messageMission = MISN_ALMARTHA;
			systemPlanet[PLANET_ALMARTHA].messageSlot = 1;
			systemPlanet[PLANET_ALMARTHA].faceImage = FS_KRASS;
			strcpy(systemPlanet[PLANET_ALMARTHA].subject, "代金の用意");

			systemPlanet[PLANET_POSWIC].messageMission = MISN_POSWIC;
			systemPlanet[PLANET_POSWIC].messageSlot = 0;
			systemPlanet[PLANET_POSWIC].faceImage = FS_URSULA;
			strcpy(systemPlanet[PLANET_POSWIC].subject, "WEAPCOの重役の搬送の阻止");

			systemPlanet[PLANET_ELLESH].messageMission = MISN_ELLESH;
			systemPlanet[PLANET_ELLESH].messageSlot = 0;
			systemPlanet[PLANET_ELLESH].faceImage = FS_PHOEBE;
			strcpy(systemPlanet[PLANET_ELLESH].subject, "WEAPCOの重役の追跡");

			break;

		case SYSTEM_SOL:
			systemPlanet[PLANET_MERCURY].y = 15;
			strcpy(systemPlanet[PLANET_MERCURY].name, "水星");
			systemPlanet[PLANET_MERCURY].image = gfx_sprites[SP_PLANET_RED];

			systemPlanet[PLANET_VENUS].y = 20;
			strcpy(systemPlanet[PLANET_VENUS].name, "金星");
			systemPlanet[PLANET_VENUS].image = gfx_sprites[SP_PLANET_ORANGE];

			systemPlanet[PLANET_EARTH].y = 20;
			strcpy(systemPlanet[PLANET_EARTH].name, "地球");
			systemPlanet[PLANET_EARTH].image = gfx_sprites[SP_PLANET_BLUE];

			systemPlanet[PLANET_MARS].y = 20;
			strcpy(systemPlanet[PLANET_MARS].name, "火星");
			systemPlanet[PLANET_MARS].image = gfx_sprites[SP_PLANET_RED];

			systemPlanet[PLANET_JUPITER].y = 30;
			strcpy(systemPlanet[PLANET_JUPITER].name, "木星");
			systemPlanet[PLANET_JUPITER].image = gfx_sprites[SP_PLANET_ORANGE];

			systemPlanet[PLANET_SATURN].y = 20;
			strcpy(systemPlanet[PLANET_SATURN].name, "土星");
			systemPlanet[PLANET_SATURN].image = gfx_sprites[SP_PLANET_GREEN];

			systemPlanet[PLANET_URANUS].y = 20;
			strcpy(systemPlanet[PLANET_URANUS].name, "天王星");
			systemPlanet[PLANET_URANUS].image = gfx_sprites[SP_PLANET_BLUE];

			systemPlanet[PLANET_NEPTUNE].y = 20;
			strcpy(systemPlanet[PLANET_NEPTUNE].name, "海王星");
			systemPlanet[PLANET_NEPTUNE].image = gfx_sprites[SP_PLANET_BLUE];

			systemPlanet[PLANET_PLUTO].y = 20;
			strcpy(systemPlanet[PLANET_PLUTO].name, "冥王星");
			systemPlanet[PLANET_PLUTO].image = gfx_sprites[SP_PLANET_BLUE];

			systemPlanet[PLANET_PLUTO].messageMission = MISN_PLUTO;
			systemPlanet[PLANET_PLUTO].messageSlot = 0;
			systemPlanet[PLANET_PLUTO].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_PLUTO].subject, "冥王星の制圧");

			systemPlanet[PLANET_NEPTUNE].messageMission = MISN_NEPTUNE;
			systemPlanet[PLANET_NEPTUNE].messageSlot = 1;
			systemPlanet[PLANET_NEPTUNE].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_NEPTUNE].subject, "海王星の制圧");

			systemPlanet[PLANET_URANUS].messageMission = MISN_URANUS;
			systemPlanet[PLANET_URANUS].messageSlot = 2;
			systemPlanet[PLANET_URANUS].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_URANUS].subject, "天王星の制圧");

			systemPlanet[PLANET_SATURN].messageMission = MISN_SATURN;
			systemPlanet[PLANET_SATURN].messageSlot = 0;
			systemPlanet[PLANET_SATURN].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_SATURN].subject, "外惑星防衛システムの破壊");

			systemPlanet[PLANET_JUPITER].messageMission = MISN_JUPITER;
			systemPlanet[PLANET_JUPITER].messageSlot = 0;
			systemPlanet[PLANET_JUPITER].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_JUPITER].subject, "救難信号の調査");

			systemPlanet[PLANET_MARS].messageMission = MISN_MARS;
			systemPlanet[PLANET_MARS].messageSlot = 0;
			systemPlanet[PLANET_MARS].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_MARS].subject, "アステロイドベルトの航行");

			systemPlanet[PLANET_EARTH].messageMission = MISN_EARTH;
			systemPlanet[PLANET_EARTH].messageSlot = 0;
			systemPlanet[PLANET_EARTH].faceImage = FS_CHRIS;
			strcpy(systemPlanet[PLANET_EARTH].subject, "地球の奪還");

			systemPlanet[PLANET_VENUS].messageMission = MISN_VENUS;
			systemPlanet[PLANET_VENUS].messageSlot = 0;
			systemPlanet[PLANET_VENUS].faceImage = FS_SID;
			strcpy(systemPlanet[PLANET_VENUS].subject, "全てを終わらせる");

			break;
	}
}

/*
Spins the planets around the sun, spaced according to their Y value
as defined in intermission_setSystemPlanets(). Moving the cursor over the planet
will show their name and their current status
*/
static bool intermission_showSystem(float x, float y, bool selectable)
{
	SDL_Rect r;
	int planet = 0;
	int planetSpace = systemPlanet[planet].y;
	bool rtn = false;

	// Blit the sun
	screen_blit(gfx_sprites[SP_SUN], 370, 220);

	for (int i = 50 ; i < 300 ; i+= planetSpace)
	{
		x *= 0.75;
		y *= 0.75;

		gfx_drawCircle(400, 250, i, screen, darkGrey);

		r.x = int(400 + (sinf(x) * i));
		r.y = int(250 + (cosf(y) * i));
		r.w = 10;
		r.h = 10;

		r.x -= (systemPlanet[planet].image->w / 2);
		r.y -= (systemPlanet[planet].image->h / 2);
		screen_blit(systemPlanet[planet].image, r.x, r.y);

		if (selectable &&
			game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6,
				r.x, r.y, systemPlanet[planet].image->w,
				systemPlanet[planet].image->h))
		{
			screen_renderString(systemPlanet[planet].name, -1, 545, FONT_WHITE);
			if ((engine.keyState[KEY_FIRE]))
			{
				game.destinationPlanet = planet;
				strcpy(game.destinationName, systemPlanet[game.destinationPlanet].name);
				rtn = true;
				engine.keyState[KEY_FIRE] = 0;
			}
		}

		planet++;
		if (systemPlanet[planet].y == -1)
			break;
		planetSpace = systemPlanet[planet].y;
	}

	return rtn;
}

/*
Scrolls the player's current information up the screen. When
the specified status line reaches a certain Y value, the entire
list is reset and the information lines begin again from the bottom
(in other words, they loop around).
*/
static void intermission_showStatus(SDL_Surface *infoSurface)
{
	int y;
	float speed = 0.25;

	if(engine.keyState[KEY_DOWN])
		speed = 1;
	else if(engine.keyState[KEY_UP])
		speed = -1;

	screen_blit(infoSurface, 100, 80);

	for (int i = TS_STATUS_HEADER + 1 ; i < TS_STATUS_FOOTER ; i++)
	{
		gfx_textSprites[i].y -= speed;
		if ((gfx_textSprites[i].y > 80) && (gfx_textSprites[i].y < 400))
			screen_blitText(i);
	}

	if (gfx_textSprites[TS_STATUS_FOOTER - 1].y < 65)
	{
		y = 400;
		for (int i = TS_STATUS_HEADER + 1 ; i < TS_STATUS_FOOTER ; i++)
		{
			y += 20;
			if ((i == TS_CHRIS_HEADER) || (i == TS_PHOEBE_HEADER) ||
					(i == TS_URSULA_HEADER))
				y += 25;

			gfx_textSprites[i].y = y;
		}
	}

	screen_drawRect(100, 80, 600, 20, 0x00, 0x00, 0x99);

	screen_drawRect(100, 400, 600, 20, 0x00, 0x00, 0x99);

	screen_blitText(TS_STATUS_HEADER);
	screen_blitText(TS_STATUS_FOOTER);
}

static void intermission_updateCommsSurface(SDL_Surface *comms)
{
	char string[255];

	if (engine.commsSection == 1)
		return;

	gfx_drawRect(comms, 0, 10, comms->w - 1, 55, 0x00, 0x22, 0x00);
	gfx_blit(gfx_faceSprites[FS_CHRIS], 20, 15, comms);
	gfx_renderString("クリス・バンフィールド", 80, 15, FONT_WHITE, 0, comms);
	sprintf(string, "現在位置: %s", systemPlanet[game.stationedPlanet].name);
	gfx_renderString(string, 80, 35, FONT_WHITE, 0, comms);
}

static void intermission_createCommsSurface(SDL_Surface *comms)
{
	engine.commsSection = 0;

	gfx_drawRect(comms, 0, 0, comms->w - 1, comms->h - 1, 0x00, 0x00, 0x25);

	gfx_renderString("+++ 現在の作戦 (クリックで詳細を表示) +++", -1, 80, FONT_GREEN, 0, comms);

	int yOffset;

	// XXX: Magic number
	for (int i = 0 ; i < 10 ; i++)
	{
		if ((systemPlanet[i].messageSlot != -1) && (systemPlanet[i].missionCompleted == 0))
		{
			yOffset = systemPlanet[i].messageSlot * 60;
			gfx_drawRect(comms, 0, 105 + yOffset, comms->w - 1, 55, 0x00, 0x00, 0x77);
			gfx_blit(gfx_faceSprites[systemPlanet[i].faceImage], 20, 110 + yOffset, comms);
			gfx_renderString(systemPlanet[i].name, 80, 110 + yOffset, FONT_WHITE, 0, comms);
			gfx_renderString(systemPlanet[i].subject, 80, 130 + yOffset, FONT_CYAN, 0, comms);
		}
	}

	intermission_updateCommsSurface(comms);
}

static int intermission_renderDialog(SDL_Surface *comms, int y, int face, const char *string)
{
	int newY;
	gfx_blit(gfx_faceSprites[face], 10, y, comms);
	newY = gfx_renderString(string, 80, y, FONT_WHITE, 1, comms) + 25;
	if (newY < y + 60)
		newY += (60 - (newY - y));
	return newY;
}

static void intermission_createMissionDetailSurface(SDL_Surface *comms, int missionSlot)
{
	char name[50];
	char string[2000];
	int y = 50;
	int mission = -1;

	// XXX: Magic number
	for (int i = 0 ; i < 10 ; i++)
	{
		if ((systemPlanet[i].messageSlot == missionSlot) && (systemPlanet[i].missionCompleted == 0))
		{
			mission = systemPlanet[i].messageMission;
		}
	}

	if (mission == -1)
		return;

	gfx_drawRect(comms, 0, 0, comms->w - 1, comms->h - 1, 0x00, 0x00, 0x25);

	// XXX: The "name" should correspond with whatever name was presented in
	// the screen listing all of the comms. For some reason, this has always
	// been defined twice, which is redundant and has led to inconsistencies in
	// the past.
	switch (mission)
	{
		case MISN_HAIL:
			strcpy(name, "クラス・タイラー");

			strcpy(string, "おい、小僧! お前のために盗んでやったファイアフライの代金がまだだぞ! だが、その代わりにWEAPCOの訓練場へ行って、そこで全ての艦を破壊してくれ。");
			y = intermission_renderDialog(comms, y, FS_KRASS, string);

			strcpy(string, "何? それは俺がお前に頼んだ仕事だろう?");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "分かっている。だが、この方法ならお前はすぐに借りを返せるぞ。この仕事をして、さらに$500稼いでこい。これで帳消しだ。もしお前が死んだら…そうだな、その艦には盗むだけの価値がなかったってことだ! ハッハッハッ!");
			y = intermission_renderDialog(comms, y, FS_KRASS, string);

			strcpy(string, "相変わらず人使いが荒いな、クラス。");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			break;

		case MISN_CERADSE:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "よお、シド。どうかしたか?");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "クリス、輸送艦がWEAPCOに襲われた。どうやらセラドセに医療品を運んでいたようだ。多くの命を救うために積荷を取り戻さなければならない!");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "いくつあるんだ?");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "全部で6つだ! もし1つでも失えば、数ヶ月の内にこのスピリット系の人々が何千人も死んでしまうだろう。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			break;

		case MISN_HINSTAG:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "おっと! ミサイル艇か?");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "そうだ。どうやらWEAPCOは君の行動に注目し始めたようだな。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "おもしろい! ファイアフライの戦闘能力のテストにぴったりだ!");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "気をつけろ、クリス。1隻のミサイル艇でも主要都市を破壊できるほどのロケットを搭載している。奴らには正面から向かわず、十分な距離を保て。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			break;

		case MISN_JOLDAR:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "ジョルダー周辺の機雷を除去しなければならない。機雷地帯のせいで惑星間交通が阻害されている。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "退屈しのぎになりそうな戦闘機は近くにいないのか?");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "今の所はな。だが奴らが引き換えしてこないとは言い切れない。機雷には気をつけろ! WEAPCOの信号を発していない艦は近づいただけで爆発する。邪魔ならば打ち落とせ。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			break;

		case MISN_MOEBO:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "緊急事態だ! WEAPCOは我々の活動を止めるためにスピリットを破壊しようとしている! その爆発は星系の全ての惑星を巻き込むだろう! フリゲートが射程内に入る前に破壊しなければならない!");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "畜生め! 今すぐ発進する!");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "クリス、皆が君に期待している! だが忘れるな - 奴らは決して『スターキラー』とは呼ばない。なぜならそれではきれい過ぎるからだ!");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			break;

		case MISN_RESCUESLAVES:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "知っての通り、この星系にはたくさんのWEAPCOの奴隷がいる。彼らを解放できれば反撃のきっかけになるはずだ。変化が現れるためには、250人は救出する必要があるだろう。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "ほとんどの奴隷は採掘をしているそうだな。");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "そうだ。だが、採掘所を直接攻撃するのは危険だ。奴隷の搬入を阻止するのがいいだろう。あたりを飛行し、WEAPCOのパトロールを迎撃しろ。もちろん奴隷船を護衛しているとは限らない。十分に気をつけろ!!");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			break;

		case MISN_NEROD:
			strcpy(name, "フェーベ・レックス");

			strcpy(string, "助けて! SOS! 誰か聞こえてる?!");
			y = intermission_renderDialog(comms, y, FS_PHOEBE, string);

			strcpy(string, "よく聞こえるぞ! どうした?");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "ああ、助かったわ! ネロド周辺でWEAPCOの大軍に囲まれているの! 支援を!");
			y = intermission_renderDialog(comms, y, FS_PHOEBE, string);

			strcpy(string, "すぐ向かう!");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			break;

		case MISN_ALLEZ:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "今、別のSOSを受信した。重要な医薬品を運ぶ輸送艦からだ。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "分かった。すぐ向かうと伝えてくれ。");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			break;

		case MISN_URUSOR:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "この星系を離れる前にいくらか資源を確保する必要がある。それがあればモルドール系での活動がより簡単になるだろう。問題は、このあたりではWEAPCOしか持ってないことだ。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "それでどこへ行けば手に入るんだ?");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "近くにその大型補給艦がいる。私が航行不能にする。君にはその間の援護をたのむ。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "分かった!");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			break;

		case MISN_DORIM:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "WEAPCOの科学者がポッドで脱走し、アステロイドベルトに隠れているらしい。救出できればモルドール系の情報を聞けるかもしれない。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "すぐ行こう。");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			break;

		case MISN_ELAMALE:
			strcpy(name, "フェーベ・レックス");

			strcpy(string, "我々が救出した奴隷たちが反乱を起こしたとの情報が入った。計画はうまく進んでいるようだ。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "WEAPCOの無人採掘船がエラメールの軌道上にあるわ。これを破壊して撹乱するのはどう?");
			y = intermission_renderDialog(comms, y, FS_PHOEBE, string);

			strcpy(string, "いいアイデアだ!");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "それはうまく行くだろう。だが油断するな。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			break;

		case MISN_CLOAKFIGHTER:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "試作戦闘機のことは何か分かったか?");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "レーダーに映らない遮蔽装置の一種を搭載している。追跡は困難だ。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "偶然出会うまで飛び回るしかないな。");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "たとえ遭遇しても相手は逃げるだろう。見つけたら可能な限りダメージを与えることだ。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			break;

		case MISN_ODEON:
			strcpy(name, "フェーベ・レックス");

			strcpy(string, "姉さんの艦をオデオンの軌道上で見つけた。でも、こちらの通信を無視しているの。");
			y = intermission_renderDialog(comms, y, FS_PHOEBE, string);

			strcpy(string, "嫌な予感がするな。彼女はWEAPCOの集団の中を自由に移動している。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "寝返ったのか?");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "ありえない。WEAPCOをとても憎んでいたもの。");
			y = intermission_renderDialog(comms, y, FS_PHOEBE, string);

			strcpy(string, "マインドコントロールの一種かもしれない。WEAPCOが最近新しい「AIトレーニングプログラム」を開発中だと聞いた。早く助けたほうが良いな!");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			break;

		case MISN_FELLON:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "反乱軍による攻撃の計画がある。彼らを支援し勝利できれば、士気は大いに高まるだろう。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "すげえ! すぐ出撃だ!");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "反乱軍の艦が全て破壊されることだけは避けなければならない。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			break;

		case MISN_SIVEDI:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "WEAPCOの採掘船から奪うのは、あまりよい方法ではなかった。だが、あのアステロイドの鉱石は武器の生産に必要だ。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "おいおい! つまり、俺に取りに行けと言っているのか?");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "その通りだ。注意しろ、クリス。君の武器はそのような作業のために設計されていない。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			break;

		case MISN_ALMARTHA:
			strcpy(name, "クラス・タイラー");

			strcpy(string, "おいクラス! 手伝ってくれ。フェーベとウーシュラがWEAPCOの重要なプラントを破壊しようとしている。ちょっと騒ぎを起こして、こちらに注意を引き付けられないか?");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "いいぞ、小僧。助けてやろう。だが、金だ…");
			y = intermission_renderDialog(comms, y, FS_KRASS, string);

			break;

		case MISN_POSWIC:
			strcpy(name, "ウーシュラ・レックス");

			strcpy(string, "記憶が完全に戻った。そこでは面白いことが行われていたぞ。私が捕らえられる直前、WEAPCOが何人もの重役をポスイックに移送するのを見た。");
			y = intermission_renderDialog(comms, y, FS_URSULA, string);

			strcpy(string, "そのようなチャンスを見逃すわけにはいかない! 私もその艦を航行不能にするために参加する。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "頼むぞ!");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			break;

		case MISN_ELLESH:
			strcpy(name, "フェーベ・レックス");

			strcpy(string, "フェーベ、お前はここを見張っててくれ。俺はあの艦を追う!");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "追いつけると思ってるの?");
			y = intermission_renderDialog(comms, y, FS_PHOEBE, string);

			strcpy(string, "もちろんだ。このファイアフライのスピードがあればな。すぐに見せてやるよ!");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			break;

		case MISN_PLUTO:
		case MISN_NEPTUNE:
		case MISN_URANUS:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "まず外惑星を制圧し、そこから切り込む。そうすれば最後の作戦のとき両側から挟まれる危険性が小さくなる。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "分かった。");
			y = intermission_renderDialog(comms, y, FS_PHOEBE, string);

			strcpy(string, "OK");
			y = intermission_renderDialog(comms, y, FS_URSULA, string);

			strcpy(string, "了解。");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			break;

		case MISN_SATURN:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "WEAPCOは土星と天王星の間に高度な防衛ラインを引いている。これを取り除く必要がある。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "どのような防衛システムなのだ?");
			y = intermission_renderDialog(comms, y, FS_URSULA, string);

			strcpy(string, "小型の複数の移動できるレーザー砲で構成されている。かつてスピリット系で戦ったスターキラーとは全く異なる種類の兵器だ。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "ならば、私のミサイルランチャーをよく点検しておかなければね!");
			y = intermission_renderDialog(comms, y, FS_PHOEBE, string);

			break;

		case MISN_JUPITER:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "皆がいない間に、木星周辺から救難信号を受信した。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "太陽系内の誰が信号を送る必要があるのだ?");
			y = intermission_renderDialog(comms, y, FS_URSULA, string);

			strcpy(string, "調べてみよう。罠かもしれんが、何とかなるさ。");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			break;

		case MISN_MARS:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "クリス、少し問題がある。WEAPCOはアステロイドベルトに機雷原を敷いている。これを取り除かなければならない。");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "分かった。安全な道を作れたら連絡する。");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			break;

		case MISN_EARTH:
			strcpy(name, "全員");

			strcpy(string, "よしみんな、いよいよだ。俺たちは苦しい中をずっと進んできた。退かず全ての力で奴らを叩くしかない!");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "引き返すには遠すぎる所まで来てしまった。全員生きて帰るんだ!");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			strcpy(string, "行きましょう、クリス。");
			y = intermission_renderDialog(comms, y, FS_PHOEBE, string);

			strcpy(string, "WEAPCOに私の頭を操ったことを後悔させてやる!");
			y = intermission_renderDialog(comms, y, FS_URSULA, string);

			break;

		case MISN_VENUS:
			strcpy(name, "シド・ウィルソン");

			strcpy(string, "ケスランは金星へ逃げた。俺は奴を追う。");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "気をつけてくれ、クリス。我々は戦争に勝利した。なのに君に死なれたとあっては耐え難い恥辱だ!");
			y = intermission_renderDialog(comms, y, FS_SID, string);

			break;

		default:
			strcpy(name, "誰もいない");
			strcpy(string, "おい、何で俺は自分に話してるんだ? ありえない! これはバグだ。");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			sprintf(string, "starfighter.nongnu.orgへ行って、このバグを報告する必要があるな。作戦番号%dで問題発生だ。", mission);
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);

			strcpy(string, "待て、何を話しているんだ? 誰も聞いていないはずだが…");
			y = intermission_renderDialog(comms, y, FS_CHRIS, string);
	}

	sprintf(string, "+++ %sとの交信 +++", name);
	gfx_renderString(string, -1, 20, FONT_GREEN, 0, comms);

	gfx_drawRect(comms, 5, comms->h - 28, 180, 20, 0x25, 0x00, 0x00);
	gfx_renderString("RETURN TO MISSIONS", 15, comms->h - 25, FONT_WHITE, 1, comms);

	engine.commsSection = 1;
}

static void intermission_doComms(SDL_Surface *comms)
{
	if ((engine.keyState[KEY_FIRE]))
	{
		if (engine.commsSection == 0)
		{
			for (int i = 0 ; i < 4 ; i++)
			{
				if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 170, 180 + (i * 60), 430, 50))
				{
					intermission_createMissionDetailSurface(comms, i);
					engine.keyState[KEY_FIRE] = 0;
				}
			}
		}
		else
		{
			if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 170, 440, 160, 20))
			{
				intermission_createCommsSurface(comms);
				engine.keyState[KEY_FIRE] = 0;
			}
		}
	}
}

static void intermission_createOptions(SDL_Surface *optionsSurface)
{
	SDL_FillRect(optionsSurface, NULL, black);

	gfx_drawRect(optionsSurface, 0, 0, optionsSurface->w - 2, optionsSurface->h - 2, 0x00, 0x00, 0x44);

	gfx_renderString("++ オプション ++", 105, 8, FONT_WHITE, 0, optionsSurface);

	gfx_drawRect(optionsSurface, 190, 45, 50, 22, 0x00, 0x00, 0x00);
	gfx_drawRect(optionsSurface, 250, 45, 50, 22, 0x00, 0x00, 0x00);
	gfx_drawRect(optionsSurface, 20, 45, 150, 22, 0x00, 0x00, 0x00);
	if (engine.useSound)
		gfx_drawRect(optionsSurface, 190, 45, 50, 22, 0xff, 0x00, 0x00);
	else
		gfx_drawRect(optionsSurface, 250, 45, 50, 22, 0xff, 0x00, 0x00);
	gfx_renderString("ON", 207, 50, FONT_WHITE, 0, optionsSurface);
	gfx_renderString("OFF", 263, 50, FONT_WHITE, 0, optionsSurface);
	gfx_renderString("効果音", 30, 50, FONT_WHITE, 0, optionsSurface);

	gfx_drawRect(optionsSurface, 190, 95, 50, 22, 0x00, 0x00, 0x00);
	gfx_drawRect(optionsSurface, 250, 95, 50, 22, 0x00, 0x00, 0x00);
	gfx_drawRect(optionsSurface, 20, 95, 150, 22, 0x00, 0x00, 0x00);
	if (engine.useMusic)
		gfx_drawRect(optionsSurface, 190, 95, 50, 22, 0xff, 0x00, 0x00);
	else
		gfx_drawRect(optionsSurface, 250, 95, 50, 22, 0xff, 0x00, 0x00);
	gfx_renderString("ON", 207, 100, FONT_WHITE, 0, optionsSurface);
	gfx_renderString("OFF", 263, 100, FONT_WHITE, 0, optionsSurface);
	gfx_renderString("音楽", 30, 100, FONT_WHITE, 0, optionsSurface);

 	gfx_drawRect(optionsSurface, 190, 145, 50, 22, 0x00, 0x00, 0x00);
	gfx_drawRect(optionsSurface, 250, 145, 50, 22, 0x00, 0x00, 0x00);
	gfx_drawRect(optionsSurface, 20, 145, 150, 22, 0x00, 0x00, 0x00);
	if (engine.fullScreen)
		gfx_drawRect(optionsSurface, 190, 145, 50, 22, 0xff, 0x00, 0x00);
	else
		gfx_drawRect(optionsSurface, 250, 145, 50, 22, 0xff, 0x00, 0x00);
	gfx_renderString("ON", 207, 150, FONT_WHITE, 0, optionsSurface);
	gfx_renderString("OFF", 263, 150, FONT_WHITE, 0, optionsSurface);
	gfx_renderString("フルスクリーン", 30, 150, FONT_WHITE, 0, optionsSurface);
}

static void intermission_doOptions(SDL_Surface *optionsSurface)
{
	if ((engine.keyState[KEY_FIRE]))
	{
		if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 417, 172, 45, 22))
			engine.useSound = true;

		if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 478, 172, 45, 22))
			engine.useSound = false;


		if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 417, 222, 45, 22))
		{
			engine.useMusic = true;
			audio_playMusic("music/through_space.ogg", -1);
		}

		if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 478, 222, 45, 22))
		{
			engine.useMusic = false;
			audio_haltMusic();
		}

		if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 417, 272, 45, 22))
		{
			if (!engine.fullScreen)
			{
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				engine.fullScreen = true;
			}
		}

		if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 478, 272, 45, 22))
		{
			if (engine.fullScreen)
			{
				SDL_SetWindowFullscreen(window, 0);
				engine.fullScreen = false;
			}
		}

		intermission_createOptions(optionsSurface);
	}
}

/*
Controls the entire intermission screen. This simply draws a background,
stars, gridlines and the icons at the bottom of the screen. Will call
(and continue to call) the specified functions when the player has
selected an icon.
*/
int intermission()
{
	int iconInfoY;

	char string[25];

	SDL_Rect r;
	SDL_Rect destRect;
	int distance = 0;
	double interceptionChance;

	int section = 1;

	float sinX = 300;
	float cosY = 300;
	bool movePlanets = true;
	int saveSlot = -1;

	int rtn = 0;

	bool redrawBackground = true;

	gfx_free();

	checkForBossMission(); // double check just to make sure!

	// Tell the game we are not in a mission so
	// do not perform certain keyboard actions
	engine.gameSection = SECTION_INTERMISSION;

	screen_clear(black);
	renderer_update();
	screen_clear(black);

	initSaveSlots();

	loadBackground(systemBackground[game.system]);

	engine.cursor_x = screen->w / 2;
	engine.cursor_y = screen->h / 2;
	gfx_sprites[SP_CURSOR] = gfx_loadImage("gfx/cursor.png");

	// Icons
	gfx_sprites[SP_START_MISSION] = gfx_loadImage("gfx/icon1.png");
	gfx_sprites[SP_MAP] = gfx_loadImage("gfx/icon2.png");
	gfx_sprites[SP_STATUS] = gfx_loadImage("gfx/icon3.png");
	gfx_sprites[SP_SAVE] = gfx_loadImage("gfx/icon4.png");
	gfx_sprites[SP_SHOP] = gfx_loadImage("gfx/icon5.png");
	gfx_sprites[SP_COMM] = gfx_loadImage("gfx/icon6.png");
	gfx_sprites[SP_OPTIONS] = gfx_loadImage("gfx/icon7.png");
	gfx_sprites[SP_EXIT] = gfx_loadImage("gfx/icon8.png");
	gfx_sprites[SP_PLASMA_MAX_OUTPUT] = gfx_loadImage("gfx/icon9.png");
	gfx_sprites[SP_PLASMA_MAX_POWER] = gfx_loadImage("gfx/icon10.png");
	gfx_sprites[SP_PLASMA_MAX_RATE] = gfx_loadImage("gfx/icon11.png");
	gfx_sprites[SP_PLASMA_AMMO] = gfx_loadImage("gfx/icon12.png");
	gfx_sprites[SP_ROCKET_AMMO] = gfx_loadImage("gfx/icon13.png");
	gfx_sprites[SP_PLASMA_MIN_OUTPUT] = gfx_loadImage("gfx/icon14.png");
	gfx_sprites[SP_PLASMA_MIN_POWER] = gfx_loadImage("gfx/icon15.png");
	gfx_sprites[SP_PLASMA_MIN_RATE] = gfx_loadImage("gfx/icon16.png");
	gfx_sprites[SP_PLASMA_MAX_AMMO] = gfx_loadImage("gfx/icon17.png");
	gfx_sprites[SP_ROCKET_MAX_AMMO] = gfx_loadImage("gfx/icon18.png");
	gfx_sprites[SP_DOUBLE_ROCKETS] = gfx_loadImage("gfx/icon19.png");
	gfx_sprites[SP_MICRO_ROCKETS] = gfx_loadImage("gfx/icon20.png");
	gfx_sprites[SP_LASER] = gfx_loadImage("gfx/icon21.png");
	gfx_sprites[SP_HOMING_MISSILE] = gfx_loadImage("gfx/icon22.png");
	gfx_sprites[SP_CHARGER] = gfx_loadImage("gfx/icon23.png");
	gfx_sprites[SP_DOUBLE_HOMING_MISSILES] = gfx_loadImage("gfx/icon24.png");
	gfx_sprites[SP_MICRO_HOMING_MISSILES] = gfx_loadImage("gfx/icon25.png");
	gfx_sprites[SP_GOTO] = gfx_loadImage("gfx/icon26.png");
	gfx_sprites[SP_BUY] = gfx_loadImage("gfx/buyIcon.png");
	gfx_sprites[SP_SELL] = gfx_loadImage("gfx/sellIcon.png");
	gfx_sprites[SP_FIREFLY] = gfx_loadImage("gfx/firefly1.png");

	// Planets 30 - 39
	gfx_sprites[SP_SUN] = gfx_loadImage("gfx/planet_sun.png");
	gfx_sprites[SP_PLANET_GREEN] = gfx_loadImage("gfx/planet_green.png");
	gfx_sprites[SP_PLANET_BLUE] = gfx_loadImage("gfx/planet_blue.png");
	gfx_sprites[SP_PLANET_RED] = gfx_loadImage("gfx/planet_red.png");
	gfx_sprites[SP_PLANET_ORANGE] = gfx_loadImage("gfx/planet_orange.png");

	// Faces
	gfx_faceSprites[FS_CHRIS] = gfx_loadImage("gfx/face_chris.png");
	gfx_faceSprites[FS_SID] = gfx_loadImage("gfx/face_sid.png");
	gfx_faceSprites[FS_KRASS] = gfx_loadImage("gfx/face_krass.png");
	gfx_faceSprites[FS_PHOEBE] = gfx_loadImage("gfx/face_phoebe.png");
	gfx_faceSprites[FS_URSULA] = gfx_loadImage("gfx/face_ursula.png");
	gfx_faceSprites[FS_KLINE] = gfx_loadImage("gfx/face_kline.png");
	gfx_faceSprites[FS_CREW] = gfx_loadImage("gfx/face_crew.png");

	engine.done = 0;
	engine.keyState[KEY_FIRE] = 0;
	engine.ssx = 0;
	engine.ssy = 0;

	intermission_setStatusLines();
	initShop();
	intermission_setSystemPlanets();

	SDL_Surface *statsSurface = gfx_createAlphaRect(600, 330, 0x00, 0x00, 0x99);
	SDL_Surface *savesSurface = gfx_createSurface(350, 300);
	SDL_Surface *optionsSurface = gfx_createSurface(320, 240);
	SDL_Surface *commsSurface = gfx_createSurface(450, 400);

	createSavesSurface(savesSurface, -1);
	intermission_createOptions(optionsSurface);
	intermission_createCommsSurface(commsSurface);

	// Remove the Supercharge, if it is there
	if ((game.difficulty != DIFFICULTY_EASY) &&
		(game.difficulty != DIFFICULTY_ORIGINAL))
	{
		weapon[W_PLAYER_WEAPON].reload[0] = MAX(
			weapon[W_PLAYER_WEAPON].reload[0],
			rate2reload[game.maxPlasmaRate]);
		weapon[W_PLAYER_WEAPON].ammo[0] = MIN(weapon[W_PLAYER_WEAPON].ammo[0],
			game.maxPlasmaOutput);
		weapon[W_PLAYER_WEAPON].damage = MIN(weapon[W_PLAYER_WEAPON].damage,
			game.maxPlasmaDamage);
	}

	switch (game.system)
	{
		case SYSTEM_SPIRIT:
			if (game.difficulty == DIFFICULTY_ORIGINAL)
				interceptionChance = 0;
			else
				interceptionChance = 1. / 600.;
			break;
		case SYSTEM_EYANANTH:
			interceptionChance = 1. / 300.;
			break;
		case SYSTEM_MORDOR:
			interceptionChance = 1. / 150.;
			break;
		case SYSTEM_SOL:
			// There is no chance of being interceptted after the final attack on Earth
			if ((game.system == SYSTEM_SOL) && (systemPlanet[2].missionCompleted))
				interceptionChance = 0;
			else
				interceptionChance = 1. / 100.;
			break;
		default:
			interceptionChance = 0;
	}

	if ((engine.useAudio) && (engine.useMusic))
		audio_playMusic("music/through_space.ogg", -1);

	sprintf(string, "星系 : %s系", systemNames[game.system]);
	gfx_createTextObject(TS_CURRENT_SYSTEM, string, -1, 15, FONT_WHITE);

	iconInfoY = 545;

	gfx_createTextObject(TS_INFO_START_MISSION, "次の作戦を開始", -1, iconInfoY, FONT_WHITE);
	gfx_createTextObject(TS_INFO_GOTO, "目的の惑星へ向かう", -1, iconInfoY, FONT_WHITE);
	gfx_createTextObject(TS_INFO_MAP, "星系マップ", -1, iconInfoY, FONT_WHITE);
	gfx_createTextObject(TS_INFO_STATUS, "統計", -1, iconInfoY, FONT_WHITE);
	gfx_createTextObject(TS_INFO_SAVE_GAME, "ゲームを保存", -1, iconInfoY, FONT_WHITE);
	gfx_createTextObject(TS_INFO_SHOP, "ファイアフライを強化", -1, iconInfoY, FONT_WHITE);
	gfx_createTextObject(TS_INFO_COMMS, "作戦", -1, iconInfoY, FONT_WHITE);
	gfx_createTextObject(TS_INFO_OPTIONS, "オプション", -1, iconInfoY, FONT_WHITE);
	gfx_createTextObject(TS_INFO_EXIT, "タイトルに戻る", -1, iconInfoY, FONT_WHITE);

	sprintf(string, "現在位置: %s ステーション", systemPlanet[game.stationedPlanet].name);
	gfx_createTextObject(TS_CURRENT_PLANET, string, 90, 450, FONT_WHITE);

	if (game.destinationPlanet > -1)
		sprintf(string, "目的地: %s", systemPlanet[game.destinationPlanet].name);
	else
		strcpy(string, "目的地: なし");
	gfx_createTextObject(TS_DEST_PLANET, string, 550, 450, FONT_WHITE);

	if (game.distanceCovered > 0)
		section = 0;
	else
		player.shield = player.maxShield;

	flushInput();
	engine.keyState[KEY_FIRE] = engine.keyState[KEY_ALTFIRE] = 0;
	engine.done = 0;

	while (!engine.done)
	{
		renderer_update();

		if (redrawBackground)
		{
			screen_drawBackground();
			redrawBackground = false;
		}
		else
		{
			screen_unBuffer();
		}

  		game_doStars();

		r.x = 0;
		r.y = 0;
		r.h = screen->h;
		r.w = 1;
		for (int i = 40 ; i < screen->w ; i+= 40)
		{
			r.x = i;
			SDL_FillRect(screen, &r, darkerBlue);
		}

		r.x = 0;
		r.y = 0;
		r.h = 1;
		r.w = screen->w;
		for (int i = 40 ; i < screen->h ; i+= 40)
		{
			r.y = i;
			SDL_FillRect(screen, &r, darkerBlue);
		}


		if (rand() % 1000 < 2)
		{
			engine.ssx = RANDRANGE(-100, 100);
			engine.ssy = RANDRANGE(-100, 100);
			engine.ssx /= 100;
			engine.ssy /= 100;
		}

		screen_blitText(TS_CURRENT_SYSTEM);

		switch(section)
		{
			case 0:
				if (game.stationedPlanet == game.destinationPlanet)
				{
					game.area = systemPlanet[game.stationedPlanet].missionNumber;
					rtn = 2;
					engine.done = 1;
				}
				else
				{
					distance = abs(game.stationedPlanet - game.destinationPlanet);
					if (interceptionChance <= 0)
						distance = 1;

					distance = (5 / distance);
					if (distance < 1)
						distance = 1;

					gfx_createTextObject(TS_CURRENT_PLANET, systemPlanet[game.stationedPlanet].name,
						135, 480, FONT_WHITE);
					gfx_createTextObject(TS_DEST_PLANET, systemPlanet[game.destinationPlanet].name,
						635, 480, FONT_WHITE);

					section = 8;

					destRect.x = 180;
					destRect.y = 450;
					destRect.w = 1;
					if (game.distanceCovered > 0)
						destRect.w = game.distanceCovered;
					destRect.h = 20;
				}
				break;

			case 1:
				if (engine.keyState[KEY_ALTFIRE])
				{
					movePlanets = !movePlanets;
					engine.keyState[KEY_ALTFIRE] = 0;
				}

				if (movePlanets)
				{
					sinX += 0.01;
					cosY += 0.01;
				}

				if (intermission_showSystem(sinX, cosY, true))
				{
					sprintf(string, "目的地: %s", systemPlanet[game.destinationPlanet].name);
					gfx_createTextObject(TS_DEST_PLANET, string, 550, 450, FONT_WHITE);
				}

				screen_blitText(TS_CURRENT_PLANET);
				if (game.stationedPlanet != game.destinationPlanet)
					screen_blitText(TS_DEST_PLANET);
				break;

			case 2:
				intermission_showStatus(statsSurface);
				break;

			case 3:
				screen_blit(savesSurface, 200, 100);
				saveSlot = showSaveSlots(savesSurface, saveSlot);
				break;

			case 4:
				showShop();
				break;

			case 5:
				screen_blit(commsSurface, 170, 70);
				intermission_doComms(commsSurface);
				break;

			case 6:
				screen_blit(optionsSurface, 230, 130);
				intermission_doOptions(optionsSurface);
				break;

			case 7:
				rtn = 0;
				engine.done = 1;
				break;

			case 8:
				intermission_showSystem(sinX, cosY, false);

				screen_blit(systemPlanet[game.stationedPlanet].image, 150, 450);
				screen_blitText(TS_CURRENT_PLANET);
				screen_blit(systemPlanet[game.destinationPlanet].image, 650, 450);
				screen_blitText(TS_DEST_PLANET);

				destRect.w += distance;
				SDL_FillRect(screen, &destRect, red);

				if (destRect.w >= 450)
				{
					game.stationedPlanet = game.destinationPlanet;
					game.distanceCovered = 0;
					player.shield = player.maxShield;
					sprintf(string, "現在位置: %s ステーション",
						systemPlanet[game.stationedPlanet].name);
					strcpy(game.stationedName,
						systemPlanet[game.stationedPlanet].name);
					gfx_createTextObject(TS_CURRENT_PLANET, string, 90, 450, FONT_WHITE);
					intermission_updateCommsSurface(commsSurface);
					section = 1;
					redrawBackground = true;
					saveGame(0);
				}
				else if (interceptionChance > 0)
				{
					if (CHANCE(interceptionChance))
					{
						game.area = MISN_INTERCEPTION;
						rtn = 2;
						engine.done = 1;
						game.distanceCovered = destRect.w;
					}
				}

				break;
		}

		screen_addBuffer(300, 545, 200, 15);

		if (section != 8)
		{
			if ((game.stationedPlanet == game.destinationPlanet) &&
					(!systemPlanet[game.stationedPlanet].missionCompleted))
				screen_blit(gfx_sprites[SP_START_MISSION], 80, 500);
			else if (game.stationedPlanet != game.destinationPlanet)
				screen_blit(gfx_sprites[SP_GOTO], 80, 500);

			screen_blit(gfx_sprites[SP_MAP], 170, 500);
			screen_blit(gfx_sprites[SP_STATUS], 260, 500);
			screen_blit(gfx_sprites[SP_SAVE], 350, 500);
			screen_blit(gfx_sprites[SP_SHOP], 440, 500);
			screen_blit(gfx_sprites[SP_COMM], 530, 500);
			screen_blit(gfx_sprites[SP_OPTIONS], 620, 500);
			screen_blit(gfx_sprites[SP_EXIT], 710, 500);

			if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 80, 500, 32, 32) &&
					((game.stationedPlanet != game.destinationPlanet) ||
						(!systemPlanet[game.stationedPlanet].missionCompleted)))
			{
				if (game.stationedPlanet == game.destinationPlanet)
					screen_blitText(TS_INFO_START_MISSION);
				else
					screen_blitText(TS_INFO_GOTO);

				if ((engine.keyState[KEY_FIRE]))
				{
					redrawBackground = true;
					section = 0;
					engine.keyState[KEY_FIRE] = 0;
				}
			}
			else if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 170, 500, 32, 32))
			{
				screen_blitText(TS_INFO_MAP);

				if ((engine.keyState[KEY_FIRE]))
				{
					redrawBackground = true;
					section = 1;
					engine.keyState[KEY_FIRE] = 0;
				}
			}
			else if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 260, 500, 32, 32))
			{
				screen_blitText(TS_INFO_STATUS);

				if ((engine.keyState[KEY_FIRE]))
				{
					redrawBackground = true;
					section = 2;
					engine.keyState[KEY_FIRE] = 0;
				}
			}
			else if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 350, 500, 32, 32))
			{
				screen_blitText(TS_INFO_SAVE_GAME);

				if ((engine.keyState[KEY_FIRE]))
				{
					redrawBackground = true;
					section = 3;
					engine.keyState[KEY_FIRE] = 0;
				}
			}
			else if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 440, 500, 32, 32))
			{
				screen_blitText(TS_INFO_SHOP);

				if ((engine.keyState[KEY_FIRE]))
				{
					redrawBackground = true;
					section = 4;
					engine.keyState[KEY_FIRE] = 0;
				}
			}
			else if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 530, 500, 32, 32))
			{
				screen_blitText(TS_INFO_COMMS);

				if ((engine.keyState[KEY_FIRE]))
				{
					redrawBackground = true;
					section = 5;
					engine.keyState[KEY_FIRE] = 0;
				}
			}
			else if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 620, 500, 32, 32))
			{
				screen_blitText(TS_INFO_OPTIONS);

				if ((engine.keyState[KEY_FIRE]))
				{
					redrawBackground = true;
					section = 6;
					engine.keyState[KEY_FIRE] = 0;
				}
			}
			else if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 710, 500, 32, 32))
			{
				screen_blitText(TS_INFO_EXIT);

				if ((engine.keyState[KEY_FIRE]))
				{
					redrawBackground = true;
					section = 7;
					engine.keyState[KEY_FIRE] = 0;
				}
			}
		}

		engine.keyState[KEY_FIRE] = 0;
		engine.keyState[KEY_ALTFIRE] = 0;
		intermission_doCursor();

		game_delayFrame();
	}

	audio_haltMusic();
	SDL_FreeSurface(statsSurface);
	SDL_FreeSurface(savesSurface);
	SDL_FreeSurface(optionsSurface);
	SDL_FreeSurface(commsSurface);

	if (game.distanceCovered == 0)
		player.shield = player.maxShield;

	return rtn;
}

