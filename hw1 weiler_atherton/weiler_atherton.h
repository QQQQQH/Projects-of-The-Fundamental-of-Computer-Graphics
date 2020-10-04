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
    int type = 0; // -2=in, -1=enter, 1=exit, 2=out
    bool visited = false;
    bool last = false;
    Point *next[2] = {nullptr};
    Point(double x0, double y0): x(x0), y(y0) {}
};

struct Polygon {
    Point *head = nullptr,
           *tail = nullptr;
    bool visited;
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
    bool noCross = true, out = false;

    vector <QLineF> lines[2];
    vector <Point> points[2];
    list <Point> crossPoints;

    vector <priority_queue<CrossPoint>> crossPointsOnLine[2];
    vector <Polygon> polygons[2];
    vector<vector <Point>> cutResPs;
    vector <QLineF> cutResLs;

    bool in_segment(double x, const QLineF &line) const;
    bool on_segment(double x, double y, const QLineF &line) const;
    bool cross_at_end(const QLineF line1, const QLineF line2) const;
    bool ray_cross(double x, double y, const QLineF &l) const;
    bool out_reverse() const;
    bool get_cross_points(const QLineF &line1, const QLineF &line2, double &x, double &y, double &d1, double &d2) const;
    bool in_polygon(const vector<Point> &plg1, const vector<QLineF> &plg2) const;
    void cross_cut();
    void get_cut_line();
    void inner_cut();

  public:
    Weiler_Atherton(const vector<QLineF> &lines1, const vector<QLineF> &lines2,
                    const vector<Point> &points1, const vector<Point> &points2);
    void get_cross_points();
    void weiler_atherton(vector<vector <Point>> &resPs, vector<QLineF> &resLs);
};

#endif // WEILER_ATHERTON_H
