#ifndef RESULTWINDOWS_H
#define RESULTWINDOWS_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class resultWindows;
}

class resultWindows : public QDialog
{
    Q_OBJECT
    
public:
    explicit resultWindows(QWidget *parent = 0, QStringListModel* m = 0);
    ~resultWindows();
    
private:
    Ui::resultWindows *ui;
};

#endif // RESULTWINDOWS_H
