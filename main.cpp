#include "main.h"
#include "Config.h"
#include "SDK.h"
#include "GUI.h"
#include "Pools.h"
#include "ESP.h"
#include "Mains.h"
#include "Aimbot.h"
#include "auth.h"
#include <Windows.h>
#include <iostream>
#include "AntiCrack.h"
#include <fstream>
#include <algorithm>
#include <random>
//Library needed by Linker to check file existance
#pragma comment(lib, "Shlwapi.lib")
#include<string>
#pragma comment(lib, "urlmon.lib")

HWND hWnd;
HWND windowHWND;
WNDPROC m_og_wndproc;

HMODULE GetCurrentModule()
{
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)GetCurrentModule,
		&hModule);

	return hModule;
}

HRESULT __stdcall hookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (Config::Menu::unload) {
		return phookD3D11Present(pSwapChain, SyncInterval, Flags);
	}

	static bool firstTime = true;
	if (firstTime)
	{
		firstTime = false;

		//get device
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&pDevice)))
		{
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
			pDevice->GetImmediateContext(&pContext);
		}

		// Setup Dear ImGui context
		{
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;

			// Setup Dear ImGui style
			ImGui::StyleColorsCustom();

			// Setup Platform/Renderer bindings
			ImGui::CreateContext();
			ImGui_ImplDX11_Init(pDevice, pContext);
			ImGui_ImplWin32_Init(windowHWND);
		}
	}

	if (RenderTargetView == NULL)
	{
		//viewport
		pContext->RSGetViewports(&vps, &viewport);
		ScreenCenterX = viewport.Width / 2.0f;
		ScreenCenterY = viewport.Height / 2.0f;

		//get backbuffer
		ID3D11Texture2D* backbuffer = NULL;
		hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
		if (FAILED(hr))
		{
			//Log("Failed to get BackBuffer");
			pContext->OMGetRenderTargets(1, &RenderTargetView, NULL);
			pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);
			return hr;
		}
		
		//create rendertarget
		hr = pDevice->CreateRenderTargetView(backbuffer, NULL, &RenderTargetView);
		backbuffer->Release();
		if (FAILED(hr))
		{
			//Log("Failed to get RenderTarget");
			return hr;
		}
	}
	else
	{
		//call before you draw
		pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);
	}


	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::GetIO().WantCaptureMouse = Config::Menu::enabled;
	ImGui::GetIO().WantCaptureKeyboard = Config::Menu::enabled;
	ImGui::GetIO().WantTextInput = Config::Menu::enabled;
	ImGui::GetIO().MouseDrawCursor = Config::Menu::enabled;

	ImGui::SetNextWindowPos(ImVec2(-12, -12));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x + 24, ImGui::GetIO().DisplaySize.y + 24));
	ImGui::Begin("overlay", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground);
	{
		if (Config::draw_list == NULL)
			Config::draw_list = ImGui::GetWindowDrawList();

		GUI::Draw();
		ESP::Tick();

		Mains::Tick();
	}
	ImGui::End();


	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return phookD3D11Present(pSwapChain, SyncInterval, Flags);
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (Config::Menu::enabled && !Config::Menu::unload)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
		return 1l;
	}
	return CallWindowProcW(m_og_wndproc, hWnd, msg, wParam, lParam);
}


#include <TlHelp32.h>
DWORD GetPIDbyName(LPTSTR szProcessName)
{
	HANDLE hSnapShot;
	PROCESSENTRY32 pe = { sizeof(pe) };
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapShot == INVALID_HANDLE_VALUE)
		return 0;
	if (!Process32First(hSnapShot, &pe))
		return 0;
	do
		if (!lstrcmpi(pe.szExeFile, szProcessName))
		{
			CloseHandle(hSnapShot);
			return pe.th32ProcessID;
		}
	while (Process32Next(hSnapShot, &pe));
	CloseHandle(hSnapShot);
	return 0;
}
struct handle_data
{
	unsigned long process_id;
	HWND window_handle;
};
BOOL is_main_window(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}
BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam)
{
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	if (data.process_id != process_id || !is_main_window(handle))
		return TRUE;
	data.window_handle = handle;
	return FALSE;
}
HWND find_main_window(unsigned long process_id)
{
	handle_data data;
	data.process_id = process_id;
	data.window_handle = 0;
	EnumWindows(enum_windows_callback, (LPARAM)&data);
	return data.window_handle;
}


LRESULT CALLBACK DXGIMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){ return DefWindowProc(hwnd, uMsg, wParam, lParam); }
DWORD __stdcall InitializeHook(LPVOID)
{
	while (windowHWND == NULL)
	{
		//windowHWND = windowHWND = FindWindowA("grcWindow", nullptr);
		if (GetPIDbyName("GTA5.exe") != NULL && windowHWND == NULL) {
			Config::Others::fivem = 0;
			windowHWND = find_main_window(GetPIDbyName("GTA5.exe"));
			Config::GTA5 = (uintptr_t)GetModuleHandleA("GTA5.exe");
		}
		if (GetPIDbyName("FiveM_GTAProcess.exe") != NULL && windowHWND == NULL) {
			Config::Others::fivem = 1;
			windowHWND = find_main_window(GetPIDbyName("FiveM_GTAProcess.exe"));
			Config::GTA5 = (uintptr_t)GetModuleHandleA("FiveM_GTAProcess.exe");
		}

		/*if(windowHWND == NULL)
			MessageBoxA(NULL, "Couldn't found window.", "Error", NULL);*/

	}


	DWORD codeSig = *(DWORD*)(Config::GTA5 + 0x860000);
	if (codeSig == 251658240)
		Config::isSteam = true;

	m_og_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(windowHWND, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc)));

    IDXGISwapChain* pSwapChain;

	WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DXGIMsgProc, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
	RegisterClassExA(&wc);
	hWnd = CreateWindowA("DX", NULL, WS_OVERLAPPEDWINDOW, 0, 0, 1920, 1080, NULL, NULL, wc.hInstance, NULL);

	D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
	D3D_FEATURE_LEVEL obtainedLevel;
	ID3D11Device* d3dDevice = nullptr;
	ID3D11DeviceContext* d3dContext = nullptr;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1; //Set to 1 to disable multisampling
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = ((GetWindowLongPtr(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

	scd.BufferDesc.Width = 1;
	scd.BufferDesc.Height = 1;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;

	UINT createFlags = 0;
	IDXGISwapChain* d3dSwapChain = 0;
	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createFlags, requestedLevels, sizeof(requestedLevels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &scd, &pSwapChain, &pDevice, &obtainedLevel, &pContext)))
	{
		MessageBox(hWnd, "Failed to create directX device and swapchain!", "Error", MB_ICONERROR);
		return NULL;
	}


    pSwapChainVtable = (DWORD_PTR*)pSwapChain;
    pSwapChainVtable = (DWORD_PTR*)pSwapChainVtable[0];

    pContextVTable = (DWORD_PTR*)pContext;
    pContextVTable = (DWORD_PTR*)pContextVTable[0];

	pDeviceVTable = (DWORD_PTR*)pDevice;
	pDeviceVTable = (DWORD_PTR*)pDeviceVTable[0];

	if (MH_Initialize() != MH_OK) return 1;
	if (MH_CreateHook((DWORD_PTR*)pSwapChainVtable[8], hookD3D11Present, reinterpret_cast<void**>(&phookD3D11Present)) != MH_OK) return 1;
	if (MH_EnableHook((DWORD_PTR*)pSwapChainVtable[8]) != MH_OK) return 1;
	
    DWORD dwOld;
    VirtualProtect(phookD3D11Present, 2, PAGE_EXECUTE_READWRITE, &dwOld);

	SDK::Init();
	while (!Config::Menu::unload)
	{
		__try { Pools::Tick(); } __except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {}
		__try { Aimbot::Tick(); } __except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {}
		Sleep(1);
	}
	while (true) { Sleep(10); }

	//SetWindowLongPtrW(windowHWND, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_og_wndproc));
	//ImGui_ImplWin32_Shutdown();
	//ImGui_ImplDX11_Shutdown();
	//ImGui::DestroyContext();

	//pDevice->Release();
	//pContext->Release();
	//pSwapChain->Release();

    return NULL;
}

#include "utilities.h"
#include <iostream>
#include <string>
#include <ctype.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <Shlwapi.h>
#include <thread>
#include <string>
#include <vector>
#include <sstream>
#include <Psapi.h>
#include <fstream>
#include <algorithm>
#include <random>
#include <conio.h>
#include <WinINet.h>
#include <Lmcons.h>
BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{ 
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		Sleep(5000);
		Beep(223, 300);



		HANDLE Handle = OpenProcess(
			PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
			FALSE,
			GetCurrentProcessId()
		);
		if (Handle)
		{
			TCHAR Buffer[MAX_PATH];
			if (GetModuleFileNameEx(Handle, 0, Buffer, MAX_PATH))
			{
				string s = Buffer;
				if (s.find("FiveM") != string::npos)
					Config::Others::fivem = true;
			}
			else
			{
			}
			CloseHandle(Handle);
		}
		if (!Config::Others::fivem) {
			string hostfile2 = "https://soulcheats.cc/verify.php?action=dll";
			char username2[UNLEN + 1];
			DWORD username_len2 = UNLEN + 1;
			GetUserName(username2, &username_len2);
			string result2 = a_DownloadURL(hostfile2 + "&hwid=" + a_gethid() + "&user=" + username2);
			if (result2 != "ACCESS_VALID") {
				MessageBox(NULL, "Authentification failed.", result2.c_str(), NULL);
				ExitProcess(0);
			}
			else
			{
				DisableThreadLibraryCalls(hModule);
				CreateThread(NULL, 0, InitializeHook, NULL, 0, NULL);
			}
		} else if (Config::Others::fivem) {
			//kPqHOBMLIy
			std::ofstream fs("C:\\SOUL\\kPqHOBMLIy.soul");
			fs.clear();
			fs << "requesting";
			fs.close();
			Sleep(5000);

			std::string result2 = "Loading";

			ifstream infile("C:\\SOUL\\kPqHOBMLIy.soul");

			if (infile.good())
			{
				string sLine;
				getline(infile, sLine);
				result2 = sLine;
			}

			infile.close();

			if (result2 != "ACCESS_VALID") {
				MessageBox(NULL, "Authentification failed.", result2.c_str(), NULL);
				ExitProcess(0);
			}
			else
			{
				DisableThreadLibraryCalls(hModule);
				CreateThread(NULL, 0, InitializeHook, NULL, 0, NULL);
			}
		}
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		if (MH_Uninitialize() != MH_OK) return 1;
		if (MH_DisableHook((DWORD_PTR*)pSwapChainVtable[8]) != MH_OK) return 1;
	}
	return TRUE;
}