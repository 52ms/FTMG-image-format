#include <iostream>

#include "ftmg.hpp"

#include <X11/Xlib.h>

#define RGB(r, g, b) ((r << 16) | (g << 8) | b)

int main() {
    Display* display = XOpenDisplay(NULL);
    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 800, 600, 0, RGB(0, 0, 0), RGB(255, 255, 255));
    XEvent e;
    GC gc = XCreateGC(display, window, 0, 0);

    XStoreName(display, window, "FTMG Image format");

    XMapWindow(display, window);
    XFlush(display);

    XSelectInput(display, window, ExposureMask);

    while (true) {
        XNextEvent(display, &e);
        if (e.type == Expose) {
            ftmgImageDraw(display, window, gc, "Squares.ftmg", 100, 100, 5);
            ftmgImageDraw(display, window, gc, "image.ftmg", 0, 0, 10);
        }
    }

    XCloseDisplay(display);

    return 0;
}