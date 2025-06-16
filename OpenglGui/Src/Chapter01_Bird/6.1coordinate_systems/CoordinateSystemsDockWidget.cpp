#include "CoordinateSystemsDockWidget.h"
#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>
#include "CoordinateSystems.h"
#include "CoordinateSystemsDepth.h"
#include "CoordinateSystemsExercise.h"
CoordinateSystemsDockWidget::CoordinateSystemsDockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

CoordinateSystemsDockWidget::~CoordinateSystemsDockWidget()
{
}
void CoordinateSystemsDockWidget::initDockWidget()
{
    DockWidgetName = "CoordinateSystemsDockWidget";
    button_list = {"pb_CoordinateSystems", "pb_CoordinateSystemsDepth", "pb_CoordinateSystemsExercise"};
    button_name_list = {"CoordinateSystems", "CoordinateSystemsDepth", "CoordinateSystemsExercise"};
}
void CoordinateSystemsDockWidget::setupWidget(QWidget *widget)
{
    // addToolBoxWithButtons();
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void CoordinateSystemsDockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
}
void CoordinateSystemsDockWidget::connectSignals(QWidget *widget)
{
    // connect(ui->pb_CoordinateSystems, &QPushButton::clicked, Manager::get_instance(), &Manager::coordinatesystems);
    // connect(ui->pb_CoordinateSystemsDepth, &QPushButton::clicked, Manager::get_instance(), &Manager::coordinatesystems_depth);
    // connect(ui->pb_CoordinateSystemsExercise, &QPushButton::clicked, Manager::get_instance(), &Manager::coordinatesystems_exercise);
    connect(example_buttoms[0], &QPushButton::clicked, this, &CoordinateSystemsDockWidget::pb_CoordinateSystems_clicked);
    connect(example_buttoms[1], &QPushButton::clicked, this, &CoordinateSystemsDockWidget::pb_CoordinateSystemsDepth_clicked);
    connect(example_buttoms[2], &QPushButton::clicked, this, &CoordinateSystemsDockWidget::pb_CoordinateSystemsExercise_clicked);
}

void CoordinateSystemsDockWidget::create_tool_example(QWidget *widget)
{
    QWidget *panel = new QWidget();
    int index = ui->toolBox->indexOf(ui->tool_example);
    // std::cout << "ui->toolBox is " << index << std::endl;
    ui->toolBox->removeItem(index);
    delete ui->tool_example;
    ui->tool_example = nullptr;
    QVBoxLayout *layout = new QVBoxLayout(panel);
    example_buttoms.clear();

    for (int i = 0; i < button_list.size(); i++)
    {
        example_buttoms.push_back(new QPushButton(ui->tool_example));
        example_buttoms[i]->setObjectName(button_list[i]);
        example_buttoms[i]->setText(QCoreApplication::translate("CoordinateSystemsDockWidget", button_name_list[i].toUtf8().constData(), nullptr));
        layout->addWidget(example_buttoms[i]);
    }
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    layout->addItem(verticalSpacer);
    ui->tool_example = panel;
    if (ui->tool_example)
    {
        ui->toolBox->insertItem(0, ui->tool_example, QString::fromUtf8("示例"));
    }
    ui->toolBox->setCurrentIndex(0);
}

void CoordinateSystemsDockWidget::pb_CoordinateSystems_clicked()
{
    std::cout << "pb_CoordinateSystems_clicked" << std::endl;
    pCoordinateSystems = new CoordinateSystems(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pCoordinateSystems);
}
void CoordinateSystemsDockWidget::pb_CoordinateSystemsDepth_clicked()
{
    std::cout << "on_pb_CoordinateSystemsDepth_clicked" << std::endl;
    pCoordinateSystemsDepth = new CoordinateSystemsDepth(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pCoordinateSystemsDepth);
    refresh_tool_control(pCoordinateSystemsDepth);
}

void CoordinateSystemsDockWidget::pb_CoordinateSystemsExercise_clicked()
{
    std::cout << "on_pb_CoordinateSystemsExercise_clicked" << std::endl;
    pCoordinateSystemsExercise = new CoordinateSystemsExercise(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pCoordinateSystemsExercise);
    refresh_tool_control(pCoordinateSystemsExercise);
}