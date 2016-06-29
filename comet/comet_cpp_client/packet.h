/*
 * =====================================================================================
 *
 *       Filename:  proto.h
 *
 *    Description:  define the basic message type
 *
 *
 *        Version:  1.0
 *        Created:  06/28/2016 05:10:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#ifndef __PROTO_H__
#define __PROTO_H__

#include<stdint.h>
#include<string>
#include<map>

#define MAX_BODY_SIZE 1<<10

//握手消息类型
#define OP_HANDSHAKE 0
#define OP_HANDSHAKE_REPLY 1

#define OP_HEARTBEAT 2
#define OP_HEARTBEAT_REPLY 3

//普通的文本消息
#define OP_SEND_SMS 4
#define OP_SEND_SMS_REPLY 5

#define OP_DISCONNECT_REPLY 6

#define OP_AUTH 7
#define OP_AUTH_REPLY 8

#define OP_HANDSHAKE_SID 9
#define OP_HANDSHAKE_SID_REPLY 10

#define OP_RAW 11
#define OP_ROOM_READY 12

#define OP_PROTO_READY 13
#define OP_PROTO_FINISH 14

#define OP_TEST 254
#define OP_TEST_REPLY 255

#define CURRENT_VER 1

//define the packet map for debug
extern std::map<int,std::string> packet_operation_map;

#pragma pack(1)

typedef struct PacketHeader
{
	int32_t pack_size; //定义包的大小
	int16_t header_size; //定义包头
	int16_t ver_size; //定义版本号
	int32_t operation_size; //定义操作码大小
	int32_t seqid_size; //定义序列号大小

}PacketHeader;

#pragma pack()


#define PACKET_SIZE sizeof(PacketHeader)

#endif
