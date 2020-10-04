#include "sketchboard.h"
#include "ui_sketchboard.h"
#include "weiler_atherton.h"

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
        vector <QPointF> &ps = points[i];
        vector <QLineF> &ls = lines[i];
        painter.setPen(QPen(colors[i], 4));
        for (QPointF p : ps) {
            painter.drawPoint(p);
        }
        for (QLineF l : ls) {
            painter.drawLine(l);
        }
    }
    painter.setPen(QPen(colors[2], 4));
    for (auto &ls : resLines) {
        for (QLineF l : ls) {
            painter.drawLine(l);
        }
    }
}

void SketchBoard::mouseReleaseEvent(QMouseEvent *e) {
    if (type == Type::Result) {
        if (resLines.size() == 1) {
            points[0] = resPoints[0];
            lines[0] = resLines[0];
            type = Type::Cut;
        }
        else {
            type = Type::Main;
        }
        points[1].clear();
        lines[1].clear();
        resPoints.clear();
        resLines.clear();
        update();
    }
    int num = type == Type::Main ? 0 : 1;
    vector <QPointF> &ps = points[num];
    vector <QLineF> &ls = lines[num];

    if (e->button() == Qt::LeftButton) {
        if (lastClick == Qt::RightButton) {
            ps.clear();
            ls.clear();
        }
        lastClick = Qt::LeftButton;
        ps.push_back(e->pos());
        update();
    }
    else if (e->button() == Qt::RightButton) {
        lastClick = Qt::RightButton;
        for (int i = 1, sz = ps.size(); i < sz; ++i) {
            ls.push_back(QLineF(ps[i - 1], ps[i]));
        }
        if (ps.size() > 2) {
            ls.push_back(QLineF(ps[ps.size() - 1], ps[0]));
        }
        update();
    }
}

void SketchBoard::mouseMoveEvent(QMouseEvent *e) {
    emit mouse_move(e->pos());
}

void SketchBoard::on_btnMain_clicked() {
    type = Type::Main;
    resPoints.clear();
    resLines.clear();
    qDebug() << "Main";
}

void SketchBoard::on_btnCut_clicked() {
    type = Type::Cut;
    resPoints.clear();
    resLines.clear();
    qDebug() << "Cut";
}

void SketchBoard::on_btnDoCut_clicked() {
    if (lines[0].size() < 3 || lines[1].size() < 3) {
        return;
    }
    for (auto &ls : lines) {
        qDebug() << ls.size();
        for (auto line : ls) {
            qDebug() << line;
        }
        qDebug() << "-------";
    }
    qDebug() << "Do cut";
    Weiler_Atherton wa(lines[0], lines[1]);
    qDebug() << "Get cross points";
    wa.get_cross_points();
    qDebug() << "Get cut";
    resPoints = wa.weiler_atherton();
    qDebug() << "Done: Get cut";
    for (auto &ps : resPoints) {
        for (QPointF &p : ps) {
            qDebug() << p.x() << " " << p.y();
        }
        qDebug() << "---";
        vector <QLineF> ls;
        for (int i = 1, sz = ps.size(); i < sz; ++i) {
            ls.push_back(QLineF(ps[i - 1], ps[i]));
        }
        if (ps.size() > 2) {
            ls.push_back(QLineF(ps[ps.size() - 1], ps[0]));
        }
        resLines.push_back(ls);
        update();
    }
    type = Type::Result;
}
