#include "ddocsetting.h"

#include <QSettings>
#include <QDebug>

DDocSetting::DDocSetting()
{
    sortFlag_ = QDir::Name;
    recursionDepth_ = 1;
    filter_ = "*.pdf,*.chm,*.doc";
    folderExclude_ = "";
    // ~/.config/DDocument/config.ini
    qsetting_ = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                              "DDocument", "config");
    qsetting_->beginGroup("path");
    QVariant pathlist = qsetting_->value("list");
    if (pathlist.isValid()) {
        docPathList_ = pathlist.toStringList();
    }
    qsetting_->endGroup();

    qsetting_->beginGroup("filter");
    QVariant filter = qsetting_->value("key");
    if (filter.isValid()) {
        filter_ = filter.toString();
    }
    qsetting_->endGroup();
}

DDocSetting::DDocSetting(QString filename)
{
    sortFlag_ = QDir::Name;
    recursionDepth_ = 1;
    filter_ = "*.pdf,*.chm,*.doc";
    folderExclude_ = "";

    // ~/.config/DDocument/config.ini
    qDebug()<<"setting:"<<filename;
    qsetting_ = new QSettings(filename, QSettings::IniFormat);
    qsetting_->beginGroup("path");
    QVariant pathlist = qsetting_->value("list");
    if (pathlist.isValid()) {
        docPathList_ = pathlist.toStringList();
    }
    qsetting_->endGroup();

    qsetting_->beginGroup("filter");
    QVariant filter = qsetting_->value("key");
    if (filter.isValid()) {
        filter_ = filter.toString();
    }
    qsetting_->endGroup();

    qsetting_->beginGroup("folder_exclude");
    QVariant folder = qsetting_->value("key");
    if (folder.isValid()) {
        folderExclude_ = folder.toString();
    }
    qsetting_->endGroup();
}

DDocSetting::~DDocSetting() {
}

void DDocSetting::setConfigFile(QString filename) {
    // ~/.config/DDocument/config.ini
    if (nullptr != qsetting_) {
        qsetting_->sync();
        delete qsetting_;
        qsetting_ = nullptr;
    }

    qsetting_ = new QSettings(filename, QSettings::IniFormat);
    qsetting_->beginGroup("path");
    QVariant pathlist = qsetting_->value("list");
    if (pathlist.isValid()) {
        docPathList_ = pathlist.toStringList();
    }
    qsetting_->endGroup();

    qsetting_->beginGroup("filter");
    QVariant filter = qsetting_->value("key");
    if (filter.isValid()) {
        filter_ = filter.toString();
    }
    qsetting_->endGroup();

    qsetting_->beginGroup("folder_exclude");
    QVariant folder = qsetting_->value("key");
    if (folder.isValid()) {
        folderExclude_ = folder.toString();
    }
    qsetting_->endGroup();
}

DDocSetting& DDocSetting::operator=(const DDocSetting& other) {
    if (!other.docPathList_.isEmpty())
        docPathList_ = other.docPathList_;

    filter_ = other.filter_;
    folderExclude_ = other.folderExclude_;
    return *this;
}

bool DDocSetting::isSameDocPathList(const DDocSetting& setting) {
    if (setting.docPathList_ == this->docPathList_) {
        return true;
    }

    return false;
}

bool DDocSetting::operator==(const DDocSetting& setting) {
    if (setting.docPathList_ == this->docPathList_) {
        return true;
    }

    return false;
}

bool DDocSetting::addPath(QString& path) {
    // loop, find and push
    bool find = false;
    foreach (const QString &str, docPathList_) {
        if (path == str) {
            find = true;
            break;
        }
    }

    if (!find) {
        docPathList_.push_back(path);
        return true;
    }
    return false;
}

bool DDocSetting::delPath(QString& path) {
    return docPathList_.removeOne(path);
}

void DDocSetting::sync() {
    qsetting_->beginGroup("path");
    qsetting_->setValue("list", docPathList_);
    qsetting_->endGroup();

    qsetting_->beginGroup("filter");
    qsetting_->setValue("key", filter_);
    qsetting_->endGroup();

    qsetting_->beginGroup("folder_exclude");
    qsetting_->setValue("key", folderExclude_);
    qsetting_->endGroup();

    qsetting_->sync();
}
