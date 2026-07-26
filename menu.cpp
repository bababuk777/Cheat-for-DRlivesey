#include <iostream>
#include <Windows.h>
#include <imgui.h>

#include "config.h"
#include "hacks.h"

bool isMenuClickable = true;

void HandleKeyboardInput(HWND window)
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		isMenuClickable = !isMenuClickable;

		if (isMenuClickable == false)
		{
			SetWindowLongPtr(window, GWL_EXSTYLE,
				WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_NOACTIVATE);
		}
		else
		{
			SetWindowLongPtr(window, GWL_EXSTYLE,
				WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_NOACTIVATE);
		}
	}
}


void RenderMenu() {
	if (!isMenuClickable) 
		return;
	ImGui::Begin("Menu");
	if (ImGui::BeginTabBar(""))
	{
		if (ImGui::BeginTabItem("Player")) 
		{
			if (ImGui::SliderFloat("Speedhack", &config.Speedhack, 1.0f, 100.0f, "%.1f"))
				Speedhack();
			if (ImGui::Checkbox("Basehealth", &config.Basehealth))
				Basehealth();
			if (ImGui::Checkbox("Goldhealth", &config.Goldhealth))
				Goldhealth();
			if (ImGui::Checkbox("Invisible", &config.Invisible))
				Invisible();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Weapon")) 
		{
			if (ImGui::Checkbox("1hit", &config.Damage))
				Damage();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Misk"))
		{
			if (ImGui::Checkbox("Invisiblienemy", &config.Invisiblienemy))
				Invisiblienemy();
			if (ImGui::Checkbox("Freewall", &config.Freewall))
				Freewall();
			if (ImGui::Checkbox("Phonkoff", &config.Music))
				Music();
			if (ImGui::Checkbox("Killboss", &config.Boss))
			{
				FindFBossadress();
				Boss();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}