#include "Visual.hpp"
#include <src/Globals.hpp>
#include <EspLines/Math/WordToScreen.hpp>
#include <EspLines/Math/Vector/Vector2.hpp>
#include <Windows.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <src/Fonts/Fonts.hpp>
#include "EspLines/Math/Vector/Vector3.hpp"
#include "NameGun.h"
#include <EspLines/Memory/Memory.hpp>
#include <EspLines/Offsets.hpp>

static std::unordered_map<int, float> g_animatedHealth;

void DrawVerticalHealthBar(int entityId, short CurrentHealth, short MaxHealth, ImVec2 Position, float TotalHeight) {
    if (MaxHealth <= 0) return;
    ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
    const float BarWidth = 6.0f, Rounding = 3.0f, AnimationSpeed = 15.0f, BorderThickness = 1.0f;
    float HealthPercentage = (std::clamp)((float)CurrentHealth / MaxHealth, 0.0f, 1.0f);
    float& AnimatedHealth = g_animatedHealth[entityId];
    float lerpFactor = (std::min)(1.0f, ImGui::GetIO().DeltaTime * AnimationSpeed);
    AnimatedHealth = ImLerp(AnimatedHealth, HealthPercentage, lerpFactor);
    ImVec2 innerPos(Position.x + BorderThickness, Position.y + BorderThickness);
    float innerWidth = BarWidth - 2 * BorderThickness;
    float innerHeight = TotalHeight - 2 * BorderThickness;
    float FilledBarHeight = innerHeight * AnimatedHealth;
    DrawList->AddRectFilled(Position, ImVec2(Position.x + BarWidth, Position.y + TotalHeight), IM_COL32(40,40,40,130), Rounding);
    ImU32 HealthBarColor = (HealthPercentage > 0.6f) ? IM_COL32(50,255,50,255) : (HealthPercentage > 0.3f) ? IM_COL32(255,255,50,255) : IM_COL32(255,50,50,255);
    if (FilledBarHeight > 0.0f) {
        DrawList->AddRectFilled(ImVec2(innerPos.x, innerPos.y + innerHeight - FilledBarHeight), ImVec2(innerPos.x + innerWidth, innerPos.y + innerHeight), HealthBarColor, Rounding - BorderThickness, ImDrawFlags_RoundCornersBottom);
    }
    DrawList->AddRect(Position, ImVec2(Position.x + BarWidth, Position.y + TotalHeight), IM_COL32(60,60,60,255), Rounding);
}

void DrawHorizontalHealthBar(int entityId, short CurrentHealth, short MaxHealth, ImVec2 Position, float TotalWidth) {
    if (MaxHealth <= 0) return;
    ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
    const float BarHeight = 6.0f, Rounding = 3.0f, AnimationSpeed = 15.0f, BorderThickness = 1.0f;
    float HealthPercentage = (std::clamp)((float)CurrentHealth / MaxHealth, 0.0f, 1.0f);
    float& AnimatedHealth = g_animatedHealth[entityId];
    float lerpFactor = (std::min)(1.0f, ImGui::GetIO().DeltaTime * AnimationSpeed);
    AnimatedHealth = ImLerp(AnimatedHealth, HealthPercentage, lerpFactor);
    ImVec2 innerPos(Position.x + BorderThickness, Position.y + BorderThickness);
    float innerWidth = TotalWidth - 2 * BorderThickness;
    float innerHeight = BarHeight - 2 * BorderThickness;
    float FilledBarWidth = innerWidth * AnimatedHealth;
    DrawList->AddRectFilled(Position, ImVec2(Position.x + TotalWidth, Position.y + BarHeight), IM_COL32(40,40,40,130), Rounding);
    ImU32 HealthBarColor = (HealthPercentage > 0.6f) ? IM_COL32(50,255,50,255) : (HealthPercentage > 0.3f) ? IM_COL32(255,255,50,255) : IM_COL32(255,50,50,255);
    if (FilledBarWidth > 0.0f) {
        DrawList->AddRectFilled(ImVec2(innerPos.x, innerPos.y), ImVec2(innerPos.x + FilledBarWidth, innerPos.y + innerHeight), HealthBarColor, Rounding - BorderThickness, ImDrawFlags_RoundCornersRight);
    }
    DrawList->AddRect(Position, ImVec2(Position.x + TotalWidth, Position.y + BarHeight), IM_COL32(60,60,60,255), Rounding);
}

void DrawCorneredBox(float x, float y, float w, float h, ImColor color, float thickness) {
    auto drawList = ImGui::GetBackgroundDrawList();
    float lineW = w / 3.0f, lineH = h / 3.0f;
    auto AddH = [&](float sx, float yp, float len) { drawList->AddLine(ImVec2(sx, yp), ImVec2(sx+len, yp), color, g_Globals.Visuals.Thickness); };
    auto AddV = [&](float xp, float ys, float len) { drawList->AddLine(ImVec2(xp, ys), ImVec2(xp, ys+len), color, g_Globals.Visuals.Thickness); };
    AddV(x, y - thickness/2, lineH); AddH(x - thickness/2, y, lineW);
    AddH(x + w - lineW, y, lineW);  AddV(x + w, y - thickness/2, lineH);
    AddV(x, y + h - lineH, lineH);  AddH(x - thickness/2, y + h, lineW);
    AddH(x + w - lineW, y + h, lineW); AddV(x + w, y + h - lineH, lineH);
}

void DrawFullBox(float x, float y, float w, float h, ImColor color, float thickness) {
    auto drawList = ImGui::GetBackgroundDrawList();
    drawList->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0.0f, ImDrawFlags_None, g_Globals.Visuals.Thickness);
}

void ESP::Players() {
    if (!g_Globals.EspConfig.Matrix) return;

    for (auto& [entityID, player] : g_Globals.EspConfig.Entities) {
        if (player.IsDead || !player.IsKnown) continue;
        if (player.Distance > g_Globals.Visuals.DistanceEsp) continue;

        auto IsOffScreen = [](const ImVec2& p) { return p.x < 1 || p.y < 1; };
        const std::vector<ImVec2> Bones = {
            W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.Head, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height),
            W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.Neck, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height),
            W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.LeftShoulder, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height),
            W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.RightShoulder, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height),
            W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.LeftElbow, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height),
            W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.RightElbow, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height),
            W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.LeftWrist, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height),
            W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.RightWrist, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height),
            W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.Hip, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height),
            W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.LeftAnkle, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height),
            W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.RightAnkle, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height),
            W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.Root, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height)
        };
        if (std::any_of(Bones.begin(), Bones.end(), IsOffScreen)) continue;

        const ImVec2& headPos = Bones[0];
        const ImVec2& neckPos = Bones[1];
        const ImVec2& leftShoulderPos = Bones[2];
        const ImVec2& rightShoulderPos = Bones[3];
        const ImVec2& leftElbowPos = Bones[4];
        const ImVec2& rightElbowPos = Bones[5];
        const ImVec2& leftWristPos = Bones[6];
        const ImVec2& rightWristPos = Bones[7];
        const ImVec2& hipPos = Bones[8];
        const ImVec2& leftAnklePos = Bones[9];
        const ImVec2& rightAnklePos = Bones[10];
        const ImVec2& rootPos = Bones[11];

        Vector3 ankleMidPoint = (player.LeftAnkle + player.RightAnkle) * 0.5f;
        Vector3 hipDirection = (player.LeftAnkle - player.RightAnkle).Normalized(true);
        Vector3 leftHipDirection = hipDirection;
        Vector3 rightHipDirection = -hipDirection;
        float ankleDistance = (player.LeftAnkle - player.RightAnkle).Magnitude(true);
        float baseHipWidth = ankleDistance * g_Globals.Visuals.HipWidthScale;
        Vector3 verticalPositionLeft = Vector3::Lerp(player.LeftAnkle, player.Hip, g_Globals.Visuals.LeftHipHeightOffset);
        Vector3 verticalPositionRight = Vector3::Lerp(player.RightAnkle, player.Hip, g_Globals.Visuals.RightHipHeightOffset);
        Vector3 leftHip = verticalPositionLeft + (leftHipDirection * (baseHipWidth * g_Globals.Visuals.HipWidthOffset));
        Vector3 rightHip = verticalPositionRight + (rightHipDirection * (baseHipWidth * g_Globals.Visuals.HipWidthOffset));
        ImVec2 leftHipPosition = W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, leftHip, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height);
        ImVec2 rightHipPosition = W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, rightHip, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height);

        float boxHeight, boxWidth;
        if (headPos.x != 0 && headPos.y != 0 && rootPos.x != 0 && rootPos.y != 0) {
            boxHeight = fabsf(headPos.y - rootPos.y);
            boxWidth = boxHeight * 0.65f;
        } else {
            boxHeight = 50.0f; boxWidth = 30.0f;
        }

        if (g_Globals.Visuals.Lines) {
            ImColor snapLineColor = ImColor(g_Globals.Visuals.LinesColor[0], g_Globals.Visuals.LinesColor[1], g_Globals.Visuals.LinesColor[2], g_Globals.Visuals.LinesColor[3]);
            ImColor KnockedColor = ImColor(g_Globals.Visuals.KnockedColor[0], g_Globals.Visuals.KnockedColor[1], g_Globals.Visuals.KnockedColor[2], g_Globals.Visuals.KnockedColor[3]);
            ImColor lineColor = player.IsKnocked ? KnockedColor : snapLineColor;

            Vector2 ScreenTop(g_Globals.EspConfig.Width / 2, 0);
            Vector2 ScreenBody(g_Globals.EspConfig.Width / 2, g_Globals.EspConfig.Height);
            auto AddGlowLine = [&](float x1, float y1, float x2, float y2) {
                ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), lineColor, g_Globals.Visuals.Thickness);
            };
            switch (g_Globals.Visuals.EspLines) {
            case 1: AddGlowLine(headPos.x, headPos.y, ScreenTop.X, ScreenTop.Y); break;
            case 2: AddGlowLine(ScreenBody.X, ScreenBody.Y, headPos.x, headPos.y); break;
            }
        }

        if (g_Globals.Visuals.Skeleton) {
            ImColor SkeletonColor = ImColor(g_Globals.Visuals.SkeletonColor[0], g_Globals.Visuals.SkeletonColor[1], g_Globals.Visuals.SkeletonColor[2], g_Globals.Visuals.SkeletonColor[3]);
            ImColor IsKnockedColor = player.IsKnocked ? ImColor(1.f,0.f,0.f,1.f) : SkeletonColor;
            float headRadius = (std::clamp)(100.0f / player.Distance, 0.5f, 1.5f) * 2.0f;
            auto dl = ImGui::GetBackgroundDrawList();
            auto Bone = [&](const ImVec2& a, const ImVec2& b) { dl->AddLine(a, b, IsKnockedColor, g_Globals.Visuals.Thickness); };
            dl->AddCircle(headPos, headRadius, IsKnockedColor, 0, g_Globals.Visuals.Thickness);
            Bone(headPos, neckPos);
            Bone(neckPos, leftShoulderPos); Bone(neckPos, rightShoulderPos);
            Bone(leftShoulderPos, leftElbowPos); Bone(rightShoulderPos, rightElbowPos);
            Bone(leftElbowPos, leftWristPos); Bone(rightElbowPos, rightWristPos);
            Bone(neckPos, hipPos);
            Bone(hipPos, leftHipPosition); Bone(leftHipPosition, leftAnklePos);
            Bone(hipPos, rightHipPosition); Bone(rightHipPosition, rightAnklePos);
        }

        if (g_Globals.Visuals.ESPHealthTEXT) {
            auto drawList = ImGui::GetBackgroundDrawList();
            float fontScale = g_Globals.Visuals.TextSize / 15.0f;
            ImGui::PushFont(FWork::Fonts::InterRegular);
            const char* healthLabel = "Health:";
            ImVec2 labelSize = ImVec2(ImGui::CalcTextSize(healthLabel).x * fontScale, ImGui::CalcTextSize(healthLabel).y * fontScale);
            char healthValue[16];
            snprintf(healthValue, sizeof(healthValue), "%d", player.Health);
            ImVec2 valueSize = ImVec2(ImGui::CalcTextSize(healthValue).x * fontScale, ImGui::CalcTextSize(healthValue).y * fontScale);
            float totalWidth = labelSize.x + valueSize.x;
            float healthTextOffset = (g_Globals.Visuals.HealthBar && g_Globals.Visuals.players_healthbar == 4) ? 13.5f : 8.0f;
            ImVec2 combinedPosition(rootPos.x - totalWidth/2, rootPos.y + healthTextOffset - 5);
            ImColor shadowColor = ImColor(0.0f, 0.0f, 0.0f, 0.3f);
            ImColor whiteColor = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
            drawList->AddText(ImVec2(combinedPosition.x + 1, combinedPosition.y + 1), shadowColor, healthLabel);
            drawList->AddText(combinedPosition, whiteColor, healthLabel);

            float healthPercentage = (std::clamp)((float)player.Health / 200.0f, 0.0f, 1.0f);
            ImVec4 greenColor(0,1,0,1), yellowColor(1,1,0,1), redColor(1,0,0,1);
            ImVec4 healthBarColor;
            if (healthPercentage > 0.5f) healthBarColor = ImLerp(greenColor, yellowColor, (1.0f - healthPercentage) * 2.0f);
            else healthBarColor = ImLerp(yellowColor, redColor, (0.5f - healthPercentage) * 2.0f);
            ImColor health = player.IsKnocked ? ImColor(1.0f, 0.0f, 0.0f, 1.0f) : ImColor(healthBarColor);
            ImVec2 valuePosition(combinedPosition.x + labelSize.x, combinedPosition.y);
            ImGui::PushFont(FWork::Fonts::InterSemiBold);
            drawList->AddText(ImVec2(valuePosition.x + 1, valuePosition.y + 1), shadowColor, healthValue);
            drawList->AddText(valuePosition, health, healthValue);
            ImGui::PopFont();
            ImGui::PopFont();
        }

        if (g_Globals.Visuals.ESPWeapon) {
            auto drawList = ImGui::GetBackgroundDrawList();
            Namegun::Init();
            std::string TextAndIcon;
            bool isIcon = false;
            if (g_Globals.Visuals.ESPWeaponIcon && Namegun::HasIcon(player.WeaponID)) {
                TextAndIcon = Namegun::GetGunIcon(player.WeaponID);
                isIcon = true;
            } else {
                TextAndIcon = Namegun::GetGunName(player.WeaponID);
            }
            ImVec2 textSize = ImGui::CalcTextSize(TextAndIcon.c_str());
            float weaponOffset = (g_Globals.Visuals.HealthBar && g_Globals.Visuals.players_healthbar == 4) ? 15.0f : 20.0f;
            if (g_Globals.Visuals.ESPHealthTEXT) weaponOffset += 10.0f;
            if (isIcon && g_Globals.Visuals.ESPWeaponIcon) weaponOffset += 13.0f;
            float xOffset = isIcon ? -13.0f : -3.0f;
            ImVec2 textPos(rootPos.x - textSize.x*0.5f + xOffset, rootPos.y + weaponOffset);
            ImColor WeaponColor = ImColor(g_Globals.Visuals.ESPWeaponColor[0], g_Globals.Visuals.ESPWeaponColor[1], g_Globals.Visuals.ESPWeaponColor[2], g_Globals.Visuals.ESPWeaponColor[3]);
            ImFont* BestFont = isIcon ? FWork::Fonts::IconWeapon : FWork::Fonts::InterRegular;
            ImGui::PushFont(BestFont);
            drawList->AddText(textPos, WeaponColor, TextAndIcon.c_str());
            ImGui::PopFont();
        }

        if (g_Globals.Visuals.Watermark) {
            ImGui::PushFont(FWork::Fonts::InterSemiBold);
            const char* text = "85Hz in discord";
            float fontScale = g_Globals.Visuals.TextSize / 15.0f;
            ImVec2 textSize = ImVec2(ImGui::CalcTextSize(text).x * fontScale, ImGui::CalcTextSize(text).y * fontScale);
            ImVec2 screenSize = ImGui::GetIO().DisplaySize;
            ImVec2 textPos((screenSize.x - textSize.x) * 0.5f, 80.0f);
            ImColor textColor = ImColor(g_Globals.Visuals.WatermarkColor[0], g_Globals.Visuals.WatermarkColor[1], g_Globals.Visuals.WatermarkColor[2], g_Globals.Visuals.WatermarkColor[3]);
            ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize()*fontScale, ImVec2(textPos.x+1, textPos.y+1), 0xFF000000, text);
            ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize()*fontScale, textPos, textColor, text);
            ImGui::PopFont();
        }

        if (g_Globals.Visuals.HealthBar) {
            switch (g_Globals.Visuals.players_healthbar) {
            case 1: DrawVerticalHealthBar(entityID, player.Health, 200, ImVec2(headPos.x - boxWidth/2 - 8, headPos.y), boxHeight); break;
            case 2: DrawVerticalHealthBar(entityID, player.Health, 200, ImVec2(headPos.x + boxWidth/2 + 2, headPos.y), boxHeight); break;
            case 3: DrawHorizontalHealthBar(entityID, player.Health, 200, ImVec2(headPos.x - boxWidth/2, headPos.y - 10), boxWidth); break;
            case 4: DrawHorizontalHealthBar(entityID, player.Health, 200, ImVec2(headPos.x - boxWidth/2, headPos.y + boxHeight + 2), boxWidth); break;
            }
        }

        if (g_Globals.Visuals.FilledBox) {
            float padding = 1.0f;
            ImColor fillColor = ImColor(g_Globals.Visuals.Filledboxcolor[0], g_Globals.Visuals.Filledboxcolor[1], g_Globals.Visuals.Filledboxcolor[2], g_Globals.Visuals.Filledboxcolor[3]);
            ImGui::GetBackgroundDrawList()->AddRectFilled(
                ImVec2(headPos.x - boxWidth/2 + padding, headPos.y + padding),
                ImVec2(headPos.x + boxWidth/2 - padding, headPos.y + boxHeight - padding),
                fillColor);
        }

        if (g_Globals.Visuals.Box) {
            ImColor boxColor = ImColor(g_Globals.Visuals.BoxColor[0], g_Globals.Visuals.BoxColor[1], g_Globals.Visuals.BoxColor[2], g_Globals.Visuals.BoxColor[3]);
            switch (g_Globals.Visuals.players_box) {
            case 1: DrawFullBox(headPos.x - boxWidth/2, headPos.y, boxWidth, boxHeight, boxColor, g_Globals.Visuals.Thickness); break;
            case 2: DrawCorneredBox(headPos.x - boxWidth/2, headPos.y, boxWidth, boxHeight, boxColor, g_Globals.Visuals.Thickness); break;
            }
        }

        if (g_Globals.Visuals.Name) {
            ImColor NameColor = ImColor(g_Globals.Visuals.NameColor[0], g_Globals.Visuals.NameColor[1], g_Globals.Visuals.NameColor[2], g_Globals.Visuals.NameColor[3]);
            ImColor DistColor = ImColor(g_Globals.Visuals.DistColor[0], g_Globals.Visuals.DistColor[1], g_Globals.Visuals.DistColor[2], g_Globals.Visuals.DistColor[3]);
            ImColor shadowColor = ImColor(0.0f, 0.0f, 0.0f, 0.3f);
            ImGui::PushFont(FWork::Fonts::InterSemiBold);
            if (player.Name.empty()) player.Name = "85hz in discord";
            float fontScale = g_Globals.Visuals.TextSize / 15.0f;
            std::string distanceText = g_Globals.Visuals.Distance ? " [" + std::to_string((int)std::round(player.Distance)) + "m]" : "";
            std::string combinedText = player.Name + distanceText;
            ImVec2 combinedSize = ImVec2(ImGui::CalcTextSize(combinedText.c_str()).x * fontScale, ImGui::CalcTextSize(combinedText.c_str()).y * fontScale);
            ImVec2 textSize = ImVec2(ImGui::CalcTextSize(player.Name.c_str()).x * fontScale, ImGui::CalcTextSize(player.Name.c_str()).y * fontScale);
            float nameOffset = (g_Globals.Visuals.HealthBar && g_Globals.Visuals.players_healthbar == 3) ? 25.0f : 16.0f;
            ImVec2 textPos(headPos.x - combinedSize.x/2, headPos.y - nameOffset);
            ImVec2 textPosDistance(textPos.x + textSize.x + 2.2f, textPos.y);
            auto dl = ImGui::GetBackgroundDrawList();
            dl->AddText(ImGui::GetFont(), ImGui::GetFontSize()*fontScale, ImVec2(textPos.x+1, textPos.y+1), shadowColor, player.Name.c_str());
            dl->AddText(ImGui::GetFont(), ImGui::GetFontSize()*fontScale, textPos, NameColor, player.Name.c_str());
            if (g_Globals.Visuals.Distance) {
                dl->AddText(ImGui::GetFont(), ImGui::GetFontSize()*fontScale, ImVec2(textPosDistance.x+1, textPosDistance.y+1), shadowColor, distanceText.c_str());
                dl->AddText(ImGui::GetFont(), ImGui::GetFontSize()*fontScale, textPosDistance, DistColor, distanceText.c_str());
            }
            ImGui::PopFont();
        }

        if (g_Globals.Visuals.Enemy) {
            static int enemyCount = 0;
            float fontScale = g_Globals.Visuals.TextSize / 15.0f;
            enemyCount = std::count_if(g_Globals.EspConfig.Entities.begin(), g_Globals.EspConfig.Entities.end(),
                [](const auto& pair) { return !pair.second.IsDead && pair.second.IsKnown; });
            static int _lastEnemyCount = 0;
            static std::string _cachedEnemyText;
            if (_lastEnemyCount != enemyCount) {
                _cachedEnemyText = "Enemies Detected: " + std::to_string(enemyCount);
                _lastEnemyCount = enemyCount;
            }
            ImGui::PushFont(FWork::Fonts::InterSemiBold);
            ImVec2 textSize = ImVec2(ImGui::CalcTextSize(_cachedEnemyText.c_str()).x * fontScale, ImGui::CalcTextSize(_cachedEnemyText.c_str()).y * fontScale);
            float textPosY = 70 + ImGui::CalcTextSize("85hz in discord").y + 10;
            Vector2 textPos((g_Globals.EspConfig.Width - textSize.x) / 2, textPosY);
            ImColor textColor = ImColor(g_Globals.Visuals.EnemyColor[0], g_Globals.Visuals.EnemyColor[1], g_Globals.Visuals.EnemyColor[2], g_Globals.Visuals.EnemyColor[3]);
            ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize()*fontScale, ImVec2(textPos.X+1, textPos.Y+1), 0xFF000000, _cachedEnemyText.c_str());
            ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize()*fontScale, ImVec2(textPos.X, textPos.Y), textColor, _cachedEnemyText.c_str());
            ImGui::PopFont();
        }
    }
}
