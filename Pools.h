#include "Config.h"
#include <Windows.h>
#include "imgui/imgui.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")

namespace Pools
{
#ifndef _MAIN_H_
#define _MAIN_H_

	struct Players
	{
		DWORD64 ped;
		std::string name;
		ImVec4 position;
		float health;
		float armor;
	};
#endif//_MAIN_H_
	extern Players players[1024];

	extern void Tick();

	extern D3DXVECTOR3 getBone(uintptr_t ped, int bone_id);
}