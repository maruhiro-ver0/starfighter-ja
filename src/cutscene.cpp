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
			strcpy(messages[0].message, "���ꥹ���Х�ե�����ɤϿ����˳��������إե������ե饤�٤�ƨ���������桢�Ĥ���WEAPCO�Υѥȥ������ª���줿��");

			messages[1].face = FS_CHRIS;
			strcpy(messages[1].message, "�ޤ��ɤä����Τ�!?");

			messages[2].face = FS_CHRIS;
			strcpy(messages[2].message, "�ޤ����δϤ���Ĥ˴���Ƥ��ʤ������ѥȥ���Ϥ򽸹������ޤ�Ϣ��ƹԤ��櫓�ˤϤ����ʤ���");

			messages[3].face = FS_CHRIS;
			strcpy(messages[3].message, "�Ĥ������臘���Ȥˤʤ�Τ����ʤ�Ф��δϤ���ǽ���Ƥ��!");

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
			strcpy(messages[0].message, "�����ָ塢���ꥹ���Х�ե�����ɤγ��������۷Ϥޤ�����ä���WEAPCO�δ�����ᴱ����ΰ����ˤĤ��ƹͤ��Ƥ�����");

			messages[1].face = FS_KLINE;
			strcpy(messages[1].message, "�벦���Ե��������ʡ����䥹�ԥ�åȷϤϤ���ȿ�ռԤΥѥ���åȤˤ�äƲ������줿��");

			messages[2].face = FS_KLINE;
			strcpy(messages[2].message, "���ˤ��С��桹�κǤ⶯�Ϥʥեꥲ���Ȥ���ΰ��ɤ��Ǥ��Τ᤹�Τ�3ʬ�ȳݤ���ʤ��ä�!");

			messages[3].face = FS_KLINE;
			strcpy(messages[3].message, "����ʺ�ǽ����ļԤϤ�ä��ˤ��ʤ����ۤϲ桹�ο�����AI�ȥ졼�˥󥰥ץ����θ���Ȥ��Ƥϴ�����!");

			messages[4].face = FS_KLINE;
			strcpy(messages[4].message, "�ۤ򻦤��ʤ���Фʤ�ʤ��Τ���ǰ����");

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
			strcpy(messages[0].message, "�⤦��������ʥ󥹷ϤؤΥ�פ��Ϥޤ롣");

			messages[1].face = FS_CHRIS;
			strcpy(messages[1].message, "�������ϤˤϤ��������WEAPCO�����줬����Τ���?");

			messages[2].face = FS_SID;
			strcpy(messages[2].message, "�����̤���������Ϥ��ζ�ϤǺǤ���ʾ���1�Ĥ���");

			messages[3].face = FS_CHRIS;
			strcpy(messages[3].message, "�����˹Ԥ���ͭ���ˤʤ��?");

			messages[4].face = FS_SID;
			strcpy(messages[4].message, "����������˷ײ褬���롣");

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
			strcpy(messages[0].message, "�����إåɥ�����! �⤷������Ʊ��Ź����ä��󤸤�ʤ�?");

			messages[1].face = FS_CHRIS;
			strcpy(messages[1].message, "��äȽ��פʤ��Ȥ����롣�������ǲ��򤷤Ƥ����? �����᤯�ˤ��Ƥ褫�ä���!");

			messages[2].face = FS_PHOEBE;
			strcpy(messages[2].message, "�Ф����õ���Ƥ���1�������ˤ��ʤ��ʤäƤ��ޤä��Ρ�");

			messages[3].face = FS_PHOEBE;
			strcpy(messages[3].message, "�Ԥä�! ���ʤ��������ԥ�åȷϤ����褿ȿ�𷳤Ǥ��礦! ��������! ���ä�äƤ���?");

			messages[4].face = FS_CHRIS;
			strcpy(messages[4].message, "���������Ϥ�? ���Ϻ���ʤ���! �ɤ���������?");

			messages[5].face = FS_SID;
			strcpy(messages[5].message, "��������������ͶƳ�ߥ�����ˤⶽ̣�����롣����Ͻ����ˤʤꤽ������");

			messages[6].face = FS_CHRIS;
			strcpy(messages[6].message, "���������櫓�����褦����!");

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
			strcpy(messages[0].message, "�������ä���������ꥹ�����������䤨�Ƥ�����");

			messages[1].face = FS_CHRIS;
			strcpy(messages[1].message, "WEAPCO�κη������˲�������������ľ��˥��饤�󡦥�������̾����ˤ˲�ä���");

			messages[2].face = FS_SID;
			strcpy(messages[2].message, "ʹ�������Ȥ����롣WEAPCO�δ�����ᴱ�Ǥ��ꡢ�Ǥ�ͥ���ʥѥ���åȤΰ�ͤ���");

			messages[3].face = FS_CHRIS;
			strcpy(messages[3].message, "�ۤϲ����Ϲ����ͤ��襤�˰�����������! �����������˵�äƹԤä���");

			messages[4].face = FS_CHRIS;
			strcpy(messages[4].message, "�����ꥷ�ɡ����ɡ���Ϥξ���Ϥ��뤫?");

			messages[5].face = FS_SID;
			strcpy(messages[5].message, "�߽Ф����ʳؼԤˤ��ȡ�WEAPCO�Ϥ����ǿ�����Ʈ���λ�򤷤Ƥ��뤽������");

			messages[6].face = FS_CHRIS;
			strcpy(messages[6].message, "�����Ĥ�֤ò����ơ���ȯ��ߤ�ͤФ�ʤ�ʡ�");

			messages[7].face = FS_SID;
			strcpy(messages[7].message, "�����̤����");

			messages[8].face = FS_PHOEBE;
			strcpy(messages[8].message, "�����˻Ф��󤬤���Τ�����ġ�");

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
			strcpy(messages[0].message, "�Ф���Ϥ������礦��?");

			messages[1].face = FS_SID;
			strcpy(messages[1].message, "��������ץ��ȤΤ褦��ʪ�����ä��������������ޤ����Ф餯�δ֤�Ƭ�ˤ����������");

			messages[2].face = FS_CHRIS;
			strcpy(messages[2].message, "�������פʤ��Ȥ�ʹ��������?");

			messages[3].face = FS_SID;
			strcpy(messages[3].message, "���������ޤǾ����Ԥ��ʤ���Фʤ�ʤ�������ץ��Ȥ����ä��Ȥ��Τ��Ȥϻפ��Ф��ʤ�������");

			messages[4].face = FS_SID;
			strcpy(messages[4].message, "����Ǥ�Ϥ���ĤϤǤ���������");

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
			strcpy(messages[0].message, "���ޤʤ����ڤ껥�򼺤äƤ��ޤä���");

			messages[1].face = FS_SID;
			strcpy(messages[1].message, "���ˤ���ʡ����˾�����ˡ�ǤϤʤ��ä����������WEAPCO���ݤ��䤹���ʤä���");

			messages[2].face = FS_SID;
			strcpy(messages[2].message, "���۷ϤǤ��襤�ϡ�����ʤ�Τˤʤ�������������˶��Ϥ��ɸ���ߤ��Ƥ����ʹ������");

			messages[3].face = FS_PHOEBE;
			strcpy(messages[3].message, "�ʤ�С���������Ϥ�ʤ��Ȥ͡�");

			messages[4].face = FS_SID;
			strcpy(messages[4].message, "�����ˤ��뷳�ĤϺ��ޤǤȤ������ۤʤ��Τ�����������Ĥ���");

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

		screen_renderString("���������򲡤��ȥ����åפ���", -1, 580, FONT_WHITE);

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
