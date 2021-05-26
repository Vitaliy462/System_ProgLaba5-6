#include "fullQuestion.h"
#include <QString>
#include <QList>

FullQuestion::FullQuestion(){
}
void FullQuestion::ParseFQ(QString &fullQ){
    QStringList slices = fullQ.split("/");
    question=slices.value(0);
    for(int i=1; i<5; i++){
        answers[i-1]=slices.value(i);
    }
}
FullQuestion::~FullQuestion(){}
const QString &FullQuestion::getQuestion() const
{
    return question;
}
QString* FullQuestion::getAnswers() {
    return answers;
}

