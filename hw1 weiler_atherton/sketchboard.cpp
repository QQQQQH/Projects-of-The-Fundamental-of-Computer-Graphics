#include "sketchboard.h"
#include "ui_sketchboard.h"

#include <QPainter>
#include <QMouseEvent>
#include <QStyle>
#include <QDebug>

SketchBoard::SketchBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SketchBoard) {
    ui->setupUi(this);
    setMouseTracking(true);
}

SketchBoard::~SketchBoard() {
    delete ui;
}

void SketchBoard::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(rect());

    for (int i = 0; i < 2; ++i) {
        painter.setPen(QPen(colors[i], 4));

        vector <Point> &ps = points[i];
        vector <QLineF> &ls = lines[i];
        for (const Point &p : ps) {
            painter.drawPoint(QPointF(p.x, p.y));
        }
        for (const QLineF &l : ls) {
            painter.drawLine(l);
        }
    }

    painter.setPen(QPen(colors[2], 4));
    for (const QLineF &l : resLines) {
        painter.drawLine(l);
    }
}

void SketchBoard::mouseReleaseEvent(QMouseEvent *e) {
    if (type == Type::Result) {
        if (resPoints.size() == 1) {
            points[0] = resPoints[0];
            lines[0] = resLines;
            qDebug() << points[0].size() << "?";
        }
        type = Type::Cut;
        points[1].clear();
        lines[1].clear();
        resPoints.clear();
        resLines.clear();
        update();
    }

    int num = type == Type::Main ? 0 : 1;

    vector <Point> &ps = points[num];
    vector <QLineF> &ls = lines[num];

    if (e->button() == Qt::LeftButton) {
        QPointF p = e->pos();
        ps.push_back(Point(p.x(), p.y()));
        if (lastClick == Qt::RightButton) {
            startPointNum = ps.size() - 1;
        }
        update();
        lastClick = Qt::LeftButton;
    }
    else if (e->button() == Qt::RightButton) {
        if (ps.size() - startPointNum < 3) {
            return;
        }
        lastClick = Qt::RightButton;
        for (int i = startPointNum, sz = ps.size() - 1; i < sz; ++i) {
            QPointF x(ps[i].x, ps[i].y), y(ps[i + 1].x, ps[i + 1].y);
            ls.push_back(QLineF(x, y));
        }
        QPointF x(ps[ps.size() - 1].x, ps[ps.size() - 1].y), y(ps[startPointNum].x, ps[startPointNum].y);
        ls.push_back(QLineF(x, y));
        ps[ps.size() - 1].last = true;
        update();
    }
}

void SketchBoard::mouseMoveEvent(QMouseEvent *e) {
    emit mouse_move(e->pos());
}

void SketchBoard::on_btnMain_clicked() {
    type = Type::Main;
    points[0].clear();
    lines[0].clear();
    resPoints.clear();
    resLines.clear();
    lastClick = Qt::RightButton;
    update();
    qDebug() << "Main";
}

void SketchBoard::on_btnCut_clicked() {
    type = Type::Cut;
    points[1].clear();
    lines[1].clear();
    resPoints.clear();
    resLines.clear();
    lastClick = Qt::RightButton;
    update();
    qDebug() << "Cut";
}

void SketchBoard::on_btnDoCut_clicked() {
    if (lines[0].size() < 3 || lines[1].size() < 3) {
        return;
    }
    qDebug() << "Do cut";
    Weiler_Atherton wa(lines[0], lines[1], points[0], points[1]);
    qDebug() << "Get cross points";
    wa.get_cross_points();
    qDebug() << "Get cut";
    wa.weiler_atherton(resPoints, resLines);
    update();
    lastClick = Qt::RightButton;
    type = Type::Result;
}

void SketchBoard::on_btnClear_clicked() {
    for (int i = 0; i < 2; ++i) {
        points[i].clear();
        lines[i].clear();
    }
    resPoints.clear();
    resLines.clear();
    type = Type::Main;
    lastClick = Qt::RightButton;
    update();
}
