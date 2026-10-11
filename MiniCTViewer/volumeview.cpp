#include "volumeview.h"

#include <vtkColorTransferFunction.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkImageData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNew.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

VolumeView::VolumeView(
    vtkSmartPointer<vtkImageData> data,
    QWidget *parent
    )
    : QVTKOpenGLNativeWidget(parent)
{
    vtkNew<vtkGenericOpenGLRenderWindow> window;
    setRenderWindow(window);

    vtkNew<vtkRenderer> renderer;
    window->AddRenderer(renderer);
    renderer->SetBackground(0.12, 0.15, 0.20);

    // 1. 받은 입체 데이터를 mapper에 연결
    vtkNew<vtkGPUVolumeRayCastMapper> mapper;
    mapper->SetInputData(data);
    mapper->SetBlendModeToComposite();

    // 2. 데이터 값에 따른 색상
    vtkNew<vtkColorTransferFunction> colors;
    colors->AddRGBPoint(20.0, 0.0, 0.0, 0.0);
    colors->AddRGBPoint(220.0, 0.9, 0.85, 0.75);

    // 3. 데이터 값에 따른 불투명도
    vtkNew<vtkPiecewiseFunction> opacity;
    opacity->AddPoint(20.0, 0.0);
    opacity->AddPoint(100.0, 0.0);
    opacity->AddPoint(220.0, 0.8);

    // 4. 색상과 불투명도를 속성으로 묶기
    vtkNew<vtkVolumeProperty> property;
    property->SetColor(colors);
    property->SetScalarOpacity(opacity);
    property->SetInterpolationTypeToLinear();
    property->ShadeOn();

    // 5. 장면에 놓을 볼륨 만들기
    vtkNew<vtkVolume> volumeActor;
    volumeActor->SetMapper(mapper);
    volumeActor->SetProperty(property);

    renderer->AddVolume(volumeActor);
    renderer->ResetCamera();

    // 6. 마우스로 카메라 조작
    vtkNew<vtkInteractorStyleTrackballCamera> style;
    interactor()->SetInteractorStyle(style);
}