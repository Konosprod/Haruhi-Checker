#include "resultwindows.h"
#include "ui_resultwindows.h"

resultWindows::resultWindows(QWidget *parent, QStringListModel* m) :
    QDialog(parent),
    ui(new Ui::resultWindows)
{
    ui->setupUi(this);
    this->setWindowTitle("Résultats");
    ui->listView->setModel(m);
}

resultWindows::~resultWindows()
{
    delete ui;
}
