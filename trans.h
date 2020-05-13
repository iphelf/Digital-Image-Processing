#ifndef TRANSNONLINEAR_H
#define TRANSNONLINEAR_H

#include <QDialog>
#include <opencv2/opencv.hpp>

namespace Ui {
class Trans;
}

class Trans : public QDialog {
    Q_OBJECT

  public:
    explicit Trans(QWidget *parent,cv::Mat mat);
    ~Trans();
    cv::Mat process();
  private slots:
    void updated();
    void on_Trans_accepted();
  signals:
    void updatePreview(const cv::Mat &mat);
  public:
    cv::Mat mat;
  private:
    Ui::Trans *ui;
};

#endif // TRANSNONLINEAR_H
