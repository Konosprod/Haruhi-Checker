#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>

namespace Ui {
class preferenceDialog;
}

class preferenceDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit preferenceDialog(QWidget *parent = 0, QSettings* s = 0);
    QString directory(void);
    bool download(void);
    int timer(void);

    ~preferenceDialog();

public slots:
    void getDir(void);
    
private:
    Ui::preferenceDialog *ui;
};

#endif // PREFERENCEDIALOG_H
