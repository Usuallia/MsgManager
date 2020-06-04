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

//��¼������Ϣ
//��������ȫ���е�
struct TestDescription {

	DATE Date;//����
	char Name[20];//��Ŀ��
	char Notes[100];//������Ϣ
	UINT Size;

	TestDescription() {};
	TestDescription(DATE date,const char* name,const char* notes,UINT s):Date(date),Size(s) {
		strcpy(Name,name);
		strcpy(Notes,notes);
	}
};
typedef TestDescription Desc;

//����������Կ��ǣ�����ѧԺ���༶����������Ϣ������д��������Ϣ�У��������Է���
class TestInfo {
		friend class TestManager;
	public:
		TestInfo() {};
		TestInfo(DATE date,const char* name,const char* notes);

		/*���º���ģ����ϱ�׼���forwardʵ��������ת�����Ա�����ɵض�mapʵʩ����������ƶ�����
		�Դﵽ������ǳ������Ч��*/
		template<typename T>
		TestInfo(DATE date,const char* name,const char* notes,T && scores);

		//�������캯��
		TestInfo(const TestInfo& other);

		//�ƶ����캯��
		TestInfo(const TestInfo && other);

		//չʾ
		void Display();
		//��ΪĿ¼չʾ
		void Index();

		//ѧ����Ϊѧ����Ψһ��ʶ����ɼ�����
		void PushInfo(LUINT number,float score);
		void PushInfo(std::pair<LUINT,float>);

		//����������
		void SortByScore(char mode);

		//��ѧ������
		void SortByNumber(char mode);

		//ɾ��ĳһ�ɼ�
		void DeleteInfo(LUINT number);

		//�޸�ĳһ�ɼ�
		void ChangeInfo(LUINT number,float score);

		//��������
		void Set(DATE date,const char* name,const char* notes);

		//�ж��Ƿ����
		bool Exist(LUINT number);

		float SingleScore(LUINT number);

		SSCORE &GetScore() {
			return Scores;
		}
		UINT getID() {
			return ID;
		}

	private:
		DATE Date;//��������
		UINT ID;//���Ա�ţ���1000��ʼ
		char Name[20];//��Ŀ��
		SSCORE Scores;//�μӿ��Ե�ѧ�������Ӧ����
		HASH HashSet;//��ѧ�ŵĹ�ϣ��
		char Notes[100];//������Ϣ
		static UINT IDRecord;//��¼�ѷ���ID
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
	print("ѧ���ɼ���");
	for(auto i:Scores) {
		print(i.first,i.second);
	}
}

void TestInfo::Index() {
	char date[20];
	date2str(Date,date);
	print("���Ա�ţ�",ID," �������ڣ�",date," �������ƣ�",Name);
	print("ע�ͣ�",Notes);


}
void TestInfo::PushInfo(LUINT number,float score) {
	//�б��Ƿ񲻴���
	assert(!HashSet.count(number));
	HashSet.insert(number);
	Scores.push_back(std::make_pair(number,score));
}

void TestInfo::PushInfo(std::pair<LUINT,float> p) {
	//�б��Ƿ񲻴���
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
	//�б��Ƿ����
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
	//�б��Ƿ����
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
