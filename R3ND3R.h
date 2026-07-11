#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <windows.h>
#include <algorithm>
#include <string>

namespace R3ND3R {
    
    struct Vec3 { float x, y, z; };
    struct Pixel { char c; int r, g, b; float z; };

    class Engine {
    private:
        HANDLE hConsole;
        int width, height;
        std::vector<Pixel> screen;
        const std::string palette = " .,-~:;=!*#$@";

        // new internal function to hijack terminal font scaling
        void SetResolution(short fontSize) {
            CONSOLE_FONT_INFOEX cfi;
            cfi.cbSize = sizeof(cfi);
            
            // grab the current font settings so we don't break the font family
            GetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
            
            // force the new size (Y is height, X is width which is roughly half)
            cfi.dwFontSize.Y = fontSize;
            cfi.dwFontSize.X = fontSize / 2; 
            
            SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
        }

        void UpdateDimensions() {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(hConsole, &csbi);
            width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
            
            if (screen.size() != width * height) {
                screen.resize(width * height);
            }
        }

        void EnableTrueColor() {
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD dwMode = 0;
            GetConsoleMode(hConsole, &dwMode);
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hConsole, dwMode);
        }

    public:
        // optional fontSize parameter. defaults to 0 (meaning ignore it)
        Engine(short fontSize = 0) {
            EnableTrueColor();
            
            // if dev requested a custom resolution, apply it before measuring dimensions
            if (fontSize > 0) {
                SetResolution(fontSize);
            }
            
            UpdateDimensions();
        }

        int GetWidth() { return width; }
        int GetHeight() { return height; }

        void Clear() {
            UpdateDimensions();
            Pixel empty = {' ', 0, 0, 0, 10000.0f};
            std::fill(screen.begin(), screen.end(), empty);
        }

        void DrawPixel(int x, int y, float z, char c, int r, int g, int b) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                int idx = y * width + x;
                if (z < screen[idx].z) {
                    screen[idx] = {c, r, g, b, z};
                }
            }
        }

        void Display() {
            std::string output;
            output.reserve(width * height * 25); 
            
            output += "\x1b[H"; 

            int last_r = -1, last_g = -1, last_b = -1;

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    Pixel& p = screen[y * width + x];
                    
                    if (p.r != last_r || p.g != last_g || p.b != last_b) {
                        output += "\x1b[38;2;" + std::to_string(p.r) + ";" 
                                               + std::to_string(p.g) + ";" 
                                               + std::to_string(p.b) + "m";
                        last_r = p.r; last_g = p.g; last_b = p.b;
                    }
                    output += p.c;
                }
                if (y < height - 1) output += "\n";
            }
            
            output += "\x1b[0m"; 

            DWORD dwBytesWritten = 0;
            WriteConsoleA(hConsole, output.c_str(), output.size(), &dwBytesWritten, NULL);
        }
    };
}
