#include <Windows.h>
#include <vector>
#include <d3d11.h>
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "winmm.lib")
#include "MinHook/include/MinHook.h" //detour x86&x64
#pragma warning(disable : 4244)

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include <DirectXMath.h>
using namespace DirectX;


typedef HRESULT(__stdcall* D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
D3D11PresentHook phookD3D11Present = NULL;

ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;

DWORD_PTR* pSwapChainVtable = NULL;
DWORD_PTR* pContextVTable = NULL;
DWORD_PTR* pDeviceVTable = NULL;


//viewport
UINT vps = 1;
D3D11_VIEWPORT viewport;
float ScreenCenterX;
float ScreenCenterY;

ID3D11RenderTargetView* RenderTargetView = NULL;
HRESULT hr;