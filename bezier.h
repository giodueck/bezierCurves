#include "olcPixelGameEngine.h"

using olc::vf2d;

class QuadraticBezier
{
public:
    int degree;
    vf2d controlPoints[3];

public:
    QuadraticBezier();
    QuadraticBezier(vf2d p1, vf2d p2, vf2d p3);

    ~QuadraticBezier();

    void Draw(olc::PixelGameEngine *pge, olc::Pixel color = olc::RED);
};

class Bezier
{
public:
    int degree;
    vf2d *controlPoints;

public:
    Bezier();
    Bezier(int degree);
    Bezier(int degree, vf2d *p);

    ~Bezier();

    void Draw(olc::PixelGameEngine *pge, olc::Pixel color = olc::RED);
};
