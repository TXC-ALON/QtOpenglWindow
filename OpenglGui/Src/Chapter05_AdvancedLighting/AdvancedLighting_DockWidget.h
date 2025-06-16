
#pragma once

#include <QDockWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QObject>
#include "glDockWidget.h"

class Advanced_lighting;
class Gamma_correction;
class Shadow_mapping_depth;
class Shadow_mapping_base;
class Shadow_mapping;
class Point_shadows;
class Point_shadows_soft;
class Normal_mapping;
class Parallax_mapping;
class Steep_parallax_mapping;
class Parallax_occlusion_mapping;
class HDR;
class Bloom;
class Deferred_shading;
class Deferred_shading_volumes;
class SSAO;

class AdvancedLighting_DockWidget : public glDockWidget
{
    Q_OBJECT
public:
    AdvancedLighting_DockWidget(QWidget *parent = nullptr);
    ~AdvancedLighting_DockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    Advanced_lighting *pAdvanced_lighting = nullptr;
    Gamma_correction *pGamma_correction = nullptr;
    Shadow_mapping_depth *pShadow_mapping_depth = nullptr;
    Shadow_mapping_base *pShadow_mapping_base = nullptr;
    Shadow_mapping *pShadow_mapping = nullptr;
    Point_shadows *pPoint_shadows = nullptr;
    Point_shadows_soft *pPoint_shadows_soft = nullptr;
    Normal_mapping *pNormal_mapping = nullptr;
    Parallax_mapping *pParallax_mapping = nullptr;
    Steep_parallax_mapping *pSteep_parallax_mapping = nullptr;
    Parallax_occlusion_mapping *pParallax_occlusion_mapping = nullptr;
    HDR *pHDR = nullptr;
    Bloom *pBloom = nullptr;
    Deferred_shading *pDeferred_shading = nullptr;
    Deferred_shading_volumes *pDeferred_shading_volumes = nullptr;
    SSAO *pSSAO = nullptr;
private slots:
    void pb_Advanced_lighting_clicked();
    void pb_Gamma_correction_clicked();
    void pb_Shadow_mapping_depth_clicked();
    void pb_Shadow_mapping_base_clicked();
    void pb_Shadow_mapping_clicked();
    void pb_Point_shadows_clicked();
    void pb_Point_shadows_soft_clicked();
    void pb_Normal_mapping_clicked();
    void pb_Parallax_mapping_clicked();
    void pb_Steep_parallax_mapping_clicked();
    void pb_Parallax_occlusion_mapping_clicked();
    void pb_HDR_clicked();
    void pb_Bloom_clicked();
    void pb_Deferred_shading_clicked();
    void pb_Deferred_shading_volumes_clicked();
    void pb_SSAO_clicked();
};


