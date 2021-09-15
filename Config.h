#include "imgui/imgui.h"
#include <Windows.h>
#include <string>

namespace Config
{
	extern DWORD64 GTA5;
	extern bool isSteam;
	extern ImDrawList* draw_list;

	namespace Menu
	{
		extern bool enabled;
		extern bool unload;
		extern bool watermark;
	}
	namespace Misc
	{
		extern std::string weapon1;
		extern std::string weapon2;
		extern std::string weapon3;
		extern std::string weapon4;
		extern std::string weapon5;
		extern float x;
		extern float y;
		extern float z;
		extern ImVec4 NameColor;
		extern ImVec4 FOV;
		extern ImVec4 BoxColor;
		extern ImVec4 SkeletColor;
	}
	namespace Player
	{
		extern bool staticgodmode;
		extern bool godmode;
		extern bool invisible;
		extern bool crosshair;
		extern bool noclip;
		extern bool seehealth;
		extern bool coords;
		extern int healkey;
		extern int armorkey;
		extern int repairkey;
		extern int vehicleespnumber;
		extern int objectespnumber;
		extern int informationnumber;
		extern int godmodenumber;
		extern int invisiblenumber;
		extern int crosshairnumber;
		extern int nogravnumber;
		extern int ragdollnumber;
		extern int tptoway;
	}
	namespace Vehicle
	{
		extern int unlock_doors;
		extern int autorepair;
		extern int speedmultiplier;
		extern int godmode;
	}
	namespace Weapon
	{
		extern int norecoilnumber;
		extern int nospreadnumber;
		extern int noreloadnumber;
		extern int rapidefirenumber;
	}
	namespace ESP
	{
		extern int enabled;
		extern int boxes;
		extern int health;
		extern int mode;
		extern int name;
		extern int skelet;
		extern int distance;
	}
	namespace Aimbot
	{
		extern int aimkey;
		extern int enablednumber;
		extern int distancenumber;
		extern int bonenumber;
		extern int hardlocknumber;
		extern int silentnumber;
		extern int drawfownumber;
		extern int fovnumber;
		extern int smoothnumber;
		extern int targetnumber;
	}
	namespace Others
	{
		extern const char* version;
		extern bool fivem;
	}

	extern void Save(int i);
	extern void Load(int i);
}