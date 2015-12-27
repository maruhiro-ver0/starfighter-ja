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

static ShopItem shopItems[MAX_SHOPITEMS];
static signed char shopSelectedItem;

static void sell(int i);

static void drawSecondaryWeaponSurface()
{
	char description[50] = "";

	drawString("Secondary Weapon", 10, 3, FONT_WHITE, shopSurface[2]);

	switch (player.weaponType[1])
	{
		case W_NONE:
			strcpy(description, "Type : None");
			break;
		case W_ROCKETS:
			strcpy(description, "Type : Rockets");
			break;
		case W_DOUBLE_ROCKETS:
			strcpy(description, "Type : Dbl Rockets");
			break;
		case W_MICRO_ROCKETS:
			strcpy(description, "Type : Micro Rockets");
			break;
		case W_LASER:
			strcpy(description, "Type : Laser");
			break;
		case W_CHARGER:
			strcpy(description, "Type : Charger");
			break;
		case W_HOMING_MISSILE:
			strcpy(description, "Type : Homing Missile");
			break;
		case W_DOUBLE_HOMING_MISSILES:
			strcpy(description, "Type : Dbl Homing Missiles");
			break;
		case W_MICRO_HOMING_MISSILES:
			strcpy(description, "Type : Mcr Homing Missiles");
			break;
	}
	drawString(description, 10, 22, FONT_WHITE, shopSurface[2]);

	if ((player.weaponType[1] != W_LASER) &&
		(player.weaponType[1] != W_CHARGER) && (player.weaponType[1] != W_NONE))
	{
		sprintf(description, "Capacity : %d", game.maxRocketAmmo);
		drawString(description, 10, 37, FONT_WHITE, shopSurface[2]);
	}
}

static void adjustShopPrices()
{
	if (game.difficulty == DIFFICULTY_ORIGINAL)
	{
		shopItems[SHOP_PLASMA_MAX_OUTPUT].price = (500 *
			game.maxPlasmaOutput);
		shopItems[SHOP_PLASMA_MAX_DAMAGE].price = (500 *
			game.maxPlasmaDamage);
		shopItems[SHOP_PLASMA_MAX_RATE].price = (500 *
			(game.maxPlasmaRate * 2 - 1));

		shopItems[SHOP_PLASMA_MIN_OUTPUT].price = (2000 *
			game.minPlasmaOutput);
		shopItems[SHOP_PLASMA_MIN_DAMAGE].price = (2000 *
			game.minPlasmaDamage);
		shopItems[SHOP_PLASMA_MIN_RATE].price = (2000 *
			(game.minPlasmaRate * 2 - 1));
	}
	else
	{
		shopItems[SHOP_PLASMA_MAX_OUTPUT].price = (1000 *
			(game.maxPlasmaOutput + 1));
		shopItems[SHOP_PLASMA_MAX_DAMAGE].price = (1000 *
			(game.maxPlasmaDamage + 1));
		shopItems[SHOP_PLASMA_MAX_RATE].price = (1000 *
			(game.maxPlasmaRate + 1));

		shopItems[SHOP_PLASMA_MIN_OUTPUT].price = (1500 *
			(game.minPlasmaOutput + 1));
		shopItems[SHOP_PLASMA_MIN_DAMAGE].price = (1500 *
			(game.minPlasmaDamage + 1));
		shopItems[SHOP_PLASMA_MIN_RATE].price = (1500 *
			(game.minPlasmaRate + 1));
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
	adjustShopPrices();

	for (int i = 0 ; i < MAX_SHOPSHAPES ; i++)
	{
		if (shopSurface[i] != NULL)
		{
			SDL_FreeSurface(shopSurface[i]);
		}
	}

	for (int i = 0 ; i < 3 ; i++)
		shopSurface[i] = createSurface(246, 91);

	for (int i = 0 ; i < 3 ; i++)
	{
		blevelRect(shopSurface[i], 0, 0, 245, 90, 0x00, 0x00, 0x55);
		blevelRect(shopSurface[i], 0, 0, 245, 20, 0x00, 0x00, 0x99);
	}

	shopSurface[4] = alphaRect(601, 101, 0x00, 0x00, 0x00);
	blevelRect(shopSurface[4], 0, 0, 600, 100, 0x00, 0x00, 0x33);

	switch (shopSelectedItem)
	{
		case -1:
		case -2:
		case -3:
		case -4:
		case -5:
		case -6:
			break;
		case 0:
		case 1:
		case 2:
		case 8:
			blevelRect(shopSurface[1], 0, 0, 245, 90, 0x55, 0x00, 0x00);
			blevelRect(shopSurface[1], 0, 0, 245, 20, 0x99, 0x00, 0x00);
			break;
		case 3:
		case 4:
			blevelRect(shopSurface[4], 0, 0, 600, 100, 0x33, 0x00, 0x00);
			break;
		case 5:
		case 6:
		case 7:
			blevelRect(shopSurface[0], 0, 0, 245, 90, 0x55, 0x00, 0x00);
			blevelRect(shopSurface[0], 0, 0, 245, 20, 0x99, 0x00, 0x00);
			break;
		default:
			blevelRect(shopSurface[2], 0, 0, 245, 90, 0x55, 0x00, 0x00);
			blevelRect(shopSurface[2], 0, 0, 245, 20, 0x99, 0x00, 0x00);
			break;
	}

	char description[100];
	strcpy(description, "");

	drawString("Primary Weapon", 10, 3, FONT_WHITE, shopSurface[0]);
	sprintf(description, "Plasma Cannons : %d", game.minPlasmaOutput);
	drawString(description, 10, 22, FONT_WHITE, shopSurface[0]);
	sprintf(description, "Plasma Power   : Stage %d",
		game.minPlasmaDamage);
	drawString(description, 10, 37, FONT_WHITE, shopSurface[0]);
	sprintf(description, "Cooler         : Stage %d",
		game.minPlasmaRate);
	drawString(description, 10, 52, FONT_WHITE, shopSurface[0]);

	drawString("Powerup Weapon", 10, 3, FONT_WHITE, shopSurface[1]);
	sprintf(description, "Plasma Output    : Stage %d",
		game.maxPlasmaOutput);
	drawString(description, 10, 22, FONT_WHITE, shopSurface[1]);
	sprintf(description, "Plasma Condensor : Stage %d",
		game.maxPlasmaDamage);
	drawString(description, 10, 37, FONT_WHITE, shopSurface[1]);
	sprintf(description, "Liquid Nitrogen  : Stage %d",
		game.maxPlasmaRate);
	drawString(description, 10, 52, FONT_WHITE, shopSurface[1]);
	sprintf(description, "Plasma Capacity  : %d", game.maxPlasmaAmmo);
	drawString(description, 10, 67, FONT_WHITE, shopSurface[1]);

	drawSecondaryWeaponSurface();

	shopSurface[3] = createSurface(601, 121);

	blevelRect(shopSurface[3], 0, 0, 600, 120, 0x00, 0x00, 0x22);

	drawString("Temporary Weapons", 10, 2, FONT_WHITE, shopSurface[3]);
	drawString("Ammo and Storage", 260, 2, FONT_WHITE, shopSurface[3]);

	drawString("Primary Weapons", 10, 62, FONT_WHITE, shopSurface[3]);

	drawString("Secondary Weapons", 260, 62, FONT_WHITE, shopSurface[3]);

	signed char icons = MAX_SHOPITEMS;

	if (game.system == 0)
		icons = SHOP_DOUBLE_ROCKETS + 1;
	else if (game.system == 1)
		icons = SHOP_LASER + 1;
	else if (game.system == 2)
		icons = SHOP_CHARGER + 1;

	for (int i = 0 ; i < icons ; i++)
	{
		blit(shape[shopItems[i].image], shopItems[i].x - 90,
			shopItems[i].y - 178, shopSurface[3]);
	}

	sprintf(description, "Shield Units : %d", player.maxShield);
	drawString(description, 10, 4, FONT_WHITE, shopSurface[4]);
	sprintf(description, "Cash : $%d", game.cash);
	drawString(description, 10, 80, FONT_WHITE, shopSurface[4]);
	sprintf(description, "Plasma Cells : %.3d", player.ammo[0]);
	drawString(description, 430, 4, FONT_WHITE, shopSurface[4]);
	sprintf(description, "Rockets : %.3d", player.ammo[1]);
	drawString(description, 475, 80, FONT_WHITE, shopSurface[4]);

	shopSurface[5] = createSurface(601, 56);
	blevelRect(shopSurface[5], 0, 0, 600, 35, 0x00, 0x99, 0x00);
	blevelRect(shopSurface[5], 0, 20, 600, 35, 0x00, 0x33, 0x00);
	drawString("Information", 5, 4, FONT_WHITE, shopSurface[5]);

	switch (shopSelectedItem)
	{
		case -1:
			break;
		case -2:
			drawString("You don't have enough money", 20, 30, FONT_WHITE,
				shopSurface[5]);
			break;
		case -3:
			drawString("Cannot upgrade ship", 5, 22, FONT_WHITE, shopSurface[5]);
			drawString("Hardware capacity has been reached", 20, 38, FONT_CYAN,
				shopSurface[5]);
			break;
		case -4:
			drawString("Ammunition limit reached", 20, 30, FONT_WHITE,
				shopSurface[5]);
			break;
		case -5:
			drawString("You cannot sell that item", 20, 30, FONT_WHITE,
				shopSurface[5]);
			break;
		case -6:
			drawString("Nothing to sell", 20, 30, FONT_WHITE, shopSurface[5]);
			break;
		case -7:
			drawString("Rockets cannot be bought for Laser or Charger Cannon",
				5, 30, FONT_WHITE, shopSurface[5]);
			break;
		case -8:
			drawString("You already have that weapon", 20, 30, FONT_WHITE,
				shopSurface[5]);
			break;
		case -9:
			drawString("This weapon's ammo limit has been reached", 20, 30,
				FONT_WHITE, shopSurface[5]);
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
			drawString(shopItems[shopSelectedItem].name, 5, 22, FONT_WHITE,
				shopSurface[5]);
			drawString(description, 20, 38, FONT_CYAN, shopSurface[5]);
			break;
	}
}

void initShop()
{
	/* ----------- Temporary Items ----------- */

	shopItems[SHOP_PLASMA_MAX_OUTPUT].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MAX_OUTPUT].name, "Plasma channel splitter");
	strcpy(shopItems[SHOP_PLASMA_MAX_OUTPUT].description,
		"Improves poweredup plasma output");
	shopItems[SHOP_PLASMA_MAX_OUTPUT].image = 9;

	shopItems[SHOP_PLASMA_MAX_DAMAGE].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MAX_DAMAGE].name, "Plasma capacity condensor");
	strcpy(shopItems[SHOP_PLASMA_MAX_DAMAGE].description,
		"Increases poweredup plasma damage");
	shopItems[SHOP_PLASMA_MAX_DAMAGE].image = 10;

	shopItems[SHOP_PLASMA_MAX_RATE].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MAX_RATE].name, "Liquid nitrogen capsules");
	strcpy(shopItems[SHOP_PLASMA_MAX_RATE].description,
		"Increases plasma firing rate");
	shopItems[SHOP_PLASMA_MAX_RATE].image = 11;

	shopItems[SHOP_PLASMA_AMMO].price = 10;
	strcpy(shopItems[SHOP_PLASMA_AMMO].name, "10 Plasma cells");
	strcpy(shopItems[SHOP_PLASMA_AMMO].description, "Plasma ammunition");
	shopItems[SHOP_PLASMA_AMMO].image = 12;

	shopItems[SHOP_ROCKET_AMMO].price = 10;
	strcpy(shopItems[SHOP_ROCKET_AMMO].name, "Rocket Ammo");
	strcpy(shopItems[SHOP_ROCKET_AMMO].description,
		"High velocity dumb fire rocket");
	shopItems[SHOP_ROCKET_AMMO].image = 13;

	/* ----------- Permanent Items ----------- */

	shopItems[SHOP_PLASMA_MIN_OUTPUT].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MIN_OUTPUT].name, "Additional Plasma Cannon");
	strcpy(shopItems[SHOP_PLASMA_MIN_OUTPUT].description,
		"Adds an extra plasma cannon to the Firefly");
	shopItems[SHOP_PLASMA_MIN_OUTPUT].image = 14;

	shopItems[SHOP_PLASMA_MIN_DAMAGE].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MIN_DAMAGE].name, "Plasma Power Booster");
	strcpy(shopItems[SHOP_PLASMA_MIN_DAMAGE].description,
		"Increases power of plasma shots");
	shopItems[SHOP_PLASMA_MIN_DAMAGE].image = 15;

	shopItems[SHOP_PLASMA_MIN_RATE].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MIN_RATE].name, "Plasma Cooling Booster");
	strcpy(shopItems[SHOP_PLASMA_MIN_RATE].description,
		"Permanently increases firing rate");
	shopItems[SHOP_PLASMA_MIN_RATE].image = 16;

	/* ----------- Ammo Items -------------- */

	shopItems[SHOP_PLASMA_MAX_AMMO].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_PLASMA_MAX_AMMO].name, "Plasma compressor");
	strcpy(shopItems[SHOP_PLASMA_MAX_AMMO].description,
		"Increases plasma ammo capacity");
	shopItems[SHOP_PLASMA_MAX_AMMO].image = 17;

	shopItems[SHOP_ROCKET_MAX_AMMO].price = 0; // Overwritten later
	strcpy(shopItems[SHOP_ROCKET_MAX_AMMO].name, "Rocket Pod");
	strcpy(shopItems[SHOP_ROCKET_MAX_AMMO].description,
		"Allows for an additional 5 rockets to be carried");
	shopItems[SHOP_ROCKET_MAX_AMMO].image = 18;

	/* ---------- Weaponary --------------- */

	shopItems[SHOP_DOUBLE_ROCKETS].price = 2000;
	strcpy(shopItems[SHOP_DOUBLE_ROCKETS].name, "Dual Rocket Launcher");
	strcpy(shopItems[SHOP_DOUBLE_ROCKETS].description,
		"Allows for two rockets to be fired at once");
	shopItems[SHOP_DOUBLE_ROCKETS].image = 19;

	shopItems[SHOP_MICRO_ROCKETS].price = 2500;
	strcpy(shopItems[SHOP_MICRO_ROCKETS].name, "Micro Rocket Launcher");
	strcpy(shopItems[SHOP_MICRO_ROCKETS].description,
		"Launches several less powerful rockets at once");
	shopItems[SHOP_MICRO_ROCKETS].image = 20;

	shopItems[SHOP_LASER].price = 5000;
	strcpy(shopItems[SHOP_LASER].name, "Laser Cannon");
	strcpy(shopItems[SHOP_LASER].description, "Laser Cannon");
	shopItems[SHOP_LASER].image = 21;

	shopItems[SHOP_HOMING_MISSILE].price = 7500;
	strcpy(shopItems[SHOP_HOMING_MISSILE].name, "Homing Missile Launcher");
	sprintf(shopItems[SHOP_HOMING_MISSILE].description,
		"Fires homing missile (max %i missiles)", maxHoming);
	shopItems[SHOP_HOMING_MISSILE].image = 22;

	shopItems[SHOP_CHARGER].price = 10000;
	strcpy(shopItems[SHOP_CHARGER].name, "Charge Cannon");
	strcpy(shopItems[SHOP_CHARGER].description, "A charge up cannon");
	shopItems[SHOP_CHARGER].image = 23;

	shopItems[SHOP_DOUBLE_HOMING_MISSILES].price = 10000;
	strcpy(shopItems[SHOP_DOUBLE_HOMING_MISSILES].name,
		"Dual Homing Missile Launcher");
	sprintf(shopItems[SHOP_DOUBLE_HOMING_MISSILES].description,
		"Fires two homing missiles (max %i missiles)", maxDoubleHoming);
	shopItems[SHOP_DOUBLE_HOMING_MISSILES].image = 24;

	shopItems[SHOP_MICRO_HOMING_MISSILES].price = 15000;
	strcpy(shopItems[SHOP_MICRO_HOMING_MISSILES].name,
		"Homing Micro Missile Launcher");
	sprintf(shopItems[SHOP_MICRO_HOMING_MISSILES].description,
		"Fires several small homing missiles (max %i missiles)", maxMicroHoming);
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

	player.image[0] = shape[0];
	player.x = 380;
	player.y = 95;

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
 	blit(shopSurface[0], 20, 395);
	blit(shopSurface[1], 275, 395);
	blit(shopSurface[2], 530, 395);
	blit(shopSurface[3], 100, 180);
	blit(shopSurface[4], 100, 50);
	blit(shopSurface[5], 100, 320);

	if (shopSelectedItem > -1)
	{
		blit(shape[27], 60, 350);
		blit(shape[28], 710, 350);
	}

	blit(shape[29], (int)player.x, (int)player.y);

	signed char icons = MAX_SHOPITEMS;

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
			if (collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6,
				shopItems[i].x, shopItems[i].y, 32, 25))
			{
				shopSelectedItem = i;
				engine.keyState[KEY_FIRE] = 0;
				drawShop();
			}
		}

		if (shopSelectedItem > -1)
		{
			if (collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 60, 350, 24, 16))
			{
				buy(shopSelectedItem);
				engine.keyState[KEY_FIRE] = 0;
				drawShop();
			}

			if (collision(engine.cursor_x + 13, engine.cursor_y + 13, 6, 6, 700, 350, 24, 16))
			{
				sell(shopSelectedItem);
				engine.keyState[KEY_FIRE] = 0;
				drawShop();
			}
		}
	}
}
