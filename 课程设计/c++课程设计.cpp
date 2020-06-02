#include<iostream>
#include<fstream>
#include<unordered_map> 

using namespace std;







int main() {
//	auto m = Manager::getManager();
//
//	cout << (m == nullptr) << endl;

	hash<string> hasher;
	char ch[] = "abcdefgw\0";
	cout << ch << endl;
	
	cout << hasher(string(ch)) << endl;
}
