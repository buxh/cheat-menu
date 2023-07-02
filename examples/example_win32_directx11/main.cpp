#include "main.h"

static int dnd_counter = 0;

static bool checkbox[5] ;

static int slider_int = 0;

const char* themes[]{ "Head","Chest", "Legs", "Hands"};

static int combo = 0;
static int keybind = 0;

float color_edit[4];

static bool animated_background;

char input_text[64];

int main(int, char**)
{
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_POPUP, 0, 0, 1920, 1080, NULL, NULL, wc.hInstance, NULL);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.Fonts->AddFontFromMemoryTTF(&RobotoBold, sizeof RobotoBold, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());

    tab_font = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 30, NULL, io.Fonts->GetGlyphRangesCyrillic());

    icon_font = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 26, NULL, io.Fonts->GetGlyphRangesCyrillic());

    hint_font = io.Fonts->AddFontFromMemoryTTF(&RobotoRegular, sizeof RobotoRegular, 16, NULL, io.Fonts->GetGlyphRangesCyrillic());

    second_font = io.Fonts->AddFontFromMemoryTTF(&RobotoMedium, sizeof RobotoMedium, 19, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

    ImGuiStyle& s = ImGui::GetStyle();

    s.FramePadding = ImVec2(5, 3);
    s.WindowPadding = ImVec2(0, 0);
    s.FrameRounding = 5.f;
    s.WindowRounding = 4.f;
    s.WindowBorderSize = 0.f;
    s.PopupBorderSize = 0.f;
    s.WindowPadding = ImVec2(0, 0);
    s.ChildBorderSize = 10;
    s.WindowShadowSize = 0.f;
    s.PopupRounding = 4.f;
    s.ScrollbarSize = 2;

    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            LoadImages();

            ImGui::GetBackgroundDrawList()->AddImage(bg, ImVec2(0, 0), ImVec2(1920, 1080));

            ImGui::SetNextWindowSize(ImVec2(755, 655));
            ImGui::Begin("General", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
            {
                auto draw = ImGui::GetWindowDrawList();
                const auto& p = ImGui::GetWindowPos();

                if (animated_background)
                Particles();

                draw->AddLine(p + ImVec2(0, 70), p + ImVec2(755, 70), ImColor(16, 16, 16, 255), 3.f);
                draw->AddRectFilled(p, p + ImVec2(80, 655), ImColor(12, 12, 12, 255), 4.f, ImDrawFlags_RoundCornersLeft);
                draw->AddLine(p + ImVec2(80, 0), p + ImVec2(80, 655), ImColor(16, 16, 16, 255), 3.f);

                draw->AddLine(p + ImVec2(10, 70), p + ImVec2(70, 70), ImColor(24, 24, 24, 255), 3.f);

                ImGui::SetCursorPos(ImVec2(0, 80));
                ImGui::BeginChild("Tabs");
                ImGui::Tab("1", &iTabs, 0);
                ImGui::Tab("8", &iTabs, 1);
                ImGui::Tab("3", &iTabs, 2);
                ImGui::Tab("2", &iTabs, 3);
                ImGui::Tab("7", &iTabs, 4);
                ImGui::Tab("4", &iTabs, 5);
                ImGui::EndChild();

                pos_offset = ImLerp(pos_offset, size_change ? 910.f : 75.f, ImGui::GetIO().DeltaTime * 9.f);

                if (pos_offset > 900 && size_change)
                    size_change = false;

                if (iTabs == 0)
                {
                    ImGui::SetCursorPos(ImVec2(110, 0));
                    ImGui::BeginChild("Sub");
                    ImGui::SubTab("AimBot", &iSubTabs, 0); ImGui::SameLine();
                    ImGui::SubTab("TriggerBot", &iSubTabs, 1); ImGui::SameLine();
                    ImGui::SubTab("Anti aim", &iSubTabs, 2);
                    ImGui::EndChild(); 

                    ImGui::SetCursorPos(ImVec2(110, pos_offset));
                    ImGui::BeginChild("main");
                    ImGui::BindBox("Enable Legitbot", &checkbox[3], "Enables legitbot aimbot", &keybind);
                    ImGui::Checkbox("Enable Ragebot", &checkbox[0], "Enables ragebot aimbot");
                    ImGui::ColorBox("Draw Bullet Trace", &checkbox[1], "Adds visual effects to bullet", color_edit);
                    ImGui::Checkbox("Draw FOV", &draw_grind, "Shows the aimbot FOV on screen");
                    ImGui::SliderInt("Field of view", &slider_int, 0, 300, "%d", 0, "The radius in which the aimbot will work");
                    ImGui::Combo("Bones selection", &combo, themes, IM_ARRAYSIZE(themes), 6, "Choosing hitbox for aimbot");

                    static bool multi[5];
                    const char* multicombo_items[] = { "Selected 1", "Selected 2", "Selected 3", "Selected 4", "Selected 5" };
                    ImGui::MultiCombo("MultiCombo", multi, multicombo_items, IM_ARRAYSIZE(multicombo_items), "This multi_combo for you :)");

                    ImGui::InputTextEx("InputText", NULL, input_text, 64, ImVec2(605, 35), 0);

                    ImGui::EndChild();
                }
                if (iTabs == 1)
                {
                    ImGui::SetCursorPos(ImVec2(110, pos_offset));
                    ImGui::BeginChild("Checkboxes");
                    static bool checboxes[40];
                    for (int i = 0; i < 40; i++)
                    {
                        ImGui::Checkbox(std::to_string(i).c_str(), &checboxes[i], "Testing checkbox");
                    }
                    ImGui::EndChild();
                }
                if (iTabs == 2)
                {
                    ImGui::SetCursorPos(ImVec2(110, pos_offset));
                    ImGui::BeginChild("Sliders");
                    static int sliders[40];
                    for (int i = 0; i < 40; i++)
                    {
                        ImGui::SliderInt(std::to_string(i).c_str(), &sliders[i], 0, 100 + i, "%d", 0, "Testing slider");
                    }
                    ImGui::EndChild();
                }

                if (iTabs == 5)
                {
                    static bool change_main_color = false;
                    ImGui::SetCursorPos(ImVec2(110, pos_offset));
                    ImGui::BeginChild("Settings");
                    ImGui::ColorBox("Render animated background", &animated_background, "Animated background in the form of particles", (float*)&color_particle);
                    ImGui::ColorBox("Change menu color", &change_main_color, "Change main menu color (default red)", (float*)&main_color);
                    ImGui::EndChild();
                }
            }
            ImGui::End();
        }

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

