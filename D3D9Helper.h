#pragma once

#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

class D3D9Helper{
public:
	IDirect3DDevice9* pDevice = nullptr;
	void** vTable = nullptr;

	ID3DXFont* pTextFont = nullptr;
	ID3DXLine* line = nullptr;

	D3D9Helper();
	bool initVTable();
	void cleanup();

	void drawText(const char* String, float x, float y, D3DCOLOR color);
	void drawText(const char* String, float x, float y, int a, int r, int g, int b);
	void drawText(std::string text, float x, float y, D3DCOLOR color);

	void drawFilledRectangle(float x, float y, float width, float height, D3DCOLOR color);
	void drawRectangle(float x, float y, float width, float height, D3DCOLOR color);
};

