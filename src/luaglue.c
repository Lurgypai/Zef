#include "luaglue.h"
#include "Renderer.h"

static PlaydateAPI* pd;

static void* getObj(int n, char* type)
{
	void* obj = pd->lua->getArgObject(n, type, NULL);

	if (obj == NULL)
		pd->system->error("object of type %s not found at stack position %i", type, n);

	return obj;
}

static Camera* getCamera(int n) { return getObj(n, "fez.camera"); }
static Cuboid* getCuboid(int n) { return getObj(n, "fez.cuboid"); }
static Texture* getTexture(int n) { return getObj(n, "fez.texture"); }

static int camera_gc(lua_State* state) {
	Camera* cam = getCamera(1);
	pd->system->realloc(cam, 0);
	return 0;
}

static int camera_new(lua_State* state) {
	Camera* cam = pd->system->realloc(NULL, sizeof(Camera));

	cam->x = pd->lua->getArgFloat(1);
	cam->y = pd->lua->getArgFloat(2);
	cam->z = pd->lua->getArgFloat(3);
	cam->angle = pd->lua->getArgFloat(4);

	pd->lua->pushObject(cam, "fez.camera", 0);
	return 1;
}

static int camera_rotateCameraAround(lua_State* state) {
	Camera* cam = getCamera(1);
	float x = pd->lua->getArgFloat(2);
	float z = pd->lua->getArgFloat(3);
	float amount = pd->lua->getArgFloat(4);
	RotateCameraAround(pd, cam, x, z, amount);
	return 0;
}

static int camera_aimToSubject(lua_State* state) {
	Camera* cam = getCamera(1);
	float subjectX = pd->lua->getArgFloat(2);
	float subjectY = pd->lua->getArgFloat(3);
	float subjectZ = pd->lua->getArgFloat(4);
	float distance = pd->lua->getArgFloat(5);
	float angle = pd->lua->getArgFloat(6);

	AimCameraToSubject(cam, subjectX, subjectY, subjectZ, distance, angle);
	return 0;
}

static int camera_index(lua_State* L)
{
	if (pd->lua->indexMetatable()) return 1;

	Camera* cam = getCamera(1);
	const char* arg = pd->lua->getArgString(2);

	if (strcmp(arg, "x") == 0)
		pd->lua->pushFloat(cam->x);
	else if (strcmp(arg, "y") == 0)
		pd->lua->pushFloat(cam->y);
	else if (strcmp(arg, "z") == 0)
		pd->lua->pushFloat(cam->z);
	else if (strcmp(arg, "angle") == 0)
		pd->lua->pushFloat(cam->angle);
	else
		pd->lua->pushNil();

	return 1;
}

static int camera_newindex(lua_State* L)
{
	Camera* cam = getCamera(1);
	const char* arg = pd->lua->getArgString(2);

	if (strcmp(arg, "x") == 0)
		cam->x = pd->lua->getArgFloat(3);
	else if (strcmp(arg, "y") == 0)
		cam->y = pd->lua->getArgFloat(3);
	else if (strcmp(arg, "z") == 0)
		cam->z = pd->lua->getArgFloat(3);
	else if (strcmp(arg, "angle") == 0)
		cam->angle = pd->lua->getArgFloat(3);

	return 0;
}

static const lua_reg cameraLib[] =
{
	{ "__gc", 				camera_gc },
	{ "__index",			camera_index },
	{ "__newindex",			camera_newindex },
	{ "new",				camera_new },
	{ "rotateAround",		camera_rotateCameraAround},
	{ "aimToSubject",		camera_aimToSubject},
	{ NULL,				NULL }
};

static int cuboid_gc(lua_State* state) {
	Cuboid* cuboid = getCuboid(1);
	pd->system->realloc(cuboid, 0);
	return 0;
}

static int cuboid_index(lua_State* state) {
	Cuboid* cuboid = getCuboid(1);
	const char* arg = pd->lua->getArgString(2);

	if (strcmp(arg, "x") == 0)
		pd->lua->pushFloat(cuboid->x);
	else if (strcmp(arg, "y") == 0)
		pd->lua->pushFloat(cuboid->y);
	else if (strcmp(arg, "z") == 0)
		pd->lua->pushFloat(cuboid->z);
	else if (strcmp(arg, "width") == 0)
		pd->lua->pushFloat(cuboid->width);
	else if (strcmp(arg, "height") == 0)
		pd->lua->pushFloat(cuboid->height);
	else if (strcmp(arg, "depth") == 0)
		pd->lua->pushFloat(cuboid->depth);
	else if (strcmp(arg, "type") == 0)
		pd->lua->pushFloat(cuboid->type);
	else
		pd->lua->pushNil();

	return 1;
}

static int cuboid_newindex(lua_State* state) {
	Cuboid* cuboid = getCuboid(1);
	const char* arg = pd->lua->getArgString(2);

	if (strcmp(arg, "x") == 0)
		cuboid->x = pd->lua->getArgFloat(3);
	else if (strcmp(arg, "y") == 0)
		cuboid->y = pd->lua->getArgFloat(3);
	else if (strcmp(arg, "z") == 0)
		cuboid->z = pd->lua->getArgFloat(3);
	else if (strcmp(arg, "width") == 0)
		cuboid->width = pd->lua->getArgFloat(3);
	else if (strcmp(arg, "height") == 0)
		cuboid->height = pd->lua->getArgFloat(3);
	else if (strcmp(arg, "depth") == 0)
		cuboid->depth = pd->lua->getArgFloat(3);
	else if (strcmp(arg, "type") == 0)
		cuboid->type = pd->lua->getArgInt(3);

	return 0;
}

static int cuboid_new(lua_State* state) {
	Cuboid* cuboid = pd->system->realloc(NULL, sizeof(Cuboid));
	cuboid->x = pd->lua->getArgFloat(1);
	cuboid->y = pd->lua->getArgFloat(2);
	cuboid->z = pd->lua->getArgFloat(3);
	cuboid->width = pd->lua->getArgFloat(4);
	cuboid->height = pd->lua->getArgFloat(5);
	cuboid->depth = pd->lua->getArgFloat(6);
	cuboid->type = pd->lua->getArgInt(7);

	pd->lua->pushObject(cuboid, "fez.cuboid", 0);
	return 1;
}

static const lua_reg cuboidLib[] =
{
	{ "__gc",				cuboid_gc},
	{ "__index",			cuboid_index },
	{ "__newindex",			cuboid_newindex },
	{ "new",				cuboid_new },
	{NULL, NULL}
};

static int fez_drawCuboid(lua_State* state) {
	Cuboid* cuboid = getCuboid(1);
	Camera* camera = getCamera(2);

	DrawCuboid(pd, cuboid, camera);
	return 0;
}

static int fez_drawCuboidTextured(lua_State* state) {
	Cuboid* cuboid = getCuboid(1);
	Texture* texture = getTexture(2);
	Camera* camera = getCamera(3);

	DrawCuboidTextured(pd, cuboid, texture, camera);
	return 0;
}

static int fez_bufferCuboidTextured(lua_State* state) {
	Cuboid* cuboid = getCuboid(1);
	Texture* texture = getTexture(2);
	Camera* camera = getCamera(3);

	BufferCuboidTextured(pd, cuboid, texture, camera);
	return 0;
}

static int fez_drawBuffered(lua_State* state) {
	Camera* cam = getCamera(1);
	DrawBuffered(pd, cam);
	return 0;
}

static int texture_gc(lua_State* state) {
	Texture* texture = getTexture(1);
	DeleteTexture(pd, texture);

	return 0;
}

static int texture_new(lua_State* state) {
	char* path = pd->lua->getArgString(1);
	Texture* texture = LoadTexture(pd, path);

	if (!texture) pd->lua->pushNil();
	else pd->lua->pushObject(texture, "fez.texture", 0);

	return 1;
}

static const lua_reg textureLib[] =
{
	{ "__gc",				texture_gc},
	{ "new",				texture_new },
	{NULL, NULL}
};

void RegisterLua(PlaydateAPI* pd_) {
	pd = pd_;
	pd->system->logToConsole("Registering Fez API...");

	const char* err;
	if (!pd->lua->addFunction(fez_drawCuboid, "fez.drawCuboid", &err)) {
		pd->system->logToConsole("%s:%i: addFunction failed, %s", __FILE__, __LINE__, err);
	}

	if (!pd->lua->addFunction(fez_drawCuboidTextured, "fez.drawCuboidTextured", &err)) {
		pd->system->logToConsole("%s:%i: addFunction failed, %s", __FILE__, __LINE__, err);
	}

	if (!pd->lua->addFunction(fez_bufferCuboidTextured, "fez.bufferCuboidTextured", &err)) {
		pd->system->logToConsole("%s:%i: addFunction failed, %s", __FILE__, __LINE__, err);
	}

	if (!pd->lua->addFunction(fez_drawBuffered, "fez.drawBuffered", &err)) {
		pd->system->logToConsole("%s:%i: addFunction failed, %s", __FILE__, __LINE__, err);
	}

	if (!pd->lua->registerClass("fez.camera", cameraLib, NULL, 0, &err)) {
		pd->system->logToConsole("%s:%i: registerClass failed, %s", __FILE__, __LINE__, err);
	}

	if (!pd->lua->registerClass("fez.cuboid", cuboidLib, NULL, 0, &err)) {
		pd->system->logToConsole("%s:%i: registerClass failed, %s", __FILE__, __LINE__, err);
	}

	if (!pd->lua->registerClass("fez.texture", textureLib, NULL, 0, &err)) {
		pd->system->logToConsole("%s:%i: registerClass failed, %s", __FILE__, __LINE__, err);
	}
}
