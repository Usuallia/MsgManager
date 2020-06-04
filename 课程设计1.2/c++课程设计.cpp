#include "Menu.h"
 
int main() {

	//初始化
	Initialize(stu,test,ac);
	stu.ReadInfo(StuInfoAddr);
	test.ReadInfo(TestInfoAddr);
	ac.ReadInfo(AcInfoAddr);

	//界面...
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
