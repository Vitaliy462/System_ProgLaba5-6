#include "mainwindow.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QFileDialog>
#include "fileManager.h"
#include "fullQuestion.h"

MainWindow::MainWindow(const QString& strHost,int nPort, QWidget* pwgt /*=0*/ ) : QWidget(pwgt)
                                                                                , m_nNextBlockSize(0)
{
    m_pTcpSocket = new QTcpSocket(this);
    m_pTcpSocket->connectToHost(strHost, nPort);
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));

    setGeometry(500,500,300,500);
//Зчитати з файлу по замовчуванню
   /* QString defaultPath="C:/Users/denko/Desktop/Cpp/cpp/FilesToRead/test.txt";
    applyToFormat(defaultPath);
    currentQ=0;
*/

//Запитання
   // Question = new QTextEdit(questions[currentQ],this);
    Question = new QTextEdit(this);
    Question->setGeometry(10,10,280,150);
    Question->setReadOnly(true);

//Можливі відповіді
    Answers= new QGroupBox(this);
    Answers->setGeometry(50,170,200,100);
    QVBoxLayout *vbox= new QVBoxLayout;
    Buttons=new QRadioButton*[4];
    for(int i=0; i<4; i++){
        Buttons[i]=new QRadioButton(this);
       // Buttons[i]->setText(variants[currentQ][i]);
        vbox->addWidget(Buttons[i]);
    }
    vbox->addStretch(1);
    Answers->setLayout(vbox);
    slotReadyRead();
//Кнопка "Відповісти"
    QPushButton *giveAnswer=new QPushButton("Answer",this);
    giveAnswer->setGeometry(50,270,200,50);
    connect(giveAnswer,SIGNAL(released()),this,SLOT(handleButton()));

//Кнопка "Вибрати файл"
    QPushButton *openFile=new QPushButton("Open File",this);
    openFile->setGeometry(50,390,200,50);
    connect(openFile,SIGNAL(released()),this,SLOT(handleOpenFile()));

//Кнопка "Зберегти відповіді у файл"
    QPushButton *saveFile=new QPushButton("Save Answers",this);
    saveFile->setGeometry(50,330,200,50);
   // connect(saveFile,SIGNAL(released()),this,SLOT(handleSaveFile()));
    connect(saveFile, SIGNAL(clicked()),this,SLOT(slotSendToServer()) );
}

void MainWindow::loadNextQuestion(){
    Question->setText(questions[currentQ]);
    for(int i=0; i<4; i++){
        Buttons[i]->setText(variants[currentQ][i]);
    }
}
void MainWindow::applyToFormat(QString &path){
    FileManager FM;
    FullQuestion FQ;
    QString *FQs=FM.getFQs(path);
    for (int i=0; i<8; i++) {
        FQ.ParseFQ(FQs[i]);
        questions[i]=FQ.getQuestion();
        QString *answers=FQ.getAnswers();
        for (int j=0;j<4 ;j++ ) {
            variants[i][j]=answers[j];
        }
    }
}
void MainWindow::handleButton(){
   for(int i=0; i<4; i++){
       if(Buttons[i]->isChecked()){
           UserAnswers[currentQ]=Buttons[i]->text();
           Buttons[i]->setAutoExclusive(false);
           Buttons[i]->setChecked(false);
           Buttons[i]->setAutoExclusive(true);
           currentQ+=1;
     //      loadNextQuestion();
           slotReadyRead();
       }
   }
}
void MainWindow::handleOpenFile(){
    QString path = QFileDialog::getOpenFileName(this, "Open Text File", "C:/Temp", "Text files(*.txt);;All files (*.*)");
    currentQ=0;
    applyToFormat(path);
    loadNextQuestion();
}
void MainWindow::handleSaveFile(){
    QString path = QFileDialog::getSaveFileName(this, "Save Text File", "C:/Temp", "Text files(*.txt);;All files (*.*)");
    FileManager FM;
    FM.saveAns(path,UserAnswers);
}
void MainWindow::slotSendToServer(){
    QByteArray  arrBlock;
       QDataStream out(&arrBlock, QIODevice::WriteOnly);
       out.setVersion(QDataStream::Qt_4_2);
       out << quint16(0) << *UserAnswers;
       out.device()->seek(0);
       out << quint16(arrBlock.size() - sizeof(quint16));
}
void MainWindow::slotReadyRead(){
    qDebug()<<"reading";
    QDataStream in(m_pTcpSocket);
        in.setVersion(QDataStream::Qt_4_2);
        for (;;) {
            if (!m_nNextBlockSize) {
                if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) {
                    break;
                }
                in >> m_nNextBlockSize;
            }

            if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
                break;
            }
            QString  q;
            QString var1;
            QString var2;
            QString var3;
            QString var4;
            in >> q >> var1>>var2>>var3>>var4;
            qDebug()<<q;
            qDebug()<<var1;
            Question->setText(q);
            Buttons[0]->setText(var1);
            Buttons[1]->setText(var2);
            Buttons[2]->setText(var3);
            Buttons[3]->setText(var4);
            m_nNextBlockSize = 0;
        }
}
MainWindow::~MainWindow()
{

}

