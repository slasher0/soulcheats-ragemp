#include <Windows.h>
#include "imgui/imgui.h"
#include "Math.h"
#include "Helper.h"

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")

namespace SDK
{
	typedef int(__fastcall* tGET_RAYCAST_RESULT)(DWORD Handle, bool* hit, D3DXVECTOR3* endCoords, D3DXVECTOR3* surfaceNormal, DWORD* entityHit);
	using give_weapon_t = void(*)(int32_t ped, uintptr_t hash, int ammo, bool equip_now);
	using pizza_to_spaghetti_t = int32_t(*)(intptr_t pointer); //pointer_to_handle actually

	//using tGET_RAYCAST_RESULT = void(*)(DWORD Handle, bool* hit, D3DXVECTOR3* endCoords, D3DXVECTOR3* surfaceNormal, DWORD* entityHit);

	extern ImVec2 WorldToScreen(Vector3 WorldPos);
	extern ImVec2 WorldToScreen2(D3DXVECTOR3 WorldPos);

	extern DWORD64 GetCameraViewAngles();
	extern DWORD64 GetReplayInterface();

	extern DWORD64 World;
	extern DWORD64 LocalPlayer;

	extern give_weapon_t give_weapon;
	extern pizza_to_spaghetti_t pointer_to_handle;

	extern tGET_RAYCAST_RESULT raycast_result_func;

	extern void Init();

	namespace PLAYER
	{
		extern bool Valid();
		extern bool In_Vehicle();

		extern Vector3 Position();
		extern void Set_Position(Vector3 vec);

		extern void set_frame_flag(uint32_t bit_flag);
		extern void Set_Health(float health);
		extern float Get_Health();
		extern void Set_Armor(float armor);
		extern float Get_Armor();
		extern void Godmode(bool value);

		extern void Invisible(bool invis);
	}
	namespace VEHICLE
	{
		extern bool Valid();
		extern void Set_Health(float engine, float gastank, float ignition);
		extern void Set_Acceleration(float val);
		extern void Set_Gravity(float val);
		extern void Godmode(bool value);
	}
	namespace WEAPON
	{
		extern void NoSpread(bool val);
		extern void NoRecoil(bool val);
	}
	namespace WORLD
	{
		extern Vector3 Waypoint_Position();
	}
}