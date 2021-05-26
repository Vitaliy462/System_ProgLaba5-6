#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QGroupBox>
#include <QRadioButton>
#include <QString>
#include <QTcpSocket>
#include <QLineEdit>
#include "fileManager.h"
#include "fullQuestion.h"
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(const QString& strHost, int nPort, QWidget* pwgt = 0);
    ~MainWindow();
private:
    int currentQ;
    QTextEdit *Question;
    QGroupBox *Answers;
    QRadioButton** Buttons;

    QTcpSocket* m_pTcpSocket;
    quint16     m_nNextBlockSize;

    QString UserAnswers[8];
    QString questions[8];
    QString variants[8][4];

    void loadNextQuestion();
    void applyToFormat(QString &path);

private slots:
    void handleButton();
    void handleOpenFile();
    void handleSaveFile();
    void slotSendToServer();
    void slotReadyRead();
};
#endif // MAINWINDOW_H
