#ifndef SLIDERWITHLABEL_H
#define SLIDERWITHLABEL_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QHBoxLayout>
#include <QString>

class SliderWithLabel : public QWidget
{
    Q_OBJECT

public:
    // 构造函数声明
    explicit SliderWithLabel(QString label_name, double minimum, double maximum, double step, double _default_value = 0.0, QWidget *parent = nullptr, int _sliderIndex = -1, int _postfix = 2);

    // 获取滑块的值
    double getValue() const;
    bool setValue(double value);
    void setEnabled(bool enabled);
    // 启用滑块和标签
    void enable();
    // 禁用滑块和标签
    void disable();
signals:
    void slider_valueChanged(int index = -1);
private slots:
    // 滑块值变化时的槽函数
    void onSliderValueChanged(int value);

private:
    QString titlename;
    QLabel *label;
    QSlider *slider;
    QLabel *label_slider_value;
    int sliderIndex = -1;
    double minimum;
    double maximum;
    double step;
    double default_value;
    int postfix = 2;
    bool isEnabled;
};

#endif // SLIDERWITHLABEL_H
