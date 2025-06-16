#pragma once
#include "glDockWidget.h"

class Textures;
class TextureWidget;
class TexturesExercise2;
class TexturesExercise3;
class TexturesExercise4;

class TextureDockWidget : public glDockWidget
{
    Q_OBJECT
public:
    TextureDockWidget(QWidget *parent = nullptr);
    ~TextureDockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    Textures *pTextures = nullptr;
    TextureWidget *pTextureWidget = nullptr;
    TexturesExercise2 *pTexturesExercise2 = nullptr;
    TexturesExercise3 *pTexturesExercise3 = nullptr;
    TexturesExercise4 *pTexturesExercise4 = nullptr;

public slots:
    void pb_Textures_clicked();
    void pb_TextureWidget_clicked();
    void pb_Texture_exercise2_clicked();
    void pb_Texture_exercise3_clicked();
    void pb_Texture_exercise4_clicked();
};
