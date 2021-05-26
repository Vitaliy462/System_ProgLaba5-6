#include "mainwindow.h"

MainWindow::MainWindow(int nPort, QWidget* pwgt /*=0*/) : QWidget(pwgt)
                                                        , m_nNextBlockSize(0)
{
        QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
        dbase.setDatabaseName("my_db.sqlite");
        if (!dbase.open()) {
            qDebug() << "Что-то не так с соединением!";
        }
        QSqlQuery query("SELECT Text FROM Questions");
        int count=0;
        while (query.next()) {
                 QString q = query.value(0).toString();
                 questions[count]=q;
                 count++;
                 if(count>8){break;}
        }
        QSqlQuery query2("SELECT Text FROM Variants");
        count=0;
        int count2=0;
                while (query2.next()) {
                         QString q = query2.value(0).toString();
                         variants[count2][count]=q;
                         count++;
                         if(count==5){count2++;count=0;}
                }
        QNum=0;
    m_ptcpServer = new QTcpServer(this);
        if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
            QMessageBox::critical(0,
                                  "Server Error",
                                  "Unable to start the server:"
                                  + m_ptcpServer->errorString()
                                 );
            m_ptcpServer->close();
            return;
        }
        connect(m_ptcpServer, SIGNAL(newConnection()),
                this,         SLOT(slotNewConnection())
               );

        m_ptxt = new QTextEdit;
        m_ptxt->setReadOnly(true);

        //Layout setup
        QVBoxLayout* pvbxLayout = new QVBoxLayout;
        pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
        pvbxLayout->addWidget(m_ptxt);
        setLayout(pvbxLayout);
}
void MainWindow::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater())
           );
    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient())
           );

    sendToClient(pClientSocket, "Server Response: Connected!");
}
void MainWindow::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }

        QString* str;
        in >> *str;
        for (int i=0; i<sizeof (str);i++){
            m_ptxt->append(str[i]);
        }
        m_nNextBlockSize = 0;
    }
}
void MainWindow::sendToClient(QTcpSocket* pSocket, const QString& str)
{
    qDebug()<<"sending";
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << questions[QNum]<<variants[QNum][0]<<variants[QNum][1]<<variants[QNum][2]<<variants[QNum][3];
    QNum++;
    m_ptxt->append(questions[QNum]);
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}
MainWindow::~MainWindow()
{
}

