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

#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct object_ {

	bool active;
	int classDef; // Used by aliens to determine what they are
	int AIType; // Type of articifial intelligence

	int id; // The "job" of the object
	struct object_ *target; //  index target in aliens array

	int reload[2];

	int systemPower; // computer systems for craft
	int shield;   // current shield
	int maxShield; // max shield (for recharging)
	int deathCounter; // how long to explode for

	int speed;
	int damage; // Contact damage for bullets
	int ammo[2]; // Ammo for 2nd weapon.

	int face; // Either 0 or 1

	struct object_ *owner; // Who owns this object

	int chance[2]; // Chance of using the weapons (out of 1000)

	SDL_Surface *image[2]; // For facing left and right
	int imageIndex[2]; // used for loading
	int hit; // used to make a craft "flash" if it is struck by a shot

	int engineX; // The place for the engine on the other side of the craft
	int engineY; // The middle of the engine on the craft

	int thinktime; // When the object will next react
	int weaponType[2]; // Weapon types

	int collectChance; // Chance of dropping the object
	int collectType; // What the object is carrying
	int collectValue; // What it is worth

	unsigned long int flags; // Various flags for an object

	float x, y, dx, dy;

	struct object_ *next;

} object;

typedef struct mission_ {

	char primaryObjective[3][50]; // Description
	int primaryType[3]; // The type of mission this is
	int target1[3]; // index of target in aliens array
	int targetValue1[3]; // Number of things to collect (slaves, cash, etc)
	int timeLimit1[3]; // In minutes
	int completed1[3];

	char secondaryObjective[3][50]; // Description
	int secondaryType[3]; // The type of mission this is
	int target2[3]; // index of target in aliens array
	int targetValue2[3]; // Number of things to collect (slaves, cash, etc)
	int timeLimit2[3]; // In minutes
	int completed2[3];

	int remainingObjectives1;
	int remainingObjectives2;
	int addAliens; // How often to add new enemies

} mission;

typedef struct Star_ {

	float x, y, dx, dy;
	int speed; // How fast the star moves

} Star;

typedef struct collectables_ {

	bool active;
	float x, y, dx, dy;
	SDL_Surface *image;
	int type; // What kind of collectable is it?
	int value; // How much is it worth?
	int life; // How long it will stay around for

	struct collectables_ *next;

} collectables;

typedef struct textObject_ {

	SDL_Surface *image;
	int life;
	float x, y;
	int fontColor;
	char text[255];

} textObject;

typedef struct Game_ {
	object thePlayer;
	object playerWeapon;

	int saveFormat;

	int difficulty;

	int system;
	int area;
	int musicVolume;
	int sfxVolume;

	int cash;
	int cashEarned;
	
	int shots;
	int hits;
	int accuracy;
	int hasWingMate1;
	int hasWingMate2;
	int totalKills;
	int wingMate1Kills;
	int wingMate2Kills;
	int wingMate1Ejects;
	int wingMate2Ejects;
	int totalOtherKills;
	int secondaryMissions;
	int secondaryMissionsCompleted;
	int shieldPickups;
	int rocketPickups;
	int cellPickups;
	int powerups;
	int minesKilled;
	int cargoPickups;

	// slaves for Eyananth
	int slavesRescued;

	// remaining shield for experimental fighter
	int experimentalShield;

	Uint32 timeTaken; // In seconds
	int missionCompleted[10];

	int stationedPlanet;
	int destinationPlanet;

	char stationedName[20];
	char destinationName[20];
	int distanceCovered;

	int minPlasmaRate;
	int minPlasmaDamage;
	int minPlasmaOutput;
	int maxPlasmaRate;
	int maxPlasmaDamage;
	int maxPlasmaOutput;
	int maxPlasmaAmmo;
	int maxRocketAmmo;

	// Limits on shop upgrades
	int minPlasmaRateLimit;
	int minPlasmaDamageLimit;
	int minPlasmaOutputLimit;
	int maxPlasmaRateLimit;
	int maxPlasmaDamageLimit;
	int maxPlasmaOutputLimit;
	int maxPlasmaAmmoLimit;
	int maxRocketAmmoLimit;

} Game;

typedef struct ShopItem_ {

	int x, y;
	int price;
	char name[50];
	char description[255];
	int image;

} ShopItem;

typedef struct bRect_ {

	int x, y, w, h;
	struct bRect_ *next;

} bRect;

typedef struct Planet_ {

	int y;
	char name[50];
	SDL_Surface *image;

	int missionNumber; // associated mission number
	int missionCompleted; // whether it has been completed

	int messageMission;
	int messageSlot;
	int faceImage;
	char from[50];
	char subject[100];

} Planet;

#endif
