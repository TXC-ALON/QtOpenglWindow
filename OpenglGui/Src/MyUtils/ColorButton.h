#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QApplication>
#include <QPushButton>
#include <QColorDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector3D>
class ColorButton : public QWidget
{
    Q_OBJECT

public:
    // 构造函数声明
    explicit ColorButton(QString title, QVector3D color = QVector3D(255, 255, 255), QWidget *parent = nullptr, bool _enabled = true);

public:
    QColor RGBToColor(QVector3D vector);
    QColor currentColor; // 用于存储当前颜色
signals:
    void color_changed();
public slots:
    void changeColor();
    // 设置颜色接口
    void setColor(QColor color);
    void setColor(QVector3D color);

    // 获取颜色接口
    QVector3D getColor();
    void setEnabled(bool enable)
    {
        isEnabled = enable;
    }

private:
    QString titlename;
    QPushButton *button;
    QLabel *label;
    QLabel *colorlabel;

    QVector3D currentColor_Vec;
    bool isEnabled = true;
};

#endif // COLORBUTTON_H
