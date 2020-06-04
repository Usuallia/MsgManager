#ifndef __STUDENT_INFORMATION_H_
#define __STUDENT_INFORMATION_H_

#include<cstring>

#include "util.h"


/*���ڽṹ������ֽڶ���Ŀ���,���¸���ṹ��������������������ռ�ֽ�����С��������
���ܵ�����������˳�򲻷���ƽʱ����дϰ��*/

class StudentBaseInfo {
	public:
		//���캯��
		StudentBaseInfo() {};
		StudentBaseInfo(const char* sex,LUINT number,const char* nation,DATE birth,const char* name
		                ,const char* place);

		//�������캯��
		StudentBaseInfo(const StudentBaseInfo &other);
		//չʾ
		Display();
		
		//��ȡ����
		LUINT number(){
			return Number;
		} 
		DATE birth(){
			return Birth;
		}
		std::string name(){
			return std::string(Name);
		}
		std::string place(){
			return std::string(Place);
		}
		
	private:
		LUINT Number;//ѧ��
		DATE Birth;//����
		char Sex[10];//�Ա�
		char Nation[10];//����
		char Name[20];//����
		char Place[40];//����
};
typedef StudentBaseInfo Stu;

Stu::StudentBaseInfo(const StudentBaseInfo &other):Number(other.Number),Birth(other.Birth) {
	strcpy(Sex,other.Sex);
	strcpy(Nation,other.Nation);
	strcpy(Name,other.Name);
	strcpy(Place,other.Place);
}

Stu::StudentBaseInfo(const char* sex,LUINT number,const char* nation,DATE birth,const char* name
                     ,const char* place):Number(number),Birth(birth) {
	strcpy(Sex,sex);
	strcpy(Nation,nation);
	strcpy(Name,name);
	strcpy(Place,place);
}

Stu::Display() {
	char date[11];
	//���ڸ�ʽת�� 
	date2str(Birth,date);
	print(Number,Name,Sex,Nation,date,Place);
}


#endif