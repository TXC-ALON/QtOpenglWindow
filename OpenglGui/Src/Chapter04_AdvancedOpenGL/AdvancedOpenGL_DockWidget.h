
#pragma once

#include <QDockWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QObject>
#include "glDockWidget.h"

class Depth_testng;
class Stencil_testing;
class Blending_discard;
class Blending_sort;
class Face_culling;
class Frame_buffers;
class Frame_buffers_exercise1;
class Cubemaps_skybox;
class Cubemaps_environment_mapping;
class Advanced_GLSL;
class Geometry_Shader_houses;
class Geometry_Shader_exploding;
class Geometry_Shader_normals;
class Instancing_quads;
class Asteroids;
class Asteroids_instanced;
class Anti_Aliasing_msaa;
class Anti_aliasing_offscreen;

class AdvancedOpenGL_DockWidget : public glDockWidget
{
    Q_OBJECT
public:
    AdvancedOpenGL_DockWidget(QWidget *parent = nullptr);
    ~AdvancedOpenGL_DockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    Depth_testng *pDepth_testng = nullptr;
    Stencil_testing *pStencil_testing = nullptr;
    Blending_discard *pBlending_discard = nullptr;
    Blending_sort *pBlending_sort = nullptr;
    Face_culling *pFace_culling = nullptr;
    Frame_buffers *pFrame_buffers = nullptr;
    Frame_buffers_exercise1 *pFrame_buffers_exercise1 = nullptr;
    Cubemaps_skybox *pCubemaps_skybox = nullptr;
    Cubemaps_environment_mapping *pCubemaps_environment_mapping = nullptr;
    Advanced_GLSL *pAdvanced_GLSL = nullptr;
    Geometry_Shader_houses *pGeometry_Shader_houses = nullptr;
    Geometry_Shader_exploding *pGeometry_Shader_exploding = nullptr;
    Geometry_Shader_normals *pGeometry_Shader_normals = nullptr;
    Instancing_quads *pInstancing_quads = nullptr;
    Asteroids *pAsteroids = nullptr;
    Asteroids_instanced *pAsteroids_instanced = nullptr;
    Anti_Aliasing_msaa *pAnti_Aliasing_msaa = nullptr;
    Anti_aliasing_offscreen *pAnti_aliasing_offscreen = nullptr;
private slots:
    void pb_Depth_testng_clicked();
    void pb_Stencil_testing_clicked();
    void pb_Blending_discard_clicked();
    void pb_Blending_sort_clicked();
    void pb_Face_culling_clicked();
    void pb_Frame_buffers_clicked();
    void pb_Frame_buffers_exercise1_clicked();
    void pb_Cubemaps_skybox_clicked();
    void pb_Cubemaps_environment_mapping_clicked();
    void pb_Advanced_GLSL_clicked();
    void pb_Geometry_Shader_houses_clicked();
    void pb_Geometry_Shader_exploding_clicked();
    void pb_Geometry_Shader_normals_clicked();
    void pb_Instancing_quads_clicked();
    void pb_Asteroids_clicked();
    void pb_Asteroids_instanced_clicked();
    void pb_Anti_Aliasing_msaa_clicked();
    void pb_Anti_aliasing_offscreen_clicked();
};


