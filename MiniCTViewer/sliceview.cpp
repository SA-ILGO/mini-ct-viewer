#include "sliceview.h"

#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageProperty.h>
#include <vtkNew.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

SliceView::SliceView(
    vtkSmartPointer<vtkImageData> data,
    QWidget *parent
    )
    : QVTKOpenGLNativeWidget(parent)
    , volume(data)
{
    vtkNew<vtkGenericOpenGLRenderWindow> window;
    setRenderWindow(window);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    window->AddRenderer(renderer);
    renderer->SetBackground(0.12, 0.15, 0.20);

    // 전달받은 공유 입체 영상에서 한 단면만 표시
    imageActor = vtkSmartPointer<vtkImageActor>::New();
    imageActor->SetInputData(volume);
    imageActor->SetDisplayExtent(0, 63, 0, 63, 32, 32);
    imageActor->InterpolateOff();

    imageActor->GetProperty()->SetColorWindow(255.0);
    imageActor->GetProperty()->SetColorLevel(127.5);

    renderer->AddActor(imageActor);

    // 단면을 정면에서 보기
    vtkCamera *camera = renderer->GetActiveCamera();
    camera->SetPosition(31.5, 31.5, 132.0);
    camera->SetFocalPoint(31.5, 31.5, 32.0);
    camera->SetViewUp(0.0, 1.0, 0.0);
    camera->ParallelProjectionOn();

    renderer->ResetCamera();

    // 이번 단계에서는 슬라이더로만 조작
    interactor()->SetInteractorStyle(nullptr);
}

SliceView::~SliceView() = default;

void SliceView::setSlice(int index)
{
    if (index < 0 || index >= 64) {
        return;
    }

    imageActor->SetDisplayExtent(0, 63, 0, 63, index, index);

    renderer->ResetCameraClippingRange();
    renderWindow()->Render();
}
