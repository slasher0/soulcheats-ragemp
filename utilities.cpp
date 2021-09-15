#include <time.h>
#include "utilities.h"
#include <chrono>
#include <ctime>   
#include "imgui/imgui.h"
#include "Logger.h"
#include <thread>

int utilities::getFPS()
{
	return ImGui::GetIO().Framerate;
};