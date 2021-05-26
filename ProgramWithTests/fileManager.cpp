#include "fileManager.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

FileManager::FileManager(){
}
FileManager::~FileManager(){}
QString *FileManager::getFQs(QString &path){
    QFile myFile(path);
    if (myFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       QTextStream in(&myFile);
       QString FQ="";
       int q=0;
       while (!in.atEnd())
       {
           FQ="";
           for(int i=0; i<5; i++){
               QString line = in.readLine();
               FQ+=line;
               FQ+="/";
           }
           fullQs[q]=FQ;

           q++;
       }
       myFile.close();

    }
    return fullQs;
}
void FileManager::saveAns(QString &path, QString UserAnswers[8]){
    if(!path.isEmpty())
    {
        QFile myFile(path);
        if (myFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&myFile);
            QString allText ;
            for(int i=0; i<8; i++){
                allText+=UserAnswers[i];
                allText+=" ";
            }
            out << allText;
            myFile.close();
        }
    }
}
