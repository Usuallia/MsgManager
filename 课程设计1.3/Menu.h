#ifndef __MENU_H___
#define __MENU_H___
//ȫ���ƿ���
#include "Manager.h"
#include<unordered_set>

//���������ͳ�ʼ���ĺ�
#define Init(_varname,_manager) _manager &_varname = _manager::getManager()
#define Initialize(_var1,_var2,_var3) Init(_var1,StuManager);    \
	Init(_var2,TestManager);   \
	Init(_var3,AcManager);
using namespace std;

//�������ݵ��ļ���(���ڵ�ǰĿ¼��)
char AcInfoAddr[] = "AcInfo.dat";
char StuInfoAddr[] = "StuInfo.dat";
char TestInfoAddr[] = "TestInfo.dat";

//��������Ŀ���
char cinbuf[1000];

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
		int index;//Ŀ¼ָ��
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
#endif
