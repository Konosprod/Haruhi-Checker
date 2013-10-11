#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "adddialog.h"
#include "resultwindows.h"
#include "preferencedialog.h"
#include <QTimer>
#include <QTextDocument>
#include <QSettings>
#include <QTextStream>
#include <QStringListModel>
#include <QFile>
#include <QDir>
#include <QSystemTrayIcon>
#include <QRegExp>
#include <QCloseEvent>
#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonValueRef>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMessageBox>

typedef struct Entry
{
    QString name;
    QString clink;
    QString cep;
}Entry;

typedef struct persoCritere
{
    QString format;
    QString team;
    QString title;
}persoCritere;

namespace Ui {
class mainWindow;
}

class mainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();

public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason);
    void loadJsonSource(QNetworkReply*);
    void addEntry(void);
    void deleteEntry(void);
    void editEntry(void);
    void editEntryDoubleClick(QModelIndex);
    void checkForUpdates(void);
    void checkEntries(QList<Entry>* list);
    void minimizeWindow(void);
    void showResultWindow(void);
    void editPrefs(void);
    void saveFile(QNetworkReply* r);

protected:
    void getJsonSource(void);
    void updateList(void);
    void setupConnexions(void);
    void setupSysIcon(void);
    void loadConfig(void);
    void closeEvent(QCloseEvent *);
    void downloadFile(QString url);

    
private:
    QList<persoCritere>* listToLook;
    QList<Entry>* newEntries;
    QTimer* timer;
    Ui::mainWindow *ui;
    QNetworkAccessManager* nam;
    QString jsonSource;
    QSystemTrayIcon* systIcon;
    QJsonDocument jsonDocument;
    QJsonDocument checkDocument;
    QSettings* settings;
};

#endif // MAINWINDOW_H
