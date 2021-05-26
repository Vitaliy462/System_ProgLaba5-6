#ifndef FILEMANAGER_H
#define FILEMANAGER_H


#include <QString>

class FileManager
{
public:
    FileManager();
    ~FileManager();
    QString *getFQs(QString &path);
    void saveAns(QString &path,QString UserAnswers[8]);
private:
    QString fullQs[8];
};

#endif // FILEMANAGER_H
