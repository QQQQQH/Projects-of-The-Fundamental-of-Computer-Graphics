#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sketchboard.h"
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void set_coord_label(QPointF p);
    void set_btnCont(bool f);

  private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
