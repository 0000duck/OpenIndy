#include "console.h"

//initialize static variables
QPointer<Console> Console::myInstance;

/*!
 * \brief Console::Console
 * \param parent
 */
Console::Console(QObject *parent) : QObject(parent){
    this->output.setStringList(this->log);

    //create and open file
    outFile.setFileName("oiLogFile.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
}

Console::~Console() {
    outFile.close();
}

/*!
 * \brief Console::getInstance
 * \return
 */
const QPointer<Console> &Console::getInstance(){
    if(Console::myInstance.isNull()){
        Console::myInstance = new Console();
    }
    return Console::myInstance;
}

/*!
 * \brief Console::getConsoleModel
 * \return
 */
QStringListModel &Console::getConsoleModel(){
    return this->output;
}

/*!
 * \brief Console::addLine
 * \param msg
 * \param msgType
 */
void Console::addLine(const QString &msg, const MessageTypes &msgType){
   this->add(msg, msgType);

}
void Console::add(const QString &msg, const MessageTypes &msgType, const QString &value){

    //update entries list and model
    QString text = QString("[%1] {%2} : %3 %4")
            .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"))
            .arg(getMessageTypeName(msgType))
            .arg(msg)
            .arg(value);
    this->log.append(text);
    this->output.setStringList(this->log);

    //inform about the new line
    // emit this->lineAdded(); // too expensive

    //write the new entry to the log file
    this->writeToLogFile(text);
}

/*!
 * \brief Console::addLine
 * \param msg
 * \param msgType
 * \param value
 */
void Console::addLine(const QString &msg, const MessageTypes &msgType, const bool &value){
    this->add(msg, msgType, value?"true":"false");
}

/*!
 * \brief Console::addLine
 * \param msg
 * \param msgType
 * \param value
 */
void Console::addLine(const QString &msg, const MessageTypes &msgType, const double &value){
    this->add(msg, msgType, QString::number(value, 'f', 6));
}

/*!
 * \brief Console::addLine
 * \param msg
 * \param msgType
 * \param value
 */
void Console::addLine(const QString &msg, const MessageTypes &msgType, const int &value){
    this->add(msg, msgType, QString::number(value));
}

/*!
 * \brief Console::writeToLogFile
 * \param msg
 */
void Console::writeToLogFile(const QString &msg){
    QTextStream textStream(&outFile);
    textStream << msg << endl;
}
