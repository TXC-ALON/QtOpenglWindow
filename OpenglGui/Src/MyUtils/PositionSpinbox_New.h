#pragma once
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QString>
#include <QStringList>
#include <QDoubleSpinBox>
#include <QVector3D>
#include <QWidget>
#include <QVector>
#include <QLabel>
#include <vector>

class PositionSpinbox_New : public QWidget
{
    Q_OBJECT

public:
    PositionSpinbox_New(QString name, QStringList _label_names, std::vector<double> _default_values, std::vector<double> _steps, std::vector<std::vector<double>> _range, QWidget *parent = nullptr);
    ~PositionSpinbox_New() override;

    double getValueByIndex(int index) const;
    bool setValueByIndex(int index, double value);

    void setEnabled(bool enabled);

signals:
    void spinbox_valueChanged();
    void spinbox_valueChangedByIndex(int index, double value);

private:
    QString name;
    QStringList label_names;
    std::vector<double> default_values;
    std::vector<double> steps;
    std::vector<std::vector<double>> range;
    QVector<QLabel *> labels;
    QVector<QDoubleSpinBox *> dspinboxs;

    void onValueChanged(int index, double value);
};
