//
//  main.c
//  Extension
//
//  Created by Dave Hayden on 7/30/14.
//  Copyright (c) 2014 Panic, Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "luaglue.h"

const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInitLua ) RegisterLua(pd);

	
	return 0;
}


#define TEXT_WIDTH 86
#define TEXT_HEIGHT 16

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240

/*
Camera cam = {
	.x = 0,
	.y = 0,
	.z = 0,
	.angle = 0
};

static int update(void* userdata)
{
	PlaydateAPI* pd = userdata;
	
	pd->graphics->clear(kColorWhite);
	pd->graphics->setFont(font);
	//pd->graphics->drawText("Hello World!", strlen("Hello World!"), kASCIIEncoding, x, y);

	//RotateCameraAround(pd, &cam, cube.x, cube.z, angle);
	AimCameraToSubject(&cam, 0, 0, 0, 300, pd->system->getCrankAngle() * (3.14159 / 180));

	Cuboid c = {
		.x = 0,
		.y = -120,
		.z = 0,
		.width = 16,
		.height = 16,
		.depth = 16
	};
	for (int i = 0; i != 10; ++i) {
		c.x = i * 16;
		for (int j = 0; j != 10; ++j) {
			c.y = -100 + j * 16;
			for (int k = 0; k != 10; ++k)
			{
				c.z = k * 16;
				DrawCuboid(pd, &c, &cam);
			}
		}
	}
        
	pd->system->drawFPS(0,0);

	return 1;
}
*/

