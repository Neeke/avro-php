/**
* Copyright (c) 2015-2018 CloudWise
* Author neeke@php.net
*/

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

char *send_udp_ip = "127.0.0.1";
int  send_udp_port = 26799;

int udp_send(avro_writer_t message,int message_len TSRMLS_DC)
{
    int socket_descriptor;

    struct sockaddr_in address;

    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(send_udp_ip);
    address.sin_port = htons(send_udp_port);

    socket_descriptor = socket(AF_INET,SOCK_DGRAM,0);//IPV4  SOCK_DGRAM 套接字

    /*int PASCAL FAR sendto( SOCKET s, const char FAR* buf, int len, int flags,const struct sockaddr FAR* to, int tolen);　　
    * s：一个标识套接口的描述字。　
    * buf：包含待发送数据的缓冲区。　　
    * len：buf缓冲区中数据的长度。　
    * flags：调用方式标志位。　　
    * to：（可选）指针，指向目的套接口的地址。　
    * tolen：to所指地址的长度。
    */
    int z = sendto(socket_descriptor,message,message_len,0,(struct sockaddr *)&address,sizeof(address));
    if (z < 0) {
        php_printf("send error\n");
    }

    char buf[80];
    sprintf(buf,"stop\n");
    sendto(socket_descriptor,buf,sizeof(buf),0,(struct sockaddr *)&address,sizeof(address));//发送stop 命令
    close(socket_descriptor);

    return SUCCESS;
}