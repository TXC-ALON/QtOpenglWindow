
#include "Normal_mapping.h"

Normal_mapping::Normal_mapping(QWidget *parent) : GLWidget(parent)
{
    // 构造函数内容
}

Normal_mapping::~Normal_mapping()
{
    // 析构函数内容
}

void Normal_mapping::initializeGL()
{
    // 初始化OpenGL内容
}

void Normal_mapping::resizeGL(int w, int h)
{
    // 调整视图大小
}

void Normal_mapping::paintGL()
{
    // 绘制OpenGL内容
}

bool Normal_mapping::event(QEvent *e)
{
    // 事件处理
    return GLWidget::event(e);
}

QWidget *Normal_mapping::createControlPanel()
{
    // 创建控制面板
    return nullptr;
}
