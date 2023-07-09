#include "Camera.h"

void RotateCameraAround(PlaydateAPI* pd, Camera* cam, float x, float z, float amount) {
	float tempCamZ = cam->z - z;
	float tempCamX = cam->x - x;

	float mat[2][2] = { { cosf(amount), -sinf(amount) },
						{ sinf(amount), cosf(amount) } };

	float newCamZ = mat[0][0] * tempCamZ + mat[0][1] * tempCamX;
	float newCamX = mat[1][0] * tempCamZ + mat[1][1] * tempCamX;

	cam->x = newCamX + x;
	cam->z = newCamZ + z;
	cam->angle += amount;
}


void AimCameraToSubject(Camera* cam, float subjectX, float subjectY, float subjectZ, float distance, float angle) {
	cam->y = subjectY - CAM_HEIGHT / 2;

	float camCenterZ = subjectZ - cosf(angle) * distance;
	float camCenterX = subjectX - sinf(angle) * distance;
	float b = 3.14159f / 2 - angle;

	cam->z = camCenterZ + cosf(b) * CAM_WIDTH / 2;
	cam->x = camCenterX - sinf(b) * CAM_WIDTH / 2;

	cam->angle = angle;
}

