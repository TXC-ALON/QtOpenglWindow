#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <QLabel>
#include "glWidget.h"
#include "CameraUtil.h"
#include "SliderWithLabel.h"
#include "PositionSpinbox.h"
#include <QString>
#include <QComboBox>
class ColorButton;
class Material
{
public:
    Material() : name(""), ambient(0.0f, 0.0f, 0.0f), diffuse(0.0f, 0.0f, 0.0f), specular(0.0f, 0.0f, 0.0f), shininess(0.0f) {}

    Material(const QString &name, const QVector3D &ambient, const QVector3D &diffuse, const QVector3D &specular, float shininess)
        : name(name), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

    QString getName() const { return name; }
    QVector3D getAmbient() const { return ambient; }
    QVector3D getDiffuse() const { return diffuse; }
    QVector3D getSpecular() const { return specular; }
    float getShininess() const { return shininess; }

    // 如果需要，可以添加设置器（setter）函数
    void setName(const QString &newName) { name = newName; }
    void setAmbient(const QVector3D &newAmbient) { ambient = newAmbient; }
    void setDiffuse(const QVector3D &newDiffuse) { diffuse = newDiffuse; }
    void setSpecular(const QVector3D &newSpecular) { specular = newSpecular; }
    void setShininess(float newShininess) { shininess = newShininess; }

public:
    QString name;
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    float shininess;
};
class Light_Materials : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Light_Materials(QWidget *parent = nullptr);
    ~Light_Materials();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;
    QWidget *createControlPanel() override;

private:
    QOpenGLVertexArrayObject m_lightVAO;
    QOpenGLVertexArrayObject m_lightCubeVAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_lightShader;
    QOpenGLShaderProgram m_lightCubeShader;

    QVector3D m_lightPos;

    QMatrix4x4 m_projection;

    CameraUtil m_camera;

    QLabel *m_labelYaw;
    QLabel *m_labelPitch;
    QLabel *m_labelCameraPos;

private:
    void set_camera_labels();
    ColorButton *modelcolor;
    ColorButton *lightcolor;
    double ambientStrength = 0.1;
    double specularStrength = 1;
    double specularShininess = 32;
    QList<SliderWithLabel *> light_controls;

    bool useAmbient = true;
    bool useDiffuse = true;
    bool useSpecular = true;
    QCheckBox *checkBoxAmbient;
    QCheckBox *checkBoxDiffuse;
    QCheckBox *checkBoxSpecular;

    PositionSpinbox *pPositionSpinbox = nullptr;
    Material current_material;
    QList<Material> materials;
    void InitializeMaterials();

private slots:
    void onCheckBoxChanged();
    void onPositionChanged();
    void updateMaterial(int index);
};
