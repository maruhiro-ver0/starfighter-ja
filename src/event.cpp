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
	"�ޤ��񤨤��ʡ��Х�ե������!",
	"ȿ�ռԤ衢����ǽ�����!",
	"����ϼ�˾�����ʤ��Ǥ����",
	"����ݤ���ȻפäƤ���Τ�?!"
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
			strcpy(events[0].message, "�ޤ��եꥲ���Ȥ��������˲��������줬�����ˤʤ�!");

			events[1].time = 120;
			events[1].face = FS_SID;
			strcpy(events[1].message, "���ꥹ���ޤ�! �եꥲ���ȤϤ⤦�����������!");

			events[2].time = 150;
			events[2].face = FS_SID;
			strcpy(events[2].message, "�եꥲ���Ȥϥ��Υ�ˤ����Ŷ���Ƥ���!");

			events[3].time = 170;
			events[3].face = FS_SID;
			strcpy(events[3].message, "���ꥹ���᤯����!!!");

			events[4].time = 180;
			events[4].face = FS_SID;
			strcpy(events[4].message, "���Ȥ������Ȥ���");
			events[4].entity = ALIEN_BOSS;
			events[4].flag = FL_LEAVESECTOR | FL_IMMORTAL;

			break;

		case MISN_NEROD:
			events[0].time = 2;
			events[0].face = FS_PHOEBE;
			strcpy(events[0].message, "���ä���! ���Υ磻�䡼���ڤ��ư����褦�ˤʤ��!");

			break;

		case MISN_ALLEZ:
			events[0].time = 2;
			events[0].face = FS_CREW;
			strcpy(events[0].message, "���˴��դ�! ���Τࡢ�����Ƥ���! �⤷���������˲����줿���");

			events[1].time = 6;
			events[1].face = FS_CHRIS;
			strcpy(events[1].message, "�ɤΤ��餤�λ��֤�ɬ�פ�?");

			events[2].time = 10;
			events[2].face = FS_CREW;
			strcpy(events[2].message, "2��3ʬ��!");

			events[3].time = 60;
			events[3].face = FS_CREW;
			strcpy(events[3].message, "�褷! ���ϻȤ���褦�ˤʤä�! ��ϥ��󥸥����");
			events[3].entity = ALIEN_FRIEND1;
			events[3].flag = -FL_NOFIRE;

			events[4].time = 80;
			events[4].face = FS_CREW;
			strcpy(events[4].message, "������! �ޤ���郎����ˤʤä�! �ɤ�����㤤�����?!");
			events[4].entity = ALIEN_FRIEND1;
			events[4].flag = FL_NOFIRE;

			events[5].time = 100;
			events[5].face = FS_CREW;
			strcpy(events[5].message, "���ꥹ�������Ƥ���!! �������ƥब�������! �����ǥå���ư���ʤ��ʤäƤ���!");
			events[5].entity = ALIEN_FRIEND1;
			events[5].flag = FL_DISABLED;

			events[6].time = 120;
			events[6].face = FS_CREW;
			strcpy(events[6].message, "�⤦��������");

			events[7].time = 140;
			events[7].face = FS_CREW;
			strcpy(events[7].message, "OK! ������������ϥ��󥸥�����꤬ʬ����С�");
			events[7].entity = ALIEN_FRIEND1;
			events[7].flag = -FL_DISABLED;

			events[8].time = 177;
			events[8].face = FS_CREW;
			strcpy(events[8].message, "���󥸥�ν�����λ! ȯ��!");

			events[9].time = 180;
			events[9].entity = ALIEN_FRIEND1;
			events[9].flag = FL_LEAVESECTOR | FL_IMMORTAL;

			break;

		case MISN_ELAMALE:
			events[0].time = 5;
			events[0].face = FS_PHOEBE;
			strcpy(events[0].message, "����������������! ��äȸ�Ҥ����Ƥ⤤���Ϥ�!");

			events[1].time = -8;
			events[1].face = FS_PHOEBE;
			strcpy(events[1].message, "����! 櫤�! ��ơ����ꥹ��������Υ��ޤ��礦!!");

			events[2].time = -9;
			events[2].entity = ALIEN_PHOEBE;
			events[2].flag = FL_LEAVESECTOR;

			events[3].time = -13;
			events[3].face = FS_SID;
			strcpy(events[3].message, "���ꥹ���ɤ����Ĥ��ġ�");

			events[4].time = -15;
			events[4].face = FS_KLINE;
			strcpy(events[4].message, "�Ĥ��˺Ǥ�����ˤ˲񤨤�!");

			events[5].time = -20;
			events[5].face = FS_CHRIS;
			strcpy(events[5].message, "��ץɥ饤�֤ΤȤ��κ����Ǹ��������ʡ�ï��?");

			events[6].time = -25;
			events[6].face = FS_KLINE;
			strcpy(events[6].message, "��ϥ��饤�󡦥�����󡣤��ξ����ʥܥ�����ߤ���褿!");

			events[7].time = -31;
			events[7].face = FS_CHRIS;
			strcpy(events[7].message, "���饤�󡦥������? �ե󡢤����֤�ȼ���������褦���ʡ�");

			events[8].time = -36;
			events[8].face = FS_KLINE;
			strcpy(events[8].message, "ƨ�������ʤ�����������ʤ��������ƽ����⤤�ʤ�! �ɤ����뤫�ڤ��ߤ���");

			events[9].time = -42;
			events[9].entity = ALIEN_KLINE;
			events[9].flag = -(FL_IMMORTAL | FL_NOFIRE);

			break;

		case MISN_ODEON:
			events[0].time = 2;
			events[0].face = FS_URSULA;
			strcpy(events[0].message, "��°�����δ�����ȯ��������������ɸ���˲����롣");

			events[1].time = 8;
			events[1].face = FS_CHRIS;
			strcpy(events[1].message, "�Ф����ͶƳ�ߥ���������㡼����äƤ���褦��! �ե����١��������ɥХ����Ϥ��뤫?");

			events[2].time = 14;
			events[2].face = FS_PHOEBE;
			strcpy(events[2].message, "����Фä��򤱤뤷���ʤ���!");

			break;

		case MISN_ALMARTHA:
			events[0].time = 1;
			events[0].face = FS_CHRIS;
			strcpy(events[0].message, "�����Ϥ�!");

			events[1].time = 7;
			events[1].face = FS_KRASS;
			strcpy(events[1].message, "�������������Ρ�������ä���������!");

			events[2].time = 60;
			events[2].face = FS_KRASS;
			strcpy(events[2].message, "���äȾ��Ρ�����䤬�ä���! �ɤ��⤪���Υ����ɤ�Ϥ��ޤ����äƤʤ��褦����!");
			events[2].entity = ALIEN_BOSS_PART1;
			events[2].flag = -FL_ACTIVATE;

			events[3].time = 90;
			events[3].entity = ALIEN_BOSS_PART2;
			events[3].flag = -FL_ACTIVATE;

			events[4].time = 93;
			events[4].face = FS_KRASS;
			strcpy(events[4].message, "�����Ĥ�򲶤θ��˶�Ť���ʡ�����ʤ��ȳ���������뤾!!!");

			events[5].time = 120;
			events[5].entity = ALIEN_BOSS_PART3;
			events[5].flag = -FL_ACTIVATE;

			events[6].time = 140;
			events[6].face = FS_PHOEBE;
			strcpy(events[6].message, "����ä�!! �����Ƥ��?");
			events[6].entity = ALIEN_PHOEBE;
			events[6].flag = -FL_ACTIVATE;

			events[7].time = 140;
			events[7].entity = ALIEN_URSULA;
			events[7].flag = -FL_ACTIVATE;

			events[8].time = 150;
			events[8].face = FS_KRASS;
			strcpy(events[8].message, "���Ͻ�ʬ�˲Ԥ��������㤢�ʾ���!");
			events[8].entity = ALIEN_FRIEND1;
			events[8].flag = FL_LEAVESECTOR;

			break;

		case MISN_POSWIC:
			events[0].time = 90;
			events[0].face = FS_SID;
			strcpy(events[0].message, "������! ƨ���褦�Ȥ��Ƥ��뤾! �ߤ����!");
			events[0].entity = ALIEN_BOSS;
			events[0].flag = FL_LEAVESECTOR;

			break;

		case MISN_SATURN:
			events[0].time = 45;
			events[0].face = FS_SID;
			strcpy(events[0].message, "���ꥹ! �̤�2���Υ졼����ˤ���ΰ�˶�Ť��Ƥ��뤾!");
			events[0].entity = ALIEN_BOSS_PART3;
			events[0].flag = -FL_ACTIVATE;

			events[1].time = 45;
			events[1].entity = ALIEN_BOSS_PART4;
			events[1].flag = -FL_ACTIVATE;

			events[2].time = 90;
			events[2].face = FS_SID;
			strcpy(events[2].message, "2�����夷��!");
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
			strcpy(events[1].message, "櫤�! ���Ȥ������Ȥ�! �����顼��!");

			events[2].time = 8;
			events[2].face = FS_KRASS;
			strcpy(events[2].message, "���ϸۤ�����������Ԥ��Ƥ������?! WEAPCO�Ϥ������Ʊ���褦�˲���ۤä���������");

			events[3].time = 14;
			events[3].face = FS_CHRIS;
			strcpy(events[3].message, "�������ʡ������˲������Ԥ��������Х����ä��衣");

			events[4].time = 15;
			events[4].entity = ALIEN_BOSS_PART2;
			events[4].flag = -FL_ACTIVATE;

			events[5].time = 20;
			events[5].face = FS_KRASS;
			strcpy(events[5].message, "�����᤯�����Ф졣���ڼ꤬�ԤäƤ��������Ρġ�");

			events[6].time = 25;
			events[6].face = FS_CHRIS;
			strcpy(events[6].message, "���ˤ�̾�������롣�����Ƥ��뤫?");

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
			strcpy(events[0].message, "����ǽ��������Х�ե�����ɡ����������������ä����������⤦�Ԥ��̤Ƥ�!");

			events[1].time = 8;
			events[1].face = FS_CHRIS;
			strcpy(events[1].message, "�����̤��! �����ˤϤ��ä�ƨ�����Ƥ������!");

			break;

		case MISN_VENUS:
			events[0].time = 2;
			events[0].face = FS_CHRIS;
			strcpy(events[0].message, "WEAPCO�Ͻ���ä����������! �����ˤ�ƨ������⡢�������⡢������ʤ���");

			events[1].time = 8;
			events[1].face = FS_KLINE;
			strcpy(events[1].message, "�����ϻ�򸫤��ӤäƤ��롢�Х�ե�����ɡ�");

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
