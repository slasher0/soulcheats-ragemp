#include "Aimbot.h"

namespace Aimbot
{

	float RandomFloat(float min, float max)
	{
		assert(max > min);
		float random = ((float)rand()) / (float)RAND_MAX;
		float range = max - min;
		return (random * range) + min;
	}

	int locked = 0;
	int locktimer = 0;

	DWORD64 addr;
	void AimVec(Vector3 point)
	{
		__try
		{
			DWORD64 addr = SDK::GetCameraViewAngles();
			if (Helper::ValidPTR(addr))
			{
				

				Vector3 CrosshairPos = *(Vector3*)(addr + 0x60);
				float distance = Vector3().Distance(CrosshairPos, point);

				if (locked != 0)
				{
					const ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(0.95f, 0.34f, 0.20f, 0.5f));
					Config::draw_list->AddText(NULL, 16.0f, ImVec2(CrosshairPos.x - 0.05, CrosshairPos.y), col, "L", NULL);
				}

				static DWORD smoothTime;
				if (timeGetTime() - smoothTime > 3)
				{
					smoothTime = timeGetTime();

					int smooth = 0;

					if (Config::Aimbot::smoothnumber == 0)
					{
						smooth = 0;
					}
					else if (Config::Aimbot::smoothnumber == 1)
					{
						smooth = 10;
					}
					else if (Config::Aimbot::smoothnumber == 2)
					{
						smooth = 20;
					}

					float threshold = 0.5f / (smooth * 20);
					Vector3 Out = Vector3((point.x - CrosshairPos.x) / distance, (point.y - CrosshairPos.y) / distance, (point.z - CrosshairPos.z) / distance);


					if (smooth <= 1)
					{
						*(Vector3*)(addr + 0x40) = Out;  //FPS
						*(Vector3*)(addr + 0x3D0) = Out; //TPS
					}
					else
					{
						//FPS
						{
							Vector3 angles = *(Vector3*)(addr + 0x40);
							if (((Out.x - angles.x) > threshold) || ((angles.x - Out.x) > threshold))
							{
								if (angles.x > Out.x)
									*(float*)(addr + 0x40 + 0x0) -= threshold;
								else if (angles.x < Out.x)
									*(float*)(addr + 0x40 + 0x0) += threshold;
							}
							if (((Out.y - angles.y) > threshold) || ((angles.y - Out.y) > threshold))
							{
								if (angles.y > Out.y)
									*(float*)(addr + 0x40 + 0x4) -= threshold;
								else if (angles.y < Out.y)
									*(float*)(addr + 0x40 + 0x4) += threshold;
							}
							if (((Out.z - angles.z) > threshold) || ((angles.z - Out.z) > threshold))
							{
								if (angles.z > Out.z)
									*(float*)(addr + 0x40 + 0x8) -= threshold;
								else if (angles.z < Out.z)
									*(float*)(addr + 0x40 + 0x8) += threshold;
							}
						}

						//TPS
						{
							Vector3 angles = *(Vector3*)(addr + 0x3D0);
							if (((Out.x - angles.x) > threshold) || ((angles.x - Out.x) > threshold))
							{
								if (angles.x > Out.x)
									*(float*)(addr + 0x3D0 + 0x0) -= threshold;
								else if (angles.x < Out.x)
									*(float*)(addr + 0x3D0 + 0x0) += threshold;
							}
							if (((Out.y - angles.y) > threshold) || ((angles.y - Out.y) > threshold))
							{
								if (angles.y > Out.y)
									*(float*)(addr + 0x3D0 + 0x4) -= threshold;
								else if (angles.y < Out.y)
									*(float*)(addr + 0x3D0 + 0x4) += threshold;
							}
							if (((Out.z - angles.z) > threshold) || ((angles.z - Out.z) > threshold))
							{
								if (angles.z > Out.z)
									*(float*)(addr + 0x3D0 + 0x8) -= threshold;
								else if (angles.z < Out.z)
									*(float*)(addr + 0x3D0 + 0x8) += threshold;
							}
						}
					}
				}
			}
		}
		__except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {}
	}

	void Tick()
	{
		if (Config::Aimbot::enablednumber != 1)
			return;

		//Fix Aim Vectors
		__try
		{
			DWORD64 m_CCameraViewAngles = SDK::GetCameraViewAngles();
			DWORD64 camParams = *(DWORD64*)(m_CCameraViewAngles + 0x10);
			//if (SDK::PLAYER::In_Vehicle())
			{
				if (*(float*)(camParams + 0x2AC) == -2)
				{
					*(float*)(camParams + 0x2AC) = 0.0f;
					*(float*)(camParams + 0x2C0) = 111.0f;
					*(float*)(camParams + 0x2C4) = 111.0f;
				}
			}
			//else
			{
				if (*(float*)(camParams + 0x130) == 8.0f)
				{
					*(float*)(camParams + 0x130) = 111.0f; //def 8.0f
					*(float*)(camParams + 0x134) = 111.0f; //def 10.0f
					*(float*)(camParams + 0x4CC) = 0.0f;   //def 4.0f

					if (*(float*)(camParams + 0x49C) == 1.0f)
						*(float*)(camParams + 0x49C) = 0.0f;   //def 1.0f

					*(float*)(camParams + 0x2AC) = 0.0f;   //def -3.0f
					*(float*)(camParams + 0x2B0) = 0.0f;   //def -8.0f
				}
			}
		}
		__except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {}

		float ScreenCX = ImGui::GetIO().DisplaySize.x / 2;
		float ScreenCY = ImGui::GetIO().DisplaySize.y / 2;

		int closest_id = -1;
		float closest_distance = 5000.0f;
		float closest_fov = 5000.0f;
		ImVec2 closest_screenPos;

		for (int i = 0; i < 1024; i++)
		{
			if (Pools::players[i].ped == NULL)
				continue;

			if (Pools::players[i].ped == SDK::LocalPlayer)
				continue;

			if (Pools::players[i].health < 1.0f)
				continue;

			Vector3 myPos;
			DWORD64 addr = SDK::GetCameraViewAngles();
			if (addr != NULL)
				myPos = *(Vector3*)(addr + 0x60);

			Vector3 pedPos = Vector3(Pools::players[i].position.x, Pools::players[i].position.y, Pools::players[i].position.z);
			if (pedPos.x == 0.0f || pedPos.y == 0.0f || pedPos.z == 0.0f)
				continue;

			float distance = Vector3().Distance(pedPos, myPos);

			if (distance < 2.0f)
				continue;

			Vector3 originPos = Vector3(Pools::players[i].position.x, Pools::players[i].position.y, Pools::players[i].position.z);
			ImVec2 screenPos = SDK::WorldToScreen(originPos);
			int fov = 100;
			if (Config::Aimbot::fovnumber == 0) {
				fov = 100;
			} else if (Config::Aimbot::fovnumber == 1) {
				fov = 200;
			} else if (Config::Aimbot::fovnumber == 2) {
				fov = 300;
			}
			if (screenPos.x > 0)
			{
				float AngleDifferences = sqrt(pow((screenPos.x - ScreenCX), 2) + pow((screenPos.y - ScreenCY), 2));
				if (AngleDifferences > 0.01f && AngleDifferences < fov && distance <= closest_distance)
				{
					closest_id = i;
					closest_distance = distance;
					closest_fov = AngleDifferences;
					closest_screenPos = screenPos;
				}
			}
		}

		//Key Pressing
		if (GetAsyncKeyState(0x2))
		{
			if (closest_fov >= 1000.0f) return;
			if (closest_id == -1) return;

			if (Pools::players[closest_id].position.x == 0.0f)
				return;

			int bone_positions[][2] = {
				{ 0, 7 },
				{ 7, 6 },
				{ 7, 5 },
				{ 7, 8 },
				{ 8, 3 },
				{ 8, 4 }
			};

			Vector3 originPos = Vector3(Pools::getBone(Pools::players[closest_id].ped, bone_positions[3][0]).x, Pools::getBone(Pools::players[closest_id].ped, bone_positions[3][0]).y, Pools::getBone(Pools::players[closest_id].ped, bone_positions[3][0]).z);
			Vector3 headPos = Vector3(Pools::getBone(Pools::players[closest_id].ped, bone_positions[0][0]).x, Pools::getBone(Pools::players[closest_id].ped, bone_positions[0][0]).y, Pools::getBone(Pools::players[closest_id].ped, bone_positions[0][0]).z);
			Vector3 neckPos = Vector3(Pools::getBone(Pools::players[closest_id].ped, bone_positions[1][0]).x, Pools::getBone(Pools::players[closest_id].ped, bone_positions[1][0]).y, Pools::getBone(Pools::players[closest_id].ped, bone_positions[1][0]).z);

			if (locked != 0) {
				if (Pools::players[locked].health < 1) {
					locked = closest_id;
				}
			}
			else
			{
				locked = closest_id;
			}

			if (locked != 0) {
				originPos = Vector3(Pools::getBone(Pools::players[locked].ped, bone_positions[3][0]).x, Pools::getBone(Pools::players[locked].ped, bone_positions[3][0]).y, Pools::getBone(Pools::players[locked].ped, bone_positions[3][0]).z);
				headPos = Vector3(Pools::getBone(Pools::players[locked].ped, bone_positions[0][0]).x, Pools::getBone(Pools::players[locked].ped, bone_positions[0][0]).y, Pools::getBone(Pools::players[locked].ped, bone_positions[0][0]).z);
				neckPos = Vector3(Pools::getBone(Pools::players[locked].ped, bone_positions[1][0]).x, Pools::getBone(Pools::players[locked].ped, bone_positions[1][0]).y, Pools::getBone(Pools::players[locked].ped, bone_positions[1][0]).z);
			}

			if (Config::Aimbot::bonenumber == 0) {
				if (closest_screenPos.x > 0)
					AimVec(headPos);
			}
			else if(Config::Aimbot::bonenumber == 1)
			{
				if (closest_screenPos.x > 0)
					AimVec(neckPos);
			}
			else if (Config::Aimbot::bonenumber == 2)
			{
				if (closest_screenPos.x > 0)
					AimVec(originPos);
			}
		}
		else
		{	
			if (locktimer >= 3) {
				locked = 0;
				locktimer = 0;
			}
			else
			{
				locktimer++;
			}
		}
	}
}