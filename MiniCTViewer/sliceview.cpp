#include "sliceview.h"

#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageProperty.h>
#include <vtkNew.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkType.h>

SliceView::SliceView(QWidget *parent)
    : QVTKOpenGLNativeWidget(parent)
{
    vtkNew<vtkGenericOpenGLRenderWindow> window;
    setRenderWindow(window);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    window->AddRenderer(renderer);
    renderer->SetBackground(0.12, 0.15, 0.20);

    // 1. 64 × 64 × 64 크기의 입체 영상 만들기
    volume = vtkSmartPointer<vtkImageData>::New();
    volume->SetDimensions(64, 64, 64);
    volume->SetSpacing(1.0, 1.0, 1.0);
    volume->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

    // 2. 구 안쪽은 밝게, 바깥쪽은 어둡게 채우기
    for (int z = 0; z < 64; ++z) {
        for (int y = 0; y < 64; ++y) {
            for (int x = 0; x < 64; ++x) {
                double dx = x - 31.5;
                double dy = y - 31.5;
                double dz = z - 31.5;

                double distanceSquared =
                    dx * dx + dy * dy + dz * dz;

                double value =
                    (distanceSquared <= 24.0 * 24.0) ? 220.0 : 20.0;

                volume->SetScalarComponentFromDouble(
                    x, y, z, 0, value
                    );
            }
        }
    }

    // 3. 입체 영상에서 한 단면만 표시
    imageActor = vtkSmartPointer<vtkImageActor>::New();
    imageActor->SetInputData(volume);
    imageActor->SetDisplayExtent(0, 63, 0, 63, 32, 32);
    imageActor->InterpolateOff();

    imageActor->GetProperty()->SetColorWindow(255.0);
    imageActor->GetProperty()->SetColorLevel(127.5);

    renderer->AddActor(imageActor);

    // 4. 단면을 정면에서 보기
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