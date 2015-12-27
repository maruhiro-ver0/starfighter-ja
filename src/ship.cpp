/*
Copyright (C) 2003 Parallel Realities
Copyright (C) 2011, 2012 Guus Sliepen
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

/*
Fill in later...
*/
void ship_fireBullet(object *ship, int weaponType)
{
	if (ship->reload[weaponType] > 0)
		return;

	int y = (ship->image[0]->h) / 5;

	// Remove some ammo from the player
	if ((ship == &player) && (weaponType == 1) && (!engine.cheatAmmo))
		player.ammo[1]--;

	object *theWeapon = &weapon[ship->weaponType[weaponType]];

	switch(theWeapon->id)
	{
		case WT_PLASMA:
		case WT_SPREAD:
		case WT_DIRECTIONAL:
			audio_playSound(SFX_PLASMA, ship->x);
			break;
		case WT_ROCKET:
			audio_playSound(SFX_MISSILE, ship->x);
			break;
		case WT_LASER:
			audio_playSound(SFX_LASER, ship->x);
			break;
		case WT_CHARGER:
			audio_playSound(SFX_PLASMA3, ship->x);
			break;
	}

	if (theWeapon->flags & WF_SPREAD && theWeapon->ammo[0] >= 3)
	{
		bullet_add(theWeapon, ship, y * 2, -1);
		bullet_add(theWeapon, ship, y * 4, 1);

		if (theWeapon->ammo[0] != 4)
			bullet_add(theWeapon, ship, y * 3, 0);
		else
		{
			bullet_add(theWeapon, ship, y * 2, 0);
			bullet_add(theWeapon, ship, y * 4, 0);
		}

		if (theWeapon->ammo[0] == 5)
		{
			bullet_add(theWeapon, ship, y * 1, -2);
			bullet_add(theWeapon, ship, y * 5, 2);
		}
	}
	else
	{
		if(theWeapon->ammo[0] & 1)
			bullet_add(theWeapon, ship, y * 3, 0);

		if(theWeapon->ammo[0] >= 2)
		{
			bullet_add(theWeapon, ship, y * 2, 0);
			bullet_add(theWeapon, ship, y * 4, 0);
		}

		if(theWeapon->ammo[0] >= 4)
		{
			bullet_add(theWeapon, ship, y * 1, 0);
			bullet_add(theWeapon, ship, y * 5, 0);
		}
	}

	// Reset the weapon reload time. Double it if it is not friendly or
	// a boss or Kline
	ship->reload[weaponType] = theWeapon->reload[0];
	if ((ship->flags & FL_WEAPCO) && (ship != &aliens[ALIEN_BOSS]) &&
			(ship != &aliens[ALIEN_KLINE]) && (theWeapon->id != W_LASER))
		ship->reload[weaponType] *= 2;

	if ((engine.cheatAmmo) || (theWeapon->id == WT_LASER))
		return;

	if ((ship == &player) && (weaponType == 0))
	{
		if (player.ammo[0] > 0)
		{
			player.ammo[0]--;
			if (player.ammo[0] <= 0)
			{
				weapon[W_PLAYER_WEAPON].ammo[0] = game.minPlasmaOutput;
				weapon[W_PLAYER_WEAPON].damage = game.minPlasmaDamage;
				weapon[W_PLAYER_WEAPON].reload[0] = rate2reload[game.minPlasmaRate];
			}
		}
	}
}

/*
Fill in later...
*/
void ship_fireRay(object *ship)
{
	SDL_Rect ray;

	if (ship->face == 0)
	{
		ray.x = (int)(ship->x + ship->image[0]->w);
	}
	else
	{
		ray.x = (int)(ship->x - 800);
	}
	ray.y = (int)(ship->y + ship->engineY - 1);
	ray.h = 3;
	ray.w = 800;

	int red = SDL_MapRGB(screen->format, rand() % 256, 0x00, 0x00);
	SDL_FillRect(screen, &ray, red);
	addBuffer(ray.x, ray.y, ray.w, ray.h);

	if (ship != &player)
	{
		if (player.shield > 0)
		{
			if (collision(player.x, player.y, player.image[0]->w,
					player.image[0]->h, ray.x, ray.y, ray.w, ray.h) &&
				(!engine.cheatShield))
			{
				if (player.shield > engine.lowShield)
				{
					if (player.shield - 1 <= engine.lowShield)
					{
						setInfoLine("!!! WARNING: SHIELD LOW !!!", FONT_RED);
					}
				}
				player.shield--;

				explosion_add(player.x, player.y, E_SMALL_EXPLOSION);
				audio_playSound(SFX_HIT, player.x);
				if (player.shield < 1)
				{
					audio_playSound(SFX_DEATH, player.x);
					audio_playSound(SFX_EXPLOSION, player.x);
				}
			}
		}
	}

	for (int i = 0 ; i < ALIEN_MAX ; i++)
	{
		if (aliens[i].flags & FL_IMMORTAL)
			continue;

		if ((aliens[i].shield > 0) && (ship != &aliens[i]) &&
			(ship->classDef != aliens[i].classDef))
		{
			if (collision(aliens[i].x, aliens[i].y, aliens[i].image[0]->w,
					aliens[i].image[0]->h, ray.x, ray.y, ray.w, ray.h))
			{
				alien_hurt(&aliens[i], ship->owner, 1, false);
			}
		}
	}

	ship->ammo[0]--;
	if (ship->ammo[0] < 1)
		ship->flags &= ~FL_FIRERAY;
}
