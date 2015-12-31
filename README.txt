Copyright (C) 2015 Julian Marchant <onpon4@riseup.net>

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.

========================================================================


Thank you for downloading Project: Starfighter! This readme is just a
basic overview of the game and instructions for building; for
instructions on how to play the game, see docs/index.html.

Project: Starfighter is a space shoot 'em up game originally developed
by Parallel Realities in 2002, and released in 2003. You assume the role
of Chris Bainfield in his quest to put an end to WEAPCO, the weapons
corporation which has enslaved the galaxy.

If you played one of the versions of Project: Starfighter distributed by
Parallel Realities, you might notice some differences in this version
compared to the original:

* The graphics, sounds, and music are all completely different. This is
  because most of the original media was not properly licensed, and
  therefore needed to be replaced to make the game 100% libre and legal.
  Most of this work was done by the previous project at SourceForge;
  this project completed that work by adding music and replacing one
  non-libre graphic that got into version 1.2 by mistake.

* There are several gameplay changes. These changes were mostly done to
  balance the game better. However, if you want the original experience,
  "original" difficulty approximates it as closely as possible (short of
  implementing bugs on purpose).

* Typing "humansdoitbetter" in the title screen no longer enables
  cheats. This is actually because the switch to SDL2 broke the original
  feature, and rather than fixing it, I just replaced it with something
  else. I'll let you figure out what the new method to access the cheat
  menu is. :)


COMPILING FROM SOURCE

Project: Starfighter depends on the following libraries to build:

* SDL2 <http://libsdl.org>
* SDL2_image <http://www.libsdl.org/projects/SDL_image/>
* SDL2_mixer <http://www.libsdl.org/projects/SDL_mixer/>

Once you have all dependencies installed, simply run:

    make

And optionally (as root):

    make install

If this doesn't work, you may need to tweak the makefile.

To play, simply run the starfighter binary.