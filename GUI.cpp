#include "GUI.h"
#include "SDK.h"
#include <string>
#include "Pools.h"
#include "utilities.h"
#include "imgui/imgui_internal.h"
#include "Logger.h"

namespace GUI
{
    static int item_current = 0;
    static float armorfloat = 50.0f;
    static float healthfloat = 50.0f;
    static std::string pname = "";

    int player = 0;

    const char* section = "Default";

    std::string onoff[2] = { "Off", "On" };

    std::string godmode[3] = { "Off", "Static", "Autoheal" };
    std::string aimbotdistance[3] = { "100", "75", "50" };
    std::string fov[3] = { "100", "200", "300" };
    std::string smooth[3] = { "0", "10", "20" };
    std::string speedhack[3] = { "Off", "1", "3" };
    std::string targets[3] = { "Both", "Player", "NPC" };
    std::string aimbotbone[3] = { "Head", "Neck", "Body" };
    std::string espdistance[3] = { "250", "500", "1000" };
    

	static void ShowOverlay()
	{
		const float DISTANCE = 10.0f;
		static int corner = 0;
		ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		if (corner != -1)
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowBgAlpha(0.0f);
		if (Config::Menu::watermark) {
			if (ImGui::Begin("overlay", NULL, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse))
			{
                if (Config::Player::informationnumber == 1)
                {
                    std::string fps = "FPS: " + std::to_string(utilities::getFPS());
                    ImGui::Text(fps.c_str());
                    ImGui::Text("soulcheats.cc");
                /*}
                if (Config::Player::coords)
                {*/
                    std::string cordstring = "X: " + std::to_string((int)round(SDK::PLAYER::Position().x)) + " - Y: " + std::to_string((int)round(SDK::PLAYER::Position().y)) + " - Z: " + std::to_string((int)round(SDK::PLAYER::Position().z));
                    ImGui::Text(cordstring.c_str());
                }
                /*if (Config::Player::seehealth)
                {
                    std::string health = "Health: " + (int)round(SDK::PLAYER::Get_Health());
                    ImGui::Text(health.c_str());
                    std::string armor = "Armor: " + (int)round(SDK::PLAYER::Get_Armor());
                    ImGui::Text(armor.c_str());
                }*/

			}
			ImGui::End();
		}
	}

    void DrawOnOff(int* number2, std::string labels[], int yaddition, std::string label) {
            ImGuiWindow* window2 = ImGui::GetCurrentWindow();

            ImVec2 vMin = ImGui::GetWindowContentRegionMin();
            ImVec2 vMax = ImGui::GetWindowContentRegionMax();

            vMin.x += ImGui::GetWindowPos().x;
            vMin.y += ImGui::GetWindowPos().y;
            vMax.x += ImGui::GetWindowPos().x;
            vMax.y += ImGui::GetWindowPos().y;

            window2->DrawList->AddText(NULL, 15.0f, ImVec2(vMin.x + 90, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), label.c_str());
            window2->DrawList->AddText(NULL, 15.0f, ImVec2(vMin.x + 260, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), "<-");
            window2->DrawList->AddText(NULL, 15.0f, ImVec2(vMin.x + 340, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), "->");
            if (labels[*number2] == "Off")
            {
                window2->DrawList->AddText(NULL, 15.0f, ImVec2(vMin.x + 295, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)), labels[*number2].c_str());
            }
            else if(labels[*number2] == "Autoheal")
            {
                window2->DrawList->AddText(NULL, 15.0f, ImVec2(vMin.x + 280, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)), labels[*number2].c_str());
            }
            else
            {
                window2->DrawList->AddText(NULL, 15.0f, ImVec2(vMin.x + 295, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)), labels[*number2].c_str());
            }
            window2->DrawList->AddLine(ImVec2(vMin.x + 90, vMin.y + 58 + yaddition), ImVec2(vMin.x + 380, vMin.y + 58 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 1.0f);
            if (GetAsyncKeyState(0x1))
            {
                POINT p3;

                GetCursorPos(&p3);

                bool hovering1 = 0;
                bool hovering2 = 0;
             
                for (int i = 0; i < 20; i++)
                {
                    if ((vMin.y + 40 + yaddition + i) == p3.y) {
                        for (int i2 = 0; i2 < 20; i2++)
                        {
                            if ((vMin.x + 260 + i2) == p3.x) {
                                hovering1 = 1;
                            }
                        }
                    }
                }

                for (int i = 0; i < 20; i++)
                {
                    if ((vMin.y + 40 + yaddition - i) == p3.y) {
                        for (int i2 = 0; i2 < 20; i2++)
                        {
                            if ((vMin.x + 260 - i2) == p3.x) {
                                hovering1 = 1;
                            }
                        }
                    }
                }

                for (int i = 0; i < 20; i++)
                {
                    if ((vMin.y + 40 + yaddition + i) == p3.y) {
                        for (int i2 = 0; i2 < 20; i2++)
                        {
                            if ((vMin.x + 340 + i2) == p3.x) {
                                hovering2 = 1;
                            }
                        }
                    }
                }

                for (int i = 0; i < 20; i++)
                {
                    if ((vMin.y + 40 + yaddition - i) == p3.y) {
                        for (int i2 = 0; i2 < 20; i2++)
                        {
                            if ((vMin.x + 340 - i2) == p3.x) {
                                hovering2 = 1;
                            }
                        }
                    }
                }


                if (hovering1)
                {
                    Logger::Log("Hovering1");
                   // window2->DrawList->AddText(ImVec2(vMin.x + 260, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), "ffffff");
                    if (*number2 > 0) {
                        *number2 = *number2 - 1;
                        Sleep(150);
                    }
                }
                else if (hovering2)
                {
                    Logger::Log("Hovering2");
                    //window2->DrawList->AddText(ImVec2(vMin.x + 340, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), "rjgfrgiujr");
                    if (*number2 < 1)
                    {
                       *number2 = *number2 + 1;
                       Sleep(150);
                    }

                }
                hovering1 = 0;
                hovering2 = 0;

                //window2->DrawList->AddText(ImVec2(p3.x, p3.y), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), ".");

            }
    }

    void Draw3Switch(int* number2, std::string labels[], int yaddition, std::string label) {
        ImGuiWindow* window2 = ImGui::GetCurrentWindow();

        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();

        vMin.x += ImGui::GetWindowPos().x;
        vMin.y += ImGui::GetWindowPos().y;
        vMax.x += ImGui::GetWindowPos().x;
        vMax.y += ImGui::GetWindowPos().y;

        window2->DrawList->AddText(NULL, 15.0f, ImVec2(vMin.x + 90, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), label.c_str());
        window2->DrawList->AddText(NULL, 15.0f, ImVec2(vMin.x + 260, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), "<-");
        window2->DrawList->AddText(NULL, 15.0f, ImVec2(vMin.x + 340, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), "->");
        if (labels[*number2] == "Off")
        {
            window2->DrawList->AddText(NULL, 15.0f, ImVec2(vMin.x + 295, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)), labels[*number2].c_str());
        }
        else if (labels[*number2] == "Autoheal")
        {
            window2->DrawList->AddText(NULL, 15.0f, ImVec2(vMin.x + 280, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)), labels[*number2].c_str());
        }
        else
        {
            window2->DrawList->AddText(NULL, 15.0f, ImVec2(vMin.x + 295, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)), labels[*number2].c_str());
        }
        window2->DrawList->AddLine(ImVec2(vMin.x + 90, vMin.y + 58 + yaddition), ImVec2(vMin.x + 380, vMin.y + 58 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 1.0f);
        if (GetAsyncKeyState(0x1))
        {
            POINT p3;

            GetCursorPos(&p3);

            bool hovering1 = 0;
            bool hovering2 = 0;

            for (int i = 0; i < 20; i++)
            {
                if ((vMin.y + 40 + yaddition + i) == p3.y) {
                    for (int i2 = 0; i2 < 20; i2++)
                    {
                        if ((vMin.x + 260 + i2) == p3.x) {
                            hovering1 = 1;
                        }
                    }
                }
            }

            for (int i = 0; i < 20; i++)
            {
                if ((vMin.y + 40 + yaddition - i) == p3.y) {
                    for (int i2 = 0; i2 < 20; i2++)
                    {
                        if ((vMin.x + 260 - i2) == p3.x) {
                            hovering1 = 1;
                        }
                    }
                }
            }

            for (int i = 0; i < 20; i++)
            {
                if ((vMin.y + 40 + yaddition + i) == p3.y) {
                    for (int i2 = 0; i2 < 20; i2++)
                    {
                        if ((vMin.x + 340 + i2) == p3.x) {
                            hovering2 = 1;
                        }
                    }
                }
            }

            for (int i = 0; i < 20; i++)
            {
                if ((vMin.y + 40 + yaddition - i) == p3.y) {
                    for (int i2 = 0; i2 < 20; i2++)
                    {
                        if ((vMin.x + 340 - i2) == p3.x) {
                            hovering2 = 1;
                        }
                    }
                }
            }


            if (hovering1)
            {
                Logger::Log("Hovering1");
                // window2->DrawList->AddText(ImVec2(vMin.x + 260, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), "ffffff");
                if (*number2 > 0) {
                    *number2 = *number2 - 1;
                    Sleep(150);
                }
            }
            else if (hovering2)
            {
                Logger::Log("Hovering2");
                //window2->DrawList->AddText(ImVec2(vMin.x + 340, vMin.y + 40 + yaddition), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), "rjgfrgiujr");
                if (*number2 < 2)
                {
                    *number2 = *number2 + 1;
                    Sleep(150);
                }

            }
            hovering1 = 0;
            hovering2 = 0;

            //window2->DrawList->AddText(ImVec2(p3.x, p3.y), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), ".");

        }
    }

    BOOL DrawButton1(std::string label, int yaddition, int textaddition) {
        ImGuiWindow* window2 = ImGui::GetCurrentWindow();

        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();

        vMin.x += ImGui::GetWindowPos().x;
        vMin.y += ImGui::GetWindowPos().y;
        vMax.x += ImGui::GetWindowPos().x;
        vMax.y += ImGui::GetWindowPos().y;
        bool hovering = false;

        POINT p3;

        GetCursorPos(&p3);

        for (int i = 0; i < 40; i++)
        {
            if ((vMin.y + 40 + yaddition + i) == p3.y) {
                for (int i2 = 0; i2 < 100; i2++)
                {
                    if ((vMin.x + 80 + i2) == p3.x) {
                        hovering = true;
                    }
                }
            }
        }


        window2->DrawList->AddRectFilled(ImVec2(vMin.x + 90, vMin.y + 40 + yaddition), ImVec2(vMin.x + 180, vMin.y + 40 + yaddition + 40), ImGui::GetColorU32(ImVec4(0.05f, 0.05f, 0.05f, 0.5f)));
        window2->DrawList->AddText(NULL, 16.0f, ImVec2(vMin.x + 100 + textaddition, vMin.y + 40 + yaddition + 10), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), label.c_str());

        
        if (GetAsyncKeyState(0x1))
        {
            

            if (hovering)
            {
                window2->DrawList->AddLine(ImVec2(vMin.x + 90, vMin.y + 40 + yaddition), ImVec2(vMin.x + 90, vMin.y + 40 + yaddition + 40), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 2.5f);
                Sleep(150);
                return true;
            }
            hovering = false;

            //window2->DrawList->AddText(ImVec2(p3.x, p3.y), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), ".");

        }
        return false;
    }

    BOOL DrawButton2(std::string label, int yaddition, int textaddition) {
        ImGuiWindow* window2 = ImGui::GetCurrentWindow();

        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();

        vMin.x += ImGui::GetWindowPos().x;
        vMin.y += ImGui::GetWindowPos().y;
        vMax.x += ImGui::GetWindowPos().x;
        vMax.y += ImGui::GetWindowPos().y;
        bool hovering = false;

        POINT p3;

        GetCursorPos(&p3);

        for (int i = 0; i < 40; i++)
        {
            if ((vMin.y + 40 + yaddition + i) == p3.y) {
                for (int i2 = 0; i2 < 90; i2++)
                {
                    if ((vMin.x + 90 + i2 + 100) == p3.x) {
                        hovering = true;
                    }
                }
            }
        }


        window2->DrawList->AddRectFilled(ImVec2(vMin.x + 90 + 100, vMin.y + 40 + yaddition), ImVec2(vMin.x + 180 + 100, vMin.y + 40 + yaddition + 40), ImGui::GetColorU32(ImVec4(0.05f, 0.05f, 0.05f, 0.5f)));
        window2->DrawList->AddText(NULL, 16.0f, ImVec2(vMin.x + 100 + textaddition + 100, vMin.y + 40 + yaddition + 10), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), label.c_str());


        if (GetAsyncKeyState(0x1))
        {


            if (hovering)
            {
                window2->DrawList->AddLine(ImVec2(vMin.x + 90 + 100, vMin.y + 40 + yaddition), ImVec2(vMin.x + 90 + 100, vMin.y + 40 + yaddition + 40), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 2.5f);
                Sleep(150);
                return true;
            }
            hovering = false;

            //window2->DrawList->AddText(ImVec2(p3.x, p3.y), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), ".");

        }
        return false;
    }

    BOOL DrawButton3(std::string label, int yaddition, int textaddition) {
        ImGuiWindow* window2 = ImGui::GetCurrentWindow();

        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();

        vMin.x += ImGui::GetWindowPos().x;
        vMin.y += ImGui::GetWindowPos().y;
        vMax.x += ImGui::GetWindowPos().x;
        vMax.y += ImGui::GetWindowPos().y;
        bool hovering = false;

        POINT p3;

        GetCursorPos(&p3);

        for (int i = 0; i < 40; i++)
        {
            if ((vMin.y + 40 + yaddition + i) == p3.y) {
                for (int i2 = 0; i2 < 90; i2++)
                {
                    if ((vMin.x + 90 + i2 + 200) == p3.x) {
                        hovering = true;
                    }
                }
            }
        }


        window2->DrawList->AddRectFilled(ImVec2(vMin.x + 90 + 200, vMin.y + 40 + yaddition), ImVec2(vMin.x + 180 + 200, vMin.y + 40 + yaddition + 40), ImGui::GetColorU32(ImVec4(0.05f, 0.05f, 0.05f, 0.5f)));
        window2->DrawList->AddText(NULL, 16.0f, ImVec2(vMin.x + 100 + textaddition + 200, vMin.y + 40 + yaddition + 10), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), label.c_str());


        if (GetAsyncKeyState(0x1))
        {


            if (hovering)
            {
                window2->DrawList->AddLine(ImVec2(vMin.x + 90 + 200, vMin.y + 40 + yaddition), ImVec2(vMin.x + 90 + 200, vMin.y + 40 + yaddition + 40), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 2.5f);
                Sleep(150);
                return true;
            }
            hovering = false;

            //window2->DrawList->AddText(ImVec2(p3.x, p3.y), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), ".");

        }
        return false;
    }

    void Draw() {
        {



            if (GetAsyncKeyState(VK_INSERT) & 1)
                Config::Menu::enabled = !Config::Menu::enabled;

            if (Config::Menu::enabled)
            {
                ImGui::CaptureKeyboardFromApp(false);
            }
            else
            {
                ImGui::CaptureKeyboardFromApp(true);
            }

            ShowOverlay();

            ImGuiStyle* style = &ImGui::GetStyle();

            style->WindowRounding = 0.0f;
            style->WindowBorderSize = 0.0f;
            style->FrameRounding = 0.0f;
            style->ScrollbarRounding = 9.0f;
            style->GrabRounding = 0.0f;

            style->Colors[ImGuiCol_TitleBg] = ImVec4(15 / 255, 15 / 255, 15 / 255, 1.0f);
            style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(15 / 255, 15 / 255, 15 / 255, 1.0f);
            style->Colors[ImGuiCol_TitleBgActive] = ImVec4(15 / 255, 15 / 255, 15 / 255, 1.0f);
            style->Colors[ImGuiCol_WindowBg] = ImVec4(15 / 255, 15 / 255, 15 / 255, 1.0f);
            style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(15 / 255, 15 / 255, 15 / 255, 1.0f);
            style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(15 / 255, 15 / 255, 15 / 255, 1.0f);
            style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(15 / 255, 15 / 255, 15 / 255, 1.0f);
            style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(15 / 255, 15 / 255, 15 / 255, 1.0f);
            style->Colors[ImGuiCol_Separator] = ImVec4(1.0f, 0.0f, 0.0f, 0.6f);
            style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.2f, 0.39f, 1.0f);
            style->Colors[ImGuiCol_Button] = ImVec4(15 / 255, 15 / 255, 15 / 255, 1.0f);
            style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.23f, 0.15, 0.2, 0.5f);
            ImGui::SetMouseCursor(ImGuiMouseCursor_None);
                ImGuiWindow* window = ImGui::GetCurrentWindow();
                POINT p;

                GetCursorPos(&p);

                    if (Config::Player::crosshairnumber == 1) {
                        float x = ImGui::GetIO().DisplaySize.x * 0.5f;
                        float y = ImGui::GetIO().DisplaySize.y * 0.5f;
                        window->DrawList->AddLine(ImVec2(x - 12, y), ImVec2(x + 12, y), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 1.4f);
                        window->DrawList->AddLine(ImVec2(x, y - 12), ImVec2(x, y + 12), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 1.4f);
                    }


                    if (!Config::Menu::enabled)
                        return;

                    

                    window->DrawList->AddLine(ImVec2(p.x - 23, p.y), ImVec2(p.x + 23, p.y), ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 1.0f, 0.6f)), 0.8f);
                    window->DrawList->AddLine(ImVec2(p.x, p.y - 23), ImVec2(p.x, p.y + 23), ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 1.0f, 0.6f)), 0.8f);
                    window->DrawList->AddLine(ImVec2(p.x - 17, p.y), ImVec2(p.x + 17, p.y), ImGui::GetColorU32(ImVec4(0.5f, 0.0f, 0.5f, 0.6f)), 1.0f);
                    window->DrawList->AddLine(ImVec2(p.x, p.y - 17), ImVec2(p.x, p.y + 17), ImGui::GetColorU32(ImVec4(0.5f, 0.0f, 0.5f, 0.6f)), 1.0f);
                    window->DrawList->AddLine(ImVec2(p.x - 15, p.y), ImVec2(p.x + 15, p.y), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 1.2f);
                    window->DrawList->AddLine(ImVec2(p.x, p.y - 15), ImVec2(p.x, p.y + 15), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 1.2f);
                    
                    std::string str55("1.0");
                    std::string se = "soulcheats.cc/store - GTA5 " + str55;
                    const char* c = se.c_str();
                    ImGui::GetStyle().WindowTitleAlign = ImVec2(0.7f, 0.5f);
                    ImGui::Begin(c, NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);
                    ImGui::SetWindowSize(ImVec2(400.0f, 300.0f));
                    ImGui::Dummy(ImVec2(70.0f, 15.0f));
                    ImGui::SameLine();
                    ImGui::Text(c);
                    ImGui::Separator();
                    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
                    ImVec2 vMax = ImGui::GetWindowContentRegionMax();

                    vMin.x += ImGui::GetWindowPos().x;
                    vMin.y += ImGui::GetWindowPos().y;
                    vMax.x += ImGui::GetWindowPos().x;
                    vMax.y += ImGui::GetWindowPos().y;

                    ImGuiWindow* window2 = ImGui::GetCurrentWindow();

                    if (section == "Player") {
                        window2->DrawList->AddLine(ImVec2(vMin.x - 2, vMin.y + 23), ImVec2(vMin.x - 2, vMin.y + 63), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 3.2f);
                        DrawOnOff(&Config::Player::informationnumber, onoff, 0, "Information");
                        DrawOnOff(&Config::Player::crosshairnumber, onoff, 25, "Crosshair");
                        Draw3Switch(&Config::Player::godmodenumber, godmode, 50, "God Mode");
                        DrawOnOff(&Config::Player::invisiblenumber, onoff, 75, "Invisible");
                        if (DrawButton1("Tp To Way", 100, 1))
                        {
                            SDK::PLAYER::Set_Position(SDK::WORLD::Waypoint_Position());
                        }

                        if (DrawButton2("Reset Health", 100, -5))
                        {
                            SDK::PLAYER::Set_Health(200.0f);
                        }

                        if (DrawButton3("Reset Armour", 100, -5))
                        {
                            SDK::PLAYER::Set_Armor(100.0f);
                        }

                        std::string heal = "0x" + std::to_string(Config::Player::healkey);
                        std::string armor = "0x" + std::to_string(Config::Player::armorkey);
                        std::string tptoway = "0x" + std::to_string(Config::Player::tptoway);

                        if (DrawButton1(tptoway.c_str(), 150, 10))
                        {
                            for (int i = 0; i <= 1000; i++) {
                                if (GetAsyncKeyState(i) && i != 1 && i != 2) {
                                    Config::Player::tptoway = i;
                                    continue;
                                }
                                if (GetAsyncKeyState(i) == 2) {
                                    Config::Player::tptoway = 0;
                                    continue;
                                }
                            }
                        }

                        

                        if (DrawButton2(heal.c_str(), 150, 20))
                        {
                            for (int i = 0; i <= 1000; i++) {
                                if (GetAsyncKeyState(i) && i != 1 && i != 2) {
                                    Config::Player::healkey = i;
                                    continue;
                                }
                                if (GetAsyncKeyState(i) == 2) {
                                    Config::Player::healkey = 0;
                                    continue;
                                }
                            }
                        }

                        if (DrawButton3(armor.c_str(), 150, 20))
                        {
                            for (int i = 0; i <= 1000; i++) {
                                if (GetAsyncKeyState(i) && i != 1 && i != 2) {
                                    Config::Player::armorkey = i;
                                    continue;
                                }
                                if (GetAsyncKeyState(i) == 2) {
                                    Config::Player::armorkey = 0;
                                    continue;
                                }
                            }
                        }

                        if (DrawButton1("Suicide", 200, 12))
                        {
                            SDK::PLAYER::Set_Health(0.0f);
                        }
                       // DrawOnOff(&Config::Player::invisiblenumber, invisible, 100, "Invisible");
                       // DrawOnOff(&Config::Player::nogravnumber, nograv, 125, "No Gravity");
                       // DrawOnOff(&Config::Player::vehicleespnumber, vehicleesp, 150, "Vehicle ESP");
                       // DrawOnOff(&Config::Player::objectespnumber, objectesp, 175, "Object ESP");
                    }

                    if (section == "Weapon") {
                        window2->DrawList->AddLine(ImVec2(vMin.x - 2, vMin.y + 69), ImVec2(vMin.x - 2, vMin.y + 107), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 4.2f);
                        DrawOnOff(&Config::Weapon::norecoilnumber, onoff, 0, "No Recoil");
                        DrawOnOff(&Config::Weapon::nospreadnumber, onoff, 25, "No Spread");
                        DrawOnOff(&Config::Weapon::noreloadnumber, onoff, 50, "No Reload");
                        DrawOnOff(&Config::Weapon::rapidefirenumber, onoff, 75, "Rapide Fire");


                        if (DrawButton1("Give Weapon 1", 150, -5))
                        {
                            static auto gib_de_weapon = [](uint32_t hash)
                            {
                                auto local_player = *(uintptr_t*)(SDK::World + 0x8);
                                auto ped = SDK::pointer_to_handle((uintptr_t)local_player);
                                SDK::give_weapon(ped, hash, 999, true);
                            };

                            static auto rage_joaat = [](const char* str) -> std::int32_t
                            {
                                static auto to_lowercase = [](char c) -> char
                                {
                                    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
                                };

                                std::uint32_t hash = 0;
                                while (*str)
                                {
                                    hash += to_lowercase(*str++);
                                    hash += (hash << 10);
                                    hash ^= (hash >> 6);
                                }

                                hash += (hash << 3);
                                hash ^= (hash >> 11);
                                hash += (hash << 15);

                                return (signed)hash;
                            };

                            gib_de_weapon(rage_joaat(Config::Misc::weapon1.c_str()));
                            
                        }


                        if (DrawButton2("Give Weapon 2", 150, -5))
                        {
                            static auto gib_de_weapon = [](uint32_t hash)
                            {
                                auto local_player = *(uintptr_t*)(SDK::World + 0x8);
                                auto ped = SDK::pointer_to_handle((uintptr_t)local_player);
                                SDK::give_weapon(ped, hash, 999, true);
                            };

                            static auto rage_joaat = [](const char* str) -> std::int32_t
                            {
                                static auto to_lowercase = [](char c) -> char
                                {
                                    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
                                };

                                std::uint32_t hash = 0;
                                while (*str)
                                {
                                    hash += to_lowercase(*str++);
                                    hash += (hash << 10);
                                    hash ^= (hash >> 6);
                                }

                                hash += (hash << 3);
                                hash ^= (hash >> 11);
                                hash += (hash << 15);

                                return (signed)hash;
                            };

                            gib_de_weapon(rage_joaat(Config::Misc::weapon2.c_str()));
                        }

                        if (DrawButton3("Give Weapon 3", 150, 0))
                        {
                            static auto gib_de_weapon = [](uint32_t hash)
                            {
                                auto local_player = *(uintptr_t*)(SDK::World + 0x8);
                                auto ped = SDK::pointer_to_handle((uintptr_t)local_player);
                                SDK::give_weapon(ped, hash, 999, true);
                            };

                            static auto rage_joaat = [](const char* str) -> std::int32_t
                            {
                                static auto to_lowercase = [](char c) -> char
                                {
                                    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
                                };

                                std::uint32_t hash = 0;
                                while (*str)
                                {
                                    hash += to_lowercase(*str++);
                                    hash += (hash << 10);
                                    hash ^= (hash >> 6);
                                }

                                hash += (hash << 3);
                                hash ^= (hash >> 11);
                                hash += (hash << 15);

                                return (signed)hash;
                            };

                            gib_de_weapon(rage_joaat(Config::Misc::weapon3.c_str()));
                        }

                        if (DrawButton1("Give Weapon 4", 200, -5))
                        {
                            static auto gib_de_weapon = [](uint32_t hash)
                            {
                                auto local_player = *(uintptr_t*)(SDK::World + 0x8);
                                auto ped = SDK::pointer_to_handle((uintptr_t)local_player);
                                SDK::give_weapon(ped, hash, 999, true);
                            };

                            static auto rage_joaat = [](const char* str) -> std::int32_t
                            {
                                static auto to_lowercase = [](char c) -> char
                                {
                                    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
                                };

                                std::uint32_t hash = 0;
                                while (*str)
                                {
                                    hash += to_lowercase(*str++);
                                    hash += (hash << 10);
                                    hash ^= (hash >> 6);
                                }

                                hash += (hash << 3);
                                hash ^= (hash >> 11);
                                hash += (hash << 15);

                                return (signed)hash;
                            };

                            gib_de_weapon(rage_joaat(Config::Misc::weapon4.c_str()));
                        }
                        if (DrawButton2("Give Weapon 5", 200, -4))
                        {
                            static auto gib_de_weapon = [](uint32_t hash)
                            {
                                auto local_player = *(uintptr_t*)(SDK::World + 0x8);
                                auto ped = SDK::pointer_to_handle((uintptr_t)local_player);
                                SDK::give_weapon(ped, hash, 999, true);
                            };

                            static auto rage_joaat = [](const char* str) -> std::int32_t
                            {
                                static auto to_lowercase = [](char c) -> char
                                {
                                    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
                                };

                                std::uint32_t hash = 0;
                                while (*str)
                                {
                                    hash += to_lowercase(*str++);
                                    hash += (hash << 10);
                                    hash ^= (hash >> 6);
                                }

                                hash += (hash << 3);
                                hash ^= (hash >> 11);
                                hash += (hash << 15);

                                return (signed)hash;
                            };

                            gib_de_weapon(rage_joaat(Config::Misc::weapon5.c_str()));
                        }

                    }

                    if (section == "Aimbot") {
                        window2->DrawList->AddLine(ImVec2(vMin.x - 2, vMin.y + 113), ImVec2(vMin.x - 2, vMin.y + 151), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 4.2f);
                        DrawOnOff(&Config::Aimbot::enablednumber, onoff, 0, "Enabled");
                        Draw3Switch(&Config::Aimbot::distancenumber, aimbotdistance, 25, "Max Distance");
                        Draw3Switch(&Config::Aimbot::bonenumber, aimbotbone, 50, "Bone");
                        DrawOnOff(&Config::Aimbot::hardlocknumber, onoff, 75, "Hard Lock");
                        DrawOnOff(&Config::Aimbot::silentnumber, onoff, 100, "Silent Aim");
                        DrawOnOff(&Config::Aimbot::drawfownumber, onoff, 125, "Draw FOV");
                        Draw3Switch(&Config::Aimbot::fovnumber, fov, 150, "FOV");
                        Draw3Switch(&Config::Aimbot::smoothnumber, smooth, 175, "Smooth");
                        Draw3Switch(&Config::Aimbot::targetnumber, targets, 200, "Target Selection");
                        std::string lost = "0x" + std::to_string(Config::Aimbot::aimkey);
                        if (DrawButton1(lost.c_str(), 215, 20))
                        {
                            for (int i = 0; i <= 1000; i++) {
                                if (GetAsyncKeyState(i) && i != 1 && i != 2) {
                                    Config::Aimbot::aimkey = i;
                                    continue;
                                }
                                if (GetAsyncKeyState(i) == 2) {
                                    Config::Aimbot::aimkey = 0;
                                    continue;
                                }
                            }
                        }
                    }

                    if (section == "Vehicle") {
                        window2->DrawList->AddLine(ImVec2(vMin.x - 2, vMin.y + 157), ImVec2(vMin.x - 2, vMin.y + 195), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 4.2f);
                        DrawOnOff(&Config::Vehicle::unlock_doors, onoff, 0, "Unlock Locked Cars");
                        DrawOnOff(&Config::Vehicle::autorepair, onoff, 25, "Fix On damage");
                        DrawOnOff(&Config::Vehicle::godmode, onoff, 50, "Vehicle Godmode");
                        Draw3Switch(&Config::Vehicle::speedmultiplier, speedhack, 75, "Vehicle Speed Multiplier");
                        if (DrawButton1("Fix", 90, 20))
                        {
                            SDK::VEHICLE::Set_Health(1000.0f, 1000.0f, 65.0f);
                        }
                        if (DrawButton2("Destroy", 90, 20))
                        {
                            SDK::VEHICLE::Set_Health(0.0f, 0.0f, 0.0f);
                        }
                    }

                    if (section == "3D Radar") {
                        window2->DrawList->AddLine(ImVec2(vMin.x - 2, vMin.y + 201), ImVec2(vMin.x - 2, vMin.y + 239), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 4.2f);
                        Draw3Switch(&Config::ESP::distance, espdistance, 0, "ESP Distance");
                        DrawOnOff(&Config::ESP::name, onoff, 25, "Name");
                        DrawOnOff(&Config::ESP::boxes, onoff, 50, "Box");
                        DrawOnOff(&Config::ESP::skelet, onoff, 75, "Skeleton");
                        DrawOnOff(&Config::ESP::health, onoff, 100, "Health");
                        DrawOnOff(&Config::ESP::enabled, onoff, 125, "Enabled");
                    }

                    if (section == "Other") {
                        window2->DrawList->AddLine(ImVec2(vMin.x - 2, vMin.y + 245), ImVec2(vMin.x - 2, vMin.y + 283), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 4.2f);
                        if (DrawButton1("Load Settings 1", 0, 20))
                        {
                            Config::Load(1);
                        }
                        if (DrawButton2("Load Settings 2", 0, 20))
                        {
                            Config::Load(2);
                        }
                        if (DrawButton3("Load Settings 3", 0, 20))
                        {
                            Config::Load(3);
                        }
                        if (DrawButton1("Save Settings 1", 50, 20))
                        {
                            Config::Save(1);
                        }
                        if (DrawButton2("Save Settings 2", 50, 20))
                        {
                            Config::Save(2);
                        }
                        if (DrawButton3("Save Settings 3", 50, 20))
                        {
                            Config::Save(3);
                        }
                        if (DrawButton1("Unhook", 100, 20))
                        {
                            Config::Menu::unload = true;
                        }
                    }

                    if (ImGui::Button("Player", ImVec2(90.0f, 40.0f)))
                        section = "Player";

                    if (ImGui::Button("Weapon", ImVec2(90.0f, 40.0f)))
                        section = "Weapon";

                    if (ImGui::Button("Aimbot", ImVec2(90.0f, 40.0f)))
                        section = "Aimbot";

                    if (ImGui::Button("Vehicle", ImVec2(90.0f, 40.0f)))
                        section = "Vehicle";

                    if (ImGui::Button("3D Radar", ImVec2(90.0f, 40.0f)))
                        section = "3D Radar";

                    if (ImGui::Button("Other", ImVec2(90.0f, 40.0f)))
                        section = "Other";

                    window2->DrawList->AddLine(ImVec2(p.x - 23, p.y), ImVec2(p.x + 23, p.y), ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 1.0f, 0.6f)), 0.8f);
                    window2->DrawList->AddLine(ImVec2(p.x, p.y - 23), ImVec2(p.x, p.y + 23), ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 1.0f, 0.6f)), 0.8f);
                    window2->DrawList->AddLine(ImVec2(p.x - 17, p.y), ImVec2(p.x + 17, p.y), ImGui::GetColorU32(ImVec4(0.5f, 0.0f, 0.5f, 0.6f)), 1.0f);
                    window2->DrawList->AddLine(ImVec2(p.x, p.y - 17), ImVec2(p.x, p.y + 17), ImGui::GetColorU32(ImVec4(0.5f, 0.0f, 0.5f, 0.6f)), 1.0f);
                    window2->DrawList->AddLine(ImVec2(p.x - 15, p.y), ImVec2(p.x + 15, p.y), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 1.2f);
                    window2->DrawList->AddLine(ImVec2(p.x, p.y - 15), ImVec2(p.x, p.y + 15), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.6f)), 1.2f);
                    ImGui::End();
                
          
        }
    }



}
