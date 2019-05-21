#include "dspicabootable.h"
#include "ui_dspicabootable.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTextDocument>
#include <QTextBlock>

dspicabootable::dspicabootable(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::dspicabootable)
{
    ui->setupUi(this);
    port=new QSerialPort(this);
    bl_stage=0;

    console_tab_enabled=false;
    if(ui->tabWidget->tabText(ui->tabWidget->currentIndex()).remove("&")=="Console")console_tab_enabled=true;

    testingconnection=false;

    connectiontrys=0;

    m_status=new QLabel(this);
    ui->statusBar->addWidget(m_status);
    showStatusMessage("");

    WelcomeCode code;

    code=WelcomeCode(0,"dsPIC33FJ128GP802","JPIC 1.0",1.0,1.0);
    welcomecodes[code.code]=code;

//    //you could use this for adding new devices
//    QByteArray ba;
//    ba.push_back((char)0);
//    ba.push_back((char)0);
//    qDebug()<<(qChecksum(ba.data(),2,Qt::ChecksumType::ChecksumIso3309)&0xFF);

    ui->m_console->setEnabled(true);
    ui->m_console->setLocalEchoEnabled(true);
    ui->m_console->setFocus();

    //hide advanced tab
    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tab_advanced));

    timeout = new QTimer(this);
    connect(timeout,SIGNAL(timeout()),this,SLOT(timeout_slot()));
    timeout->setInterval(500);

    QTimer *timeout2 = new QTimer(this);
    connect(timeout2,SIGNAL(timeout()),this,SLOT(combox_timeout()));
    timeout2->setInterval(1000);


    load_comboBox_port();

    QSettings settings;
    ui->lineEdit_filename->setText(settings.value("lineEdit_filename", DEFAULT_FILENAME).toString());
    ui->checkBox_writefuses->setChecked(settings.value("checkBox_writefuses", false).toBool());
    ui->comboBox_port->setCurrentText(settings.value("comboBox_port", "").toString());
    ui->comboBox_mpu->setCurrentText(settings.value("comboBox_mpu", "").toString());
    ui->tabWidget->setCurrentIndex(settings.value("tabWidget", "").toInt());
    ui->checkBox_localecho->setChecked(settings.value("checkBox_localecho",true).toBool());
    ui->comboBox_speed->setCurrentText(settings.value("comboBox_speed", "").toString());
    restoreGeometry(settings.value("Geometry").toByteArray());
    restoreState(settings.value("State").toByteArray());

    connect(port,SIGNAL(readyRead()),this,SLOT(portReadyRead()));
    connect(ui->m_console,SIGNAL(getData(QByteArray)),this,SLOT(console_writeData(QByteArray)));

    loadhexfile();
    if(ui->tabWidget->tabText(ui->tabWidget->currentIndex()).remove("&")=="Console")test_connection();

    timeout2->start();
}

void dspicabootable::putc(const char ch)
{
    if(port->write(&ch,1)!=1)ui->plainTextEdit->appendPlainText("Error writing to the serial port. This is a problem with the serial connection.");
}

Intel8HEXDecoder::DecoderReternValue dspicabootable::get_next_page(QByteArray &hexpage)
{
    quint8 ch;
    while(true)
    {

        if(bl_hextext_ptr>=bl_hextext.size())return Intel8HEXDecoder::DecoderReternValue::NoChar;
        ch=bl_hextext[bl_hextext_ptr++];

        hexpage.append((const char*)&ch,1);

        switch (intel8hexdecoder.decode(ch))
        {
        case Intel8HEXDecoder::DecoderReternValue::NewPage:
            return Intel8HEXDecoder::DecoderReternValue::NewPage;
            break;
        case Intel8HEXDecoder::DecoderReternValue::ErrorFormat:
            return Intel8HEXDecoder::DecoderReternValue::ErrorFormat;
            break;
        case Intel8HEXDecoder::DecoderReternValue::EndOfFile:
            return Intel8HEXDecoder::DecoderReternValue::EndOfFile;
            break;
        case Intel8HEXDecoder::DecoderReternValue::OK:
            break;
        default:
            return Intel8HEXDecoder::DecoderReternValue::ErrorFormat;
            break;
        }

    }

    return Intel8HEXDecoder::DecoderReternValue::ErrorFormat;

}

bool dspicabootable::put_next_page()
{

    //load a page (2048 bytes for the dsPIC33FJ128GP802)
    QByteArray ba;
    Intel8HEXDecoder::DecoderReternValue result=get_next_page(ba);

    //if we cant load a page then return false (EndOfFile and NewPage are pages)
    switch(result)
    {
    case Intel8HEXDecoder::DecoderReternValue::NoChar:
        return false;
        break;
    case Intel8HEXDecoder::DecoderReternValue::NewPage:
        break;
    case Intel8HEXDecoder::DecoderReternValue::ErrorFormat:
        return false;
        break;
    case Intel8HEXDecoder::DecoderReternValue::EndOfFile:
        break;
    case Intel8HEXDecoder::DecoderReternValue::OK:
        return false;
        break;
    default:
        return false;
        break;
    }

    //write it to the serial port
    if(ba.size())
    {
        if(port->write(ba)!=ba.size())
        {
            ui->plainTextEdit->appendPlainText("Error writing to the serial port. This is a problem with the serial connection, maybe there is a buffering issue of the serial port?");
            return false;
        }
    }
    return true;

}

//for testing
bool dspicabootable::get_next_line(QByteArray &hexline)
{
    if(bl_hextext_ptr>=bl_hextext.size())
    {
        ui->plainTextEdit->appendPlainText("No more lines");
        return false;
    }
    ui->plainTextEdit->appendPlainText("loading next line");
    int next_index=bl_hextext.indexOf(':',bl_hextext_ptr+1);
    if(next_index==-1)next_index=bl_hextext.size();
    int len=next_index-bl_hextext_ptr;
    hexline=bl_hextext.mid(bl_hextext_ptr,len);
    bl_hextext_ptr=next_index;


    switch (intel8hexdecoder.decode_line(hexline))
    {
    case Intel8HEXDecoder::DecoderReternValue::NewPage:
        ui->plainTextEdit->appendPlainText("NewPage");
        break;
    case Intel8HEXDecoder::DecoderReternValue::ErrorFormat:
        ui->plainTextEdit->appendPlainText("ErrorFormat");
        break;
    case Intel8HEXDecoder::DecoderReternValue::EndOfFile:
        ui->plainTextEdit->appendPlainText("EndOfFile");
        break;
    case Intel8HEXDecoder::DecoderReternValue::OK:
        ui->plainTextEdit->appendPlainText("OK");
        break;
    default:
        ui->plainTextEdit->appendPlainText("Unknown return type");
        break;
    }


    return true;
}

bool dspicabootable::loadhexfile()
{

    ui->plainTextEdit->clear();

    QString filename=ui->lineEdit_filename->text();

    //init decoder
    quint8 ch;
   // int lastlinecount=0;
    intel8hexdecoder.reset();

    intel8hexdecoder.usefuses=ui->checkBox_writefuses->isChecked();

    //load hex file
    bl_hextext.clear();
    bl_hextext_ptr=0;
    bl_hextext_number_of_lines=-1;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->plainTextEdit->appendPlainText("Error: Can't open file");
        return false;
    }
    bl_hextext=file.readAll();
    bl_hextext=bl_hextext.toUpper();
    file.close();

    ui->plainTextEdit->appendPlainText("Scanning File...");

    //scan through hex file
    while(true)
    {
        if(bl_hextext_ptr>=bl_hextext.size())
        {
            ui->plainTextEdit->appendPlainText("Unexpected End Of File");
            return false;
        }
        ch=bl_hextext[bl_hextext_ptr++];

        switch (intel8hexdecoder.decode(ch))
        {
        case Intel8HEXDecoder::DecoderReternValue::NewPage:
 //           qDebug()<<"NewPage";
            break;
        case Intel8HEXDecoder::DecoderReternValue::ErrorFormat:
//            qDebug()<<"ErrorFormat";
            ui->plainTextEdit->appendPlainText("Error: File Format");
            return false;
            break;
        case Intel8HEXDecoder::DecoderReternValue::EndOfFile:
            ui->plainTextEdit->appendPlainText("File Read successfully");
            ui->plainTextEdit->appendPlainText(QString::number(intel8hexdecoder.line_count)+" Lines");
   //         qDebug()<<"EndOfFile";

        //    ui->plainTextEdit->appendPlainText(intel8hexdecoder.outputhexdata);


            //bl_hextext=intel8hexdecoder.outputhexdata;
            //bl_hextext_number_of_lines=intel8hexdecoder.line_count;

            //rephreased hex file. this gets rid of the mess that microchip does with their phrasing
            bl_hextext=intel8hexdecoder.form;
            bl_hextext.replace("\n","");
            bl_hextext_number_of_lines=intel8hexdecoder.form.linecount;

            //ui->plainTextEdit->setPlainText(bl_hextext);

            bl_hextext_ptr=0;
            intel8hexdecoder.reset();


//{
//      QByteArray hexline;
//      while(get_next_line(hexline))
//      {
//          ui->plainTextEdit->appendPlainText(((QString)"type=%1 page=%2").arg(intel8hexdecoder.type).arg(intel8hexdecoder.page));
//          ui->plainTextEdit->appendPlainText(hexline+"\n");
//      }
//}

            return true;
            break;
        case Intel8HEXDecoder::DecoderReternValue::OK:
            //qDebug()<<"OK";
            break;
        default:
   //         qDebug()<<"Unknown return type";
            ui->plainTextEdit->appendPlainText("Error: Unknown Type Format");
            return false;
            break;
        }

//        if(lastlinecount!=intel8hexdecoder.line_count)
//        {
//            lastlinecount=intel8hexdecoder.line_count;
//            qDebug()<<"Line count="<<lastlinecount;
//        }



    }

    ui->plainTextEdit->appendPlainText("Error: No End Of File Type Given");

    return false;

}

void dspicabootable::timeout_slot()
{
    bool okconnection=false;
    if(!timeout->isActive())return;
    if((bl_stage<5)&&(bl_stage>0))
    {
        ui->plainTextEdit->appendPlainText("Timeout: Got some responce from device I just don't know what it is.");
    }
     else
     {
        switch (bl_stage)
        {
        case 0:
            ui->plainTextEdit->appendPlainText("Timeout: No responce");
            showStatusMessage("Timeout: No responce");
            break;
        case 5:
            ui->plainTextEdit->appendPlainText("Timeout: Connection lost");
            showStatusMessage("Timeout: Connection lost");
            break;
        case 6:
            if(bl_rxed_number_of_acks==bl_hextext_number_of_lines)
            {
                ui->plainTextEdit->appendPlainText("Success: All done");
                showStatusMessage("Success: All done");
            }
             else
             {
                ui->plainTextEdit->appendPlainText("Failure: Did not receive the expected number of ACKs from device.");
                showStatusMessage("Failure: Did not receive the expected number of ACKs from device.");
             }
            okconnection=true;
            break;
        case 11:
            if(!testingconnection)
            {
                ui->plainTextEdit->appendPlainText("Fail: Error writting data");
                showStatusMessage("Fail: Error writting data");
            }
             else
             {
                ui->plainTextEdit->appendPlainText("Fail: Error connecting with device");
                showStatusMessage("Fail: Error connecting with device");
             }
            break;
        case 12:
            ui->plainTextEdit->appendPlainText("Fail: Error writting data due to local PC copy of program");
            showStatusMessage("Fail: Error writting data due to local PC copy of program");
            break;
        case 13:
            if(console_tab_enabled)
            {
                showStatusMessage("Port opened");
                ui->m_console->portopen=true;
                break;
            }
            ui->plainTextEdit->appendPlainText("Connection OK");
            if(bl_hextext_number_of_lines<=0)showStatusMessage("Connection OK but can't burn!!!");
             else
             {
                showStatusMessage("Connection OK");
                okconnection=true;
             }
            break;
        default:
            ui->plainTextEdit->appendPlainText("Timeout: Unknown");
            showStatusMessage("Timeout: Unknown");
            break;
        }

     }
    timeout->stop();
    if(okconnection)ui->pushButton_burn->setEnabled(true);
    ui->tabWidget->setEnabled(true);
}

void dspicabootable::portReadyRead()
{
    QByteArray ba=port->readAll();

    if(console_tab_enabled&&(bl_stage==13))ui->m_console->putData(ba);

    if(bl_stage==5)timeout->start();

    for(int i=0;i<ba.size();i++)
    {

        //start off by waiting for the SYNC from the device
        if(bl_stage==0)
        {
            if(ba[i]==(char)SYNC)
            {
                putc(ESC);
                bl_stage=1;
            }
            continue;
        }

        //then wait for the SOH. if it didn't hear our ESC and it sends a SYNC again then send the ESC again. anything else makes us go back to stage 0
        if(bl_stage==1)
        {
            if(ba[i]==SOH)
            {
                //ui->plainTextEdit->appendPlainText("<SOH>");
                bl_stage=2;
                continue;
            }
            if(ba[i]==(char)SYNC)
            {
                putc(ESC);
                continue;
            }
            bl_stage=0;
            continue;
        }

        //fill in welcome packet from bootloader that tells us about the device we are talking to
        if(bl_stage>=2&&bl_stage<=4)
        {
            welcome_packet_from_bootloader.resize(3);
            welcome_packet_from_bootloader[bl_stage-2]=ba[i];
            bl_stage++;
            bl_rxed_number_of_acks=0;
            if(bl_stage==5)
            {
                if(((quint8)welcome_packet_from_bootloader[2])!=(quint8)(qChecksum(welcome_packet_from_bootloader.data(),2,Qt::ChecksumType::ChecksumIso3309)&0x00FF))
                {
                    ui->plainTextEdit->appendPlainText("Device packet = \""+welcome_packet_from_bootloader.toHex()+"\"");
                    ui->plainTextEdit->appendPlainText(((QString)"Device CRC failed (received=0x%1 expected=0x%2)").arg(((quint8)welcome_packet_from_bootloader[2]),2,16,QChar('0')).arg((quint8)(qChecksum(welcome_packet_from_bootloader.data(),2,Qt::ChecksumType::ChecksumIso3309)&0x00FF),2,16,QChar('0'))   );
                    bl_stage=11;
                }

                quint16 welcomecode=(((quint16)((quint8)welcome_packet_from_bootloader[1]))<<8) | ((quint16)((quint8)welcome_packet_from_bootloader[0]));
                if(!welcomecodes.contains(welcomecode))
                {
                  ui->plainTextEdit->appendPlainText("Device responded with ID "+QString::number(welcomecode)+". This number isn't known");
                  bl_stage=11;
                }
                 else
                 {
                    ui->plainTextEdit->appendPlainText(welcomecodes[welcomecode]);
                    if(ui->comboBox_mpu->currentText()!=welcomecodes[welcomecode].mpu_string)
                    {
                        ui->plainTextEdit->appendPlainText("Device responce doesn't match desired MPU in the MPU combo box above");
                        bl_stage=11;
                    }
                     else
                     {
                        if(!testingconnection)ui->plainTextEdit->appendPlainText("Burning...");
                     }

                 }

                if((testingconnection)&&(bl_stage!=11))bl_stage=13;
//stop for testing
//if(bl_stage!=13)bl_stage=11;


            }
            continue;
        }

        if(bl_stage==5)
        {

            switch (ba[i])
            {
            case ACKLOD:
                //ui->plainTextEdit->appendPlainText("<ACK>");
                bl_rxed_number_of_acks++;
                //ui->plainTextEdit->appendPlainText(((QString)"ACKs received %1 Total number of ACKs expected %2").arg(bl_rxed_number_of_acks).arg(bl_hextext_number_of_lines));
                break;
            case NAKLOD:
                ui->plainTextEdit->appendPlainText("<NAK>");
                bl_stage=11;
                break;
            case READY:
                //ui->plainTextEdit->appendPlainText("<READY>");
                {
                    double progress=100.0*(((double)bl_rxed_number_of_acks)/((double)bl_hextext_number_of_lines));
                    QString str="  "+(((QString)"%1% done").arg(  progress,'f',0, 2,QLatin1Char(' '))).trimmed();//"trimmed"???
                    //ui->statusBar->showMessage(str,5000);
                    showStatusMessage(str);
                }
                if(!put_next_page())bl_stage=12;
                break;
            case EOT:
                //ui->plainTextEdit->appendPlainText("<EOT>");
                bl_stage=6;
                break;
            case WRITE_FAIL:
                ui->plainTextEdit->appendPlainText("<WRITE_FAIL>");
                bl_stage=11;
                break;
            case ERROR_UNKNOWN_TYPE:
                ui->plainTextEdit->appendPlainText("<ERROR_UNKNOWN_TYPE>");
                bl_stage=11;
                break;
            case ERROR_BOOTLOADER_ADDRESS_OR_OUT_OF_BOUNDS_ADDRESS_OR_LACK_OF_DATA:
                ui->plainTextEdit->appendPlainText("<ERROR_BOOTLOADER_ADDRESS_OR_OUT_OF_BOUNDS_ADDRESS_OR_LACK_OF_DATA>");
                bl_stage=11;
                break;
            default:
                break;
            }
            continue;
        }

    }

    return;
}

void dspicabootable::console_writeData(const QByteArray &data)
{
    if(console_tab_enabled&&(bl_stage==13))
    {
        port->write(data);
    }
}

void dspicabootable::load_comboBox_port()
{
    QList<QSerialPortInfo> ports=QSerialPortInfo::availablePorts();
    bool changed=false;
    if(ui->comboBox_port->count()!=ports.size())changed=true;
    for(int i=0;(i<ports.size())&&(i<ui->comboBox_port->count())&&(!changed);i++)
    {
        QString str=ports[i].portName()+" ("+ports[i].description()+")";
        if(ui->comboBox_port->itemText(i)!=str)
        {
           changed=true;
        }
    }
    if(!changed)return;
    QString str=ui->comboBox_port->currentText();
    ui->comboBox_port->clear();
    for(int i=0;i<ports.size();i++)ui->comboBox_port->addItem(ports[i].portName()+" ("+ports[i].description()+")");
    ui->comboBox_port->setCurrentText(str);
}

dspicabootable::~dspicabootable()
{
    QSettings settings;
    settings.setValue("lineEdit_filename", ui->lineEdit_filename->text());
    settings.setValue("checkBox_writefuses", ui->checkBox_writefuses->isChecked());
    settings.setValue("comboBox_port", ui->comboBox_port->currentText());
    settings.setValue("comboBox_mpu", ui->comboBox_mpu->currentText());
    settings.setValue("tabWidget",ui->tabWidget->currentIndex());
    settings.setValue("checkBox_localecho", ui->checkBox_localecho->isChecked());
    settings.setValue("comboBox_speed", ui->comboBox_speed->currentText());
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("State", saveState());

    delete ui;
}

void dspicabootable::clear_RequestToSend()
{
   if(port->isOpen())
   {
       //port->setRequestToSend(false);
       port->setDataTerminalReady(true);//sets low
   }
}

void dspicabootable::connect_to_device()
{
    ui->pushButton_burn->setEnabled(false);
    ui->m_console->portopen=false;

    if(port->isOpen())port->close();

    bl_stage=0;
    bl_rxed_number_of_acks=-1;
    bl_hextext_ptr=0;
    if(bl_hextext_number_of_lines<=0)
    {
        ui->plainTextEdit->appendPlainText("No Successfully Loaded Hex File");
        if(!testingconnection)
        {
            showStatusMessage("No Successfully Loaded Hex File");
            //ui->pushButton_burn->setEnabled(true);
            //return;
            testingconnection=true;
        }
    }

    QList<QSerialPortInfo> ports=QSerialPortInfo::availablePorts();
    for(int i=0;i<ports.size();i++)
    {
        QStringList port_name_and_description=ui->comboBox_port->currentText().split(" ");
        QString port_name=port_name_and_description[0];
        if(port_name==ports[i].portName())
        {
            ui->plainTextEdit->appendPlainText("Opening "+port_name);

            port->setPort(ports[i]);

            ui->m_console->setLocalEchoEnabled(ui->checkBox_localecho->isChecked());

            if(console_tab_enabled)
            {
                if(ui->comboBox_speed->currentText()!="Default")
                {
                    port->setBaudRate(ui->comboBox_speed->currentText().toInt());
                }
                 else port->setBaudRate(DEFAULT_SPEED);
                bl_stage=13;
            }
             else port->setBaudRate(DEFAULT_SPEED);
            port->setDataBits(QSerialPort::Data8);
            port->setFlowControl(QSerialPort::NoFlowControl);
            port->setParity(QSerialPort::NoParity);
            port->setStopBits(QSerialPort::OneStop);
            if(!port->open(QIODevice::ReadWrite))
            {
                ui->plainTextEdit->appendPlainText("Can't open com port");
                showStatusMessage("Can't open com port");
                return;
            }

            //disable ui till the burn is done, timeout etc
            if(!testingconnection)ui->tabWidget->setEnabled(false);

            timeout->start();

            //reset device by  pulling down the RTS line for 200ms
            //port->setRequestToSend(true);
        //    port->setDataTerminalReady(false);//sets high
        //    QTimer::singleShot(200, this, SLOT(clear_RequestToSend()));
        }
    }
}

void dspicabootable::test_connection()
{
    connectiontrys++;
    testingconnection=true;
    connect_to_device();
}

void dspicabootable::on_pushButton_burn_clicked()
{
    loadhexfile();
    testingconnection=false;
    connect_to_device();
}

void dspicabootable::on_action_Open_triggered()
{
    QString hexfilename = QFileDialog::getOpenFileName(this,tr("Open Intel8Hex"), ui->lineEdit_filename->text(), tr("Image Files (*.hex)"));
    if(hexfilename.trimmed().isEmpty())hexfilename=ui->lineEdit_filename->text();
    ui->lineEdit_filename->setText(hexfilename);
    loadhexfile();
    test_connection();
}

void dspicabootable::on_toolButton_openhex_clicked()
{
    on_action_Open_triggered();
}

void dspicabootable::on_action_About_triggered()
{

        QMessageBox::about(this,"dsPICaBootable",""
                                         "<H1>A bootloader for PIC microcontrollers</H1>"
                                         "<H3>v1.1</H3>"
                                         "<p>This program can be used to write programms to <a href=\"https://www.microchip.com/design-centers/microcontrollers\">PIC</a> microcontrolers without the need for for an external programming device such as the <a href=\"https://www.microchip.com/developmenttools/ProductDetails/PG164140\">PICkit</a>. Have a look at wikipedia <a href=\"https://en.wikipedia.org/wiki/PIC_microcontrollers\">https://en.wikipedia.org/wiki/PIC_microcontrollers</a> for more info about these PIC chips.</p>"
                                         "<p>For more information about this application see <a href=\"http://jontio.zapto.org/hda1/dspicabootable.html\">http://jontio.zapto.org/hda1/dspicabootable.html</a>.</p>"
                                         "<p>Currently only dsPIC33fj128GP802 has a bootloader written for it.</p>"
                                         "<p>Jonti 2019</p>" );

}

void dspicabootable::on_checkBox_writefuses_stateChanged(int usefuses)
{
    Q_UNUSED(usefuses);
    loadhexfile();
    test_connection();
}

void dspicabootable::on_lineEdit_filename_editingFinished()
{
    loadhexfile();
    test_connection();
}

void dspicabootable::on_checkBox_writefuses_clicked(bool checked)
{

    if(!checked)return;

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Writeing the fuses may brick the chip and requre an ICSP device to reprogram the device. Don't check this unless you really know what you are doing.");
    msgBox.setInformativeText("Are you sure you want to write the fuses?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    switch (msgBox.exec())
    {
    case QMessageBox::Yes:
        return;
        break;
    default:
        break;
    }

    ui->checkBox_writefuses->setChecked(false);

}

void dspicabootable::combox_timeout()
{
    //poll the serial port avalability
    load_comboBox_port();

    //if we cant connect to device then try again
    if((ui->tabWidget->tabText(ui->tabWidget->currentIndex()).remove("&")=="Upload")&&(!timeout->isActive())&&(!ui->pushButton_burn->isEnabled())&&(testingconnection))
    {
        if(connectiontrys<3)
        {
            test_connection();
        }
    } else connectiontrys=0;

    //close port if done
    if((ui->tabWidget->tabText(ui->tabWidget->currentIndex()).remove("&")=="Upload")&&(!timeout->isActive()))port->close();

}

void dspicabootable::on_comboBox_port_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    connectiontrys=0;
    ui->plainTextEdit->clear();
    test_connection();
}

void dspicabootable::on_comboBox_mpu_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    ui->plainTextEdit->clear();
    test_connection();
}

void dspicabootable::writeData(const QByteArray &data)
{
    port->write(data);
}

void dspicabootable::showStatusMessage(const QString &message)
{
    m_status->setText(" "+message+" ");
}

void dspicabootable::on_tabWidget_currentChanged(int index)
{
    if(ui->tabWidget->tabText(index).remove("&")=="Console")
    {
        console_tab_enabled=true;
        ui->m_console->setFocus();
    }
     else console_tab_enabled=false;
    ui->plainTextEdit->clear();
    ui->m_console->clear();
    test_connection();
}
