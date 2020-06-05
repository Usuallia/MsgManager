#ifndef __AccountInfo_H_
#define __AccountInfo_H_

#include<unordered_map>
#include<cstring>

#include"util.h"

using ACMAP = std::unordered_map<std::string,LUINT>;

struct AccountInfo {
	Type type;//账户类型
	LUINT password; //哈希加密后的密码
	char ID[20];//帐号，学生对应学号，教师对应工号

};

//该模板只用于区分子类的静态成员
template<typename T>
class Account {
		friend class AcManager;
	public:
		//构造函数
		Account() {};
		Account(const char* id,const char* passw);
		void Set(const char* id,const char* passw);
		void Set(AccountInfo info);
		Statu Register();//注册
		Statu SignIn();//登录
		virtual Type Permission() = 0;//权限
		AccountInfo GetInfo();

	private:
		LUINT password; //哈希加密后的密码
		char ID[20];//帐号，学生对应学号，教师对应工号
		static std::hash<std::string> hasher;
		static ACMAP ACMap;
};
//初始化静态成员 
template<typename T> ACMAP Account<T>::ACMap;
template<typename T> std::hash<std::string> Account<T>::hasher;

/*用c++标准库提供的哈希类简单加密密码*/
template<typename T>
Account<T>::Account(const char* id,const char* passw) {

	strcpy(ID,id);
	
	password = hasher(std::string(passw));
}
template<typename T>
void Account<T>::Set(const char* id,const char* passw){
	strcpy(ID,id);
	password = hasher(std::string(passw));
}
template<typename T>
void Account<T>::Set(AccountInfo info){
	strcpy(ID,info.ID);
	password = info.password;
}
template<typename T>
Statu Account<T>::Register() {
	if(ACMap.count(std::string(ID))) {
		return _dispatch;
	}
	ACMap.insert(std::make_pair(std::string(ID),password));
	return _success;
}
template<typename T>
Statu Account<T>::SignIn() {
	if(!ACMap.count(std::string(ID))) {
		return _fail;
	}
	if(password != ACMap[std::string(ID)]) {
		return _dispatch;
	}
	return _success;
}
template<typename T>
AccountInfo Account<T>::GetInfo() {
	AccountInfo ac;
	ac.type = Permission();
	ac.password = password;
	strcpy(ac.ID,ID);
	return ac;
}

class StudentAc:public Account<StudentAc> {
	public:
		StudentAc(){};
		StudentAc(const char* id,const char* passw):Account(id,passw) {

		}
		AccountInfo GetInfo(){
			auto ac = Account<StudentAc>::GetInfo();
			ac.type = Permission();
			return ac;
		}
		Type Permission() {
			return Student;
		}
};
class TeacherAc:public Account<TeacherAc> {
	public:
		TeacherAc(){};
		TeacherAc(const char* id,const char* passw):Account(id,passw) {

		}
		AccountInfo GetInfo(){
			auto ac = Account<TeacherAc>::GetInfo();
			ac.type = Permission();
			return ac;
		}
		Type Permission() {
			return Teacher;
		}
};

#endif
