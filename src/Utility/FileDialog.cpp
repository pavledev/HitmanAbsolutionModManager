#include <Windows.h>
#include <commdlg.h>

#include <Utility/FileDialog.h>

std::string FileDialog::OpenFileDialog()
{
    OPENFILENAME ofn;
    char szFile[260];
    HWND hwnd = NULL;
    HANDLE hf;

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0'; //Set lpstrFile[0] to '\0' so that GetOpenFileName does not use the contents of szFile to initialize itself.
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "ZIP Files (*.zip)\0*.zip\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        return ofn.lpstrFile;
    }

    return "";
}
