#ifndef __STUDENT_INFORMATION_H_
#define __STUDENT_INFORMATION_H_

#include<cstring>

#include "util.h"


/*出于结构体和类字节对齐的考虑,以下各组结构体和类的数据类型依据所占字节数由小到大排列
可能导致数据排列顺序不符合平时的书写习惯*/

class StudentBaseInfo {
	public:
		//构造函数
		StudentBaseInfo() {};
		StudentBaseInfo(const char* sex,LUINT number,const char* nation,DATE birth,const char* name
		                ,const char* place);

		//拷贝构造函数
		StudentBaseInfo(const StudentBaseInfo &other);
		//展示
		void Display();
		
		//获取属性
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
		LUINT Number;//学号
		DATE Birth;//生日
		char Sex[10];//性别
		char Nation[10];//民族
		char Name[20];//姓名
		char Place[40];//籍贯
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

void Stu::Display() {
	char date[11];
	//日期格式转换 
	date2str(Birth,date);
	print(Number,Name,Sex,Nation,date,Place);
}


#endif
