#include <cstdio>
#include <rfb/rfb.h>
#include <rfb/keysym.h>

#include "win32screencap.h"


#ifndef HAVE_HANDLEKEY
static void HandleKey(rfbBool down, rfbKeySym key, rfbClientPtr cl) {
    if (down && (key == XK_Escape || key == 'q' || key == 'Q'))
        rfbCloseClient(cl);
}
#endif


int main(int argc, char **argv) {
    LPBYTE buffer = NULL;
    int width = 0;
    int height = 0;
    WORD bbp = 32;

    SetDpiAwareness();

    CaptureScreen(buffer, width, height, bbp);

    int paddedWidth = width;
    if (width & 3)
        paddedWidth += 4 - (width & 3);

    /* initialize data for vnc server */
    rfbScreenInfoPtr rfbScreen = rfbGetScreen(&argc, argv, paddedWidth, height, 8, 3, bbp / 8);
    if (!rfbScreen)
        return 1;
    if (argc > 1)
        rfbScreen->desktopName = argv[1];
    else
        rfbScreen->desktopName = "Picture";
    rfbScreen->alwaysShared = TRUE;
    rfbScreen->kbdAddEvent = HandleKey;
    rfbScreen->frameBuffer = (char*) malloc(paddedWidth * height * (bbp/8));

    /* enable http */
    rfbScreen->httpDir = "../webclients";

    /* initialize server */
    rfbInitServer(rfbScreen);

    /* run event loop */
    rfbRunEventLoop(rfbScreen, -1,TRUE);

    while(true) {
        CaptureScreen(buffer, width, height, bbp);
        for(int i=3; i<(width*height*(bbp/8)); i+=4) {
            rfbScreen->frameBuffer[i-3] = (char) buffer[i-1];
            rfbScreen->frameBuffer[i-2] = (char) buffer[i-2];
            rfbScreen->frameBuffer[i-1] = (char) buffer[i-3];
            rfbScreen->frameBuffer[i] = (char) buffer[i];
        }
        rfbMarkRectAsModified(rfbScreen, 0, 0, width, height);
        _sleep(1000/5);
    }

    return (0);
}
