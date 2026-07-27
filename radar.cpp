#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include "globals.h"
#include "math.h"
#include <imgui.h>
#include <vector>


struct EnemyInfo {
    Vector3 position;
    float distance;
    bool valid;
};

struct EnemyTrack {
    Vector3 lastPos{};
    int stillFrames = 0;
    bool active = false;
};

EnemyTrack tracks[64]{};
std::vector<EnemyInfo> enemies;  



void UpdateEnemies()
{
    enemies.clear();


    float maxDist = 40.0f;
    for (int i = 0; ; i++)
    {
        Vector3 pos = RPM<Vector3>(A::EntPositionlist + 0x240 + i * 0x2C0);

        if (pos == Vector3{ 0.0f, 0.0f, 0.0f })
            break;


        float moveDist = sqrtf(
            (pos.x - tracks[i].lastPos.x) * (pos.x - tracks[i].lastPos.x) +
            (pos.z - tracks[i].lastPos.z) * (pos.z - tracks[i].lastPos.z)
        );

        if (pos != tracks[i].lastPos) {
            tracks[i].stillFrames = 0;
            tracks[i].active = true;
        }
        else {
            tracks[i].stillFrames++;
        }

        tracks[i].lastPos = pos;

        if (tracks[i].stillFrames > 500) { 
            tracks[i].active = false;
        }

        if (!tracks[i].active)
            continue;
  

        Vector3 dif = pos - A::PlayerPosition;
        float dist = sqrtf(dif.x * dif.x + dif.z * dif.z);

        if (dist > maxDist)
            continue;

        EnemyInfo enemy;
        enemy.position = pos;
        enemy.distance = dist;
        enemy.valid = true;

        enemies.push_back(enemy);
    }
}

void Radar()
{

    ImGui::Begin("Radar");
    ImDrawList* Drawlist = ImGui::GetWindowDrawList();

    Vector2 center = {
        ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x * 0.5f,
        ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMax().y * 0.5f
    };

    float c = -cosf(A::rotation);
    float s = sinf(A::rotation);

    for (const auto& enemy : enemies)
    {
        Vector3 dif = enemy.position - A::PlayerPosition;

        float y = dif.x * s - dif.z * c;
        float x = dif.x * c + dif.z * s;

        Drawlist->AddCircleFilled(center + Vector2(x, y) * 2.0f, 3.0f, IM_COL32(255, 0, 0, 255));
    }

    Drawlist->AddCircleFilled(center, 3.0f, IM_COL32(0, 255, 0, 255));

    ImGui::End();
}


void AIM(){

    bool hasTarget = false;
    Vector3 targetpos{};

    for (const auto& enemy : enemies) {
        targetpos = enemy.position;
        hasTarget = true;
        break;
    }

    if ((GetAsyncKeyState('C') & 0x8000) && (hasTarget))
    {
        Vector3 abspos = targetpos - A::PlayerPosition;

        float yaw = atan2f(abspos.x, -abspos.z) * 0.5f;

        WPM<float>(A::cosyewAddress, cosf(yaw));
        WPM<float>(A::sinyewAddress, sinf(yaw));
    }
}
