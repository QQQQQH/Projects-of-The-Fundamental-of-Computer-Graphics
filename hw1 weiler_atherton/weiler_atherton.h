#ifndef WEILER_ATHERTON_H
#define WEILER_ATHERTON_H

#include <QPointF>
#include <QLineF>
#include <vector>
#include <queue>
#include <list>
#include <algorithm>

using namespace std;

struct Point {
    double x, y;
    int type = 0; // -1=in, 1=out
    bool visited = false;
    Point *next0 = nullptr, *next1 = nullptr;
    Point(double x0, double y0): x(x0), y(y0) {}
};

class Weiler_Atherton {
    struct CrossPoint {
        double d;
        Point *p = nullptr;
        CrossPoint(double d0, Point *p0): d(d0), p(p0) {}
        bool operator < (const CrossPoint &cp) const {
            return d > cp.d;
        }
    };

    const double eps = 1e-50;
    int unvisited = 0;
    bool noCross = true, cover = true;
    vector <priority_queue<CrossPoint>> crossPointsOnLine[2];
    vector <Point> points[2];
    list <Point> crossPoints;
    vector <QLineF> lines[2];
    Point *polygons[2], *polygonTails[2];

    bool in_segment(double x, const QLineF &line) const;
    bool cross_at_end(const QLineF line1, const QLineF line2)const;
    bool ray_cross(double x, double y, int linesNum, int lineNum) const;
    bool cover_reverse()const;
  public:
    Weiler_Atherton(const vector<QLineF> &line1, const vector<QLineF> &line2);
    void get_cross_points();
    vector<vector <QPointF>> weiler_atherton();
};

#endif // WEILER_ATHERTON_H
