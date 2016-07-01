/*
 * =====================================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  define the main process
 *
 *        Version:  1.0
 *        Created:  06/28/2016 06:31:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#include <stdio.h>

#include <string>
#include <vector>
#include <iostream>

#include "string_util.h"
#include "img_util.h"
#include "net_util.h"
#include "packet.h"
#include "packet_coder.h"
#include "log.h"
#include "cJSON.h"
#include "simple_packet_handler.h"

#define UNUSED_PARAM(param) (void)param

void Usage(int argc,char *argv[])
{
	UNUSED_PARAM(argc);
	std::cerr<<"usage:"<<argv[0]<< " ip:port"<<std::endl;
	exit(1);
}

std::string  BuildAuthData(int64_t user_id,int32_t room_id)
{
	cJSON *jroot=cJSON_CreateObject();

	cJSON_AddItemToObject(jroot,"userid",cJSON_CreateNumber(user_id));
	cJSON_AddItemToObject(jroot,"roomid",cJSON_CreateNumber(room_id));

	std::string str="{\"userid\":0,\"roomid\":1}";

	char *result=cJSON_PrintUnformatted(jroot);

	if(result!=NULL){

		str=result;

		free(result);
	}

	return str;
}

SimplePacketHandler default_handler;

int global_seq=0;

//send heartbeat package
void * HeartBeatThread(void *args)
{
	SimplePacketHandler *handler=(SimplePacketHandler*)args;

	assert(handler!=NULL);

	while(true){

		Packet packet;

		packet.Reset();

		packet.packet_header.seqid_size=global_seq++;
		packet.packet_header.header_size=PACKET_SIZE;
		packet.packet_header.operation_size=OP_HEARTBEAT;

		if(handler->SendPacket(packet)==false){
			log_info("sendPacket failed");
			break;
		}

		log_info("send OP_HEARTBEAT packet");

		sleep(2);
	}

	return NULL;
}

int main(int argc,char *argv[])
{
	if(argc<2)
		Usage(argc,argv);

	//初始化log,打日志
	log_open(stdout);

	std::string host=argv[1];

	std::vector<string> parts;

	img_util::split_string(host,parts,":",2);

	std::string ip="127.0.0.1";
	int port=8080;

	if(parts.size()>0){
		if(parts[0]!="")
			ip=parts[0];
	}

	if(parts.size()>1){

		if(parts[1]!=""){
			port=string_util::convert<int>(parts[1]);
		}
	}

	default_handler.SetHostInfo(ip,port);

	//first we need to send a auth packet
	Packet auth_packet;
	auth_packet.Reset();
	auth_packet.packet_header.seqid_size=global_seq++;
	auth_packet.packet_header.header_size=PACKET_SIZE;
	auth_packet.packet_header.operation_size=OP_AUTH;
	auth_packet.body=BuildAuthData(12345678,15);

	if(default_handler.SendPacket(auth_packet)==false){
		log_info("Send Auth packet failed");
		return 2;
	}

	pthread_t pid=thread_wrapper::start_thread(HeartBeatThread,(void*)&default_handler,false);

	//use the text to control
	//
	while(true){

		Packet * packet=default_handler.RecvPacket();

		if(packet==NULL){

			log_info("default_handler.RecvPacket return NULL");
			continue;
		}

		packet->DebugPacket();

		if(packet->body=="QUIT")
			break;

		//现在没有进行packet复用
		default_handler.FreePacket(packet);
	}

	pthread_cancel(pid);

	//wait for the thread to exit
	pthread_join(pid,NULL);
}
