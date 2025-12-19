#include "appmain.hpp"
#include "ui_AppMain.h"


AppMain::AppMain(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::AppMain) {
    ui->setupUi(this);
}

AppMain::~AppMain() {
    delete ui;
}
