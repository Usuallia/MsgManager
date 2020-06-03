
#include "Manager.h"
//用来声明的宏
#define Init(_varname,_manager) _manager &_varname = _manager::getManager()
using namespace std;

//几个数据的文件名(都在当前目录下)
char AcInfo[] = "AcInfo.dat";
char StuInfo[] = "StuInfo.dat";
char TestInfo[] = "TestInfo.dat";

//减少溢出的可能
char cinbuf[1000];

//菜单类
class Menu {
	public:
		Menu():next(nullptr) {};
		//显示
		virtual void Show() = 0;
		//等待输入
		virtual void Wait() = 0;
		//转换菜单
		Menu* Transition(){
			return next;
		};
		void Initialize(){
			//初始化 
			AcManager &ac = AcManager::getManager();
			ac.ReadInfo(AcInfo);
			StuManager &stu = StuManager::getManager();
			stu.ReadInfo(StuInfo);
			TestManager &test = TestManager::getManager();
			test.ReadInfo(TestInfo);
		}
	protected:
		Menu* next;
};

//登录菜单
class LoginMenu:public Menu {
	public:
		void Show();
		void Wait();		
};

//教师界面菜单 
class TeacherMenu:public Menu{
	public:
		void Show();
		void Wait();
};

//学籍信息菜单 
class StuInfoMenu:public Menu{
	public:
		void Show();
		void Wait();
};

//成绩管理菜单 
class ScoreMenu:public Menu{
	public:
		void Show();
		void Wait();
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
			break;
		}

		//枚举整形加法，用于判断不匹配还是未注册
		if(isStudent + isTeacher == _fail) {
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
	printf("          (4)退出程序\n");
	printf("************************************\n");

}
void TeacherMenu::Wait(){
	while(cin.good()){
		cout << "请输入需要的功能序号：" << flush;
		cin >> cinbuf;
		char choose = cinbuf[0];
		switch(choose){
			case '1':
				next = new StuInfoMenu(); 
				break;
			case '2':
				next = new ScoreMenu();
				break;
			case '3':
				
				break;
			case '4':	
				break;
			default:
				cout << "请选择正确的功能序号，" << flush;
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
	printf("          (4)退出程序\n");
	printf("************************************\n");
}
void StuInfoMenu::Wait(){
	StuManager &m = StuManager::getManager();
	
}

void ScoreMenu::Show() {
	system("cls");

}
void ScoreMenu::Wait(){
	
}

int main() {
	//界面...
	Menu *head = new LoginMenu();
	head->Initialize();
	Menu *m = head;
	
	while(m != nullptr) {
		m->Show();
		m->Wait();
		m = m->Transition();
	}
	while(head != nullptr){
		cout << "delete" << endl;
		m = head;
		head = head->Transition();
		delete m;
	}
	return 0;
}
