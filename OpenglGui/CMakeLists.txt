# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
### GladImGui ###
SET (ProName OpenglGui)
project(${ProName} LANGUAGES CXX)
find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)
qt_standard_project_setup()

set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/OpenglGui/3rd/glm")
set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/OpenglGui/3rd/assimp")

message("GLM_INCLUDE_DIR is ${GLM_INCLUDE_DIR}")
message("ASSIMP_INCLUDE_DIR is ${ASSIMP_INCLUDE_DIR}")

INCLUDE_DIRECTORIES(
        ${GLM_INCLUDE_DIR}
        ${ASSIMP_INCLUDE_DIR}
        Src
        Src/utils
        Src/MyUtils
)




SET(SRC
        Src/Chapter_headers.h
        Src/main.cpp
        Src/openglwindow.h
        Src/openglwindow.cpp
        Src/mainwindow.h
        Src/mainwindow.cpp
        Src/Manager.h
        Src/Manager.cpp
        Src/glWidget.h
        Src/glWidget.cpp
        Src/glDockWidget.h
        Src/glDockWidget.cpp
        Src/Utils_Widget.h
        Src/Utils_Widget.cpp


        #第一章 入门
        #2.1 三角形triangle
        Src/Chapter01_Bird/2.1hello_triangle/HelloTriangleDockWidget.h
        Src/Chapter01_Bird/2.1hello_triangle/HelloTriangleDockWidget.cpp
        Src/Chapter01_Bird/2.1hello_triangle/HelloTriangle.h
        Src/Chapter01_Bird/2.1hello_triangle/HelloTriangle.cpp
        Src/Chapter01_Bird/2.1hello_triangle/HelloTriangleIndexed.h
        Src/Chapter01_Bird/2.1hello_triangle/HelloTriangleIndexed.cpp
        Src/Chapter01_Bird/2.1hello_triangle/HelloTriangleExercise1.h
        Src/Chapter01_Bird/2.1hello_triangle/HelloTriangleExercise1.cpp
        Src/Chapter01_Bird/2.1hello_triangle/HelloTriangleExercise2.h
        Src/Chapter01_Bird/2.1hello_triangle/HelloTriangleExercise2.cpp
        Src/Chapter01_Bird/2.1hello_triangle/HelloTriangleExercise3.h
        Src/Chapter01_Bird/2.1hello_triangle/HelloTriangleExercise3.cpp
        #3.1 着色器shader
        Src/Chapter01_Bird/3.1shaders_interpolation/ShadersInterpolation.h
        Src/Chapter01_Bird/3.1shaders_interpolation/ShadersInterpolation.cpp
        Src/Chapter01_Bird/3.1shaders_interpolation/ShadersDockWidget.h
        Src/Chapter01_Bird/3.1shaders_interpolation/ShadersDockWidget.cpp
        #4.1 纹理Texture
        Src/Chapter01_Bird/4.1texture/TextureDockWidget.h
        Src/Chapter01_Bird/4.1texture/TextureDockWidget.cpp
        Src/Chapter01_Bird/4.1texture/Textures.h
        Src/Chapter01_Bird/4.1texture/Textures.cpp
        Src/Chapter01_Bird/4.1texture/TextureWidget.h
        Src/Chapter01_Bird/4.1texture/TextureWidget.cpp
        Src/Chapter01_Bird/4.1texture/TexturesExercise2.h
        Src/Chapter01_Bird/4.1texture/TexturesExercise2.cpp
        Src/Chapter01_Bird/4.1texture/TexturesExercise3.h
        Src/Chapter01_Bird/4.1texture/TexturesExercise3.cpp
        Src/Chapter01_Bird/4.1texture/TexturesExercise4.h
        Src/Chapter01_Bird/4.1texture/TexturesExercise4.cpp
        #5.1 变换 transformation
        Src/Chapter01_Bird/5.1transformations/Transformations.h
        Src/Chapter01_Bird/5.1transformations/Transformations.cpp
        Src/Chapter01_Bird/5.1transformations/TransformationsDockWidget.h
        Src/Chapter01_Bird/5.1transformations/TransformationsDockWidget.cpp
        #6.1 坐标系
        Src/Chapter01_Bird/6.1coordinate_systems/CoordinateSystems.cpp
        Src/Chapter01_Bird/6.1coordinate_systems/CoordinateSystemsDepth.h
        Src/Chapter01_Bird/6.1coordinate_systems/CoordinateSystemsDepth.cpp
        Src/Chapter01_Bird/6.1coordinate_systems/CoordinateSystemsExercise.h
        Src/Chapter01_Bird/6.1coordinate_systems/CoordinateSystemsExercise.cpp
        Src/Chapter01_Bird/6.1coordinate_systems/CoordinateSystemsDockWidget.h
        Src/Chapter01_Bird/6.1coordinate_systems/CoordinateSystemsDockWidget.cpp
        #7.1 照相机
        Src/Chapter01_Bird/7.1camera/CameraDockWidget.h
        Src/Chapter01_Bird/7.1camera/CameraDockWidget.cpp
        Src/Chapter01_Bird/7.1camera/CameraCircle.h
        Src/Chapter01_Bird/7.1camera/CameraCircle.cpp
        Src/Chapter01_Bird/7.1camera/CameraInteract.h
        Src/Chapter01_Bird/7.1camera/CameraInteract.cpp
        Src/Chapter01_Bird/7.1camera/CameraClass.h
        Src/Chapter01_Bird/7.1camera/CameraClass.cpp
        #8.1 复习 Review
        Src/Chapter01_Bird/8.1review/ReviewDockWidget.h
        Src/Chapter01_Bird/8.1review/ReviewDockWidget.cpp
        Src/Chapter01_Bird/8.1review/Review_glsl.h
        Src/Chapter01_Bird/8.1review/Review_glsl.cpp
        Src/Chapter01_Bird/8.1review/Review_camera.h
        Src/Chapter01_Bird/8.1review/Review_camera.cpp
        #第二章 光照
        #2.1 光照基础
        Src/Chapter02_Lighting/2.1_basic_lighting/BasicLightingDiffuse.cpp
        Src/Chapter02_Lighting/2.1_basic_lighting/BasicLightingDiffuse.h
        Src/Chapter02_Lighting/2.1_basic_lighting/BasicLightingExercise1.cpp
        Src/Chapter02_Lighting/2.1_basic_lighting/BasicLightingExercise1.h
        Src/Chapter02_Lighting/2.1_basic_lighting/BasicLightingExercise3.cpp
        Src/Chapter02_Lighting/2.1_basic_lighting/BasicLightingExercise3.h
        Src/Chapter02_Lighting/2.1_basic_lighting/BasicLightingSpecular.cpp
        Src/Chapter02_Lighting/2.1_basic_lighting/BasicLightingSpecular.h
        Src/Chapter02_Lighting/2.1_basic_lighting/BasicLighting_DockWidget.cpp
        Src/Chapter02_Lighting/2.1_basic_lighting/BasicLighting_DockWidget.h
        Src/Chapter02_Lighting/2.1_basic_lighting/Colors.cpp
        Src/Chapter02_Lighting/2.1_basic_lighting/Colors.h
        #3.1 光照材质
        Src/Chapter02_Lighting/3.1_materials/Materials.cpp
        Src/Chapter02_Lighting/3.1_materials/Materials.h
        Src/Chapter02_Lighting/3.1_materials/Materials_DockWidget.cpp
        Src/Chapter02_Lighting/3.1_materials/Materials_DockWidget.h
        #4.1 光照贴图
        Src/Chapter02_Lighting/4.1_lighting_maps/LightingMapsDiffuseMap.cpp
        Src/Chapter02_Lighting/4.1_lighting_maps/LightingMapsDiffuseMap.h
        Src/Chapter02_Lighting/4.1_lighting_maps/LightingMapsExercise2.cpp
        Src/Chapter02_Lighting/4.1_lighting_maps/LightingMapsExercise2.h
        Src/Chapter02_Lighting/4.1_lighting_maps/LightingMapsExercise4.cpp
        Src/Chapter02_Lighting/4.1_lighting_maps/LightingMapsExercise4.h
        Src/Chapter02_Lighting/4.1_lighting_maps/LightingMapsSpecularMap.cpp
        Src/Chapter02_Lighting/4.1_lighting_maps/LightingMapsSpecularMap.h
        Src/Chapter02_Lighting/4.1_lighting_maps/LightingMaps_DockWidget.cpp
        Src/Chapter02_Lighting/4.1_lighting_maps/LightingMaps_DockWidget.h
        #5.1 光投射
        Src/Chapter02_Lighting/5.1_light_casters/LightCastersDirectional.cpp
        Src/Chapter02_Lighting/5.1_light_casters/LightCastersDirectional.h
        Src/Chapter02_Lighting/5.1_light_casters/LightCastersPoint.cpp
        Src/Chapter02_Lighting/5.1_light_casters/LightCastersPoint.h
        Src/Chapter02_Lighting/5.1_light_casters/LightCastersSpot.cpp
        Src/Chapter02_Lighting/5.1_light_casters/LightCastersSpot.h
        Src/Chapter02_Lighting/5.1_light_casters/LightCasters_DockWidget.cpp
        Src/Chapter02_Lighting/5.1_light_casters/LightCasters_DockWidget.h
        #6.1多光源
        Src/Chapter02_Lighting/6.1_multiple_lights/MultiLights_DockWidget.cpp
        Src/Chapter02_Lighting/6.1_multiple_lights/MultiLights_DockWidget.h
        Src/Chapter02_Lighting/6.1_multiple_lights/MultipleLights.cpp
        Src/Chapter02_Lighting/6.1_multiple_lights/MultipleLights.h

        #7.1光照复习
        Src/Chapter02_Lighting/7.1_review/ReviewLighting_DockWidget.h
        Src/Chapter02_Lighting/7.1_review/ReviewLighting_DockWidget.cpp
        Src/Chapter02_Lighting/7.1_review/Basic_Colors.h
        Src/Chapter02_Lighting/7.1_review/Basic_Colors.cpp
        Src/Chapter02_Lighting/7.1_review/Light_Materials.h
        Src/Chapter02_Lighting/7.1_review/Light_Materials.cpp
        Src/Chapter02_Lighting/7.1_review/Light_Multiple.h
        Src/Chapter02_Lighting/7.1_review/Light_Multiple.cpp
        

        #第三章 模型加载
        #1.1 模型加载
        
        Src/Chapter03_Model/1.1_load_model/LoadModel_DockWidget.h
        Src/Chapter03_Model/1.1_load_model/LoadModel_DockWidget.cpp
        Src/Chapter03_Model/1.1_load_model/LoadModel1.h
        Src/Chapter03_Model/1.1_load_model/LoadModel1.cpp

        #第四章 高级OpenGL
        Src/Chapter04_AdvancedOpenGL/AdvancedOpenGL_DockWidget.h
        Src/Chapter04_AdvancedOpenGL/AdvancedOpenGL_DockWidget.cpp
        Src/Chapter04_AdvancedOpenGL/Depth_testng.h
        Src/Chapter04_AdvancedOpenGL/Depth_testng.cpp
        Src/Chapter04_AdvancedOpenGL/Stencil_testing.h
        Src/Chapter04_AdvancedOpenGL/Stencil_testing.cpp
        Src/Chapter04_AdvancedOpenGL/Blending_discard.h
        Src/Chapter04_AdvancedOpenGL/Blending_discard.cpp
        Src/Chapter04_AdvancedOpenGL/Blending_sort.h
        Src/Chapter04_AdvancedOpenGL/Blending_sort.cpp
        Src/Chapter04_AdvancedOpenGL/Face_culling.h
        Src/Chapter04_AdvancedOpenGL/Face_culling.cpp
        Src/Chapter04_AdvancedOpenGL/Frame_buffers.h
        Src/Chapter04_AdvancedOpenGL/Frame_buffers.cpp
        Src/Chapter04_AdvancedOpenGL/Frame_buffers_exercise1.h
        Src/Chapter04_AdvancedOpenGL/Frame_buffers_exercise1.cpp
        Src/Chapter04_AdvancedOpenGL/Cubemaps_skybox.h
        Src/Chapter04_AdvancedOpenGL/Cubemaps_skybox.cpp
        Src/Chapter04_AdvancedOpenGL/Cubemaps_environment_mapping.h
        Src/Chapter04_AdvancedOpenGL/Cubemaps_environment_mapping.cpp
        Src/Chapter04_AdvancedOpenGL/Advanced_GLSL.h
        Src/Chapter04_AdvancedOpenGL/Advanced_GLSL.cpp
        Src/Chapter04_AdvancedOpenGL/Geometry_Shader_houses.h
        Src/Chapter04_AdvancedOpenGL/Geometry_Shader_houses.cpp
        Src/Chapter04_AdvancedOpenGL/Geometry_Shader_exploding.h
        Src/Chapter04_AdvancedOpenGL/Geometry_Shader_exploding.cpp
        Src/Chapter04_AdvancedOpenGL/Geometry_Shader_normals.h
        Src/Chapter04_AdvancedOpenGL/Geometry_Shader_normals.cpp
        Src/Chapter04_AdvancedOpenGL/Instancing_quads.h
        Src/Chapter04_AdvancedOpenGL/Instancing_quads.cpp
        Src/Chapter04_AdvancedOpenGL/Asteroids.h
        Src/Chapter04_AdvancedOpenGL/Asteroids.cpp
        Src/Chapter04_AdvancedOpenGL/Asteroids_instanced.h
        Src/Chapter04_AdvancedOpenGL/Asteroids_instanced.cpp
        Src/Chapter04_AdvancedOpenGL/Anti_Aliasing_msaa.h
        Src/Chapter04_AdvancedOpenGL/Anti_Aliasing_msaa.cpp
        Src/Chapter04_AdvancedOpenGL/Anti_aliasing_offscreen.h
        Src/Chapter04_AdvancedOpenGL/Anti_aliasing_offscreen.cpp

        #第五章 高级光照
        Src/Chapter05_AdvancedLighting/AdvancedLighting_DockWidget.h
        Src/Chapter05_AdvancedLighting/AdvancedLighting_DockWidget.cpp
        Src/Chapter05_AdvancedLighting/Advanced_lighting.h
        Src/Chapter05_AdvancedLighting/Advanced_lighting.cpp
        Src/Chapter05_AdvancedLighting/Gamma_correction.h
        Src/Chapter05_AdvancedLighting/Gamma_correction.cpp
        Src/Chapter05_AdvancedLighting/Shadow_mapping_depth.h
        Src/Chapter05_AdvancedLighting/Shadow_mapping_depth.cpp
        Src/Chapter05_AdvancedLighting/Shadow_mapping_base.h
        Src/Chapter05_AdvancedLighting/Shadow_mapping_base.cpp
        Src/Chapter05_AdvancedLighting/Shadow_mapping.h
        Src/Chapter05_AdvancedLighting/Shadow_mapping.cpp
        Src/Chapter05_AdvancedLighting/Point_shadows.h
        Src/Chapter05_AdvancedLighting/Point_shadows.cpp
        Src/Chapter05_AdvancedLighting/Point_shadows_soft.h
        Src/Chapter05_AdvancedLighting/Point_shadows_soft.cpp
        Src/Chapter05_AdvancedLighting/Normal_mapping.h
        Src/Chapter05_AdvancedLighting/Normal_mapping.cpp
        Src/Chapter05_AdvancedLighting/Parallax_mapping.h
        Src/Chapter05_AdvancedLighting/Parallax_mapping.cpp
        Src/Chapter05_AdvancedLighting/Steep_parallax_mapping.h
        Src/Chapter05_AdvancedLighting/Steep_parallax_mapping.cpp
        Src/Chapter05_AdvancedLighting/Parallax_occlusion_mapping.h
        Src/Chapter05_AdvancedLighting/Parallax_occlusion_mapping.cpp
        Src/Chapter05_AdvancedLighting/HDR.h
        Src/Chapter05_AdvancedLighting/HDR.cpp
        Src/Chapter05_AdvancedLighting/Bloom.h
        Src/Chapter05_AdvancedLighting/Bloom.cpp
        Src/Chapter05_AdvancedLighting/Deferred_shading.h
        Src/Chapter05_AdvancedLighting/Deferred_shading.cpp
        Src/Chapter05_AdvancedLighting/Deferred_shading_volumes.h
        Src/Chapter05_AdvancedLighting/Deferred_shading_volumes.cpp
        Src/Chapter05_AdvancedLighting/SSAO.h
        Src/Chapter05_AdvancedLighting/SSAO.cpp

        
        )
SET(UI_FILES
        Src/mainwindow.ui
        Src/glDockWidget.ui
)
SET(UTILS_FILES
        Src/utils/CameraUtil.h
        Src/utils/CameraUtil.cpp
        Src/utils/Config.h
        Src/utils/Mesh.h
        Src/utils/Mesh.cpp
        Src/utils/ModelUtil.h
        Src/utils/ModelUtil.cpp
        Src/utils/ShaderUtil.h
        Src/utils/CubemapTextureUtil.h
        Src/utils/OpenGLUniformbufferObject.h
        Src/utils/OpenGLUniformbufferObject.cpp

        
        Src/MyUtils/ColorButton.h
        Src/MyUtils/ColorButton.cpp
        Src/MyUtils/SliderWithLabel.h
        Src/MyUtils/SliderWithLabel.cpp
        Src/MyUtils/PositionSpinbox.h
        Src/MyUtils/PositionSpinbox.cpp
        Src/MyUtils/PositionSpinbox_New.h
        Src/MyUtils/PositionSpinbox_New.cpp
        Src/MyUtils/Light.h
        Src/MyUtils/Light.cpp


        
)

#Assimp 模型解析库
LINK_DIRECTORIES (${ASSIMP_INCLUDE_DIR}/assimpLib/)
LINK_LIBRARIES (assimp-vc142-mt.lib)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/ico.rc")
qt6_add_resources(RESOURCE_FILES res.qrc )
#设置输出的格式 exe
qt_add_executable(${ProName}
        ${SRC} ${UI_FILES} ${UTILS_FILES} ${app_icon_resource_windows}
)
target_sources(${ProName} PRIVATE ${RESOURCE_FILES})
set_target_properties(${ProName} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
        LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
    LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS"
)


target_link_libraries(${ProName} PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Widgets
)
message("{CMAKE_CURRENT_SOURCE_DIR} is ${CMAKE_CURRENT_SOURCE_DIR}")
message("{CMAKE_RUNTIME_OUTPUT_DIRECTORY} is ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message("{MY_PROJECT_OUTPUT_DIR} is ${MY_PROJECT_OUTPUT_DIR}")
file(COPY ${ASSIMP_INCLUDE_DIR}/assimpDll/assimp-vc142-mt.dll
    DESTINATION ${MY_PROJECT_OUTPUT_DIR})
