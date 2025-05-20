#pragma once

typedef struct Color{
    public:
        Color()
        {
            r = 0;
            g = 0;
            b = 0;
            a = 255;
        }
        Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
} Color;
