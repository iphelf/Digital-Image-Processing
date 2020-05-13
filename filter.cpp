#include "filter.h"
#include "ui_filter.h"
#include <iostream>

Filter::Filter(QWidget *parent,cv::Mat mat) :
    QDialog(parent),mat(mat),
    ui(new Ui::Filter) {
    ui->setupUi(this);
    connect(ui->radioButton_mean,SIGNAL(clicked(bool)),
            this,SLOT(updated()));
    connect(ui->radioButton_median,SIGNAL(clicked(bool)),
            this,SLOT(updated()));
    connect(ui->radioButton_laplacian,SIGNAL(clicked(bool)),
            this,SLOT(updated()));
    connect(ui->spinBox_size,SIGNAL(valueChanged(int)),
            this,SLOT(updated()));
    connect(ui->horizontalSlider_size,SIGNAL(valueChanged(int)),
            this,SLOT(updated()));
    connect(ui->checkBox_preview,SIGNAL(clicked(bool)),
            this,SLOT(updated()));
}

Filter::~Filter() {
    delete ui;
}

cv::Mat Filter::process() {
    cv::Mat res=mat.clone();
    if(ui->radioButton_mean->isChecked()) {
        int n=ui->spinBox_size->value();
        cv::blur(res,res,cv::Size(n,n));
    } else if(ui->radioButton_median->isChecked()) {
        int n=ui->spinBox_size->value();
        cv::medianBlur(res,res,n);
    } else if(ui->radioButton_laplacian->isChecked()) {
        cv::Mat mask=res.clone();
        cv::Laplacian(mask,mask,CV_8UC3);
        cv::subtract(res,mask,res);
    }
    return res;
}

void Filter::updated() {
    if(!ui->checkBox_preview->isChecked()) return;
    emit updatePreview(process());
}

void Filter::on_Filter_accepted() {
    mat=process();
}
