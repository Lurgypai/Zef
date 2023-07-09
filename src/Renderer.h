#pragma once
#include "Cube.h"
#include "Cuboid.h"
#include "Camera.h"
#include "Face.h"
#include "Texture.h"

#define RENDER_NODE_COUNT 400
#define NULL_RENDER_NODE (RENDER_NODE_COUNT + 1)

static LCDPattern kColorChekerboard = { 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static LCDPattern kColorTest = { 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static LCDPattern kColorTiled = { 0xff, 0x81, 0xbd, 0xa5, 0xa5, 0xbd, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static LCDPattern kColorTiledDark = { 0xff, 0xd5, 0xbf, 0xf5, 0xaf, 0xfd, 0xab, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

void DrawCube(PlaydateAPI* pd, Cube* cube, Camera* cam);
void DrawCuboid(PlaydateAPI* pd, Cuboid* cuboid, Camera* cam);
void DrawCuboidTextured(PlaydateAPI* pd, Cuboid* cuboid, Texture* texture, Camera* cam);

int GetFaceTransformed(Face* face, Camera* cam, int* x, int* width, int* z);
void DrawFace(PlaydateAPI* pd, Face* face, Camera* cam);
void DrawFaceBlack(PlaydateAPI* pd, Face* face, Camera* cam);
void DrawFaceCheckered(PlaydateAPI* pd, Face* face, Camera* cam);
void DrawFaceTest(PlaydateAPI* pd, Face* face, Camera* cam);
void DrawFaceTiled(PlaydateAPI* pd, Face* face, Camera* cam);
void DrawFaceTiledDark(PlaydateAPI* pd, Face* face, Camera* cam);
void DrawFaceTextured(PlaydateAPI* pd, Face* face, Texture* t, Camera* cam);

void BufferCuboidTextured(PlaydateAPI* pd, Cuboid* cuboid, Texture* texture, Camera* cam);
void BufferFaceTextured(PlaydateAPI* pd, Face* face, Texture* t, Camera* cam);
void DrawBuffered(PlaydateAPI* pd, Camera* cam);

typedef struct RenderBufferNode_ {
	unsigned int i;
	unsigned int next;
	Texture* texture;
	int x;
	int y;
	int z;
	int width;
} RenderBufferNode;

void AllocRenderBufferNodeArr(PlaydateAPI* pd);
void FreeRenderBufferNodeArr(PlaydateAPI* pd);

