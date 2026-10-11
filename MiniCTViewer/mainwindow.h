#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

class MainWindow : public QMainWindow{
public:
    MainWindow();

private:
    QPixmap originalImage;
};

#endif
