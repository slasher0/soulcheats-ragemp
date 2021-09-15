#include "Pools.h"
#include "SDK.h"
#include "Math.h"
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")

namespace Pools
{
	Players players[1024];

	D3DXVECTOR3 getBone(uintptr_t ped, int bone_id) {
		auto matrix = *(D3DXMATRIX*)(ped + 0x60);
		auto bone = *(D3DXVECTOR3*)(ped + (0x430 + bone_id * 0x10));
		D3DXVECTOR4 transform;
		D3DXVec3Transform(&transform, &bone, &matrix);
		return D3DXVECTOR3(transform.x, transform.y, transform.z);
	}

	void Tick()
	{
		DWORD64 list_interface = NULL;
		DWORD64 list = NULL;
		int list_max_ptr = 0;

		__try
		{
			DWORD64 replayInterface = SDK::GetReplayInterface();
			if (Helper::ValidPTR(replayInterface))
			{
				list_interface = *(DWORD64*)(replayInterface + 0x18);
				list = *(DWORD64*)(list_interface + 0x100);
				list_max_ptr = *(int*)(list_interface + 0x108);
			}
		}
		__except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {}

		for (int i = 0; i < 1024; i++)
		{
			players[i].ped = NULL;
			players[i].position.x = 0.0f;

			if (i < list_max_ptr)
			{
				__try
				{
					DWORD64 ped = *(DWORD64*)(list + i * 0x10);
					if (Helper::ValidPTR(ped))
					{
						players[i].ped = ped;
						Vector3 pedPos = *(Vector3*)(ped + 0x90);
						players[i].position = ImVec4(pedPos.x, pedPos.y, pedPos.z, 0.0f);
						players[i].health = *(float*)(ped + 0x280) - 100.0f;
						players[i].armor = *(float*)(ped + 0x14E0);
					}
				}
				__except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {}
			}
		}

		//Unlock Doors
		if (Config::Vehicle::unlock_doors == 1)
		{
			DWORD64 list_interface;
			DWORD64 list;
			int list_max_ptr;

			__try
			{
				DWORD64 replayInterface = SDK::GetReplayInterface();
				list_interface = *(DWORD64*)(replayInterface + 0x10);
				list = *(DWORD64*)(list_interface + 0x180);
				list_max_ptr = *(int*)(list_interface + 0x188);
			}
			__except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {}


			for (int i = 0; i < list_max_ptr; i++)
			{
				__try
				{
					DWORD64 veh = *(DWORD64*)(list + i * 0x10);
					*(BYTE*)(veh + 0x1390) = (BYTE)1;
				}
				__except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {}
			}
		}
	}

	

}