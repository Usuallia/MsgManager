//写于到处跑的小工具集

#ifndef _UTIL__H_
#define _UTIL__H_
#include<cmath>
#include<iostream>
#include<vector>
#include<fstream>

using DATE = long long;//用一组长整形记录生日信息
using LUINT = unsigned long long;//非负长整形
using UINT = unsigned int;

//利用模板进行可变参数式打印
template<typename T> print(const T &tar) {
	std::cout << tar << std::endl;
}
template<typename T,typename ...Args> print(const T &tar,const Args ...Lists) {
	std::cout << tar << ' ';
	print(Lists...);
}


//日期和C风格字符串的互相转换
void date2str(long long a,char *str) {

	//日期十位,结束符一位
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
	//判断文件是否存在 
	if(!In){
		return false;
	}
	//获取dat文件大小
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
	//设置读指针偏移
	In.seekg(offset,std::ios::beg);
	vec.resize(size/sizeof(T));
	In.read((char*)vec.data(),size);
	In.close();
	return true;
}

#endif
