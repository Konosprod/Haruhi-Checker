#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 * Cliquer sur la liste => renvoie sur la page de DL
 */

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Haruhi Checker");

    nam = new QNetworkAccessManager(this);
    systIcon = new QSystemTrayIcon(QIcon(":rc/icon"));
    listToLook = new QList<persoCritere>();
    newEntries = new QList<Entry>();
    timer = new QTimer(this);
    settings = new QSettings("Konosprod", "Haruhi-Checker", this);

    QDir d;
    d.mkdir("cfg/");

    loadConfig();

    systIcon->show();
    ui->tableEntry->setSelectionBehavior(QAbstractItemView::SelectRows);
    timer->setInterval(settings->value("timer", 3600000).toInt());

    setupSysIcon();
    setupConnexions();

    timer->start();
}

void mainWindow::editPrefs()
{
    preferenceDialog* p = new preferenceDialog(this, settings);

    p->setWindowTitle("Préférences");

    if(p->exec() == QDialog::Accepted)
    {
        settings->setValue("timer", p->timer());
        settings->setValue("output", p->directory());
        settings->setValue("download", p->download());
        timer->stop();
        timer->setInterval(settings->value("timer", 3600000).toInt());
        timer->start();
    }
}

void mainWindow::checkForUpdates()
{
    getJsonSource();
}

void mainWindow::setupSysIcon()
{
    QMenu* m = new QMenu();
    m->addAction(ui->actionV_rifier);
    m->addAction(ui->actionR_sultats);
    m->addSeparator();
    m->addAction(ui->actionQuitter);
    systIcon->setContextMenu(m);
}

void mainWindow::setupConnexions()
{
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(loadJsonSource(QNetworkReply*)));
    connect(ui->actionAjouter_une_entr_e, SIGNAL(triggered()), this, SLOT(addEntry()));
    connect(ui->actionSupprimer_l_entr_e, SIGNAL(triggered()), this, SLOT(deleteEntry()));
    connect(ui->actionEditer_l_entr_e, SIGNAL(triggered()), this, SLOT(editEntry()));
    connect(timer, SIGNAL(timeout()), this, SLOT(checkForUpdates()));
    connect(ui->actionV_rifier, SIGNAL(triggered()), this, SLOT(checkForUpdates()));
    connect(ui->actionMini, SIGNAL(triggered()), this, SLOT(minimizeWindow()));
    connect(systIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(ui->actionR_sultats, SIGNAL(triggered()), this, SLOT(showResultWindow()));
    connect(ui->actionQuitter, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(editPrefs()));
    connect(ui->tableEntry, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editEntryDoubleClick(QModelIndex)));
}

void mainWindow::showResultWindow()
{
    QStringList l;
    QStringListModel* m = new QStringListModel();

    for(int i = 0; i < newEntries->count(); i++)
    {
        l.append(newEntries->at(i).cep);
    }

    m->setStringList(l);

    resultWindows* rw = new resultWindows(this, m);
    rw->setWindowTitle("Résultats");
    rw->exec();
}

void mainWindow::downloadFile(QString url)
{
    QTextDocument doc;
    doc.setHtml(url);

    QNetworkAccessManager* d = new QNetworkAccessManager(this);
    d->get(QNetworkRequest(QUrl(doc.toPlainText())));
    connect(d, SIGNAL(finished(QNetworkReply*)), this, SLOT(saveFile(QNetworkReply*)));
}

void mainWindow::saveFile(QNetworkReply *r)
{
    //QMessageBox::information(this, "title", r->rawHeader("Content-Disposition"));
    QString fn = r->rawHeader("Content-Disposition");
    fn = fn.remove(0, fn.indexOf("\"")+1);
    fn = fn.replace("\"", "");

    QFile f(settings->value("output").toString() + QDir::separator() + fn);
    f.open(QIODevice::WriteOnly);
    f.write(r->readAll());
    f.close();
}

void mainWindow::loadConfig()
{
    QString s = "";
    QFile f("cfg/list.cfg");
    f.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream in(&f);

    while(!in.atEnd())
    {
        QString s = in.readLine();
        QStringList l = s.split(";");
        ui->tableEntry->setRowCount(ui->tableEntry->rowCount()+1);
        ui->tableEntry->setItem(ui->tableEntry->rowCount()-1, 0, new QTableWidgetItem(l.at(0)));
        ui->tableEntry->setItem(ui->tableEntry->rowCount()-1, 1, new QTableWidgetItem(l.at(1)));
        ui->tableEntry->setItem(ui->tableEntry->rowCount()-1, 2, new QTableWidgetItem(l.at(2)));
    }

    f.close();
}

void mainWindow::closeEvent(QCloseEvent *event)
{
    QFile f("cfg/list.cfg");
    f.open(QIODevice::ReadWrite | QIODevice::Text);
    f.resize(0);
    QTextStream out(&f);

    for(int i = 0; i < ui->tableEntry->rowCount(); i++)
    {
        QString s = ui->tableEntry->item(i, 0)->text() + ";" + ui->tableEntry->item(i, 1)->text()
                + ";" + ui->tableEntry->item(i, 2)->text();
        out << s + "\n";
    }

    f.close();
    systIcon->hide();
    event->accept();
}

void mainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        systIcon->hide();
        show();
    break;

    default:
        ;
    }
}

void mainWindow::minimizeWindow()
{
    systIcon->show();
    hide();
}

void mainWindow::updateList()
{
    listToLook->clear();
    for(int i = 0; i < ui->tableEntry->rowCount(); i++)
    {
        persoCritere p;
        p.format = ui->tableEntry->item(i, 0)->text();
        p.team = ui->tableEntry->item(i, 1)->text();
        p.title = ui->tableEntry->item(i, 2)->text();

        listToLook->append(p);
    }
}

mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::loadJsonSource(QNetworkReply *r)
{
    QList<Entry>* listEntry = new QList<Entry>();
    jsonSource = r->readAll();
    jsonDocument = QJsonDocument::fromJson(jsonSource.toUtf8());

    QFile f;
    f.setFileName("cfg/last.json");
    f.open(QIODevice::ReadWrite);
    QString s = f.readAll();

    if(s == jsonSource)
    {
        return;
    }

    checkDocument = QJsonDocument::fromJson(s.toUtf8());

    if(jsonDocument.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Unable to read JSON document");
    }

    QJsonArray array = jsonDocument.object().value("releases").toArray();
    QJsonArray checkArray = checkDocument.object().value("releases").toArray();

    foreach(QJsonValue v, array)
    {
        Entry e;
        QJsonObject o = v.toObject();
        if(!checkArray.contains(v))
        {
            e.name = o.value("name").toString();
            e.cep = o.value("cep").toString();
            e.clink = o.value("clink").toString();
            listEntry->append(e);
        }
    }

     checkEntries(listEntry);
     f.resize(0);
     f.write(jsonSource.toUtf8());
     f.close();
}

void mainWindow::checkEntries(QList<Entry> *list)
{
    updateList();
    for(int j = 0; j < listToLook->count(); j++)
    {
        QRegExp r("*" + listToLook->at(j).format + "*", Qt::CaseInsensitive,QRegExp::Wildcard);
        QRegExp re("*"+listToLook->at(j).team+"*", Qt::CaseInsensitive,QRegExp::Wildcard);

        for(int i = 0; i < list->count(); i++)
        {
            Entry e = list->at(i);

            if(e.name == listToLook->at(j).title)
            {
                if(r.exactMatch(e.cep))
                {
                    if(!listToLook->at(j).team.isEmpty())
                    {
                        if(re.exactMatch(e.cep))
                        {
                            newEntries->append(e);
                            systIcon->showMessage("Nouveau !", "Nouveau " + e.name + "\n" + "de " + listToLook->at(j).team,
                                                  QSystemTrayIcon::Information, 2000);
                            if(settings->value("download").toBool())
                            {
                                downloadFile(e.clink);
                            }
                        }
                    }
                    else
                    {
                        newEntries->append(e);
                        systIcon->showMessage("Nouveau !", "Nouveau " + e.name + "\n", QSystemTrayIcon::Information, 2000);
                        if(settings->value("download").toBool())
                        {
                            downloadFile(e.clink);
                        }
                    }
                }
            }
        }
    }
}

void mainWindow::getJsonSource()
{
    nam->get(QNetworkRequest(QUrl("http://haruhichan.com/feed/feed.php?mode=json")));
}

void mainWindow::addEntry()
{
    addDialog* ad = new addDialog(this);

    ad->setWindowTitle("Ajouter");

    if(ad->exec() != QDialog::Rejected)
    {
        QString title = ad->getTitle();
        QString team = ad->getTeam();
        QString format = ad->getFormat();

        if((title.isEmpty()) || (format.isEmpty()))
        {
            QMessageBox::critical(this, "Erreur", "Vous devez au moins préciser le titre et le format !");
            return;
        }
        else
        {
            ui->tableEntry->setRowCount(ui->tableEntry->rowCount()+1);
            ui->tableEntry->setItem(ui->tableEntry->rowCount()-1, 0, new QTableWidgetItem(format));
            ui->tableEntry->setItem(ui->tableEntry->rowCount()-1, 1, new QTableWidgetItem(team));
            ui->tableEntry->setItem(ui->tableEntry->rowCount()-1, 2, new QTableWidgetItem(title));
        }

        updateList();
    }
}

void mainWindow::deleteEntry()
{
    if(ui->tableEntry->currentIndex().isValid())
    {
        ui->tableEntry->removeRow(ui->tableEntry->currentRow());
        updateList();
    }
}

void mainWindow::editEntryDoubleClick(QModelIndex i)
{
    int row = i.row();
    QString cTitle = ui->tableEntry->item(row, 2)->text();
    QString cTeam = ui->tableEntry->item(row, 1)->text();
    QString cFormat = ui->tableEntry->item(row, 0)->text();

    addDialog* ad = new addDialog(this, cFormat, cTeam, cTitle);

    ad->setWindowTitle("Editer");

    if(ad->exec() != QDialog::Rejected)
    {
        QString title = ad->getTitle();
        QString team = ad->getTeam();
        QString format = ad->getFormat();

        if((title.isEmpty()) || (format.isEmpty()))
        {
            QMessageBox::critical(this, "Erreur", "Vous devez au moins préciser le titre et le format !");
            return;
        }
        else
        {
            ui->tableEntry->setItem(row, 0, new QTableWidgetItem(format));
            ui->tableEntry->setItem(row, 1, new QTableWidgetItem(team));
            ui->tableEntry->setItem(row, 2, new QTableWidgetItem(title));
        }

       updateList();
    }
}

void mainWindow::editEntry()
{
    if(ui->tableEntry->currentIndex().isValid())
    {
        int row = ui->tableEntry->currentRow();
        QString cTitle = ui->tableEntry->item(row, 2)->text();
        QString cTeam = ui->tableEntry->item(row, 1)->text();
        QString cFormat = ui->tableEntry->item(row, 0)->text();

        addDialog* ad = new addDialog(this, cFormat, cTeam, cTitle);

        ad->setWindowTitle("Editer");

        if(ad->exec() != QDialog::Rejected)
        {
            QString title = ad->getTitle();
            QString team = ad->getTeam();
            QString format = ad->getFormat();

            if((title.isEmpty()) || (format.isEmpty()))
            {
                QMessageBox::critical(this, "Erreur", "Vous devez au moins préciser le titre et le format !");
                return;
            }
            else
            {
                ui->tableEntry->setItem(row, 0, new QTableWidgetItem(format));
                ui->tableEntry->setItem(row, 1, new QTableWidgetItem(team));
                ui->tableEntry->setItem(row, 2, new QTableWidgetItem(title));
            }

            updateList();
        }
    }
}
