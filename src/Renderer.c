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

void DrawCuboidTextured(PlaydateAPI* pd, Cuboid* cuboid, Texture* texture, Camera* cam)
{
	float x = cuboid->x;
	float y = cuboid->y;
	float z = cuboid->z;

	Face f = {
		.x = x,
		.y = y,
		.z = z,
		.width = TEXTURE_WIDTH,
		.height = TEXTURE_WIDTH,
		.depth = 0,
	};
	// front
	for (int i = 0; i != cuboid->width / TEXTURE_WIDTH; ++i) {
		f.x = x + i * TEXTURE_WIDTH;
		for (int j = 0; j != cuboid->height / TEXTURE_WIDTH; ++j) {
			f.y = y + j * TEXTURE_WIDTH;
			DrawFaceTextured(pd, &f, texture, cam);
		}
	}
	

	// left
	f.x = x + cuboid->width;
	f.width = 0;
	f.depth = TEXTURE_WIDTH;
	for (int i = 0; i != cuboid->depth / TEXTURE_WIDTH; ++i) {
		f.z = z + i * TEXTURE_WIDTH;
		for (int j = 0; j != cuboid->height / TEXTURE_WIDTH; ++j) {
			f.y = y + j * TEXTURE_WIDTH;
			DrawFaceTextured(pd, &f, texture, cam);
		}
	}

	// back
	f.z = z + cuboid->depth;
	f.depth = 0;
	f.width = -TEXTURE_WIDTH;
	for (int i = 0; i != cuboid->width / TEXTURE_WIDTH; ++i) {
		f.x = x + cuboid->width - i * TEXTURE_WIDTH;
		for (int j = 0; j != cuboid->height / TEXTURE_WIDTH; ++j) {
			f.y = y + j * TEXTURE_WIDTH;
			DrawFaceTextured(pd, &f, texture, cam);
		}
	}

	// right
	f.x = x;
	f.depth = -TEXTURE_WIDTH;
	f.width = 0;
	for (int i = 0; i != cuboid->depth / TEXTURE_WIDTH; ++i) {
		f.z = z + cuboid->depth - i * TEXTURE_WIDTH;
		for (int j = 0; j != cuboid->height / TEXTURE_WIDTH; ++j) {
			f.y = y + j * TEXTURE_WIDTH;
			DrawFaceTextured(pd, &f, texture, cam);
		}
	}
	
}

int GetFaceTransformed(Face* face, Camera* cam, int* x, int* width, int* z) {
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
	if (newWidth <= 0) return 0;

	*x = finalFaceXMin;
	*width = newWidth;
	if (z != NULL) *z = finalFaceZMin;
	return 1;
}

void DrawFace(PlaydateAPI* pd, Face* face, Camera* cam) {
	int x = 0, width = 0;
	if (!GetFaceTransformed(face, cam, &x, &width, NULL)) return;
	pd->graphics->drawRect(x, face->y - cam->y, width, face->height, kColorBlack);
}

void DrawFaceBlack(PlaydateAPI* pd, Face* face, Camera* cam) {
	int x = 0, width = 0;
	if (!GetFaceTransformed(face, cam, &x, &width, NULL)) return;
	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, kColorBlack);
}

void DrawFaceCheckered(PlaydateAPI* pd, Face* face, Camera* cam) {
	int x = 0, width = 0;
	if (!GetFaceTransformed(face, cam, &x, &width, NULL)) return;
	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, (void*)kColorChekerboard);
}

void DrawFaceTest(PlaydateAPI* pd, Face* face, Camera* cam) {
	int x = 0, width = 0;
	if (!GetFaceTransformed(face, cam, &x, &width, NULL)) return;
	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, (void*)kColorTest);
}

void DrawFaceTiled(PlaydateAPI* pd, Face* face, Camera* cam) {
	int x = 0, width = 0;
	if (!GetFaceTransformed(face, cam, &x, &width, NULL)) return;
	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, (void*)kColorTiled);
}

void DrawFaceTiledDark(PlaydateAPI* pd, Face* face, Camera* cam)
{
	int x = 0, width = 0;
	if (!GetFaceTransformed(face, cam, &x, &width, NULL)) return;
	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, (void*)kColorTiledDark);
}

void DrawFaceTextured(PlaydateAPI* pd, Face* face, Texture* t, Camera* cam)
{
	int x = 0, width = 0;
	if (!GetFaceTransformed(face, cam, &x, &width, NULL)) return;
	pd->graphics->drawBitmap(t->bmps[width - 1], x, face->y - cam->y, kBitmapUnflipped);
}

void BufferCuboidTextured(PlaydateAPI* pd, Cuboid* cuboid, Texture* texture, Camera* cam)
{
	float x = cuboid->x;
	float y = cuboid->y;
	float z = cuboid->z;

	Face f = {
		.x = x,
		.y = y,
		.z = z,
		.width = TEXTURE_WIDTH,
		.height = TEXTURE_WIDTH,
		.depth = 0,
	};
	for (int i = 0; i != cuboid->width / TEXTURE_WIDTH; ++i) {
		f.x = x + i * TEXTURE_WIDTH;
		for (int j = 0; j != cuboid->height / TEXTURE_WIDTH; ++j) {
			f.y = y + j * TEXTURE_WIDTH;
			BufferFaceTextured(pd, &f, texture, cam);
		}
	}

	// left
	f.x = x + cuboid->width;
	f.width = 0;
	f.depth = TEXTURE_WIDTH;
	for (int i = 0; i != cuboid->depth / TEXTURE_WIDTH; ++i) {
		f.z = z + i * TEXTURE_WIDTH;
		for (int j = 0; j != cuboid->height / TEXTURE_WIDTH; ++j) {
			f.y = y + j * TEXTURE_WIDTH;
			BufferFaceTextured(pd, &f, texture, cam);
		}
	}

	// back
	f.z = z + cuboid->depth;
	f.depth = 0;
	f.width = -TEXTURE_WIDTH;
	for (int i = 0; i != cuboid->width / TEXTURE_WIDTH; ++i) {
		f.x = x + cuboid->width - i * TEXTURE_WIDTH;
		for (int j = 0; j != cuboid->height / TEXTURE_WIDTH; ++j) {
			f.y = y + j * TEXTURE_WIDTH;
			BufferFaceTextured(pd, &f, texture, cam);
		}
	}

	// right
	f.x = x;
	f.depth = -TEXTURE_WIDTH;
	f.width = 0;
	for (int i = 0; i != cuboid->depth / TEXTURE_WIDTH; ++i) {
		f.z = z + cuboid->depth - i * TEXTURE_WIDTH;
		for (int j = 0; j != cuboid->height / TEXTURE_WIDTH; ++j) {
			f.y = y + j * TEXTURE_WIDTH;
			BufferFaceTextured(pd, &f, texture, cam);
		}
	}
}


static RenderBufferNode* head = NULL;
static RenderBufferNode* renderBufferNodeArr = NULL;
static unsigned int allocRenderBufferNodePos = 0;

static RenderBufferNode* AllocRenderBufferNode(PlaydateAPI* pd) {
	if (allocRenderBufferNodePos == RENDER_NODE_COUNT) {
		pd->system->logToConsole("Out of RenderBufferNodes!");
		return NULL;
	}

	RenderBufferNode* node = &renderBufferNodeArr[allocRenderBufferNodePos];
	node->i = allocRenderBufferNodePos;
	node->next = NULL_RENDER_NODE;
	node->texture = NULL;
	++allocRenderBufferNodePos;
	return node;
}
static RenderBufferNode* FreeRenderBufferNodes() {
	allocRenderBufferNodePos = 0;
}

static RenderBufferNode* GetRenderBufferNode(unsigned int pos) {
	if (pos == NULL_RENDER_NODE) return NULL;
	return &renderBufferNodeArr[pos];
}


void BufferFaceTextured(PlaydateAPI* pd, Face* face, Texture* t, Camera* cam)
{

	int x = 0, width = 0, z = 0;
	if (!GetFaceTransformed(face, cam, &x, &width, &z)) return;

	RenderBufferNode* newNode = AllocRenderBufferNode(pd);
	newNode->texture = t;
	newNode->x = x;
	newNode->y = face->y;
	newNode->z = z;
	newNode->width = width;

	RenderBufferNode* currNode = NULL;
	RenderBufferNode* nextNode = head;

	// assign head
	if (head == NULL) {
		head = newNode;
		return;
	}

	while (nextNode != NULL) {
		if (newNode->z >= nextNode->z) {
			if (currNode != NULL) currNode->next = newNode->i;
			newNode->next = nextNode->i;
			//replace head
			if (nextNode == head) head = newNode;
			return;
		}

		currNode = nextNode;
		nextNode = GetRenderBufferNode(currNode->next);
	}

	// if we made it here we're at the end
	currNode->next = newNode->i;
}

void DrawBuffered(PlaydateAPI* pd, Camera* cam) {
	for (RenderBufferNode* currNode = head; currNode != NULL;) {
		pd->graphics->drawBitmap(currNode->texture->bmps[currNode->width - 1], currNode->x, currNode->y - cam->y, kBitmapUnflipped);
		currNode = GetRenderBufferNode(currNode->next);
	}
	head = NULL;
	FreeRenderBufferNodes();
}

void AllocRenderBufferNodeArr(PlaydateAPI* pd) {
	// initialize render buffer node arr
	renderBufferNodeArr = pd->system->realloc(NULL, sizeof(RenderBufferNode) * RENDER_NODE_COUNT);
}

void FreeRenderBufferNodeArr(PlaydateAPI* pd) {
	pd->system->realloc(renderBufferNodeArr, 0);
	renderBufferNodeArr = NULL;
}




