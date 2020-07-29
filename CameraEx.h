#pragma once
#include "Camera.h"
#include <d3dx9math.h> //sqrt, D3DXVec3Normalize
//#include <iostream>

const uintptr_t pCamera = 0x006AC65C;
const uintptr_t pResolution  = 0x0069C638;

class CameraEx{
public:
	float windowWidth = 0, windowHeigth = 0;
	Camera* camera;

	CameraEx();

	Vector3 WorldToScreen(Vector3 coord);
};

