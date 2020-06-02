//д�ڵ����ܵ�С���߼�

#ifndef _UTIL__H_
#define _UTIL__H_
#include<cmath>
#include<iostream>
#include<vector>
#include<fstream>

using DATE = long long;//��һ�鳤���μ�¼������Ϣ
using LUINT = unsigned long long;//�Ǹ�������
using UINT = unsigned int;

//����ģ����пɱ����ʽ��ӡ
template<typename T> print(const T &tar) {
	std::cout << tar << std::endl;
}
template<typename T,typename ...Args> print(const T &tar,const Args ...Lists) {
	std::cout << tar << ' ';
	print(Lists...);
}


//���ں�C����ַ����Ļ���ת��
void date2str(long long a,char *str) {

	//����ʮλ,������һλ
	for(int i = 9; i >= 0; --i) {
		str[i] = (a % 10) + '0';
		a /= 10;
	}
	str[4] = '-';
	str[7] = '-';
	str[10] = '\0';
}


template<typename T>
void VecSave(const char* addr,std::vector<T> &vec) {
	std::ofstream Out(addr,std::ios::out|std::ios::binary);
	Out.write((char*)vec.data(),vec.size()*sizeof(T));
	Out.close();
}

template<typename T>
bool VecRead(const char* addr,std::vector<T> &vec) {
	std::ifstream In(addr,std::ios::in|std::ios::binary);
	//�ж��ļ��Ƿ���� 
	if(!In){
		return false;
	}
	//��ȡdat�ļ���С
	In.seekg(0,std::ios::end);
	UINT size = In.tellg();
	In.seekg(0,std::ios::beg);
	vec.resize(size/sizeof(T));
	In.read((char*)vec.data(),size);
	In.close();
	return true;
}

template<typename T>
bool VecRead(const char* addr,std::vector<T> &vec,UINT size,int offset) {
	std::ifstream In(addr,std::ios::in|std::ios::binary);
	if(!In){
		return false;
	}
	//���ö�ָ��ƫ��
	In.seekg(offset,std::ios::beg);
	vec.resize(size/sizeof(T));
	In.read((char*)vec.data(),size);
	In.close();
	return true;
}

#endif
