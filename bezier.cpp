#include "olcPixelGameEngine.h"

#include "bezier.h"

using olc::vf2d;

QuadraticBezier::QuadraticBezier()
{
    degree = 2;
    for (int i = 0; i < 3; i++)
    {
        controlPoints[i] = {0.0, 0.0};
    }
}

QuadraticBezier::QuadraticBezier(vf2d p1, vf2d p2, vf2d p3)
{
    degree = 2;
    controlPoints[0] = p1;
    controlPoints[1] = p2;
    controlPoints[2] = p3;
}

QuadraticBezier::~QuadraticBezier() { }

void QuadraticBezier::Draw(olc::PixelGameEngine *pge, olc::Pixel color)
{
    /*
    Bezier Method: Blending points
        Quadratic: p = (1-t)^2 *P0 + 2*(1-t)*t*P1 + t*t*P2
        Cubic: p = (1-t)^3 *P0 + 3*t*(1-t)^2*P1 + 3*t^2*(1-t)*P2 + t^3*P3 

    Alternative:
    de Casteljau Method: recursively split the control path until you arrive at the point using a linear interpolation
    */

    float interval = (controlPoints[0] - controlPoints[1]).mag() + (controlPoints[1] - controlPoints[2]).mag();
    interval += (controlPoints[0] - controlPoints[2]).mag();
    interval /= 4;
    interval = 1 / interval;

    vf2d p;
    for (float i = 0; i <= 1; i += interval)
    {
        p = (1-i) * (1-i) * controlPoints[0] + 2 * i * (1-i) * controlPoints[1] + i * i * controlPoints[2];

        pge->FillCircle(p, 2, color);
    }
}



Bezier::Bezier()
{
    degree = 0;
    controlPoints = nullptr;
}

Bezier::Bezier(int degree, vf2d *p)
{
    this->degree = degree;
    // controlPoints = new vf2d[degree + 1];
    // controlPoints = (vf2d *) malloc(sizeof(vf2d) * (degree + 1));
    
    // for (int i = 0; i < degree + 1; i++)
    // {
    //     controlPoints[i] = p[i];
    // }
    this->controlPoints = p;
}

Bezier::Bezier(int degree)
{
    this->degree = degree;
    // controlPoints = (vf2d *) malloc(sizeof(vf2d) * (degree + 1));

    // for (int i = 0; i < degree + 1; i++)
    // {
    //     controlPoints[i] = {0, 0};
    // }
}

Bezier::~Bezier()
{
    // if (controlPoints != nullptr)
    //     delete controlPoints;
}

// Helper for getting binomial coefficients from Wikipedia (https://en.wikipedia.org/wiki/Binomial_coefficient#In_programming_languages)
unsigned long binomial(unsigned long n, unsigned long k)
{
    unsigned long c = 1, i;

    if (k > n-k) // take advantage of symmetry
        k = n-k;

    for (i = 1; i <= k; i++, n--)
    {
        // Ignore possibility of overflow (don't care)

        c = c / i * n + c % i * n / i; // split c * n / i into (c / i * i + c % i) * n / i
    }

    return c;
}

void Bezier::Draw(olc::PixelGameEngine *pge, olc::Pixel color, int controlPointRadius)
{
    // dotted lines
    for (int i = 1; i <= degree; i++)
    {
        pge->DrawLine(controlPoints[i - 1], controlPoints[i], olc::DARK_GREY, 0xFF00FF00);
    }

    // Curve itself
    DrawBare(pge, color);

    // control points
        // Last point
    if (degree > 0)
    pge->FillCircle(controlPoints[degree], controlPointRadius, olc::WHITE);

    for (int i = 1; i < degree; i++)
    {
        pge->FillCircle(controlPoints[i], controlPointRadius, olc::YELLOW);
    }
        // First point
    pge->FillCircle(controlPoints[0], controlPointRadius, olc::WHITE);
}

void Bezier::DrawBare(olc::PixelGameEngine *pge, olc::Pixel color)
{
    float interval = 0;
    for (int i = 1; i <= degree; i++)
    {
        interval += (controlPoints[i] - controlPoints[i - 1]).mag();
    }
    interval = 1 / (interval);

    vf2d p;
    float t_;
    for (float t = 0; t <= 1; t += interval)
    {
        // p = (1-t) * (1-t) * controlPoints[0] + 2 * t * (1-t) * controlPoints[1] + t * t * controlPoints[2];

        p = {0, 0};
        t_ = 1 - t;

        for (int j = 0; j <= degree; j++)
        {
            p += binomial(degree, j) * powf(t_, degree - j) * powf(t, j) * controlPoints[j];
        }

        pge->FillCircle(p, 2, color);
    }
}
