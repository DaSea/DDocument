#ifndef DDOCINFOSQLITE_H
#define DDOCINFOSQLITE_H

#include <QSqlDatabase>
#include "ddocsetting.h"

/**
 * @brief The DDocInfoSqlite class
 *        Save doc info for search
 */

class DDocInfoSqlite
{
public:
    DDocInfoSqlite(QString name);
    ~DDocInfoSqlite();

    bool isOpen() const;
    bool open();

    void fillSearchIndex(const DDocSetting& setting);

    void clear();

    const QSqlDatabase& database() { return sqlData_; }

private:
    void fillFileFromDir(const QString& path, QStringList& filters);

private:
    // manager sqlite database
    QSqlDatabase sqlData_;
    // fully name
    QString dbname_;
    // item cnt
    int itemCnt_;
};

#endif // DDOCINFOSQLITE_H
