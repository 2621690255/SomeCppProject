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
typedef pair<time_t, bool> pr; //预约时间，是否成功
const int pharmacy_cnt = 5; //药店数量
const int mask_cnt = 10; //每个店铺能预约成功的口罩数量

void Get_Time(time_t); //输出时间
int String_To_Int(string); //字符串变成整数
string Input_PassWord(); //输入密码显示*号

struct User {
	//用户类
	string name; //姓名
	string ID; //身份证号
	string password; //账号密码
	int success_cnt; //总预约成功次数
	int choose_pharmacy; //记录预约的哪家店铺
	//typedef pair<time_t, bool> pr; 预约时间，是否成功
	pr order; //最新预约纪录
	vector<pr> history; //历史记录
	//const int pharmacy_cnt = 5; //药店数量
	int cnt[pharmacy_cnt]; //每家药店预约失败的次数

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
	}//用户初始化

	void Write(ofstream& s) {
		s << name << endl << ID << endl << password << endl;
		s << success_cnt << endl << choose_pharmacy << endl;
		s << order.first << endl << order.second << endl;
		s << history.size() << endl;
		for (vector<pr>::iterator it = history.begin(); it != history.end(); ++it) s << it->first << endl << it->second << endl;
		for (int i = 0; i < pharmacy_cnt; ++i) s << cnt[i] << endl;
	} //写入文件流
	void Read(ifstream& s) {
		s >> name >> ID >> password;
		s >> success_cnt >> choose_pharmacy;
		s >> order.first >> order.second;
		int n; s >> n; pr it;
		for (int i = 0; i < n; ++i) s >> it.first >> it.second, history.push_back(it);
		for (int i = 0; i < pharmacy_cnt; ++i) s >> cnt[i];
	} //从文件流读入
	void Display() {
		cout << "用户姓名：" << name << endl;
		cout << "    总预约成功次数：" << success_cnt << endl;
		printf("    %d 号店铺累计失败预约次数：%d\n", choose_pharmacy + 1, cnt[choose_pharmacy]);
		cout << "    预约时间："; Get_Time(order.first); cout << endl << endl;
	}//查看用户信息
};

bool operator<(const User& a, const User& b) {
	if (a.success_cnt == b.success_cnt) {
		if (a.cnt[a.choose_pharmacy] == b.cnt[b.choose_pharmacy])
			//预约时间早的用户 优于 预约时间晚的用户
			return a.order.first > b.order.first;
		//针对每家店铺：
		//连续预约失败次数多的用户 优于 连续预约失败次数少的用户
		return a.cnt[a.choose_pharmacy] < b.cnt[b.choose_pharmacy];
	}
	//总预约成功次数少的用户 优于 总预约成功次数多的用户
	return a.success_cnt > b.success_cnt;
}

priority_queue<User> pharmacy[pharmacy_cnt]; //优先队列
map<string, User> users; //用户数据

void Save(); //保存数据
void Load(); //从文件加载数据
void User_Registered(); //用户注册
void User_Login(); //用户登录
void Main_Menu(); //主菜单
void Judge(); //判断是否预约成功
void System_Text(); //测试系统

void User_Menu(User); //用户菜单
void Online_Booking(User); //在线预约
void History_Record(User); //查看历史记录
void Query_Rules(); //查询优先级规则

int main() {
	system("color 30");
	Load(); //加载文件
	Main_Menu(); //进入菜单页
	printf("\n\t退出系统成功，祝您生活愉快！\n\n"); SPC;
	return 0;
}

void Get_Time(time_t tim) {
	//输出时间
	struct tm* at; at = localtime(&tim);
	char now[80]; strftime(now, 79, "%Y-%m-%d %H:%M:%S", at);
	printf("%s", now);
}

int String_To_Int(string op) {
	//字符串变成整数
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
		if (ch != 8) { //不是退格就录入
			//PassWord.push_back(ch);
			str[k++] = ch; putchar('*'); //并输出*号
		}
		else {
			//这里是删除一个，我们通过输出退格符\b，回退一个，输出空格盖住刚才的*，再回撤一格等待输入
			putchar('\b'); putchar(' '); putchar('\b');
			//if (!PassWord.empty()) PassWord.pop_back();
			if (k > 0) --k;
		}
	}
	putchar('\n'); str[k] = '\0'; PassWord = str; return PassWord;
}//输入密码并显示*号

void Save() { //存档
	ofstream OutputStream("Account.in"); //打开要写的文件
	for (map<string, User>::iterator it = users.begin(); it != users.end(); ++it) it->second.Write(OutputStream);
	OutputStream.close(); //关闭文件
}

void Load() { //读档
	ifstream InStream("Account.in"); //打开要读的文件
	while (true) {
		if (InStream.fail()) break;
		User temp; temp.Read(InStream);
		if (temp.ID != "") users[temp.ID] = temp;
		if (temp.choose_pharmacy != -1) pharmacy[temp.choose_pharmacy].push(temp);
	}
	InStream.close(); //关闭文件
}

void Main_Menu() {
	while (true) {
		PRINTX;
		cout << "   欢迎使用XX口罩预约系统，很高兴为您服务！" << endl;
		cout << "\t\t尊敬的用户,您好!" << endl;
		cout << "\t当前系统时间："; time_t t; Get_Time(time(&t)); cout << endl;
		PRINTX;
		cout << "\t\t1.用户注册" << endl;
		cout << "\t\t2.用户登录" << endl;
		cout << "\t\t3.手动处理预约" << endl;
		cout << "\t\t4.测试系统" << endl;
		cout << "\t\t0.退出系统" << endl;
		PRINTX;
		printf("请输入选项前的数字以继续操作："); string op; cin >> op;
		int switch_on = String_To_Int(op); system("cls");
		switch (switch_on) {
		case 1: User_Registered(); break;
		case 2: User_Login(); break;
		case 3: Judge(); break;
		case 4: System_Text(); break;
		case 0: return;
		default: printf("错误的指令，请重新输入！\n"); SPC; break;
		}
	}
}

void Judge() {
	for (int i = 0; i < pharmacy_cnt; ++i) {
		if (pharmacy[i].empty()) { printf("%d 号店铺无人预约！\n", i + 1); continue; }
		printf("%d 号店铺成功预约的用户：\n", i + 1);
		int cnt = 0;
		while (!pharmacy[i].empty() && cnt < mask_cnt) {
			User item = pharmacy[i].top(); pharmacy[i].pop(); ++cnt;
			map<string, User>::iterator foundit = users.find(item.ID);
			if (foundit != users.end()) {
				foundit->second.success_cnt++; foundit->second.history.push_back(pr(item.order.first, true));
				foundit->second.choose_pharmacy = -1; foundit->second.cnt[i] = 0;
			}
			cout << "\t" << cnt << "、 ID:" << item.ID << " 姓名: " << item.name << endl;
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
	cout << "已处理最新预约用户!" << endl; SPC;
}

void System_Text() {
	string op;
	while (true) {
		PRINTL;
		cout << "  欢迎进入软件测试中心！" << endl;
		cout << "\t当前系统时间："; time_t t; Get_Time(time(&t)); cout << endl;
		PRINTL;
		printf("您可以选择使用特定数据或随机生成数据：\n");
		printf(" 1、使用特定数据  2、随机生成数据\n 0、返回主菜单\n");
		PRINTL;
		printf("请输入您的选择："); cin >> op;
		if (op == "1" || op == "2") break;
		else if (op == "0") { system("cls"); return; }
		else { printf("错误的命令，请重新输入！\n"); SPC; }
	}
	if (op == "1") {
		User t[5]; //姓名，ID，店铺号，连续预约，总成功，最新预约
		t[0] = User("特定用户1", "特定用户1", 0, 5, 10, pr(time(NULL) - 10, false));
		t[1] = User("特定用户2", "特定用户2", 0, 2, 5, pr(time(NULL) + 30, false));
		t[2] = User("特定用户3", "特定用户3", 0, 10, 10, pr(time(NULL), false));
		t[3] = User("特定用户4", "特定用户4", 0, 20, 15, pr(time(NULL) - 25, false));
		t[4] = User("特定用户5", "特定用户5", 0, 10, 10, pr(time(NULL) + 15, false));
		for (int i = 0; i < 5; ++i) t[i].Display(), pharmacy[0].push(t[i]);
	}
	else {
		printf("温馨提示：客官下手轻点、数据不要那么刁钻 ORZ\n");
		printf("请输入需要生成的用户数量：");
		int n; cin >> n; printf("\n");
		srand((unsigned)time(NULL));
		for (int i = 0; i < n; ++i) {
			char num[100]; User t;
			t.name = "随机用户"; t.name += itoa(i + 1, num, 10); t.ID = t.name;
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
		cout << "   欢迎进入用户注册界面，很高兴为您服务！" << endl;
		cout << "\t当前系统时间："; time_t t; Get_Time(time(&t)); cout << endl;
		PRINTL;
		cout << "请输入您需要注册的身份证号(输入-1返回主菜单)：" << endl;
		string user_id; cin >> user_id;
		if (user_id == "-1") { system("cls");  return; }
		if (!users.count(user_id)) {
			string user_password[2]; cout << "请输入您的密码： " << endl;
			while (user_password[0] = Input_PassWord(), user_password[0] == "");
			cout << "请再次确认密码： " << endl; user_password[1] = Input_PassWord();
			if (user_password[0] == user_password[1]) {
				string user_name; cout << "请输入您的姓名：" << endl; cin >> user_name;
				users[user_id] = User(user_id, user_password[0], user_name);
				Save();
				cout << "注册成功!" << endl; PRINTL; SPC; return;
			}
			else { cout << "第二次密码与第一次输入不匹配，请重新注册！" << endl; PRINTL; SPC; }
		}
		else { cout << "该账户已被注册，请重新输入！" << endl; PRINTL; SPC; }
	}
}

void User_Login() {
	while (true) {
		PRINTL;
		cout << "   欢迎进入用户登录界面，很高兴为您服务！" << endl;
		cout << "\t当前系统时间："; time_t t; Get_Time(time(&t)); cout << endl;
		PRINTL;
		cout << "请输入您需要登录的账号(输入-1返回主菜单)：" << endl;
		string user_id; cin >> user_id; system("cls");
		if (user_id == "-1") { system("cls");  return; }
		map<string, User>::iterator foundit = users.find(user_id);
		if (foundit != users.end()) {
			PRINTL;
			cout << "用户：" << user_id << "  您好!\n" << endl;
			cout << "(不输入密码直接按回车键即可退出本界面)" << endl;
			PRINTL; printf("请输入您的密码：\n");
			string user_password = Input_PassWord();
			if (user_password == "") { system("cls"); return; }
			if (user_password == foundit->second.password) {
				cout << "身份验证通过！正在跳转..." << endl; SPC;
				User_Menu(foundit->second); return;
			}
			else { cout << "密码错误，请重试！" << endl; SPC; }
		}
		else { PRINTL; cout << "账户" << user_id << "不存在，请重新输入！" << endl; PRINTL; SPC; }
	}
}

void User_Menu(User item) {
	while (true) {
		map<string, User>::iterator foundit; foundit = users.find(item.ID);
		PRINTX;
		cout << "  尊敬的用户 " << foundit->second.name << ": 您好!" << endl;
		cout << "\t当前系统时间："; time_t t; Get_Time(time(&t)); cout << endl;
		PRINTX;
		cout << "\t\t1.在线预约" << endl;
		cout << "\t\t2.历史记录" << endl;
		cout << "\t\t3.查询规则" << endl;
		cout << "\t\t0.返回主菜单" << endl;
		PRINTX;
		printf("请输入选项前的数字以继续操作："); string op; cin >> op;
		int switch_on = String_To_Int(op); system("cls");
		switch (switch_on) {
		case 1: Online_Booking(foundit->second); break;
		case 2: History_Record(foundit->second); break;
		case 3: Query_Rules(); break;
		case 0: return;
		default: printf("错误的指令，请重新输入！\n"); SPC; break;
		}
	}
}

void Online_Booking(User item) {
	while (true) {
		map<string, User>::iterator foundit; foundit = users.find(item.ID);
		PRINTL;
		cout << "  尊敬的用户 " << foundit->second.name << ": 您好!" << endl;
		cout << "\t当前系统时间："; time_t t; Get_Time(time(&t)); cout << endl;
		PRINTL;
		if (foundit->second.choose_pharmacy != -1) {
			printf("您已于 ");
			Get_Time(foundit->second.order.first);
			printf(" 在 %d号店铺 预约, 无须重复预约！\n", foundit->second.choose_pharmacy + 1);
			PRINTL; SPC; return;
		}
		for (int i = 0; i < pharmacy_cnt; ++i) printf("\t\t%d.店铺%d\n", i + 1, i + 1);
		cout << "\t\t0.取消预约" << endl;
		PRINTL;
		cout << "请选择您需要预约的店铺：" << endl; string op; cin >> op;
		int switch_on = String_To_Int(op);
		if (switch_on > 0 && switch_on <= pharmacy_cnt) {
			foundit->second.cnt[switch_on - 1]++; foundit->second.choose_pharmacy = switch_on - 1;
			foundit->second.order = pr(time(NULL), false);
			pharmacy[switch_on - 1].push(foundit->second);
			Save(); printf("您已预约！\n"); SPC; return;
		}
		else if (!switch_on) { SPC; return; }
		else { printf("错误的指令，请重新输入！\n"); SPC; }
	}
}

void History_Record(User item) {
	if (item.history.empty()) {
		PRINTL;
		cout << "用户: " << item.name << " 无预约记录！" << endl;
		PRINTL; SPC; return;
	}
	int Size = item.history.size(), Page = Size % 5 ? Size / 5 + 1 : Size / 5;
	for (int i = 0, j = 0; i < Page;) {
		PRINTL;
		printf("当前显示为第%d页\t共%d页\n\n", i + 1, Page);
		printf("\t\t日期\t\t\t预约情况\n");
		for (int k = j; k < j + 5 && k < Size; ++k) {
			printf("%8d. ", k + 1); Get_Time(item.history[k].first); cout << "\t\t";
			if (item.history[k].second) cout << "预约成功" << endl;
			else cout << "预约失败" << endl;
		}
		PRINTL;
		printf("\tP.上一页\tN.下一页\n\tA.输入页面进行查询\n\t0.返回上一层\n");
		PRINTL;
		printf("请输入选项前的代码以继续操作："); string str; cin >> str;
		int op = 0; if (str == "P") op = -1; else if (str == "N") op = 1;
		else if (str == "0") { cout << "正在退出，请稍后...\n"; SPC; return; }
		else if (str == "A") {
			cout << "请输入您需要跳转的页面：";
			cin >> str; int switch_on = String_To_Int(str);
			if (switch_on >= 1 && switch_on <= Page) { i = switch_on - 1; j = i * 5; }
			else { printf("输入页数不存在,请重新输入！\n"); SPC; continue; }
		}
		else { cout << "错误的命令，请重新输入！" << endl; SPC; continue; }
		if ((i + op >= 0) && (i + op < Page)) { i += op; j = i * 5; }
		else { cout << "访问的页面不存在,请重新输入！" << endl; SPC; continue; }
		cout << "正在跳转，请稍后..." << endl; SPC;
	}
}

void  Query_Rules() {
	PRINTL;
	cout << "1、总预约成功次数少的用户 > 总预约成功次数多的用户" << endl;
	cout << "2、针对每家店铺：连续预约失败次数多的用户 > 连续预约失败次数少的用户" << endl;
	cout << "3、预约时间早的用户 > 预约时间晚的用户" << endl;
	PRINTL;
	SPC;
}
