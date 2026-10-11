#include "mainwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QPixmap> // 이미지를 다루는 클래스
#include <QStatusBar>

MainWindow::MainWindow()
{
    setWindowTitle("First CT Viwer");
    resize(800, 600);

    statusBar()->showMessage("Ready Selecting image...");

    QWidget *container = new QWidget(this);
    setCentralWidget(container);

    QLabel *label = new QLabel("CT Video is showed here", container);
    label->setAlignment(Qt::AlignHCenter);

    QPushButton *button = new QPushButton("Select File", container);
    QPushButton *clearButton = new QPushButton("Remove File", container);
    clearButton->setEnabled(false);

    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->addWidget(label);
    layout->addWidget(button);
    layout->addWidget(clearButton);

    connect(button, &QPushButton::clicked, label, [this, label, clearButton]() {
        QString filePath = QFileDialog::getOpenFileName(
            this,
            "Slecet Image File",
            QString(),
            "이미지 파일 (*.png *.jpg *.jpeg *.bmp)"
        );

        if(filePath.isEmpty()){
            return;
        }

        QPixmap image(filePath);

        if(image.isNull()){
            label->setText("Can't upload Image");
            clearButton->setEnabled(false);

            statusBar()->showMessage("Failed Upload Image");
            return;
        }

        QPixmap resizedImage = image.scaled(
            600,
            400,
            Qt::KeepAspectRatio, // 원래 가로 세로 비율 유지
            Qt::SmoothTransformation // 크기 바꿀 때 부드럽게 처리
        );

        label->setPixmap(resizedImage);
        clearButton->setEnabled(true);

        statusBar()->showMessage("Complete Upload Image");
    });

    connect(clearButton, &QPushButton::clicked, label, [this, label, clearButton]() {
        label->clear();
        label->setText("Selcet Image");
        clearButton->setEnabled(false);

        statusBar()->showMessage("Remove Image");
    });
}














