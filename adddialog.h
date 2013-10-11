#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

namespace Ui {
class addDialog;
}

class addDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit addDialog(QWidget *parent = 0, QString format = "", QString team = "", QString title = "");
    ~addDialog();

public slots:
    QString getTitle(void);
    QString getFormat(void);
    QString getTeam(void);
    
private:
    Ui::addDialog *ui;
};

#endif // ADDDIALOG_H
