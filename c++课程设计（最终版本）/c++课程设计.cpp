//�˵��� 
#include "Menu.h"
 
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
