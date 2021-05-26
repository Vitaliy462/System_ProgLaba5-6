#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include <QTextEdit>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QSqlDatabase>
#include <QSqlQuery>

class MainWindow : public QWidget
{
    Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
    QTextEdit*  m_ptxt;
    quint16     m_nNextBlockSize;
    QString questions[8];
    QString variants[8][4];
    int QNum;

private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);

public:
    MainWindow(int nPort, QWidget* pwgt = 0);
    ~MainWindow();

public slots:
    virtual void slotNewConnection();
            void slotReadClient   ();


};
#endif // MAINWINDOW_H
