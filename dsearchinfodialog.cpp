#include "dsearchinfodialog.h"
#include "ui_dsearchinfodialog.h"

// #include <QSqlQueryModel>
#include <QDebug>

#include <QStringListModel>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QKeyEvent>
#include <QDesktopServices>

DSearchInfoDialog::DSearchInfoDialog(QWidget* parent, DDocInfoSqlite* db):
    QDialog(parent), ui(new Ui::DSearchInfoDialog), db_(db) {
    ui->setupUi(this);
    setWindowFlags(Qt::Popup);

    listModel_ = new QStringListModel();
    ui->listView_search->setModel(listModel_);
    ui->listView_search->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->lineEdit_edit->setFocus();
    ui->lineEdit_edit->installEventFilter(this);
    ui->listView_search->installEventFilter(this);

    QFile file(":/qss/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = tr(file.readAll());
    this->setStyleSheet(styleSheet);
    file.close();
}

DSearchInfoDialog::~DSearchInfoDialog()
{
    delete ui;
    if (nullptr != listModel_) {
        delete listModel_;
        listModel_ = nullptr;
    }
}

void DSearchInfoDialog::initQuery() {
    QSqlQuery query("SELECT path FROM searchIndex", db_->database());
    QSqlRecord rec = query.record();
    QStringList strlist;
    int pathcol = rec.indexOf("path"); // index of the field "path"
    while (query.next()){
        strlist<<query.value(pathcol).toString();
    }

    // qDebug()<<"strlist"<<strlist;
    listModel_->setStringList(strlist);
    ui->listView_search->setCurrentIndex(listModel_->index(0, 0));
}

void DSearchInfoDialog::search()
{
    // execute search
    QString edit = ui->lineEdit_edit->text();
    QString querystr = QString("SELECT path FROM searchIndex WHERE name like '%%1%'").arg(edit);
    QSqlQuery query(querystr, db_->database());
    QSqlRecord rec = query.record();
    QStringList strlist;
    int pathcol = rec.indexOf("path"); // index of the field "path"
    while (query.next()){
        strlist<<query.value(pathcol).toString();
    }

    listModel_->setStringList(strlist);
    ui->listView_search->setCurrentIndex(listModel_->index(0, 0));
}

bool DSearchInfoDialog::eventFilter(QObject* obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        return handleKeyPress(obj, event);
    }

    if ( event->type() == QEvent::KeyRelease ) {
    }

    return false;
}

bool DSearchInfoDialog::handleKeyPress(QObject *, QEvent* event) {
    QModelIndex currentIndex = ui->listView_search->currentIndex();
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    int key = keyEvent->key();
    // qDebug()<<"event filter:"<<key<<"-"<<(int)Qt::Key_Enter;
    // Toggle insert completion string
    if (Qt::Key_Up == key) {
        QModelIndex prevIndex = listModel_->index(0, 0);
        if (currentIndex.isValid()) {
            int prevRow = currentIndex.row()-1;
            if (prevRow < 0) {
                prevRow = listModel_->rowCount()-1;
            }
            prevIndex = listModel_->index(prevRow, currentIndex.column());
        }

        if (prevIndex.isValid()) {
            ui->listView_search->setCurrentIndex(prevIndex);
        }
        return (true);
    } else if (Qt::Key_Down == key) {
        QModelIndex nextIndex = listModel_->index(0, 0);
        if (currentIndex.isValid()) {
            int nextRow = currentIndex.row()+1;
            if (nextRow >= listModel_->rowCount()) {
                nextRow = 0;
            }
            nextIndex = listModel_->index(nextRow, currentIndex.column());
        }

        if (nextIndex.isValid()) {
            ui->listView_search->setCurrentIndex(nextIndex);
        }
        return (true);
    } else if (Qt::Key_Return == key) {
        // Key_Enter is small keyborder, return is in big key keyborder
        if (keyEvent->modifiers()&Qt::ControlModifier) {
            // open file
            if (currentIndex.isValid()) {
                QString docpath = listModel_->data(currentIndex).toString();
                // qDebug()<<"open file name is:"<<docpath;
                QDesktopServices::openUrl(QUrl::fromLocalFile(docpath));
            }
            return true;
        } else {
            search();
            return true;
        }
    }

    return false;
}
