//菜单类 
#include "Menu.h"
 
int main() {

	//初始化
	Initialize(stu,test,ac);
	stu.ReadInfo(StuInfoAddr);
	test.ReadInfo(TestInfoAddr);
	ac.ReadInfo(AcInfoAddr);

	//登录界面...
	Menu *head = new LoginMenu();
	Menu *m = head;
	
	//菜单循环，遵循统一接口的菜单都能加入循环 
	while(m != nullptr) {
		m->Show();
		m->Wait();
		Menu::Transition(m);
	}
	
	//销毁最后一页菜单 
	delete m;

	return 0;
}
