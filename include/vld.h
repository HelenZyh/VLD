/*************************************************************************
	> File Name: vld.h
	> Author: zyh_helen
	> Mail: zyh_helen@yeah.net 
	> Created Time: Sat 19 Mar 2016 10:25:27 PM EDT
 ************************************************************************/
#pragma once
#include<malloc.h>
#include<iostream>
#include<stdio.h>
#include<assert.h>

/*定义开辟内存节点的信息*/
typedef struct MemNode{
	char *_file;           /*内存泄漏所在的函数*/
	int _line;			   /*内存泄漏所在的行*/
	int _size;             /*泄漏内存的大小*/
	struct MemNode *_next; 
}MemNode;

/*
 * hash_table[0] 链接new开辟的空间
 * hash_table[1] 链接new[]开辟的空间
 * 提高插入删除效率
 *
 */
const int size = 2;
typedef MemNode* hash_table_t[size];/*为了提高查找，删除效率，改链表为哈系表*/

void init_hash_table();
void* operator new(size_t size,char *file,int line,int flag = 0);
void* operator new[](size_t size,char *file,int line);
void operator delete(void *ptr,int flag = 0);
void operator delete[](void *ptr);
void check_vld();

class Vld{
public:
	Vld();	
	~Vld();
};

