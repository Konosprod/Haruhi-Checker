#include "adddialog.h"
#include "ui_adddialog.h"

addDialog::addDialog(QWidget *parent, QString format, QString team, QString title) :
    QDialog(parent),
    ui(new Ui::addDialog)
{
    ui->setupUi(this);
    ui->lineFormat->setText(format);
    ui->lineTeam->setText(team);
    ui->lineTitle->setText(title);
}

addDialog::~addDialog()
{
    delete ui;
}

QString addDialog::getFormat()
{
    return ui->lineFormat->text();
}

QString addDialog::getTeam()
{
    return ui->lineTeam->text();
}

QString addDialog::getTitle()
{
    return ui->lineTitle->text();
}
