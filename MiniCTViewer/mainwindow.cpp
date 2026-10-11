#include "mainwindow.h"
#include "imageview.h"
#include "vtkview.h"

#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QTabWidget>

#include "sliceview.h"
#include <QSlider>
#include <QLabel>

#include "samplevolume.h"

#include "volumeview.h"

MainWindow::MainWindow()
{
    setWindowTitle("Mini CT Viewer");
    resize(800, 600);

    QTabWidget *tabs = new QTabWidget(this);
    setCentralWidget(tabs);

    QWidget *container = new QWidget(tabs);
    tabs->addTab(container, "2D 이미지");

    QWidget *threeDPage = new QWidget(tabs);
    VtkView *vtkView = new VtkView(threeDPage);

    QPushButton *resetViewButton = new QPushButton("3D 시점 초기화", threeDPage);

    QVBoxLayout *threeDLayout = new QVBoxLayout(threeDPage);
    threeDLayout->addWidget(vtkView, 1);
    threeDLayout->addWidget(resetViewButton);

    tabs->addTab(threeDPage, "3D 보기");

    connect(resetViewButton, &QPushButton::clicked, vtkView, [vtkView]() {
        vtkView->resetView();
    });

    QWidget *slicePage = new QWidget(tabs);

    auto sharedVolume = createSampleVolume();
    SliceView *sliceView = new SliceView(sharedVolume, slicePage);

    VolumeView *volumeView = new VolumeView(sharedVolume, tabs);
    tabs->addTab(volumeView, "볼륨 3D");

    QLabel *sliceLabel = new QLabel("단면: 33 / 64", slicePage);

    QSlider *sliceSlider = new QSlider(Qt::Horizontal, slicePage);
    sliceSlider->setRange(0, 63);
    sliceSlider->setValue(32);

    QVBoxLayout *sliceLayout = new QVBoxLayout(slicePage);
    sliceLayout->addWidget(sliceView, 1);
    sliceLayout->addWidget(sliceLabel);
    sliceLayout->addWidget(sliceSlider);

    tabs->addTab(slicePage, "단면 보기");

    connect(sliceSlider, &QSlider::valueChanged,
            sliceView, [sliceView, sliceLabel](int value) {
                sliceView->setSlice(value);

                sliceLabel->setText(
                    QString("단면: %1 / 64").arg(value + 1)
                    );
            });

    QGraphicsScene *scene = new QGraphicsScene(this); // 이미지를 놓을 공간

    ImageView *view = new ImageView(scene, container); // 그 공간을 보여주는 화면
    view->setRenderHint(QPainter::SmoothPixmapTransform);

    view->setDragMode(QGraphicsView::ScrollHandDrag); // 마우스로 잡아서 화면 이동

    QPushButton *openButton = new QPushButton("이미지 선택", container);
    QPushButton *clearButton = new QPushButton("이미지 지우기", container);
    QPushButton *zoomInButton = new QPushButton("확대", container);
    QPushButton *zoomOutButton = new QPushButton("축소", container);
    QPushButton *fitButton = new QPushButton("화면에 맞추기", container);
    QPushButton *originalSizeButton = new QPushButton("원본 크기 (100%)", container);

    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->addWidget(view, 1);
    layout->addWidget(openButton);
    layout->addWidget(clearButton);
    layout->addWidget(zoomInButton);
    layout->addWidget(zoomOutButton);
    layout->addWidget(fitButton);
    layout->addWidget(originalSizeButton);

    auto setImageControlsEnabled = [clearButton, zoomInButton, zoomOutButton, fitButton, originalSizeButton](bool enabled){
        clearButton->setEnabled(enabled);
        zoomInButton->setEnabled(enabled);
        zoomOutButton->setEnabled(enabled);
        fitButton->setEnabled(enabled);
        originalSizeButton->setEnabled(enabled);
    };

    setImageControlsEnabled(false);
    statusBar()->showMessage("이미지를 선택해주세요");

    connect(openButton, &QPushButton::clicked, this, [this, scene, view, setImageControlsEnabled](){
        QString filePath = QFileDialog::getOpenFileName(
            this,
            "이미지 선택",
            QString(),
            "이미지 파일 (*.png *.jpg *.jpeg *.bmp)"
        );

        if(filePath.isEmpty()) return;

        QPixmap image(filePath);

        if(image.isNull()){
            statusBar()->showMessage("이미지를 읽지 못했습니다.");
            return;
        }

        originalImage = image;

        scene->clear();
        scene->addPixmap(originalImage);
        QRectF imageRect = scene->itemsBoundingRect();

        scene->setSceneRect(imageRect.adjusted(
            -imageRect.width() * 100,
            -imageRect.height() * 100,
            imageRect.width() * 100,
            imageRect.height() * 100
        ));

        view->resetTransform();
        view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

        setImageControlsEnabled(true);
        statusBar()->showMessage("확대한 뒤 마우스로 드래그해 보세요");
    });

    connect(zoomInButton, &QPushButton::clicked, this, [view]() {
        view->zoomAtCenter(1.25);
    });

    connect(zoomOutButton, &QPushButton::clicked, this, [view]() {
        view->zoomAtCenter(1.0 / 1.25);
    });

    connect(fitButton, &QPushButton::clicked, this, [scene, view]() {
        view->resetTransform();
        view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    });

    connect(clearButton, &QPushButton::clicked, this, [this, scene,
                                                       view, setImageControlsEnabled]() {
        scene->clear();
        scene->setSceneRect(QRectF());

        originalImage = QPixmap();
        view->resetTransform();

        setImageControlsEnabled(false);
        statusBar()->showMessage("이미지를 지웠습니다.");
    });

    connect(originalSizeButton, &QPushButton::clicked, this, [this, scene, view]() {
        view->resetTransform();
        view->centerOn(scene->itemsBoundingRect().center());

        statusBar()->showMessage("배율: 100%");
    });

}


