#include "Menu.h"
 
int main() {

	//��ʼ��
	Initialize(stu,test,ac);
	stu.ReadInfo(StuInfoAddr);
	test.ReadInfo(TestInfoAddr);
	ac.ReadInfo(AcInfoAddr);

	//����...
	Menu *head = new LoginMenu();
	Menu *m = head;

	while(m != nullptr) {
		m->Show();
		m->Wait();
		Menu::Transition(m);
	}
	delete m;

	return 0;
}
