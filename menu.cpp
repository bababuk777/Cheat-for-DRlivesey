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
			if (ImGui::SliderFloat("Speedhack", &config.Speedhack, 1.0f, 100.0f, "%.1f")) Speedhack();
			if (ImGui::Checkbox("Infiniti_HP", &config.InfinityHP));
			if (ImGui::Checkbox("Invisible", &config.Invisible));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Weapon"))
		{
			if (ImGui::Checkbox("AIM", &config.AIM));
			if (ImGui::Checkbox("1hit", &config.Damage));
			if (ImGui::Checkbox("RapidFire", &config.RapidFire));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("ESP")) {

			//if (ImGui::Checkbox("Line", &config.Freewall));
			//if (ImGui::Checkbox("box", &config.Freewall));
			if (ImGui::Checkbox("radar", &config.Radar));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Misk"))
		{
			if (ImGui::Checkbox("GhostMod", &config.GhostMod));
			if (ImGui::Checkbox("Phonk_Off", &config.Music));
				
			if (ImGui::Checkbox("Kill_All", &config.KillAll));
			if (ImGui::Checkbox("FastDoor", &config.FastDoor));

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}
