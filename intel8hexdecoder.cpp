#include "intel8hexdecoder.h"

#include <QDebug>
#include <QtEndian>
#include <QString>
#include <QFile>

Intel8HEXDecoder::Intel8HEXDecoder()
{
    reset();
}

void Intel8HEXDecoder::reset()
{
    //init variables that are really important
    address_full=0;
    page=0xFFFF;
    line_count=0;
    outputhexdata.clear();
    usefuses=false;
    line.clear();
    fuse_base_address=0x00F80000;
    form.clear();
}


void Intel8HEXDecoder::hex_digit_to_number(char *ch)
{
    bool bStatus = false;
    QString sValue=QString::fromLatin1(ch,1);
    *ch=sValue.toUInt(&bStatus,16);
}

Intel8HEXDecoder::DecoderReternValue Intel8HEXDecoder::decode_line(QByteArray line)
{
    DecoderReternValue retval=DecoderReternValue::EndOfFile;
    line=line.trimmed();
    for(int i=0;i<line.size();i++)
    {
        quint8 ch;
        ch=line[i];
        retval=(decode(ch));
    }
    return retval;
}

Intel8HEXDecoder::DecoderReternValue Intel8HEXDecoder::decode(quint8 ch)
{

    quint16 proposed_page;


    switch (bl_state)
    {
    case bl_state_waiting_for_cmd:
        if(ch==':')
        {
            line+=ch;
            bl_state=bl_state_waiting_for_high_byte;
            bl_byte_state=bl_byte_state_len;
        }
        break;
    case bl_state_waiting_for_high_byte:
        bl_state=bl_state_waiting_for_low_byte;
        line+=ch;
        hex_digit_to_number((char*)(&ch));
        val=ch<<4;
        break;
    case bl_state_waiting_for_low_byte:
        bl_state=bl_state_waiting_for_high_byte;
        line+=ch;
        hex_digit_to_number((char*)(&ch));
        val|=ch;
        switch (bl_byte_state)
        {
        case bl_byte_state_len:
            bl_byte_state=bl_byte_state_address_high;
            len=val;
            check=val;
            break;
        case bl_byte_state_address_high:
            bl_byte_state=bl_byte_state_address_low;
            address=val<<8;
            check+=val;
            break;
        case bl_byte_state_address_low:
            bl_byte_state=bl_byte_state_type;
            address|=val;
            check+=val;
            break;
        case bl_byte_state_type:
            bl_byte_state=bl_byte_state_data;
            type=val;
            check+=val;
            data_idx=0;
            break;
        case bl_byte_state_data:

            if(len)
            {
                len--;
                data[data_idx]=val;
                data_idx++;
                check+=val;
            }
            else
            {
                line_count++;
                bl_state=bl_state_waiting_for_cmd;
                check=(qint8)(-(qint8)check);
                if(check!=val)
                {
#ifdef Intel8HEXDecoder_DEBUG
                    qDebug()<<"BAD check";
#endif
                    return DecoderReternValue::ErrorFormat;
                    break;
                }

                //line is valid let's work on it
                switch (type)
                {
                    case 1://EOF
#ifdef Intel8HEXDecoder_DEBUG
                        qDebug()<<"EOF";
#endif
                        outputhexdata+=line;line.clear();

//#ifdef Intel8HEXDecoder_DEBUG
                        //this will same the rephrased version of the hex file to disk
                        {
                        QFile file("tmp.hex");
                        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
                        {
                            file.write(form);
                        }
                        }
//#endif


                        return DecoderReternValue::EndOfFile;
                        break;
                    case 4://to tell us to update the high 16bit address field
                        address_full=qFromBigEndian<quint16>(data);
                        address_full<<=16;

                        if(((address_full>>1)<fuse_base_address)||(usefuses))
                        {
                            outputhexdata+=line;
                        }
                        if(((address_full>>1)>=fuse_base_address)&&(!usefuses))line_count--;
                        line.clear();
                        break;
                    case 0://to tell us what to write to the flash
                        //update the low 16bits of the address
                        address_full&=0xFFFF0000;
                        address_full|=address;

                        if(((address_full>>1)<fuse_base_address)||usefuses)
                        {
                            outputhexdata+=line;
                        }
                        if(((address_full>>1)>=fuse_base_address)&&(!usefuses))line_count--;
                        line.clear();

                        proposed_page=(address_full>>11);

                        //save data to the form
                        if(((address_full>>1)<fuse_base_address)||usefuses)
                        {
                            for(int i=0;i<data_idx;i++)
                            {
                                form[address_full+i]=data[i];
                            }
                        }

#ifdef Intel8HEXDecoder_DEBUG
                        //debug print the address and instructions
                        instructions=(quint32*)data;
                        address_full_tmp=address_full>>1;
                        for(int i=0;i<data_idx/4;i++)
                        {
                            qDebug().noquote()<<((QString)"%1  %2 %3").arg(address_full_tmp, 8, 16,QChar('0')).arg(instructions[i], 6, 16,QChar('0')).arg(proposed_page);//.arg(address,4,16,QChar('0')).arg(address_full, 8, 16,QChar('0'));
                            address_full_tmp+=2;
                        }
#endif

                        //if we have a change of page then write the old and read the new page
                        if(page!=proposed_page)
                        {
                            //calc new page in words not bytes
                            page=proposed_page;
                            return DecoderReternValue::NewPage;
                        }

                        return DecoderReternValue::OK;
                        break;
                    default:
#ifdef Intel8HEXDecoder_DEBUG
                        qDebug()<<"unknown type"<<type;
#endif
                        return DecoderReternValue::ErrorFormat;
                        break;
                }

            }

            break;
        default:
            break;
        }

        break;
    default:
        break;
    }


    return DecoderReternValue::OK;

}





