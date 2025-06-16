#include "ColorButton.h"

ColorButton::ColorButton(QString title, QVector3D color, QWidget *parent, bool _enabled)
    : QWidget(parent),
      label(new QLabel(title + ":", this)),
      button(new QPushButton("", this)),
      colorlabel(new QLabel("RGB: 255, 255, 255", this)),
      isEnabled(_enabled)
{
    titlename = title;
    button->setFixedSize(25, 25);                    // 设置按钮的固定大小为25x25像素
    label->setFixedWidth(label->sizeHint().width()); // 设置label的固定宽度
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(2, 2, 2, 2);
    hLayout->setSpacing(5);
    hLayout->addWidget(label);
    hLayout->addWidget(button);
    hLayout->addWidget(colorlabel);
    setLayout(hLayout);
    connect(button, &QPushButton::clicked, this, &ColorButton::changeColor);
    setColor(RGBToColor(color));
}

QColor ColorButton::RGBToColor(QVector3D vector)
{
    // 将0-255范围的值转换为0-1范围的浮点数
    qreal red = vector.x() / 255.0;
    qreal green = vector.y() / 255.0;
    qreal blue = vector.z() / 255.0;
    // 创建QColor对象
    return QColor::fromRgbF(red, green, blue);
}

void ColorButton::setColor(QColor color)
{
    currentColor = color;
    // qDebug() << titlename << " setcolor is " << currentColor.redF() << " , " << currentColor.greenF() << " , " << currentColor.blueF();
    button->setStyleSheet(QString("background-color: %1").arg(currentColor.name()));
    colorlabel->setText(QString("RGB: %1, %2, %3").arg(color.red()).arg(color.green()).arg(color.blue()));
}

void ColorButton::setColor(QVector3D color)
{
    qreal red = color.x();
    qreal green = color.y();
    qreal blue = color.z();
    // 创建QColor对象
    currentColor = QColor::fromRgbF(red, green, blue);
    setColor(currentColor);
}

QVector3D ColorButton::getColor()
{
    currentColor_Vec = QVector3D(currentColor.redF(), currentColor.greenF(), currentColor.blueF());
    return currentColor_Vec;
}

void ColorButton::changeColor()
{
    if (isEnabled) // 检查是否启用颜色对话框
    {
        QColorDialog dialog;
        dialog.setCurrentColor(currentColor);
        if (dialog.exec() == QColorDialog::Accepted)
        {
            QColor color = dialog.currentColor();
            setColor(color);
            emit color_changed();
        }
    }
}
