#include <QString>
#include <list>

class processor
{
private:
    std::list<QChar> operators;
    std::list<double> numbers;
    QString errorMsg;
    QString result;
    std::list<QChar>::iterator opIter;
    std::list<double>::iterator numIter;
    void addCurState();
    void handlePowAndSqrt();
    void handleMultAndDiv();
    void handleSumAndSub();
    void removeOperator();
    void resetProcessing(int &i);
public:
    processor(QString input);
    void calculate();
    QString getResult();
    QString getErrorMsg();

};
