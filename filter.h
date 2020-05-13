#ifndef FILTER_H
#define FILTER_H

#include <QDialog>
#include <opencv2/opencv.hpp>

namespace Ui {
class Filter;
}

class Filter : public QDialog {
    Q_OBJECT

  public:
    explicit Filter(QWidget *parent, cv::Mat mat);
    ~Filter();
    cv::Mat process();
  private slots:
    void updated();
    void on_Filter_accepted();
  signals:
    void updatePreview(const cv::Mat &mat);
  public:
    cv::Mat mat;
  private:
    Ui::Filter *ui;
};

#endif // FILTER_H
