#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>
#include <QDesktopServices>
#include <QMediaPlayer>
#include <opencv2/opencv.hpp>

namespace Ui {
class AboutWindow;
}

class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = 0);
    ~AboutWindow();

private slots:
    void OpenCVSlot();
    void FeedBackSlot();
    void OpenSourceSlot();

private:
    Ui::AboutWindow *ui;
    QMediaPlayer *Music;
    void SetLogo();
    void SetInformation();

};

#endif // ABOUTWINDOW_H
