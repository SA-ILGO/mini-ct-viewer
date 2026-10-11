#ifndef VOLUMEVIEW_H
#define VOLUMEVIEW_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>

class vtkImageData;

class VolumeView : public QVTKOpenGLNativeWidget
{
public:
    explicit VolumeView(
        vtkSmartPointer<vtkImageData> data,
        QWidget *parent = nullptr
        );
};

#endif // VOLUMEVIEW_H