#include "dip.h"
#include "ui_dip.h"
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include "trans.h"
#include "filter.h"

DIP::DIP(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DIP) {
    ui->setupUi(this);
}

DIP::~DIP() {
    delete ui;
}

void DIP::updateHeld() {
    if(stackHeld.empty()) ui->label_held->clear();
    else ui->label_held->setPixmap(
            QPixmap::fromImage(cvtMat2QImage(stackHeld.top()))
            .scaled(ui->label_held->size(),Qt::KeepAspectRatio));
}

void DIP::updateFlying() {
    if(stackFlying.empty()) ui->label_flying->clear();
    else ui->label_flying->setPixmap(
            QPixmap::fromImage(cvtMat2QImage(stackFlying.top()))
            .scaled(ui->label_flying->size(),Qt::KeepAspectRatio));
}

void DIP::updatePreview(cv::Mat mat) {
    if(mat.empty()) ui->label_flying->clear();
    else ui->label_flying->setPixmap(
            QPixmap::fromImage(cvtMat2QImage(mat))
            .scaled(ui->label_flying->size(),Qt::KeepAspectRatio));
}

void DIP::on_actionOpen_triggered() {
    QString filename=QFileDialog::getOpenFileName(
                         this, tr("Open Image"),"",
                         tr("Image (*.bmp *.jpeg *.jpg *.png *.tif *.tiff)"));
    if(filename.isEmpty()) return;
    stackHeld.push(cv::imread(filename.toStdString(),1));
    stackFlying.push(stackHeld.top());
    updateHeld();
    updateFlying();
    stackSide.push(Side::Both);
    ui->listWidget_process->insertItem(0,QString("Open \"%1\"").arg(filename));
}

void DIP::on_actionPop_triggered() {
    if(stackSide.empty()) return;
    if(stackSide.top()&Side::Held) {
        stackHeld.pop();
        updateHeld();
    }
    if(stackSide.top()&Side::Flying) {
        stackFlying.pop();
        updateFlying();
    }
    stackSide.pop();
    delete ui->listWidget_process->takeItem(0);
}

void DIP::on_actionClose_triggered() {
    while(!stackHeld.empty()) stackHeld.pop();
    while(!stackFlying.empty()) stackFlying.pop();
    while(!stackSide.empty()) stackSide.pop();
    ui->listWidget_process->clear();
    updateHeld();
    updateFlying();
}

void DIP::on_actionSaveAs_triggered() {
    if(stackHeld.empty()) return;
    QString filename=QFileDialog::getSaveFileName(
                         this,tr("Save as..."),"",
                         tr("Image (*.bmp *.jpeg *.jpg *.png *.tif *.tiff)"));
    if(filename.isEmpty()) return;
    cv::imwrite(filename.toStdString(),stackHeld.top());
}

void DIP::on_actionHold_triggered() {
    if(stackFlying.empty() || stackFlying.top().data==stackHeld.top().data)
        return;
    stackHeld.push(stackFlying.top());
    updateHeld();
    stackSide.push(Side::Held);
    ui->listWidget_process->insertItem(0,"Hold");
}

QImage cvtMat2QImage(const cv::Mat &mat) {
    /* TODO: QImage::rgbSwapped() failed when image too large
     */
    int width = mat.cols;
    int height = mat.rows;
    int bytesPerLine = static_cast<int>(mat.step);
    cv::Mat cloned=mat.clone();
    switch (mat.type()) {
    // 8bit, ARGB
    case CV_8UC4: {
        QImage image(cloned.data, width, height, bytesPerLine,
                     QImage::Format_ARGB32);
        return image.rgbSwapped();
    }
    // 8bit, BGR
    case CV_8UC3: {
        QImage image(cloned.data, width, height, bytesPerLine,
                     QImage::Format_RGB888);
        //swap blue and red channel
        return image.rgbSwapped();
    }
    // 8bit, Grayscale
    case CV_8UC1: {
        QImage image(cloned.data, width, height, bytesPerLine,
                     QImage::Format_Grayscale8);
        return image;
    }
    default: {
        // Unsupported format
        qWarning() << "Unsupported cv::Mat tpye: " << mat.type()
                   << ", Empty QImage will be returned!";
        return QImage();
    }
    }
}


void DIP::on_actionHis_triggered() {
    cv::Mat res;
    cv::cvtColor(stackFlying.top().clone(),res,cv::COLOR_BGR2GRAY);
    res.convertTo(res,CV_8UC1);
    cv::equalizeHist(res,res);
    stackFlying.push(res);
    stackSide.push(Side::Flying);
    ui->listWidget_process->insertItem(0,"Histogram equalization");
    updateFlying();
}

void DIP::on_actionTrans_triggered() {
    Trans trans(this,stackFlying.top());
    connect(&trans,SIGNAL(updatePreview(cv::Mat)),
            this,SLOT(updatePreview(cv::Mat)));
    if(trans.exec()==QDialog::Accepted) {
        ui->listWidget_process->insertItem(0,"Transformation");
        stackFlying.push(trans.mat);
        stackSide.push(Side::Flying);
    }
    disconnect(&trans,SIGNAL(updatePreview(cv::Mat)),
               this,SLOT(updatePreview(cv::Mat)));
    updateFlying();
}

void DIP::on_actionFilter_triggered() {
    Filter filter(this,stackFlying.top());
    connect(&filter,SIGNAL(updatePreview(cv::Mat)),
            this,SLOT(updatePreview(cv::Mat)));
    if(filter.exec()==QDialog::Accepted) {
        ui->listWidget_process->insertItem(0,"Filter");
        stackFlying.push(filter.mat);
        stackSide.push(Side::Flying);
    }
    disconnect(&filter,SIGNAL(updatePreview(cv::Mat)),
               this,SLOT(updatePreview(cv::Mat)));
    updateFlying();
}
