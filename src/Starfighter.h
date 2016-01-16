/*
Copyright (C) 2003 Parallel Realities
Copyright (C) 2011 Guus Sliepen
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

#ifndef STARFIGHTER_H
#define STARFIGHTER_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#include "defs.h"
#include "structs.h"

#include "alien.h"
#include "audio.h"
#include "bullet.h"
#include "cargo.h"
#include "collectable.h"
#include "colors.h"
#include "cutscene.h"
#include "engine.h"
#include "event.h"
#include "explosion.h"
#include "game.h"
#include "gfx.h"
#include "intermission.h"
#include "loadSave.h"
#include "messages.h"
#include "misc.h"
#include "missions.h"
#include "player.h"
#include "renderer.h"
#include "resources.h"
#include "screen.h"
#include "ship.h"
#include "shop.h"
#include "title.h"
#include "weapons.h"
#include "window.h"

#endif
