#ifndef FULLQUESTION_H
#define FULLQUESTION_H

#include <QString>

class FullQuestion
{
public:
    FullQuestion();
    ~FullQuestion();
    void ParseFQ(QString &fullQ);
    const QString &getQuestion() const;
     QString* getAnswers() ;

private:
    QString fullQ;
    QString answers[4];
    QString question;



};

#endif // FULLQUESTION_H

