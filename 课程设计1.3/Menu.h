#ifndef __MENU_H___
#define __MENU_H___
//全局掌控类
#include "Manager.h"
#include<unordered_set>

//用来声明和初始化的宏
#define Init(_varname,_manager) _manager &_varname = _manager::getManager()
#define Initialize(_var1,_var2,_var3) Init(_var1,StuManager);    \
	Init(_var2,TestManager);   \
	Init(_var3,AcManager);
using namespace std;

//几个数据的文件名(都在当前目录下)
char AcInfoAddr[] = "AcInfo.dat";
char StuInfoAddr[] = "StuInfo.dat";
char TestInfoAddr[] = "TestInfo.dat";

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
		int index;//目录指针
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
#endif
