
#include "AdvancedOpenGL_DockWidget.h"
#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>
#include <QVBoxLayout>

#include "Depth_testng.h"
#include "Stencil_testing.h"
#include "Blending_discard.h"
#include "Blending_sort.h"
#include "Face_culling.h"
#include "Frame_buffers.h"
#include "Frame_buffers_exercise1.h"
#include "Cubemaps_skybox.h"
#include "Cubemaps_environment_mapping.h"
#include "Advanced_GLSL.h"
#include "Geometry_Shader_houses.h"
#include "Geometry_Shader_exploding.h"
#include "Geometry_Shader_normals.h"
#include "Instancing_quads.h"
#include "Asteroids.h"
#include "Asteroids_instanced.h"
#include "Anti_Aliasing_msaa.h"
#include "Anti_aliasing_offscreen.h"

AdvancedOpenGL_DockWidget::AdvancedOpenGL_DockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

AdvancedOpenGL_DockWidget::~AdvancedOpenGL_DockWidget()
{
}
void AdvancedOpenGL_DockWidget::initDockWidget()
{
    DockWidgetName = "AdvancedOpenGL_DockWidget";
    button_list = {"pb_Depth_testng", "pb_Stencil_testing", "pb_Blending_discard", "pb_Blending_sort", "pb_Face_culling", "pb_Frame_buffers", "pb_Frame_buffers_exercise1", "pb_Cubemaps_skybox", "pb_Cubemaps_environment_mapping", "pb_Advanced_GLSL", "pb_Geometry_Shader_houses", "pb_Geometry_Shader_exploding", "pb_Geometry_Shader_normals", "pb_Instancing_quads", "pb_Asteroids", "pb_Asteroids_instanced", "pb_Anti_Aliasing_msaa", "pb_Anti_aliasing_offscreen"};
    button_name_list = {"Depth_testng", "Stencil_testing", "Blending_discard", "Blending_sort", "Face_culling", "Frame_buffers", "Frame_buffers_exercise1", "Cubemaps_skybox", "Cubemaps_environment_mapping", "Advanced_GLSL", "Geometry_Shader_houses", "Geometry_Shader_exploding", "Geometry_Shader_normals", "Instancing_quads", "Asteroids", "Asteroids_instanced", "Anti_Aliasing_msaa", "Anti_aliasing_offscreen"};
    // Initialize your dock widget here
}
void AdvancedOpenGL_DockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void AdvancedOpenGL_DockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
    // Set up your widget layout and style here
}
void AdvancedOpenGL_DockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Depth_testng_clicked);
    connect(example_buttoms[1], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Stencil_testing_clicked);
    connect(example_buttoms[2], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Blending_discard_clicked);
    connect(example_buttoms[3], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Blending_sort_clicked);
    connect(example_buttoms[4], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Face_culling_clicked);
    connect(example_buttoms[5], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Frame_buffers_clicked);
    connect(example_buttoms[6], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Frame_buffers_exercise1_clicked);
    connect(example_buttoms[7], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Cubemaps_skybox_clicked);
    connect(example_buttoms[8], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Cubemaps_environment_mapping_clicked);
    connect(example_buttoms[9], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Advanced_GLSL_clicked);
    connect(example_buttoms[10], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Geometry_Shader_houses_clicked);
    connect(example_buttoms[11], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Geometry_Shader_exploding_clicked);
    connect(example_buttoms[12], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Geometry_Shader_normals_clicked);
    connect(example_buttoms[13], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Instancing_quads_clicked);
    connect(example_buttoms[14], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Asteroids_clicked);
    connect(example_buttoms[15], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Asteroids_instanced_clicked);
    connect(example_buttoms[16], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Anti_Aliasing_msaa_clicked);
    connect(example_buttoms[17], &QPushButton::clicked, this, &AdvancedOpenGL_DockWidget::pb_Anti_aliasing_offscreen_clicked);
}

void AdvancedOpenGL_DockWidget::pb_Depth_testng_clicked()
{
    std::cout << "pb_Depth_testng_clicked" << std::endl;
    pDepth_testng = new Depth_testng(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pDepth_testng);
    refresh_tool_control(pDepth_testng);
}

void AdvancedOpenGL_DockWidget::pb_Stencil_testing_clicked()
{
    std::cout << "pb_Stencil_testing_clicked" << std::endl;
    pStencil_testing = new Stencil_testing(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pStencil_testing);
    refresh_tool_control(pStencil_testing);
}

void AdvancedOpenGL_DockWidget::pb_Blending_discard_clicked()
{
    std::cout << "pb_Blending_discard_clicked" << std::endl;
    pBlending_discard = new Blending_discard(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pBlending_discard);
    refresh_tool_control(pBlending_discard);
}

void AdvancedOpenGL_DockWidget::pb_Blending_sort_clicked()
{
    std::cout << "pb_Blending_sort_clicked" << std::endl;
    pBlending_sort = new Blending_sort(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pBlending_sort);
    refresh_tool_control(pBlending_sort);
}

void AdvancedOpenGL_DockWidget::pb_Face_culling_clicked()
{
    std::cout << "pb_Face_culling_clicked" << std::endl;
    pFace_culling = new Face_culling(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pFace_culling);
    // refresh_tool_control(pFace_culling);
}

void AdvancedOpenGL_DockWidget::pb_Frame_buffers_clicked()
{
    std::cout << "pb_Frame_buffers_clicked" << std::endl;
    pFrame_buffers = new Frame_buffers(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pFrame_buffers);
    refresh_tool_control(pFrame_buffers);
}

void AdvancedOpenGL_DockWidget::pb_Frame_buffers_exercise1_clicked()
{
    std::cout << "pb_Frame_buffers_exercise1_clicked" << std::endl;
    pFrame_buffers_exercise1 = new Frame_buffers_exercise1(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pFrame_buffers_exercise1);
    // refresh_tool_control(pFrame_buffers_exercise1);
}

void AdvancedOpenGL_DockWidget::pb_Cubemaps_skybox_clicked()
{
    std::cout << "pb_Cubemaps_skybox_clicked" << std::endl;
    pCubemaps_skybox = new Cubemaps_skybox(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pCubemaps_skybox);
    // refresh_tool_control(pCubemaps_skybox);
}

void AdvancedOpenGL_DockWidget::pb_Cubemaps_environment_mapping_clicked()
{
    std::cout << "pb_Cubemaps_environment_mapping_clicked" << std::endl;
    pCubemaps_environment_mapping = new Cubemaps_environment_mapping(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pCubemaps_environment_mapping);
    refresh_tool_control(pCubemaps_environment_mapping);
}

void AdvancedOpenGL_DockWidget::pb_Advanced_GLSL_clicked()
{
    std::cout << "pb_Advanced_GLSL_clicked" << std::endl;
    pAdvanced_GLSL = new Advanced_GLSL(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pAdvanced_GLSL);
    // refresh_tool_control(pAdvanced_GLSL);
}

void AdvancedOpenGL_DockWidget::pb_Geometry_Shader_houses_clicked()
{
    std::cout << "pb_Geometry_Shader_houses_clicked" << std::endl;
    pGeometry_Shader_houses = new Geometry_Shader_houses(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pGeometry_Shader_houses);
    // refresh_tool_control(pGeometry_Shader_houses);
}

void AdvancedOpenGL_DockWidget::pb_Geometry_Shader_exploding_clicked()
{
    std::cout << "pb_Geometry_Shader_exploding_clicked" << std::endl;
    pGeometry_Shader_exploding = new Geometry_Shader_exploding(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pGeometry_Shader_exploding);
    // refresh_tool_control(pGeometry_Shader_exploding);
}

void AdvancedOpenGL_DockWidget::pb_Geometry_Shader_normals_clicked()
{
    std::cout << "pb_Geometry_Shader_normals_clicked" << std::endl;
    pGeometry_Shader_normals = new Geometry_Shader_normals(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pGeometry_Shader_normals);
    refresh_tool_control(pGeometry_Shader_normals);
}

void AdvancedOpenGL_DockWidget::pb_Instancing_quads_clicked()
{
    std::cout << "pb_Instancing_quads_clicked" << std::endl;
    pInstancing_quads = new Instancing_quads(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pInstancing_quads);
    // refresh_tool_control(pInstancing_quads);
}

void AdvancedOpenGL_DockWidget::pb_Asteroids_clicked()
{
    std::cout << "pb_Asteroids_clicked" << std::endl;
    pAsteroids = new Asteroids(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pAsteroids);
    // refresh_tool_control(pAsteroids);
}

void AdvancedOpenGL_DockWidget::pb_Asteroids_instanced_clicked()
{
    std::cout << "pb_Asteroids_instanced_clicked" << std::endl;
    pAsteroids_instanced = new Asteroids_instanced(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pAsteroids_instanced);
    refresh_tool_control(pAsteroids_instanced);
}

void AdvancedOpenGL_DockWidget::pb_Anti_Aliasing_msaa_clicked()
{
    Manager::get_instance()->setupSurfaceFormat(8); // 需要在窗口创建前指定精度
    std::cout << "pb_Anti_Aliasing_msaa_clicked" << std::endl;
    pAnti_Aliasing_msaa = new Anti_Aliasing_msaa(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pAnti_Aliasing_msaa);
    // refresh_tool_control(pAnti_Aliasing_msaa);
}

void AdvancedOpenGL_DockWidget::pb_Anti_aliasing_offscreen_clicked()
{
    std::cout << "pb_Anti_aliasing_offscreen_clicked" << std::endl;
    pAnti_aliasing_offscreen = new Anti_aliasing_offscreen(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pAnti_aliasing_offscreen);
    // refresh_tool_control(pAnti_aliasing_offscreen);
}
