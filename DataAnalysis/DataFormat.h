#pragma once

#ifndef __DATAFORMAT_H__TEST__
#define __DATAFORMAT_H__TEST__

/***************************************************************** 
  Data Packet ���� 
    1. Header   : Packet ����       (nHeader byte) 
    2. Group    : Data Group/Type   (bUseGrout True�϶��� ����)
    3. Data     : nData�� ����      (size�� nValue / Data type�� ���� ����)
    4. Checksum :                   (checksum type > 0)
    5. Tail     : Packet ����       (nTail byte)
*******************************************************************/ 

#define N_MIN_DATA  100000
#define N_MIN_VALUE 16
#define N_MIN_MATH  16

#define N_MAX_DATA  1000000
#define N_MAX_VALUE 1000
#define N_MAX_MATH  1000


typedef struct {
    BYTE nHeader;    
    BYTE bUseGroup;

    BYTE ChecksumType;
    BYTE nTail;
    
    BYTE* pHeader;
    BYTE* pValueType; 
    BYTE *pTail;

    int nValue;
    int nMath;
    int nData;

    int PacketValueType;
} DataFormat_t;


typedef struct {
    BYTE group;
    double *pValue;
    double* pMath;
} Data_t;


typedef enum {
    TYPE_INT32 = 0,
    TYPE_UINT32,
    TYPE_INT8,
    TYPE_UINT8,
    TYPE_INT16,
    TYPE_UINT16,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_LAST,
} valuetype_e;

#endif

