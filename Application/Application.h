#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Application.h"

class Application : public QMainWindow
{
    Q_OBJECT

public:
    Application(QWidget *parent = Q_NULLPTR);

public slots:
    void testSlots();

private:
    Ui::ApplicationClass ui;
};
