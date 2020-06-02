
#include<unordered_map>
#include<cstring>
#include"util.h"

using CHINT = std::unordered_map<std::string,LUINT>;
class Account{
	public:
		Account();
		Account(const char* id,const char* passw);//ע�� 
		int SingIn();//��¼ 
		virtual Function() = 0;//�����Ӧ���� 
	private:
		LUINT password; //��ϣ���ܺ������ 
		char ID[20];//�ʺţ�ѧ����Ӧѧ�ţ����Ҷ�Ӧ���� 
		static CHINT ACMap;
};

CHINT Account::ACMap;

/*��c++��׼���ṩ�Ĺ�ϣ��򵥼�������*/
Account::Account(const char* id,const char* passw){
	strcpy(ID,id);
	std::hash<std::string> hasher;
	password = hasher(std::string(passw));
	SingIn();
}

Account::SingIn(){
	if(!ACMap.count(std::string(ID))){
		return 0;
	}
	if(password != ACMap[std::string(ID)]){
		return -1;
	}
	Function();
	return 1;
}
int main(){
	
	using namespace std;
	hash<string> hasher;
	unsigned long long k = hasher(string("avcs"));
	print(k);
	cout << sizeof(hasher) << endl;
		
	
}
