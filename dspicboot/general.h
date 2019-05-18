#ifndef _GENERAL_
#define _GENERAL_

#ifdef Q_IDE
#include <QtGlobal>
#define int1 int
#define uint8 quint8
#define uint16 quint16
#define uint32 quint32
#define uint64 quint64
#define int16 qint16
#define int32 qint32
#define int64 qint64
#define float32 float
#define float64 double
#define bool bool
#else
#define bool BOOLEAN
#define uint8 unsigned int8
#define uint16 unsigned int16
#define uint32 unsigned int32
#define uint64 unsigned int64
#endif


#endif
