#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->btnMain, &QPushButton::clicked, ui->sketchBoard, &SketchBoard::on_btnMain_clicked);
    connect(ui->btnCut, &QPushButton::clicked, ui->sketchBoard, &SketchBoard::on_btnCut_clicked);
    connect(ui->btnDoCut, &QPushButton::clicked, ui->sketchBoard, &SketchBoard::on_btnDoCut_clicked);
    connect(ui->btnClear, &QPushButton::clicked, ui->sketchBoard, &SketchBoard::on_btnClear_clicked);
    connect(ui->sketchBoard, &SketchBoard::mouse_move, this, &MainWindow::set_coord_label);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::set_coord_label(QPointF p) {
    ui->coordLabel->setText("x: " + QString::number(p.rx()) + "\ny: " + QString::number(p.ry()));
}

