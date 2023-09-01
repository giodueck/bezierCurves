#include "olcPixelGameEngine.h"

#include "bezier.h"

using olc::vi2d;
using olc::vf2d;

class BezierCurves : public olc::PixelGameEngine
{
private:
    // QuadraticBezier bezier;
    Bezier bezier;
    
    int degree = 2;
    int controlPointRadius = 8;
    vf2d *p;

public:
    BezierCurves()
    {
        sAppName = "BezierCurves";
    }

public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        
        p = (vf2d *) malloc(sizeof(vf2d) * (degree + 1));
        
        for (int i = 0; i <= degree; i++)
        {
            p[i] = {(float) ScreenWidth() / (i + 2), (float) ScreenHeight() / (i + 2)};
        }
        bezier = Bezier(degree, p);

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called once per frame

        /* Controls */
        static int selPoint = -1;
        static vi2d lastMousePos = {0, 0};

        if (selPoint == -1 && GetMouse(olc::Mouse::LEFT).bPressed)
        {
            for (int i = 0; i <= bezier.degree; i++)
            {
                if ((GetMousePos() - bezier.controlPoints[i]).mag() <= controlPointRadius)
                {
                    selPoint = i;
                    lastMousePos = GetMousePos();
                    break;
                }
            }
        }
        else if (GetMouse(olc::Mouse::LEFT).bReleased)
        {
            selPoint = -1;
        }
        else if (selPoint != -1)
        {
            vi2d dPos = GetMousePos() - lastMousePos;
            bezier.controlPoints[selPoint] += dPos;
            lastMousePos = GetMousePos();
        }

        // Add points
        if (selPoint == -1 && GetKey(olc::SPACE).bPressed)
        {
            bezier.degree++;
            bezier.controlPoints = (vf2d *) realloc(bezier.controlPoints, sizeof(vf2d) * (bezier.degree + 1));
            bezier.controlPoints[bezier.degree] = GetMousePos();
        }
        // Delete points
        if (selPoint == -1 && GetKey(olc::BACK).bPressed)
        {
            bezier.degree--;
            if (bezier.degree < 0)
                bezier.degree = 0;
        }

        /* Drawing */
        Clear(olc::BLACK);

        // dotted lines
        for (int i = 1; i <= bezier.degree; i++)
        {
            DrawLine(bezier.controlPoints[i - 1], bezier.controlPoints[i], olc::DARK_GREY, 0xFF00FF00);
        }

        // Bezier curve
        bezier.Draw(this);

        // control points
            // Last point
        if (bezier.degree > 0)
        FillCircle(bezier.controlPoints[bezier.degree], controlPointRadius, olc::WHITE);

        for (int i = 1; i < bezier.degree; i++)
        {
            FillCircle(bezier.controlPoints[i], controlPointRadius, olc::YELLOW);
        }
            // First point
        FillCircle(bezier.controlPoints[0], controlPointRadius, olc::WHITE);

        return true;
    }
};


int main()
{
    BezierCurves demo;
    if (demo.Construct(1024, 576, 1, 1)) // 16:9
    // if (demo.Construct(512, 288, 2, 2)) // 16:9
        demo.Start();

    return 0;
}
