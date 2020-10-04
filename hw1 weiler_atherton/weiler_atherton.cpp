#include "weiler_atherton.h"

#include <QDebug>

Weiler_Atherton::Weiler_Atherton(const vector<QLineF> &lines1, const vector<QLineF> &lines2,
                                 const vector<Point> &points1, const vector<Point> &points2) {
    lines[0] = lines1, lines[1] = lines2;
    points[0] = points1, points[1] = points2;
    for (int i = 0; i < 2; ++i) {
        crossPointsOnLine[i] = vector <priority_queue<CrossPoint>> (points[i].size());
    }
}

bool Weiler_Atherton::in_segment(double x, const QLineF &line) const {
    return x > line.x1() && x < line.x2() || x < line.x1() && x > line.x2();
}

bool Weiler_Atherton::on_segment(double x, double y, const QLineF &line) const {
    return (x - line.x1()) * (line.y2() - y) == (y - line.y1()) * (line.x2() - x);
}

bool Weiler_Atherton::cross_at_end(const QLineF line1, const QLineF line2) const {
    return (line1.x2() - line1.x1()) * (line2.y1() - line1.y1()) == (line1.y2() - line1.y1()) * (line2.x1() - line1.x1()) ||
           (line1.x2() - line1.x1()) * (line2.y2() - line1.y1()) == (line1.y2() - line1.y1()) * (line2.x2() - line1.x1()) ||
           (line2.x2() - line2.x1()) * (line1.y1() - line2.y1()) == (line2.y2() - line2.y1()) * (line1.x1() - line2.x1()) ||
           (line2.x2() - line2.x1()) * (line1.y2() - line2.y1()) == (line2.y2() - line2.y1()) * (line1.x2() - line2.x1());
}

bool Weiler_Atherton::ray_cross(double x, double y, const QLineF &l) const {
    double x1 = l.x1(),
           y1 = l.y1(),
           x2 = l.x2(),
           y2 = l.y2();
    if (y1 == y2 ||
        y > y1 && y > y2 ||
        y < y1 && y < y2) {
        return false;
    }
    double tx = (x2 - x1) * (y - y1) / (y2 - y1) + x1;
    return tx > x;
}

bool Weiler_Atherton::out_reverse() const {
    for (int i = 0, szi = lines[1].size(); i < szi; ++i) {
        const QLineF &line1 = lines[1][i];
        int type = 2;
        bool noCrossAtEnd = true;
        for (int j = 0, szj = lines[0].size(); j < szj; ++j) {
            const QLineF &line2 = lines[0][j];
            if (cross_at_end(line1, line2)) {
                noCrossAtEnd = false;
                continue;
            }
            if (ray_cross(line1.x1(), line1.y1(), lines[0][j])) {
                type = -type;
            }
        }
        if (noCrossAtEnd && type == 2) {
            return true;
        }
    }
    return false;
}

bool Weiler_Atherton::get_cross_points(const QLineF &line1, const QLineF &line2, double &x, double &y, double &d1, double &d2) const {
    double a1 = line1.y1() - line1.y2(),
           b1 = line1.x2() - line1.x1(),
           c1 = line1.x2() * line1.y1() - line1.x1() * line1.y2();
    double a2 = line2.y1() - line2.y2(),
           b2 = line2.x2() - line2.x1(),
           c2 = line2.x2() * line2.y1() - line2.x1() * line2.y2();
    const double D = a1 * b2 - a2 * b1;
    if (D == 0) {
        return false;
    }
    x = (b2 * c1 - b1 * c2) / D;
    y = (a1 * c2 - a2 * c1) / D;
    if (!in_segment(x, line1) || !in_segment(x, line2)) {
        return false;
    }
    const double l1 = line1.x2() - line1.x1(),
                 l2 = line2.x2() - line2.x1();
    d1 = (x - line1.x1()) / l1,
    d2 = (x - line2.x1()) / l2;
    return true;
}

bool Weiler_Atherton::in_polygon(const vector<Point> &plg1, const vector<QLineF> &plg2) const {
    for (int i = 0, szi = plg1.size(); i < szi; ++i) {
        bool out = true;
        double x = plg1[i].x,
               y = plg1[i].y;
        for (int j = 0, szj = plg2.size(); j < szj; ++j) {
            const QLineF &l = plg2[j];
            if (on_segment(x, y, l)) {
                out = false;
                break;
            }
            if (ray_cross(x, y, l)) {
                out = !out;
            }
        }
        if (out) {
            return false;
        }
    }
    return true;
}

void Weiler_Atherton::cross_cut() {
    if (noCross) {
        if (out) {
            if (out_reverse()) {
                cutResPs = {{}};
                return;
            }
            vector <Point> res;
            for (auto &line : lines[1]) {
                res.push_back(Point(line.x1(), line.y1()));
            }
            cutResPs.push_back(res);
            return;
        }
        if (out_reverse()) {
            vector <Point> res;
            for (auto &line : lines[0]) {
                res.push_back(Point(line.x1(), line.y1()));
            }
            cutResPs.push_back(res);
            return;
        }
        cutResPs = {{}};
        return;
    }

    while (unvisited > 0) {
        Point *p;
        vector<Point> res;
        for (int i = 0, sz = polygons[0].size(); i < sz; ++i) {
            if (polygons[0][i].visited) {
                continue;
            }
            p = polygons[0][i].head;
            while (p->type != -1 || p->visited) {
                p = p->next[0];
                if (p == polygons[0][i].head) {
                    polygons[0][i].visited = true;
                    break;
                }
            }
            if (polygons[0][i].visited) {
                continue;
            }
            break;
        }
        p->visited = true;
        --unvisited;
        res.push_back(Point(p->x, p->y));
        Point *start = p;
        Point *q = p;
        p = p->next[0];
        while (p != start) {
            if (p->visited) {
                break;
            }
            p->visited = true;
            res.push_back(Point(p->x, p->y));
            if (p->type == -1) {
                q = p;
                p = p->next[0];
                --unvisited;
            }
            else if (p->type == 1) {
                q = p;
                p = p->next[1];
            }
            else {
                if (q->type == -1) {
                    p = p->next[0];
                }
                else {
                    p = p->next[1];
                }
            }
        }
        p = q;
        cutResPs.push_back(res);
    }
}

void Weiler_Atherton::get_cut_line() {
    for (auto &ps : cutResPs) {
        if (ps.size() < 3) {
            ps.clear();
            continue;
        }
        for (int i = 0, sz = ps.size() - 1; i < sz; ++i) {
            QPointF x(ps[i].x, ps[i].y), y(ps[i + 1].x, ps[i + 1].y);
            cutResLs.push_back(QLineF(x, y));
        }
        QPointF x(ps[ps.size() - 1].x, ps[ps.size() - 1].y), y(ps[0].x, ps[0].y);
        cutResLs.push_back(QLineF(x, y));
        ps[ps.size() - 1].last = true;
    }
}

void Weiler_Atherton::inner_cut() {
    for (int i = 0; i < 2; ++i) {
        for (int j = 1, szj = polygons[i].size(); j < szj; ++j) {
            vector<Point> ps;
            bool hasCross = false;
            Point *p = polygons[i][j].head;
            ps.push_back(*p);
            p = p->next[i];
            while (p != polygons[i][j].head) {
                if (p->type == -1 || p->type == 1) {
                    hasCross = true;
                    break;
                }
                ps.push_back(*p);
                p = p->next[i];
            }
            if (hasCross || ps.size() < 3) {
                continue;
            }
            if (in_polygon(ps, cutResLs)) {
                for (int k = 0, sz = ps.size() - 1; k < sz; ++k) {
                    QPointF x(ps[k].x, ps[k].y), y(ps[k + 1].x, ps[k + 1].y);
                    cutResLs.push_back(QLineF(x, y));
                }
                QPointF x(ps[ps.size() - 1].x, ps[ps.size() - 1].y), y(ps[0].x, ps[0].y);
                cutResLs.push_back(QLineF(x, y));
                cutResPs.push_back(ps);
            }
        }
    }
}

void Weiler_Atherton::get_cross_points() {
    for (int i = 0, szi = lines[0].size(); i < szi; ++i) {
        const QLineF &line1 = lines[0][i];
        int type = 2;
        bool noCrossAtEnd = true;

        for (int j = 0, szj = lines[1].size(); j < szj; ++j) {
            const QLineF &line2 = lines[1][j];
            if (cross_at_end(line1, line2)) {
                noCrossAtEnd = false;
                continue;
            }
            if (ray_cross(line1.x1(), line1.y1(), lines[1][j])) {
                type = -type;
            }
            double x, y, d1, d2;
            if (!get_cross_points(line1, line2, x, y, d1, d2)) {
                continue;
            }
            crossPoints.push_back(Point(x, y));

            Point *p = &crossPoints.back();
            crossPointsOnLine[0][i].push(CrossPoint(d1, p));
            crossPointsOnLine[1][j].push(CrossPoint(d2, p));
        }

        if (crossPoints.size() > 0) {
            noCross = false;
        }
        if (noCrossAtEnd && type == 2) {
            out = true;
        }

        points[0][i].type = type;
        qDebug() << points[0][i].x << " " << points[0][i].y << " " << points[0][i].type;
    }

    for (int i = 0; i < 2; ++i) {
        polygons[i].push_back({});
        for (int j = 0, sz = lines[i].size(); j < sz; ++j) {
            Point *p = &points[i][j];

            Polygon &plg = polygons[i][polygons[i].size() - 1];
            if (!plg.head) {
                plg.head = plg.tail = p;
            }
            else {
                plg.tail->next[i] = p;
                plg.tail = p;
            }
            priority_queue<CrossPoint> &q = crossPointsOnLine[i][j];
            int type;
            if (i == 0) {
                if (p->type == 2) {
                    type = -1;
                }
                else {
                    type = 1;
                }
            }
            while (!q.empty()) {
                Point *t = q.top().p;
                q.pop();
                if (i == 0) {
                    if (type == -1) {
                        ++unvisited;
                    }
                    t->type = type;
                    type = -type;
                }
                plg.tail->next[i] = t;
                plg.tail = t;
            }
            if (p->last == true) {
                plg.tail->next[i] = plg.head;
                polygons[i].push_back({});
            }
        }
        polygons[i].pop_back();
    }
}

void Weiler_Atherton::weiler_atherton(vector<vector <Point>> &resPs, vector<QLineF> &resLs) {
    cross_cut();
    get_cut_line();
    inner_cut();
    resPs = cutResPs;
    resLs = cutResLs;
}
