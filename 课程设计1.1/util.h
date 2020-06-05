//д�ڵ����ܵ�С���߼�

#ifndef _UTIL__H_
#define _UTIL__H_
#include<cmath>
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
using DATE = long long;//��һ�鳤���μ�¼������Ϣ
using LUINT = unsigned long long;//�Ǹ�������
using UINT = unsigned int;

//�˻���� 
enum Type {Student = 0,Teacher};
//����ģʽ 
enum MODE{
	//С���� 
	_byNumber = 0,_byBirth,_byName,_byPlace,_byID,
	//��С 
	_byrNumber,_byrBirth,_byrName,_byrPlace,_byrID
};
//ע����¼״̬ 
enum Statu{
	_dispatch = -1,_fail,_success

};

//�жϺ���ģ�� 
template<typename T>
bool CmpNumber(T a,T b,MODE m){
	return (m == _byNumber)?(a.number()<b.number()):(a.number()>b.number());
}
template<typename T>
bool CmpBirth(T a,T b,MODE m){
	return (m == _byBirth)?(a.birth()<b.birth()):(a.birth()>b.birth());
}
template<typename T>
bool CmpName(T a,T b,MODE m){
	return (m == _byName)?(a.name()<b.name()):(a.name()>b.name());
}
template<typename T>
bool CmpPlace(T a,T b,MODE m){
	return (m == _byPlace)?(a.place()<b.place()):(a.place()>b.place());
}


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
	Out.write(reinterpret_cast<char*>(vec.data()),vec.size()*sizeof(T));
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
	In.read(reinterpret_cast<char*>(vec.data()),size);
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
	In.read(reinterpret_cast<char*>(vec.data()),size);
	In.close();
	return true;
}

#endif
