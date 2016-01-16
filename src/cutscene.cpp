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

typedef struct Message_ {

	int face;
	char message[255];

} Message;


static Message messages[MAX_EVENTS];

void cutscene_init(int scene)
{
	screen_clear(black);
	renderer_update();
	screen_clear(black);

	engine.keyState[KEY_FIRE] = 0; 
	engine.keyState[KEY_ALTFIRE] = 0;

	engine.ssx = -0.5;
	engine.ssy = 0;
	engine.smx = 0;
	engine.smy = 0;

	screen_flushBuffer();
	gfx_free();
	resetLists();
	loadGameGraphics();

	for (int i = 0 ; i < ALIEN_MAX ; i++)
	{
		// 0 is the dualfighter, but there doesn't seem to be any
		// particular reason for choosing this alien def.
		aliens[i] = alien_defs[0];
		aliens[i].face = 0;
		aliens[i].active = false;
	}

	for (int i = 0 ; i < MAX_EVENTS ; i++)
	{
		strcpy(messages[i].message, "");
		messages[i].face = -1;
	}

	switch (scene)
	{
		case 0:
			loadBackground("gfx/spirit.jpg");
			engine.ssx = -3;
			engine.ssy = 0;

			aliens[0].image[0] = gfx_shipSprites[SS_FIREFLY];
			aliens[0].x = screen->w * 3 / 5;
			aliens[0].y = screen->h / 2;
			aliens[0].dx = 3.1;
			aliens[0].active = true;

			for (int i = 1 ; i < 7 ; i++)
			{
				aliens[i].image[0] = gfx_shipSprites[SS_DUALFIGHTER];
				aliens[i].x = RANDRANGE(0, screen->w / 8);
				aliens[i].y = RANDRANGE(50, screen->h - 50);
				aliens[i].dx = 3;
				aliens[i].active = true;
			}

			messages[0].face = -1;
			strcpy(messages[0].message, "クリス・バンフィールドは新たに獲得した『ファイアフライ』で逃走する途中、ついにWEAPCOのパトロールに捕捉された…");

			messages[1].face = FS_CHRIS;
			strcpy(messages[1].message, "まだ追って来るのか!?");

			messages[2].face = FS_CHRIS;
			strcpy(messages[2].message, "まだこの艦の操縦に慣れていないが、パトロール艦を集合地点まで連れて行くわけにはいかない…");

			messages[3].face = FS_CHRIS;
			strcpy(messages[3].message, "…いずれ戦うことになるのだ。ならばこの艦の性能を試してやる!");

			break;

		case 1:
			loadBackground("gfx/sol.jpg");
			engine.ssx = -0.5;
			engine.ssy = 0;

			aliens[0].image[0] = gfx_shipSprites[SS_KLINE];
			aliens[0].x = screen->w / 2;
			aliens[0].y = screen->h / 2;
			aliens[0].dx = 0.5;
			aliens[0].active = true;

			for (int i = 1 ; i < 15 ; i++)
			{
				aliens[i].image[0] = gfx_shipSprites[SS_DUALFIGHTER];
				aliens[i].x = RANDRANGE(0, screen->w);
				aliens[i].y = RANDRANGE(50, screen->h - 50);
				aliens[i].dx = RANDRANGE(1, 3);
				aliens[i].active = true;
			}

			messages[0].face = -1;
			strcpy(messages[0].message, "数時間後、クリス・バンフィールドの活躍は太陽系まで伝わった。WEAPCOの艦隊司令官は彼の扱いについて考えていた。");

			messages[1].face = FS_KLINE;
			strcpy(messages[1].message, "帝王は不機嫌だろうな。今やスピリット系はあの反逆者のパイロットによって解放された。");

			messages[2].face = FS_KLINE;
			strcpy(messages[2].message, "報告によれば、我々の最も強力なフリゲートの内の一隻を打ちのめすのに3分と掛からなかった!");

			messages[3].face = FS_KLINE;
			strcpy(messages[3].message, "そんな才能を持つ者はめったにいない。奴は我々の新しいAIトレーニングプログラムの候補としては完璧だ!");

			messages[4].face = FS_KLINE;
			strcpy(messages[4].message, "奴を殺さなければならないのが残念だ…");

			break;

		case 2:
			loadBackground("gfx/spirit.jpg");
			engine.ssx = -1.4;
			engine.ssy = 0;

			aliens[0].image[0] = gfx_shipSprites[SS_FIREFLY];
			aliens[0].x = screen->w / 4;
			aliens[0].y = screen->h / 2;
			aliens[0].dx = 1.5;
			aliens[0].active = true;

			aliens[1].image[0] = gfx_shipSprites[SS_SID];
			aliens[1].x = screen->w / 4 - 50;
			aliens[1].y = screen->h / 2 - 40;
			aliens[1].dx = 1.5;
			aliens[1].active = true;

			messages[0].face = FS_SID;
			strcpy(messages[0].message, "もうすぐエヤナンス系へのワープが始まる。");

			messages[1].face = FS_CHRIS;
			strcpy(messages[1].message, "その星系にはたくさんのWEAPCOの奴隷がいるのだろう?");

			messages[2].face = FS_SID;
			strcpy(messages[2].message, "その通りだ。そこはこの銀河で最も危険な場所の1つだ。");

			messages[3].face = FS_CHRIS;
			strcpy(messages[3].message, "そこに行けば有利になると?");

			messages[4].face = FS_SID;
			strcpy(messages[4].message, "そうだ。私に計画がある。");

			break;

		case 3:
			loadBackground("gfx/eyananth.jpg");
			engine.ssx = -0.5;
			engine.ssy = 0;

			aliens[0].image[0] = gfx_shipSprites[SS_FIREFLY];
			aliens[0].x = screen->w * 3 / 8;
			aliens[0].y = screen->h / 2;
			aliens[0].dx = 0.5;
			aliens[0].active = true;

			aliens[1].image[0] = gfx_shipSprites[SS_SID];
			aliens[1].x = screen->w * 3 / 8 - 50;
			aliens[1].y = screen->h / 2 - 40;
			aliens[1].dx = 0.5;
			aliens[1].active = true;

			aliens[2].image[0] = gfx_shipSprites[SS_FRIEND];
			aliens[2].x = screen->w * 3 / 8 - 50;
			aliens[2].y = screen->h / 2 + 40;
			aliens[2].dx = 0.5;
			aliens[2].active = true;

			messages[0].face = FS_PHOEBE;
			strcpy(messages[0].message, "いいヘッドギアね! もしかして同じ店で買ったんじゃない?");

			messages[1].face = FS_CHRIS;
			strcpy(messages[1].message, "もっと重要なことがある。あそこで何をしてたんだ? 俺が近くにいてよかったよ!");

			messages[2].face = FS_PHOEBE;
			strcpy(messages[2].message, "姉さんを探してた。1週間前にいなくなってしまったの。");

			messages[3].face = FS_PHOEBE;
			strcpy(messages[3].message, "待って! あなたたちスピリット系から来た反乱軍でしょう! すごいわ! 私も加わっていい?");

			messages[4].face = FS_CHRIS;
			strcpy(messages[4].message, "新たな戦力か? 俺は困らないな! どうだ、シド?");

			messages[5].face = FS_SID;
			strcpy(messages[5].message, "いいだろう。その誘導ミサイルにも興味がある。それは助けになりそうだ。");

			messages[6].face = FS_CHRIS;
			strcpy(messages[6].message, "そういうわけだ。ようこそ!");

			break;

		case 4:
			loadBackground("gfx/eyananth.jpg");
			engine.ssx = -1.4;
			engine.ssy = 0;

			aliens[0].image[0] = gfx_shipSprites[SS_FIREFLY];
			aliens[0].x = screen->w / 4;
			aliens[0].y = screen->h / 2;
			aliens[0].dx = 1.5;
			aliens[0].active = true;

			aliens[1].image[0] = gfx_shipSprites[SS_SID];
			aliens[1].x = screen->w / 4 - 50;
			aliens[1].y = screen->h / 2 - 40;
			aliens[1].dx = 1.5;
			aliens[1].active = true;

			aliens[2].image[0] = gfx_shipSprites[SS_FRIEND];
			aliens[2].x = screen->w / 4 - 50;
			aliens[2].y = screen->h / 2 + 40;
			aliens[2].dx = 1.5;
			aliens[2].active = true;

			messages[0].face = FS_SID;
			strcpy(messages[0].message, "何があったんだ、クリス。映像が途絶えていた。");

			messages[1].face = FS_CHRIS;
			strcpy(messages[1].message, "WEAPCOの採掘船を破壊したが、その直後にクライン・ケスランと名乗る男に会った。");

			messages[2].face = FS_SID;
			strcpy(messages[2].message, "聞いたことがある。WEAPCOの艦隊司令官であり、最も優秀なパイロットの一人だ。");

			messages[3].face = FS_CHRIS;
			strcpy(messages[3].message, "奴は俺を地獄の様な戦いに引きずり込んだ! だが、すぐに去って行った。");

			messages[4].face = FS_CHRIS;
			strcpy(messages[4].message, "それよりシド、モルドール系の情報はあるか?");

			messages[5].face = FS_SID;
			strcpy(messages[5].message, "救出した科学者によると、WEAPCOはそこで新型戦闘機の試験をしているそうだ。");

			messages[6].face = FS_CHRIS;
			strcpy(messages[6].message, "そいつをぶっ壊して、開発を止めねばらなんな。");

			messages[7].face = FS_SID;
			strcpy(messages[7].message, "その通りだ。");

			messages[8].face = FS_PHOEBE;
			strcpy(messages[8].message, "そこに姉さんがいるのかしら…。");

			break;

		case 5:
			loadBackground("gfx/mordor.jpg");
			engine.ssx = -0.5;
			engine.ssy = 0;

			aliens[0].image[0] = gfx_shipSprites[SS_FIREFLY];
			aliens[0].x = screen->w * 3 / 8;
			aliens[0].y = screen->h / 2;
			aliens[0].dx = 0.5;
			aliens[0].active = true;

			aliens[1].image[0] = gfx_shipSprites[SS_SID];
			aliens[1].x = screen->w * 3 / 8 - 50;
			aliens[1].y = screen->h / 2 - 40;
			aliens[1].dx = 0.5;
			aliens[1].active = true;

			aliens[2].image[0] = gfx_shipSprites[SS_FRIEND];
			aliens[2].x = screen->w * 3 / 8 - 50;
			aliens[2].y = screen->h / 2 + 40;
			aliens[2].dx = 0.5;
			aliens[2].active = true;

			messages[0].face = FS_PHOEBE;
			strcpy(messages[0].message, "姉さんはだいじょうぶ?");

			messages[1].face = FS_SID;
			strcpy(messages[1].message, "何かインプラントのような物があったが取り除けた。まだしばらくの間は頭痛がするだろう。");

			messages[2].face = FS_CHRIS;
			strcpy(messages[2].message, "何か重要なことは聞けそうか?");

			messages[3].face = FS_SID;
			strcpy(messages[3].message, "記憶が戻るまで少し待たなければならない。インプラントがあったときのことは思い出せないだろう。");

			messages[4].face = FS_SID;
			strcpy(messages[4].message, "それでも艦の操縦はできそうだ。");

			break;

		case 6:
			loadBackground("gfx/mordor.jpg");
			engine.ssx = -1.4;
			engine.ssy = 0;

			aliens[0].image[0] = gfx_shipSprites[SS_FIREFLY];
			aliens[0].x = screen->w / 4;
			aliens[0].y = screen->h / 2;
			aliens[0].dx = 1.5;
			aliens[0].active = true;

			aliens[1].image[0] = gfx_shipSprites[SS_SID];
			aliens[1].x = screen->w / 4 - 50;
			aliens[1].y = screen->h / 2 - 40;
			aliens[1].dx = 1.5;
			aliens[1].active = true;

			aliens[2].image[0] = gfx_shipSprites[SS_FRIEND];
			aliens[2].x = screen->w / 4 - 50;
			aliens[2].y = screen->h / 2 + 40;
			aliens[2].dx = 1.5;
			aliens[2].active = true;

			aliens[3].image[0] = gfx_shipSprites[SS_FRIEND];
			aliens[3].x = screen->w / 4 - 90;
			aliens[3].y = screen->h / 2;
			aliens[3].dx = 1.5;
			aliens[3].active = true;

			messages[0].face = FS_CHRIS;
			strcpy(messages[0].message, "すまない。切り札を失ってしまった。");

			messages[1].face = FS_SID;
			strcpy(messages[1].message, "気にするな。私の望んだ方法ではなかったが、これでWEAPCOを倒しやすくなった。");

			messages[2].face = FS_SID;
			strcpy(messages[2].message, "太陽系での戦いは、困難なものになるだろう。外惑星に強力な防御を敷いていると聞いた。");

			messages[3].face = FS_PHOEBE;
			strcpy(messages[3].message, "ならば、そこから始めないとね。");

			messages[4].face = FS_SID;
			strcpy(messages[4].message, "そこにいる軍団は今までとは全く異なるものだ。皆、気をつけろ。");

			break;
	}

	/*
		Because we can fiddle with the images, we need to set the engines to 
		the correct places on the craft. Otherwise it will look wrong
	*/
	for (int i = 0 ; i < 15 ; i++)
	{
		aliens[i].engineX = aliens[i].image[0]->w;
		aliens[i].engineY = (aliens[i].image[0]->h / 2);
	}

	bool showMessage = false;
	int currentMessage = -1;
	int timer = 60 * 4;

	screen_drawBackground();

	SDL_Surface *face;
	
	flushInput();

	while (true)
	{
		renderer_update();
		screen_unBuffer();
		getPlayerInput();
		game_doStars();
		game_doExplosions();

		for (int i = 0 ; i < 15 ; i++)
		{
			if (aliens[i].active)
			{
				explosion_addEngine(&aliens[i]);
				if (scene == 0 && i > 0 && (timer % 15) == i) {
					aliens[i].dx += (drand48() - 0.5) * 0.1;
					aliens[i].dy += (drand48() - 0.5) * 0.1;
					if (aliens[i].x > 500 - timer)
						aliens[i].dx -= 0.2;
					if (aliens[i].x < 0)
						aliens[i].dx += 0.2;
				}
				aliens[i].x += aliens[i].dx;
				aliens[i].y += aliens[i].dy;
				aliens[i].x += engine.ssx + engine.smx;
				screen_blit(aliens[i].image[0], (int)aliens[i].x, (int)aliens[i].y);
				if (aliens[i].x > (screen->w + 50))
				{
					aliens[i].x = -50;
					aliens[i].y = rand() % (screen->h - 40);
				}
				if (aliens[i].y < -50)
					aliens[i].y = (screen->h + 50);
				if (aliens[i].y > (screen->h + 50))
					aliens[i].y = -50;
			}
		}

		timer--;
		if (timer == 0)
		{
			showMessage = !showMessage;
			timer = 120;
			if (showMessage)
			{
				timer = 60 * 7;
				currentMessage++;

				if (currentMessage == 10)
					break;

				if (strcmp(messages[currentMessage].message, "") == 0)
					break;

				face = NULL;
				if (messages[currentMessage].face != -1)
					face = gfx_faceSprites[messages[currentMessage].face];
				gfx_createMessageBox(face, messages[currentMessage].message, 0);
			}
		}

		if ((showMessage) && (gfx_messageBox != NULL))
			screen_blit(gfx_messageBox, (screen->w - gfx_messageBox->w) / 2, screen->h - 100);

		screen_renderString("何かキーを押すとスキップする", -1, 580, FONT_WHITE);

		game_delayFrame();

		if ((engine.keyState[KEY_ESCAPE]) || (engine.keyState[KEY_FIRE]) ||
				(engine.keyState[KEY_ALTFIRE]))
			break;
	}

	screen_flushBuffer();
	gfx_free();
	screen_clear(black);
	renderer_update();
}
