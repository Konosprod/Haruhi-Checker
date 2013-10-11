#include "preferencedialog.h"
#include "ui_preferencedialog.h"

preferenceDialog::preferenceDialog(QWidget *parent, QSettings *s) :
    QDialog(parent),
    ui(new Ui::preferenceDialog)
{
    ui->setupUi(this);
    ui->spinTime->setValue(s->value("timer", 3600000).toInt());
    ui->lineDir->setText(s->value("output", "").toString());
    ui->checkBox->setChecked(s->value("download", false).toBool());

    connect(ui->buttonDir, SIGNAL(clicked()), this, SLOT(getDir()));
}

preferenceDialog::~preferenceDialog()
{
    delete ui;
}

void preferenceDialog::getDir()
{
    QString s = "";
    if(!(s = QFileDialog::getExistingDirectory(this, "Choisir un dossier de sortie", QDir::homePath())).isEmpty())
    {
        ui->lineDir->setText(s);
    }
}

QString preferenceDialog::directory()
{
    return ui->lineDir->text();
}

bool preferenceDialog::download()
{
    return ui->checkBox->isChecked();
}

int preferenceDialog::timer()
{
    return ui->spinTime->value();
}

