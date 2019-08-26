#include "ddocinfosqlite.h"

#include <QtSql>
#include <QDir>
#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>

DDocInfoSqlite::DDocInfoSqlite(QString name)
{
    dbname_ = name;
    bool isExist = QFile::exists(dbname_);
    if (!isExist) {
        sqlData_ = QSqlDatabase::addDatabase("QSQLITE", "ddocument");
    } else  {
        sqlData_ = QSqlDatabase::addDatabase("QSQLITE");
    }

    qDebug()<<"data base:"<<dbname_;
    sqlData_.setDatabaseName(dbname_);
    if (!sqlData_.open()) {
        qDebug() << "open database failed ---" << sqlData_.lastError().text();
    }

    if (sqlData_.isOpen()) {
        checkAndCreateTable();
    }
}

void DDocInfoSqlite::checkAndCreateTable() {
    QStringList tableNameList  = sqlData_.tables();
    qDebug() << "Number of tables = " << tableNameList.count();

    QString tablename("searchIndex");
    bool isFind  = false;
    foreach (QString name, tableNameList) {
        isFind = false;
        // sqlite_sequence 排除
        if (QString("sqlite_sequence") == name) {
            continue;
        }

        if (tablename == name) {
            isFind = true;
            break;
        }
    }

    if (!isFind) {
        QSqlQuery q(sqlData_);
        if (!q.exec("CREATE TABLE IF NOT EXISTS searchIndex ( "
                    "  id INTEGER PRIMARY KEY NOT NULL, "
                    "  name varchar(100),"
                    "  path varchar(200) "
                    "); ")) {
            qFatal("Unable to create table 'query_handler': %s",
                   q.lastError().text().toUtf8().constData());
        }
    }
}

DDocInfoSqlite::~DDocInfoSqlite() {
    sqlData_.close();
}

bool DDocInfoSqlite::isOpen() const {
    return sqlData_.isOpen();
}

bool DDocInfoSqlite::open() {
    return sqlData_.open();
}

void DDocInfoSqlite::clear() {
    QSqlQuery query(sqlData_);
    query.prepare("delete from searchIndex");
    if (!query.exec()) {
        qDebug()<<"DDocInfoSqlite::clear data failed!";
    } else {
        qDebug()<<"DDocInfoSqlite::clear data success!";
        itemCnt_ = 0;
    }
}

void DDocInfoSqlite::fillSearchIndex(const DDocSetting& setting) {
    if (!sqlData_.isOpen()) {
        return ;
    }
    QStringList filters = setting.filter().split(',');
    QStringList folderExclude = setting.folderExclude().split(',');

    // TODO: The depth of find file may be need set
    foreach (QString pathitem, setting.docPathList()) {
        QDir dir(pathitem);
        fillFileFromDir(pathitem, filters);
        foreach (QFileInfo fileinfo, dir.entryInfoList(QDir::Dirs, setting.sortFlag())) {
            QString basename = fileinfo.baseName();
            if (basename.isEmpty())
                continue;

            if (folderExclude.contains(basename)) {
                continue;
            }

            if (fileinfo.isDir()) {
                fillFileFromDir(fileinfo.absoluteFilePath(), filters);
            }
        }
    }
}

void DDocInfoSqlite::fillFileFromDir(const QString& path, QStringList& filters) {
    QDir dir(path);
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);
    QFileInfoList filelist = dir.entryInfoList();
    if (filelist.size() < 1) {
        return ;
    }

    // id, name, fullpath
    QSqlQuery query(sqlData_);
    query.prepare("INSERT INTO searchIndex (id, name, path) VALUES (:id, :name, :path)");
    foreach (QFileInfo fileinfo, filelist) {
        // find and insert it to db
        query.bindValue(":id", itemCnt_);
        query.bindValue(":name", fileinfo.baseName());
        query.bindValue(":path", fileinfo.absoluteFilePath());
        query.exec();

        ++itemCnt_;
    }
}

