#pragma once

#include "pd_api.h"
#include "Renderer.h"

void DrawCube(PlaydateAPI* pd, Cube* cube, Camera* cam) {

	float x = cube->x * CUBE_SIZE;
	float y = cube->y * CUBE_SIZE;
	float z = cube->z * CUBE_SIZE;

	// float x = cube->x;
	// float y = cube->y;
	// float z = cube->z;

	Face f1 = {
		.x = x,
		.y = y,
		.z = z,
		.width = CUBE_SIZE,
		.height = CUBE_SIZE,
		.depth = 0,
	};
	Face f2 = {
		.x = x + CUBE_SIZE,
		.y = y,
		.z = z,
		.width = 0,
		.height = CUBE_SIZE,
		.depth = CUBE_SIZE,
	};
	Face f3 = {
		.x = x + CUBE_SIZE,
		.y = y,
		.z = z + CUBE_SIZE,
		.width = -CUBE_SIZE,
		.height = CUBE_SIZE,
		.depth = 0,
	};
	Face f4 = {
		.x = x,
		.y = y,
		.z = z + CUBE_SIZE,
		.width = 0,
		.height = CUBE_SIZE,
		.depth = -CUBE_SIZE,
	};

	DrawFaceBlack(pd, &f1, cam);
	DrawFaceCheckered(pd, &f2, cam);
	DrawFaceBlack(pd, &f3, cam);
	DrawFaceCheckered(pd, &f4, cam);
}

void DrawCuboid(PlaydateAPI* pd, Cuboid* cuboid, Camera* cam) {
	float x = cuboid->x;
	float y = cuboid->y;
	float z = cuboid->z;

	Face f1 = {
		.x = x,
		.y = y,
		.z = z,
		.width = cuboid->width,
		.height = cuboid->height,
		.depth = 0,
	};
	Face f2 = {
		.x = x + cuboid->width,
		.y = y,
		.z = z,
		.width = 0,
		.height = cuboid->height,
		.depth = cuboid->depth,
	};
	Face f3 = {
		.x = x + cuboid->width,
		.y = y,
		.z = z + cuboid->depth,
		.width = -cuboid->width,
		.height = cuboid->height,
		.depth = 0,
	};
	Face f4 = {
		.x = x,
		.y = y,
		.z = z + cuboid->depth,
		.width = 0,
		.height = cuboid->height,
		.depth = -cuboid->depth,
	};

	switch (cuboid->type) {
	case 0:
		DrawFaceBlack(pd, &f1, cam);
		DrawFaceTest(pd, &f2, cam);
		DrawFaceBlack(pd, &f3, cam);
		DrawFaceTest(pd, &f4, cam);
		break;
	case 1:
		DrawFaceTest(pd, &f1, cam);
		DrawFaceBlack(pd, &f2, cam);
		DrawFaceTest(pd, &f3, cam);
		DrawFaceBlack(pd, &f4, cam);
		break;
	case 2:
		DrawFaceTest(pd, &f1, cam);
		DrawFaceTiled(pd, &f2, cam);
		DrawFaceTest(pd, &f3, cam);
		DrawFaceTiled(pd, &f4, cam);
		break;
	case 3:
		DrawFaceTiled(pd, &f1, cam);
		DrawFaceTiledDark(pd, &f2, cam);
		DrawFaceTiled(pd, &f3, cam);
		DrawFaceTiledDark(pd, &f4, cam);
		break;
	}
}

int GetFaceTransformed(Face* face, Camera* cam, int* x, int* width) {
	float newFaceXMin = face->x - cam->x;
	float newFaceZMin = face->z - cam->z;

	float newFaceXMax = newFaceXMin + face->width;
	float newFaceZMax = newFaceZMin + face->depth;

	float mat[2][2] = { { cosf(-cam->angle), -sinf(-cam->angle) },
						{ sinf(-cam->angle), cosf(-cam->angle) } };

	int finalFaceZMin = mat[0][0] * newFaceZMin + mat[0][1] * newFaceXMin;
	if (finalFaceZMin < 0) return 0;
	int finalFaceZMax = mat[0][0] * newFaceZMax + mat[0][1] * newFaceXMax;
	if (finalFaceZMax < 0) return 0;

	int finalFaceXMin = mat[1][0] * newFaceZMin + mat[1][1] * newFaceXMin;
	int finalFaceXMax = mat[1][0] * newFaceZMax + mat[1][1] * newFaceXMax;
	float newWidth = finalFaceXMax - finalFaceXMin;
	if (newWidth < 0) return 0;

	*x = finalFaceXMin;
	*width = newWidth;
	return 1;
}

void DrawFace(PlaydateAPI* pd, Face* face, Camera* cam) {
	int x = 0, width = 0;
	if (!GetFaceTransformed(face, cam, &x, &width)) return;
	pd->graphics->drawRect(x, face->y - cam->y, width, face->height, kColorBlack);
}

void DrawFaceBlack(PlaydateAPI* pd, Face* face, Camera* cam) {
	int x = 0, width = 0;
	if (!GetFaceTransformed(face, cam, &x, &width)) return;
	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, kColorBlack);
}

void DrawFaceCheckered(PlaydateAPI* pd, Face* face, Camera* cam) {
	int x = 0, width = 0;
	if (!GetFaceTransformed(face, cam, &x, &width)) return;
	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, (void*)kColorChekerboard);
}

void DrawFaceTest(PlaydateAPI* pd, Face* face, Camera* cam) {
	int x = 0, width = 0;
	if (!GetFaceTransformed(face, cam, &x, &width)) return;
	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, (void*)kColorTest);
}

void DrawFaceTiled(PlaydateAPI* pd, Face* face, Camera* cam) {
	int x = 0, width = 0;
	if (!GetFaceTransformed(face, cam, &x, &width)) return;
	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, (void*)kColorTiled);
}

void DrawFaceTiledDark(PlaydateAPI* pd, Face* face, Camera* cam)
{
	int x = 0, width = 0;
	if (!GetFaceTransformed(face, cam, &x, &width)) return;
	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, (void*)kColorTiledDark);
}
