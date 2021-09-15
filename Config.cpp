#include "Config.h"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <direct.h>
#include <string>
#include <basetsd.h>

namespace Config
{
	DWORD64 GTA5;
	bool isSteam = false;
	ImDrawList* draw_list;

	namespace Player
	{
		bool staticgodmode = false;
		bool godmode = false;
		bool invisible = false;
		bool crosshair = false;
		bool noclip = false;
		bool coords = false;
		bool seehealth = false;
		int healkey = 0;
		int repairkey = 0;
		int armorkey = 0;
		int tptoway = 0;

		//NEW GUI
		int crosshairnumber = 0;
		int informationnumber = 0;
		int godmodenumber = 0;
		int ragdollnumber = 0;
		int invisiblenumber = 0;
		int nogravnumber = 0;
		int vehicleespnumber = 0;
		int objectespnumber = 0;
	}
	namespace Vehicle
	{
		int unlock_doors = false;
		int autorepair = false;
		int speedmultiplier = 0;
		int godmode = 0;
	}
	namespace Weapon
	{
		int norecoilnumber = 0;
		int nospreadnumber = 0;
		int noreloadnumber = 0;
		int rapidefirenumber = 0;
	}
	namespace Menu
	{
		bool enabled = false;
		bool unload = false;
		bool watermark = true;
	}
	namespace ESP
	{
		int enabled = false;
		int boxes = false;
		int health = false;
		int name = false;
		int skelet = false;
		int mode = 0;
		int distance = 0;
	}
	namespace Aimbot
	{
		int aimkey = 0;
		int enablednumber = 0;
		int distancenumber = 0;
		int bonenumber = 1;
		int hardlocknumber = 0;
		int silentnumber = 0;
		int drawfownumber = 0;
		int fovnumber = 0;
		int smoothnumber = 0;
		int targetnumber = 0;
	}
	namespace Misc
	{
		std::string weapon1 = "WEAPON_PISTOL";
		std::string weapon2 = "WEAPON_PISTOL";
		std::string weapon3 = "WEAPON_PISTOL";
		std::string weapon4 = "WEAPON_PISTOL";
		std::string weapon5 = "WEAPON_PISTOL";
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		ImVec4 BoxColor = ImVec4(255 / 255, 200 / 255, 0 / 255, 180 / 255);
		ImVec4 SkeletColor = ImVec4(255 / 255, 255 / 255, 255 / 255, 255 / 255);
		ImVec4 NameColor = ImVec4(255 / 255, 255 / 255, 255 / 255, 255 / 255);
		ImVec4 FOV = ImVec4(255 / 255, 255 / 255, 0 / 255, 70 / 255);
	}
	namespace Others
	{
		const char* version = "v0.1";
		bool fivem = 0;
	}

	bool replace(std::string& str, const std::string& from, const std::string& to)
	{
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}
	bool LoadParam(std::string line, char* name, int* var)
	{
		if (line.find(name) != std::string::npos)
		{
			replace(line, name, "");
			*var = atoi(line.c_str());
			return true;
		}
		return false;
	}
	bool LoadParam(std::string line, char* name, float* var)
	{
		if (line.find(name) != std::string::npos)
		{
			replace(line, name, "");
			*var = std::stof(line.c_str());
			return true;
		}
		return false;
	}
	bool LoadParam(std::string line, char* name, bool* var)
	{
		if (line.find(name) != std::string::npos)
		{
			replace(line, name, "");
			*var = std::atoi(line.c_str());
			return true;
		}
		return false;
	}
	bool LoadParam(std::string line, char* name, std::string* var)
	{
		if (line.find(name) != std::string::npos)
		{
			replace(line, name, "");
			*var = line;
			return true;
		}
		return false;
	}


	void Save(int i)
	{
		mkdir("C:\\SOUL");
		Beep(523, 200);
		std::ofstream out;
		std::string cfg = "C:\\SOUL\\Config" + std::to_string(i) + ".ini";
		out.open(cfg);
		if (out.is_open())
		{
			out << "::PLAYER::" << std::endl;
			out << "player_vehicleesp: " << Player::vehicleespnumber << std::endl;
			out << "player_objectesp: " << Player::objectespnumber << std::endl;
			out << "player_information: " << Player::informationnumber << std::endl;
			out << "player_godmode: " << Player::godmodenumber << std::endl;
			out << "player_invisible: " << Player::invisiblenumber << std::endl;
			out << "player_crosshair: " << Player::crosshairnumber << std::endl;
			out << "player_nograv: " << Player::nogravnumber << std::endl;
			out << "player_ragdoll: " << Player::ragdollnumber << std::endl;
			

			// NEW GUI ^^^^
			out << "player_noclip: " << Player::noclip << std::endl;
			out << "player_coords: " << Player::coords << std::endl;
			out << "player_seehealth: " << Player::seehealth << std::endl;
			out << "player_healkey: " << Player::healkey << std::endl;
			out << "player_armorkey: " << Player::armorkey << std::endl;
			out << "player_repairkey: " << Player::repairkey << std::endl;
			out << "" << std::endl;

			out << "::MENU::" << std::endl;
			out << "menu_watermark: " << Menu::watermark << std::endl;
			out << "" << std::endl;

			out << "::VEHICLE::" << std::endl;
			out << "vehicle_unlock_doors: " << Vehicle::unlock_doors << std::endl;
			out << "vehicle_autorepair: " << Vehicle::autorepair << std::endl;
			out << "vehicle_speedmultiplier: " << Vehicle::speedmultiplier << std::endl;
			out << "vehicle_godmode: " << Vehicle::godmode << std::endl;
			out << "" << std::endl;

			out << "::WEAPON::" << std::endl;
			out << "weapon_norecoil: " << Weapon::norecoilnumber << std::endl;
			out << "weapon_nospread: " << Weapon::nospreadnumber << std::endl;
			out << "" << std::endl;

			out << "::ESP::" << std::endl;
			out << "esp_enabled: " << ESP::enabled << std::endl;
			out << "esp_boxes: " << ESP::boxes << std::endl;
			out << "esp_health: " << ESP::health << std::endl;
			out << "esp_name: " << ESP::name << std::endl;
			out << "esp_skelet: " << ESP::skelet << std::endl;
			out << "esp_mode: " << ESP::mode << std::endl;
			out << "esp_distance: " << ESP::distance << std::endl;
			out << "" << std::endl;

			out << "::AIMBOT::" << std::endl;
			out << "aimbot_enabled: " << Aimbot::enablednumber << std::endl;
			out << "aimbot_fov: " << Aimbot::fovnumber << std::endl;
			out << "aimbot_draw_fov: " << Aimbot::drawfownumber << std::endl;
			out << "aimbot_smooth: " << Aimbot::smoothnumber << std::endl;
			out << "aimbot_mode: " << Aimbot::bonenumber << std::endl;
			out << "" << std::endl;

			out << "::MISC::" << std::endl;
			out << "giveweapon1: " << Misc::weapon1 << std::endl;
			out << "giveweapon2: " << Misc::weapon2 << std::endl;
			out << "giveweapon3: " << Misc::weapon3 << std::endl;
			out << "giveweapon4: " << Misc::weapon4 << std::endl;
			out << "giveweapon5: " << Misc::weapon5 << std::endl;
			out << "misc_x: " << Misc::x << std::endl;
			out << "misc_y: " << Misc::y << std::endl;
			out << "misc_z: " << Misc::z << std::endl;
			out << "misc_boxcolorsx: " << Misc::BoxColor.x << std::endl;
			out << "misc_boxcolorsy: " << Misc::BoxColor.y << std::endl;
			out << "misc_boxcolorsz: " << Misc::BoxColor.z << std::endl;
			out << "misc_boxcolorsw: " << Misc::BoxColor.w << std::endl;
			out << "misc_skeletcolorx: " << Misc::SkeletColor.x << std::endl;
			out << "misc_skeletcolory: " << Misc::SkeletColor.y << std::endl;
			out << "misc_skeletcolorz: " << Misc::SkeletColor.z << std::endl;
			out << "misc_skeletcolorw: " << Misc::SkeletColor.w << std::endl;
			out << "misc_namecolorx: " << Misc::NameColor.x << std::endl;
			out << "misc_namecolory: " << Misc::NameColor.y << std::endl;
			out << "misc_namecolorz: " << Misc::NameColor.z << std::endl;
			out << "misc_namecolorw: " << Misc::NameColor.w << std::endl;
			out << "misc_fovcolorx: " << Misc::FOV.x << std::endl;
			out << "misc_fovcolory: " << Misc::FOV.y << std::endl;
			out << "misc_fovcolorz: " << Misc::FOV.z << std::endl;
			out << "misc_fovcolorw: " << Misc::FOV.w << std::endl;
			out << "" << std::endl;

			out.close();
		}
	}
	void Load(int i)
	{
		Beep(523, 200);
		std::string line;
		std::string cfg = "C:\\SOUL\\Config" + std::to_string(i) + ".ini";
		std::ifstream in(cfg);
		if (in.is_open())
		{
			while (getline(in, line))
			{

				LoadParam(line, "player_staticgodmode: ", &Player::staticgodmode);
				LoadParam(line, "player_godmode: ", &Player::godmodenumber);
				LoadParam(line, "player_invisible: ", &Player::invisiblenumber);
				LoadParam(line, "player_crosshair: ", &Player::crosshairnumber);
				LoadParam(line, "player_vehicleesp: ", &Player::vehicleespnumber);
				LoadParam(line, "player_objectesp: ", &Player::objectespnumber);
				LoadParam(line, "player_information: ", &Player::informationnumber);
				LoadParam(line, "player_nograv: ", &Player::nogravnumber);
				LoadParam(line, "player_ragdoll: ", &Player::ragdollnumber);

				// NEW GUI ^^^^
				
				LoadParam(line, "player_noclip: ", &Player::noclip);
				LoadParam(line, "player_coords: ", &Player::coords);
				LoadParam(line, "player_seehealth: ", &Player::seehealth);
				LoadParam(line, "player_healkey: ", &Player::healkey);
				LoadParam(line, "player_armorkey: ", &Player::armorkey);
				LoadParam(line, "player_repairkey: ", &Player::repairkey);

				LoadParam(line, "menu_watermark: ", &Menu::watermark);

				LoadParam(line, "vehicle_unlock_doors: ", &Vehicle::unlock_doors);
				LoadParam(line, "vehicle_autorepair: ", &Vehicle::autorepair);
				LoadParam(line, "vehicle_speedmultiplier: ", &Vehicle::speedmultiplier);
				LoadParam(line, "vehicle_godmode: ", &Vehicle::godmode);

				LoadParam(line, "weapon_norecoil: ", &Weapon::norecoilnumber);
				LoadParam(line, "weapon_nospread: ", &Weapon::nospreadnumber);

				LoadParam(line, "esp_enabled: ", &ESP::enabled);
				LoadParam(line, "esp_boxes: ", &ESP::boxes);
				LoadParam(line, "esp_health: ", &ESP::health);
				LoadParam(line, "esp_name: ", &ESP::name);
				LoadParam(line, "esp_mode: ", &ESP::mode);
				LoadParam(line, "esp_skelet: ", &ESP::skelet);
				LoadParam(line, "esp_distance: ", &ESP::distance);

				LoadParam(line, "aimbot_enabled: ", &Aimbot::enablednumber);
				LoadParam(line, "aimbot_fov: ", &Aimbot::fovnumber);
				LoadParam(line, "aimbot_draw_fov: ", &Aimbot::drawfownumber);
				LoadParam(line, "aimbot_smooth: ", &Aimbot::smoothnumber);
				LoadParam(line, "aimbot_mode: ", &Aimbot::bonenumber);

				LoadParam(line, "giveweapon1: ", &Misc::weapon1);
				LoadParam(line, "giveweapon2: ", &Misc::weapon2);
				LoadParam(line, "giveweapon3: ", &Misc::weapon3);
				LoadParam(line, "giveweapon4: ", &Misc::weapon4);
				LoadParam(line, "giveweapon5: ", &Misc::weapon5);
				LoadParam(line, "misc_x: ", &Misc::x);
				LoadParam(line, "misc_y: ", &Misc::y);
				LoadParam(line, "misc_z: ", &Misc::z);
				LoadParam(line, "misc_boxcolorsx: ", &Misc::BoxColor.x);
				LoadParam(line, "misc_boxcolorsy: ", &Misc::BoxColor.y);
				LoadParam(line, "misc_boxcolorsz: ", &Misc::BoxColor.z);
				LoadParam(line, "misc_boxcolorsw: ", &Misc::BoxColor.w);
				LoadParam(line, "misc_skeletcolorx: ", &Misc::SkeletColor.x);
				LoadParam(line, "misc_skeletcolory: ", &Misc::SkeletColor.y);
				LoadParam(line, "misc_skeletcolorz: ", &Misc::SkeletColor.z);
				LoadParam(line, "misc_skeletcolorw: ", &Misc::SkeletColor.w);
				LoadParam(line, "misc_namecolorx: ", &Misc::NameColor.x);
				LoadParam(line, "misc_namecolory: ", &Misc::NameColor.y);
				LoadParam(line, "misc_namecolorz: ", &Misc::NameColor.z);
				LoadParam(line, "misc_namecolorw: ", &Misc::NameColor.w);
				LoadParam(line, "misc_fovcolorx: ", &Misc::FOV.x);
				LoadParam(line, "misc_fovcolory: ", &Misc::FOV.y);
				LoadParam(line, "misc_fovcolorz: ", &Misc::FOV.z);
				LoadParam(line, "misc_fovcolorw: ", &Misc::FOV.w);
			}
		}
	}

}