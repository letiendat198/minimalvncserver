#include <stdio.h>

#include "win32screencap.h"
#include "util.h"

BOOL GetBitmapBuffer(HDC hdc, HBITMAP hBitmap, LPBYTE& bp, DWORD& size, WORD& bbp) {
    BITMAPINFO bmi;
    LPBITMAPINFO lpbmi =  &bmi;

    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);

    // Load information of bitmap to BITMAPINFO
    int r = GetDIBits(hdc, hBitmap, 0, 0, NULL, lpbmi, DIB_RGB_COLORS);
    if (!r) {
        printf("GetDIBits for info failed\n");
        int errCode = GetLastError();
        PrintError(errCode);
        return FALSE;
    }

    if (bp!=NULL) free(bp);
    LPBYTE pixBuf = (LPBYTE) malloc(bmi.bmiHeader.biSizeImage);
    bp = pixBuf;

    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biHeight = -abs(bmi.bmiHeader.biHeight); // Positive height means top down bitmap

    r = GetDIBits(hdc, hBitmap, 0, bmi.bmiHeader.biHeight, bp, lpbmi, DIB_RGB_COLORS);
    size = bmi.bmiHeader.biSizeImage;
    bbp = bmi.bmiHeader.biBitCount;

    if (!r) {
        printf("GetDIBits for data failed\n");
        return FALSE;
    }
    return TRUE;
}

void CaptureScreen(LPBYTE& buffer, int& width, int& height, WORD& bbp) {
    DWORD size = 0;

    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    BOOL r;

    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);

    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
    SelectObject(hMemoryDC, hBitmap);
    r = BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0,SRCCOPY);
    if (!r) {
        printf("BitBlt failed\n");
        int errCode = GetLastError();
        PrintError(errCode);
        goto cleanup;
    }

    // Buffer is a LPBYTE - which is a pointer itself, so pass it by reference as normal
    r = GetBitmapBuffer(hScreenDC, hBitmap, buffer, size, bbp);
    if(!r) {
        printf("Screen capture failed\n");
    }
cleanup:
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);
}

BOOL SetDpiAwareness() {
    printf("SetProcessDpiAwareness\n");
    PGNSI pGNSI = (PGNSI) GetProcAddress(GetModuleHandle(TEXT("user32.dll")),
                                         "SetProcessDpiAwarenessContext");
    if (pGNSI) {
        BOOL r = pGNSI(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
        if(r) return TRUE;
    }
    DWORD errorCode = GetLastError();
    PrintError(errorCode);
    return FALSE;
}
