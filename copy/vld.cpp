/*************************************************************************
	> File Name: vld.cpp
	> Author: zyh_helen
	> Mail: zyh_helen@yeah.net 
	> Created Time: Sun 20 Mar 2016 12:46:31 AM EDT
 ************************************************************************/

#include"vld.h"
hash_table_t ht;
/*初始化哈希表*/
void init_hash_table(){
	for(int i = 0;i < size; ++i){
		ht[i] = NULL;
	}
}

/*
 * 采用头插方式：（原因有两点）
 * 1:提高插入效率  
 * 2:当new的是类对象，由于先构造的对象后析构，当调用析构函数释放空间时
 *   采用头插，后new的对象在前面，因此顺序查找删除时，效率提高
 *
 */
void* operator new(size_t size,char *file,int line,int flag){
	void *result;
	int total_size = size + sizeof(MemNode);      /*开辟的空间包括两部分 1：描述内存的头信息  2：用户申请的实际空间*/
	MemNode *tmp = (MemNode *)malloc(total_size); /*tmp只能看到头信息*/
	assert(NULL != tmp);
	tmp->_file = file;
	tmp->_line = line;
	tmp->_size = size;
	tmp->_next = NULL;

	tmp->_next = ht[flag];
	ht[flag] = tmp;

	result = tmp + 1;        /*略过头信息，返回用户实际申请的用户空间*/
	return result;
}

void* operator new[](size_t size,char *file,int line){
	return operator new(size,file,line,1);
}


/*释放用户开辟的空间*/
void operator delete(void *ptr,int flag){
	if(!ptr || !ht[flag]){
		return;
	}
	MemNode *p = ht[flag];
	if(p + 1 == ptr){    //头删  p+1才是new之后返回给用户的指针
		ht[flag] = p->_next;
		free(p);
	}
	else{      //寻找所删除节点的前一个节点
		while((p->_next + 1) != ptr){
			p = p->_next;
		}
		MemNode *del = p->_next;
		p->_next = del->_next;
		free(del);
	}
}
void operator delete[](void *ptr){
	operator delete(ptr,1);
}


void check_vld(){
	if( !ht[0] && !ht[1] ){
		std::cout<<"no memeroy detected."<<std::endl;
		return;
	}
	else{
		MemNode *cur = ht[0];
		while(cur != NULL){
			std::cout<<"Warning :Visual Leak Detector detected memory"<<std::endl;
			std::cout<<"At "<<cur+1<<":"<<cur->_size<<"Bytes"<<std::endl;
			std::cout<<"file:"<<cur->_file<<" "<<"lines:"<<cur->_line<<std::endl;
			cur = cur->_next;
		}
		cur = ht[1];
		while(cur != NULL){
			std::cout<<"Warning :Visual Leak Detector detected memory"<<std::endl;
			std::cout<<"At "<<cur+1<<":"<<cur->_size<<"Bytes"<<std::endl;
			std::cout<<"file:"<<cur->_file<<" "<<"lines:"<<cur->_line<<std::endl;
			cur = cur->_next;
		}
		cur = ht[1];
	}

}
Vld::Vld(){	
	init_hash_table();
}
Vld::~Vld(){
	check_vld();
}
Vld v;
#define new new(__FILE__,__LINE__)
int main()
{
	int *p = new int[10];
}
