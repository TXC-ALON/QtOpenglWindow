
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
class Deferred_shading : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Deferred_shading(QWidget *parent = nullptr);
    ~Deferred_shading();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

public:
    QWidget *createControlPanel() override;
signals:
    void initialized();

private:
    // 这里将添加成员变量和方法
};
