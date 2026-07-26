#include <Windows.h>
#include <iostream>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <dwmapi.h>
#include <d3d11.h>
#include <imgui_internal.h>

#include "menu.h"
#include "config.h"
#include "memory_functions.h"
#include "globals.h"
#include "hacks.h"

using namespace std;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool g_ExitApplication = false;

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param))
	{
		return 0L;
	}
	if (message == WM_DESTROY || message == WM_QUIT) {
		g_ExitApplication = true;
		return 0L;
	}

	return DefWindowProc(window, message, w_param, l_param);
}

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {

#pragma region InitializationAndSetup

	WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, window_procedure, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };

	RegisterClassExW(&wc);

	const HWND window = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_NOACTIVATE,
		wc.lpszClassName,
		L"External Overlay",
		WS_POPUP,
		0,
		0,
		1920,
		1080,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);


	RECT client_area{}; GetClientRect(window, &client_area);
	RECT window_area{};
	GetWindowRect(window, &window_area);
	POINT diff{};
	ClientToScreen(window, &diff);
	const MARGINS margins{
		window_area.left + (diff.x - window_area.left),
		window_area.top + (diff.y - window_area.top),
		client_area.right,
		client_area.bottom
	};
	DwmExtendFrameIntoClientArea(window, &margins);

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.RefreshRate.Numerator = 60U;
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2U;
	sd.OutputWindow = window;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.Flags = 0;

	constexpr D3D_FEATURE_LEVEL levels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};
	ID3D11Device* device{ nullptr };
	ID3D11DeviceContext* device_context{ nullptr };
	IDXGISwapChain* swap_chain{ nullptr };
	ID3D11RenderTargetView* render_target_view{ nullptr };
	D3D_FEATURE_LEVEL level{};

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0U,
		levels,
		2U,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&level,
		&device_context
	);

	ID3D11Texture2D* back_buffer{ nullptr }; swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));
	if (back_buffer) {
		device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
		back_buffer->Release();
	}
	else {
		return 1;
	}

	ShowWindow(window, cmd_show); UpdateWindow(window);
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, device_context);
#pragma endregion


	AllocConsole();
	FILE* consoleOut;
	freopen_s(&consoleOut, "CONOUT$", "w", stdout);

	std::cout << "Waiting for the game to start";

	int dots = 0;
	while (!AttachToProcess())
	{
		std::cout << "\rWaiting for the game to start";
		for (int i = 0; i < dots; i++) std::cout << ".";
		std::cout << "   " << std::flush;

		dots = (dots + 1) % 4;
		Sleep(1000);
	}

	std::cout << "\r                                              \r";
	std::cout << "Game found! Successfully attached." << std::flush;
	Sleep(700);

	std::cout << "\r                                              \r";
	std::cout << "Searching for addresses";
	dots = 0;

	bool addressesFound = false;

	while (!addressesFound)
	{
		std::cout << "\rSearching for addresses";
		for (int i = 0; i < dots; i++) std::cout << ".";
		std::cout << "   " << std::flush;

		dots = (dots + 1) % 4;
		Sleep(200);

		FindinvisiblienemyAddress();
		UpdateAdress();

		if (InventoryListAdress != 0)
		{
			addressesFound = true;
		}
	}

	std::cout << "\rCheat Ready!                              " << std::endl;
	
	while (!g_ExitApplication) {
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		HandleKeyboardInput(window);
		RenderMenu();
		static int frameCount = 0;
		if (++frameCount >= 60)          // примерно раз в секунду (при 60 FPS)
		{
			UpdateAdress();
			Boss();
			frameCount = 0;
		}
		if(config.Damage)
			Damage();
		if (config.Freewall)
			Freewall();
		if (config.Music)
			Music();
		if (config.Boss)
			Boss();
		if (config.Speedhack)
			Speedhack();
		if (config.Invisible)
			Invisible();
		if (config.Goldhealth)
			Goldhealth();
		if (config.Basehealth)
			Basehealth();

		SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();

		
		ImGui::Render();
		constexpr float color[4]{ 0.f, 0.f, 0.f, 0.f };
		device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
		device_context->ClearRenderTargetView(render_target_view, color);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		swap_chain->Present(1U, 0U);
	}

#pragma region CleanupAndShutdown
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (swap_chain) {
		swap_chain->Release();
	}

	if (device_context) {
		device_context->Release();
	}

	if (device) {
		device->Release();
	}

	if (render_target_view) {
		render_target_view->Release();
	}

	DestroyWindow(window);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
#pragma endregion

	return 0;
}