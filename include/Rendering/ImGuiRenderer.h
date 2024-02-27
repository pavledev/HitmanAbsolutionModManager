#pragma once

#include <d3d11.h>

#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class ImGuiRenderer
{
public:
    bool Setup(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext, HWND hwnd);
    void Cleanup();

    void AddFonts();
    void SetStyle();
    void SetScale();

    ImFont* GetRegularFont() const;
    ImFont* GetBoldFont() const;

private:
    HWND hwnd;

    ImFont* regularFont;
    ImFont* boldFont;
};
