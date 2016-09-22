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

static Event events[MAX_EVENTS];
static const char *klineGreeting[] = {
	"また会えたな、バンフィールド!",
	"反逆者よ、これで終わりだ!",
	"今回は失望させないでくれ…",
	"私を倒せると思っているのか?!"
};

void events_init()
{
	for (int i = 0 ; i < MAX_EVENTS ; i++)
	{
		events[i].time = 0;
		events[i].face = -1;
		strcpy(events[i].message, "");
		events[i].entity = -1;
		events[i].flag = 0;
	}

	switch (game.area)
	{
		case MISN_INTERCEPTION:
			if ((aliens[ALIEN_KLINE].classDef == CD_KLINE) &&
					(aliens[ALIEN_KLINE].active))
				events[0].time = 2;
				events[0].face = FS_KLINE;
				strcpy(events[0].message, klineGreeting[rand() % 4]);

			break;

		case MISN_MOEBO:
			events[0].time = 2;
			events[0].face = FS_SID;
			strcpy(events[0].message, "まずフリゲートの武装を破壊しろ。それが助けになる!");

			events[1].time = 120;
			events[1].face = FS_SID;
			strcpy(events[1].message, "クリス、急げ! フリゲートはもうすぐ射程内だ!");

			events[2].time = 150;
			events[2].face = FS_SID;
			strcpy(events[2].message, "フリゲートはカノン砲を装填している!");

			events[3].time = 170;
			events[3].face = FS_SID;
			strcpy(events[3].message, "クリス、早くしろ!!!");

			events[4].time = 180;
			events[4].face = FS_SID;
			strcpy(events[4].message, "何ということだ…");
			events[4].entity = ALIEN_BOSS;
			events[4].flag = FL_LEAVESECTOR | FL_IMMORTAL;

			break;

		case MISN_NEROD:
			events[0].time = 2;
			events[0].face = FS_PHOEBE;
			strcpy(events[0].message, "こっちよ! このワイヤーを切れば動けるようになるわ!");

			break;

		case MISN_ALLEZ:
			events[0].time = 2;
			events[0].face = FS_CREW;
			strcpy(events[0].message, "神に感謝だ! たのむ、助けてくれ! もしこの船が破壊されたら…");

			events[1].time = 6;
			events[1].face = FS_CHRIS;
			strcpy(events[1].message, "どのくらいの時間が必要だ?");

			events[2].time = 10;
			events[2].face = FS_CREW;
			strcpy(events[2].message, "2、3分だ!");

			events[3].time = 60;
			events[3].face = FS_CREW;
			strcpy(events[3].message, "よし! 武器は使えるようになった! 後はエンジンだ…");
			events[3].entity = ALIEN_FRIEND1;
			events[3].flag = -FL_NOFIRE;

			events[4].time = 80;
			events[4].face = FS_CREW;
			strcpy(events[4].message, "クソッ! また武器がダメになった! どうすりゃいいんだ?!");
			events[4].entity = ALIEN_FRIEND1;
			events[4].flag = FL_NOFIRE;

			events[5].time = 100;
			events[5].face = FS_CREW;
			strcpy(events[5].message, "クリス、助けてくれ!! 全システムがダウンだ! 全員デッキで動けなくなっている!");
			events[5].entity = ALIEN_FRIEND1;
			events[5].flag = FL_DISABLED;

			events[6].time = 120;
			events[6].face = FS_CREW;
			strcpy(events[6].message, "もう少しだ…");

			events[7].time = 140;
			events[7].face = FS_CREW;
			strcpy(events[7].message, "OK! 回復した。後はエンジンの問題が分かれば…");
			events[7].entity = ALIEN_FRIEND1;
			events[7].flag = -FL_DISABLED;

			events[8].time = 177;
			events[8].face = FS_CREW;
			strcpy(events[8].message, "エンジンの修理完了! 発進!");

			events[9].time = 180;
			events[9].entity = ALIEN_FRIEND1;
			events[9].flag = FL_LEAVESECTOR | FL_IMMORTAL;

			break;

		case MISN_ELAMALE:
			events[0].time = 5;
			events[0].face = FS_PHOEBE;
			strcpy(events[0].message, "何かがおかしい…! もっと護衛がいてもいいはず!");

			events[1].time = -8;
			events[1].face = FS_PHOEBE;
			strcpy(events[1].message, "クッ! 罠よ! 来て、クリス、ここを離れましょう!!");

			events[2].time = -9;
			events[2].entity = ALIEN_PHOEBE;
			events[2].flag = FL_LEAVESECTOR;

			events[3].time = -13;
			events[3].face = FS_SID;
			strcpy(events[3].message, "クリス、どうし…た……");

			events[4].time = -15;
			events[4].face = FS_KLINE;
			strcpy(events[4].message, "ついに最も危険な男に会えた!");

			events[5].time = -20;
			events[5].face = FS_CHRIS;
			strcpy(events[5].message, "ワープドライブのときの混線で見かけたな。誰だ?");

			events[6].time = -25;
			events[6].face = FS_KLINE;
			strcpy(events[6].message, "私はクライン・ケスラン。その小さなボロ船を止めに来た!");

			events[7].time = -31;
			events[7].face = FS_CHRIS;
			strcpy(events[7].message, "クライン・ケスラン? フン、ずいぶんと自信があるようだな。");

			events[8].time = -36;
			events[8].face = FS_KLINE;
			strcpy(events[8].message, "逃げる場所もなく、隠れる場所もなく、そして助けもいない! どうするか楽しみだ…");

			events[9].time = -42;
			events[9].entity = ALIEN_KLINE;
			events[9].flag = -(FL_IMMORTAL | FL_NOFIRE);

			break;

		case MISN_ODEON:
			events[0].time = 2;
			events[0].face = FS_URSULA;
			strcpy(events[0].message, "所属不明の艦船を発見。ただちに目標を破壊する。");

			events[1].time = 8;
			events[1].face = FS_CHRIS;
			strcpy(events[1].message, "姉さんも誘導ミサイルランチャーを持っているようだ! フェーベ、何かアドバイスはあるか?");

			events[2].time = 14;
			events[2].face = FS_PHOEBE;
			strcpy(events[2].message, "がんばって避けるしかないわ!");

			break;

		case MISN_ALMARTHA:
			events[0].time = 1;
			events[0].face = FS_CHRIS;
			strcpy(events[0].message, "いい艦だ!");

			events[1].time = 7;
			events[1].face = FS_KRASS;
			strcpy(events[1].message, "そうだろう、小僧。俺が作ったんだからな!");

			events[2].time = 60;
			events[2].face = FS_KRASS;
			strcpy(events[2].message, "おっと小僧、現れやがったぞ! どうもお前のダチどもはうまくいってないようだな!");
			events[2].entity = ALIEN_BOSS_PART1;
			events[2].flag = -FL_ACTIVATE;

			events[3].time = 90;
			events[3].entity = ALIEN_BOSS_PART2;
			events[3].flag = -FL_ACTIVATE;

			events[4].time = 93;
			events[4].face = FS_KRASS;
			strcpy(events[4].message, "こいつらを俺の後ろに近づけるな。さもないと割り増しを取るぞ!!!");

			events[5].time = 120;
			events[5].entity = ALIEN_BOSS_PART3;
			events[5].flag = -FL_ACTIVATE;

			events[6].time = 140;
			events[6].face = FS_PHOEBE;
			strcpy(events[6].message, "ちょっと!! 何してるの?");
			events[6].entity = ALIEN_PHOEBE;
			events[6].flag = -FL_ACTIVATE;

			events[7].time = 140;
			events[7].entity = ALIEN_URSULA;
			events[7].flag = -FL_ACTIVATE;

			events[8].time = 150;
			events[8].face = FS_KRASS;
			strcpy(events[8].message, "俺は十分に稼いだ。じゃあな小僧!");
			events[8].entity = ALIEN_FRIEND1;
			events[8].flag = FL_LEAVESECTOR;

			break;

		case MISN_POSWIC:
			events[0].time = 90;
			events[0].face = FS_SID;
			strcpy(events[0].message, "クソッ! 逃げようとしているぞ! 止めるんだ!");
			events[0].entity = ALIEN_BOSS;
			events[0].flag = FL_LEAVESECTOR;

			break;

		case MISN_SATURN:
			events[0].time = 45;
			events[0].face = FS_SID;
			strcpy(events[0].message, "クリス! 別の2機のレーザー砲が領域に近づいているぞ!");
			events[0].entity = ALIEN_BOSS_PART3;
			events[0].flag = -FL_ACTIVATE;

			events[1].time = 45;
			events[1].entity = ALIEN_BOSS_PART4;
			events[1].flag = -FL_ACTIVATE;

			events[2].time = 90;
			events[2].face = FS_SID;
			strcpy(events[2].message, "2機到着した!");
			events[2].entity = ALIEN_BOSS_PART5;
			events[2].flag = -FL_ACTIVATE;

			events[3].time = 90;
			events[3].entity = ALIEN_BOSS_PART6;
			events[3].flag = -FL_ACTIVATE;

			break;

		case MISN_JUPITER:
			events[0].time = 1;
			events[0].entity = ALIEN_BOSS_PART1;
			events[0].flag = -FL_ACTIVATE;

			events[1].time = 2;
			events[1].face = FS_URSULA;
			strcpy(events[1].message, "罠だ! 何ということだ! タイラーめ!");

			events[2].time = 8;
			events[2].face = FS_KRASS;
			strcpy(events[2].message, "俺は雇われだ。何を期待していたんだ?! WEAPCOはお前らと同じように俺を雇っただけだ。");

			events[3].time = 14;
			events[3].face = FS_CHRIS;
			strcpy(events[3].message, "そうだな。お前に何か期待した俺がバカだったよ。");

			events[4].time = 15;
			events[4].entity = ALIEN_BOSS_PART2;
			events[4].flag = -FL_ACTIVATE;

			events[5].time = 20;
			events[5].face = FS_KRASS;
			strcpy(events[5].message, "さあ早くくたばれ。小切手が待ってるんだ、小僧…。");

			events[6].time = 25;
			events[6].face = FS_CHRIS;
			strcpy(events[6].message, "俺にも名前がある。憶えているか?");

			events[7].time = 45;
			events[7].entity = ALIEN_BOSS_PART3;
			events[7].flag = -FL_ACTIVATE;

			events[8].time = 45;
			events[8].entity = ALIEN_BOSS_PART4;
			events[8].flag = -FL_ACTIVATE;

			break;

		case MISN_EARTH:
			events[0].time = 2;
			events[0].face = FS_KLINE;
			strcpy(events[0].message, "これで終わりだ、バンフィールド。たいした悪運だったが、それももう尽き果てた!");

			events[1].time = 8;
			events[1].face = FS_CHRIS;
			strcpy(events[1].message, "その通りだ! お前にはずっと逃げられてたからな!");

			break;

		case MISN_VENUS:
			events[0].time = 2;
			events[0].face = FS_CHRIS;
			strcpy(events[0].message, "WEAPCOは終わった、ケスラン! お前には逃げる場所も、隠れる場所も、助けもない。");

			events[1].time = 8;
			events[1].face = FS_KLINE;
			strcpy(events[1].message, "お前は私を見くびっている、バンフィールド。");

			break;
	}
}

void events_check()
{
	for (int i = 0 ; i < MAX_EVENTS ; i++)
	{
		if (engine.timeTaken == events[i].time)
		{
			if (strcmp(events[i].message, "") != 0)
			{
				setRadioMessage(events[i].face, events[i].message, 1);
			}

			if (events[i].entity > -1)
			{
				if (events[i].flag != -FL_ACTIVATE)
				{
					aliens[events[i].entity].flags += events[i].flag;
				}
				else
				{
					aliens[events[i].entity].active = true;
					aliens[events[i].entity].x = ((int)player.x +
						RANDRANGE(400, 800));
					aliens[events[i].entity].y = ((int)player.y +
						RANDRANGE(-400, 800));
				}
			}

			events[i].time = 0;
		}
	}
}

void events_sync()
{
	for (int i = 0 ; i < MAX_EVENTS ; i++)
	{
		if (events[i].time < 0)
		{
			events[i].time = engine.timeTaken + abs(events[i].time);
		}
	}
}
