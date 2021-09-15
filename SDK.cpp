#include "SDK.h"
#include <string>

namespace SDK
{
	typedef BOOLEAN(__cdecl* WorldToScreen_t)(Vector3* WorldPos, float* x, float* y);
	WorldToScreen_t World2Screen = nullptr;
	tGET_RAYCAST_RESULT raycast_result_func = nullptr;
	ImVec2 WorldToScreen(Vector3 WorldPos)
	{
		ImVec2 pos;
		if (Helper::ValidPTR((DWORD64)World2Screen))
		{
			if (static_cast<BOOLEAN>(World2Screen(&WorldPos, &pos.x, &pos.y)))
			{
				pos.x = pos.x * ImGui::GetIO().DisplaySize.x;
				pos.y = pos.y * ImGui::GetIO().DisplaySize.y;
				return pos;
			}
		}
		return pos;
	}

	ImVec2 WorldToScreen2(D3DXVECTOR3 WorldPos)
	{
		return WorldToScreen(Vector3(WorldPos.x, WorldPos.y, WorldPos.z));
	}

	DWORD64 CameraViewAngles;
	DWORD64 GetCameraViewAngles()
	{
		if (Helper::ValidPTR(CameraViewAngles))
			return *(DWORD64*)(CameraViewAngles + 0x0);
	}

	DWORD64 ReplayInterface;
	DWORD64 GetReplayInterface()
	{
		if (Helper::ValidPTR(ReplayInterface))
			return *(DWORD64*)(ReplayInterface + 0x0);
	}

	DWORD64 World;
	DWORD64 LocalPlayer;

	give_weapon_t give_weapon;
	pizza_to_spaghetti_t pointer_to_handle;

	void Init()
	{
		const char* process = "GTA5.exe";

		if (Config::Others::fivem == 1)
		{
			process = "FiveM_GTAProcess.exe";
		}

		World = Helper::GetPointerAddress(Helper::FindPattern(process, "48 8B 05 ? ? ? ? 48 8B 58 08 48 85 DB 74 32"), 0x3);
		World = *(DWORD64*)(World + 0x0);
		LocalPlayer = *(DWORD64*)(World + 0x8);


		raycast_result_func = (tGET_RAYCAST_RESULT)Helper::FindPattern(process, "48 8B C4 48 89 58 08 48 90 70 10 48 89 78 18 4C 89 70 20 55 48 8D A8 00 00 00 00 48 81 EC 00 00 00 00 33 DB 45 8B F0 48 8B FA 48 8B F1 8B C3 45 85 C9 74 08 41 8B C9 E8 00 00 00 00 F3 0F 10 1F");
		CameraViewAngles = Helper::GetPointerAddress(Helper::FindPattern(process, "48 8B 05 ? ? ? ? 48 8B 98 ? ? ? ? EB 32"), 0x3);
		ReplayInterface = Helper::GetPointerAddress(Helper::FindPattern(process, "48 8D 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 48 8D 0D ? ? ? ? 8A D8 E8 ? ? ? ? 84 DB 75 13 48 8D 0D"), 0x3);
		World2Screen = (WorldToScreen_t)Helper::FindPattern(process, "48 89 5C 24 ?? 55 56 57 48 83 EC 70 65 4C 8B 0C 25 ?? 00 00 00 8B");

		give_weapon = (give_weapon_t)Helper::FindPattern(process, "XMR8B D8 48 85 C0 74 63");
		pointer_to_handle = (pizza_to_spaghetti_t)Helper::FindPattern(process, "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 8B 15 ? ? ? ? 48 8B F9 48 83 C1 10 33 DB");

	}

	namespace PLAYER
	{
		bool Valid()
		{
			if (Helper::ValidPTR(World))
			{
				LocalPlayer = *(DWORD64*)(World + 0x8);
				if (LocalPlayer != NULL)
					return true;
			}
			return false;
		}
		bool In_Vehicle()
		{
			if (PLAYER::Valid())
			{
				BYTE state = *(BYTE*)(LocalPlayer + 0x148B);
				if (state == 12)
					return true;
			}
			return false;
		}
		
		Vector3 Position()
		{
			if (PLAYER::Valid())
				return *(Vector3*)(LocalPlayer + 0x90);
		}
		void Set_Position(Vector3 vec)
		{
			if (PLAYER::Valid())
			{
				DWORD64 pos = *(DWORD64*)(LocalPlayer + 0x30);
				*(Vector3*)(pos + 0x50) = vec;

				*(Vector3*)(LocalPlayer + 0x90) = vec;
			}
			if (VEHICLE::Valid())
			{
				DWORD64 veh = *(DWORD64*)(LocalPlayer + 0xD28);
				DWORD64 pos = *(DWORD64*)(veh + 0x30);
				*(Vector3*)(pos + 0x50) = vec;
				*(Vector3*)(veh + 0x90) = vec;
			}
		}
		void set_frame_flag(uint32_t bit_flag)
		{
			*reinterpret_cast<uint32_t*>(*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(World + 0x8) + 0x10C8) + 0x1F8) |= bit_flag;
		}
		void Godmode(bool value)
		{
			if (PLAYER::Valid()) {
				if(value)
					*(float*)(LocalPlayer + 0x189) = 1;

				if(!value)
					*(float*)(LocalPlayer + 0x189) = 0;
			}
		}

		void Set_Health(float health)
		{
			if (PLAYER::Valid()) {
				*(float*)(LocalPlayer + 0x280) = health;
				*(float*)(LocalPlayer + 0x2A0) = health;
			}
		}


		float Get_Health()
		{
			if (PLAYER::Valid())
				return *(float*)(LocalPlayer + 0x280);
		}


		void Set_Armor(float armor)
		{
			if (PLAYER::Valid())
				*(float*)(LocalPlayer + 0x14E0) = armor;
		}

		float Get_Armor()
		{
			if (PLAYER::Valid())
				return *(float*)(LocalPlayer + 0x14E0);
		}

		void Invisible(bool invis)
		{
			if (PLAYER::Valid()) {

				if (invis) 
					*(byte*)(LocalPlayer + 0x2C) = 0x1;

				if (!invis)
					*(byte*)(LocalPlayer + 0x2C) = 0x37;
			}
		}
	}
	namespace VEHICLE
	{
		bool Valid()
		{
			if (Helper::ValidPTR(World))
			{
				DWORD64 vehicle = *(DWORD64*)(LocalPlayer + 0xD28);
				if (vehicle != NULL)
					return true;
			}
			return false;
		}

		void Godmode(bool value)
		{
			if (PLAYER::Valid()) {
				DWORD64 veh = *(DWORD64*)(LocalPlayer + 0xD28);

				if (value) {
					*(byte*)(veh + 0x189) = 1;
					*(float*)(veh + 0x280) = 1000;
					*(float*)(veh + 0x8E8) = 1000;
					*(float*)(veh + 0x824) = 1000;
					*(float*)(veh + 0x0820) = 1000;
				}

				if (!value)
					*(byte*)(veh + 0x189) = 0;
			}
		}

		void Set_Health(float engine, float gastank, float ignition)
		{
			if (VEHICLE::Valid() && PLAYER::In_Vehicle())
			{
				DWORD64 veh = *(DWORD64*)(LocalPlayer + 0xD28);
				if (veh != NULL)
				{
					*(float*)(veh + 0x908) = engine;   //Engine
					*(float*)(veh + 0x844) = gastank;  //Gas Tank
					*(float*)(veh + 0x854) = ignition; //Ignition
				}
			}
		}
		void Set_Acceleration(float val)
		{
			if (PLAYER::Valid() && VEHICLE::Valid() && PLAYER::In_Vehicle())
			{
				DWORD64 veh = *(DWORD64*)(LocalPlayer + 0xD28);
				if (Helper::ValidPTR(veh))
				{
					veh = *(DWORD64*)(veh + 0x938);
					*(float*)(veh + 0x4C) = val;
				}
			}
		}
		void Set_Gravity(float val)
		{
			if (PLAYER::Valid() && VEHICLE::Valid() && PLAYER::In_Vehicle())
			{
				DWORD64 veh = *(DWORD64*)(LocalPlayer + 0xD28);
				if (Helper::ValidPTR(veh))
					*(float*)(veh + 0xC5C) = val;
			}
		}
	}
	namespace WEAPON
	{
		void NoSpread(bool val)
		{
			if (PLAYER::Valid())
			{
				DWORD64 Addr = *(DWORD64*)(LocalPlayer + 0x10D8);
				if (Helper::ValidPTR(Addr))
				{
					Addr = *(DWORD64*)(Addr + 0x20);
					if (Helper::ValidPTR(Addr))
					{
						if (val)
						{
							*(float*)(Addr + 0x74) = 0.0f;
						}
						else
						{
							float val = *(float*)(Addr + 0x74);
							if (val == 0.0f)
								*(float*)(Addr + 0x74) = 1.0f;
						}
					}
				}
			}
		}
		void NoRecoil(bool val)
		{
			if (PLAYER::Valid())
			{
				DWORD64 Addr = *(DWORD64*)(LocalPlayer + 0x10D8);
				if (Helper::ValidPTR(Addr))
				{
					Addr = *(DWORD64*)(Addr + 0x20);
					if (Helper::ValidPTR(Addr))
					{
						if (val)
						{
							*(float*)(Addr + 0x2F4) = 0.0f;
						}
						else
						{
							float val = *(float*)(Addr + 0x2F4);
							if (val == 0.0f)
								*(float*)(Addr + 0x2F4) = 0.3f;
						}
					}
				}
			}
		}
	}
	namespace WORLD
	{
		Vector3 Waypoint_Position()
		{
			Vector3 vec;

			if (Config::isSteam)
				vec = *(Vector3*)(Config::GTA5 + 0x1F7A860);
			else
				vec = *(Vector3*)(Config::GTA5 + 0x1F76370);

			if (vec.x != 64000.0f && vec.x != 0.0f)
			{
				vec.z = -225.0f;
				return vec;
			}
			return PLAYER::Position();
		}
	}
}