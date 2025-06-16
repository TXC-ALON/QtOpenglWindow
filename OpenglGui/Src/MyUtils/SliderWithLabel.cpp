#include "SliderWithLabel.h"

SliderWithLabel::SliderWithLabel(QString label_name, double minimum, double maximum, double step, double _default_value, QWidget *parent, int _sliderIndex, int _postfix)
    : QWidget(parent), titlename(label_name), minimum(minimum), maximum(maximum), step(step), default_value(_default_value), sliderIndex(_sliderIndex), postfix(_postfix)
{
    if (default_value < minimum || default_value > maximum)
    {
        default_value = minimum; // 如果默认值不在范围内，设置为最小值
    }
    int sliderMax = static_cast<int>(std::round((maximum - minimum) / step));

    label = new QLabel(label_name, this);
    label->setMinimumSize(QSize(80, 25)); // 设置最小尺寸
    slider = new QSlider(Qt::Horizontal, this);
    label_slider_value = new QLabel(this);
    label_slider_value->setAlignment(Qt::AlignRight);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(1); // 移除内部组件之间的间距
    layout->addWidget(label);
    layout->addWidget(slider);
    layout->addWidget(label_slider_value);

    slider->setRange(0, sliderMax);
    slider->setSingleStep(1); // 设置步长为1，因为我们将值转换为整数倍的step
    slider->setPageStep(1);
    // 设置滑块的初始位置
    int initialSliderValue = static_cast<int>(std::round((default_value - minimum) / step));
    slider->setValue(initialSliderValue);

    label_slider_value->setText(QString::number(getValue(), 'f', postfix));
    this->adjustSize();
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
}

double SliderWithLabel::getValue() const
{
    return minimum + slider->value() * step;
}

bool SliderWithLabel::setValue(double value)
{
    if (value < minimum || value > maximum)
    {
        // 如果值不在范围内，设置为最接近的边界值
        value = qMin(qMax(value, minimum), maximum);
        // return false;
    }
    int sliderValue = static_cast<int>((value - minimum) / step);

    // 只有当滑块的值实际改变时，才触发信号
    if (slider->value() != sliderValue)
    {
        slider->setValue(sliderValue); // 这将触发 valueChanged 信号
        label_slider_value->setText(QString::number(value, 'f', postfix));
        return true;
    }
    return false;
}
void SliderWithLabel::setEnabled(bool enabled)
{
    isEnabled = enabled;
    isEnabled ? enable() : disable();
}
// 启用滑块的标签
void SliderWithLabel::enable()
{
    slider->setEnabled(true);
    label->setEnabled(true);
    label_slider_value->setEnabled(true);
}

// 禁用滑块和标签
void SliderWithLabel::disable()
{
    slider->setEnabled(false);
    label->setEnabled(false);
    label_slider_value->setEnabled(false);
}

void SliderWithLabel::onSliderValueChanged(int value)
{
    label_slider_value->setText(QString::number(getValue(), 'f', postfix));
    // qDebug() << sliderIndex << " onSliderValueChanged " << titlename << " value is" << getValue();
    emit slider_valueChanged(sliderIndex);
}
