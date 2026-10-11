#ifndef VTKVIEW_H
#define VTKVIEW_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>

class vtkRenderer;

class VtkView : public QVTKOpenGLNativeWidget
{
public:
    explicit VtkView(QWidget *parent = nullptr);

    void resetView();

private:
    vtkSmartPointer<vtkRenderer> renderer;
};

#endif // VTKVIEW_H
