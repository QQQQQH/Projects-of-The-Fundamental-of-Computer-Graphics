#ifndef SKETCHBOARD_H
#define SKETCHBOARD_H

#include <QFrame>
#include <vector>

using namespace std;

namespace Ui {
class SketchBoard;
}

class SketchBoard : public QWidget {
    Q_OBJECT

  public:
    enum Type {
        Main, Cut, Result
    };

    explicit SketchBoard(QWidget *parent = nullptr);
    ~SketchBoard();
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void setType(Type type0);


  private:
    Ui::SketchBoard *ui;
    vector <QPointF> points[2];
    vector<vector <QPointF>> resPoints;
    vector <QLineF> lines[2];
    vector<vector <QLineF>> resLines;

    Type type = Type::Main;

    Qt::MouseButton lastClick = Qt::RightButton;

    QColor colors [3] = {Qt::blue, Qt::green, Qt::red};

  signals:
    void mouse_move(QPointF p);
  public slots:
    void on_btnMain_clicked();
    void on_btnCut_clicked();
    void on_btnDoCut_clicked();
};

#endif // SKETCHBOARD_H
