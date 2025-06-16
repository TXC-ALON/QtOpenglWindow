#include "TextureDockWidget.h"
#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>
#include "Textures.h"
#include "TextureWidget.h"
#include "TexturesExercise2.h"
#include "TexturesExercise3.h"
#include "TexturesExercise4.h"
TextureDockWidget::TextureDockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

TextureDockWidget::~TextureDockWidget()
{
}
void TextureDockWidget::initDockWidget()
{
    DockWidgetName = "HelloTriangleDockWidget";
    button_list = {"pb_Texture", "pb_TextureWidget", "pb_Texture_exercise2", "pb_Texture_exercise3", "pb_Texture_exercise4"};
    button_name_list = {"Texture", "TextureWidget", "Texture_exercise2", "Texture_exercise3", "Texture_exercise4"};
}
void TextureDockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void TextureDockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
}
void TextureDockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &TextureDockWidget::pb_Textures_clicked);
    connect(example_buttoms[1], &QPushButton::clicked, this, &TextureDockWidget::pb_TextureWidget_clicked);
    connect(example_buttoms[2], &QPushButton::clicked, this, &TextureDockWidget::pb_Texture_exercise2_clicked);
    connect(example_buttoms[3], &QPushButton::clicked, this, &TextureDockWidget::pb_Texture_exercise3_clicked);
    connect(example_buttoms[4], &QPushButton::clicked, this, &TextureDockWidget::pb_Texture_exercise4_clicked);
}
void TextureDockWidget::pb_Textures_clicked()
{
    std::cout << "pb_Textures_clicked" << std::endl;
    pTextures = new Textures(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pTextures);
}
void TextureDockWidget::pb_TextureWidget_clicked()
{
    std::cout << "pb_TextureWidget_clicked" << std::endl;
    pTextureWidget = new TextureWidget(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pTextureWidget);
}
void TextureDockWidget::pb_Texture_exercise2_clicked()
{
    std::cout << "pb_Texture_exercise2_clicked" << std::endl;
    pTexturesExercise2 = new TexturesExercise2(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pTexturesExercise2);
}
void TextureDockWidget::pb_Texture_exercise3_clicked()
{
    std::cout << "pb_Texture_exercise3_clicked" << std::endl;
    pTexturesExercise3 = new TexturesExercise3(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pTexturesExercise3);
}
void TextureDockWidget::pb_Texture_exercise4_clicked()
{
    std::cout << "pb_Texture_exercise4_clicked" << std::endl;
    pTexturesExercise4 = new TexturesExercise4(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pTexturesExercise4);
    refresh_tool_control(pTexturesExercise4);
}