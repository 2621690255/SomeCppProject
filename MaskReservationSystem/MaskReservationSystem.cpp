#include <iostream>
#include <cstdio>
#include <fstream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <stack>
#include <queue>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <list>

#define endl '\n'
#define SPC system("pause"); system("cls");
#define PRINTX 	printf("*************************************************\n");
#define PRINTL printf("+---------------------------------------------+\n");

using namespace std;
typedef pair<time_t, bool> pr; //ԤԼʱ�䣬�Ƿ�ɹ�
const int pharmacy_cnt = 5; //ҩ������
const int mask_cnt = 10; //ÿ��������ԤԼ�ɹ��Ŀ�������

void Get_Time(time_t); //���ʱ��
int String_To_Int(string); //�ַ����������
string Input_PassWord(); //����������ʾ*��

struct User {
	//�û���
	string name; //����
	string ID; //���֤��
	string password; //�˺�����
	int success_cnt; //��ԤԼ�ɹ�����
	int choose_pharmacy; //��¼ԤԼ���ļҵ���
	//typedef pair<time_t, bool> pr; ԤԼʱ�䣬�Ƿ�ɹ�
	pr order; //����ԤԼ��¼
	vector<pr> history; //��ʷ��¼
	//const int pharmacy_cnt = 5; //ҩ������
	int cnt[pharmacy_cnt]; //ÿ��ҩ��ԤԼʧ�ܵĴ���

	User() {
		name = ""; ID = ""; password = ""; success_cnt = 0;
		order = pr(0, false);
		choose_pharmacy = -1; history.clear(); memset(cnt, 0, sizeof(cnt));
	}
	User(string user_id, string user_password, string user_name) {
		name = user_name; ID = user_id; password = user_password;
		order = pr(0, false);
		success_cnt = 0; choose_pharmacy = -1; history.clear(); memset(cnt, 0, sizeof(cnt));
	}
	User(string _name, string _ID, int _choose_pharmacy, int _cnt, int _success_cnt, pr _order) {
		name = _name; ID = _ID; password = "";
		order = _order; success_cnt = _success_cnt;
		choose_pharmacy = _choose_pharmacy; memset(cnt, 0, sizeof(cnt));
		cnt[choose_pharmacy] = _cnt; history.clear();
	}//�û���ʼ��

	void Write(ofstream& s) {
		s << name << endl << ID << endl << password << endl;
		s << success_cnt << endl << choose_pharmacy << endl;
		s << order.first << endl << order.second << endl;
		s << history.size() << endl;
		for (vector<pr>::iterator it = history.begin(); it != history.end(); ++it) s << it->first << endl << it->second << endl;
		for (int i = 0; i < pharmacy_cnt; ++i) s << cnt[i] << endl;
	} //д���ļ���
	void Read(ifstream& s) {
		s >> name >> ID >> password;
		s >> success_cnt >> choose_pharmacy;
		s >> order.first >> order.second;
		int n; s >> n; pr it;
		for (int i = 0; i < n; ++i) s >> it.first >> it.second, history.push_back(it);
		for (int i = 0; i < pharmacy_cnt; ++i) s >> cnt[i];
	} //���ļ�������
	void Display() {
		cout << "�û�������" << name << endl;
		cout << "    ��ԤԼ�ɹ�������" << success_cnt << endl;
		printf("    %d �ŵ����ۼ�ʧ��ԤԼ������%d\n", choose_pharmacy + 1, cnt[choose_pharmacy]);
		cout << "    ԤԼʱ�䣺"; Get_Time(order.first); cout << endl << endl;
	}//�鿴�û���Ϣ
};

bool operator<(const User& a, const User& b) {
	if (a.success_cnt == b.success_cnt) {
		if (a.cnt[a.choose_pharmacy] == b.cnt[b.choose_pharmacy])
			//ԤԼʱ������û� ���� ԤԼʱ������û�
			return a.order.first > b.order.first;
		//���ÿ�ҵ��̣�
		//����ԤԼʧ�ܴ�������û� ���� ����ԤԼʧ�ܴ����ٵ��û�
		return a.cnt[a.choose_pharmacy] < b.cnt[b.choose_pharmacy];
	}
	//��ԤԼ�ɹ������ٵ��û� ���� ��ԤԼ�ɹ���������û�
	return a.success_cnt > b.success_cnt;
}

priority_queue<User> pharmacy[pharmacy_cnt]; //���ȶ���
map<string, User> users; //�û�����

void Save(); //��������
void Load(); //���ļ���������
void User_Registered(); //�û�ע��
void User_Login(); //�û���¼
void Main_Menu(); //���˵�
void Judge(); //�ж��Ƿ�ԤԼ�ɹ�
void System_Text(); //����ϵͳ

void User_Menu(User); //�û��˵�
void Online_Booking(User); //����ԤԼ
void History_Record(User); //�鿴��ʷ��¼
void Query_Rules(); //��ѯ���ȼ�����

int main() {
	system("color 30");
	Load(); //�����ļ�
	Main_Menu(); //����˵�ҳ
	printf("\n\t�˳�ϵͳ�ɹ���ף��������죡\n\n"); SPC;
	return 0;
}

void Get_Time(time_t tim) {
	//���ʱ��
	struct tm* at; at = localtime(&tim);
	char now[80]; strftime(now, 79, "%Y-%m-%d %H:%M:%S", at);
	printf("%s", now);
}

int String_To_Int(string op) {
	//�ַ����������
	int sum = 0;
	for (string::iterator it = op.begin(); it != op.end(); ++it)
		if (*it >= '0' && *it <= '9') sum = sum * 10 + (*it - '0');
		else return -1;
	return sum;
}

string Input_PassWord() {
	char str[1000]; int k = 0;
	string PassWord; PassWord.clear();
	char ch; while (ch = getch(), ch != '\r' && ch != '\n') {
		if (ch != 8) { //�����˸��¼��
			//PassWord.push_back(ch);
			str[k++] = ch; putchar('*'); //�����*��
		}
		else {
			//������ɾ��һ��������ͨ������˸��\b������һ��������ո��ס�ղŵ�*���ٻس�һ��ȴ�����
			putchar('\b'); putchar(' '); putchar('\b');
			//if (!PassWord.empty()) PassWord.pop_back();
			if (k > 0) --k;
		}
	}
	putchar('\n'); str[k] = '\0'; PassWord = str; return PassWord;
}//�������벢��ʾ*��

void Save() { //�浵
	ofstream OutputStream("Account.in"); //��Ҫд���ļ�
	for (map<string, User>::iterator it = users.begin(); it != users.end(); ++it) it->second.Write(OutputStream);
	OutputStream.close(); //�ر��ļ�
}

void Load() { //����
	ifstream InStream("Account.in"); //��Ҫ�����ļ�
	while (true) {
		if (InStream.fail()) break;
		User temp; temp.Read(InStream);
		if (temp.ID != "") users[temp.ID] = temp;
		if (temp.choose_pharmacy != -1) pharmacy[temp.choose_pharmacy].push(temp);
	}
	InStream.close(); //�ر��ļ�
}

void Main_Menu() {
	while (true) {
		PRINTX;
		cout << "   ��ӭʹ��XX����ԤԼϵͳ���ܸ���Ϊ������" << endl;
		cout << "\t\t�𾴵��û�,����!" << endl;
		cout << "\t��ǰϵͳʱ�䣺"; time_t t; Get_Time(time(&t)); cout << endl;
		PRINTX;
		cout << "\t\t1.�û�ע��" << endl;
		cout << "\t\t2.�û���¼" << endl;
		cout << "\t\t3.�ֶ�����ԤԼ" << endl;
		cout << "\t\t4.����ϵͳ" << endl;
		cout << "\t\t0.�˳�ϵͳ" << endl;
		PRINTX;
		printf("������ѡ��ǰ�������Լ���������"); string op; cin >> op;
		int switch_on = String_To_Int(op); system("cls");
		switch (switch_on) {
		case 1: User_Registered(); break;
		case 2: User_Login(); break;
		case 3: Judge(); break;
		case 4: System_Text(); break;
		case 0: return;
		default: printf("�����ָ����������룡\n"); SPC; break;
		}
	}
}

void Judge() {
	for (int i = 0; i < pharmacy_cnt; ++i) {
		if (pharmacy[i].empty()) { printf("%d �ŵ�������ԤԼ��\n", i + 1); continue; }
		printf("%d �ŵ��̳ɹ�ԤԼ���û���\n", i + 1);
		int cnt = 0;
		while (!pharmacy[i].empty() && cnt < mask_cnt) {
			User item = pharmacy[i].top(); pharmacy[i].pop(); ++cnt;
			map<string, User>::iterator foundit = users.find(item.ID);
			if (foundit != users.end()) {
				foundit->second.success_cnt++; foundit->second.history.push_back(pr(item.order.first, true));
				foundit->second.choose_pharmacy = -1; foundit->second.cnt[i] = 0;
			}
			cout << "\t" << cnt << "�� ID:" << item.ID << " ����: " << item.name << endl;
		}
		while (!pharmacy[i].empty()) {
			User item = pharmacy[i].top(); pharmacy[i].pop();
			map<string, User>::iterator foundit = users.find(item.ID);
			if (foundit != users.end()) {
				foundit->second.history.push_back(item.order); foundit->second.choose_pharmacy = -1;
			}
		}
	}
	Save();
	cout << "�Ѵ�������ԤԼ�û�!" << endl; SPC;
}

void System_Text() {
	string op;
	while (true) {
		PRINTL;
		cout << "  ��ӭ��������������ģ�" << endl;
		cout << "\t��ǰϵͳʱ�䣺"; time_t t; Get_Time(time(&t)); cout << endl;
		PRINTL;
		printf("������ѡ��ʹ���ض����ݻ�����������ݣ�\n");
		printf(" 1��ʹ���ض�����  2�������������\n 0���������˵�\n");
		PRINTL;
		printf("����������ѡ��"); cin >> op;
		if (op == "1" || op == "2") break;
		else if (op == "0") { system("cls"); return; }
		else { printf("�����������������룡\n"); SPC; }
	}
	if (op == "1") {
		User t[5]; //������ID�����̺ţ�����ԤԼ���ܳɹ�������ԤԼ
		t[0] = User("�ض��û�1", "�ض��û�1", 0, 5, 10, pr(time(NULL) - 10, false));
		t[1] = User("�ض��û�2", "�ض��û�2", 0, 2, 5, pr(time(NULL) + 30, false));
		t[2] = User("�ض��û�3", "�ض��û�3", 0, 10, 10, pr(time(NULL), false));
		t[3] = User("�ض��û�4", "�ض��û�4", 0, 20, 15, pr(time(NULL) - 25, false));
		t[4] = User("�ض��û�5", "�ض��û�5", 0, 10, 10, pr(time(NULL) + 15, false));
		for (int i = 0; i < 5; ++i) t[i].Display(), pharmacy[0].push(t[i]);
	}
	else {
		printf("��ܰ��ʾ���͹�������㡢���ݲ�Ҫ��ô���� ORZ\n");
		printf("��������Ҫ���ɵ��û�������");
		int n; cin >> n; printf("\n");
		srand((unsigned)time(NULL));
		for (int i = 0; i < n; ++i) {
			char num[100]; User t;
			t.name = "����û�"; t.name += itoa(i + 1, num, 10); t.ID = t.name;
			t.choose_pharmacy = rand() % pharmacy_cnt;
			t.cnt[t.choose_pharmacy] = rand();
			t.success_cnt = rand();
			t.order = pr((time(NULL) - rand()) % LONG_MAX, false);
			t.Display(); pharmacy[t.choose_pharmacy].push(t);
		}
	}
	Judge();
}

void User_Registered() {
	while (true) {
		PRINTL;
		cout << "   ��ӭ�����û�ע����棬�ܸ���Ϊ������" << endl;
		cout << "\t��ǰϵͳʱ�䣺"; time_t t; Get_Time(time(&t)); cout << endl;
		PRINTL;
		cout << "����������Ҫע������֤��(����-1�������˵�)��" << endl;
		string user_id; cin >> user_id;
		if (user_id == "-1") { system("cls");  return; }
		if (!users.count(user_id)) {
			string user_password[2]; cout << "�������������룺 " << endl;
			while (user_password[0] = Input_PassWord(), user_password[0] == "");
			cout << "���ٴ�ȷ�����룺 " << endl; user_password[1] = Input_PassWord();
			if (user_password[0] == user_password[1]) {
				string user_name; cout << "����������������" << endl; cin >> user_name;
				users[user_id] = User(user_id, user_password[0], user_name);
				Save();
				cout << "ע��ɹ�!" << endl; PRINTL; SPC; return;
			}
			else { cout << "�ڶ����������һ�����벻ƥ�䣬������ע�ᣡ" << endl; PRINTL; SPC; }
		}
		else { cout << "���˻��ѱ�ע�ᣬ���������룡" << endl; PRINTL; SPC; }
	}
}

void User_Login() {
	while (true) {
		PRINTL;
		cout << "   ��ӭ�����û���¼���棬�ܸ���Ϊ������" << endl;
		cout << "\t��ǰϵͳʱ�䣺"; time_t t; Get_Time(time(&t)); cout << endl;
		PRINTL;
		cout << "����������Ҫ��¼���˺�(����-1�������˵�)��" << endl;
		string user_id; cin >> user_id; system("cls");
		if (user_id == "-1") { system("cls");  return; }
		map<string, User>::iterator foundit = users.find(user_id);
		if (foundit != users.end()) {
			PRINTL;
			cout << "�û���" << user_id << "  ����!\n" << endl;
			cout << "(����������ֱ�Ӱ��س��������˳�������)" << endl;
			PRINTL; printf("�������������룺\n");
			string user_password = Input_PassWord();
			if (user_password == "") { system("cls"); return; }
			if (user_password == foundit->second.password) {
				cout << "�����֤ͨ����������ת..." << endl; SPC;
				User_Menu(foundit->second); return;
			}
			else { cout << "������������ԣ�" << endl; SPC; }
		}
		else { PRINTL; cout << "�˻�" << user_id << "�����ڣ����������룡" << endl; PRINTL; SPC; }
	}
}

void User_Menu(User item) {
	while (true) {
		map<string, User>::iterator foundit; foundit = users.find(item.ID);
		PRINTX;
		cout << "  �𾴵��û� " << foundit->second.name << ": ����!" << endl;
		cout << "\t��ǰϵͳʱ�䣺"; time_t t; Get_Time(time(&t)); cout << endl;
		PRINTX;
		cout << "\t\t1.����ԤԼ" << endl;
		cout << "\t\t2.��ʷ��¼" << endl;
		cout << "\t\t3.��ѯ����" << endl;
		cout << "\t\t0.�������˵�" << endl;
		PRINTX;
		printf("������ѡ��ǰ�������Լ���������"); string op; cin >> op;
		int switch_on = String_To_Int(op); system("cls");
		switch (switch_on) {
		case 1: Online_Booking(foundit->second); break;
		case 2: History_Record(foundit->second); break;
		case 3: Query_Rules(); break;
		case 0: return;
		default: printf("�����ָ����������룡\n"); SPC; break;
		}
	}
}

void Online_Booking(User item) {
	while (true) {
		map<string, User>::iterator foundit; foundit = users.find(item.ID);
		PRINTL;
		cout << "  �𾴵��û� " << foundit->second.name << ": ����!" << endl;
		cout << "\t��ǰϵͳʱ�䣺"; time_t t; Get_Time(time(&t)); cout << endl;
		PRINTL;
		if (foundit->second.choose_pharmacy != -1) {
			printf("������ ");
			Get_Time(foundit->second.order.first);
			printf(" �� %d�ŵ��� ԤԼ, �����ظ�ԤԼ��\n", foundit->second.choose_pharmacy + 1);
			PRINTL; SPC; return;
		}
		for (int i = 0; i < pharmacy_cnt; ++i) printf("\t\t%d.����%d\n", i + 1, i + 1);
		cout << "\t\t0.ȡ��ԤԼ" << endl;
		PRINTL;
		cout << "��ѡ������ҪԤԼ�ĵ��̣�" << endl; string op; cin >> op;
		int switch_on = String_To_Int(op);
		if (switch_on > 0 && switch_on <= pharmacy_cnt) {
			foundit->second.cnt[switch_on - 1]++; foundit->second.choose_pharmacy = switch_on - 1;
			foundit->second.order = pr(time(NULL), false);
			pharmacy[switch_on - 1].push(foundit->second);
			Save(); printf("����ԤԼ��\n"); SPC; return;
		}
		else if (!switch_on) { SPC; return; }
		else { printf("�����ָ����������룡\n"); SPC; }
	}
}

void History_Record(User item) {
	if (item.history.empty()) {
		PRINTL;
		cout << "�û�: " << item.name << " ��ԤԼ��¼��" << endl;
		PRINTL; SPC; return;
	}
	int Size = item.history.size(), Page = Size % 5 ? Size / 5 + 1 : Size / 5;
	for (int i = 0, j = 0; i < Page;) {
		PRINTL;
		printf("��ǰ��ʾΪ��%dҳ\t��%dҳ\n\n", i + 1, Page);
		printf("\t\t����\t\t\tԤԼ���\n");
		for (int k = j; k < j + 5 && k < Size; ++k) {
			printf("%8d. ", k + 1); Get_Time(item.history[k].first); cout << "\t\t";
			if (item.history[k].second) cout << "ԤԼ�ɹ�" << endl;
			else cout << "ԤԼʧ��" << endl;
		}
		PRINTL;
		printf("\tP.��һҳ\tN.��һҳ\n\tA.����ҳ����в�ѯ\n\t0.������һ��\n");
		PRINTL;
		printf("������ѡ��ǰ�Ĵ����Լ���������"); string str; cin >> str;
		int op = 0; if (str == "P") op = -1; else if (str == "N") op = 1;
		else if (str == "0") { cout << "�����˳������Ժ�...\n"; SPC; return; }
		else if (str == "A") {
			cout << "����������Ҫ��ת��ҳ�棺";
			cin >> str; int switch_on = String_To_Int(str);
			if (switch_on >= 1 && switch_on <= Page) { i = switch_on - 1; j = i * 5; }
			else { printf("����ҳ��������,���������룡\n"); SPC; continue; }
		}
		else { cout << "�����������������룡" << endl; SPC; continue; }
		if ((i + op >= 0) && (i + op < Page)) { i += op; j = i * 5; }
		else { cout << "���ʵ�ҳ�治����,���������룡" << endl; SPC; continue; }
		cout << "������ת�����Ժ�..." << endl; SPC;
	}
}

void  Query_Rules() {
	PRINTL;
	cout << "1����ԤԼ�ɹ������ٵ��û� > ��ԤԼ�ɹ���������û�" << endl;
	cout << "2�����ÿ�ҵ��̣�����ԤԼʧ�ܴ�������û� > ����ԤԼʧ�ܴ����ٵ��û�" << endl;
	cout << "3��ԤԼʱ������û� > ԤԼʱ������û�" << endl;
	PRINTL;
	SPC;
}
