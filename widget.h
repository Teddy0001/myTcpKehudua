#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QTcpSocket *tcpSocket;

    QString serverIpaddr;
    QString serverPort;





private slots:
    void receiveMessages();
    void mStateChanged(QAbstractSocket::SocketState);
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_lineEdit_4_textChanged(const QString &arg1);
    void on_comboBox_currentIndexChanged(int index);
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_3_textChanged(const QString &arg1);
    void on_pushButton_6_clicked();
};
#endif // WIDGET_H
