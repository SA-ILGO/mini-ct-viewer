#include "vtkview.h"

#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkRenderer.h>

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>

VtkView::VtkView(QWidget *parent) : QVTKOpenGLNativeWidget(parent)
{
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    setRenderWindow(renderWindow);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    renderer->SetBackground(0.12, 0.15, 0.20);

    // 1. 구의 모양 만들기
    vtkNew<vtkSphereSource> sphere;
    sphere->SetRadius(1.0);
    sphere->SetThetaResolution(20);
    sphere->SetPhiResolution(20);

    // 2. 구 데이터를 mapper에 연결
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(sphere->GetOutputPort());

    // 3. 장면에 놓을 물체 만들기
    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0.85, 0.90, 0.95);

    actor->GetProperty()->EdgeVisibilityOn();
    actor->GetProperty()->SetEdgeColor(0.2, 0.3, 0.4);

    // 4. 물체를 장면에 추가하고 카메라 맞추기
    renderer->AddActor(actor);
    renderer->ResetCamera();

    vtkNew<vtkInteractorStyleTrackballCamera> style;
    interactor()->SetInteractorStyle(style);
}

void VtkView::resetView()
{
    vtkCamera *camera = renderer->GetActiveCamera();

    camera->SetFocalPoint(0.0, 0.0, 1.0);
    camera->SetPosition(0.0, 1.0, 0.0);
    camera->SetViewUp(0.0, 0.0, 0.0);

    renderer->ResetCamera();
    renderer->ResetCameraClippingRange();

    renderWindow()->Render();
}