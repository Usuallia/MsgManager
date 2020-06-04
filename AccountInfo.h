#ifndef __AccountInfo_H_
#define __AccountInfo_H_

#include<unordered_map>
#include<cstring>

#include"util.h"

using ACMAP = std::unordered_map<std::string,LUINT>;

struct AccountInfo {
	Type type;//�˻�����
	LUINT password; //��ϣ���ܺ������
	char ID[20];//�ʺţ�ѧ����Ӧѧ�ţ���ʦ��Ӧ����

};

//��ģ��ֻ������������ľ�̬��Ա
template<typename T>
class Account {
		friend class AcManager;
	public:
		//���캯��
		Account() {};
		Account(const char* id,const char* passw);
		void Set(const char* id,const char* passw);
		void Set(AccountInfo info);
		Statu Register();//ע��
		Statu SignIn();//��¼
		virtual Type Permission() = 0;//Ȩ��
		AccountInfo GetInfo();

	private:
		LUINT password; //��ϣ���ܺ������
		char ID[20];//�ʺţ�ѧ����Ӧѧ�ţ���ʦ��Ӧ����
		static std::hash<std::string> hasher;
		static ACMAP ACMap;
};
//��ʼ����̬��Ա 
template<typename T> ACMAP Account<T>::ACMap;
template<typename T> std::hash<std::string> Account<T>::hasher;

/*��c++��׼���ṩ�Ĺ�ϣ��򵥼�������*/
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
