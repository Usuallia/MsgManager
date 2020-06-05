//菜单类 
#include<cmath>
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<cstring>
#include<cassert>
#include<algorithm>
#include<functional>
#include<unordered_map>
#include<unordered_set>

//用来声明和初始化的宏
#define Init(_varname,_manager) _manager &_varname = _manager::getManager()
#define Initialize(_var1,_var2,_var3) Init(_var1,StuManager);    \
	Init(_var2,TestManager);   \
	Init(_var3,AcManager);

using DATE = unsigned long long;//用一组长整形记录生日信息
using LUINT = unsigned long long;//非负长整形
using LINT = long long;
using UINT = unsigned int;
using SSpair = std::pair<LUINT,float>;
using SSCORE = std::vector<SSpair>;
using HASH = std::unordered_set<LUINT>;
using ACMAP = std::unordered_map<std::string,LUINT>;

//账户类别 
enum Type {Student = 0,Teacher};
//排序模式 
enum MODE{
	//小到大 
	_byNumber = 0,_byBirth,_byName,_byPlace,_byID,
	//大到小 
	_byrNumber,_byrBirth,_byrName,_byrPlace,_byrID
};
//注册或登录状态 
enum Statu{
	_dispatch = -1,_fail,_success

};

//判断函数模板 
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


//利用模板进行可变参数式打印
template<typename T>
void print(const T &tar) {
	std::cout << tar << std::endl;
}
template<typename T,typename ...Args>
void print(const T &tar,const Args ...Lists) {
	std::cout << tar << ' ';
	print(Lists...);
}


//日期和C风格字符串的互相转换
void date2str(DATE a,char *str) {

	//日期十位,结束符一位
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

	while(i < max && str[i] != '.') {
		if(str[i] == '-') {
			tmp = 0;//用于判别日期
		} else {
			tmp = str[i] - '0';
		}

		++i;
		result += tmp * pow(10,max - i);
	}

	return result;
}

//长整形和C风格字符串的互相转换
void long2str(LUINT a,char *str) {
	bool isneg = 0;

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
	//判断负号 
	if(str[0] == '-') {
		i = 1;
	}
	//'.'是判断小数点 
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
	//判断文件是否存在 
	if(!In){
		return false;
	}
	//获取dat文件大小
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
	//设置读指针偏移
	In.seekg(offset,std::ios::beg);
	vec.resize(size/sizeof(T));
	In.read(reinterpret_cast<char*>(vec.data()),size);
	In.close();
	return true;
}

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
	return 0; 
}

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
		//录入 
		void Enter(Stu stu);
		void Enter(const char* sex,LUINT number,const char* nation,DATE birth,const char* name,const char* place);
		void Sort(MODE mode);
		//按学号寻找
		int Find(LUINT number);
		//删除某一学号对应的学生数据
		void Erase(LUINT number);

};

void StuManager::Enter(Stu stu) {
	Information.push_back(stu);
}

void StuManager::Enter(const char* sex,LUINT number,const char* nation,DATE birth,const char* name,const char* place) {
	Information.emplace_back(sex,number,nation,birth,name,place);
}

void StuManager::Sort(MODE mode) {
	//用于排序的function
	std::function<bool(Stu,Stu,MODE)> sortByMode;

	//根据模式进行选择function
	switch(mode) {
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
		default:
			return;
	}
	//利用lambda表达式联系函数
	auto sortfoo = [mode,&sortByMode](Stu a,Stu b)->bool {
		return sortByMode(a,b,mode);
	};
	//排序
	sort(Information.begin(),Information.end(),sortfoo);
}

int StuManager::Find(LUINT number) {
	auto result = find_if(Information.begin(),Information.end(),[number](Stu u)->bool {
		auto tmp = u.number();
		return (tmp == number);
	});
	return result - Information.begin();
}
void StuManager::Erase(LUINT number) {
	Information.erase(Information.begin() + Find(number));
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
		//找
		int Find(UINT ID); 
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
		TestInfo t(i.Date,i.Name,i.Notes,tmpScore);
		Information.emplace_back(std::move(t));
		offset += i.Size;
	}
}
int TestManager::Find(UINT ID) {
	auto result = find_if(Information.begin(),Information.end(),[ID](TestInfo t)->bool {
		auto tmp = t.ID;
		return (tmp == ID);
	});
	return result - Information.begin();
}


//账户管理类

class AcManager:public Manager<AcManager,AccountInfo> {
	public:		
		//info通常是从磁盘里读的信息
		//非info版本通常用于注册
		Statu Register(AccountInfo info);
		Statu Register(const char* id,const char *passw,Type t);
		Statu SignIn(AccountInfo info);
		Statu SignIn(const char* id,const char *passw,Type t);
		
		//重载读函数 
		void ReadInfo(const char* addr);
		
		//按学号寻找
		int Find(LUINT number);
		//删除某一学号对应的学生数据
		void Erase(LUINT number);

	private:
		StudentAc StuAc;
		TeacherAc TeaAc;
};

Statu AcManager::Register(const char* id,const char *passw,Type t = Student) {
	Statu result;
	AccountInfo tmpinfo;
	switch(t) {
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
	if(result == _success) {
		Information.push_back(tmpinfo);
	}
	return result;
}

Statu AcManager::Register(AccountInfo info) {
	switch(info.type) {
		case Student:
			StuAc.Set(info);
			return StuAc.Register();
		case Teacher:
			TeaAc.Set(info);
			return TeaAc.Register();
	}
	return _dispatch;
}
Statu AcManager::SignIn(const char* id,const char *passw,Type t = Student) {
	switch(t) {
		case Student:
			StuAc.Set(id,passw);
			return StuAc.SignIn();
		case Teacher:
			TeaAc.Set(id,passw);
			return TeaAc.SignIn();
	}
	return _dispatch;
}

Statu AcManager::SignIn(AccountInfo info) {

	switch(info.type) {
		case Student:
			StuAc.Set(info);
			return StuAc.SignIn();
		case Teacher:
			TeaAc.Set(info);
			return TeaAc.SignIn();
	}
	return _dispatch;
}
void AcManager::ReadInfo(const char* addr) {
	Manager<AcManager,AccountInfo>::ReadInfo(addr);
	for(auto i:Information) {
		Register(i);
	}
}
int AcManager::Find(LUINT ID){
	auto result = find_if(Information.begin(),Information.end(),[ID](AccountInfo ac)->bool {
		return (date2long(ac.ID) == ID);
	});
	return result - Information.begin();
}

void AcManager::Erase(LUINT ID){
	auto order = Find(ID);
	auto ac = Information[order];
	char id[20];
	long2str(ID,id);
	if(ac.type = Student){
		StudentAc::ACMap.erase(std::string(id));
	}else{
		TeacherAc::ACMap.erase(std::string(id));
	}
	Information.erase(Information.begin() + order);
}
//几个数据的文件名(都在当前目录下)
char AcInfoAddr[] = "AcInfo.dat";
char StuInfoAddr[] = "StuInfo.dat";
char TestInfoAddr[] = "TestInfo.dat";

//减少溢出的可能
char cinbuf[1000];
using namespace std; 
//菜单类
class Menu {
	public:
		Menu():next(nullptr) {};
		//显示
		virtual void Show() = 0;
		//等待输入
		virtual void Wait() = 0;

		//转换菜单
		//参数必须为指针的引用，目的是改变指针的指向
		static void Transition(Menu *&menu);
		bool cinBad(const char* err);

	protected:
		Menu* next;

};
typedef Menu* PMenu;
void Menu::Transition(PMenu &menu) {
	PMenu tmp = menu;
	menu = menu->next;
	delete tmp;
}
bool Menu::cinBad(const char* err) {
	if(!cin.good()) {
		cout << err << flush;
		cin.clear();
		cin.ignore(1000,'\n');
		system("pause");
		Show();
		return true;
	}
	return false;
}



//登录菜单
class LoginMenu:public Menu {
	public:
		void Show();
		void Wait();
};

//教师界面菜单
class TeacherMenu:public Menu {
	public:
		void Show();
		void Wait();
};

//学籍信息菜单
class StuInfoMenu:public Menu {
	public:
		void Show();
		void Wait();
};

//成绩管理菜单
class ScoreMenu:public Menu {
	public:
		void Show();
		void Wait();
};

//学生界面菜单
class StudentMenu:public Menu {
	public:
		void Show();
		void Wait();
		StudentMenu(LUINT number,Type t = Student):IDNumber(number),type(t) {}
	private:
		LUINT IDNumber;//学号
		Type type;
		unordered_set<UINT> TestID;//记录考场号
};


//学籍信息排序菜单
class StuInfoOrderMenu:public Menu {
	public:
		void Show();
		void Wait();
};


//考试成绩排序菜单
class ScoreOrderMenu:public Menu {
	public:
		void Show();
		void Wait();
		ScoreOrderMenu(UINT i):index(i) {}
	private:
		UINT index;
};

void LoginMenu::Show() {
	system("cls");
	cout << "欢迎使用本学籍管理系统！\n请先登录后再进行操作\n(ps:学生的账号为学号，初始密码为123456)" << endl;
}
void LoginMenu::Wait() {
	putchar('\n');
	AcManager &ac = AcManager::getManager();
	char id[20];
	char password[20];
	while(1) {
		cout << "帐号:" << flush;
		while(cin.good()) {
			cin >> cinbuf;
			if(strlen(cinbuf) <= 20) {
				strcpy(id,cinbuf);
				break;
			}
			cout << "帐号密码均不能超过20位" << endl;
		}
		cout << "密码:" << flush;
		while(cin.good()) {
			cin >> cinbuf;
			if(strlen(cinbuf) <= 20) {
				strcpy(password,cinbuf);
				break;
			}
			cout << "帐号密码均不能超过20位" << endl;
		}

		Statu isStudent = ac.SignIn(id,password);
		Statu isTeacher = ac.SignIn(id,password,Teacher);
		if(isTeacher == _success) {
			next = new TeacherMenu();
			break;
		}
		if(isStudent == _success) {
			next = new StudentMenu(str2long(id));
			break;
		}

		//枚举整形加法，用于判断不匹配还是未注册
		if(isStudent + isTeacher == _dispatch) {
			cout << "账号密码不匹配" << endl;
		} else {
			cout << "该帐号尚未注册" << endl;
		}

		system("pause");
		system("cls");
	}
}

void TeacherMenu::Show() {
	system("cls");
	//这里用printf打印是方便起见
	printf("***************菜单*****************\n");
	printf("          (1)学生信息\n");
	printf("          (2)成绩管理\n");
	printf("          (3)查询某一学生\n");
	printf("          (4)退出\n");
	printf("************************************\n");

}
void TeacherMenu::Wait() {
	LUINT Number;
	while(cin.good()) {
		cout << "输入需要的功能序号：" << flush;
		cin >> cinbuf;
		char choose = cinbuf[0];
		switch(choose) {
			case '1':
				next = new StuInfoMenu();
				break;
			case '2':
				next = new ScoreMenu();
				break;
			case '3':
				cout << "想要查询的学生的学号：" << flush;
				cin >> Number;
				if(cinBad("非法输入！")) continue;
				next = new StudentMenu(Number,Teacher);
				break;
			case '4':

				break;
			default:
				cout << "必须选择正确的功能序号！" << flush;
				system("pause");
				Show();
				continue;
		}
		goto A;
	}
A:
	return;
}
void StuInfoMenu::Show() {
	system("cls");
	printf("*************学生信息***************\n");
	printf("          (1)录入学生信息\n");
	printf("          (2)修改学生信息\n");
	printf("          (3)删除学生信息\n");
	printf("          (4)查看所有学生信息\n");
	printf("          (5)返回\n");
	printf("          (6)退出程序\n");
	printf("************************************\n");
}
void StuInfoMenu::Wait() {
	StuManager &s = StuManager::getManager();
	AcManager &ac = AcManager::getManager();
	//switch中不能声明变量
	LUINT Number;//学号
	DATE Birth;//生日
	char Sex[10];//性别
	char Nation[10];//民族
	char Name[20];//姓名
	char Place[40];//籍贯
	char birth[20];//生日，字符型
	while(cin.good()) {
		cout << "输入需要的功能序号：" << flush;
		cin >> cinbuf;

		char choose = cinbuf[0];
		switch(choose) {
			case '1':
				//录入
				cout << "学号：" << flush;
				cin >> Number;
				if(cinBad("非法输入！")) continue;

				cout << "姓名：" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 20) {
					cout << "长度不应该超过10个中文字符，" << flush;
					system("pause");
					Show();
				}
				strcpy(Name,cinbuf);

				cout << "性别：" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 10) {
					cout << "长度不应该超过5个中文字符，" << flush;
					system("pause");
					Show();
				}
				strcpy(Sex,cinbuf);

				cout << "民族：" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 10) {
					cout << "长度不应该超过5个中文字符，" << flush;
					system("pause");
					Show();
				}
				strcpy(Nation,cinbuf);

				cout << "生日(以年-月-日的格式输入)：" << flush;
				cin >> cinbuf;
				memcpy(birth,cinbuf,20);
				Birth = date2long(birth);

				cout << "籍贯：" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 40) {
					cout << "长度不应该超过20个中文字符，" << flush;
					system("pause");
					Show();
					continue;
				}
				strcpy(Place,cinbuf);

				//判断学生信息是否已经录入
				//学生默认密码123456
				char num[20];
				long2str(Number,num);
				if(ac.Register(num,"123456") != _success) {
					cout << "该学生信息已经存在！" << flush;
					system("pause");
					Show();
					continue;
				}
				s.Enter(Sex,Number,Nation,Birth,Name,Place);
				s.SaveInfo(StuInfoAddr);
				ac.SaveInfo(AcInfoAddr);
				cout << "录入成功！" << flush;
				system("pause");
				Show();

				break;
			case '2':
				cout << "想要修改的学生的学号：" << flush;
				cin >> Number;
				if(cinBad("非法输入！")) continue;

				//判断是否为空
				if(s.Find(Number) == s.Size()) {
					cout << "尚未收录该学号," << flush;
					system("pause");
					Show();
					continue;
				}
				//录入
				cout << "重新录入姓名：" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 20) {
					cout << "长度不应该超过10个中文字符，" << flush;
					system("pause");
					Show();
				}
				strcpy(Name,cinbuf);

				cout << "性别：" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 10) {
					cout << "长度不应该超过5个中文字符，" << flush;
					system("pause");
					Show();
				}
				strcpy(Sex,cinbuf);

				cout << "民族：" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 10) {
					cout << "长度不应该超过5个中文字符，" << flush;
					system("pause");
					Show();
				}
				strcpy(Nation,cinbuf);

				cout << "生日(以年-月-日的格式输入)：" << flush;
				cin >> cinbuf;
				memcpy(birth,cinbuf,20);
				Birth = date2long(birth);

				cout << "籍贯：" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 40) {
					cout << "长度不应该超过20个中文字符，" << flush;
					system("pause");
					Show();
					continue;
				}
				strcpy(Place,cinbuf);

				//删除旧数据
				s.Erase(Number);
				s.Enter(Sex,Number,Nation,Birth,Name,Place);
				s.SaveInfo(StuInfoAddr);
				cout << "录入成功！" << flush;
				system("pause");
				Show();
				break;
			case '3':
				cout << "想要删除的学生的学号：" << flush;
				cin >> Number;
				if(cinBad("非法输入！")) continue;

				//判断是否为空

				if(s.Find(Number) == s.Size()) {
					cout << "尚未收录该学号," << flush;
					system("pause");
					Show();
					continue;
				}
				s.Erase(Number);
				ac.Erase(Number);
				s.SaveInfo(StuInfoAddr);
				ac.SaveInfo(AcInfoAddr);
				cout << "删除成功！" << flush;
				system("pause");
				Show();
				break;

			case '4':
				next = new StuInfoOrderMenu();
				goto A;
				break;
			case '5':
				next = new TeacherMenu();
				goto A;
			case '6':
				goto A;
			default:
				cout << "请选择正确的功能序号，" << flush;
				system("pause");
				Show();
				continue;
		}
	}
A:
	return;
}

void ScoreMenu::Show() {
	system("cls");
	printf("*************成绩管理***************\n");
	printf("          (1)新建考试信息\n");
	printf("          (2)查询某一场考试的具体信息\n");
	printf("          (3)查询考试目录信息\n");
	printf("          (4)返回\n");
	printf("          (5)退出程序\n");
	printf("************************************\n");
}
void ScoreMenu::Wait() {
	StuManager &s = StuManager::getManager();
	TestManager &t = TestManager::getManager();

	while(cin.good()) {
		cout << "输入需要的功能序号：" << flush;
		cin >> cinbuf;
		char choose = cinbuf[0];
		char Name[20];//科目名
		DATE Date;//考试日期
		char Notes[100];//描述信息
		char date[20];//字符型日期
		LUINT number;
		float score;//成绩
		UINT ID;//考场号
		UINT index;//目录指针
		int size;//记录大小
		TestInfo test;//考试信息

		switch(choose) {
			case '1':
				//录入
				cout << "科目名：" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 20) {
					cout << "长度不应该超过10个中文字符，" << flush;
					system("pause");
					Show();
				}
				strcpy(Name,cinbuf);

				cout << "考试日期(以年-月-日的格式输入)：" << flush;
				cin >> cinbuf;
				strcpy(date,cinbuf);
				Date = date2long(date);

				cout << "注释（描述信息）：" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 20) {
					cout << "长度不应该超过10个中文字符，" << flush;
					system("pause");
					Show();
				}
				strcpy(Notes,cinbuf);
				test.Set(Date,Name,Notes);
				cout << "录入学生的个数：" << flush;
				//个数不可能为负数
				UINT n;
				cin >> n;
				if(cinBad("无效的个数!")) continue;
				for(index = 0; index < n; ++index) {

					cout << "学号：" << flush;
					cin >> number;

					if(cinBad("无效的成绩!")) continue;
					if(s.Find(number) == s.Size()) {
						cout << "该学号尚未录入，无法登记成绩！" << flush;
						system("pause");
						Show();
						break;
					}

					cout << "成绩：" << flush;
					cin >> score;

					if(cinBad("无效的成绩!")) continue;
					test.PushInfo(number,score);
				}
				t.Enter(test);
				t.SaveInfo(TestInfoAddr);
				cout << "录入成功！" << flush;
				system("pause");
				Show();
				break;
			case '2':
				ID = 0;
				cout << "输入考场号（在目录中查询）：";
				cin >> ID;
				if(cinBad("无效的考场号!")) continue;
				index = t.Find(ID);
				if(index == t.Size()) {
					cout << "未找到该考场号对应的考试信息，请确认是否录入！\n" << flush;
					system("pause");
					Show();
					break;
				}

				next = new ScoreOrderMenu(index);

				goto A;
			case '3':
				size = t.Size();
				if(size == 0) {
					cout << "空目录!\n" << flush;
					system("pause");
					Show();
					break;
				}
				for(index = 0; index < size; ++index) {
					t[index].Index();
					putchar('\n');
				}
				cout << "展示完毕！" << flush;
				system("pause");
				Show();
				break;
			case '4':
				next = new TeacherMenu();
				goto A;
			case '5':

				goto A;
			default:
				cout << "必须选择正确的功能序号！" << flush;
				system("pause");
				Show();
				continue;
		}
	}
A:
	return;
}

void StudentMenu::Show() {
	auto &s = StuManager::getManager();
	TestManager &test = TestManager::getManager();
	system("cls");
	printf("***************科目*****************\n");
	for(int i = 0; i < test.Size(); ++i) {
		if(test[i].Exist(IDNumber)) {
			test[i].Index();
			TestID.insert(test[i].getID());
		}
	}
	if(type = Student) {
		printf("************(输入0退出)*************\n");
	} else {
		printf("************(输入0返回)*************\n");
	}

	print(IDNumber,":",s[s.Find(IDNumber)].name());
}
void StudentMenu::Wait() {
	auto &s = StuManager::getManager();
	auto &t = TestManager::getManager();
	if(type = Teacher) {
		next = new TeacherMenu();
	}
	while(cin.good()) {
		cout << "欲查询的考试号：" << flush;
		UINT n;
		cin >> n;
		if(cinBad("无效的考试号!")) continue;
		if(n == 0) {
			break;
		}
		//判断是否有该考试
		if(t.Find(n) == t.Size()) {
			cout << "从未举办该次考试!" << flush;
			system("pause");
			Show();
			continue;
		}
		//判断学生是否参与过考试
		if(TestID.count(n) == 0) {
			cout << "此学号的学生并未参加该考试!" << flush;
			system("pause");
			Show();
			continue;
		}
		system("cls");
		cout << "查询结果如下：" << endl;
		int index = s.Find(IDNumber);
		int TestIndex = t.Find(n);
		t[TestIndex].Index();
		print(IDNumber,s[index].name(),"成绩：",t[TestIndex].SingleScore(IDNumber));
		system("pause");
		Show();
	}

}
void StuInfoOrderMenu::Show() {
	system("cls");
	printf("*************排序模式***************\n");
	printf("          (1)按学号\n");
	printf("          (2)按姓名\n");
	printf("          (3)按生日\n");
	printf("          (4)按籍贯\n");
	printf("          (5)返回\n");
	printf("          (6)退出程序\n");
	printf("************************************\n");
}

void StuInfoOrderMenu::Wait() {

	StuManager &s = StuManager::getManager();

	while(cin.good()) {
		cout << "输入需要的功能序号：" << flush;
		cin >> cinbuf;
		char choose = cinbuf[0];

		switch(choose) {
			case '1':
				s.Sort(_byNumber); 
				break;
			case '2':
				s.Sort(_byName); 
				break;
			case '3':
				s.Sort(_byBirth); 
				break;
			case '4':
				s.Sort(_byPlace); 
				break;
			case '5':
				next = new StuInfoMenu();
				goto A;
			case '6':
				goto A;
			default:
				cout << "必须选择正确的功能序号！" << flush;
				system("pause");
				Show();
				continue;
		}
		for(int i = 0; i < s.Size(); ++i) {
			s[i].Display();
		}
		cout << "展示完毕！" << flush;
		system("pause");
		Show();
	}
A:
	return;
}

void ScoreOrderMenu::Show() {
	system("cls");
	printf("*************编号：%d***************\n",index);
	printf("          (1)成绩从高到低\n");
	printf("          (2)成绩从低到高\n");
	printf("          (3)学号从高到低\n");
	printf("          (4)学号从低到高\n");
	printf("          (5)返回\n");
	printf("          (6)退出程序\n");
	printf("************************************\n");
}
void ScoreOrderMenu::Wait() {

	StuManager &s = StuManager::getManager();
	TestManager &t = TestManager::getManager();
	int tmp = 0;
	while(cin.good()) {
		cout << "输入需要的功能序号：" << flush;
		cin >> cinbuf;
		char choose = cinbuf[0];

		switch(choose) {
			case '1':
				t[index].SortByScore('>');
				break;
			case '2':
				t[index].SortByScore('<');
				break;
			case '3':
				t[index].SortByNumber('>');
				break;
			case '4':
				t[index].SortByNumber('<');
				break;
			case '5':
				next = new ScoreMenu();
				goto A;
			case '6':
				goto A;
			default:
				cout << "必须选择正确的功能序号！" << flush;
				system("pause");
				Show();
				continue;
		}
		for(auto i:t[index].GetScore()) {
			tmp = s.Find(i.first);
			print(i.first,s[tmp].name(),i.second);
		}
		cout << "查询完毕," << flush;
		system("pause");
		Show();
	}
A:
	return;
}

int main() {

	//初始化
	Initialize(stu,test,ac);
	stu.ReadInfo(StuInfoAddr);
	test.ReadInfo(TestInfoAddr);
	ac.ReadInfo(AcInfoAddr);

	//登录界面...
	Menu *head = new LoginMenu();
	Menu *m = head;
	
	//菜单循环，遵循统一接口的菜单都能加入循环 
	while(m != nullptr) {
		m->Show();
		m->Wait();
		Menu::Transition(m);
	}
	//销毁最后一页菜单 
	delete m;

	return 0;
}
