#ifndef __MANAGER_H_
#define __MANAGER_H_
#include<cassert>

#include<vector>

#include "StudentBaseInfo.h"
#include "TestInfo.h"

//全局掌控类（基类）
template<typename T,typename U>
class Manager {
	public:

		//实现简单的单例模式，Manager全局最多存在一个
		static T &getManager() {
			if(only == nullptr) {
				//判别开关
				CreateByGet = true;
				only = new T();
				CreateByGet = false;
			}
			return *only;
		}

		/*出于简易型，不对前数据进行修改检查，不考虑数据个数过大
		即无论是否修改过源数据，管理对象都会一口气读或写完数据*/

		//保存信息,提供默认版本
		virtual void SaveInfo(const char* addr);

		//读取信息,提供默认版本
		virtual void ReadInfo(const char* addr);

		//排序接口,分模式进行
		virtual void Sort(char mode) = 0;

		//返回信息（值）
		U Back(int ite) {
			return Information[ite];
		}
		int Size() {
			return Information.size();
		}
		//（引用）
		U &operator[](int ite) {
			return Information[ite];
		}

	protected:
		//由于派生类会调用基类的构造函数，故用protected修饰
		Manager() {
			//判断是否由提供方法调用
			assert(CreateByGet);
		};

		//同理
		std::vector<U> Information;
	private:
		//单例模式，闲置赋值构造函数与拷贝构造函数
		Manager(const Manager &)=delete;
		Manager& operator=(const Manager&)=delete;
		//判别是否为提供函数所调用
		static bool CreateByGet;

		//唯一对象
		static T *only;
};
//初始化静态成员
template<typename T,typename U>
T *Manager<T,U>::only = nullptr;

template<typename T,typename U>
bool Manager<T,U>::CreateByGet = false;



template<typename T,typename U>
void Manager<T,U>::SaveInfo(const char* addr) {
	VecSave(addr,Information);
}

template<typename T,typename U>
void Manager<T,U>::ReadInfo(const char* addr) {
	if(!VecRead(addr,Information))
		return;
}


//学生信息管理类
class StuManager:public Manager<StuManager,Stu> {
		friend class Manager<StuManager,Stu>;
	public:
		void Sort(char mode);

};
void StuManager::Sort(char mode) {

}


//考试信息管理类
class TestManager:public Manager<TestManager,TestInfo> {
	public:
		void Sort(char mode);

		//录入数据
		//考虑到录完一场考试的数据后，往往不会马上再来一场，故info和scores采用引用传递的模式
		//不加const的原因是要使用移动语义
		void Enter(TestInfo &info);
		void Enter(DATE date,const char* name,const char* notes,SSCORE &scores);

		//保存信息,重载
		void SaveInfo(const char* addr);

		//读取信息,重载
		void ReadInfo(const char* addr);

	private:
		//记录考试划分信息的目录
		std::vector<Desc> TestIndex;
};

void TestManager::Sort(char mode = 0) {

}

void TestManager::Enter(TestInfo &info) {
	//记录位置
	UINT size = info.Scores.size()*sizeof(SSpair);
	//利用emplace_back在容器内部直接构造
	TestIndex.emplace_back(info.Date,info.Name,info.Notes,size);
	//移动语义，浅拷贝scores
	Information.emplace_back(std::move(info));
}
void TestManager::Enter(DATE date,const char* name,const char* notes,SSCORE &scores) {

	//同上
	UINT size = scores.size()*sizeof(SSpair);
	TestIndex.emplace_back(date,name,notes,size);
	Information.emplace_back(date,name,notes,std::move(scores));
}

void TestManager::SaveInfo(const char* addr) {
	//令记录信息的数据保存文件名后缀为datdesc
	char addr2[100];
	strcpy(addr2,addr);
	strcat(addr2,"desc");
	//保存记录
	std::ofstream Out(addr2,std::ios::out|std::ios::binary);
	for(auto i:addr2) {
		VecSave(addr2,TestIndex);
	}
	Out.close();

	//保存数据
	Out.open(addr,std::ios::out|std::ios::binary);
	//TestInfo较大,用for_each能以迭代器的方式调用数据，避免拷贝
	for_each(Information.begin(),Information.end(),[&Out](TestInfo tar) {
		Out.write((char*)tar.Scores.data(),tar.Scores.size()*sizeof(SSpair));
	});
	Out.close();
}
void TestManager::ReadInfo(const char* addr) {
	//令记录信息的数据保存文件名后缀为datdesc
	char addr2[100];
	strcpy(addr2,addr);
	strcat(addr2,"desc");

	if(!VecRead(addr2,TestIndex)) 
		return;
	

	int offset = 0;
	for(auto i:TestIndex) {
		SSCORE tmpScore;
		VecRead(addr,tmpScore,i.Size,offset);
		Information.emplace_back(i.Date,i.Name,i.Notes,std::move(tmpScore));
		offset += i.Size;
	}
}

int main() {

//	TestInfo A(2020006008,"数学分析","无");
//	A.PushInfo(3118006260,72);
//	A.PushInfo(3118006261,78);
//	A.PushInfo(3118006262,63);

	TestManager &M = TestManager::getManager();


	M.ReadInfo("TestInfo.dat");
	
	print(M.Size());
}
#endif
