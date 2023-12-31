/*
Copyright (C) 2003 Parallel Realities
Copyright (C) 2011, 2012, 2013 Guus Sliepen
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

static ShopItem shopItems[SHOP_MAX];
static int shopSelectedItem;

static void sell(int i);

static void drawSecondaryWeaponSurface()
{
	char description[50] = "";

	gfx_renderString("副装備", 10, 3, FONT_WHITE, 0, gfx_shopSprites[2]);

	switch (player.weaponType[1])
	{
		case W_NONE:
			strcpy(description, "タイプ：なし");
			break;
		case W_ROCKETS:
			strcpy(description, "タイプ：ロケット");
			break;
		case W_DOUBLE_ROCKETS:
			strcpy(description, "タイプ：ダブル ロケット");
			break;
		case W_MICRO_ROCKETS:
			strcpy(description, "タイプ：マイクロ ロケット");
			break;
		case W_LASER:
			strcpy(description, "タイプ：レーザー");
			break;
		case W_CHARGER:
			strcpy(description, "タイプ：チャージ");
			break;
		case W_HOMING_MISSILE:
			strcpy(description, "タイプ：ホーミング ミサイル");
			break;
		case W_DOUBLE_HOMING_MISSILES:
			strcpy(description, "タイプ：ダブル ホーミング ミサイル");
			break;
		case W_MICRO_HOMING_MISSILES:
			strcpy(description, "タイプ：マイクロ ホーミング ミサイル");
			break;
	}
	gfx_renderString(description, 10, 22, FONT_WHITE, 0, gfx_shopSprites[2]);

	if ((player.weaponType[1] != W_LASER) &&
		(player.weaponType[1] != W_CHARGER) && (player.weaponType[1] != W_NONE))
	{
		sprintf(description, "最大数：%d", game.maxRocketAmmo);
		gfx_renderString(description, 10, 37, FONT_WHITE, 0, gfx_shopSprites[2]);
	}
}

static void adjustShopPrices()
{
	if (game.difficulty == DIFFICULTY_ORIGINAL)
	{
		shopItems[SHOP_PLASMA_MAX_OUTPUT].price = 500 * game.maxPlasmaOutput;
		shopItems[SHOP_PLASMA_MAX_DAMAGE].price = 500 * game.maxPlasmaDamage;
		shopItems[SHOP_PLASMA_MAX_RATE].price = 500 * (game.maxPlasmaRate * 2 - 1);

		shopItems[SHOP_PLASMA_MIN_OUTPUT].price = 2000 * game.minPlasmaOutput;
		shopItems[SHOP_PLASMA_MIN_DAMAGE].price = 2000 * game.minPlasmaDamage;
		shopItems[SHOP_PLASMA_MIN_RATE].price = 2000 * (game.minPlasmaRate * 2 - 1);
	}
	else
	{
		shopItems[SHOP_PLASMA_MAX_OUTPUT].price = 1000 * (game.maxPlasmaOutput + 1);
		shopItems[SHOP_PLASMA_MAX_DAMAGE].price = 1000 * (game.maxPlasmaDamage + 1);
		shopItems[SHOP_PLASMA_MAX_RATE].price = 1000 * (game.maxPlasmaRate + 1);

		shopItems[SHOP_PLASMA_MIN_OUTPUT].price = 1500 * (game.minPlasmaOutput + 1);
		shopItems[SHOP_PLASMA_MIN_DAMAGE].price = 1500 * (game.minPlasmaDamage + 1);
		shopItems[SHOP_PLASMA_MIN_RATE].price = 1500 * (game.minPlasmaRate + 1);
	}

	if (game.maxPlasmaOutput <= game.minPlasmaOutput)
		shopItems[SHOP_PLASMA_MIN_OUTPUT].price += shopItems[SHOP_PLASMA_MAX_OUTPUT].price;
	if (game.maxPlasmaDamage <= game.minPlasmaDamage)
		shopItems[SHOP_PLASMA_MIN_DAMAGE].price += shopItems[SHOP_PLASMA_MAX_DAMAGE].price;
	if (game.maxPlasmaRate <= game.minPlasmaRate)
		shopItems[SHOP_PLASMA_MIN_RATE].price += shopItems[SHOP_PLASMA_MAX_RATE].price;

	shopItems[SHOP_PLASMA_MAX_AMMO].price = (10 * (game.maxPlasmaAmmo - 75));
	shopItems[SHOP_ROCKET_MAX_AMMO].price = (25 * game.maxRocketAmmo);
	
	if (game.maxPlasmaOutput >= game.maxPlasmaOutputLimit)
		shopItems[SHOP_PLASMA_MAX_OUTPUT].price = 0;

	if (game.maxPlasmaDamage >= game.maxPlasmaDamageLimit)
		shopItems[SHOP_PLASMA_MAX_DAMAGE].price = 0;

	if (game.maxPlasmaRate >= game.maxPlasmaRateLimit)
		shopItems[SHOP_PLASMA_MAX_RATE].price = 0;
		
	if (game.minPlasmaOutput >= game.minPlasmaOutputLimit)
		shopItems[SHOP_PLASMA_MIN_OUTPUT].price = 0;

	if (game.minPlasmaDamage >= game.minPlasmaDamageLimit)
		shopItems[SHOP_PLASMA_MIN_DAMAGE].price = 0;

	if (game.minPlasmaRate >= game.minPlasmaRateLimit)
		shopItems[SHOP_PLASMA_MIN_RATE].price = 0;

	if (game.maxPlasmaAmmo >= game.maxPlasmaAmmoLimit)
		shopItems[SHOP_PLASMA_MAX_AMMO].price = 0;

	if (game.maxRocketAmmo >= game.maxRocketAmmoLimit)
		shopItems[SHOP_ROCKET_MAX_AMMO].price = 0;
}

static void drawShop()
{
	char description[100];
	int icons = SHOP_MAX;

	adjustShopPrices();

	for (int i = 0 ; i < SHOP_S_MAX ; i++)
	{
		if (gfx_shopSprites[i] != NULL)
		{
			SDL_FreeSurface(gfx_shopSprites[i]);
		}
	}

	gfx_shopSprites[SHOP_S_PRIMARY] = gfx_createSurface(246, 91);
	gfx_shopSprites[SHOP_S_POWERUP] = gfx_createSurface(246, 91);
	gfx_shopSprites[SHOP_S_SECONDARY] = gfx_createSurface(246, 91);

	gfx_drawRect(gfx_shopSprites[SHOP_S_PRIMARY], 0, 0, 245, 90, 0x00, 0x00, 0x55);
	gfx_drawRect(gfx_shopSprites[SHOP_S_PRIMARY], 0, 0, 245, 20, 0x00, 0x00, 0x99);
	gfx_drawRect(gfx_shopSprites[SHOP_S_POWERUP], 0, 0, 245, 90, 0x00, 0x00, 0x55);
	gfx_drawRect(gfx_shopSprites[SHOP_S_POWERUP], 0, 0, 245, 20, 0x00, 0x00, 0x99);
	gfx_drawRect(gfx_shopSprites[SHOP_S_SECONDARY], 0, 0, 245, 90, 0x00, 0x00, 0x55);
	gfx_drawRect(gfx_shopSprites[SHOP_S_SECONDARY], 0, 0, 245, 20, 0x00, 0x00, 0x99);

	gfx_shopSprites[SHOP_S_SHIP_INFO] = gfx_createAlphaRect(601, 101, 0x00, 0x00, 0x00);
	gfx_drawRect(gfx_shopSprites[SHOP_S_SHIP_INFO], 0, 0, 600, 100, 0x00, 0x00, 0x33);

	switch (shopSelectedItem)
	{
		case -1:
		case -2:
		case -3:
		case -4:
		case -5:
		case -6:
			break;
		case SHOP_PLASMA_MAX_OUTPUT:
		case SHOP_PLASMA_MAX_DAMAGE:
		case SHOP_PLASMA_MAX_RATE:
		case SHOP_PLASMA_MAX_AMMO:
			gfx_drawRect(gfx_shopSprites[SHOP_S_POWERUP], 0, 0, 245, 90, 0x55, 0x00, 0x00);
			gfx_drawRect(gfx_shopSprites[SHOP_S_POWERUP], 0, 0, 245, 20, 0x99, 0x00, 0x00);
			break;
		case SHOP_PLASMA_MIN_OUTPUT:
		case SHOP_PLASMA_MIN_DAMAGE:
		case SHOP_PLASMA_MIN_RATE:
			gfx_drawRect(gfx_shopSprites[SHOP_S_PRIMARY], 0, 0, 245, 90, 0x55, 0x00, 0x00);
			gfx_drawRect(gfx_shopSprites[SHOP_S_PRIMARY], 0, 0, 245, 20, 0x99, 0x00, 0x00);
			break;
		case SHOP_PLASMA_AMMO:
		case SHOP_ROCKET_AMMO:
			gfx_drawRect(gfx_shopSprites[SHOP_S_SHIP_INFO], 0, 0, 600, 100, 0x33, 0x00, 0x00);
			break;
		default:
			gfx_drawRect(gfx_shopSprites[SHOP_S_SECONDARY], 0, 0, 245, 90, 0x55, 0x00, 0x00);
			gfx_drawRect(gfx_shopSprites[SHOP_S_SECONDARY], 0, 0, 245, 20, 0x99, 0x00, 0x00);
			break;
	}

	strcpy(description, "");

	gfx_renderString("主装備", 10, 3, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_PRIMARY]);
	sprintf(description, "プラズマカノン：%d門", game.minPlasmaOutput);
	gfx_renderString(description, 10, 22, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_PRIMARY]);
	sprintf(description, "プラズマ火力　：レベル %d",
		game.minPlasmaDamage);
	gfx_renderString(description, 10, 37, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_PRIMARY]);
	sprintf(description, "冷却装置　　　：レベル %d",
		game.minPlasmaRate);
	gfx_renderString(description, 10, 52, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_PRIMARY]);

	gfx_renderString("一時強化", 10, 3, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_POWERUP]);
	sprintf(description, "プラズマ出力範囲　：レベル %d",
		game.maxPlasmaOutput);
	gfx_renderString(description, 10, 22, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_POWERUP]);
	sprintf(description, "プラズマコンデンサ：レベル %d",
		game.maxPlasmaDamage);
	gfx_renderString(description, 10, 37, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_POWERUP]);
	sprintf(description, "液体窒素　　　　　：レベル %d",
		game.maxPlasmaRate);
	gfx_renderString(description, 10, 52, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_POWERUP]);
	sprintf(description, "プラズマ容量　　　：%d", game.maxPlasmaAmmo);
	gfx_renderString(description, 10, 67, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_POWERUP]);

	drawSecondaryWeaponSurface();

	gfx_shopSprites[SHOP_S_CATALOG] = gfx_createSurface(601, 121);

	gfx_drawRect(gfx_shopSprites[SHOP_S_CATALOG], 0, 0, 600, 120, 0x00, 0x00, 0x22);

	gfx_renderString("一時強化", 10, 2, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_CATALOG]);
	gfx_renderString("爆薬とコンテナ", 260, 2, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_CATALOG]);

	gfx_renderString("主装備", 10, 62, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_CATALOG]);

	gfx_renderString("副装備", 260, 62, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_CATALOG]);

	if (game.system == 0)
		icons = SHOP_DOUBLE_ROCKETS + 1;
	else if (game.system == 1)
		icons = SHOP_LASER + 1;
	else if (game.system == 2)
		icons = SHOP_CHARGER + 1;

	for (int i = 0 ; i < icons ; i++)
	{
		gfx_blit(gfx_sprites[shopItems[i].image], shopItems[i].x - 90,
			shopItems[i].y - 178, gfx_shopSprites[SHOP_S_CATALOG]);
	}

	sprintf(description, "ｼｰﾙﾄﾞ ﾕﾆｯﾄ : %d", player.maxShield);
	gfx_renderString(description, 10, 4, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_SHIP_INFO]);
	sprintf(description, "ｷｬｯｼｭ : $%d", game.cash);
	gfx_renderString(description, 10, 80, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_SHIP_INFO]);
	sprintf(description, "ﾌﾟﾗｽﾞﾏ ｾﾙ : %.3d", player.ammo[0]);
	gfx_renderString(description, 430, 4, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_SHIP_INFO]);
	sprintf(description, "ﾛｹｯﾄ : %.3d", player.ammo[1]);
	gfx_renderString(description, 475, 80, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_SHIP_INFO]);

	gfx_shopSprites[SHOP_S_ITEM_INFO] = gfx_createSurface(601, 56);
	gfx_drawRect(gfx_shopSprites[SHOP_S_ITEM_INFO], 0, 0, 600, 35, 0x00, 0x99, 0x00);
	gfx_drawRect(gfx_shopSprites[SHOP_S_ITEM_INFO], 0, 20, 600, 35, 0x00, 0x33, 0x00);
	gfx_renderString("説明", 5, 4, FONT_WHITE, 0, gfx_shopSprites[5]);

	switch (shopSelectedItem)
	{
		case -1:
			break;
		case -2:
			gfx_renderString("代金が足りない", 20, 30, FONT_WHITE,
				0, gfx_shopSprites[SHOP_S_ITEM_INFO]);
			break;
		case -3:
			gfx_renderString("強化できない", 5, 22, FONT_WHITE, 0,
				gfx_shopSprites[SHOP_S_ITEM_INFO]);
			gfx_renderString("機体の限界に達している", 20, 38,
				FONT_CYAN, 0, gfx_shopSprites[SHOP_S_ITEM_INFO]);
			break;
		case -4:
			gfx_renderString("容量の限界に達している", 20, 30, FONT_WHITE, 0,
				gfx_shopSprites[SHOP_S_ITEM_INFO]);
			break;
		case -5:
			gfx_renderString("それは売却できない", 20, 30, FONT_WHITE,
				0, gfx_shopSprites[SHOP_S_ITEM_INFO]);
			break;
		case -6:
			gfx_renderString("売却できるものがない", 20, 30, FONT_WHITE, 0,
				gfx_shopSprites[SHOP_S_ITEM_INFO]);
			break;
		case -7:
			gfx_renderString("レーザーまたはチャージの場合はロケット弾を購入できない",
				5, 30, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_ITEM_INFO]);
			break;
		case -8:
			gfx_renderString("既に装備している", 20, 30,
				FONT_WHITE, 0, gfx_shopSprites[SHOP_S_ITEM_INFO]);
			break;
		case -9:
			gfx_renderString("その装備の限界に達している", 20,
				30, FONT_WHITE, 0, gfx_shopSprites[SHOP_S_ITEM_INFO]);
			break;
		default:
			if (shopItems[shopSelectedItem].price != 0)
			{
				sprintf(description, "%s ($%d)",
					shopItems[shopSelectedItem].description,
					shopItems[shopSelectedItem].price);
			}
			else
			{
				sprintf(description, "%s (N/A)",
					shopItems[shopSelectedItem].description);
			}
			gfx_renderString(shopItems[shopSelectedItem].name, 5, 22,
				FONT_WHITE, 0, gfx_shopSprites[SHOP_S_ITEM_INFO]);
			gfx_renderString(description, 20, 38, FONT_CYAN, 0, gfx_shopSprites[SHOP_S_ITEM_INFO]);
			break;
	}
}

void initShop()
{
	/* ----------- Temporary Items ----------- */

	shopItems[SHOP_PLASMA_MAX_OUTPUT].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MAX_OUTPUT].name, "プラズマチャネル スプリッター");
	strcpy(shopItems[SHOP_PLASMA_MAX_OUTPUT].description,
		"一時強化時のプラズマ出力範囲が増加する");
	shopItems[SHOP_PLASMA_MAX_OUTPUT].image = 9;

	shopItems[SHOP_PLASMA_MAX_DAMAGE].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MAX_DAMAGE].name, "プラズマキャパシティー コンデンサー");
	strcpy(shopItems[SHOP_PLASMA_MAX_DAMAGE].description,
		"一時強化時のプラズマ火力が増加する");
	shopItems[SHOP_PLASMA_MAX_DAMAGE].image = 10;

	shopItems[SHOP_PLASMA_MAX_RATE].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MAX_RATE].name, "液体窒素カプセル");
	strcpy(shopItems[SHOP_PLASMA_MAX_RATE].description,
		"一時強化時の連射速度が増加する");
	shopItems[SHOP_PLASMA_MAX_RATE].image = 11;

	if (game.difficulty == DIFFICULTY_ORIGINAL)
		shopItems[SHOP_PLASMA_AMMO].price = 50;
	else
		shopItems[SHOP_PLASMA_AMMO].price = 1;

	strcpy(shopItems[SHOP_PLASMA_AMMO].name, "10 プラズマセル");
	strcpy(shopItems[SHOP_PLASMA_AMMO].description, "プラズマ火薬");
	shopItems[SHOP_PLASMA_AMMO].image = 12;

	if (game.difficulty == DIFFICULTY_ORIGINAL)
		shopItems[SHOP_ROCKET_AMMO].price = 50;
	else
		shopItems[SHOP_ROCKET_AMMO].price = 1;

	strcpy(shopItems[SHOP_ROCKET_AMMO].name, "ロケット弾");
	strcpy(shopItems[SHOP_ROCKET_AMMO].description,
		"無誘導高速ロケット弾");
	shopItems[SHOP_ROCKET_AMMO].image = 13;

	/* ----------- Permanent Items ----------- */

	shopItems[SHOP_PLASMA_MIN_OUTPUT].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MIN_OUTPUT].name, "プラズマカノンの増設");
	strcpy(shopItems[SHOP_PLASMA_MIN_OUTPUT].description,
		"ファイアフライに新たなプラズマカノン砲を増設する");
	shopItems[SHOP_PLASMA_MIN_OUTPUT].image = 14;

	shopItems[SHOP_PLASMA_MIN_DAMAGE].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MIN_DAMAGE].name, "プラズマパワーブースター");
	strcpy(shopItems[SHOP_PLASMA_MIN_DAMAGE].description,
		"プラズマ弾の火力を増大させる");
	shopItems[SHOP_PLASMA_MIN_DAMAGE].image = 15;

	shopItems[SHOP_PLASMA_MIN_RATE].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MIN_RATE].name, "プラズマ冷却ブースター");
	strcpy(shopItems[SHOP_PLASMA_MIN_RATE].description,
		"連射速度を増大させる");
	shopItems[SHOP_PLASMA_MIN_RATE].image = 16;

	/* ----------- Ammo Items -------------- */

	shopItems[SHOP_PLASMA_MAX_AMMO].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MAX_AMMO].name, "プラズマコンデンサー");
	strcpy(shopItems[SHOP_PLASMA_MAX_AMMO].description,
		"プラズマ火薬の容量が増加する");
	shopItems[SHOP_PLASMA_MAX_AMMO].image = 17;

	shopItems[SHOP_ROCKET_MAX_AMMO].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_ROCKET_MAX_AMMO].name, "ロケットポッド");
	strcpy(shopItems[SHOP_ROCKET_MAX_AMMO].description,
		"装備できるロケット弾が 5本 増加する");
	shopItems[SHOP_ROCKET_MAX_AMMO].image = 18;

	/* ---------- Weaponary --------------- */

	shopItems[SHOP_DOUBLE_ROCKETS].price = 2000;
	strcpy(shopItems[SHOP_DOUBLE_ROCKETS].name, "2連装ロケットランチャー");
	strcpy(shopItems[SHOP_DOUBLE_ROCKETS].description,
		"一度に2本のロケット弾を発射する");
	shopItems[SHOP_DOUBLE_ROCKETS].image = 19;

	shopItems[SHOP_MICRO_ROCKETS].price = 2500;
	strcpy(shopItems[SHOP_MICRO_ROCKETS].name, "マイクロ ロケットランチャー");
	strcpy(shopItems[SHOP_MICRO_ROCKETS].description,
		"多数の小型ロケット弾を一度に発射する");
	shopItems[SHOP_MICRO_ROCKETS].image = 20;

	shopItems[SHOP_LASER].price = 5000;
	strcpy(shopItems[SHOP_LASER].name, "レーザーカノン");
	strcpy(shopItems[SHOP_LASER].description, "レーザーカノン");
	shopItems[SHOP_LASER].image = 21;

	shopItems[SHOP_HOMING_MISSILE].price = 7500;
	strcpy(shopItems[SHOP_HOMING_MISSILE].name, "誘導ミサイルランチャー");
	sprintf(shopItems[SHOP_HOMING_MISSILE].description,
		"誘導ミサイルを発射する (最大 %i本)", maxHoming);
	shopItems[SHOP_HOMING_MISSILE].image = 22;

	shopItems[SHOP_CHARGER].price = 10000;
	strcpy(shopItems[SHOP_CHARGER].name, "チャージカノン");
	strcpy(shopItems[SHOP_CHARGER].description, "エネルギーを充填するカノン砲");
	shopItems[SHOP_CHARGER].image = 23;

	shopItems[SHOP_DOUBLE_HOMING_MISSILES].price = 10000;
	strcpy(shopItems[SHOP_DOUBLE_HOMING_MISSILES].name,
		"2連装誘導ミサイルランチャー");
	sprintf(shopItems[SHOP_DOUBLE_HOMING_MISSILES].description,
		"一度に2発の誘導ミサイルを発射する (最大 %i本)", maxDoubleHoming);
	shopItems[SHOP_DOUBLE_HOMING_MISSILES].image = 24;

	shopItems[SHOP_MICRO_HOMING_MISSILES].price = 15000;
	strcpy(shopItems[SHOP_MICRO_HOMING_MISSILES].name,
		"誘導マイクロミサイルランチャー");
	sprintf(shopItems[SHOP_MICRO_HOMING_MISSILES].description,
		"多数の小型誘導ミサイルを一度に発射する (最大 %i本)", maxMicroHoming);
	shopItems[SHOP_MICRO_HOMING_MISSILES].image = 25;

	shopItems[SHOP_PLASMA_MAX_OUTPUT].x = 100;
	shopItems[SHOP_PLASMA_MAX_OUTPUT].y = 200;

	shopItems[SHOP_PLASMA_MAX_DAMAGE].x = 150;
	shopItems[SHOP_PLASMA_MAX_DAMAGE].y = 200;

	shopItems[SHOP_PLASMA_MAX_RATE].x = 200;
	shopItems[SHOP_PLASMA_MAX_RATE].y = 200;

	shopItems[SHOP_PLASMA_MIN_OUTPUT].x = 100;
	shopItems[SHOP_PLASMA_MIN_OUTPUT].y = 260;

	shopItems[SHOP_PLASMA_MIN_DAMAGE].x = 150;
	shopItems[SHOP_PLASMA_MIN_DAMAGE].y = 260;

	shopItems[SHOP_PLASMA_MIN_RATE].x = 200;
	shopItems[SHOP_PLASMA_MIN_RATE].y = 260;

	shopItems[SHOP_PLASMA_AMMO].x = 350;
	shopItems[SHOP_PLASMA_AMMO].y = 200;

	shopItems[SHOP_ROCKET_AMMO].x = 400;
	shopItems[SHOP_ROCKET_AMMO].y = 200;

	shopItems[SHOP_PLASMA_MAX_AMMO].x = 450;
	shopItems[SHOP_PLASMA_MAX_AMMO].y = 200;

	shopItems[SHOP_ROCKET_MAX_AMMO].x = 500;
	shopItems[SHOP_ROCKET_MAX_AMMO].y = 200;

	shopItems[SHOP_DOUBLE_ROCKETS].x = 350;
	shopItems[SHOP_DOUBLE_ROCKETS].y = 260;

	shopItems[SHOP_MICRO_ROCKETS].x = 400;
	shopItems[SHOP_MICRO_ROCKETS].y = 260;

	shopItems[SHOP_LASER].x = 450;
	shopItems[SHOP_LASER].y = 260;

	shopItems[SHOP_HOMING_MISSILE].x = 500;
	shopItems[SHOP_HOMING_MISSILE].y = 260;

	shopItems[SHOP_CHARGER].x = 550;
	shopItems[SHOP_CHARGER].y = 260;

	shopItems[SHOP_DOUBLE_HOMING_MISSILES].x = 600;
	shopItems[SHOP_DOUBLE_HOMING_MISSILES].y = 260;

	shopItems[SHOP_MICRO_HOMING_MISSILES].x = 650;
	shopItems[SHOP_MICRO_HOMING_MISSILES].y = 260;

 	shopSelectedItem = -1;

	drawShop();
}

static void shop_sellSecondaryWeapon()
{
	switch (player.weaponType[1])
	{
		case W_DOUBLE_ROCKETS:
			sell(SHOP_DOUBLE_ROCKETS);
			break;

		case W_MICRO_ROCKETS:
			sell(SHOP_MICRO_ROCKETS);
			break;

		case W_LASER:
			sell(SHOP_LASER);
			break;

		case W_HOMING_MISSILE:
			sell(SHOP_HOMING_MISSILE);
			break;

		case W_CHARGER:
			sell(SHOP_CHARGER);
			break;

		case W_DOUBLE_HOMING_MISSILES:
			sell(SHOP_DOUBLE_HOMING_MISSILES);
			break;

		case W_MICRO_HOMING_MISSILES:
			sell(SHOP_MICRO_HOMING_MISSILES);
			break;
	}
}

static void buy(int i)
{
	if ((game.cash < shopItems[i].price) && (!engine.cheatCash))
	{
		shopSelectedItem = -2;
		drawShop();
		return;
	}

	switch (i)
	{
		case SHOP_PLASMA_MAX_OUTPUT:
			if (game.maxPlasmaOutput >= game.maxPlasmaOutputLimit)
			{
				shopSelectedItem = -3;
				return;
			}
			game.maxPlasmaOutput++;
			break;

		case SHOP_PLASMA_MAX_DAMAGE:
			if (game.maxPlasmaDamage >= game.maxPlasmaDamageLimit)
			{
				shopSelectedItem = -3;
				return;
			}
			game.maxPlasmaDamage++;
			break;

		case SHOP_PLASMA_MAX_RATE:
			if (game.maxPlasmaRate >= game.maxPlasmaRateLimit)
			{
				shopSelectedItem = -3;
				return;
			}
			game.maxPlasmaRate++;
			break;

		case SHOP_PLASMA_AMMO:
			if (player.ammo[0] >= game.maxPlasmaAmmo)
			{
				shopSelectedItem = -4;
				return;
			}
			LIMIT_ADD(player.ammo[0], 10, 0, game.maxPlasmaAmmo);
			break;

		case SHOP_ROCKET_AMMO:
			if ((player.weaponType[1] == W_CHARGER) ||
				(player.weaponType[1] == W_LASER))
			{
				shopSelectedItem = -7;
				return;
			}
			if (player.ammo[1] == game.maxRocketAmmo)
			{
				shopSelectedItem = -4;
				return;
			}
			if ((player.weaponType[1] == W_HOMING_MISSILE) &&
				(player.ammo[1] >= maxHoming))
			{
				shopSelectedItem = -9;
				return;
			}
			if ((player.weaponType[1] == W_DOUBLE_HOMING_MISSILES) &&
				(player.ammo[1] >= maxDoubleHoming))
			{
				shopSelectedItem = -9;
				return;
			}
			if ((player.weaponType[1] == W_MICRO_HOMING_MISSILES) &&
				(player.ammo[1] >= maxMicroHoming))
			{
				shopSelectedItem = -9;
				return;
			}
			player.ammo[1]++;
			break;

		case SHOP_PLASMA_MIN_OUTPUT:
			if (game.minPlasmaOutput >= game.minPlasmaOutputLimit)
			{
				shopSelectedItem = -3;
				return;
			}
			game.minPlasmaOutput++;
			if (game.maxPlasmaOutput < game.minPlasmaOutput)
				game.maxPlasmaOutput = game.minPlasmaOutput;
			break;

		case SHOP_PLASMA_MIN_DAMAGE:
			if (game.minPlasmaDamage >= game.minPlasmaDamageLimit)
			{
				shopSelectedItem = -3;
				return;
			}
			game.minPlasmaDamage++;
			if (game.maxPlasmaDamage < game.minPlasmaDamage)
				game.maxPlasmaDamage = game.minPlasmaDamage;
			break;

		case SHOP_PLASMA_MIN_RATE:
			if (game.minPlasmaRate >= game.minPlasmaRateLimit)
			{
				shopSelectedItem = -3;
				return;
			}
			game.minPlasmaRate++;
			if (game.maxPlasmaRate < game.minPlasmaRate)
				game.maxPlasmaRate = game.minPlasmaRate;
			break;

		case SHOP_PLASMA_MAX_AMMO:
			if (game.maxPlasmaAmmo >= game.maxPlasmaAmmoLimit)
			{
				shopSelectedItem = -3;
				return;
			}
			game.maxPlasmaAmmo += 25;
			break;

		case SHOP_ROCKET_MAX_AMMO:
			if ((player.weaponType[1] == W_CHARGER) ||
				(player.weaponType[1] == W_LASER))
			{
				shopSelectedItem = -7;
				return;
			}
			if ((player.weaponType[1] == W_HOMING_MISSILE) &&
				(game.maxRocketAmmo >= maxHoming))
			{
				shopSelectedItem = -9;
				return;
			}
			if ((player.weaponType[1] == W_DOUBLE_HOMING_MISSILES) &&
				(game.maxRocketAmmo >= maxDoubleHoming))
			{
				shopSelectedItem = -9;
				return;
			}
			if ((player.weaponType[1] == W_MICRO_HOMING_MISSILES) &&
				(game.maxRocketAmmo >= maxMicroHoming))
			{
				shopSelectedItem = -9;
				return;
			}
			if (game.maxRocketAmmo >= game.maxRocketAmmoLimit)
			{
				shopSelectedItem = -3;
				return;
			}
			game.maxRocketAmmo += 5;
			break;

		case SHOP_DOUBLE_ROCKETS:
			if (player.weaponType[1] == W_DOUBLE_ROCKETS)
			{
				shopSelectedItem = -8;
				return;
			}
			shop_sellSecondaryWeapon();
			player.weaponType[1] = W_DOUBLE_ROCKETS;
			LIMIT(game.maxRocketAmmo, 5, 50);
			shopSelectedItem = -1;
			break;

		case SHOP_MICRO_ROCKETS:
			if (player.weaponType[1] == W_MICRO_ROCKETS)
			{
				shopSelectedItem = -8;
				return;
			}
			shop_sellSecondaryWeapon();
			player.weaponType[1] = W_MICRO_ROCKETS;
			LIMIT(game.maxRocketAmmo, 5, 50);
			shopSelectedItem = -1;
			break;

		case SHOP_LASER:
			if (player.weaponType[1] == W_LASER)
			{
				shopSelectedItem = -8;
				return;
			}
			shop_sellSecondaryWeapon();
			player.weaponType[1] = W_LASER;

			while (game.maxRocketAmmo > 5)
				sell(SHOP_ROCKET_MAX_AMMO);
			while (player.ammo[1] > 0)
				sell(SHOP_ROCKET_AMMO);

			shopSelectedItem = -1;
			break;

		case SHOP_HOMING_MISSILE:
			if (player.weaponType[1] == W_HOMING_MISSILE)
			{
				shopSelectedItem = -8;
				return;
			}
			shop_sellSecondaryWeapon();
			player.weaponType[1] = W_HOMING_MISSILE;

			while (game.maxRocketAmmo > maxHoming)
				sell(SHOP_ROCKET_MAX_AMMO);

			LIMIT(game.maxRocketAmmo, 5, maxHoming);
			shopSelectedItem = -1;
			break;

		case SHOP_CHARGER:
			if (player.weaponType[1] == W_CHARGER)
			{
				shopSelectedItem = -8;
				return;
			}
			shop_sellSecondaryWeapon();
			player.weaponType[1] = W_CHARGER;

			while (game.maxRocketAmmo > 5)
				sell(SHOP_ROCKET_MAX_AMMO);
			while (player.ammo[1] > 0)
				sell(SHOP_ROCKET_AMMO);

			shopSelectedItem = -1;
			break;

		case SHOP_DOUBLE_HOMING_MISSILES:
			if (player.weaponType[1] == W_DOUBLE_HOMING_MISSILES)
			{
				shopSelectedItem = -8;
				return;
			}
			shop_sellSecondaryWeapon();
			player.weaponType[1] = W_DOUBLE_HOMING_MISSILES;

			while (game.maxRocketAmmo > maxDoubleHoming)
				sell(SHOP_ROCKET_MAX_AMMO);

			LIMIT(game.maxRocketAmmo, 5, maxDoubleHoming);
			shopSelectedItem = -1;
			break;

		case SHOP_MICRO_HOMING_MISSILES:
			if (player.weaponType[1] == W_MICRO_HOMING_MISSILES)
			{
				shopSelectedItem = -8;
				return;
			}
			shop_sellSecondaryWeapon();
			player.weaponType[1] = W_MICRO_HOMING_MISSILES;

			while (game.maxRocketAmmo > maxMicroHoming)
				sell(SHOP_ROCKET_MAX_AMMO);

			LIMIT(game.maxRocketAmmo, 5, maxMicroHoming);
			shopSelectedItem = -1;
			break;
	}

	if (!engine.cheatCash)
		game.cash -= shopItems[i].price;
	saveGame(0);
}

static void sell(int i)
{
	switch (i)
	{
		case SHOP_PLASMA_MAX_OUTPUT:
			if (game.maxPlasmaOutput <= 1)
			{
				shopSelectedItem = -5;
				return;
			}

			while (game.minPlasmaOutput >= game.maxPlasmaOutput)
				sell(SHOP_PLASMA_MIN_OUTPUT);

			game.maxPlasmaOutput--;
			if (weapon[W_PLAYER_WEAPON].ammo[0] <= game.maxPlasmaOutput + 1)
				weapon[W_PLAYER_WEAPON].ammo[0] = MIN(
					weapon[W_PLAYER_WEAPON].ammo[0],
					game.maxPlasmaOutput);
			break;

		case SHOP_PLASMA_MAX_DAMAGE:
			if (game.maxPlasmaDamage <= 1)
			{
				shopSelectedItem = -5;
				return;
			}

			while (game.minPlasmaDamage >= game.maxPlasmaDamage)
				sell(SHOP_PLASMA_MIN_DAMAGE);

			game.maxPlasmaDamage--;
			if (weapon[W_PLAYER_WEAPON].damage <= game.maxPlasmaDamage + 1)
				weapon[W_PLAYER_WEAPON].damage = MIN(
					weapon[W_PLAYER_WEAPON].damage,
					game.maxPlasmaDamage);
			break;

		case SHOP_PLASMA_MAX_RATE:
			if (game.maxPlasmaRate <= 1)
			{
				shopSelectedItem = -5;
				return;
			}

			while (game.minPlasmaRate >= game.maxPlasmaRate)
				sell(SHOP_PLASMA_MIN_RATE);

			game.maxPlasmaRate--;
			if (weapon[W_PLAYER_WEAPON].reload[0] >= rate2reload[game.maxPlasmaRate + 1])
				weapon[W_PLAYER_WEAPON].reload[0] = MAX(
					weapon[W_PLAYER_WEAPON].reload[0],
					rate2reload[game.maxPlasmaRate]);
			break;

		case SHOP_PLASMA_MIN_OUTPUT:
			if (game.minPlasmaOutput <= 1)
			{
				shopSelectedItem = -5;
				return;
			}
			game.minPlasmaOutput--;
			if (player.ammo[0] <= 0)
				weapon[W_PLAYER_WEAPON].ammo[0] = game.minPlasmaOutput;
			break;

		case SHOP_PLASMA_MIN_DAMAGE:
			if (game.minPlasmaDamage <= 1)
			{
				shopSelectedItem = -5;
				return;
			}
			game.minPlasmaDamage--;
			if (player.ammo[0] <= 0)
				weapon[W_PLAYER_WEAPON].damage = game.minPlasmaDamage;
			break;

		case SHOP_PLASMA_MIN_RATE:
			if (game.minPlasmaRate <= 1)
			{
				shopSelectedItem = -5;
				return;
			}
			game.minPlasmaRate--;
			if (player.ammo[0] <= 0)
				weapon[W_PLAYER_WEAPON].reload[0] = rate2reload[game.minPlasmaRate];
			break;

		case SHOP_PLASMA_AMMO:
			if (player.ammo[0] <= 0)
			{
				shopSelectedItem = -6;
				return;
			}
			if (player.ammo[0] > 10)
				player.ammo[0] -= 10;
			else
			{
				player.ammo[0] = 0;
				weapon[W_PLAYER_WEAPON].ammo[0] = game.minPlasmaOutput;
				weapon[W_PLAYER_WEAPON].damage = game.minPlasmaDamage;
				weapon[W_PLAYER_WEAPON].reload[0] = rate2reload[game.minPlasmaRate];
			}
			break;

		case SHOP_ROCKET_AMMO:
			if (player.ammo[1] <= 0)
			{
				shopSelectedItem = -6;
				return;
			}
			player.ammo[1]--;
			break;

		case SHOP_PLASMA_MAX_AMMO:
			if (game.maxPlasmaAmmo <= 100)
			{
				shopSelectedItem = -1;
				return;
			}
			game.maxPlasmaAmmo -= 25;

			while (player.ammo[0] > game.maxPlasmaAmmo)
				sell(SHOP_PLASMA_AMMO);

			break;

		case SHOP_ROCKET_MAX_AMMO:
			if (game.maxRocketAmmo <= 5)
			{
				shopSelectedItem = -1;
				return;
			}
			game.maxRocketAmmo -= 5;

			while (player.ammo[1] > game.maxRocketAmmo)
				sell(SHOP_ROCKET_AMMO);

			break;

		case SHOP_DOUBLE_ROCKETS:
			if (player.weaponType[1] != W_DOUBLE_ROCKETS)
			{
				shopSelectedItem = -1;
				return;
			}
			player.weaponType[1] = W_ROCKETS;
			shopSelectedItem = -1;
			break;

		case SHOP_MICRO_ROCKETS:
			if (player.weaponType[1] != W_MICRO_ROCKETS)
			{
				shopSelectedItem = -1;
				return;
			}
			player.weaponType[1] = W_ROCKETS;
			shopSelectedItem = -1;
			break;

		case SHOP_LASER:
			if (player.weaponType[1] != W_LASER)
			{
				shopSelectedItem = -1;
				return;
			}
			player.weaponType[1] = W_ROCKETS;
			player.ammo[1] = 0;
			shopSelectedItem = -1;
			break;

		case SHOP_HOMING_MISSILE:
			if (player.weaponType[1] != W_HOMING_MISSILE)
			{
				shopSelectedItem = -1;
				return;
			}
			player.weaponType[1] = W_ROCKETS;
			shopSelectedItem = -1;
			break;

		case SHOP_CHARGER:
			if (player.weaponType[1] != W_CHARGER)
			{
				shopSelectedItem = -1;
				return;
			}
			player.weaponType[1] = W_ROCKETS;
			player.ammo[1] = 0;
			shopSelectedItem = -1;
			break;

		case SHOP_DOUBLE_HOMING_MISSILES:
			if (player.weaponType[1] != W_DOUBLE_HOMING_MISSILES)
			{
				shopSelectedItem = -1;
				return;
			}
			player.weaponType[1] = W_ROCKETS;
			shopSelectedItem = -1;
			break;

		case SHOP_MICRO_HOMING_MISSILES:
			if (player.weaponType[1] != W_MICRO_HOMING_MISSILES)
			{
				shopSelectedItem = -1;
				return;
			}
			player.weaponType[1] = W_ROCKETS;
			shopSelectedItem = -1;
			break;
	}

	adjustShopPrices();

	if (game.difficulty == DIFFICULTY_ORIGINAL)
		game.cash += shopItems[i].price / 2;
	else
		game.cash += shopItems[i].price;

	saveGame(0);
}

void showShop()
{
 	int icons = SHOP_MAX;

	screen_blit(gfx_shopSprites[SHOP_S_PRIMARY], 20, 395);
	screen_blit(gfx_shopSprites[SHOP_S_POWERUP], 275, 395);
	screen_blit(gfx_shopSprites[SHOP_S_SECONDARY], 530, 395);
	screen_blit(gfx_shopSprites[SHOP_S_CATALOG], 100, 180);
	screen_blit(gfx_shopSprites[SHOP_S_SHIP_INFO], 100, 50);
	screen_blit(gfx_shopSprites[SHOP_S_ITEM_INFO], 100, 320);

	if (shopSelectedItem > -1)
	{
		screen_blit(gfx_sprites[SP_BUY], 60, 350);
		screen_blit(gfx_sprites[SP_SELL], 710, 350);
	}

	screen_blit(gfx_sprites[SP_FIREFLY], 380, 95);

	if (game.system == 0)
		icons = SHOP_DOUBLE_ROCKETS + 1;
	else if (game.system == 1)
		icons = SHOP_LASER + 1;
	else if (game.system == 2)
		icons = SHOP_CHARGER + 1;

	if ((engine.keyState[KEY_FIRE]))
	{
		for (int i = 0 ; i < icons ; i++)
		{
			if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6,
				shopItems[i].x, shopItems[i].y, 32, 25))
			{
				shopSelectedItem = i;
				engine.keyState[KEY_FIRE] = 0;
				drawShop();
			}
		}

		if (shopSelectedItem > -1)
		{
			if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 60, 350, 24, 16))
			{
				buy(shopSelectedItem);
				engine.keyState[KEY_FIRE] = 0;
				drawShop();
			}

			if (game_collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 700, 350, 24, 16))
			{
				sell(shopSelectedItem);
				engine.keyState[KEY_FIRE] = 0;
				drawShop();
			}
		}
	}
}
