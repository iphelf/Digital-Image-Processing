#include "trans.h"
#include "ui_trans.h"

Trans::Trans(QWidget *parent,cv::Mat mat) :
    QDialog(parent),mat(mat),
    ui(new Ui::Trans) {
    ui->setupUi(this);
    connect(ui->doubleSpinBox_pow,SIGNAL(valueChanged(double)),
            this,SLOT(updated()));
    connect(ui->doubleSpinBox_root,SIGNAL(valueChanged(double)),
            this,SLOT(updated()));
    connect(ui->radioButton_i,SIGNAL(clicked(bool)),
            this,SLOT(updated()));
    connect(ui->radioButton_n,SIGNAL(clicked(bool)),
            this,SLOT(updated()));
    connect(ui->radioButton_log,SIGNAL(clicked(bool)),
            this,SLOT(updated()));
    connect(ui->radioButton_exp,SIGNAL(clicked(bool)),
            this,SLOT(updated()));
    connect(ui->radioButton_pow,SIGNAL(clicked(bool)),
            this,SLOT(updated()));
    connect(ui->radioButton_root,SIGNAL(clicked(bool)),
            this,SLOT(updated()));
    connect(ui->checkBox_preview,SIGNAL(clicked(bool)),
            this,SLOT(updated()));
}

Trans::~Trans() {
    delete ui;
}

cv::Mat Trans::process() {
    cv::Mat res;
    mat.convertTo(res,CV_64FC3);
    cv::normalize(res,res,1,0,cv::NORM_MINMAX);
    if(ui->radioButton_i->isChecked()) {
        ;
    } else if(ui->radioButton_n->isChecked()) {
        res=cv::Scalar::all(1)-res;
    } else if(ui->radioButton_log->isChecked()) {
        cv::log(res+cv::Scalar::all(1),res);
        res=res/cv::Scalar::all(cv::log(2));
    } else if(ui->radioButton_exp->isChecked()) {
        cv::exp(res.mul(cv::Scalar::all(cv::log(2))),res);
        res=res-cv::Scalar::all(1);
    } else if(ui->radioButton_pow->isChecked()) {
        double n=ui->doubleSpinBox_pow->value();
        cv::pow(res,n,res);
    } else if(ui->radioButton_root->isChecked()) {
        double n=ui->doubleSpinBox_root->value();
        cv::pow(res,1/n,res);
    }
    cv::normalize(res,res,255,0,cv::NORM_MINMAX);
    res.convertTo(res,CV_8UC3);
    return res;
}

void Trans::updated() {
    if(!ui->checkBox_preview->isChecked()) return;
    emit updatePreview(process());
}

void Trans::on_Trans_accepted() {
    mat=process();
}
