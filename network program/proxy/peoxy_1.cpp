/*
* 参考自Like Zhang   Email: lzhang@cs.utsa.edu   
*
* 该程序为了简洁，端口默认为80
*
*/
#include "StdAfx.h"
#include "winsock2.h"
#include <process.h>
#include <iostream>
#include <string>
#include <queue>
#include <winsock.h>
#pragma comment(lib,"wsock32.lib")
#include <stdlib.h>
#include <stdio.h>
#define SPACE_CHAR  ' '
#define RESPONSE_BUFFER_SIZE 8092

using namespace std;
queue<SOCKET> socketBuf;
HANDLE queueMutex;
HANDLE threadSymaphore;
const int MAX_THREADS = 40;

DWORD WINAPI requestThread( LPVOID lpParam );

//得到浏览器访问的主机
void GetClient(const char* request,string &host);  //得到浏览器请求的ip port等

//访问服务器，下载数据同时发送给浏览器
void GetServerResponse(int threadId,const char *target_url,
	const char *target_port,const char *request,int sockfd);
//判断是否Alive
bool isAlive(const char *buffer,int len);
int main(int argc, char* argv[])
{
	SOCKET serverSock,clientSock;
	sockaddr_in ser_addr;
	sockaddr_in remote_addr;
	int len;
	int i;

	char buf[1024];
	WSAData wsa;

	WSAStartup(0x101,&wsa);
	serverSock = socket(AF_INET,SOCK_STREAM,0);
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	ser_addr.sin_port = htons(8000);

	bind(serverSock,(sockaddr*)&ser_addr,sizeof(ser_addr));

	listen(serverSock,5);

	const int ThreadNum = MAX_THREADS;
	DWORD dwThreadId[ThreadNum], dwThrdParam[ThreadNum];
	HANDLE threads[ThreadNum];

	//建立ThreadNum个线程
	for(int i=0; i<ThreadNum; i++)
	{
		dwThrdParam[i] = i;
		threads[i] = CreateThread(
			NULL,
			0,
			requestThread,
			&dwThrdParam[i],
			0,
			&dwThreadId[i]);
	}

	while(true)
	{
		clientSock = accept(serverSock, NULL, NULL);
		WaitForSingleObject(queueMutex,INFINITE);
		socketBuf.push(clientSock);
		ReleaseMutex(queueMutex);
	}

	closesocket(serverSock);
	WSACleanup();
	return 0;
}

DWORD WINAPI requestThread( LPVOID lpParam )
{
	int id = *(int *)lpParam;
	while(true)
	{
		SOCKET clientSock;

		WaitForSingleObject(queueMutex,INFINITE);

		if(socketBuf.size()<1)
		{
			ReleaseMutex(queueMutex);
			continue;
		}

		try
		{
			clientSock=socketBuf.front();
			socketBuf.pop();
			ReleaseMutex(queueMutex);
		}catch(exception)
		{
			ReleaseMutex(queueMutex);
			continue;
		}

		try{

			const int bufferSize=1024;
			char buf[bufferSize];
			memset(buf,0,bufferSize);

			string data="";

			int recvLen=recv(clientSock,buf,bufferSize-1,0);
			if(recvLen<=0)
			{
				return 0;
			}
			data+=buf;
			printf("%s\n", buf);
			while(recvLen>=bufferSize-1)
			{
				memset(buf,0,bufferSize);
				recvLen=recv(clientSock,buf,bufferSize-1,0);
				data+=buf;
			}
			string targetURL="";
			string Port="80";
			GetClient(data.c_str(),targetURL);
			GetServerResponse(id,targetURL.c_str(),Port.c_str(),data.c_str(),clientSock);
			closesocket(clientSock);
			cout<<"Thread "<<id<<" socket closed"<<endl;
		} catch(exception)
		{
			shutdown(clientSock,2);
			closesocket(clientSock);
		}
	}

	return 0;


}

//查找主机名
void GetClient(const char* request,string &host)
{

	host="";
	string tmp = request;
	int start,end;
	start = tmp.find("www");
	end = tmp.find("/", start);
	char tt[20];
	memset(tt,0,20);
	memcpy(tt, request+start, end-start);
	host = tt;
}

void GetServerResponse(int threadId,const char *target_url,
	const char *target_port,const char *request,int sockfd)
{
	int request_sockfd;
	struct sockaddr_in client_addr;
	struct hostent *phost;
	bool keep_alive = false;

	request_sockfd = socket(AF_INET,SOCK_STREAM,0);  //连接服务器
	phost = gethostbyname(target_url);
	memset(&client_addr,0,sizeof(struct sockaddr_in));
	if(phost == NULL){
		cout<<"get host name error"<<endl;
		return;
	}

	client_addr.sin_family = AF_INET;
	memcpy(&client_addr.sin_addr,phost->h_addr,phost->h_length);
	client_addr.sin_port = htons(atoi(target_port));
	if(connect(request_sockfd,(struct sockaddr *)&client_addr,
		sizeof(struct sockaddr_in)) == -1){
			cout<<"connect error"<<endl;
			closesocket(request_sockfd);
			return;
	}
	
	//发送请求
	if(send(request_sockfd,request,strlen(request),0) == -1){
		cout<<"Error when receiving"<<endl;
		return;
	}
	
	const int buffer_size = RESPONSE_BUFFER_SIZE;
	char recv_buffer[buffer_size];     //接收缓存
	char send_buffer[buffer_size];     //发送缓存
	memset(recv_buffer,0,buffer_size);
	memset(send_buffer,0,buffer_size);
	
	int nread = 0;
	nread = recv(request_sockfd,recv_buffer,buffer_size,0);    //接收服务器的response
	keep_alive = isAlive(recv_buffer,nread);        //检测是否可连通
	
	if(keep_alive){
		do{
			cout<<"Thread "<<threadId<<"[Connection : keep-alive] : "<<nread<<endl;
			memcpy(send_buffer,recv_buffer,buffer_size);
			send(sockfd,send_buffer,nread,0);
			memset(send_buffer,0,buffer_size);
			nread = recv(request_sockfd,recv_buffer,buffer_size,0);
			if(nread <= 0){
				cout<<"recv error"<<endl;
				break;
			}
		}while(true);
	}else{
		do{
			cout<<"Thread "<<threadId<<"[Connection : close] : "<<nread<<endl;
			memcpy(send_buffer,recv_buffer,buffer_size);
			send(sockfd,send_buffer,nread,0);
			nread = recv(request_sockfd,recv_buffer,buffer_size,0);
			if(nread <= 0){
				cout<<"recv error"<<endl;
				break;
			}
		}while(true);
	}
	cout<<"close socket"<<endl;
	closesocket(request_sockfd);
}


bool isAlive(const char *buffer,int len)
{

	int count = 0;
	while(count < len && *buffer != '\0')
	{
		if(strncmp(buffer,"Connection:",strlen("Connection:")) == 0)
		{
			buffer += strlen("Connection:") + 1; 	//跳过 "\nConnection:"
			while(*buffer == SPACE_CHAR)
				++buffer;							//跳过空格
			if(strncmp(buffer,"Close",strlen("Close")) == 0)
				return false;						//
			return true;
		}
		++buffer;
	}
	return true;		

}