#ifndef MSG_TYPE_H
#define MSG_TYPE_H


typedef _int64 U64 ;
typedef unsigned int U32 ;
typedef unsigned short U16 ;
typedef char U8 ;

#define HEARTBEAT 0x01
#define CELLCONFIG 0x02
#define RUNNINGMODE 0x03
#define TURNONRF 0x04
#define TURNOFFRF 0x05
#define RESTARTSYSTEM 0x06
#define RESTARTCARD 0x07
#define CARRFREPOINTCHECK 0x08

#define ADDBLACKIMSI 0x50
#define ADDWHITEIMSI 0x51
#define DELETEBLACKIMSI 0x52
#define DELETEWHITEIMSI 0x53
#define CLEARBLACKIMSI 0x54
#define CLEARWHITEIMSI 0x55
#define CHECKBLACKIMSI 0x56
#define CHECKWHITEIMSI 0x57
#define IMSIREPORT 0x18

#pragma pack(1)

typedef struct msgInfo
{
	int ipIndex; 
	U8 carrierIndex;	
	char msgType;		
} MsgInfo;

typedef struct imsiCheckInfo
{
	int ipIndex; 		
	U8 carrierIndex;	
	char msgType;
	char imsi[15];
} ImsiCheckInfo;


typedef struct imsiReportInfo
{
	int ipIndex; 		//ip±êºÅ
	U8 carrierIndex;	//0:ÔØ²¨Ò»;1:ÔØ²¨¶þ
	char imsi[15];
	char imei[15];
}ImsiReportInfo;


typedef struct carrFrePointCheckInfo{
	int ipIndex; 
	U8 carrierIndex;
	int carrFrePoint;
}CarrFrePointCheckInfo;


typedef struct msgHeader{
	U32 msgLenth;
	U8  msgSerialNum;
	U8 msgCodeNum;
	U8 msgCarrier;
	U8 msgParam;
}MsgHeader;

typedef struct unit_0{
	U8 lenth;
	U16 serialNum;
}Unit_0;

typedef struct unit_1{
	U8 lenth;
	U16 serialNum;
	U8 data;
}Unit_1;

typedef struct unit_4{
	U8 lenth;
	U16 serialNum;
	U8 data[4];
}Unit_4;

typedef struct unit_8{
	U8 lenth;
	U16 serialNum;
	U8 data[8];
}Unit_8;

typedef struct cellConfig{
	MsgHeader header;
	Unit_8  mcc;
	Unit_8  mnc;
	Unit_8  lac;
	Unit_8  ci;
	Unit_4  tac;
	Unit_8  carrierFrePoint;
	Unit_8  downAttenuation;
}CellConfig;

typedef struct runningMode{
	MsgHeader header;
	Unit_1  controlMode;
	Unit_1  workMode;
	Unit_1 msgSendMode;
}RunningMode;

typedef struct unit_35{
	U8 lenth;
	U16 serialNum;
	U8 data[35];
}Unit_35;

typedef struct dataInfo_0{
	MsgHeader header;
	Unit_0 msgInfo;
}DataInfo_0;

typedef struct dataInfo_35{
	MsgHeader header;
	Unit_35 msgInfo;
}DataInfo_35;

typedef struct unit_20{
	U8 lenth;
	U16 serialNum;
	U8 data[20];
}Unit_20;

typedef struct getImsiReport{
	MsgHeader header;
	Unit_4 data1;
	Unit_20 data2;
	Unit_20 data3;
	Unit_4 data4;
}GetImsiReport;


typedef struct carrFrePointCheck{
	MsgHeader header;
	Unit_8  carr;
}CarrFrePointCheck;

#pragma pack()


#endif
