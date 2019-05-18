#ifndef DSPICABOOTABLE_H
#define DSPICABOOTABLE_H

#include "intel8hexdecoder.h"
#include "console.h"

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include <QMap>
#include <QLabel>
namespace Ui {
class dspicabootable;
}

//some codes that we may use for comms
//i was going to use the ctrl chars but am just using random lettters
//so the names dont match the ascii codes
#define SOH        'S'//start of comms
#define EOT        'E'//end of comms
#define ACKLOD     'A'//packet received aok
#define NAKLOD     'N'//comms error
#define READY      0x52 //'R' //after a slow operation such as writeing a page to flash the big PC needs to wait for this b4 sending more data. the big pc needs to work out itself when these operations will take place.
#define WRITE_FAIL 0x46 //'F' //as in writing to the flash
#define ERROR_UNKNOWN_TYPE 'U'
#define ERROR_BOOTLOADER_ADDRESS_OR_OUT_OF_BOUNDS_ADDRESS_OR_LACK_OF_DATA 'B'
#define ESC    0x1B
#define SYNC   0x02

#define DEFAULT_FILENAME "[click file->open to select file to open]"

#define DEFAULT_SPEED 921600


class WelcomeCode
{
public:
    int code;
    QString mpu_string;
    QString hardware_string;
    double hardware_version;
    double bootloader_version;
    WelcomeCode(int _code,QString _mpu_string,QString _hardware_string,double _hardware_version,double _bootloader_version)
    {
        mpu_string=_mpu_string;
        hardware_string=_hardware_string;
        hardware_version=_hardware_version;
        bootloader_version=_bootloader_version;
        code=_code;
    }
    WelcomeCode()
    {
       WelcomeCode(-1,"","",0,0);
    }
    operator QString () const
    {
        QString str;
        if(code<0)
        {
            str="Invalid Code ID";
            return str;
        }
        str+="MPU:\t\t"+mpu_string+"\n";
        str+="Hardware:\t\t"+hardware_string+"\n";
        str+="Hardware Version:\t"+QString::number(hardware_version)+"\n";
        str+="Bootloader Version:\t"+QString::number(bootloader_version)+"\n";
        str+="Code ID:\t\t"+QString::number(code);
        return str;
    }
};

class dspicabootable : public QMainWindow
{
    Q_OBJECT

public:
    explicit dspicabootable(QWidget *parent = 0);
    ~dspicabootable();
    void load_comboBox_port();

private slots:
    void on_pushButton_burn_clicked();
    void portReadyRead();
    void console_writeData(const QByteArray &data);
    void clear_RequestToSend();

    void on_action_Open_triggered();

    void on_toolButton_openhex_clicked();

    void on_action_About_triggered();

    void on_checkBox_writefuses_stateChanged(int usefuses);

    void on_lineEdit_filename_editingFinished();

    void timeout_slot();

    void on_checkBox_writefuses_clicked(bool checked);


    void combox_timeout();

    void on_comboBox_port_currentIndexChanged(int index);

    void test_connection();
    void connect_to_device();
    void on_comboBox_mpu_currentIndexChanged(int index);

    void writeData(const QByteArray &data);
    void showStatusMessage(const QString &message);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::dspicabootable *ui;
    QSerialPort *port;

    int bl_stage;
    int bl_rxed_number_of_acks;
    enum class BL_STAGE{bl_state_none,bl_state_};

    QByteArray bl_hextext;
    int bl_hextext_ptr;
    int bl_hextext_number_of_lines;
    QByteArray welcome_packet_from_bootloader;

    bool bootloader_cmd_state;

    void putc(const char ch);

    bool put_next_page();
    bool get_next_line(QByteArray &hexline);

    Intel8HEXDecoder::DecoderReternValue get_next_page(QByteArray &hexpage);
    bool loadhexfile();

    Intel8HEXDecoder intel8hexdecoder;
    bool putpageresult;

    QTimer *timeout;

    QMap<int,WelcomeCode> welcomecodes;

    bool testingconnection;

    QLabel *m_status = nullptr;

    bool console_tab_enabled;

    int connectiontrys;


};

#endif // DSPICABOOTABLE_H
