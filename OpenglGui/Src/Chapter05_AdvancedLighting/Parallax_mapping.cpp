
#include "Parallax_mapping.h"

Parallax_mapping::Parallax_mapping(QWidget *parent) : GLWidget(parent)
{
    // 构造函数内容
}

Parallax_mapping::~Parallax_mapping()
{
    // 析构函数内容
}

void Parallax_mapping::initializeGL()
{
    // 初始化OpenGL内容
}

void Parallax_mapping::resizeGL(int w, int h)
{
    // 调整视图大小
}

void Parallax_mapping::paintGL()
{
    // 绘制OpenGL内容
}

bool Parallax_mapping::event(QEvent *e)
{
    // 事件处理
    return GLWidget::event(e);
}

QWidget *Parallax_mapping::createControlPanel()
{
    // 创建控制面板
    return nullptr;
}
