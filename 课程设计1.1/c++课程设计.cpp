
#include "Manager.h"
//���������ĺ�
#define Init(_varname,_manager) _manager &_varname = _manager::getManager()
using namespace std;

//�������ݵ��ļ���(���ڵ�ǰĿ¼��)
char AcInfo[] = "AcInfo.dat";
char StuInfo[] = "StuInfo.dat";
char TestInfo[] = "TestInfo.dat";

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
		Menu* Transition(){
			return next;
		};
		void Initialize(){
			//��ʼ�� 
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

//��¼�˵�
class LoginMenu:public Menu {
	public:
		void Show();
		void Wait();		
};

//��ʦ����˵� 
class TeacherMenu:public Menu{
	public:
		void Show();
		void Wait();
};

//ѧ����Ϣ�˵� 
class StuInfoMenu:public Menu{
	public:
		void Show();
		void Wait();
};

//�ɼ�����˵� 
class ScoreMenu:public Menu{
	public:
		void Show();
		void Wait();
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
			break;
		}

		//ö�����μӷ��������жϲ�ƥ�仹��δע��
		if(isStudent + isTeacher == _fail) {
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
	printf("          (4)�˳�����\n");
	printf("************************************\n");

}
void TeacherMenu::Wait(){
	while(cin.good()){
		cout << "��������Ҫ�Ĺ�����ţ�" << flush;
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
				cout << "��ѡ����ȷ�Ĺ�����ţ�" << flush;
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
	printf("          (4)�˳�����\n");
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
	//����...
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
