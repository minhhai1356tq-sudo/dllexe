#include "ui.hpp"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include <src/Overlay/Overlay.hpp>
#include <src/Globals.hpp>
#include <src/Fonts/FontInter.hpp>
#include <imgui/imgui_internal.h>
#include <src/Fonts/FontAwesome.hpp>
#include <src/Fonts/Fonts.hpp>
#include <thread>
#include <Auth/auth.hpp>
#define CURL_STATICLIB
#include "Auth/Curl/curl.h"
#include "src/Fonts/Fonts.hpp"
#include <FontAwesome6.hpp>
#include "Logo.hpp"
#include <lmcons.h>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <d3d11.h>
#include <wincodec.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "windowscodecs.lib")

static inline ImVec2 Size = ImVec2(640, 440);
int CurrentTab = 2;

char Username[255] = "";
char Password[255] = "";
char Key[255] = "";
char Gmail[255] = "";

ImFont* InterBlack = nullptr;
ImFont* InterBold = nullptr;
ImFont* InterBold12 = nullptr;
ImFont* InterExtraBold = nullptr;
ImFont* InterExtraLight = nullptr;
ImFont* InterLight = nullptr;
ImFont* InterMedium = nullptr;
ImFont* InterRegular = nullptr;
ImFont* InterSemiBold = nullptr;
ImFont* InterThin = nullptr;

ImFont* FontAwesomeRegular = nullptr;
ImFont* FontAwesomeSolid = nullptr;
ImFont* FontAwesomeSolid14 = nullptr;
ImFont* FontAwesomeBrands = nullptr;

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
HWND hwnd = nullptr;
ID3D11ShaderResourceView* Logo = nullptr;

std::string GetUsername()
{
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    if (GetUserNameA(username, &username_len))
        return std::string(username);
    return "Unknown";
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace FWork {
    void Interface::Initialize(HWND Window, HWND TargetWindow, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) {
        hWindow = Window;
        IDevice = Device;
        g_pd3dDevice = Device;
        g_pd3dDeviceContext = DeviceContext;

        ImGui::CreateContext();
        ImGui_ImplWin32_Init(hWindow);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

        InterBlack = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(InterBlack_compressed_data, InterBlack_compressed_size, 14);
        InterBold = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(InterBold_compressed_data, InterBold_compressed_size, 16);
        InterBold12 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(InterBold_compressed_data, InterBold_compressed_size, 12);
        InterExtraBold = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(InterExtraBold_compressed_data, InterExtraBold_compressed_size, 14);
        InterExtraLight = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(InterExtraLight_compressed_data, InterExtraLight_compressed_size, 14);
        InterLight = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(InterLight_compressed_data, InterLight_compressed_size, 16);
        InterMedium = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(InterMedium_compressed_data, InterMedium_compressed_size, 16);
        InterRegular = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(InterRegular_compressed_data, InterRegular_compressed_size, 16);
        InterSemiBold = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(InterSemiBold_compressed_data, InterSemiBold_compressed_size, 16);
        InterThin = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(InterThin_compressed_data, InterThin_compressed_size, 14);

        ImFontConfig FontAwesomeConfig;
        static const ImWchar FontAwesomeRanges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        static const ImWchar FontAwesomeRangesBrands[] = { ICON_MIN_FAB, ICON_MAX_FAB, 0 };

        FontAwesomeRegular = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(FontAwesome6Regular_compressed_data, FontAwesome6Regular_compressed_size, 17.f, &FontAwesomeConfig, FontAwesomeRanges);
        FontAwesomeSolid = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(FontAwesome6Solid_compressed_data, FontAwesome6Solid_compressed_size, 17.f, &FontAwesomeConfig, FontAwesomeRanges);
        FontAwesomeSolid14 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(FontAwesome6Solid_compressed_data, FontAwesome6Solid_compressed_size, 14.f, &FontAwesomeConfig, FontAwesomeRanges);
        FontAwesomeBrands = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(FontAwesome6Brands_compressed_data, FontAwesome6Brands_compressed_size, 17.f, &FontAwesomeConfig, FontAwesomeRangesBrands);

        Fonts::Initialize(IDevice);
        // Load logo bằng WIC
        IWICImagingFactory* wicFactory = nullptr;
        CoInitializeEx(NULL, COINIT_MULTITHREADED);
        CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
        if (wicFactory) {
            IWICStream* stream = nullptr;
            wicFactory->CreateStream(&stream);
            if (stream) {
                stream->InitializeFromMemory((BYTE*)LogoBytes, sizeof(LogoBytes));
                IWICBitmapDecoder* decoder = nullptr;
                if (SUCCEEDED(wicFactory->CreateDecoderFromStream(stream, NULL, WICDecodeMetadataCacheOnDemand, &decoder))) {
                    IWICBitmapFrameDecode* frame = nullptr;
                    if (SUCCEEDED(decoder->GetFrame(0, &frame))) {
                        IWICFormatConverter* converter = nullptr;
                        wicFactory->CreateFormatConverter(&converter);
                        if (converter) {
                            converter->Initialize(frame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
                            UINT w, h;
                            converter->GetSize(&w, &h);
                            std::vector<BYTE> pixels(w * h * 4);
                            converter->CopyPixels(NULL, w * 4, (UINT)pixels.size(), pixels.data());
                            D3D11_TEXTURE2D_DESC desc = {};
                            desc.Width = w; desc.Height = h; desc.MipLevels = 1; desc.ArraySize = 1;
                            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; desc.SampleDesc.Count = 1;
                            desc.Usage = D3D11_USAGE_DEFAULT; desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
                            D3D11_SUBRESOURCE_DATA sub = {}; sub.pSysMem = pixels.data(); sub.SysMemPitch = w * 4;
                            ID3D11Texture2D* texture = nullptr;
                            if (SUCCEEDED(g_pd3dDevice->CreateTexture2D(&desc, &sub, &texture))) {
                                D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
                                srvDesc.Format = desc.Format; srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                                srvDesc.Texture2D.MipLevels = 1;
                                g_pd3dDevice->CreateShaderResourceView(texture, &srvDesc, &Logo);
                                texture->Release();
                            }
                            converter->Release();
                        }
                        frame->Release();
                    }
                    decoder->Release();
                }
                stream->Release();
            }
            wicFactory->Release();
        }
        CoUninitialize();
        InitializeMenu();
    }

    void Interface::InitializeMenu() {
        bIsMenuOpen = true;
        SetWindowLong(hWindow, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT);
        SetForegroundWindow(hWindow);
        SetWindowPos(hWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
    }

    void Interface::UpdateStyle() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGuiStyle* Style = &ImGui::GetStyle();
        Style->AntiAliasedLines = true;
        Style->AntiAliasedLinesUseTex = true;
        Style->AntiAliasedFill = true;
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
        Style->WindowRounding = 7;
        Style->WindowBorderSize = 1;
        Style->WindowPadding = ImVec2(0, 0);
        Style->WindowShadowSize = 0;
        Style->ScrollbarSize = 8;
        Style->Colors[ImGuiCol_Separator] = ImColor(0, 0, 0, 0);
        Style->Colors[ImGuiCol_SeparatorActive] = ImColor(0, 0, 0, 0);
        Style->Colors[ImGuiCol_SeparatorHovered] = ImColor(0, 0, 0, 0);
        Style->Colors[ImGuiCol_ResizeGrip] = ImColor(0, 0, 0, 0);
        Style->Colors[ImGuiCol_ResizeGripActive] = ImColor(0, 0, 0, 0);
        Style->Colors[ImGuiCol_ResizeGripHovered] = ImColor(0, 0, 0, 0);
        Style->Colors[ImGuiCol_WindowBg] = ImColor(12, 12, 12);
        Style->Colors[ImGuiCol_ChildBg] = ImColor(0, 0, 0, 0);
        Style->Colors[ImGuiCol_Border] = ImColor(23, 24, 25);
        Style->Colors[ImGuiCol_Text] = ImColor(1.f, 1.f, 1.f, 0.8f);
        Style->Colors[ImGuiCol_TextSelectedBg] = ImColor(47, 108, 153, 100);
    }

    void Interface::RenderGui() {
        if (!bIsMenuOpen) return;

        ImGui::SetNextWindowSize(ImVec2(640, 440));
        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        {
            ImDrawList* DrawList = ImGui::GetWindowDrawList();
            ImVec2 Pos = ImGui::GetWindowPos();
            ImVec2 Size = ImGui::GetWindowSize();

            // Sidebar
            DrawList->AddRectFilled(Pos, Pos + ImVec2(Size.x / 3.5f, Size.y), ImColor(16, 16, 16), ImGui::GetStyle().WindowRounding, ImDrawFlags_RoundCornersLeft);
            DrawList->AddLine(Pos + ImVec2(Size.x / 3.5f, 0), Pos + ImVec2(Size.x / 3.5f, Size.y), ImGui::GetColorU32(ImGuiCol_Border));

            ImGui::PushFont(InterMedium);
            std::string username = GetUsername();
            DrawList->AddText(Pos + ImVec2(56, Size.y - 30), ImColor(255, 255, 255), username.c_str());
            ImGui::PopFont();

            ImGui::PushFont(InterRegular);
            DrawList->AddText(Pos + ImVec2(56, Size.y - 15), ImColor(112, 112, 121), "Expiry: Lifetime");
            ImGui::PopFont();

            float scale = g_Globals.Visuals.LogoScale;
            ImVec2 logoSize(100.0f * scale, 100.0f * scale);
            ImVec2 topLeft(Pos.x + (Size.x / 3.5f - logoSize.x) * 0.5f, Pos.y + 30.0f);
            DrawList->AddImage(Logo, topLeft, topLeft + logoSize);

            DrawList->AddLine(Pos + ImVec2(0, Size.y - 60), Pos + ImVec2(Size.x / 3.5f, Size.y - 60), ImGui::GetColorU32(ImGuiCol_Border));

            ImGui::BeginChild("LeftChild", ImVec2(Size.x / 3.5f, Size.y));
            {
                ImGui::SetCursorPos(ImVec2(20, Size.y / 7.0f + 10));
                ImGui::BeginGroup();
                {
                    ImGui::PushFont(InterBold);
                    ImGui::TextColored(ImColor(1.f, 1.f, 1.f, 0.2f), "Combat");
                    ImGui::PopFont();
                    if (ImGui::Selectable("Aimbot", CurrentTab == 2)) CurrentTab = 2;

                    ImGui::PushFont(InterBold);
                    ImGui::TextColored(ImColor(1.f, 1.f, 1.f, 0.2f), "Visuals");
                    ImGui::PopFont();
                    if (ImGui::Selectable("World", CurrentTab == 3)) CurrentTab = 3;

                    ImGui::PushFont(InterBold);
                    ImGui::TextColored(ImColor(1.f, 1.f, 1.f, 0.2f), "Others");
                    ImGui::PopFont();
                    if (ImGui::Selectable("Misc", CurrentTab == 4)) CurrentTab = 4;
                    if (ImGui::Selectable("Configs", CurrentTab == 5)) CurrentTab = 5;
                }
                ImGui::EndGroup();
            }
            ImGui::EndChild();

            ImGui::SetCursorPos(ImVec2(Size.x / 3.5f, 0));
            ImGui::BeginChild("MainChild", ImVec2(Size.x - (Size.x / 3.5f), Size.y), ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            {
                if (CurrentTab == 2) {
                    ImGui::BeginTabBar("AimbotTab");
                    if (ImGui::BeginTabItem("Combat")) {
                        ImGui::Checkbox("Aimbot Legit", &g_Globals.AimBot.Enabled);
                        ImGui::Checkbox("Aimbot Rage", &g_Globals.AimBot.AimShoulder);
                        ImGui::InputInt("KeyBind", &g_Globals.AimBot.AimbotBind);
                        ImGui::Checkbox("Ignore Knocked", &g_Globals.AimBot.IgnoreKnocked);
                        ImGui::Checkbox("Ignore Bots", &g_Globals.AimBot.IgnoreBots);
                        ImGui::Checkbox("Enable Fov", &g_Globals.Misc.ShowAimbotFov);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Settings")) {
                        ImGui::SliderInt("Field of View", &g_Globals.AimBot.DistanceAim, 0, 360, "%d");
                        ImGui::SliderInt("Max Distance", &g_Globals.AimBot.Fov, 0, 200, "%dm");
                        ImGui::SliderInt("Chest Rate ( ms )", &g_Globals.AimBot.UpdateInterval, 0, 500, "%dms");
                        ImGui::ColorEdit4("Fov Filled Color", g_Globals.AimBot.Fillcolor);
                        ImGui::ColorEdit4("Fov Color", g_Globals.Misc.AimbotFovColor);
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                else if (CurrentTab == 3) {
                    ImGui::BeginTabBar("WorldTab");
                    if (ImGui::BeginTabItem("World")) {
                        ImGui::Checkbox("Enable Esp", &g_Globals.Visuals.Enable);
                        ImGui::Checkbox("Enable Observe Esp", &g_Globals.Visuals.Observe);
                        ImGui::Checkbox("Enable Watermark", &g_Globals.Visuals.Watermark);
                        ImGui::Checkbox("Enable Enemies", &g_Globals.Visuals.Enemy);
                        ImGui::Checkbox("Esp Weapons Text", &g_Globals.Visuals.ESPWeapon);
                        ImGui::Checkbox("Esp Weapons Icon", &g_Globals.Visuals.ESPWeaponIcon);
                        ImGui::Checkbox("Esp Lines", &g_Globals.Visuals.Lines);
                        ImGui::Checkbox("Enable Health Bar", &g_Globals.Visuals.HealthBar);
                        ImGui::Checkbox("Health Text", &g_Globals.Visuals.ESPHealthTEXT);
                        ImGui::Checkbox("Enable Box", &g_Globals.Visuals.Box);
                        ImGui::Checkbox("Enable FilledBox", &g_Globals.Visuals.FilledBox);
                        ImGui::Checkbox("Enable Name", &g_Globals.Visuals.Name);
                        ImGui::Checkbox("Enable Distance", &g_Globals.Visuals.Distance);
                        ImGui::Checkbox("Enable Skeleton", &g_Globals.Visuals.Skeleton);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Colors")) {
                        ImGui::ColorEdit4("Name Color", g_Globals.Visuals.NameColor);
                        ImGui::ColorEdit4("Distance Color", g_Globals.Visuals.DistColor);
                        ImGui::ColorEdit4("Skeleton Color", g_Globals.Visuals.SkeletonColor);
                        ImGui::ColorEdit4("Box Color", g_Globals.Visuals.BoxColor);
                        ImGui::ColorEdit4("Lines Color", g_Globals.Visuals.LinesColor);
                        ImGui::ColorEdit4("Health Color", g_Globals.Visuals.texthColor);
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                else if (CurrentTab == 4) {
                    ImGui::Checkbox("Stream Mode", &g_Globals.General.Capture);
                    ImGui::SliderInt("Delay", &g_Globals.General.Delay, 0, 100, "%dms");
                    ImGui::SliderFloat("Logo Scale", &g_Globals.Visuals.LogoScale, 0.10f, 1.00f, "%.2f");
                    ImGui::InputInt("Keybind Menu", &g_Globals.General.MenuKey);
                    if (ImGui::Button("Unload Menu")) g_Globals.General.ShutDown = true;
                }
                else if (CurrentTab == 5) {
                    ImGui::Text("In Maintenance...");
                }
            }
            ImGui::EndChild();
            DrawList->AddRect(Pos, Pos + Size, ImGui::GetColorU32(ImGuiCol_Border), ImGui::GetStyle().WindowRounding);
        }
        ImGui::End();
    }

    void Interface::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (uMsg == WM_SIZE) {
            if (wParam != SIZE_MINIMIZED) {
                ResizeWidht = (UINT)LOWORD(lParam);
                ResizeHeight = (UINT)HIWORD(lParam);
            }
        }
        if (bIsMenuOpen) {
            ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        }
    }

    void Interface::HandleMenuKey() {
        static bool MenuKeyDown = false;
        if (GetAsyncKeyState(g_Globals.General.MenuKey) & 0x8000) {
            if (!MenuKeyDown) {
                MenuKeyDown = true;
                bIsMenuOpen = !bIsMenuOpen;
                if (bIsMenuOpen) {
                    SetWindowLong(hWindow, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE);
                    SetForegroundWindow(hWindow);
                } else {
                    SetWindowLong(hWindow, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_NOACTIVATE);
                    SetForegroundWindow(hTargetWindow);
                }
                SetWindowPos(hWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
            }
        } else {
            MenuKeyDown = false;
        }
    }

    void Interface::ShutDown() {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        Overlay::ShutDown();
    }
}
