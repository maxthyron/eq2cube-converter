#include "window.h"
#include "ui_window.h"

Window::Window(QWidget* parent) :
        QWidget(parent),
        ui(new Ui::Window)
{
    ui->setupUi(this);
}

Window::~Window()
{
    delete ui;
}

void Window::on_pushButton_clicked()
{

}

void Window::on_pushButton_2_clicked()
{

}

void Window::on_pushButton_3_clicked()
{

}

void Window::on_pushButton_4_clicked()
{

}
