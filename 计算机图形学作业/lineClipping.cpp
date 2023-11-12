#include "lineClipping.h"
using namespace std;

int Clip_Top(float p, float q, float& umax, float& umin) {
    float r = 0.0;
    if (p < 0.0) {
        r = q / p;
        if (r > umin)
            return 0;
        else if (r > umax)
            umax = r;
    } else if (p > 0.0) {
        r = q / p;
        if (r < umax)
            return 0;
        else if (r < umin)
            umin = r;
    } else if (q < 0.0)
        return 0;
    return 1;
}

void Line_Clipping(vector<point>& points, rect& winRect) {
    if (points.size() >= 2) {
        point &p1 = points[0], &p2 = points[1];
        float dx = p2.x - p1.x, dy = p2.y - p1.y, umax = 0.0, umin = 1.0;
        point p = p1;
        if (Clip_Top(-dx, p1.x - winRect.w_xmin, umax, umin))
            if (Clip_Top(dx, winRect.w_xmax - p1.x, umax, umin))
                if (Clip_Top(-dy, p1.y - winRect.w_ymin, umax, umin))
                    if (Clip_Top(dy, winRect.w_ymax - p1.y, umax, umin)) {
                        p1.x = (int)(p.x + umax * dx);
                        p1.y = (int)(p.y + umax * dy);
                        p2.x = (int)(p.x + umin * dx);
                        p2.y = (int)(p.y + umin * dy);
                    }
    } else {
        cout << "Not enough points to clip the line." << endl;
    }
}