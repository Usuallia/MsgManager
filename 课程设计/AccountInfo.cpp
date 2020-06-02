
#include<unordered_map>
#include<cstring>
#include"util.h"

using CHINT = std::unordered_map<std::string,LUINT>;
class Account{
	public:
		Account();
		Account(const char* id,const char* passw);//注册 
		int SingIn();//登录 
		virtual Function() = 0;//进入对应功能 
	private:
		LUINT password; //哈希加密后的密码 
		char ID[20];//帐号，学生对应学号，教室对应工号 
		static CHINT ACMap;
};

CHINT Account::ACMap;

/*用c++标准库提供的哈希类简单加密密码*/
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
