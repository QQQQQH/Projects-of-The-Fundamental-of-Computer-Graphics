#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->btnMain, &QPushButton::clicked, ui->sketchBoard, &SketchBoard::on_btnMain_clicked);
    connect(ui->btnCut, &QPushButton::clicked, ui->sketchBoard, &SketchBoard::on_btnCut_clicked);
    connect(ui->btnDoCut, &QPushButton::clicked, ui->sketchBoard, &SketchBoard::on_btnDoCut_clicked);
    connect(ui->btnCont, &QPushButton::clicked, ui->sketchBoard, &SketchBoard::on_btnCont_clicked);
    connect(ui->sketchBoard, &SketchBoard::mouse_move, this, &MainWindow::set_coord_label);
    connect(ui->sketchBoard, &SketchBoard::set_btnCont, this, &MainWindow::set_btnCont);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::set_coord_label(QPointF p) {
    ui->coordLabel->setText("x: " + QString::number(p.rx()) + "\ny: " + QString::number(p.ry()));
}

void MainWindow::set_btnCont(bool f) {
    ui->btnCont->setEnabled(f);
}
