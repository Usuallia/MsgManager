//�˵��� 
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

//���������ͳ�ʼ���ĺ�
#define Init(_varname,_manager) _manager &_varname = _manager::getManager()
#define Initialize(_var1,_var2,_var3) Init(_var1,StuManager);    \
	Init(_var2,TestManager);   \
	Init(_var3,AcManager);

using DATE = unsigned long long;//��һ�鳤���μ�¼������Ϣ
using LUINT = unsigned long long;//�Ǹ�������
using LINT = long long;
using UINT = unsigned int;
using SSpair = std::pair<LUINT,float>;
using SSCORE = std::vector<SSpair>;
using HASH = std::unordered_set<LUINT>;
using ACMAP = std::unordered_map<std::string,LUINT>;

//�˻���� 
enum Type {Student = 0,Teacher};
//����ģʽ 
enum MODE{
	//С���� 
	_byNumber = 0,_byBirth,_byName,_byPlace,_byID,
	//��С 
	_byrNumber,_byrBirth,_byrName,_byrPlace,_byrID
};
//ע����¼״̬ 
enum Statu{
	_dispatch = -1,_fail,_success

};

//�жϺ���ģ�� 
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


//����ģ����пɱ����ʽ��ӡ
template<typename T>
void print(const T &tar) {
	std::cout << tar << std::endl;
}
template<typename T,typename ...Args>
void print(const T &tar,const Args ...Lists) {
	std::cout << tar << ' ';
	print(Lists...);
}


//���ں�C����ַ����Ļ���ת��
void date2str(DATE a,char *str) {

	//����ʮλ,������һλ
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
			tmp = 0;//�����б�����
		} else {
			tmp = str[i] - '0';
		}

		++i;
		result += tmp * pow(10,max - i);
	}

	return result;
}

//�����κ�C����ַ����Ļ���ת��
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
	//�жϸ��� 
	if(str[0] == '-') {
		i = 1;
	}
	//'.'���ж�С���� 
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
	//�ж��ļ��Ƿ���� 
	if(!In){
		return false;
	}
	//��ȡdat�ļ���С
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
	//���ö�ָ��ƫ��
	In.seekg(offset,std::ios::beg);
	vec.resize(size/sizeof(T));
	In.read(reinterpret_cast<char*>(vec.data()),size);
	In.close();
	return true;
}

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
		void Display();
		
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

void Stu::Display() {
	char date[11];
	//���ڸ�ʽת�� 
	date2str(Birth,date);
	print(Number,Name,Sex,Nation,date,Place);
}


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
	return 0; 
}

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
//ȫ���ƿ��ࣨ���ࣩ
template<typename T,typename U>
class Manager {
	public:

		//ʵ�ּ򵥵ĵ���ģʽ��Manager����ȫ��������һ��
		static T &getManager() {
			if(only == nullptr) {
				//�б𿪹�
				CreateByGet = true;
				only = new T();
				CreateByGet = false;
			}
			return *only;
		}

		/*���ڼ����ԣ�����ǰ���ݽ����޸ļ�飬���������ݸ�������
		�������Ƿ��޸Ĺ�Դ���ݣ�������󶼻�һ��������д������*/

		//������Ϣ,�ṩĬ�ϰ汾
		virtual void SaveInfo(const char* addr);

		//��ȡ��Ϣ,�ṩĬ�ϰ汾
		virtual void ReadInfo(const char* addr);


		//������Ϣ��ֵ��
		U Back(int ite) {
			return Information[ite];
		}
		int Size() {
			return Information.size();
		}
		//�����ã�
		U &operator[](int ite) {
			return Information[ite];
		}

	protected:
		//�������������û���Ĺ��캯��������protected����
		Manager() {
			//�ж��Ƿ����ṩ��������
			assert(CreateByGet);
		};

		//ͬ��
		std::vector<U> Information;
	private:
		//����ģʽ�����ø�ֵ���캯���뿽�����캯��
		Manager(const Manager &)=delete;
		Manager& operator=(const Manager&)=delete;
		//�б��Ƿ�Ϊ�ṩ����������
		static bool CreateByGet;

		//Ψһ����
		static T *only;
};
//��ʼ����̬��Ա
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


//ѧ����Ϣ������
class StuManager:public Manager<StuManager,Stu> {
	 
		friend class Manager<StuManager,Stu>;
	public:
		//¼�� 
		void Enter(Stu stu);
		void Enter(const char* sex,LUINT number,const char* nation,DATE birth,const char* name,const char* place);
		void Sort(MODE mode);
		//��ѧ��Ѱ��
		int Find(LUINT number);
		//ɾ��ĳһѧ�Ŷ�Ӧ��ѧ������
		void Erase(LUINT number);

};

void StuManager::Enter(Stu stu) {
	Information.push_back(stu);
}

void StuManager::Enter(const char* sex,LUINT number,const char* nation,DATE birth,const char* name,const char* place) {
	Information.emplace_back(sex,number,nation,birth,name,place);
}

void StuManager::Sort(MODE mode) {
	//���������function
	std::function<bool(Stu,Stu,MODE)> sortByMode;

	//����ģʽ����ѡ��function
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
	//����lambda���ʽ��ϵ����
	auto sortfoo = [mode,&sortByMode](Stu a,Stu b)->bool {
		return sortByMode(a,b,mode);
	};
	//����
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

//������Ϣ������
class TestManager:public Manager<TestManager,TestInfo> {
	public:
		//¼������
		//���ǵ�¼��һ�����Ե����ݺ�����������������һ������info��scores�������ô��ݵ�ģʽ
		//����const��ԭ����Ҫʹ���ƶ�����
		void Enter(TestInfo &info);
		void Enter(DATE date,const char* name,const char* notes,SSCORE &scores);

		//������Ϣ,����
		void SaveInfo(const char* addr);

		//��ȡ��Ϣ,����
		void ReadInfo(const char* addr);
		//��
		int Find(UINT ID); 
	private:
		//��¼���Ի�����Ϣ��Ŀ¼
		std::vector<Desc> TestIndex;
};

void TestManager::Enter(TestInfo &info) {
	//��¼λ��
	UINT size = info.Scores.size()*sizeof(SSpair);
	//����emplace_back�������ڲ�ֱ�ӹ���
	TestIndex.emplace_back(info.Date,info.Name,info.Notes,size);
	//�ƶ����壬ǳ����scores
	Information.emplace_back(std::move(info));
}
void TestManager::Enter(DATE date,const char* name,const char* notes,SSCORE &scores) {

	//ͬ��
	UINT size = scores.size()*sizeof(SSpair);
	TestIndex.emplace_back(date,name,notes,size);
	Information.emplace_back(date,name,notes,std::move(scores));
}

void TestManager::SaveInfo(const char* addr) {
	//���¼��Ϣ�����ݱ����ļ�����׺Ϊdatdesc
	char addr2[100];
	strcpy(addr2,addr);
	strcat(addr2,"desc");
	//�����¼
	std::ofstream Out(addr2,std::ios::out|std::ios::binary);
	for(auto i:addr2) {
		VecSave(addr2,TestIndex);
	}
	Out.close();

	//��������
	Out.open(addr,std::ios::out|std::ios::binary);
	//TestInfo�ϴ�,��for_each���Ե������ķ�ʽ�������ݣ����⿽��
	for_each(Information.begin(),Information.end(),[&Out](TestInfo tar) {
		Out.write(reinterpret_cast<char*>(tar.Scores.data()),tar.Scores.size()*sizeof(SSpair));
	});
	Out.close();
}
void TestManager::ReadInfo(const char* addr) {
	//���¼��Ϣ�����ݱ����ļ�����׺Ϊdatdesc
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


//�˻�������

class AcManager:public Manager<AcManager,AccountInfo> {
	public:		
		//infoͨ���ǴӴ����������Ϣ
		//��info�汾ͨ������ע��
		Statu Register(AccountInfo info);
		Statu Register(const char* id,const char *passw,Type t);
		Statu SignIn(AccountInfo info);
		Statu SignIn(const char* id,const char *passw,Type t);
		
		//���ض����� 
		void ReadInfo(const char* addr);
		
		//��ѧ��Ѱ��
		int Find(LUINT number);
		//ɾ��ĳһѧ�Ŷ�Ӧ��ѧ������
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
//�������ݵ��ļ���(���ڵ�ǰĿ¼��)
char AcInfoAddr[] = "AcInfo.dat";
char StuInfoAddr[] = "StuInfo.dat";
char TestInfoAddr[] = "TestInfo.dat";

//��������Ŀ���
char cinbuf[1000];
using namespace std; 
//�˵���
class Menu {
	public:
		Menu():next(nullptr) {};
		//��ʾ
		virtual void Show() = 0;
		//�ȴ�����
		virtual void Wait() = 0;

		//ת���˵�
		//��������Ϊָ������ã�Ŀ���Ǹı�ָ���ָ��
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



//��¼�˵�
class LoginMenu:public Menu {
	public:
		void Show();
		void Wait();
};

//��ʦ����˵�
class TeacherMenu:public Menu {
	public:
		void Show();
		void Wait();
};

//ѧ����Ϣ�˵�
class StuInfoMenu:public Menu {
	public:
		void Show();
		void Wait();
};

//�ɼ�����˵�
class ScoreMenu:public Menu {
	public:
		void Show();
		void Wait();
};

//ѧ������˵�
class StudentMenu:public Menu {
	public:
		void Show();
		void Wait();
		StudentMenu(LUINT number,Type t = Student):IDNumber(number),type(t) {}
	private:
		LUINT IDNumber;//ѧ��
		Type type;
		unordered_set<UINT> TestID;//��¼������
};


//ѧ����Ϣ����˵�
class StuInfoOrderMenu:public Menu {
	public:
		void Show();
		void Wait();
};


//���Գɼ�����˵�
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
	cout << "��ӭʹ�ñ�ѧ������ϵͳ��\n���ȵ�¼���ٽ��в���\n(ps:ѧ�����˺�Ϊѧ�ţ���ʼ����Ϊ123456)" << endl;
}
void LoginMenu::Wait() {
	putchar('\n');
	AcManager &ac = AcManager::getManager();
	char id[20];
	char password[20];
	while(1) {
		cout << "�ʺ�:" << flush;
		while(cin.good()) {
			cin >> cinbuf;
			if(strlen(cinbuf) <= 20) {
				strcpy(id,cinbuf);
				break;
			}
			cout << "�ʺ���������ܳ���20λ" << endl;
		}
		cout << "����:" << flush;
		while(cin.good()) {
			cin >> cinbuf;
			if(strlen(cinbuf) <= 20) {
				strcpy(password,cinbuf);
				break;
			}
			cout << "�ʺ���������ܳ���20λ" << endl;
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

		//ö�����μӷ��������жϲ�ƥ�仹��δע��
		if(isStudent + isTeacher == _dispatch) {
			cout << "�˺����벻ƥ��" << endl;
		} else {
			cout << "���ʺ���δע��" << endl;
		}

		system("pause");
		system("cls");
	}
}

void TeacherMenu::Show() {
	system("cls");
	//������printf��ӡ�Ƿ������
	printf("***************�˵�*****************\n");
	printf("          (1)ѧ����Ϣ\n");
	printf("          (2)�ɼ�����\n");
	printf("          (3)��ѯĳһѧ��\n");
	printf("          (4)�˳�\n");
	printf("************************************\n");

}
void TeacherMenu::Wait() {
	LUINT Number;
	while(cin.good()) {
		cout << "������Ҫ�Ĺ�����ţ�" << flush;
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
				cout << "��Ҫ��ѯ��ѧ����ѧ�ţ�" << flush;
				cin >> Number;
				if(cinBad("�Ƿ����룡")) continue;
				next = new StudentMenu(Number,Teacher);
				break;
			case '4':

				break;
			default:
				cout << "����ѡ����ȷ�Ĺ�����ţ�" << flush;
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
	printf("*************ѧ����Ϣ***************\n");
	printf("          (1)¼��ѧ����Ϣ\n");
	printf("          (2)�޸�ѧ����Ϣ\n");
	printf("          (3)ɾ��ѧ����Ϣ\n");
	printf("          (4)�鿴����ѧ����Ϣ\n");
	printf("          (5)����\n");
	printf("          (6)�˳�����\n");
	printf("************************************\n");
}
void StuInfoMenu::Wait() {
	StuManager &s = StuManager::getManager();
	AcManager &ac = AcManager::getManager();
	//switch�в�����������
	LUINT Number;//ѧ��
	DATE Birth;//����
	char Sex[10];//�Ա�
	char Nation[10];//����
	char Name[20];//����
	char Place[40];//����
	char birth[20];//���գ��ַ���
	while(cin.good()) {
		cout << "������Ҫ�Ĺ�����ţ�" << flush;
		cin >> cinbuf;

		char choose = cinbuf[0];
		switch(choose) {
			case '1':
				//¼��
				cout << "ѧ�ţ�" << flush;
				cin >> Number;
				if(cinBad("�Ƿ����룡")) continue;

				cout << "������" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 20) {
					cout << "���Ȳ�Ӧ�ó���10�������ַ���" << flush;
					system("pause");
					Show();
				}
				strcpy(Name,cinbuf);

				cout << "�Ա�" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 10) {
					cout << "���Ȳ�Ӧ�ó���5�������ַ���" << flush;
					system("pause");
					Show();
				}
				strcpy(Sex,cinbuf);

				cout << "���壺" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 10) {
					cout << "���Ȳ�Ӧ�ó���5�������ַ���" << flush;
					system("pause");
					Show();
				}
				strcpy(Nation,cinbuf);

				cout << "����(����-��-�յĸ�ʽ����)��" << flush;
				cin >> cinbuf;
				memcpy(birth,cinbuf,20);
				Birth = date2long(birth);

				cout << "���᣺" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 40) {
					cout << "���Ȳ�Ӧ�ó���20�������ַ���" << flush;
					system("pause");
					Show();
					continue;
				}
				strcpy(Place,cinbuf);

				//�ж�ѧ����Ϣ�Ƿ��Ѿ�¼��
				//ѧ��Ĭ������123456
				char num[20];
				long2str(Number,num);
				if(ac.Register(num,"123456") != _success) {
					cout << "��ѧ����Ϣ�Ѿ����ڣ�" << flush;
					system("pause");
					Show();
					continue;
				}
				s.Enter(Sex,Number,Nation,Birth,Name,Place);
				s.SaveInfo(StuInfoAddr);
				ac.SaveInfo(AcInfoAddr);
				cout << "¼��ɹ���" << flush;
				system("pause");
				Show();

				break;
			case '2':
				cout << "��Ҫ�޸ĵ�ѧ����ѧ�ţ�" << flush;
				cin >> Number;
				if(cinBad("�Ƿ����룡")) continue;

				//�ж��Ƿ�Ϊ��
				if(s.Find(Number) == s.Size()) {
					cout << "��δ��¼��ѧ��," << flush;
					system("pause");
					Show();
					continue;
				}
				//¼��
				cout << "����¼��������" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 20) {
					cout << "���Ȳ�Ӧ�ó���10�������ַ���" << flush;
					system("pause");
					Show();
				}
				strcpy(Name,cinbuf);

				cout << "�Ա�" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 10) {
					cout << "���Ȳ�Ӧ�ó���5�������ַ���" << flush;
					system("pause");
					Show();
				}
				strcpy(Sex,cinbuf);

				cout << "���壺" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 10) {
					cout << "���Ȳ�Ӧ�ó���5�������ַ���" << flush;
					system("pause");
					Show();
				}
				strcpy(Nation,cinbuf);

				cout << "����(����-��-�յĸ�ʽ����)��" << flush;
				cin >> cinbuf;
				memcpy(birth,cinbuf,20);
				Birth = date2long(birth);

				cout << "���᣺" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 40) {
					cout << "���Ȳ�Ӧ�ó���20�������ַ���" << flush;
					system("pause");
					Show();
					continue;
				}
				strcpy(Place,cinbuf);

				//ɾ��������
				s.Erase(Number);
				s.Enter(Sex,Number,Nation,Birth,Name,Place);
				s.SaveInfo(StuInfoAddr);
				cout << "¼��ɹ���" << flush;
				system("pause");
				Show();
				break;
			case '3':
				cout << "��Ҫɾ����ѧ����ѧ�ţ�" << flush;
				cin >> Number;
				if(cinBad("�Ƿ����룡")) continue;

				//�ж��Ƿ�Ϊ��

				if(s.Find(Number) == s.Size()) {
					cout << "��δ��¼��ѧ��," << flush;
					system("pause");
					Show();
					continue;
				}
				s.Erase(Number);
				ac.Erase(Number);
				s.SaveInfo(StuInfoAddr);
				ac.SaveInfo(AcInfoAddr);
				cout << "ɾ���ɹ���" << flush;
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
				cout << "��ѡ����ȷ�Ĺ�����ţ�" << flush;
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
	printf("*************�ɼ�����***************\n");
	printf("          (1)�½�������Ϣ\n");
	printf("          (2)��ѯĳһ�����Եľ�����Ϣ\n");
	printf("          (3)��ѯ����Ŀ¼��Ϣ\n");
	printf("          (4)����\n");
	printf("          (5)�˳�����\n");
	printf("************************************\n");
}
void ScoreMenu::Wait() {
	StuManager &s = StuManager::getManager();
	TestManager &t = TestManager::getManager();

	while(cin.good()) {
		cout << "������Ҫ�Ĺ�����ţ�" << flush;
		cin >> cinbuf;
		char choose = cinbuf[0];
		char Name[20];//��Ŀ��
		DATE Date;//��������
		char Notes[100];//������Ϣ
		char date[20];//�ַ�������
		LUINT number;
		float score;//�ɼ�
		UINT ID;//������
		UINT index;//Ŀ¼ָ��
		int size;//��¼��С
		TestInfo test;//������Ϣ

		switch(choose) {
			case '1':
				//¼��
				cout << "��Ŀ����" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 20) {
					cout << "���Ȳ�Ӧ�ó���10�������ַ���" << flush;
					system("pause");
					Show();
				}
				strcpy(Name,cinbuf);

				cout << "��������(����-��-�յĸ�ʽ����)��" << flush;
				cin >> cinbuf;
				strcpy(date,cinbuf);
				Date = date2long(date);

				cout << "ע�ͣ�������Ϣ����" << flush;
				cin >> cinbuf;
				if (strlen(cinbuf) >= 20) {
					cout << "���Ȳ�Ӧ�ó���10�������ַ���" << flush;
					system("pause");
					Show();
				}
				strcpy(Notes,cinbuf);
				test.Set(Date,Name,Notes);
				cout << "¼��ѧ���ĸ�����" << flush;
				//����������Ϊ����
				UINT n;
				cin >> n;
				if(cinBad("��Ч�ĸ���!")) continue;
				for(index = 0; index < n; ++index) {

					cout << "ѧ�ţ�" << flush;
					cin >> number;

					if(cinBad("��Ч�ĳɼ�!")) continue;
					if(s.Find(number) == s.Size()) {
						cout << "��ѧ����δ¼�룬�޷��Ǽǳɼ���" << flush;
						system("pause");
						Show();
						break;
					}

					cout << "�ɼ���" << flush;
					cin >> score;

					if(cinBad("��Ч�ĳɼ�!")) continue;
					test.PushInfo(number,score);
				}
				t.Enter(test);
				t.SaveInfo(TestInfoAddr);
				cout << "¼��ɹ���" << flush;
				system("pause");
				Show();
				break;
			case '2':
				ID = 0;
				cout << "���뿼���ţ���Ŀ¼�в�ѯ����";
				cin >> ID;
				if(cinBad("��Ч�Ŀ�����!")) continue;
				index = t.Find(ID);
				if(index == t.Size()) {
					cout << "δ�ҵ��ÿ����Ŷ�Ӧ�Ŀ�����Ϣ����ȷ���Ƿ�¼�룡\n" << flush;
					system("pause");
					Show();
					break;
				}

				next = new ScoreOrderMenu(index);

				goto A;
			case '3':
				size = t.Size();
				if(size == 0) {
					cout << "��Ŀ¼!\n" << flush;
					system("pause");
					Show();
					break;
				}
				for(index = 0; index < size; ++index) {
					t[index].Index();
					putchar('\n');
				}
				cout << "չʾ��ϣ�" << flush;
				system("pause");
				Show();
				break;
			case '4':
				next = new TeacherMenu();
				goto A;
			case '5':

				goto A;
			default:
				cout << "����ѡ����ȷ�Ĺ�����ţ�" << flush;
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
	printf("***************��Ŀ*****************\n");
	for(int i = 0; i < test.Size(); ++i) {
		if(test[i].Exist(IDNumber)) {
			test[i].Index();
			TestID.insert(test[i].getID());
		}
	}
	if(type = Student) {
		printf("************(����0�˳�)*************\n");
	} else {
		printf("************(����0����)*************\n");
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
		cout << "����ѯ�Ŀ��Ժţ�" << flush;
		UINT n;
		cin >> n;
		if(cinBad("��Ч�Ŀ��Ժ�!")) continue;
		if(n == 0) {
			break;
		}
		//�ж��Ƿ��иÿ���
		if(t.Find(n) == t.Size()) {
			cout << "��δ�ٰ�ôο���!" << flush;
			system("pause");
			Show();
			continue;
		}
		//�ж�ѧ���Ƿ���������
		if(TestID.count(n) == 0) {
			cout << "��ѧ�ŵ�ѧ����δ�μӸÿ���!" << flush;
			system("pause");
			Show();
			continue;
		}
		system("cls");
		cout << "��ѯ������£�" << endl;
		int index = s.Find(IDNumber);
		int TestIndex = t.Find(n);
		t[TestIndex].Index();
		print(IDNumber,s[index].name(),"�ɼ���",t[TestIndex].SingleScore(IDNumber));
		system("pause");
		Show();
	}

}
void StuInfoOrderMenu::Show() {
	system("cls");
	printf("*************����ģʽ***************\n");
	printf("          (1)��ѧ��\n");
	printf("          (2)������\n");
	printf("          (3)������\n");
	printf("          (4)������\n");
	printf("          (5)����\n");
	printf("          (6)�˳�����\n");
	printf("************************************\n");
}

void StuInfoOrderMenu::Wait() {

	StuManager &s = StuManager::getManager();

	while(cin.good()) {
		cout << "������Ҫ�Ĺ�����ţ�" << flush;
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
				cout << "����ѡ����ȷ�Ĺ�����ţ�" << flush;
				system("pause");
				Show();
				continue;
		}
		for(int i = 0; i < s.Size(); ++i) {
			s[i].Display();
		}
		cout << "չʾ��ϣ�" << flush;
		system("pause");
		Show();
	}
A:
	return;
}

void ScoreOrderMenu::Show() {
	system("cls");
	printf("*************��ţ�%d***************\n",index);
	printf("          (1)�ɼ��Ӹߵ���\n");
	printf("          (2)�ɼ��ӵ͵���\n");
	printf("          (3)ѧ�ŴӸߵ���\n");
	printf("          (4)ѧ�Ŵӵ͵���\n");
	printf("          (5)����\n");
	printf("          (6)�˳�����\n");
	printf("************************************\n");
}
void ScoreOrderMenu::Wait() {

	StuManager &s = StuManager::getManager();
	TestManager &t = TestManager::getManager();
	int tmp = 0;
	while(cin.good()) {
		cout << "������Ҫ�Ĺ�����ţ�" << flush;
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
				cout << "����ѡ����ȷ�Ĺ�����ţ�" << flush;
				system("pause");
				Show();
				continue;
		}
		for(auto i:t[index].GetScore()) {
			tmp = s.Find(i.first);
			print(i.first,s[tmp].name(),i.second);
		}
		cout << "��ѯ���," << flush;
		system("pause");
		Show();
	}
A:
	return;
}

int main() {

	//��ʼ��
	Initialize(stu,test,ac);
	stu.ReadInfo(StuInfoAddr);
	test.ReadInfo(TestInfoAddr);
	ac.ReadInfo(AcInfoAddr);

	//��¼����...
	Menu *head = new LoginMenu();
	Menu *m = head;
	
	//�˵�ѭ������ѭͳһ�ӿڵĲ˵����ܼ���ѭ�� 
	while(m != nullptr) {
		m->Show();
		m->Wait();
		Menu::Transition(m);
	}
	//�������һҳ�˵� 
	delete m;

	return 0;
}
