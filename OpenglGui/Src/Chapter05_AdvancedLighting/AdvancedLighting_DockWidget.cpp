
#include "AdvancedLighting_DockWidget.h"
#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>
#include <QVBoxLayout>

#include "Advanced_lighting.h"
#include "Gamma_correction.h"
#include "Shadow_mapping_depth.h"
#include "Shadow_mapping_base.h"
#include "Shadow_mapping.h"
#include "Point_shadows.h"
#include "Point_shadows_soft.h"
#include "Normal_mapping.h"
#include "Parallax_mapping.h"
#include "Steep_parallax_mapping.h"
#include "Parallax_occlusion_mapping.h"
#include "HDR.h"
#include "Bloom.h"
#include "Deferred_shading.h"
#include "Deferred_shading_volumes.h"
#include "SSAO.h"

AdvancedLighting_DockWidget::AdvancedLighting_DockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

AdvancedLighting_DockWidget::~AdvancedLighting_DockWidget()
{
}
void AdvancedLighting_DockWidget::initDockWidget()
{
    DockWidgetName = "AdvancedLighting_DockWidget";
    button_list = {"pb_Advanced_lighting", "pb_Gamma_correction", "pb_Shadow_mapping_depth", "pb_Shadow_mapping_base", "pb_Shadow_mapping", "pb_Point_shadows", "pb_Point_shadows_soft", "pb_Normal_mapping", "pb_Parallax_mapping", "pb_Steep_parallax_mapping", "pb_Parallax_occlusion_mapping", "pb_HDR", "pb_Bloom", "pb_Deferred_shading", "pb_Deferred_shading_volumes", "pb_SSAO"};
    button_name_list = {"Advanced_lighting", "Gamma_correction", "Shadow_mapping_depth", "Shadow_mapping_base", "Shadow_mapping", "Point_shadows", "Point_shadows_soft", "Normal_mapping", "Parallax_mapping", "Steep_parallax_mapping", "Parallax_occlusion_mapping", "HDR", "Bloom", "Deferred_shading", "Deferred_shading_volumes", "SSAO"};
    // Initialize your dock widget here
}
void AdvancedLighting_DockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void AdvancedLighting_DockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
    // Set up your widget layout and style here
}
void AdvancedLighting_DockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Advanced_lighting_clicked);
    connect(example_buttoms[1], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Gamma_correction_clicked);
    connect(example_buttoms[2], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Shadow_mapping_depth_clicked);
    connect(example_buttoms[3], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Shadow_mapping_base_clicked);
    connect(example_buttoms[4], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Shadow_mapping_clicked);
    connect(example_buttoms[5], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Point_shadows_clicked);
    connect(example_buttoms[6], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Point_shadows_soft_clicked);
    connect(example_buttoms[7], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Normal_mapping_clicked);
    connect(example_buttoms[8], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Parallax_mapping_clicked);
    connect(example_buttoms[9], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Steep_parallax_mapping_clicked);
    connect(example_buttoms[10], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Parallax_occlusion_mapping_clicked);
    connect(example_buttoms[11], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_HDR_clicked);
    connect(example_buttoms[12], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Bloom_clicked);
    connect(example_buttoms[13], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Deferred_shading_clicked);
    connect(example_buttoms[14], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_Deferred_shading_volumes_clicked);
    connect(example_buttoms[15], &QPushButton::clicked, this, &AdvancedLighting_DockWidget::pb_SSAO_clicked);
}


void AdvancedLighting_DockWidget::pb_Advanced_lighting_clicked()
{
    std::cout << "pb_Advanced_lighting_clicked" << std::endl;
    pAdvanced_lighting = new Advanced_lighting(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pAdvanced_lighting);
    // refresh_tool_control(pAdvanced_lighting); 
}


void AdvancedLighting_DockWidget::pb_Gamma_correction_clicked()
{
    std::cout << "pb_Gamma_correction_clicked" << std::endl;
    pGamma_correction = new Gamma_correction(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pGamma_correction);
    // refresh_tool_control(pGamma_correction); 
}


void AdvancedLighting_DockWidget::pb_Shadow_mapping_depth_clicked()
{
    std::cout << "pb_Shadow_mapping_depth_clicked" << std::endl;
    pShadow_mapping_depth = new Shadow_mapping_depth(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pShadow_mapping_depth);
    // refresh_tool_control(pShadow_mapping_depth); 
}


void AdvancedLighting_DockWidget::pb_Shadow_mapping_base_clicked()
{
    std::cout << "pb_Shadow_mapping_base_clicked" << std::endl;
    pShadow_mapping_base = new Shadow_mapping_base(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pShadow_mapping_base);
    // refresh_tool_control(pShadow_mapping_base); 
}


void AdvancedLighting_DockWidget::pb_Shadow_mapping_clicked()
{
    std::cout << "pb_Shadow_mapping_clicked" << std::endl;
    pShadow_mapping = new Shadow_mapping(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pShadow_mapping);
    // refresh_tool_control(pShadow_mapping); 
}


void AdvancedLighting_DockWidget::pb_Point_shadows_clicked()
{
    std::cout << "pb_Point_shadows_clicked" << std::endl;
    pPoint_shadows = new Point_shadows(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pPoint_shadows);
    // refresh_tool_control(pPoint_shadows); 
}


void AdvancedLighting_DockWidget::pb_Point_shadows_soft_clicked()
{
    std::cout << "pb_Point_shadows_soft_clicked" << std::endl;
    pPoint_shadows_soft = new Point_shadows_soft(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pPoint_shadows_soft);
    // refresh_tool_control(pPoint_shadows_soft); 
}


void AdvancedLighting_DockWidget::pb_Normal_mapping_clicked()
{
    std::cout << "pb_Normal_mapping_clicked" << std::endl;
    pNormal_mapping = new Normal_mapping(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pNormal_mapping);
    // refresh_tool_control(pNormal_mapping); 
}


void AdvancedLighting_DockWidget::pb_Parallax_mapping_clicked()
{
    std::cout << "pb_Parallax_mapping_clicked" << std::endl;
    pParallax_mapping = new Parallax_mapping(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pParallax_mapping);
    // refresh_tool_control(pParallax_mapping); 
}


void AdvancedLighting_DockWidget::pb_Steep_parallax_mapping_clicked()
{
    std::cout << "pb_Steep_parallax_mapping_clicked" << std::endl;
    pSteep_parallax_mapping = new Steep_parallax_mapping(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pSteep_parallax_mapping);
    // refresh_tool_control(pSteep_parallax_mapping); 
}


void AdvancedLighting_DockWidget::pb_Parallax_occlusion_mapping_clicked()
{
    std::cout << "pb_Parallax_occlusion_mapping_clicked" << std::endl;
    pParallax_occlusion_mapping = new Parallax_occlusion_mapping(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pParallax_occlusion_mapping);
    // refresh_tool_control(pParallax_occlusion_mapping); 
}


void AdvancedLighting_DockWidget::pb_HDR_clicked()
{
    std::cout << "pb_HDR_clicked" << std::endl;
    pHDR = new HDR(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pHDR);
    // refresh_tool_control(pHDR); 
}


void AdvancedLighting_DockWidget::pb_Bloom_clicked()
{
    std::cout << "pb_Bloom_clicked" << std::endl;
    pBloom = new Bloom(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pBloom);
    // refresh_tool_control(pBloom); 
}


void AdvancedLighting_DockWidget::pb_Deferred_shading_clicked()
{
    std::cout << "pb_Deferred_shading_clicked" << std::endl;
    pDeferred_shading = new Deferred_shading(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pDeferred_shading);
    // refresh_tool_control(pDeferred_shading); 
}


void AdvancedLighting_DockWidget::pb_Deferred_shading_volumes_clicked()
{
    std::cout << "pb_Deferred_shading_volumes_clicked" << std::endl;
    pDeferred_shading_volumes = new Deferred_shading_volumes(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pDeferred_shading_volumes);
    // refresh_tool_control(pDeferred_shading_volumes); 
}


void AdvancedLighting_DockWidget::pb_SSAO_clicked()
{
    std::cout << "pb_SSAO_clicked" << std::endl;
    pSSAO = new SSAO(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pSSAO);
    // refresh_tool_control(pSSAO); 
}

