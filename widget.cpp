#include "widget.h"
#include "ui_widget.h"
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QFileDialog>
#include <QString>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    tcpSocket = new QTcpSocket(this);

    this->setWindowTitle("客户端");

    ui->pushButton_2->setEnabled(false);
    ui->pushButton_4->setEnabled(false);


    //QSettings settings("org", "MyApp");
    //QString name = ui->lineEdit->text(); // 获取输入框中的文本
    //settings.setValue("name", NULL); // 将名字保存到配置中

    // 在函数内部创建 QSettings 对象并使用
    QSettings settings("MyCompany", "MyApp");
    QString savedName = settings.value("UserName").toString();

    if (!savedName.isEmpty()) {
        ui->lineEdit_4->setText(savedName);
    }

    connect(tcpSocket, &QTcpSocket::readyRead, this, &Widget::receiveMessages);
    //connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveMessages()));
    //connect(tcpSocket, SIGNAL(QTcpSocket::readyRead()), this, SLOT(receiveMessages()));

    connect(tcpSocket, &QTcpSocket::stateChanged, this, &Widget::mStateChanged);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::receiveMessages()
{

    QByteArray message = tcpSocket->readAll();
    QString receivedString = QString::fromUtf8(message);
    qDebug() <<"收到的消息："<< message <<endl;
    if (receivedString.startsWith("Username:")) {


        qDebug() <<"筛选之后的消息："<< message <<endl;
        // 使用 split 方法分割字符串，以逗号作为分隔符
        QStringList parts = receivedString.split(',');
        // 检查分割后的列表中是否至少有两个元素
        if (parts.size() >= 2) {
            // 提取第二个元素，即用户名
            QString username1 = parts[0].trimmed(); // trimmed() 去除字符串前后的空白
            QStringList parts2 = username1.split(':');
            if (parts2.size() >= 2) {
                QString username = parts2[1].trimmed(); // trimmed() 去除字符串前后的空白
                // 现在 username 变量包含了 "Teddy001"
                // 你可以将 username 添加到 QComboBox 中

                // 检查 QComboBox 中是否已经存在 username
                int index = ui->comboBox->findText(username);

                // 如果不存在，则添加新的用户名到 QComboBox
                if (index == -1) {
                    ui->comboBox->addItem(username);
                }
                qDebug() << "客户端添加username:" << username<<endl;
            }

        }
    } else if(receivedString.startsWith("Removename:")) {

        qDebug() <<"筛选之后的消息："<< message <<endl;
            // 使用 split 方法分割字符串，以逗号作为分隔符
            QStringList parts = receivedString.split(',');
        // 检查分割后的列表中是否至少有两个元素
        if (parts.size() >= 2) {
            // 提取第二个元素，即用户名
            QString username1 = parts[0].trimmed(); // trimmed() 去除字符串前后的空白
            QStringList parts2 = username1.split(':');
            if (parts2.size() >= 2) {
                // 假设 removename 是你想要从 QComboBox 中移除的用户名
                QString removename = parts2[1].trimmed(); // trimmed() 去除字符串前后的空白

                // 找到与 removename 匹配的项的索引
                int index = ui->comboBox->findText(removename);

                // 如果找到了匹配的项，则移除它
                if (index != -1) {
                    ui->comboBox->removeItem(index);
                }
                qDebug() << "客户端去除Removename:" << removename<<endl;
            }

        }

    } else {
        //接收消息
        ui->textBrowser->append("服务端：" + receivedString);
    }

}

void Widget::mStateChanged(QAbstractSocket::SocketState state)
{
    QTcpSocket *socket = (QTcpSocket *)sender();


    switch (state) {
    default:
        break;
    case QAbstractSocket::UnconnectedState:
        ui->textBrowser->append("已服务端断开连接");
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(false);


        break;
    case QAbstractSocket::ConnectedState:
        ui->textBrowser->append("已连接服务端");
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(true);
        QString username = ui->lineEdit_4->text(); // 假设这是你想要发送的用户名

        socket->write(("USERNAME:" + username).toUtf8());
        qDebug() << ("USERNAME:" + username).toUtf8()<<endl;


        break;



    }
}


void Widget::on_pushButton_3_clicked()
{
    //发送消息
    if (tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QString sendMessage = QString("Rereiver:" + ui->comboBox->currentText() + "," + ui->lineEdit->text());
        tcpSocket->write(sendMessage.toUtf8());
    } else {
        ui->textBrowser->append("请先与服务器连接");
    }


//    //发送消息
//    if (tcpSocket->state() == QAbstractSocket::ConnectedState)
//        tcpSocket->write(ui->lineEdit->text().toUtf8());
//    else
//        ui->textBrowser->append("请先与服务器连接");
}

void Widget::on_pushButton_clicked()
{
    //绑定ip地址和端口
    serverIpaddr = this->ui->lineEdit_2->text();
    serverPort = this->ui->lineEdit_3->text();
    quint16 port = serverPort.toUInt();

    // 连接服务端， 指定服务器的IP地址与端口
    tcpSocket->connectToHost(QHostAddress(serverIpaddr), port);


//    QTcpSocket *socket = new QTcpSocket(this);
//    socket->connectToHost("10.6.22.1", 9999);



//   QString username = ui->lineEdit_4->text(); // 假设这是你想要发送的用户名

//   socket->write(("USERNAME:" + username).toUtf8());
//    socket->flush();
}

void Widget::on_pushButton_2_clicked()
{
    // 断开连接
    tcpSocket->disconnectFromHost();

    ui->comboBox->clear();
}

void Widget::on_pushButton_4_clicked()
{

    // 当文本框内容变化时保存名字
    QSettings settings("MyCompany", "MyApp");
    QString name = ui->lineEdit_4->text(); // 获取输入框中的文本
    settings.setValue("UserName", name);

//    // 获取当前工作目录
//    QString currentPath = QDir::currentPath();
//    ui->textBrowser->setText(currentPath);

//    // 使用文件浏览器打开当前工作目录
//    QDesktopServices::openUrl(QUrl::fromLocalFile(currentPath));


//    // 弹出文件对话框，让用户选择目录
//    QString selectedDirectory = QFileDialog::getExistingDirectory(nullptr, "选择工作目录", QDir::currentPath());

//    if (!selectedDirectory.isEmpty()) {
//        // 设置当前工作目录为用户选择的目录
//        QDir::setCurrent(selectedDirectory);

//        // 打印当前工作目录
//        qDebug() << "当前工作目录：" << QDir::currentPath();
//    } else {
//        // 用户取消选择
//        qDebug() << "用户取消选择";
//    }
}


void Widget::on_pushButton_5_clicked()
{
    ui->textBrowser->clear();
}


void Widget::on_lineEdit_4_textChanged(const QString &arg1)
{
//    QSettings settings("org", "MyApp");

//    QString name = ui->lineEdit->text(); // 获取输入框中的文本
//    settings.setValue("name", name); // 将名字保存到配置中




    ui->pushButton_4->setEnabled(true);
}


void Widget::on_comboBox_currentIndexChanged(int index)
{

}

