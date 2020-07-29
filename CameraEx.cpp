#include "pch.h"
#include "CameraEx.h"

CameraEx::CameraEx(){
	camera = (Camera*)pCamera;
	windowHeigth = *(__int16*)pResolution;
	windowWidth = *(__int16*)(pResolution + 0x02);
	//aspectRatio = windowWidth / windowHeigth;
}

Vector3 CameraEx::WorldToScreen(Vector3 coord){
	float yFOV = camera->fov / windowWidth * windowHeigth;
	D3DXVECTOR3 camToObj(coord.x - camera->World.x, coord.y - camera->World.y, coord.z - camera->World.z );
	float distToObj = sqrt(camToObj.x * camToObj.x + camToObj.y * camToObj.y + camToObj.z * camToObj.z);
	D3DXVec3Normalize(&camToObj, &camToObj);

	// All angles are in radians, -PI<angle<PI, and no i dont know which could be equal to. but who cares - it's a float so it's impossible.
	float camYaw = atan2f(camera->lookAt.y, camera->lookAt.x);
	float objYaw = atan2f(camToObj.y, camToObj.x);

	// Relative(to cam) yaw ends up from -2 PI < yaw < 2 PI, but we want it from -PI<yaw<PI
	float relYaw = camYaw - objYaw;
	if (relYaw > D3DX_PI) // yaw>180 degrees. convert to negative, smaller.
		relYaw -= 2 * D3DX_PI;
	if (relYaw < -D3DX_PI)
		relYaw += 2 * D3DX_PI;
	// [/Difference]

	float objPitch = asin(camToObj.z);
	float camPitch = asin(camera->lookAt.z);

	float relPitch = camPitch - objPitch;
	//float x_pos = -relative_yaw * 2 / camera->fov; // radian angle measurement cancels here.
	//float y_pos = relative_pitch * 2 / Y_fov; // and that's the (relative pitch) / (fov / 2)

	float x = relYaw * 2 / camera->fov; // radian angle measurement cancels here.
	float y = relPitch * 2 / yFOV; // and that's the (relative pitch) / (fov / 2)

	x = (x + 1) / 2; // Lastly, change from range (-1,1) to (0,1)  Also, it CAN be outside of that range - if it's outside of the FOV.
	y = (y + 1) / 2;

	return { x * windowWidth, y * windowHeigth, distToObj };
}
