#include <iostream>
using namespace std;
void main() {
	string s = "01234";
	for (unsigned int i = s.size() - 1; i >= 0; i--) {
		cout << s[i] << endl;
	}
}
