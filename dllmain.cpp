// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <iostream>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "detours.h"
#pragma comment(lib, "detours.lib")

#include "EntityEx.h"
#include "CameraEx.h"
#include "D3D9Helper.h"

HINSTANCE DllHandle;

//Endsceene Hooking
typedef HRESULT(__stdcall* endScene)(IDirect3DDevice9* pDevice);
endScene pEndScene;

D3D9Helper d3D9Helper;

//Menu
float menuX = 35, menuWidth = 220, menuY = 100, menuHeight = 70;
float padding = 10;
float lineHeight = 20;
std::vector<std::string> lines = { "Press Numpad0 for ESP", "Press Numpad1 to Exit" };


//ESP
bool drawESP = false;
std::vector<EntityEx> entities;
uintptr_t pObjectTableBase = 0x400506EC;
uintptr_t pMasterChief = 0x40102B04;
BYTE nEntitesOffset = 0x08;
CameraEx cameraEx = CameraEx();

std::vector<EntityEx> loadEntities() {
    int failCounter = 0;
    std::vector<EntityEx> entities;
    //int nEntites = *(__int16*)(objectTableBase - nEntitesOffset);
    //std::cout << "#Entites according to memory " << nEntites << std::endl;
    for (int i = 0; i < 9999; i++) {
        uintptr_t pointer = *(uintptr_t*)(pObjectTableBase + 0x08 + i * 12);
        if (pointer && pointer > 0x400506EC) {
            failCounter = 0;
            Entity* entity = (Entity*)pointer;
            __int16 typeID = *(__int16*)(pObjectTableBase + 0x08 + i * 12 - 0x02);
            //std::cout << std::hex << pointer << std::endl;
            if(entity->health > 0)
                entities.emplace_back(entity, typeID);
        }
        else {
            failCounter++;
        }
        if (failCounter > 5)
            return entities;
    }
    //std::cout << "#Entites found: " << std::dec << entities.size() << std::endl;
    return entities;
}

HRESULT __stdcall hookedEndScene(IDirect3DDevice9* pDevice) {
    d3D9Helper.pDevice = pDevice;

    d3D9Helper.drawFilledRectangle(menuX, menuY, menuWidth, menuHeight, D3DCOLOR_ARGB(120, 54, 162, 255));
    for (size_t i = 0; i < lines.size();++i) {
        d3D9Helper.drawText(lines.at(i), menuX + padding, menuY + padding + i * lineHeight, D3DCOLOR_ARGB(255, 153, 255, 153));
    }

    //ESP
    if (drawESP) {
        entities = loadEntities();
        for (EntityEx entityEx : entities) {
            if (entityEx.entity) {
                Vector3 feetCoords = cameraEx.WorldToScreen(entityEx.entity->feet);
                if (feetCoords.z < 100) {//screenCoords.x > 0 && screenCoords.y > 0 && screenCoords.x < 1 && screenCoords.y < 1 && 
                    //std::cout << screenCoords.x << ", " << screenCoords.y << ", " << screenCoords.z << std::endl;
                    Vector3 torsoCoords = cameraEx.WorldToScreen(entityEx.entity->torso);
                    float heightEntity = abs(feetCoords.y - torsoCoords.y)  * 2;
                    //text
                    std::string str = "health: " + std::to_string((int)(entityEx.entity->health * 100));
                    //str = "typeID: " + std::to_string(entity.typeID);
                    d3D9Helper.drawText(str, torsoCoords.x -20, torsoCoords.y - heightEntity / 1.2, D3DCOLOR_ARGB(255, 153, 255, 153));
                    //box
                    D3DCOLOR boxColor = D3DCOLOR_ARGB(255, 255, 0, 0); //red
                    if (entityEx.typeID == 3680)
                        boxColor = D3DCOLOR_ARGB(255, 0, 255, 0); //green
                    float widthEntity = heightEntity / 2;
                    d3D9Helper.drawRectangle(feetCoords.x - widthEntity/2, torsoCoords.y  - heightEntity / 2, widthEntity, heightEntity, boxColor);
                }
            }
        }
    }

    return pEndScene(pDevice); // call original endScene 
}

void hook() {
    pEndScene = (endScene)DetourFunction((PBYTE)d3D9Helper.vTable[42],(PBYTE)hookedEndScene);
}

void unhook() {
    DetourRemove((PBYTE)pEndScene, (PBYTE)hookedEndScene);
}

DWORD __stdcall EjectThread(LPVOID lpParameter) {
    Sleep(100);
    FreeLibraryAndExitThread(DllHandle, 0);
    return 0;
}

DWORD WINAPI Menue(HINSTANCE hModule) {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout); //sets cout to be used with our newly created console

    if (!d3D9Helper.initVTable()) {
        std::cout << "could not init d3D9Helper exiting!" << std::endl;
        Sleep(1000);
        fclose(fp);
        FreeConsole();
        CreateThread(0, 0, EjectThread, 0, 0, 0);
        return 0;
    }

    hook();

    while (true) {
        Sleep(50);
        if (GetAsyncKeyState(VK_NUMPAD0)) {
            drawESP = !drawESP;
            Sleep(1000);
        }
        if (GetAsyncKeyState(VK_NUMPAD1)) {
            unhook();
            d3D9Helper.cleanup();
            break;
        }
    }
    std::cout << "ight imma head out" << std::endl;
    Sleep(1000);
    fclose(fp);
    FreeConsole();
    CreateThread(0, 0, EjectThread, 0, 0, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DllHandle = hModule;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Menue, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

