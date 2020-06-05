//д�ڵ����ܵ�С���߼�

#ifndef _UTIL__H_
#define _UTIL__H_
#include<cmath>
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
using DATE = unsigned long long;//��һ�鳤���μ�¼������Ϣ
using LUINT = unsigned long long;//�Ǹ�������
using LINT = long long;
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
template<typename T> 
void print(const T &tar) {
	std::cout << tar << std::endl;
}
template<typename T,typename ...Args>
void print(const T &tar,const Args ...Lists) {
	std::cout << tar << ' ';
	print(Lists...);
}


//���ں�C����ַ����Ļ���ת��
void date2str(DATE a,char *str) {

	//����ʮλ,������һλ
	for(int i = 9; i >= 0; --i) {
		str[i] = (a % 10) + '0';
		a /= 10;
	}
	str[4] = '-';
	str[7] = '-';
	str[10] = '\0';
}

DATE date2long(const char *str) {

	int i = 0,max = strlen(str);
	LUINT result = 0,tmp = 0;
	if(str[0] == '-') {
		i = 1;
	}
	while(i < max && str[i] != '.') {
		if(str[i] == '-') {
			tmp = 0;//�����б�����
		} else {
			tmp = str[i] - '0';
		}

		++i;
		result += tmp * pow(10,max - i);
	}
	if(str[0] == '-') {
		result = -result;
	}
	return result;
}

//�����κ�C����ַ����Ļ���ת��
void long2str(LUINT a,char *str) {
	bool isneg = 0;
	if(a < 0) {
		a = -a;
		str[0] = '-';
		isneg = 1;
	}
	char buf[20] = {0};
	int j = 0;
	while(a != 0) {
		buf[j] = (a % 10) + '0';
		++j;
		a /= 10;
		if(a == 0) {
			buf[j] = '\0';
		}
	}
	int k = 0;
	while(k < j) {
		str[isneg + k] = buf[j - k - 1];
		++k;
	}
	str[isneg + k] = '\0';
}
LUINT str2long(const char *str) {

	int i = 0,max = strlen(str);
	long long result = 0,tmp = 0;
	//�жϸ��� 
	if(str[0] == '-') {
		i = 1;
	}
	//'.'���ж�С���� 
	while(i < max && str[i] != '.') {
		tmp = str[i] - '0';
		++i;
		result += tmp * pow(10,max - i);
	}
	if(str[0] == '-') {
		result = -result;
	}
	return result;
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
	auto size = In.tellg();
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
