#ifndef __MANAGER_H_
#define __MANAGER_H_
#include<cassert>

#include<vector>
#include<functional>

#include "StudentBaseInfo.h"
#include "TestInfo.h"
#include "AccountInfo.h"

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
		void Sort(MODE mode);

};


void StuManager::Sort(MODE mode) {
	//���������function 
	std::function<bool(Stu,Stu,MODE)> sortByMode;
	
	//����ģʽ����ѡ��function 
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
	//����lambda���ʽ��ϵ���� 
	auto sortfoo = [mode,&sortByMode](Stu a,Stu b)->bool{
		return sortByMode(a,b,mode);
	};
	//���� 
	sort(Information.begin(),Information.end(),sortfoo);
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
		Information.emplace_back(i.Date,i.Name,i.Notes,std::move(tmpScore));
		offset += i.Size;
	}
}

//�˻�������

class AcManager:public Manager<AcManager,AccountInfo>{
	public:
		//infoͨ���ǴӴ����������Ϣ
		//��info�汾ͨ������ע�� 
		Statu Register(AccountInfo info);
		Statu Register(const char* id,const char *passw,Type t);
		Statu SignIn(AccountInfo info);
		Statu SignIn(const char* id,const char *passw,Type t);
		//����
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
//	TestInfo A(2020006008,"��ѧ����","��");
//	A.PushInfo(3118006260,72);
//	A.PushInfo(3118006261,78);
//	A.PushInfo(3118006262,63);
//	TestManager &M = TestManager::getManager();
//	M.ReadInfo("TestInfo.dat");
//	print(M.Size());
//}
#endif
