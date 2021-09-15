#include "GUI.h"
#include "SDK.h"

namespace Mains
{

	void Player()
	{
		//Simple Godmode
		if (Config::Player::godmodenumber == 1)
		{
			SDK::PLAYER::Godmode(true);
		}
		else
		{
			SDK::PLAYER::Godmode(false);
		}

		if (Config::Player::godmodenumber == 2)
			SDK::PLAYER::Set_Health(200.0f);

		if (Config::Player::invisiblenumber == 1)
		{
			SDK::PLAYER::Invisible(true);
		}
		else
		{
			SDK::PLAYER::Invisible(false);
		}

		if (Config::Player::ragdollnumber == 1)
		{
			//SDK::PLAYER::NoRagdoll(true);
		}
		else
		{
			//SDK::PLAYER::NoRagdoll(false);
		}

		if (Config::Player::healkey != 0) {
			if (GetAsyncKeyState(Config::Player::healkey)) {
				SDK::PLAYER::Set_Health(200.0f);
			}
		}

		if (Config::Player::tptoway != 0) {
			if (GetAsyncKeyState(Config::Player::tptoway)) {
				SDK::PLAYER::Set_Position(SDK::WORLD::Waypoint_Position());
			}
		}

		if (Config::Player::armorkey != 0) {
			if (GetAsyncKeyState(Config::Player::armorkey)) {
				SDK::PLAYER::Set_Armor(200.0f);
			}
		}

		if (Config::Player::repairkey != 0) {
			if (GetAsyncKeyState(Config::Player::repairkey)) {
				SDK::VEHICLE::Set_Health(1000.0f, 1000.0f, 65.0f);
			}
		}
		

		if (Config::Player::invisible) {
			SDK::PLAYER::Invisible(true);
		}
		else
		{
			SDK::PLAYER::Invisible(false);
		}
	
	}

	void Weapon()
	{
		//NoRecoil
		if (Config::Weapon::norecoilnumber == 0)
			SDK::WEAPON::NoRecoil(false);

		if (Config::Weapon::norecoilnumber == 1)
			SDK::WEAPON::NoRecoil(true);

		//NoSpread
		if(Config::Weapon::nospreadnumber == 0)
			SDK::WEAPON::NoSpread(false);

		if (Config::Weapon::nospreadnumber == 1)
			SDK::WEAPON::NoSpread(true);
	}

	void Vehicle()
	{
		//SDK::VEHICLE::Godmode(Config::Vehicle::godmode);

		//Speedhack
		if (Config::Vehicle::speedmultiplier > 0)
		{
			SDK::VEHICLE::Set_Acceleration(1.0f * Config::Vehicle::speedmultiplier);
			SDK::VEHICLE::Set_Gravity((9.7f * Config::Vehicle::speedmultiplier) / 2);
		}
		else
		{
			SDK::VEHICLE::Set_Acceleration(1.0f);
			SDK::VEHICLE::Set_Gravity(9.7f);
		}
		if (Config::Vehicle::autorepair == 1)
		{
			SDK::VEHICLE::Set_Health(1000.0f, 1000.0f, 65.0f);

		}
	}

	void Menu()
	{
	/*	int ScreenWidth, ScreenHeight;
		engine->GetScreenSize(ScreenWidth, ScreenHeight);

		int x = (int)(ScreenWidth * 0.5f);
		int y = (int)(ScreenHeight * 0.5f);
		int dx = ScreenWidth / OverrideView::currentFOV;
		int dy = ScreenHeight / OverrideView::currentFOV;

		int crosshairX = (int)(x - (dx * punchAngle.y));
		int crosshairY = (int)(y + (dy * punchAngle.x));

		// outline horizontal
		Draw::FilledRectangle(Vector2D(crosshairX - 4, crosshairY - 1), Vector2D(crosshairX + 5, crosshairY + 2), Color(0, 0, 0, 170));
		// outline vertical
		Draw::FilledRectangle(Vector2D(crosshairX - 1, crosshairY - 4), Vector2D(crosshairX + 2, crosshairY + 5), Color(0, 0, 0, 170));
		// line horizontal
		Draw::Line(Vector2D(crosshairX - 3, crosshairY), Vector2D(crosshairX + 4, crosshairY), Color(255, 255, 255, 255));
		// line vertical
		Draw::Line(Vector2D(crosshairX, crosshairY + 3), Vector2D(crosshairX, crosshairY - 4), Color(255, 255, 255, 255));*/
	}


	void Tick()
	{
		Player();
		Weapon();
		Vehicle();
		Menu();
	}
}