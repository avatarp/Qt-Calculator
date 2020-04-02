#include "processor.h"
#include <list>
#include <QtMath>

processor::processor(QString input)
{
    bool enteringNumber=false;
    bool enteredOperator=false;
    result="";
    QString number;

    for(int i=0;i<input.size();i++)
    {
        if(input[i].isDigit())
        {
            enteredOperator=false;
            if(!enteringNumber)
            {
                number+=input[i];
                enteringNumber=true;
            }
            else
            {
                number+=input[i];
            }
        }

        if(enteringNumber)
        {
            if(input[i]==','||input[i]=='.')
            number+='.';
        }


        if(enteredOperator)
        {
            if(input[i]=="-")
            {
                enteringNumber=true;
                number='-';
                enteredOperator=false;
            }
            else if(input[i]=="√")
            {
                operators.push_back(input[i]);
                enteredOperator=true;
                if(enteringNumber && !number.isEmpty())
                {
                    enteringNumber=false;
                    numbers.push_back(number.toDouble());
                    number.clear();
                }
            }
            else
            {
                errorMsg="Invalid input";
                return;
            }
        }
        else if(input[i]=="+"||input[i]=="-"||
                input[i]=="x"||input[i]=="÷"||
                input[i]=="^"||input[i]=="√")
            {
                if(i==0)
                {
                    if(input[i]=="-")
                    {
                        enteringNumber=true;
                        number='-';
                        enteredOperator=false;
                        operators.push_back(input[i]);
                        enteredOperator=true;
                        if(enteringNumber && !number.isEmpty())
                        {
                            enteringNumber=false;
                            numbers.push_back(number.toDouble());
                            number.clear();
                        }
                    }
                    else if(input[i]=="√")
                    {
                        operators.push_back(input[i]);
                        enteredOperator=true;
                        if(enteringNumber && !number.isEmpty())
                        {
                            enteringNumber=false;
                            numbers.push_back(number.toDouble());
                            number.clear();
                        }
                    }
                    else
                    {
                        errorMsg="Invalid input";
                        return;
                    }
                }
                else
                {
                    operators.push_back(input[i]);
                    enteredOperator=true;
                    if(enteringNumber && !number.isEmpty())
                    {
                        enteringNumber=false;
                        numbers.push_back(number.toDouble());
                        number.clear();
                    }
                    }
            }
    }

    if(!number.isNull()&& !number.isEmpty())
    {
        numbers.push_back(number.toDouble());
    }
    errorMsg="";
}
/*
QString processor::get()
{
    QString result="num: ";
    while(numbers.size()>0)
    {
        result.append(QString::number(numbers.front())+" | ");
        numbers.pop_front();
    }
    result+="\nops: ";
    while(operators.size()>0)
    {
        result.append(operators.front());
        result.append(" | ");
        operators.pop_front();
    }
    return result;
}*/

void processor::addCurState()
{
    std::list<QChar>::iterator opIter = operators.begin();
    std::list<double>::iterator numIter = numbers.begin();

    if(!result.isEmpty())
    {
        result+='\n';
    }

        for(int i=0;i<operators.size();i++)
        {

            if(*opIter=="√")
            {
                result+=*opIter;
                result+=QString::number(*numIter);
            }
            else
            {
                result+=QString::number(*numIter)+ *opIter;
                std::advance(numIter,1);
                result+=QString::number(*numIter);
            }
            result+="|";
            std::advance(opIter,1);
        }
}

void processor::removeOperator()
{
    if(--operators.end()==opIter)
    {
        operators.pop_back();
    }
    else if(operators.begin()==opIter)
    {
        opIter++;
        operators.pop_front();
    }
    else
    {
        operators.erase(opIter);
    }
}

void processor::resetProcessing(int &i)
{
    i=-1;
    opIter=operators.begin();
    numIter=numbers.begin();
}

void processor::handlePowAndSqrt()
{
    opIter = operators.begin();
    numIter = numbers.begin();

    for(int i=0;i<operators.size();i++)
    {
        QChar Operator=*opIter;

        if(Operator=="√")
        {
            if(i!=0 && numIter!=--numbers.end())
            {
                std::advance(numIter,1);
            }
            if(*numIter<0)
            {
                errorMsg="Square of negative number";
                return;
            }
            *numIter=qSqrt(*numIter);
            removeOperator();
            resetProcessing(i);
        }
        else if(Operator=="^")
        {
            std::list<double>::iterator tempNumIter = numIter;
            std::advance(tempNumIter,1);
            *numIter=pow(*numIter,*tempNumIter);
            numbers.erase(tempNumIter);
            removeOperator();
            resetProcessing(i);
        }
        else
        {
            std::advance(numIter,1);
            std::advance(opIter,1);
        }
    }
}

void processor::handleMultAndDiv()
{
    opIter = operators.begin();
    numIter = numbers.begin();

    for(int i=0;i<operators.size();i++)
    {
        QChar Operator=*opIter;

        if(Operator=="x")
        {
            std::list<double>::iterator tempNumIter = numIter;
            std::advance(tempNumIter,1);
            *numIter=(*numIter)*(*tempNumIter);
            numbers.erase(tempNumIter);
            removeOperator();
            resetProcessing(i);
        }
        else if(Operator=="÷")
        {
            std::list<double>::iterator tempNumIter = numIter;
            std::advance(tempNumIter,1);
            if(*tempNumIter==0.0)
            {
                errorMsg="Division by 0";
                return;
            }
            *numIter=(*numIter)/(*tempNumIter);
            numbers.erase(tempNumIter);
            removeOperator();
            resetProcessing(i);
        }
        else
        {
            std::advance(numIter,1);
            std::advance(opIter,1);
        }
    }
}

void processor::handleSumAndSub()
{
    opIter = operators.begin();
    numIter = numbers.begin();

    for(int i=0;i<operators.size();i++)
    {
        QChar Operator=*opIter;
        if(Operator=="-")
        {
            std::list<double>::iterator tempNumIter = numIter;
            std::advance(tempNumIter,1);
            *numIter=(*numIter)-(*tempNumIter);
            numbers.erase(tempNumIter);
            removeOperator();
            resetProcessing(i);
        }
        else if(Operator=="+")
        {
            std::list<double>::iterator tempNumIter = numIter;
            std::advance(tempNumIter,1);
            *numIter=(*numIter)+(*tempNumIter);
            numbers.erase(tempNumIter);
            removeOperator();
            resetProcessing(i);
        }
        else
        {
            std::advance(numIter,1);
            std::advance(opIter,1);
        }

    }
}

QString processor::getErrorMsg()
{
    return errorMsg;
}

QString processor::getResult()
{return result;}

void processor::calculate()
{
    while(operators.size()>0 && errorMsg.isEmpty())
    {
        //addCurState();
        handlePowAndSqrt();
        //addCurState();
        if(errorMsg.isEmpty())
        handleMultAndDiv();
        //addCurState();
        if(errorMsg.isEmpty())
        handleSumAndSub();
    }

    result=QString::number(*numbers.begin());
}
