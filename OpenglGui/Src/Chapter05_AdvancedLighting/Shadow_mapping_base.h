
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
class Shadow_mapping_base : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Shadow_mapping_base(QWidget *parent = nullptr);
    ~Shadow_mapping_base();

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
