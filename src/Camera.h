#pragma once
#include "pd_api.h"

#define CAM_WIDTH 400
#define CAM_HEIGHT 240

typedef struct Camera_ {
	float x;
	float y;
	float z;
	float angle;
} Camera;

// rotate the camera by amount around x, z
void RotateCameraAround(PlaydateAPI* pd, Camera* cam, float x, float z, float amount);

// aim the camera at the subject from distance away
void AimCameraToSubject(Camera* cam, float subjectX, float subjectY, float subjectZ, float distance, float angle);