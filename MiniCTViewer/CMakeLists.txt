cmake_minimum_required(VERSION 3.19)
project(MiniCTViewer LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

set(VTK_DIR "C:/Users/user/Documents/VTK-9.7.1/build-release")

find_package(VTK REQUIRED COMPONENTS
    CommonCore
    FiltersSources
    RenderingCore
    RenderingOpenGL2
    InteractionStyle
    GUISupportQt
)

qt_standard_project_setup()

qt_add_executable(MiniCTViewer
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    imageview.h
    imageview.cpp
    vtkview.h
    vtkview.cpp
)

target_link_libraries(MiniCTViewer
    PRIVATE
        Qt::Core
        Qt::Widgets
        ${VTK_LIBRARIES}
)

vtk_module_autoinit(
    TARGETS MiniCTViewer
    MODULES ${VTK_LIBRARIES}
)

install(TARGETS MiniCTViewer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MiniCTViewer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
