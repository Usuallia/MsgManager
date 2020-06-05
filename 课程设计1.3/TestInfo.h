#ifndef __TEST_INFORMATION_H_
#define __TEST_INFORMATION_H_

#include<cstring>
#include<cassert>
#include<algorithm>
#include<unordered_set>
#include "util.h"

using SSpair = std::pair<LUINT,float>;
using SSCORE = std::vector<SSpair>;
using HASH = std::unordered_set<LUINT>;

//记录考试信息
//该类是完全公有的
struct TestDescription {

	DATE Date;//日期
	char Name[20];//科目名
	char Notes[100];//描述信息
	UINT Size;

	TestDescription() {};
	TestDescription(DATE date,const char* name,const char* notes,UINT s):Date(date),Size(s) {
		strcpy(Name,name);
		strcpy(Notes,notes);
	}
};
typedef TestDescription Desc;

//出于易设计性考虑，忽略学院，班级，考场等信息（或将其写入描述信息中），不加以分类
class TestInfo {
		friend class TestManager;
	public:
		TestInfo() {};
		TestInfo(DATE date,const char* name,const char* notes);

		/*以下函数模板配合标准库的forward实现了完美转发，以便对自由地对map实施拷贝语义和移动语义
		以达到任意深浅拷贝的效果*/
		template<typename T>
		TestInfo(DATE date,const char* name,const char* notes,T && scores);

		//拷贝构造函数
		TestInfo(const TestInfo& other);

		//移动构造函数
		TestInfo(const TestInfo && other);

		//展示
		void Display();
		//作为目录展示
		void Index();

		//学号作为学生的唯一标识，与成绩键对
		void PushInfo(LUINT number,float score);
		void PushInfo(std::pair<LUINT,float>);

		//按分数排序
		void SortByScore(char mode);

		//按学号排序
		void SortByNumber(char mode);

		//删除某一成绩
		void DeleteInfo(LUINT number);

		//修改某一成绩
		void ChangeInfo(LUINT number,float score);

		//设置属性
		void Set(DATE date,const char* name,const char* notes);

		//判断是否存在
		bool Exist(LUINT number);

		float SingleScore(LUINT number);

		SSCORE &GetScore() {
			return Scores;
		}
		UINT getID() {
			return ID;
		}

	private:
		DATE Date;//考试日期
		UINT ID;//考试编号，从1000开始
		char Name[20];//科目名
		SSCORE Scores;//参加考试的学生及其对应分数
		HASH HashSet;//查学号的哈希表
		char Notes[100];//描述信息
		static UINT IDRecord;//记录已分配ID
};
UINT TestInfo::IDRecord = 1000;

void TestInfo::Set(DATE date,const char* name,const char* notes) {
	ID = ++IDRecord;
	Date = date;
	strcpy(Name,name);
	strcpy(Notes,notes);

}

TestInfo::TestInfo(DATE date,const char* name,const char* notes):
	Date(date),ID(++IDRecord) {
	strcpy(Name,name);
	strcpy(Notes,notes);

}

template<typename T>
TestInfo::TestInfo(DATE date,const char* name,const char* notes,T && scores):
	Date(date),ID(++IDRecord),Scores(std::forward<T>(scores)) {
	for(auto i:scores) {
		HashSet.insert(i.first);
	}
	strcpy(Name,name);
	strcpy(Notes,notes);

}

TestInfo::TestInfo(const TestInfo& other):Date(other.Date),ID(other.ID),Scores(other.Scores),HashSet(other.HashSet) {
	strcpy(Name,other.Name);
	strcpy(Notes,other.Notes);
}

TestInfo::TestInfo(const TestInfo && other):Date(other.Date),ID(other.ID),Scores(std::move(other.Scores))
	,HashSet(std::move(other.HashSet)) {
	strcpy(Name,other.Name);
	strcpy(Notes,other.Notes);

}

void TestInfo::Display() {
	Index();
	print("学生成绩：");
	for(auto i:Scores) {
		print(i.first,i.second);
	}
}

void TestInfo::Index() {
	char date[20];
	date2str(Date,date);
	print("考试编号：",ID," 考试日期：",date," 考试名称：",Name);
	print("注释：",Notes);


}
void TestInfo::PushInfo(LUINT number,float score) {
	//判别是否不存在
	assert(!HashSet.count(number));
	HashSet.insert(number);
	Scores.push_back(std::make_pair(number,score));
}

void TestInfo::PushInfo(std::pair<LUINT,float> p) {
	//判别是否不存在
	assert(!HashSet.count(p.first));
	HashSet.insert(p.first);
	Scores.push_back(p);
}
void TestInfo::SortByNumber(char mode = '<') {

	auto foo1 = [](SSpair a,SSpair b) {
		return a.first < b.first;
	};
	auto foo2 = [](SSpair a,SSpair b) {
		return a.first > b.first;
	};
	if(mode == '<') {
		sort(Scores.begin(),Scores.end(),foo1);
	} else {
		sort(Scores.begin(),Scores.end(),foo2);
	}
}
void TestInfo::SortByScore(char mode = '>') {
	print("HelloSortScore");
	auto foo1 = [](SSpair a,SSpair b) {
		return a.second > b.second;
	};
	auto foo2 = [](SSpair a,SSpair b) {
		return a.second < b.second;
	};
	if(mode == '>') {
		sort(Scores.begin(),Scores.end(),foo1);
	} else {
		sort(Scores.begin(),Scores.end(),foo2);
	}
}

void TestInfo::DeleteInfo(LUINT number) {
	//判别是否存在
	assert(HashSet.count(number));
	for(auto i = Scores.begin(); i != Scores.end(); ++i) {
		if(i->first == number) {
			Scores.erase(i);
			break;
		}
	}
	HashSet.erase(number);
}
void TestInfo::ChangeInfo(LUINT number,float score) {
	DeleteInfo(number);
	PushInfo(number,score);
}
bool TestInfo::Exist(LUINT number) {
	//判别是否存在
	return (HashSet.count(number) == 1);
}

float TestInfo::SingleScore(LUINT number) {

	assert(HashSet.count(number));
	for(auto i = Scores.begin(); i != Scores.end(); ++i) {
		if(i->first == number) {
			return i->second;
		}
	}
}
#endif
