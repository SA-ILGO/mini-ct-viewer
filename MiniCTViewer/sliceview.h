#ifndef SLICEVIEW_H
#define SLICEVIEW_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>

class vtkImageData;
class vtkImageActor;
class vtkRenderer;

class SliceView : public QVTKOpenGLNativeWidget
{
public:
    explicit SliceView(
        vtkSmartPointer<vtkImageData> data,
        QWidget *parent = nullptr
    );
    ~SliceView() override;

    void setSlice(int index);

private:
    vtkSmartPointer<vtkImageData> volume;
    vtkSmartPointer<vtkImageActor> imageActor;
    vtkSmartPointer<vtkRenderer> renderer;
};

#endif // SLICEVIEW_H
