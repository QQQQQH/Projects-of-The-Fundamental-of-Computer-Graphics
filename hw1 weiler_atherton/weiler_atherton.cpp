#include "weiler_atherton.h"

#include <QDebug>

bool Weiler_Atherton::in_segment(double x, const QLineF &line) const {
    return x > line.x1() && x < line.x2() || x < line.x1() && x > line.x2();
}

bool Weiler_Atherton::cross_at_end(const QLineF line1, const QLineF line2) const {
    return (line1.x2() - line1.x1()) * (line2.y1() - line1.y1()) == (line1.y2() - line1.y1()) * (line2.x1() - line1.x1()) ||
           (line1.x2() - line1.x1()) * (line2.y2() - line1.y1()) == (line1.y2() - line1.y1()) * (line2.x2() - line1.x1()) ||
           (line2.x2() - line2.x1()) * (line1.y1() - line2.y1()) == (line2.y2() - line2.y1()) * (line1.x1() - line2.x1()) ||
           (line2.x2() - line2.x1()) * (line1.y2() - line2.y1()) == (line2.y2() - line2.y1()) * (line1.x2() - line2.x1());
}

bool Weiler_Atherton::ray_cross(double x, double y, int linesNum, int lineNum) const {
    QLineF line = lines[linesNum][lineNum];
    int sz = lines[1].size();
    if (x == line.x1()) {
        if (x == line.x2()) {
            return false;
        }
        if (y < line.y1()) {
            return false;
        }
        QLineF line2 = lines[linesNum][(lineNum - 1 + sz) % sz];
        if (x < line2.x1() && x < line.x2() || x > line2.x1() && x > line.x2()) {
            return false;
        }
        return true;
    }
    if (x == line.x2()) {
        if (y < line.y2()) {
            return false;
        }
        QLineF line2 = lines[linesNum][(lineNum + 1) % sz];
        if (x < line2.x2() && x < line.x1() || x > line2.x2() && x > line.x1()) {
            return false;
        }
        return true;
    }
    if (x > line.x1() && x > line.x2() || x < line.x1() && x < line.x2()) {
        return false;
    }
    double ty = (x - line.x1()) * (line.y2() - line.y1()) / (line.x2() - line.x1()) + line.y1();
    return y > ty;
}

bool Weiler_Atherton::cover_reverse() const {
    for (int i = 0, szi = lines[1].size(); i < szi; ++i) {
        const QLineF &line1 = lines[1][i];
        int type = -1;
        for (int j = 0, szj = lines[0].size(); j < szj; ++j) {
            const QLineF &line2 = lines[0][j];
            if (cross_at_end(line1, line2)) {
                continue;
            }
            if (ray_cross(line1.x1(), line1.y1(), 0, j)) {
                type = -type;
            }
        }
        if (type == 1) {
            return false;
        }
    }
    return true;
}

Weiler_Atherton::Weiler_Atherton(const vector<QLineF> &line1, const vector<QLineF> &line2) {
    lines[0] = line1, lines[1] = line2;
    for (int i = 0; i < 2; ++i) {
        qDebug() << "line" << i + 1;
        int sz = lines[i].size();
        for (int j = 0; j < sz; ++j) {
            points[i].push_back(Point(lines[i][j].x1(), lines[i][j].y1()));
            //            qDebug() << lines[i][j].x1() << " " << lines[i][j].y1();
        }
        crossPointsOnLine[i] = vector <priority_queue<CrossPoint>> (points[i].size());
        polygons[i] = nullptr;
        polygonTails[i] = nullptr;
    }
}

void Weiler_Atherton::get_cross_points() {
    for (int i = 0, szi = lines[0].size(); i < szi; ++i) {
        const QLineF &line1 = lines[0][i];
        const double l1 = line1.x2() - line1.x1();
        int type = -1;
        for (int j = 0, szj = lines[1].size(); j < szj; ++j) {
            const QLineF &line2 = lines[1][j];
            if (cross_at_end(line1, line2)) {
                continue;
            }
            if (ray_cross(line1.x1(), line1.y1(), 1, j)) {
                type = -type;
            }
            double a1 = line1.y1() - line1.y2(),
                   b1 = line1.x2() - line1.x1(),
                   c1 = line1.x2() * line1.y1() - line1.x1() * line1.y2();
            double a2 = line2.y1() - line2.y2(),
                   b2 = line2.x2() - line2.x1(),
                   c2 = line2.x2() * line2.y1() - line2.x1() * line2.y2();
            const double D = a1 * b2 - a2 * b1;
            if (D == 0) {
                continue;
            }
            const double x = (b2 * c1 - b1 * c2) / D,
                         y = (a1 * c2 - a2 * c1) / D;
            if (!in_segment(x, line1) || !in_segment(x, line2)) {
                continue;
            }
            crossPoints.push_back(Point(x, y));
            const double l2 = line2.x2() - line2.x1();
            const double d1 = (x - line1.x1()) / l1,
                         d2 = (x - line2.x1()) / l2;
            Point *p = &crossPoints.back();
            //            qDebug() << "1 " << p << " " << p->x << " " << p->y;

            crossPointsOnLine[0][i].push(CrossPoint(d1, p));
            crossPointsOnLine[1][j].push(CrossPoint(d2, p));
            //            qDebug() << "2 " << p << " " << p->x << " " << p->y;
        }
        if (type == 1) {
            cover = false;
        }
        Point *p = &points[0][i];
        if (!polygons[0]) {
            polygons[0] = polygonTails[0] = p;
        }
        else {
            polygonTails[0]->next0 = p;
            polygonTails[0] = p;
        }
        priority_queue<CrossPoint> &q = crossPointsOnLine[0][i];
        while (!q.empty()) {
            Point *p = q.top().p;
            //            qDebug() << "3 " << p << " " << p->x << " " << p->y;
            q.pop();
            p->type = type;
            type = -type;
            polygonTails[0]->next0 = p;
            polygonTails[0] = p;
        }
    }
    if (crossPoints.size() > 0) {
        noCross = false;
    }
    for (int i = 0, sz = lines[1].size(); i < sz; ++i) {
        Point *p = &points[1][i];
        if (!polygons[1]) {
            polygons[1] = polygonTails[1] = p;
        }
        else {
            polygonTails[1]->next1 = p;
            polygonTails[1] = p;
        }
        priority_queue<CrossPoint> &q = crossPointsOnLine[1][i];
        while (!q.empty()) {
            ++unvisited;
            Point *p = q.top().p;
            q.pop();
            polygonTails[1]->next1 = p;
            polygonTails[1] = p;
            //            qDebug() << "4 " << p << " " << p->x << " " << p->y;
        }
    }
    //    qDebug() << "polygon1";
    //    Point *p = polygons[0];
    //    while (p) {
    //        qDebug() << " " << p->x << " " << p->y << p->type;
    //        p = p->next0;
    //    }
    //    qDebug() << "polygon2";
    //    p = polygons[1];
    //    while (p) {
    //        qDebug() << " " << p->x << " " << p->y << p->type;
    //        p = p->next1;
    //    }
    polygonTails[0]->next0 = polygons[0];
    polygonTails[1]->next1 = polygons[1];
}

vector<vector <QPointF>> Weiler_Atherton::weiler_atherton() {
    vector<vector <QPointF>> cutRes;
    if (noCross) {
        if (cover) {
            if (cover_reverse()) {
                return {{}};
            }
            vector <QPointF> res;
            for (auto &line : lines[1]) {
                res.push_back(line.p1());
            }
            cutRes.push_back(res);
            return cutRes;
        }
        if (cover_reverse()) {
            vector <QPointF> res;
            for (auto &line : lines[0]) {
                res.push_back(line.p1());
            }
            cutRes.push_back(res);
            return cutRes;
        }
        return {{}};
    }
    Point *p = polygons[0];
    while (unvisited) {
        qDebug() << unvisited;
        vector<QPointF> res;
        while (p->type != -1 || p->visited) {
            p = p->next0;
        }
        p->visited = true;
        if (p->type == -1 || p->type == 1) {
            --unvisited;
        }
        res.push_back(QPointF(p->x, p->y));
        Point *start = p;
        Point *q = p;
        p = p->next0;

        while (p != start) {
            p->visited = true;
            if (p->type == -1 || p->type == 1) {
                --unvisited;
            }
            res.push_back(QPointF(p->x, p->y));
            if (p->type == -1) {
                q = p;
                p = p->next0;
            }
            else if (p->type == 1) {
                q = p;
                p = p->next1;
            }
            else {
                if (q->type == -1) {
                    p = p->next0;
                }
                else {
                    p = p->next1;
                }
            }
        }
        p = q;
        cutRes.push_back(res);
    }
    return cutRes;
}
