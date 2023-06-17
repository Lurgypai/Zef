#pragma once
#include "pd_api.h"

#define TEXTURE_WIDTH 32

// a renderable texture. these are always 32 x 32, and they come with squished versions at every resolution from 32 to 1 so that they can be rendered at all the necessary orientation
typedef struct Texture_ {
	//all possible texture widths
	LCDBitmap* bmps[TEXTURE_WIDTH];
	// z for rendering at different depths
	float z;
} Texture;

Texture* LoadTexture(PlaydateAPI* pd, const char* dir);
void DeleteTexture(PlaydateAPI* pd, Texture* t);