#include <iostream> //C++ 流输入输出库
#include <fstream> //C++ 文件流输入输出库
#include <cstdio> //C 标准输入输出库
#include <conio.h> //C 控制台输入输出库 getch()不回显读入
#include <cstdlib> //C 标准函数库
#include <cstring> //C 字符串函数库
#include <ctime> //C 系统时间库
#include <string> //C++ string类库
#include <vector> //C++ vector向量库
#include <map> //C++ map映射库
#include <queue> //C++ queue队列库
#include <stack> //C++ stack栈库
#include <set> //C++ set集合库
#include <list> //C++ list链表库
#include <deque> //C++ deque双端队列库
#include <algorithm> //C++ 常用算法库

//宏定义部分
#define rap(i, a, b) for(int i = (a); i < (b); ++i)
#define rep(i, a, b) for(int i = (a); i >= (b); --i)
#define _PAC_ system("pause"), system("cls")

using namespace std;

//定义别名，方便使用
typedef pair<int, int> pii;
typedef pair<string, string> pss;
typedef pair<string, int> psi;
typedef pair<int, string> pis;
typedef pair<pii, string> piis;

//类声明
struct Time; //时间类
struct Ticket; //客票类
struct User; //用户类
struct Flight; //航线类
struct Graph; //图类

//常量区
const string _STAR_ = "****************************************";
const string _LINE_ = "----------------------------------------";
const string ChineseDay[] = { "日", "一", "二", "三", "四", "五", "六" };
const int filecount = 2; //文件数量
const string filename[] = { "Users.data", "Flights.data" }; //文件名称

//全局变量区
map<string, User> allusers; //用户列表 账号->账户
map<string, Flight> allflights; //航线列表 航班号->航线
multimap<pss, string> nonstopflight; //直达航班 <起点，终点> 航班号
//Graph graph; //图 换乘路线搜索用

//时间类
struct Time {
private:
	tm* mtm;
	time_t mt;
public:
	Time() { mt = time(&mt); mtm = localtime(&mt); }
	int GetYear() { return mtm->tm_year + 1900; }
	int GetMon() { return mtm->tm_mon + 1; }
	int GetDay() { return mtm->tm_mday; }
	int GetWeekDay() { return mtm->tm_wday; }
	void Add(time_t sec) { mt = time(&mt); mt += sec; mtm = localtime(&mt); }
	void YMD() {
		cout << mtm->tm_year + 1900 << "/" << mtm->tm_mon + 1 << "/" << mtm->tm_mday << endl;
	}
	void Display() {
		mt = time(&mt); mtm = localtime(&mt);
		cout << "当前系统时间：";
		cout << mtm->tm_year + 1900 << "年" << mtm->tm_mon + 1 << "月" << mtm->tm_mday << "日 ";
		printf("%02d:%02d:%02d", mtm->tm_hour, mtm->tm_min, mtm->tm_sec);
		cout << " 周" << ChineseDay[mtm->tm_wday] << endl;
	}
};

//自定义图类
struct Graph {
private:
	map<string, int> nametoidx; //地点序号
	map<int, string> idxtoname; //序号代表的地点
	vector<vector<pis> > graph; //图 （邻接表）
	vector<pis> path; //保存路径用 父亲的（编号，航班号）
	vector<bool> vis; //DFS是否已访问
	int answer; //标记两点之间是否有路线可达

	void Add(string); //添加地点 （邻接表加行）
	void OutPath(int); //输出路径
	void DFS(int, int);

public:
	void Clear(); //清空
	int GetIdx(string); //得到地点序号
	string GetName(int); //得到序号地点
	int GetSize(); //不同地点个数
	int GetCount(string); //查询地点是否存在
	void AddEdge(string, string, string); //填加航线 （邻接表加列）
	void Search_Menu(string, string); //搜索菜单
};
void Graph::Clear() {
	nametoidx.clear(); idxtoname.clear();
	graph.clear(); path.clear(); vis.clear(); answer = 0;
}
int Graph::GetIdx(string name) { return nametoidx[name]; }
string Graph::GetName(int idx) { return idxtoname[idx]; }
int Graph::GetSize() { return nametoidx.size(); }
int Graph::GetCount(string name) { return nametoidx.count(name); }
void Graph::Add(string name) {
	if (nametoidx.count(name)) return; //已在则不用加
	int idx = nametoidx.size();
	nametoidx[name] = idx; idxtoname[idx] = name;

	vector<pis> vec; graph.push_back(vec);
	path.push_back(pis(-1, ""));
	vis.push_back(false);
}
void Graph::AddEdge(string v, string u, string way) {
	Add(v); Add(u);
	graph[GetIdx(v)].push_back(pis(GetIdx(u), way));
}
void Graph::Search_Menu(string _start, string _end) {
	cout << _STAR_ << endl;
	Time nowtime; nowtime.Display();
	cout << _STAR_ << endl;
	cout << "\t  您已进入 换乘查询 系统" << endl;
	cout << "1.本页面可查询相关 换乘航班 的详细信息！" << endl;
	cout << _STAR_ << endl;
	answer = 0;
	cout << _start << " 到 " << _end << " 的换乘航线如下：" << endl;
	if (GetCount(_start) && GetCount(_end)) {
		vis[GetIdx(_start)] = true;
		DFS(GetIdx(_start), GetIdx(_end));
		vis[GetIdx(_start)] = false;
	}
	if (!answer) cout << "\n经系统搜索，暂无 " << _start << " 到 " << _end << " 的换乘航线" << endl;
	else cout << "\n请牢记您需要购票的 航班号，并使用 客票预订 功能进行购票操作！" << endl;
	cout << _STAR_ << endl; _PAC_; return;
}
void Graph::DFS(int u, int v) {
	if (u == v) {
		OutPath(v); return;
	}
	for (auto it : graph[u]) {
		if (!vis[it.first]) {
			path[it.first] = pis(u, it.second); vis[it.first] = true;
			DFS(it.first, v);
			path[it.first] = pis(-1, ""); vis[it.first] = false;
		}
	}
}
void Graph::OutPath(int x) {
	int s = x; //起点城市s 终点城市x
	vector<pis> vec; vec.clear();
	//从终点城市x 一直回溯到 起点城市s 并保留路径
	while (path[s].first != -1) vec.push_back(path[s]), s = path[s].first;
	//按相反的顺序输出路径
	cout << endl << ++answer << ". " << endl;
	cout << _LINE_ << endl;
	for (auto it = vec.rbegin(); it != vec.rend(); ++it) cout << GetName(it->first), cout << " =" << it->second << "=》";
	cout << GetName(x) << endl;//输出终点城市
	cout << _LINE_ << endl;
}
Graph graph; //图 换乘路线搜索用

//客票类
struct Ticket {
	string flightNO; //航班号
	string start, end; //起始点，终点
	//pss belong; //客票归属 乘客账号，乘客姓名
	//pii seat; //座位 舱等级，座位号
	string account, name; //客票归属 乘客账号，乘客姓名
	int rating, seatnumber; //座位 舱等级，座位号
	Ticket();
	//Ticket(Flight&, int, int);
	Ticket(string, string, string, int, int);
	void Write(ofstream&);
	Ticket Read(ifstream& in);
	void Display();
};
Ticket::Ticket() {
	flightNO.clear(); start.clear(); end.clear();
	account.clear(); name.clear();
	rating = seatnumber = -1;
}
//Ticket::Ticket(Flight& flight, int _rating, int _seatnumber) {
//	flightNO = flight.flightNO; start = flight.start; end = flight.end;
//	rating = _rating, seatnumber = _seatnumber;
//}
Ticket::Ticket(string _flightNO, string _start, string _end, int  _rating, int _seatnumber) {
	flightNO = _flightNO; start = _start; end = _end;
	account.clear(); name.clear();
	rating = _rating; seatnumber = _seatnumber;
}
void Ticket::Write(ofstream& out) {
	out << flightNO << endl << start << endl << end << endl;
	out << account << endl << name << endl;
	out << rating << endl << seatnumber << endl;
}
Ticket Ticket::Read(ifstream& in) {
	Ticket ticket;
	getline(in, ticket.flightNO); getline(in, ticket.start); getline(in, ticket.end);
	getline(in, ticket.account); getline(in, ticket.name);
	string op; getline(in, op); ticket.rating = stoi(op); getline(in, op); ticket.seatnumber = stoi(op);
	return ticket;
}
void Ticket::Display() {
	cout << _LINE_ << endl;
	cout << "航班号：" << flightNO << endl;
	cout << "起始地-目的地：" << start << "->" << end << endl;
	cout << "(购票账号) 乘客姓名：" << "(" << account << ") " << name << endl;
	cout << "[舱等级] 座位号：" << "[" << rating + 1 << "等] " << seatnumber << "号座" << endl;
	cout << _LINE_ << endl;
}

//客户类
struct User {
	string account, password; //账号，密码
	string name; //姓名
	vector<Ticket> preorder; //预订票列表
	map<string, pii> alternate; //候补记录 航班号->舱等级，候补数量

	User();
	User(string, string, string);

	void Write(ofstream&); //写入文件流
	User Read(ifstream&); //文件流读出

	string Input_PassWord(); //录入密码并显示*号
	void Push_Ticket(Ticket); //取票
	Ticket Pop_Ticket(int); //退票
	void Display(bool); //取票
};
User::User() {
	account.clear(); password.clear(); name.clear(); preorder.clear(); alternate.clear();
}
User::User(string _account, string _password, string _name) {
	account = _account; password = _password; name = _name;
	preorder.clear(); alternate.clear();
}
string User::Input_PassWord() {
	string password;
	do {
		password.clear();
		char ch; while (ch = getch(), ch != '\n' && ch != '\r') {
			if (ch != '\b') password.push_back(ch), putchar('*');
			else {
				if (!password.empty()) putchar('\b'), putchar(' '), putchar('\b'), password.pop_back();
			}
		}
		putchar('\n');
	} while (password.empty());
	return password;
}
void User::Write(ofstream& out) {
	out << account << endl << password << endl << name << endl;
	out << preorder.size() << endl;
	for (auto it : preorder) it.Write(out);
	out << alternate.size() << endl;
	for (auto it : alternate) out << it.first << endl << it.second.first << endl << it.second.second << endl;
}
User User::Read(ifstream& in) {
	User user;
	getline(in, user.account); getline(in, user.password); getline(in, user.name);
	string size;
	getline(in, size);
	rep(i, stoi(size), 1) { Ticket ticket; ticket = ticket.Read(in); user.preorder.push_back(ticket); }
	getline(in, size);
	string flightno, rating, cnt;
	rep(i, stoi(size), 1) getline(in, flightno), getline(in, rating), getline(in, cnt), user.alternate[flightno] = pii(stoi(rating), stoi(cnt));
	return user;
}
void User::Push_Ticket(Ticket ticket) {
	ticket.account = account; ticket.name = name;
	preorder.push_back(ticket); //用户预订票压入
}
Ticket User::Pop_Ticket(int idx) {
	int i = 1; Ticket ticket;
	for (auto it = preorder.begin(); it != preorder.end(); ++i) {
		if (i < idx) ++it; else if (i > idx) break;
		else {
			ticket = *it;
			it = preorder.erase(it);
		}
	}
	return ticket;
}
void User::Display(bool admin) {
	cout << "+" << _LINE_ << "+" << endl;
	cout << "客户账号：" << account << endl;
	if (admin) cout << "账号密码：" << password << endl;
	cout << "客户姓名：" << name << endl;
	cout << "客户订票列表：";
	if (preorder.empty()) cout << "暂无数据！" << endl;
	else {
		cout << endl; int i = 1;
		for (auto it : preorder) cout << i++ << "." << endl, it.Display();
	}
	cout << "客户候补信息：";
	if (alternate.empty()) cout << "暂无数据！" << endl;
	else {
		cout << endl; int i = 1;
		cout << "输出格式：序号.（航班号）[舱等级] 候补数量" << endl;
		for (auto it : alternate) cout << i++ << ".（" << it.first << "）[" << it.second.first + 1 << "等舱] " << it.second.second << "张" << endl;
	}
	cout << "+" << _LINE_ << "+" << endl;
}

//航线类
struct Flight {
	string start, end; //起始地，目的地
	string flightNO, planemodel; //航班号，机型
	int workday; //工作周日
	const static int allrating = 3; //舱等级数量
	int capacity[allrating + 1]; //舱对应容量
	//vector<Ticket> remaining[allrating]; //每个舱位的余票
	queue<Ticket> remaining[allrating]; //每个舱位的余票
	//vector<Ticket> preorder[allrating]; //预定客户
	map<psi, int> preorder; //预订客户， （账号，舱等级）->数量
	//multimap<pii, string> alternate; //候补客户 (舱等级，数量)->账号
	vector<piis> alternate;//候补客户 (舱等级，数量)->账号

	Flight();
	void Write(ofstream&); //写入文件
	Flight Read(ifstream&); //文件读出

	void Full_Ticket(); //余票补齐
	void Push_Ticket(Ticket); //退票 
	Ticket Pop_Ticket(psi); //出票 （乘客账号，舱等级）
	void Alternate_Ticket(pii); //候补排队 (舱等级，数量)
	void Handle_Alternate(int); //处理候补 舱等级

	void Add_NonStopFlight(); //添加直达航线
	void Display(bool);	//显示信息 是否管理
};
Flight::Flight() {
	start.clear(); end.clear(); flightNO.clear(); planemodel.clear();
	workday = 0; memset(capacity, 0, sizeof(capacity));
	//rap(i, 0, allrating) remaining[i].clear();
	rap(i, 0, allrating) while (!remaining[i].empty())remaining[i].pop();
	//rap(i, 0, allrating) preorder[i].clear();
	preorder.clear();
	alternate.clear();
}
void Flight::Write(ofstream& out) {
	out << start << endl << end << endl << flightNO << endl << planemodel << endl;
	out << workday << endl;
	rap(i, 0, allrating + 1) out << capacity[i] << endl;
	rap(i, 0, allrating) {
		out << remaining[i].size() << endl;
		//for (auto it : remaining[i]) it.Write(out);
		while (!remaining[i].empty()) remaining[i].front().Write(out), remaining[i].pop();
	}
	/*rap(i, 0, allrating) {
		out << preorder[i].size() << endl;
		for (auto it : preorder[i]) it.Write(out);
	}*/
	out << preorder.size() << endl;
	for (auto it : preorder) out << it.first.first << endl << it.first.second << endl << it.second << endl;

	out << alternate.size() << endl;
	//for (auto it : alternate) it.Write(out);
	for (auto it : alternate) out << it.first.first << endl << it.first.second << endl << it.second << endl;
}
Flight Flight::Read(ifstream& in) {
	Flight flight;
	getline(in, flight.start); getline(in, flight.end); getline(in, flight.flightNO); getline(in, flight.planemodel);
	string op; getline(in, op); flight.workday = stoi(op);
	rap(i, 0, allrating + 1) getline(in, op), flight.capacity[i] = stoi(op);
	rap(i, 0, allrating) {
		getline(in, op); rep(i, stoi(op), 1) {
			Ticket ticket; ticket = ticket.Read(in);
			//flight.remaining[ticket.rating].push_back(ticket);
			flight.remaining[ticket.rating].push(ticket);
		}
	}
	/*rap(i, 0, allrating) {
		getline(in, op); rep(i, stoi(op), 1) {
			Ticket ticket; ticket = ticket.Read(in);
			flight.preorder[ticket.rating].push_back(ticket);
		}
	}*/
	getline(in, op); rep(i, stoi(op), 1) {
		string _account, _rating, _cnt;
		getline(in, _account); getline(in, _rating); getline(in, _cnt);
		flight.preorder[psi(_account, stoi(_rating))] = stoi(_cnt);
	}

	getline(in, op); rep(i, stoi(op), 1) {
		/*Ticket ticket; ticket = ticket.Read(in);
		flight.alternate.push_back(ticket);*/
		string _account, _rating, _cnt;
		getline(in, _rating); getline(in, _cnt); getline(in, _account);
		//alternate.insert(make_pair(pii(stoi(_rating), stoi(_cnt)), _account));
		flight.alternate.push_back(piis(pii(stoi(_rating), stoi(_cnt)), _account));
	}
	return flight;
}
void Flight::Full_Ticket() {
	rap(i, 0, allrating) rap(j, 0, capacity[i])
		//remaining[i].push_back(Ticket(flightNO, start, end, i, j));
		remaining[i].push(Ticket(flightNO, start, end, i, j));
}
void Flight::Push_Ticket(Ticket ticket) {
	--preorder[psi(ticket.account, ticket.rating)]; //客户预订票 -1
	ticket.account.clear(); ticket.name.clear(); //机票归属置空
	remaining[ticket.rating].push(ticket);
}
Ticket Flight::Pop_Ticket(psi pr) {
	Ticket ticket = remaining[pr.second].front();
	remaining[pr.second].pop(); //航线余票弹出
	preorder[pr]++; //预订列表
	return ticket;
}
void Flight::Alternate_Ticket(pii seat) {
	//alternate.insert(make_pair(seat, flightNO));
	alternate.push_back(piis(seat, flightNO));
}
void Flight::Handle_Alternate(int _rating) {
	int size = remaining[_rating].size();
	for (auto it = alternate.begin(); it != alternate.end();) {
		if (it->first.first == _rating && size == it->first.second) {
			User& user = allusers[it->second];
			while (size--) {
				Ticket ticket = Pop_Ticket(psi(user.account, _rating));
				user.Push_Ticket(ticket);
			}
			it = alternate.erase(it); break;
		}
		else ++it;
	}
}
void Flight::Add_NonStopFlight() {
	nonstopflight.insert(make_pair(pss(start, end), flightNO));
}
void Flight::Display(bool admin) {
	const string nextday[] = { "今天","明天","后天","3天后","4天后","5天后","6天后","一周后" };
	cout << _LINE_ << endl;
	cout << "航班号：" << flightNO << endl;
	if (admin) cout << "起始地-目的地：" << start << "->" << end << endl;
	cout << "飞机型号：" << planemodel << endl;
	cout << "工作周日：" << "周" << ChineseDay[workday] << endl;
	cout << "下次航班：";
	Time nowtime; int day = (workday - nowtime.GetWeekDay() + 7) % 7;
	cout << nextday[day] << " "; nowtime.Add(day * 24ll * 60 * 60); nowtime.YMD();
	cout << "该航班余票详情如下：" << endl;
	rap(i, 0, allrating) {
		cout << "  " << i + 1 << "等舱：" << remaining[i].size() << "张";
		if (admin) cout << " [总载客量：" << capacity[i] << "人]";
		cout << endl;
	}
	if (admin) {
		int i = 1;
		cout << "预购乘客列表(姓名 [舱等级] 购票数量)：";
		if (preorder.empty()) cout << "暂无数据！" << endl;
		else {
			cout << endl;
			for (auto it : preorder)
				cout << i++ << ". " << allusers[it.first.first].name << " [" << it.first.second + 1 << "等舱] " << it.second << "张" << endl;
		}
		i = 1;
		cout << "候补乘客列表(姓名 [舱等级] 需求数量)：";
		if (alternate.empty())cout << "暂无数据！" << endl;
		else {
			cout << endl;
			for (auto it : alternate)
				cout << i++ << ". " << allusers[it.second].name << " [" << it.first.first << "等舱] " << it.first.second << "张" << endl;
		}
	}
	cout << _LINE_ << endl;
}

void Main_Menu(); //主菜单
void User_Menu(User&); //用户菜单

void Tourist_Mode(); //访客模式
void User_Mode(); //用户模式
void Admin_Mode(); //管理模式
void Help_Mode(); //使用说明

void Add_Flight(); //添加航线
void View_Infor(string); //查看信息

void Query_Flight(); //查询航班
void Book_Ticket(User&); //客票预订
void Handle_Refund(User&); //办理退票

void Load(); //文件载入
bool Save(int); //文件写出 参数：0存用户 1存航线

void Initilaze(); //初始化

int stoi(string&); //string 转 操作指令op, 非法操作指令返回-1

int main() {
	Load();
	system("color 30");
	Help_Mode();
	Main_Menu();
	return 0;
}

int stoi(string& s) {
	int sum = 0;
	for (auto it : s)
		if (it >= '0' && it <= '9') sum = sum * 10 + it - '0';
		else return -1;
	return sum;
}

void Main_Menu() {
	while (true) {
		cout << _STAR_ << endl;
		Time nowtime; nowtime.Display();
		cout << _STAR_ << endl << endl;
		cout << "欢迎使用 \"乌鸦坐飞机\" 航空客运订票系统!" << endl;
		cout << endl << _STAR_ << endl;

		cout << "\t1.访客模式" << endl;
		cout << "\t2.用户模式" << endl;
		cout << "\t3.管理模式" << endl;
		cout << "\t4.使用说明" << endl;
		cout << "\t0.退出系统" << endl;
		cout << _STAR_ << endl;

		cout << "请输入选项前的数字以继续操作："; string op; while (getline(cin, op), op.empty());
		switch (stoi(op))
		{
		case 1: system("cls"); Tourist_Mode(); break;
		case 2: system("cls"); User_Mode(); break;
		case 3: system("cls"); Admin_Mode(); break;
		case 4: system("cls"); Help_Mode(); break;
		case 0: cout << "退出系统成功！" << endl; cout << _STAR_ << endl; return;
		default:
			cout << "输入选项有误，请检查后重新输入！ OvO" << endl;
			cout << _STAR_ << endl; _PAC_;
			break;
		}
	}
}

void User_Menu(User& user) {
	bool tourist = false; if (user.account.empty()) tourist = true;
	while (true) {
		cout << _STAR_ << endl;
		Time nowtime; nowtime.Display();
		cout << _STAR_ << endl;
		if (tourist) cout << "\t您正在使用访客模式\n部分功能受限，如需体验全部功能，请使用用户模式!" << endl;
		else cout << "尊敬的用户：" << user.name << ", 您好！" << endl;
		cout << _STAR_ << endl;
		cout << "\t1.查询航班" << endl;
		cout << "\t2.客票预订" << endl;
		cout << "\t3.办理退票" << endl;
		cout << "\t0.返回主菜单" << endl;
		cout << _STAR_ << endl;
		cout << "请输入选项前的数字以继续操作："; string op; while (getline(cin, op), op.empty());
		switch (stoi(op))
		{
		case 1: system("cls"); Query_Flight(); break;
		case 2: system("cls");
			if (!tourist) Book_Ticket(user);
			else {
				cout << _STAR_ << endl;
				cout << "\n  访客模式下，客票预定系统不可用！\n" << endl;
				cout << _STAR_ << endl; _PAC_;
			}
			break;
		case 3: system("cls");
			if (!tourist) Handle_Refund(user);
			else {
				cout << _STAR_ << endl;
				cout << "\n  访客模式下，办理退票业务不可用！\n" << endl;
				cout << _STAR_ << endl; _PAC_;
			}
			break;
		case 0: system("cls"); return;
		default:
			cout << "输入选项有误，请检查后重新输入！ OvO" << endl;
			cout << _STAR_ << endl; _PAC_;
			break;
		}
	}
}

void Tourist_Mode() {
	User user;
	User_Menu(user);
}

void User_Mode() {
	User user;
	cout << _STAR_ << endl;
	Time nowtime; nowtime.Display();
	cout << _STAR_ << endl;
	cout << "\t  您正在使用用户模式\n\t请先注册/登录您的账号！" << endl;
	cout << "若您暂无账号，系统将自动为您注册账号！" << endl;
	cout << _STAR_ << endl;
	cout << "请输入您的账号："; while (getline(cin, user.account), user.account.empty());
	if (user.account == "-1") { system("cls"); return; }
	bool foundit = false; if (allusers.count(user.account)) foundit = true;
	if (!foundit) cout << "账号 " << user.account << " 尚未注册，系统将为您自动注册账号！" << endl;
	cout << "请输入您的密码："; user.password = user.Input_PassWord();
	if (!foundit) {
		cout << "请再次输入您的密码："; string password2 = user.Input_PassWord();
		if (password2 != user.password) {
			cout << "\n注册失败，两次密码不一致，正在返回上一层..." << endl;
			cout << _STAR_ << endl; _PAC_; return;
		}
		else {
			cout << "请输入您的姓名："; while (getline(cin, user.name), user.name.empty());
			if (user.name == "-1") { system("cls"); return; }

			allusers[user.account] = user; Save(0);
			cout << "\n注册成功，请牢记您的账号和密码, 正在跳转..." << endl;
			cout << _STAR_ << endl; _PAC_;
			User_Menu(allusers[user.account]); return;
		}
	}
	else {
		if (user.password != allusers[user.account].password) {
			cout << "\n登录失败，输入密码不正确，正在返回上一层..." << endl;
			cout << _STAR_ << endl; _PAC_; return;
		}
		else system("cls"), User_Menu(allusers[user.account]);
	}
}

void Admin_Mode() {
	while (true) {
		cout << _STAR_ << endl;
		Time nowtime; nowtime.Display();
		cout << _STAR_ << endl;
		cout << "\t您正在使用管理模式\n暂仅支持以下几项操作，其余功能正在陆续开发..." << endl;
		cout << _STAR_ << endl;
		cout << "\t1.添加航线" << endl;
		cout << "\t2.查看航线信息" << endl;
		cout << "\t3.查看乘客信息" << endl;
		cout << "\t0.返回主菜单" << endl;
		cout << _STAR_ << endl;
		cout << "请输入选项前的数字以继续操作："; string op; while (getline(cin, op), op.empty());
		switch (stoi(op))
		{
		case 1: system("cls"); Add_Flight(); break;
		case 2: system("cls"); View_Infor("航线"); break;
		case 3: system("cls"); View_Infor("乘客"); break;
		case 0: system("cls"); return;
		default:
			cout << "输入选项有误，请检查后重新输入！ OvO" << endl;
			cout << _STAR_ << endl; _PAC_;
			break;
		}
	}
}

void Help_Mode() {
	cout << _STAR_ << _STAR_ << endl;
	cout << "使用本系统前，请先认真阅读本页，以学习系统使用方法! OvO" << endl;
	cout << "系统共三种模式可以使用,如下所示：" << endl;
	cout << "\n  访客模式（无需注册账号即可使用）：" << endl;
	cout << "      仅支持 查询航班 功能，如需 购票、退票 请使用用户模式！" << endl;
	cout << "    1.查询航班：" << endl;
	cout << "        输入 起始点 和 目的地 名称即可查询相关航班信息。" << endl;
	cout << "        系统优先显示 直达航班， 若无直达，可选择是否启用 换乘服务。" << endl;
	cout << "        换乘服务：会为您搜索并输出 所有 可达的换乘路线 。" << endl;
	cout << "\n  用户模式（需注册/登录账号后方可使用）：" << endl;
	cout << "    1.查询航班：" << endl;
	cout << "        输入 起始点 和 目的地 名称即可查询相关航班信息。" << endl;
	cout << "    2.客票预订：" << endl;
	cout << "        输入 航班号 和 购票需求（舱位等级、购票数量） 即可预订对应航班。" << endl;
	cout << "        如余票充足，则显示对应座位号； 若余票不足，则询问是否排队候补。" << endl;
	cout << "    3.办理退票：" << endl;
	cout << "        系统输出 用户预订记录表，输入对应 序号 即可取消相应航班。" << endl;
	cout << "\n  管理模式（用于航空公司管理后台）：" << endl;
	cout << "      暂仅支持以下几项功能，其他功能陆续开发中... 敬请期待！" << endl;
	cout << "    1.添加航线：" << endl;
	cout << "        根据系统提示输入 对应航线信息 即可录入相关航班信息。" << endl;
	cout << "    2.查看航线信息：" << endl;
	cout << "        您可以在此项功能中查看 所有航线 的数据信息。" << endl;
	cout << "    3.查看客户信息：" << endl;
	cout << "        您可以在此项功能中查看 所有客户 的数据信息。" << endl;
	cout << "\n  其他技巧（一些补充的操作说明）：" << endl;
	cout << "    1.输入数据：" << endl;
	cout << "        除了一些特定内容外，请确保输入的每一项均位于一行，以免影响其他操作。" << endl;
	cout << "    2.返回上一层：" << endl;
	//cout << "        理论上，除了显示了选项的菜单、输入密码界面。您可以在任何输入项输入 -1 以中断输入并返回。" << endl;
	cout << "        大多数情况下，您可以通过输入 -1 来中断输入并返回上一层界面。" << endl;
	cout << endl << _STAR_ << _STAR_ << endl;
	cout << "(注：页面可能显示不完整，请往上滑! OvO )" << endl; _PAC_;
}

void Add_Flight() {
	Flight flight;
	while (true) {
		cout << _STAR_ << endl << endl;
		Time nowtime; nowtime.Display();
		cout << _STAR_ << endl;
		cout << "\t您正在使用 添加航线 功能" << endl;
		cout << " 1.您可以在任何一项输入时，输入 -1 以返回上一层菜单！" << endl;
		cout << " 2.请确保每一项的输入内容均位于同一行！" << endl;
		cout << endl << _STAR_ << endl;
		string op;

		cout << "请输入航线的起点城市（如：西安）" << endl; while (getline(cin, op), op.empty());
		if (op == "-1") { system("cls"); return; }
		else flight.start = op;

		cout << "请输入航线的终点城市（如：北京）" << endl; while (getline(cin, op), op.empty());
		if (op == "-1") { system("cls"); return; }
		else flight.end = op;

		cout << "请输入航线的航班号（如：CA1202）" << endl; while (getline(cin, op), op.empty());
		if (op == "-1") { system("cls"); return; }
		else if (!allflights.count(op)) flight.flightNO = op;
		else { cout << "该航班号已存在！请重新录入航线信息！" << endl; cout << _STAR_ << endl; _PAC_; continue; }

		cout << "请输入航线的飞机型号（如：BOEING737-800 WINGLETS）" << endl; while (getline(cin, op), op.empty());
		if (op == "-1") { system("cls"); return; }
		else flight.planemodel = op;

		while (true) {
			cout << "请输入航线的运营周日（0~6）(如周日：0)" << endl;
			while (getline(cin, op), op.empty());
			if (op == "-1") { system("cls"); return; }
			int cnt = stoi(op);
			if (cnt >= 0 && cnt < 7) { flight.workday = cnt; break; }
			else cout << "请输入一个 0 到 6 之间的数字！" << endl;
		}

		flight.capacity[flight.allrating] = 0;
		rap(i, 0, flight.allrating) {
			//while (getline(cin, op), op.empty());
			while (true) {
				cout << "请输入" << i + 1 << "等舱 座位数量（如：" << 10 * i + 10 << "）" << endl;
				while (getline(cin, op), op.empty());
				if (op == "-1") { system("cls"); return; }
				int cnt = stoi(op);
				if (cnt >= 0 && cnt < INT_MAX) { flight.capacity[i] = cnt; break; }
				else cout << "请输入一个 0 到 " << INT_MAX << " 之间的数字！" << endl;
				//	cout << "请输入 " << i + 1 << "等舱 座位数量（如：" << 10 * i + 10 << "）" << endl;
			}
			flight.capacity[flight.allrating] += flight.capacity[i];
		}

		flight.Full_Ticket(); //补齐余票
		flight.Add_NonStopFlight(); //添加直达航班
		allflights[flight.flightNO] = flight; Save(1);

		system("cls"); cout << _STAR_ << endl;
		cout << "\n航线 " << flight.flightNO << " 已添加成功！" << endl;
		cout << "是否继续添加？ 1.是 0.否" << endl;
		cout << _STAR_ << endl;
		while (true) {
			cout << "请输入选项前的数字以继续操作：";
			while (getline(cin, op), op.empty());
			if (op == "1") { system("cls"); break; }
			else if (op == "0") { system("cls"); return; }
			else cout << "输入选项有误，请重新输入！" << endl;
		}
	}
}

void View_Infor(string it) {
	cout << _STAR_ << endl << endl;
	Time nowtime; nowtime.Display();
	cout << _STAR_ << endl;
	cout << "\t您正在使用 查看" << it << "信息 功能" << endl;
	cout << " 1.我们将为您显示 所有" << it << "的信息！" << endl;
	cout << endl << _STAR_ << endl;
	if (it == "航线") for (auto it : allflights) it.second.Display(true);
	else for (auto it : allusers) it.second.Display(true);
	cout << _STAR_ << endl; _PAC_;
}

void Query_Flight() {
	string op;
	while (true) {
		cout << _STAR_ << endl;
		Time nowtime; nowtime.Display();
		cout << _STAR_ << endl;
		cout << "\t  您已进入 自助查询 系统\n  本页面可查询航线 航班号 等信息！" << endl;
		cout << "1.输入 起始地、目的地 即可查询相关直达航班信息！" << endl;
		cout << "2.如无 直达航班 信息，可以选择启动 换乘服务！" << endl;
		cout << "3.输入-1以返回上一层菜单" << endl;
		cout << _STAR_ << endl;

		string _start, _end;
		cout << "请输入您的起始地点(如：西安)："; while (getline(cin, _start), _start.empty());
		if (_start == "-1") { system("cls"); return; }
		cout << "请输入您的目的地点(如：北京)："; while (getline(cin, _end), _end.empty());
		if (_end == "-1") { system("cls"); return; }

		auto left = nonstopflight.lower_bound(pss(_start, _end));
		auto right = nonstopflight.upper_bound(pss(_start, _end));
		if (left == right) {
			cout << _STAR_ << endl;
			cout << " " << _start << " 与 " << _end << " 之间暂无直达航线！" << endl;
			cout << "\n系统提供 换乘查询 服务，是否需要为您启动该服务？ 1.需要 0.不需要" << endl;
			cout << _STAR_ << endl;
			while (true) {
				cout << "请输入选项前的数字以继续操作：";
				while (getline(cin, op), op.empty());
				if (op == "-1") { system("cls"); return; }
				else if (op == "1") {
					system("cls"); graph.Search_Menu(_start, _end); break;
				}
				else if (op == "0") break;
			}
			system("cls"); return;
		}
		else {
			cout << "\n为您查询到以下 直达航班 信息：" << endl;
			for (auto it = left; it != right; ++it) allflights[it->second].Display(false);
			cout << "请牢记您需要购票的 航班号 ，并使用 客票预订 功能进行购票操作！" << endl;
			cout << _STAR_ << endl; _PAC_; return;
		}
	}
}

void Book_Ticket(User& user) {
	string op;
	while (true) {
		cout << _STAR_ << endl;
		Time nowtime; nowtime.Display();
		cout << _STAR_ << endl;
		cout << "\t  您已进入自助购票系统\n  本页面需录入 航班号 以完成购票操作！" << endl;
		cout << "1.若具体航班号未知，请先使用 查询航班 功能获取航班号！" << endl;
		cout << "2.输入 购票数量 时，若 输入数量 大于 余票数量 则可使用 候补功能。" << endl;
		cout << "3.输入-1以返回上一层菜单" << endl;
		cout << _STAR_ << endl;
		cout << "请输入您需要购票的航班号(如：CA1202)："; while (getline(cin, op), op.empty());
		if (op == "-1") { system("cls"); return; }
		else if (!allflights.count(op)) {
			cout << _STAR_ << endl;
			cout << "航班号 " << op << " 不存在，请检查输入是否正确！" << endl;
			cout << _STAR_ << endl; _PAC_; continue;
		}
		else {
			Flight& foundit = allflights[op];
			cout << "\n该航班尚有余票详情如下：" << endl;
			rap(i, 0, foundit.allrating) cout << "  " << i + 1 << "等舱：" << foundit.remaining[i].size() << "张" << endl;

			int _rating;
			while (true) {
				cout << "\n请输入您需要预订票的舱等级（如：1）：";
				while (getline(cin, op), op.empty());
				if (op == "-1") { system("cls"); return; }
				int cnt = stoi(op);
				if (cnt > 0 && cnt <= foundit.allrating) { _rating = cnt - 1; break; }
				else cout << "请输入一个 1 到 " << foundit.allrating << " 之间的数字！" << endl;
			}

			while (true) {
				cout << "请输入您需要预订" << _rating + 1 << "等舱票的数量（如：2）：";
				while (getline(cin, op), op.empty());
				if (op == "-1") { system("cls"); return; }
				int cnt = stoi(op);
				if (cnt >= 0) {
					if (cnt <= foundit.remaining[_rating].size()) {
						rap(i, 0, cnt) {
							Ticket ticket = foundit.Pop_Ticket(psi(user.account, _rating));
							user.Push_Ticket(ticket);
							cout << "\n出票成功, 客票信息如下：" << endl;
							ticket.Display();
						}
						cout << "正在为您预订..." << endl;
					}
					else {
						cout << "余票数量不足，是否需要为您登记 排队候补 购票？" << endl;
						cout << "1.需要 0.不需要" << endl;
						while (true) {
							cout << "请输入选项前的数字以继续操作：";
							while (getline(cin, op), op.empty());
							if (op == "-1") { system("cls"); return; }
							else if (op == "1") {
								cout << "您的购票需求：" << foundit.flightNO << " " << _rating << "等舱 " << cnt << "张" << endl;
								foundit.Alternate_Ticket(pii(_rating, cnt));
								cout << "正在为您登记排队..." << endl; break;
							}
							else if (op == "0") break;
						}
					}

					Save(0); Save(1);
					cout << _STAR_ << endl; cout << "\t操作成功！" << endl;
					cout << _STAR_ << endl; _PAC_; return;
				}
				else //cout << "请输入一个 0 到 " << foundit.remaining[_rating].size() << " 之间的数字！" << endl;
					cout << "请输入一个正整数（大于余票数量可排队候补）" << endl;
			}
		}
	}
}

void Handle_Refund(User& user) {
	string op;
	while (true) {
		cout << _STAR_ << endl;
		Time nowtime; nowtime.Display();
		cout << _STAR_ << endl;
		cout << "\t  您已进入自助退票系统\n  本页面需录入 已购票序号 以完成退票操作！" << endl;
		cout << "1.系统将输出您的 预购票 列表，输入对应 序号 即可完成退票操作！" << endl;
		cout << "2.输入-1以返回上一层菜单" << endl;
		cout << _STAR_ << endl;
		int size = user.preorder.size(), i = 1;
		if (!size) {
			cout << "暂时还没有您的购票记录，先去预订一张机票吧！" << endl;
			cout << _STAR_ << endl; _PAC_; return;
		}
		cout << "您的购票记录如下：" << endl;
		for (auto it : user.preorder) cout << "\n客票序号" << i << "：" << endl, it.Display(), ++i;

		while (true) {
			cout << "请输入您需要退票的课票序号(如：1)："; while (getline(cin, op), op.empty());
			if (op == "-1") { system("cls"); return; } int idx = stoi(op);
			if (idx > 0 && idx <= size) {
				Ticket ticket = user.Pop_Ticket(idx); //乘客退票
				allflights[ticket.flightNO].Push_Ticket(ticket); //客票回收
				allflights[ticket.flightNO].Handle_Alternate(ticket.rating); //处理候补
				Save(0); Save(1);
				cout << "退订成功！" << endl;
				cout << _STAR_ << endl; _PAC_; return;
			}
			else cout << "请输入一个 1 到 " << size << " 的数字！" << endl;
		}
	}
}

bool Save(int i) {
	if (i >= filecount || i < 0) return false;
	ofstream file(filename[i]);
	if (!file.is_open()) { //如果打开文件失败
		cout << " 数据文件 " << filename[i] << " 打开失败!" << endl;
		cout << "请检查: 是否有权限写入文件！" << endl;
		exit(-1); //终止程序，抛出异常值
	}
	if (!i) for (auto it : allusers) it.second.Write(file);
	else if (i == 1) for (auto it : allflights) it.second.Write(file);
	file.close(); file.clear();//关闭文件流
	return true;
}

void Load() {
	cout << "数据加载中..." << endl;
	ifstream file;
	rap(i, 0, filecount) {
		file.open(filename[i]);
		if (!file.is_open()) { //如果打开文件失败
			if (Save(i));
			else {
				cout << " 数据文件 " << filename[i] << " 打开失败!" << endl;
				cout << "请检查: 是否有权限读取文件！" << endl;
				exit(-1); //终止程序，抛出异常值
			}
		}

		if (!i) {
			while (file.peek() != EOF) {
				User user = user.Read(file);
				allusers[user.account] = user;
			}
		}
		else {
			while (file.peek() != EOF) {
				Flight flight = flight.Read(file);
				allflights[flight.flightNO] = flight;
			}
		}

		file.close(); file.clear();//关闭文件流
	}

	Initilaze(); //初始化
	cout << "数据加载完毕！" << endl; system("cls");
}

void Initilaze() {
	//直达航线
	nonstopflight.clear();
	for (auto it : allflights) nonstopflight.insert(make_pair(pss(it.second.start, it.second.end), it.first));
	//换乘航线
	graph.Clear();
	for (auto it : allflights) graph.AddEdge(it.second.start, it.second.end, it.second.flightNO);
}


void Text() {
	//Flight("");

}