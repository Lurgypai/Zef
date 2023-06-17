#pragma once
#include "Cube.h"
#include "Cuboid.h"
#include "Camera.h"
#include "Face.h"
#include "Texture.h"

static LCDPattern kColorChekerboard = { 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static LCDPattern kColorTest = { 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static LCDPattern kColorTiled = { 0xff, 0x81, 0xbd, 0xa5, 0xa5, 0xbd, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static LCDPattern kColorTiledDark = { 0xff, 0xd5, 0xbf, 0xf5, 0xaf, 0xfd, 0xab, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

void DrawCube(PlaydateAPI* pd, Cube* cube, Camera* cam);
void DrawCuboid(PlaydateAPI* pd, Cuboid* cuboid, Camera* cam);
void DrawCuboidTextured(PlaydateAPI* pd, Cuboid* cuboid, Texture* texture, Camera* cam);

int GetFaceTransformed(Face* face, Camera* cam, int* x, int* width);
void DrawFace(PlaydateAPI* pd, Face* face, Camera* cam);
void DrawFaceBlack(PlaydateAPI* pd, Face* face, Camera* cam);
void DrawFaceCheckered(PlaydateAPI* pd, Face* face, Camera* cam);
void DrawFaceTest(PlaydateAPI* pd, Face* face, Camera* cam);
void DrawFaceTiled(PlaydateAPI* pd, Face* face, Camera* cam);
void DrawFaceTiledDark(PlaydateAPI* pd, Face* face, Camera* cam);
void DrawFaceTextured(PlaydateAPI* pd, Face* face, Texture* t, Camera* cam);
