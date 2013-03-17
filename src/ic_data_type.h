#ifndef IC_DATA_TYPE_H
#define IC_DATA_TYPE_H

#include <gtk/gtk.h>

#define DATA_TYPE_USERINFO	1
#define DATA_TYPE_MESSAGE	2

#define STATUS_ONLINE  0
#define STATUS_CALLME  1
#define STATUS_HIDDEN  2
#define STATUS_BUSY    3
#define STATUS_AWAY	   4 
#define STATUS_OFFLINE 5

#define BUFSIZE 1024*5
#define SERV_ADDR "127.0.0.1"

typedef struct _IcData IcData;
struct _IcData
{
	int type;
	gpointer data;
};

typedef struct{
	char user_name[20];
	char nick_name[20];
	char sex;
	int age;
	char email[30];
	char signature[100];
	GList *friend_list;
	int status;	
	char ip[16];
	int port;
}UserInfo;

typedef struct{
	char friend_name[20];
	char nick_name[20];
	char relation[20];
	char signature[100];
	int status;
	char ip[16];
	int port;
}FriendInfo;

typedef struct{
	long mesg_id;
	char sender[20];
	char receiver[20];
	long long time;
	char content[BUFSIZE];
}Message;

#endif
