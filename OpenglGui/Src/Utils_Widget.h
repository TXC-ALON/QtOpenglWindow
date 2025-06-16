#pragma once
#include <QWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QStringList>
QWidget *createButtomsControl(QWidget *parent, QStringList bottoms, QStringList button_name_list, QList<QPushButton *> &buttonReferences);