#include "ESP.h"
#include <string>

namespace ESP
{

	float Distance(const Vector3& point1, const Vector3& point2)
	{
		float distance = sqrt((point1.x - point2.x) * (point1.x - point2.x) +
			(point1.y - point2.y) * (point1.y - point2.y) +
			(point1.z - point2.z) * (point1.z - point2.z));
		return distance;
	}

	void DrawSkeleton(uintptr_t ped) {
		int bone_positions[][2] = {
			{ 0, 7 },
			{ 7, 6 },
			{ 7, 5 },
			{ 7, 8 },
			{ 8, 3 },
			{ 8, 4 }
		};

		for (int i = 0; i < 6; ++i) {
			ImVec2 screen_1, screen_2;
			auto bone_1 = Pools::getBone(ped, bone_positions[i][0]),
				bone_2 = Pools::getBone(ped, bone_positions[i][1]);
			screen_1 = SDK::WorldToScreen2(bone_1);
			screen_2 = SDK::WorldToScreen2(bone_2);
			if (bone_1.x != 0 && bone_1.y != 0 && bone_1.z != 0 &&
				bone_2.x != 0 && bone_2.y != 0 && bone_2.z != 0 &&
				screen_1.x != 0 && screen_1.y != 0 &&
				screen_2.x != 0 && screen_2.y != 0) {
				if(Distance(Vector3(screen_1.x, screen_1.y, 0.0f), Vector3(screen_2.x, screen_2.y, 0.0f)) < 6.0f)
					Config::draw_list->AddLine(ImVec2(screen_1.x, screen_1.y), ImVec2(screen_2.x, screen_2.y), ImColor(Config::Misc::SkeletColor.x * 255, Config::Misc::SkeletColor.y * 255, Config::Misc::SkeletColor.z * 255, 255.0f), 1.50f);
			}
		}
	}

	void Tick()
	{
		int fov = 100;
		if (Config::Aimbot::fovnumber == 0) {
			fov = 100;
		}
		else if (Config::Aimbot::fovnumber == 1) {
			fov = 200;
		}
		else if (Config::Aimbot::fovnumber == 2) {
			fov = 300;
		}

		//Aimbot FOV
		if (Config::Aimbot::enablednumber == 1 && Config::Aimbot::drawfownumber == 1)
		{
			float ScreenCX = ImGui::GetIO().DisplaySize.x / 2;
			float ScreenCY = ImGui::GetIO().DisplaySize.y / 2;
			Config::draw_list->AddCircle(ImVec2(ScreenCX, ScreenCY), fov, ImColor(Config::Misc::FOV.x * 255, Config::Misc::FOV.y * 255, Config::Misc::FOV.z * 255, 255.0f), 24, 1.5f);
		}

		//Players
		if (Config::ESP::enabled == 1)
		{
			for (int i = 0; i < 1024; i++)
			{
					if (Pools::players[i].ped == SDK::LocalPlayer)
						continue;

					Vector3 pedPos = Vector3(Pools::players[i].position.x, Pools::players[i].position.y, Pools::players[i].position.z);

					/*if (Distance(SDK::PLAYER::Position(), pedPos) <= (Config::ESP::distance * 3))
						continue;*/

					if (pedPos.x == 0.0f || pedPos.y == 0.0f || pedPos.z == 0.0f)
						continue;

					float health = Pools::players[i].health;
					if (health > 200.0f)    health = 200.0f;
					else if (health < 0.0f) health = 0.0f;

					float armor = Pools::players[i].armor;
					if (armor > 100.0f)    armor = 100.0f;
					else if (armor < 0.0f) armor = 0.0f;

					Vector3 originPos = Vector3(pedPos.x, pedPos.y, pedPos.z - 1.0f);
					Vector3 headPos = Vector3(pedPos.x, pedPos.y, pedPos.z + 0.9f);
					ImVec2 screenPos = SDK::WorldToScreen(originPos);
					ImVec2 screenPosHead = SDK::WorldToScreen(headPos);

					if (screenPos.x > 0 && screenPosHead.x > 0)
					{
						float h = screenPosHead.y - screenPos.y;
						float w = (screenPos.y - screenPosHead.y) / 3.5f;

						if (Config::ESP::name == 1 && Distance(Vector3(Pools::players[i].position.x, Pools::players[i].position.y, Pools::players[i].position.z), SDK::PLAYER::Position()) < 10.0f) {
							const ImU32 col = ImGui::ColorConvertFloat4ToU32(Config::Misc::NameColor);
							Config::draw_list->AddText(NULL, 15.0f, ImVec2(screenPos.x - (w / 2.5), screenPosHead.y), col, std::to_string(Pools::players[i].ped).c_str(), NULL);
						}

						//Box
						if (Config::ESP::boxes == 1)
						{
							Config::draw_list->AddLine(ImVec2(screenPos.x - w, screenPosHead.y), ImVec2(screenPos.x + w, screenPosHead.y), ImColor(Config::Misc::BoxColor.x * 255, Config::Misc::BoxColor.y * 255, Config::Misc::BoxColor.z * 255, 255.0f), 1.50f);
							Config::draw_list->AddLine(ImVec2(screenPos.x - w, screenPos.y), ImVec2(screenPos.x - w, screenPosHead.y), ImColor(Config::Misc::BoxColor.x * 255, Config::Misc::BoxColor.y * 255, Config::Misc::BoxColor.z * 255, 255.0f), 1.50f);
							Config::draw_list->AddLine(ImVec2(screenPos.x + w, screenPos.y), ImVec2(screenPos.x + w, screenPosHead.y), ImColor(Config::Misc::BoxColor.x * 255, Config::Misc::BoxColor.y * 255, Config::Misc::BoxColor.z * 255, 255.0f), 1.50f);
							Config::draw_list->AddLine(ImVec2(screenPos.x - w, screenPos.y), ImVec2(screenPos.x + w, screenPos.y), ImColor(Config::Misc::BoxColor.x * 255, Config::Misc::BoxColor.y * 255, Config::Misc::BoxColor.z * 255, 255.0f), 1.50f);
						}

						if (Config::ESP::skelet == 1) {
							DrawSkeleton(Pools::players[i].ped);
						}

						//Health
						if (Config::ESP::health == 1)// && Config::ESP::healthmode == 0)
						{
							if (Config::ESP::mode == 0) {
								float oneHealth = h / 100;
								Config::draw_list->AddLine(ImVec2(screenPos.x - w - 6.0f, screenPos.y), ImVec2(screenPos.x - w - 6.0f, screenPosHead.y), ImColor(0, 0, 0, 255), 2.0f);
								Config::draw_list->AddLine(ImVec2(screenPos.x - w - 6.0f, screenPos.y), ImVec2(screenPos.x - w - 6.0f, screenPosHead.y - h + oneHealth * health), ImColor(0, 255, 0, 255), 2.0f);

								if (armor > 0)
								{
									Config::draw_list->AddLine(ImVec2(screenPos.x - w - 9.0f, screenPos.y), ImVec2(screenPos.x - w - 9.0f, screenPosHead.y), ImColor(0, 0, 0, 255), 2.0f);
									Config::draw_list->AddLine(ImVec2(screenPos.x - w - 9.0f, screenPos.y), ImVec2(screenPos.x - w - 9.0f, screenPosHead.y - h + oneHealth * armor), ImColor(0, 0, 255, 255), 2.0f);
								}
							}
							else
							{
								//Config::draw_list->AddLine(ImVec2(screenPos.x - w, screenPosHead.y), ImVec2(screenPos.x + w, screenPosHead.y), ImColor(Config::Misc::BoxColor.x * 255, Config::Misc::BoxColor.y * 255, Config::Misc::BoxColor.z * 255, 255.0f), 1.50f);
								float oneHealth = w / 100;
								Config::draw_list->AddLine(ImVec2(screenPos.x - w, screenPos.y + h + 6.0f), ImVec2(screenPos.x - w, screenPosHead.y + h + 6.0f), ImColor(0, 0, 0, 255), 2.0f);
								Config::draw_list->AddLine(ImVec2(screenPos.x - w, screenPos.y + h + 6.0f), ImVec2(screenPos.x - w - oneHealth * health, screenPosHead.y + h + 6.0f), ImColor(0, 255, 0, 255), 2.0f);

								if (armor > 0)
								{
									Config::draw_list->AddLine(ImVec2(screenPos.x - w, screenPos.y + h + 6.0f), ImVec2(screenPos.x - w, screenPosHead.y + h + 9.0f), ImColor(0, 0, 0, 255), 2.0f);
									Config::draw_list->AddLine(ImVec2(screenPos.x - w, screenPos.y + h + 6.0f), ImVec2(screenPos.x - w - oneHealth * armor, screenPosHead.y + h + 9.0f), ImColor(0, 0, 255, 255), 2.0f);
								}
							}
						}



						/*if (Config::ESP::health && Config::ESP::healthmode == 1)
						{
							float oneHealth = h / 100;
							Config::draw_list->AddLine(ImVec2(screenPos.x - w - 6.0f, screenPos.y), ImVec2(screenPos.x - w - 6.0f, screenPosHead.y), ImColor(0, 0, 0, 255), 2.0f);
							Config::draw_list->AddLine(ImVec2(screenPos.x - w - 6.0f, screenPos.y), ImVec2(screenPos.x - w - 6.0f, screenPosHead.y - h + oneHealth * health), ImColor(0, 255, 0, 255), 2.0f);

							if (armor > 0)
							{
								Config::draw_list->AddLine(ImVec2(screenPos.x - w - 9.0f, screenPos.y), ImVec2(screenPos.x - w - 9.0f, screenPosHead.y), ImColor(0, 0, 0, 255), 2.0f);
								Config::draw_list->AddLine(ImVec2(screenPos.x - w - 9.0f, screenPos.y), ImVec2(screenPos.x - w - 9.0f, screenPosHead.y - h + oneHealth * armor), ImColor(0, 0, 255, 255), 2.0f);
							}
						}*/
					}
				}
			
		}
	}

}