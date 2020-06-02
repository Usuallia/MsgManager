#ifndef __MANAGER_H_
#define __MANAGER_H_
#include<cassert>

#include<vector>

#include "StudentBaseInfo.h"
#include "TestInfo.h"

//ȫ���ƿ��ࣨ���ࣩ
template<typename T,typename U>
class Manager {
	public:

		//ʵ�ּ򵥵ĵ���ģʽ��Managerȫ��������һ��
		static T &getManager() {
			if(only == nullptr) {
				//�б𿪹�
				CreateByGet = true;
				only = new T();
				CreateByGet = false;
			}
			return *only;
		}

		/*���ڼ����ͣ�����ǰ���ݽ����޸ļ�飬���������ݸ�������
		�������Ƿ��޸Ĺ�Դ���ݣ�������󶼻�һ��������д������*/

		//������Ϣ,�ṩĬ�ϰ汾
		virtual void SaveInfo(const char* addr);

		//��ȡ��Ϣ,�ṩĬ�ϰ汾
		virtual void ReadInfo(const char* addr);

		//����ӿ�,��ģʽ����
		virtual void Sort(char mode) = 0;

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
		void Sort(char mode);

};
void StuManager::Sort(char mode) {

}


//������Ϣ������
class TestManager:public Manager<TestManager,TestInfo> {
	public:
		void Sort(char mode);

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

void TestManager::Sort(char mode = 0) {

}

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
		Out.write((char*)tar.Scores.data(),tar.Scores.size()*sizeof(SSpair));
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

int main() {

//	TestInfo A(2020006008,"��ѧ����","��");
//	A.PushInfo(3118006260,72);
//	A.PushInfo(3118006261,78);
//	A.PushInfo(3118006262,63);

	TestManager &M = TestManager::getManager();


	M.ReadInfo("TestInfo.dat");
	
	print(M.Size());
}
#endif
