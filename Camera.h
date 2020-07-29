#pragma once
#include "Vector.h"

class Camera
{
public:
	char pad_0000[4]; //0x0000
	void* N00000A95; //0x0004
	char pad_0008[4]; //0x0008
	Vector3 World; //0x000C
	char pad_0018[16]; //0x0018
	float fov; //0x0028
	Vector3 lookAt; //0x002C
	Vector3 up; //0x0038
	Vector3 velocity; //0x0044
	char pad_0050[36]; //0x0050
	Vector3 World2; //0x0074
	char pad_0080[20]; //0x0080
	Vector3 N00000AAF; //0x0094
	Vector3 N00000AB0; //0x00A0
	float fov2; //0x00AC
	Vector3 N00000AB2; //0x00B0
	char pad_00BC[16]; //0x00BC
	float fov3; //0x00CC
	Vector3 N00000AB8; //0x00D0
	Vector3 N00000AB9; //0x00DC
	char pad_00E8[444]; //0x00E8
}; //Size: 0x02A4