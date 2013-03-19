#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdlib.h>
#include <string.h>
#include "ic_xml_parser.h"
#include "ic_client_context.h"

static UserInfo *xml_parse_userinfo(xmlNodePtr curNode);
static GList *xml_parse_friendlist(xmlNodePtr curNode);

IcData *ic_xml_parser_file(char *filename) {
	xmlDocPtr docPtr=NULL;
	xmlNodePtr curNode=NULL;
	IcData *data = NULL;
	
	xmlKeepBlanksDefault(0);
	docPtr = xmlParseFile (filename);
	curNode = xmlDocGetRootElement(docPtr);
	
	if(xmlStrcmp(curNode->name, BAD_CAST"Data")==0){		
		xmlChar *data_type = NULL;
		data_type = xmlGetProp (curNode, BAD_CAST"type");
		if(xmlStrcmp(data_type, BAD_CAST"user_info") == 0)
		{
			curNode = curNode->xmlChildrenNode;		
			if(xmlStrcmp(curNode->name, BAD_CAST"login")==0){
				xmlChar *result = xmlGetProp(curNode, BAD_CAST"result");	
				if(xmlStrcmp(result, BAD_CAST"true")==0){	
					curNode = curNode->xmlChildrenNode;
					data = (IcData *)malloc(sizeof(IcData));
					UserInfo *user = xml_parse_userinfo(curNode);				
					data->type = DATA_TYPE_USERINFO;
					data->data = (gpointer)user;
							
				}
				xmlFree(result);
			}
		}
		xmlFree(data_type);	
	}
	xmlFreeDoc(docPtr);
	
	return data;
}

static UserInfo *xml_parse_userinfo(xmlNodePtr curNode) {
	xmlNodePtr attrNode=curNode;
	UserInfo *user_info = (UserInfo *)malloc(sizeof(UserInfo));
	xmlChar *result = NULL;	
	
	if(xmlStrcmp(curNode->name, BAD_CAST"user_name")==0){
		result = xmlNodeGetContent(curNode);
		strcpy(user_info->user_name, (char *)result);
		xmlFree(result);
	}
	curNode = curNode->next;
	if(xmlStrcmp(curNode->name, BAD_CAST"nick_name")==0){
		result = xmlNodeGetContent(curNode);
		strcpy(user_info->nick_name, (char *)result);
		xmlFree(result);
	}
	curNode = curNode->next;
	if(xmlStrcmp(curNode->name, BAD_CAST"sex")==0){
		result = xmlNodeGetContent(curNode);
		user_info->sex = (char)result[0];
		xmlFree(result);
	}
	curNode = curNode->next;
	if(xmlStrcmp(curNode->name, BAD_CAST"age")==0){
		result = xmlNodeGetContent(curNode);
		user_info->age = atoi((char *)result);
		xmlFree(result);
	}
	curNode = curNode->next;
	if(xmlStrcmp(curNode->name, BAD_CAST"email")==0){
		result = xmlNodeGetContent(curNode);
		strcpy(user_info->email, (char *)result);
		xmlFree(result);
	}
	curNode = curNode->next;
	if(xmlStrcmp(curNode->name, BAD_CAST"signature")==0){
		result = xmlNodeGetContent(curNode);
		strcpy(user_info->signature, (char *)result);
		xmlFree(result);
	}
	
	curNode = curNode->next;
	if(xmlStrcmp(curNode->name, BAD_CAST"friend_list")==0){
		user_info->friend_list = xml_parse_friendlist(curNode);
	}
	
	curNode = attrNode;
	curNode = curNode->next;
	if(!xmlStrcmp(curNode->name, BAD_CAST"status")){
		result = xmlNodeGetContent(curNode);
		user_info->status = atoi((char *)result);
		xmlFree(result);
	}
	curNode = curNode->next;
	if(!xmlStrcmp(curNode->name, BAD_CAST"ip")){
		result = xmlNodeGetContent(curNode);
		strcpy(user_info->ip, (char *)result);
		xmlFree(result);
	}
	curNode = curNode->next;
	if(!xmlStrcmp(curNode->name, BAD_CAST"port")){
		result = xmlNodeGetContent(curNode);
		user_info->port = atoi((char *)result);
		xmlFree(result);
	}

	return user_info;
}

static GList *xml_parse_friendlist(xmlNodePtr curNode) {
	GList *friend_list=NULL;
	curNode = curNode->xmlChildrenNode;
	xmlNodePtr friendNode = curNode;
	xmlChar *result = NULL;
	while(friendNode != NULL){
		FriendInfo *friend = (FriendInfo *)malloc(sizeof(FriendInfo));
		curNode = friendNode->xmlChildrenNode;		
		
		if(!xmlStrcmp(curNode->name, BAD_CAST"friend_name")){
			result = xmlNodeGetContent(curNode);
			strcpy(friend->friend_name, (char *)result);
			xmlFree(result);
		}
		curNode = curNode->next;
		if(!xmlStrcmp(curNode->name, BAD_CAST"nick_name")){
			result = xmlNodeGetContent(curNode);
			strcpy(friend->nick_name, (char *)result);
			xmlFree(result);
		}
		curNode = curNode->next;
		if(!xmlStrcmp(curNode->name, BAD_CAST"relation")){
			result = xmlNodeGetContent(curNode);
			strcpy(friend->relation, (char *)result);
			xmlFree(result);
		}
		curNode = curNode->next;
		if(!xmlStrcmp(curNode->name, BAD_CAST"signature")){	
			result = xmlNodeGetContent(curNode);
			strcpy(friend->signature, (char *)result);
			xmlFree(result);
		}
		curNode = curNode->next;
		if(!xmlStrcmp(curNode->name, BAD_CAST"status")){
			result = xmlNodeGetContent(curNode);
			friend->status = atoi((char *)result);
			xmlFree(result);
		}
		curNode = curNode->next;
		if(!xmlStrcmp(curNode->name, BAD_CAST"ip")){
			result = xmlNodeGetContent(curNode);
			strcpy(friend->ip, (char *)result);
			xmlFree(result);
		}
		curNode = curNode->next;
		if(!xmlStrcmp(curNode->name, BAD_CAST"port")){
			result = xmlNodeGetContent(curNode);
			friend->port = atoi((char *)result);
			xmlFree(result);
		}
		friend_list = g_list_append(friend_list, friend);
		friendNode = friendNode->next;
	}

	return friend_list;
}

