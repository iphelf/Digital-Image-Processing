#ifndef DIP_H
#define DIP_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui {
class DIP;
}
QT_END_NAMESPACE

class DIP : public QMainWindow {
    Q_OBJECT

    enum Side {Held=1,Flying=2,Both=3};

  public:
    DIP(QWidget *parent = nullptr);
    ~DIP();

    void updateHeld();
    void updateFlying();

  public slots:
    void updatePreview(cv::Mat mat);

  private slots:
    void on_actionOpen_triggered();
    void on_actionPop_triggered();
    void on_actionClose_triggered();
    void on_actionSaveAs_triggered();
    void on_actionHold_triggered();
    void on_actionHis_triggered();
    void on_actionTrans_triggered();

    void on_actionFilter_triggered();

  private:
    Ui::DIP *ui;
    std::stack<cv::Mat> stackHeld;
    std::stack<cv::Mat> stackFlying;
    std::stack<Side> stackSide;
};

QImage cvtMat2QImage(const cv::Mat &mat);

#endif // DIP_H
