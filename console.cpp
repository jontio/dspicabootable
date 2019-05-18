
#include "console.h"
#include "dspicabootable.h"

#include <QScrollBar>
#include <QMenu>
#include <QDebug>
#include <QShortcut>
#include <QClipboard>
#include <QApplication>

Console::Console(QWidget *parent) :
    QPlainTextEdit(parent)
{
    timer=new QTimer(this);
    timer->setInterval(40);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerSlot()));

    document()->setMaximumBlockCount(10000);

    //i really can't decide on the colors
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    //p.setColor(QPalette::Text, Qt::white);
   // setPalette(p);

    portopen=false;

    copyAct = new QAction(this);
    copyAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    copyAct->setText("Copy");
    connect(copyAct,SIGNAL(triggered(bool)),this,SLOT(copySlot()));

    copyallAct = new QAction(this);
    copyallAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
    copyallAct->setText("Copy All");
    connect(copyallAct,SIGNAL(triggered(bool)),this,SLOT(copyallSlot()));

    sendtextAct = new QAction(this);
    sendtextAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    sendtextAct->setText("Send Text");
    connect(sendtextAct,SIGNAL(triggered(bool)),this,SLOT(sendtextSlot()));

    clearAct = new QAction(this);
    clearAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    clearAct->setText("Clear console");
    connect(clearAct,SIGNAL(triggered(bool)),this,SLOT(clearSlot()));

}

//sort of works but has problems when the top line is deleted, then everything moves up by 1 line. how to fix?
//if someone could do a better job of this that would be good.
//this also takes care of flicker if you are redrawing the screen
void Console::putData(const QByteArray &_data)
{

    //remove any chars of 22 and \r
    QByteArray data=_data;
    data.replace((char)22,"");
//    data.replace("\r",""); //no not \r. why did i want to remove \r??

    //if we get a clear screen then print the remaining data
    if(data.contains((char)2))
    {
        int idx=data.indexOf((char)2);
        if(idx<0)return;
        data_buffer+=data.left(idx);
        data=data.right(data.length()-idx);
        timerSlot();
    }

    data_buffer+=data;

    if(data_buffer.size()>2048)timerSlot();

    if(!data_buffer.isEmpty())timer->start();
}
void Console::timerSlot()
{
    timer->stop();
    putData_act(data_buffer);
    data_buffer.clear();
}
void Console::putData_act(const QByteArray &_data)
{

    //remove any chars of 22 and \r
    QByteArray data=_data;
    data.replace((char)22,"");
//    data.replace("\r",""); //no not \r. why did i want to remove \r??

    //clear screen if we get a char of 2
    while(data.contains((char)2))
    {
        int idx=data.indexOf((char)2);
        if(idx<0)return;
        data=data.right(data.length()-idx-1);
        clear();
        if(data.trimmed().isEmpty())return;
    }

    if(data.isEmpty())return;

    const QTextCursor old_cursor = textCursor();
    const int old_scrollbar_value = verticalScrollBar()->value();
    const bool is_scrolled_down = old_scrollbar_value == verticalScrollBar()->maximum();

    // Move the cursor to the end of the document.
    moveCursor(QTextCursor::End);

    // Insert the text at the position of the cursor (which is the end of the document).
    textCursor().insertText(QString(data));

    if (old_cursor.hasSelection() || !is_scrolled_down)
    {
        // The user has selected text or scrolled away from the bottom: maintain position.
        setTextCursor(old_cursor);
        verticalScrollBar()->setValue(old_scrollbar_value);
    }
    else
    {
        // The user hasn't selected any text and the scrollbar is at the bottom: scroll to the bottom.
        moveCursor(QTextCursor::End);
        verticalScrollBar()->setValue(verticalScrollBar()->maximum());
    }

    if(is_scrolled_down)
    {
        QScrollBar *bar = verticalScrollBar();
        bar->setValue(bar->maximum());
    }
}

void Console::putPlainData(const QByteArray &data)
{
    insertPlainText(data);

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void Console::setLocalEchoEnabled(bool set)
{
    m_localEchoEnabled = set;
}

void Console::keyPressEvent(QKeyEvent *e)
{

    if(e->modifiers().testFlag(Qt::ControlModifier))
    {
        if(e->key()==Qt::Key_C)
        {
            copySlot();
        }
        if(e->key()==Qt::Key_A)
        {
            copyallSlot();
        }
        if(e->key()==Qt::Key_V)
        {
            sendtextSlot();
        }
        if(e->key()==Qt::Key_N)
        {
            clearSlot();
        }
        return;
    }
    switch (e->key()) {
    case Qt::Key_Backspace:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    default:
        if(!portopen)break;
        if (m_localEchoEnabled)
        {
            moveCursor(QTextCursor::End);
            QPlainTextEdit::keyPressEvent(e);
        }
        emit getData(e->text().toLocal8Bit());
    }
}

//void Console::mousePressEvent(QMouseEvent *e)
//{
//    Q_UNUSED(e)
//    setFocus();
//}

//void Console::mouseDoubleClickEvent(QMouseEvent *e)
//{
//    Q_UNUSED(e)
//}

void Console::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(copyAct);
    menu.addAction(copyallAct);
    menu.addSeparator();
    menu.addAction(sendtextAct);
    menu.addSeparator();
    menu.addAction(clearAct);
    menu.exec(event->globalPos());
}

void Console::clearSlot()
{
    clear();
}

void Console::copySlot()
{
    copy();
}

void Console::copyallSlot()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(toPlainText());
}

void Console::sendtextSlot()
{
    if(!portopen)return;
    QClipboard *clipboard = QApplication::clipboard();
    if (m_localEchoEnabled) appendPlainText(clipboard->text());
    emit getData(clipboard->text().toLocal8Bit());
}
