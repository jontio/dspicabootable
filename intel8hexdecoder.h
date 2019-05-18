#ifndef INTEL8HEXDECODER_H
#define INTEL8HEXDECODER_H
#include <QtGlobal>
#include <QByteArray>
#include <QString>
#include <QVector>
#include <QMap>
#include <QDebug>
#include <QtEndian>

//#define Intel8HEXDecoder_DEBUG

template <typename T>
class BasicLine : public QVector<T>
{
public:
    BasicLine()
    {
        QVector<T>::fill(0,16);
    }
};


//may or maynot fail on fuses, havent checked. but you really shouldnt touch the fuses without a programmer
class BasicForm : public QMap<int,BasicLine<quint8>>
{
public:
    BasicForm()
    {
        clear();
    }

    void clear()
    {
        QMap::clear();
        document.clear();
        linecount=0;
    }

    QString getLine(quint32 linenumber)
    {
        QString stringline;
        BasicLine<quint8> line=QMap::operator [](linenumber);
        quint32 address=linenumber<<4;

        //header
        stringline.append(((QString)":10"));
        stringline.append(((QString)"%1").arg(address&0xFFFF, 4, 16,QChar('0')).toUpper());
        stringline.append(((QString)"00"));
        quint8 linecheck=16;
        linecheck+=(quint8)((address&0xFF00)>>8);
        linecheck+=(quint8)(address&0xFF);

        //bytes
        for(int m=0;m<line.size();m++)
        {
            quint8 abyte=line[m];
            stringline.append(((QString)"%1").arg(abyte, 2, 16,QChar('0')).toUpper());
            linecheck+=(quint8)abyte;
        }

        //checksum
        linecheck=(qint8)(-(qint8)linecheck);
        stringline.append(((QString)"%1").arg(linecheck, 2, 16,QChar('0')).toUpper());

        return stringline;
    }

    QString getHighAddressLine(quint32 linenumber)
    {
        QString stringline;
        quint32 address=linenumber<<4;

        //header
        stringline.append(((QString)":02000004"));
        quint8 linecheck=2;
        linecheck+=4;
        quint16 address_high=((address>>16)&0xFFFF);
        quint8 address_high_bytes[2];
        qToBigEndian<quint16>(address_high,&address_high_bytes);
        linecheck+=address_high_bytes[0];
        linecheck+=address_high_bytes[1];

        //high address bytes
        stringline.append(((QString)"%1").arg(address_high, 4, 16,QChar('0')).toUpper());

        //checksum
        linecheck=(qint8)(-(qint8)linecheck);
        stringline.append(((QString)"%1").arg(linecheck, 2, 16,QChar('0')).toUpper());

        return stringline;
    }

    int linecount;
    QString document;
    QString createDocument()
    {
        document.clear();
        linecount=0;
        QList<int> linenumbers=keys();
        quint32 lastaddress=0xFFFFFFFF;
        for(int i=0;i<linenumbers.size();i++)
        {
            quint32 address=linenumbers[i]<<4;
            if((address>>16)!=(lastaddress>>16))
            {
                document.append(getHighAddressLine(linenumbers[i])+"\n");linecount++;//high address line
                lastaddress=address;
            }
            document.append(getLine(linenumbers[i])+"\n");linecount++;//data line
        }
        document.append(":00000001FF\n");linecount++;//eof line
        return document;
    }

    //for dealing with bytes in addresses
    quint8 &operator[](int address)
    {
        quint32 linenumber=address>>4;
        quint16 columnnumber=(address&0x0F);
        BasicLine<quint8> &line=QMap::operator [](linenumber);
        quint8 &abyte=line.operator [](columnnumber);
        return abyte;
    }

    operator QByteArray()
    {
        createDocument();
        return document.toLatin1();
    }


};


class Intel8HEXDecoder
{
public:
    Intel8HEXDecoder();

    enum class DecoderReternValue { NewPage, ErrorFormat, EndOfFile, OK, NoChar };

    quint16 val;
    enum BL_STATE{bl_state_waiting_for_cmd,bl_state_waiting_for_eol, bl_state_waiting_for_high_byte,bl_state_waiting_for_low_byte};
    enum BL_BYTE_STATE{bl_byte_state_none,bl_byte_state_len,bl_byte_state_address_high,bl_byte_state_address_low,bl_byte_state_type,bl_byte_state_data,bl_byte_state_check};
    BL_STATE bl_state=bl_state_waiting_for_cmd;
    BL_BYTE_STATE bl_byte_state;
    quint16 len;
    quint16 address;
    quint16 type;
    quint8  data[256];
    quint16 data_idx;
    quint16 page;
    quint32 address_full;
    char chtmp[256];
    quint8 check;

    void reset();
    DecoderReternValue decode_line(QByteArray line);
    DecoderReternValue decode(quint8 ch);
    void hex_digit_to_number(char *ch);

    int line_count;

    bool usefuses;

    QByteArray outputhexdata;

    QByteArray line;

    quint32 fuse_base_address;

    QString debugtext;


    //
    BasicForm form;

};

#endif // INTEL8HEXDECODER_H
