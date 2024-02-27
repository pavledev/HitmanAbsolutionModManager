#pragma once

class D3D11Utility
{
public:
    template <typename T>
    static void Release(T* pointer)
    {
        if (pointer)
        {
            pointer->Release();

            pointer = nullptr;
        }
    }
};
