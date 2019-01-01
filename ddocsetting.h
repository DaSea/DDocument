#ifndef DDOCSETTING_H
#define DDOCSETTING_H

#include <QStringList>
#include <QDir>

class QSettings;
class DDocSetting
{
public:
    DDocSetting();
    DDocSetting(QString filename);
    ~DDocSetting();

    void setConfigFile(QString filename);

    DDocSetting& operator=(const DDocSetting& setting);
    bool operator==(const DDocSetting& setting);

    const QStringList& docPathList() const { return docPathList_; }

    void setSortFlag(QDir::SortFlag flag) { sortFlag_ = flag; }
    QDir::SortFlag sortFlag() const { return sortFlag_; }

    void setRecursionDepth(int depth) { recursionDepth_ = depth; }
    int recursionDepth() const { return recursionDepth_; }

    void setFilter(QString filter) { filter_ = filter; }
    const QString& filter() const { return filter_; }

    void setFolderExclude(QString folderExclude) { folderExclude_ = folderExclude; }
    const QString& folderExclude() const { return folderExclude_; }

    bool addPath(QString& path);
    bool delPath(QString& path);

    void sync();

private:
    // path list
    QStringList docPathList_;
    QDir::SortFlag sortFlag_;
    // file filter: the name include
    QString filter_;
    // folder exclude
    QString folderExclude_;
    // recursion depth
    int recursionDepth_;

    // config file path
    QString fileName_;
    // write setting to file
    QSettings* qsetting_;
};

#endif // DDOCSETTING_H
