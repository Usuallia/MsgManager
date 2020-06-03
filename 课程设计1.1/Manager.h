#ifndef __MANAGER_H_
#define __MANAGER_H_
#include<cassert>

#include<vector>
#include<functional>

#include "StudentBaseInfo.h"
#include "TestInfo.h"
#include "AccountInfo.h"

//全局掌控类（基类）
template<typename T,typename U>
class Manager {
	public:

		//实现简单的单例模式，Manager对象全局最多存在一个
		static T &getManager() {
			if(only == nullptr) {
				//判别开关
				CreateByGet = true;
				only = new T();
				CreateByGet = false;
			}
			return *only;
		}

		/*出于简易性，不对前数据进行修改检查，不考虑数据个数过大
		即无论是否修改过源数据，管理对象都会一口气读或写完数据*/

		//保存信息,提供默认版本
		virtual void SaveInfo(const char* addr);

		//读取信息,提供默认版本
		virtual void ReadInfo(const char* addr);


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
		void Sort(MODE mode);

};


void StuManager::Sort(MODE mode) {
	//用于排序的function 
	std::function<bool(Stu,Stu,MODE)> sortByMode;
	
	//根据模式进行选择function 
	switch(mode){
		case _byNumber:
		case _byrNumber:
			sortByMode = CmpNumber<Stu>;
			break;
		case _byBirth:
		case _byrBirth:
			sortByMode = CmpBirth<Stu>;
			break;
		case _byName:
		case _byrName:
			sortByMode = CmpName<Stu>;
			break;
		case _byPlace:
		case _byrPlace:
			sortByMode = CmpPlace<Stu>;
			break;
		default:return;
	}
	//利用lambda表达式联系函数 
	auto sortfoo = [mode,&sortByMode](Stu a,Stu b)->bool{
		return sortByMode(a,b,mode);
	};
	//排序 
	sort(Information.begin(),Information.end(),sortfoo);
}


//考试信息管理类
class TestManager:public Manager<TestManager,TestInfo> {
	public:
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
		Out.write(reinterpret_cast<char*>(tar.Scores.data()),tar.Scores.size()*sizeof(SSpair));
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

//账户管理类

class AcManager:public Manager<AcManager,AccountInfo>{
	public:
		//info通常是从磁盘里读的信息
		//非info版本通常用于注册 
		Statu Register(AccountInfo info);
		Statu Register(const char* id,const char *passw,Type t);
		Statu SignIn(AccountInfo info);
		Statu SignIn(const char* id,const char *passw,Type t);
		//重载
		void ReadInfo(const char* addr); 
		
	private:
	StudentAc StuAc;
	TeacherAc TeaAc;
}; 

Statu AcManager::Register(const char* id,const char *passw,Type t = Student){
	Statu result;
	AccountInfo tmpinfo;
	switch(t){
		case Student:
			StuAc.Set(id,passw);
			result = StuAc.Register();
			tmpinfo = StuAc.GetInfo();
			break;
		case Teacher:
			TeaAc.Set(id,passw);
			result =  TeaAc.Register();
			tmpinfo = TeaAc.GetInfo();
			break;
	}
	if(result == _success){
		Information.push_back(tmpinfo);
	}
	return result;
}

Statu AcManager::Register(AccountInfo info){
	switch(info.type){
		case Student:
			StuAc.Set(info);
			return StuAc.Register();
		case Teacher:
			TeaAc.Set(info);
			return TeaAc.Register();
	}
}
Statu AcManager::SignIn(const char* id,const char *passw,Type t = Student){
	switch(t){
		case Student:
			StuAc.Set(id,passw);
			return StuAc.SignIn();
		case Teacher:
			TeaAc.Set(id,passw);
			return TeaAc.SignIn();
	}
}

Statu AcManager::SignIn(AccountInfo info){
	switch(info.type){
		case Student:
			StuAc.Set(info);
			return StuAc.SignIn();
		case Teacher:
			TeaAc.Set(info);
			return TeaAc.SignIn();
	}
}
void AcManager::ReadInfo(const char* addr){
	Manager<AcManager,AccountInfo>::ReadInfo(addr);
	for(auto i:Information){
		Register(i);
	}
}


//int main() {
//	
//	AcManager &A = AcManager::getManager();
//
//	A.ReadInfo("AcInfo.dat");
//	print(A.SignIn("admin","123456",Teacher));
//	print(A.Register("adming","123457",Teacher));
//	
//	print(A[0].ID);
//
//	print(A[0].password);
//	print(A[1].password);
//
//	print(A.Size());
//	
//	TestInfo A(2020006008,"数学分析","无");
//	A.PushInfo(3118006260,72);
//	A.PushInfo(3118006261,78);
//	A.PushInfo(3118006262,63);
//	TestManager &M = TestManager::getManager();
//	M.ReadInfo("TestInfo.dat");
//	print(M.Size());
//}
#endif
