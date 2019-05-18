
#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>

class Console : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void getData(const QByteArray &data);

public:
    explicit Console(QWidget *parent = nullptr);

    void putPlainData(const QByteArray &_data);
    void putData(const QByteArray &data);

    void setLocalEchoEnabled(bool set);

    bool portopen;

protected:
    void keyPressEvent(QKeyEvent *e) override;
    //void mousePressEvent(QMouseEvent *e) override;
    //void mouseDoubleClickEvent(QMouseEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

private:
    bool m_localEchoEnabled = false;
    QAction *copyAct;
    QAction *copyallAct;
    QAction *sendtextAct;
    QAction *clearAct;

    void putData_act(const QByteArray &data);
    QTimer *timer;
    QByteArray data_buffer;

private slots:
    void copySlot();
    void copyallSlot();
    void sendtextSlot();
    void clearSlot();

    void timerSlot();

};

#endif // CONSOLE_H
