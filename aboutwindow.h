#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>
#include <QDesktopServices>
#include <QMediaPlayer>

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
    void CUDASlot();
private:
    Ui::AboutWindow *ui;
    QMediaPlayer *Music;
    void SetLogo();
    void SetInformation();
};


#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#endif // ABOUTWINDOW_H
