#include <iostream> //C++ �����������
#include <fstream> //C++ �ļ������������
#include <cstdio> //C ��׼���������
#include <conio.h> //C ����̨��������� getch()�����Զ���
#include <cstdlib> //C ��׼������
#include <cstring> //C �ַ���������
#include <ctime> //C ϵͳʱ���
#include <string> //C++ string���
#include <vector> //C++ vector������
#include <map> //C++ mapӳ���
#include <queue> //C++ queue���п�
#include <stack> //C++ stackջ��
#include <set> //C++ set���Ͽ�
#include <list> //C++ list�����
#include <deque> //C++ deque˫�˶��п�
#include <algorithm> //C++ �����㷨��

//�궨�岿��
#define rap(i, a, b) for(int i = (a); i < (b); ++i)
#define rep(i, a, b) for(int i = (a); i >= (b); --i)
#define _PAC_ system("pause"), system("cls")

using namespace std;

//�������������ʹ��
typedef pair<int, int> pii;
typedef pair<string, string> pss;
typedef pair<string, int> psi;
typedef pair<int, string> pis;
typedef pair<pii, string> piis;

//������
struct Time; //ʱ����
struct Ticket; //��Ʊ��
struct User; //�û���
struct Flight; //������
struct Graph; //ͼ��

//������
const string _STAR_ = "****************************************";
const string _LINE_ = "----------------------------------------";
const string ChineseDay[] = { "��", "һ", "��", "��", "��", "��", "��" };
const int filecount = 2; //�ļ�����
const string filename[] = { "Users.data", "Flights.data" }; //�ļ�����

//ȫ�ֱ�����
map<string, User> allusers; //�û��б� �˺�->�˻�
map<string, Flight> allflights; //�����б� �����->����
multimap<pss, string> nonstopflight; //ֱ�ﺽ�� <��㣬�յ�> �����
//Graph graph; //ͼ ����·��������

//ʱ����
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
		cout << "��ǰϵͳʱ�䣺";
		cout << mtm->tm_year + 1900 << "��" << mtm->tm_mon + 1 << "��" << mtm->tm_mday << "�� ";
		printf("%02d:%02d:%02d", mtm->tm_hour, mtm->tm_min, mtm->tm_sec);
		cout << " ��" << ChineseDay[mtm->tm_wday] << endl;
	}
};

//�Զ���ͼ��
struct Graph {
private:
	map<string, int> nametoidx; //�ص����
	map<int, string> idxtoname; //��Ŵ���ĵص�
	vector<vector<pis> > graph; //ͼ ���ڽӱ�
	vector<pis> path; //����·���� ���׵ģ���ţ�����ţ�
	vector<bool> vis; //DFS�Ƿ��ѷ���
	int answer; //�������֮���Ƿ���·�߿ɴ�

	void Add(string); //��ӵص� ���ڽӱ���У�
	void OutPath(int); //���·��
	void DFS(int, int);

public:
	void Clear(); //���
	int GetIdx(string); //�õ��ص����
	string GetName(int); //�õ���ŵص�
	int GetSize(); //��ͬ�ص����
	int GetCount(string); //��ѯ�ص��Ƿ����
	void AddEdge(string, string, string); //��Ӻ��� ���ڽӱ���У�
	void Search_Menu(string, string); //�����˵�
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
	if (nametoidx.count(name)) return; //�������ü�
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
	cout << "\t  ���ѽ��� ���˲�ѯ ϵͳ" << endl;
	cout << "1.��ҳ��ɲ�ѯ��� ���˺��� ����ϸ��Ϣ��" << endl;
	cout << _STAR_ << endl;
	answer = 0;
	cout << _start << " �� " << _end << " �Ļ��˺������£�" << endl;
	if (GetCount(_start) && GetCount(_end)) {
		vis[GetIdx(_start)] = true;
		DFS(GetIdx(_start), GetIdx(_end));
		vis[GetIdx(_start)] = false;
	}
	if (!answer) cout << "\n��ϵͳ���������� " << _start << " �� " << _end << " �Ļ��˺���" << endl;
	else cout << "\n���μ�����Ҫ��Ʊ�� ����ţ���ʹ�� ��ƱԤ�� ���ܽ��й�Ʊ������" << endl;
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
	int s = x; //������s �յ����x
	vector<pis> vec; vec.clear();
	//���յ����x һֱ���ݵ� ������s ������·��
	while (path[s].first != -1) vec.push_back(path[s]), s = path[s].first;
	//���෴��˳�����·��
	cout << endl << ++answer << ". " << endl;
	cout << _LINE_ << endl;
	for (auto it = vec.rbegin(); it != vec.rend(); ++it) cout << GetName(it->first), cout << " =" << it->second << "=��";
	cout << GetName(x) << endl;//����յ����
	cout << _LINE_ << endl;
}
Graph graph; //ͼ ����·��������

//��Ʊ��
struct Ticket {
	string flightNO; //�����
	string start, end; //��ʼ�㣬�յ�
	//pss belong; //��Ʊ���� �˿��˺ţ��˿�����
	//pii seat; //��λ �յȼ�����λ��
	string account, name; //��Ʊ���� �˿��˺ţ��˿�����
	int rating, seatnumber; //��λ �յȼ�����λ��
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
	cout << "����ţ�" << flightNO << endl;
	cout << "��ʼ��-Ŀ�ĵأ�" << start << "->" << end << endl;
	cout << "(��Ʊ�˺�) �˿�������" << "(" << account << ") " << name << endl;
	cout << "[�յȼ�] ��λ�ţ�" << "[" << rating + 1 << "��] " << seatnumber << "����" << endl;
	cout << _LINE_ << endl;
}

//�ͻ���
struct User {
	string account, password; //�˺ţ�����
	string name; //����
	vector<Ticket> preorder; //Ԥ��Ʊ�б�
	map<string, pii> alternate; //�򲹼�¼ �����->�յȼ���������

	User();
	User(string, string, string);

	void Write(ofstream&); //д���ļ���
	User Read(ifstream&); //�ļ�������

	string Input_PassWord(); //¼�����벢��ʾ*��
	void Push_Ticket(Ticket); //ȡƱ
	Ticket Pop_Ticket(int); //��Ʊ
	void Display(bool); //ȡƱ
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
	preorder.push_back(ticket); //�û�Ԥ��Ʊѹ��
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
	cout << "�ͻ��˺ţ�" << account << endl;
	if (admin) cout << "�˺����룺" << password << endl;
	cout << "�ͻ�������" << name << endl;
	cout << "�ͻ���Ʊ�б�";
	if (preorder.empty()) cout << "�������ݣ�" << endl;
	else {
		cout << endl; int i = 1;
		for (auto it : preorder) cout << i++ << "." << endl, it.Display();
	}
	cout << "�ͻ�����Ϣ��";
	if (alternate.empty()) cout << "�������ݣ�" << endl;
	else {
		cout << endl; int i = 1;
		cout << "�����ʽ�����.������ţ�[�յȼ�] ������" << endl;
		for (auto it : alternate) cout << i++ << ".��" << it.first << "��[" << it.second.first + 1 << "�Ȳ�] " << it.second.second << "��" << endl;
	}
	cout << "+" << _LINE_ << "+" << endl;
}

//������
struct Flight {
	string start, end; //��ʼ�أ�Ŀ�ĵ�
	string flightNO, planemodel; //����ţ�����
	int workday; //��������
	const static int allrating = 3; //�յȼ�����
	int capacity[allrating + 1]; //�ն�Ӧ����
	//vector<Ticket> remaining[allrating]; //ÿ����λ����Ʊ
	queue<Ticket> remaining[allrating]; //ÿ����λ����Ʊ
	//vector<Ticket> preorder[allrating]; //Ԥ���ͻ�
	map<psi, int> preorder; //Ԥ���ͻ��� ���˺ţ��յȼ���->����
	//multimap<pii, string> alternate; //�򲹿ͻ� (�յȼ�������)->�˺�
	vector<piis> alternate;//�򲹿ͻ� (�յȼ�������)->�˺�

	Flight();
	void Write(ofstream&); //д���ļ�
	Flight Read(ifstream&); //�ļ�����

	void Full_Ticket(); //��Ʊ����
	void Push_Ticket(Ticket); //��Ʊ 
	Ticket Pop_Ticket(psi); //��Ʊ ���˿��˺ţ��յȼ���
	void Alternate_Ticket(pii); //���Ŷ� (�յȼ�������)
	void Handle_Alternate(int); //����� �յȼ�

	void Add_NonStopFlight(); //���ֱ�ﺽ��
	void Display(bool);	//��ʾ��Ϣ �Ƿ����
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
	--preorder[psi(ticket.account, ticket.rating)]; //�ͻ�Ԥ��Ʊ -1
	ticket.account.clear(); ticket.name.clear(); //��Ʊ�����ÿ�
	remaining[ticket.rating].push(ticket);
}
Ticket Flight::Pop_Ticket(psi pr) {
	Ticket ticket = remaining[pr.second].front();
	remaining[pr.second].pop(); //������Ʊ����
	preorder[pr]++; //Ԥ���б�
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
	const string nextday[] = { "����","����","����","3���","4���","5���","6���","һ�ܺ�" };
	cout << _LINE_ << endl;
	cout << "����ţ�" << flightNO << endl;
	if (admin) cout << "��ʼ��-Ŀ�ĵأ�" << start << "->" << end << endl;
	cout << "�ɻ��ͺţ�" << planemodel << endl;
	cout << "�������գ�" << "��" << ChineseDay[workday] << endl;
	cout << "�´κ��ࣺ";
	Time nowtime; int day = (workday - nowtime.GetWeekDay() + 7) % 7;
	cout << nextday[day] << " "; nowtime.Add(day * 24ll * 60 * 60); nowtime.YMD();
	cout << "�ú�����Ʊ�������£�" << endl;
	rap(i, 0, allrating) {
		cout << "  " << i + 1 << "�Ȳգ�" << remaining[i].size() << "��";
		if (admin) cout << " [���ؿ�����" << capacity[i] << "��]";
		cout << endl;
	}
	if (admin) {
		int i = 1;
		cout << "Ԥ���˿��б�(���� [�յȼ�] ��Ʊ����)��";
		if (preorder.empty()) cout << "�������ݣ�" << endl;
		else {
			cout << endl;
			for (auto it : preorder)
				cout << i++ << ". " << allusers[it.first.first].name << " [" << it.first.second + 1 << "�Ȳ�] " << it.second << "��" << endl;
		}
		i = 1;
		cout << "�򲹳˿��б�(���� [�յȼ�] ��������)��";
		if (alternate.empty())cout << "�������ݣ�" << endl;
		else {
			cout << endl;
			for (auto it : alternate)
				cout << i++ << ". " << allusers[it.second].name << " [" << it.first.first << "�Ȳ�] " << it.first.second << "��" << endl;
		}
	}
	cout << _LINE_ << endl;
}

void Main_Menu(); //���˵�
void User_Menu(User&); //�û��˵�

void Tourist_Mode(); //�ÿ�ģʽ
void User_Mode(); //�û�ģʽ
void Admin_Mode(); //����ģʽ
void Help_Mode(); //ʹ��˵��

void Add_Flight(); //��Ӻ���
void View_Infor(string); //�鿴��Ϣ

void Query_Flight(); //��ѯ����
void Book_Ticket(User&); //��ƱԤ��
void Handle_Refund(User&); //������Ʊ

void Load(); //�ļ�����
bool Save(int); //�ļ�д�� ������0���û� 1�溽��

void Initilaze(); //��ʼ��

int stoi(string&); //string ת ����ָ��op, �Ƿ�����ָ���-1

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
		cout << "��ӭʹ�� \"��ѻ���ɻ�\" ���տ��˶�Ʊϵͳ!" << endl;
		cout << endl << _STAR_ << endl;

		cout << "\t1.�ÿ�ģʽ" << endl;
		cout << "\t2.�û�ģʽ" << endl;
		cout << "\t3.����ģʽ" << endl;
		cout << "\t4.ʹ��˵��" << endl;
		cout << "\t0.�˳�ϵͳ" << endl;
		cout << _STAR_ << endl;

		cout << "������ѡ��ǰ�������Լ���������"; string op; while (getline(cin, op), op.empty());
		switch (stoi(op))
		{
		case 1: system("cls"); Tourist_Mode(); break;
		case 2: system("cls"); User_Mode(); break;
		case 3: system("cls"); Admin_Mode(); break;
		case 4: system("cls"); Help_Mode(); break;
		case 0: cout << "�˳�ϵͳ�ɹ���" << endl; cout << _STAR_ << endl; return;
		default:
			cout << "����ѡ������������������룡 OvO" << endl;
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
		if (tourist) cout << "\t������ʹ�÷ÿ�ģʽ\n���ֹ������ޣ���������ȫ�����ܣ���ʹ���û�ģʽ!" << endl;
		else cout << "�𾴵��û���" << user.name << ", ���ã�" << endl;
		cout << _STAR_ << endl;
		cout << "\t1.��ѯ����" << endl;
		cout << "\t2.��ƱԤ��" << endl;
		cout << "\t3.������Ʊ" << endl;
		cout << "\t0.�������˵�" << endl;
		cout << _STAR_ << endl;
		cout << "������ѡ��ǰ�������Լ���������"; string op; while (getline(cin, op), op.empty());
		switch (stoi(op))
		{
		case 1: system("cls"); Query_Flight(); break;
		case 2: system("cls");
			if (!tourist) Book_Ticket(user);
			else {
				cout << _STAR_ << endl;
				cout << "\n  �ÿ�ģʽ�£���ƱԤ��ϵͳ�����ã�\n" << endl;
				cout << _STAR_ << endl; _PAC_;
			}
			break;
		case 3: system("cls");
			if (!tourist) Handle_Refund(user);
			else {
				cout << _STAR_ << endl;
				cout << "\n  �ÿ�ģʽ�£�������Ʊҵ�񲻿��ã�\n" << endl;
				cout << _STAR_ << endl; _PAC_;
			}
			break;
		case 0: system("cls"); return;
		default:
			cout << "����ѡ������������������룡 OvO" << endl;
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
	cout << "\t  ������ʹ���û�ģʽ\n\t����ע��/��¼�����˺ţ�" << endl;
	cout << "���������˺ţ�ϵͳ���Զ�Ϊ��ע���˺ţ�" << endl;
	cout << _STAR_ << endl;
	cout << "�����������˺ţ�"; while (getline(cin, user.account), user.account.empty());
	if (user.account == "-1") { system("cls"); return; }
	bool foundit = false; if (allusers.count(user.account)) foundit = true;
	if (!foundit) cout << "�˺� " << user.account << " ��δע�ᣬϵͳ��Ϊ���Զ�ע���˺ţ�" << endl;
	cout << "�������������룺"; user.password = user.Input_PassWord();
	if (!foundit) {
		cout << "���ٴ������������룺"; string password2 = user.Input_PassWord();
		if (password2 != user.password) {
			cout << "\nע��ʧ�ܣ��������벻һ�£����ڷ�����һ��..." << endl;
			cout << _STAR_ << endl; _PAC_; return;
		}
		else {
			cout << "����������������"; while (getline(cin, user.name), user.name.empty());
			if (user.name == "-1") { system("cls"); return; }

			allusers[user.account] = user; Save(0);
			cout << "\nע��ɹ������μ������˺ź�����, ������ת..." << endl;
			cout << _STAR_ << endl; _PAC_;
			User_Menu(allusers[user.account]); return;
		}
	}
	else {
		if (user.password != allusers[user.account].password) {
			cout << "\n��¼ʧ�ܣ��������벻��ȷ�����ڷ�����һ��..." << endl;
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
		cout << "\t������ʹ�ù���ģʽ\n�ݽ�֧�����¼�����������๦������½������..." << endl;
		cout << _STAR_ << endl;
		cout << "\t1.��Ӻ���" << endl;
		cout << "\t2.�鿴������Ϣ" << endl;
		cout << "\t3.�鿴�˿���Ϣ" << endl;
		cout << "\t0.�������˵�" << endl;
		cout << _STAR_ << endl;
		cout << "������ѡ��ǰ�������Լ���������"; string op; while (getline(cin, op), op.empty());
		switch (stoi(op))
		{
		case 1: system("cls"); Add_Flight(); break;
		case 2: system("cls"); View_Infor("����"); break;
		case 3: system("cls"); View_Infor("�˿�"); break;
		case 0: system("cls"); return;
		default:
			cout << "����ѡ������������������룡 OvO" << endl;
			cout << _STAR_ << endl; _PAC_;
			break;
		}
	}
}

void Help_Mode() {
	cout << _STAR_ << _STAR_ << endl;
	cout << "ʹ�ñ�ϵͳǰ�����������Ķ���ҳ����ѧϰϵͳʹ�÷���! OvO" << endl;
	cout << "ϵͳ������ģʽ����ʹ��,������ʾ��" << endl;
	cout << "\n  �ÿ�ģʽ������ע���˺ż���ʹ�ã���" << endl;
	cout << "      ��֧�� ��ѯ���� ���ܣ����� ��Ʊ����Ʊ ��ʹ���û�ģʽ��" << endl;
	cout << "    1.��ѯ���ࣺ" << endl;
	cout << "        ���� ��ʼ�� �� Ŀ�ĵ� ���Ƽ��ɲ�ѯ��غ�����Ϣ��" << endl;
	cout << "        ϵͳ������ʾ ֱ�ﺽ�࣬ ����ֱ���ѡ���Ƿ����� ���˷���" << endl;
	cout << "        ���˷��񣺻�Ϊ����������� ���� �ɴ�Ļ���·�� ��" << endl;
	cout << "\n  �û�ģʽ����ע��/��¼�˺ź󷽿�ʹ�ã���" << endl;
	cout << "    1.��ѯ���ࣺ" << endl;
	cout << "        ���� ��ʼ�� �� Ŀ�ĵ� ���Ƽ��ɲ�ѯ��غ�����Ϣ��" << endl;
	cout << "    2.��ƱԤ����" << endl;
	cout << "        ���� ����� �� ��Ʊ���󣨲�λ�ȼ�����Ʊ������ ����Ԥ����Ӧ���ࡣ" << endl;
	cout << "        ����Ʊ���㣬����ʾ��Ӧ��λ�ţ� ����Ʊ���㣬��ѯ���Ƿ��ŶӺ򲹡�" << endl;
	cout << "    3.������Ʊ��" << endl;
	cout << "        ϵͳ��� �û�Ԥ����¼�������Ӧ ��� ����ȡ����Ӧ���ࡣ" << endl;
	cout << "\n  ����ģʽ�����ں��չ�˾�����̨����" << endl;
	cout << "      �ݽ�֧�����¼���ܣ���������½��������... �����ڴ���" << endl;
	cout << "    1.��Ӻ��ߣ�" << endl;
	cout << "        ����ϵͳ��ʾ���� ��Ӧ������Ϣ ����¼����غ�����Ϣ��" << endl;
	cout << "    2.�鿴������Ϣ��" << endl;
	cout << "        �������ڴ�����в鿴 ���к��� ��������Ϣ��" << endl;
	cout << "    3.�鿴�ͻ���Ϣ��" << endl;
	cout << "        �������ڴ�����в鿴 ���пͻ� ��������Ϣ��" << endl;
	cout << "\n  �������ɣ�һЩ����Ĳ���˵������" << endl;
	cout << "    1.�������ݣ�" << endl;
	cout << "        ����һЩ�ض������⣬��ȷ�������ÿһ���λ��һ�У�����Ӱ������������" << endl;
	cout << "    2.������һ�㣺" << endl;
	//cout << "        �����ϣ�������ʾ��ѡ��Ĳ˵�������������档���������κ����������� -1 ���ж����벢���ء�" << endl;
	cout << "        ���������£�������ͨ������ -1 ���ж����벢������һ����档" << endl;
	cout << endl << _STAR_ << _STAR_ << endl;
	cout << "(ע��ҳ�������ʾ�������������ϻ�! OvO )" << endl; _PAC_;
}

void Add_Flight() {
	Flight flight;
	while (true) {
		cout << _STAR_ << endl << endl;
		Time nowtime; nowtime.Display();
		cout << _STAR_ << endl;
		cout << "\t������ʹ�� ��Ӻ��� ����" << endl;
		cout << " 1.���������κ�һ������ʱ������ -1 �Է�����һ��˵���" << endl;
		cout << " 2.��ȷ��ÿһ����������ݾ�λ��ͬһ�У�" << endl;
		cout << endl << _STAR_ << endl;
		string op;

		cout << "�����뺽�ߵ������У��磺������" << endl; while (getline(cin, op), op.empty());
		if (op == "-1") { system("cls"); return; }
		else flight.start = op;

		cout << "�����뺽�ߵ��յ���У��磺������" << endl; while (getline(cin, op), op.empty());
		if (op == "-1") { system("cls"); return; }
		else flight.end = op;

		cout << "�����뺽�ߵĺ���ţ��磺CA1202��" << endl; while (getline(cin, op), op.empty());
		if (op == "-1") { system("cls"); return; }
		else if (!allflights.count(op)) flight.flightNO = op;
		else { cout << "�ú�����Ѵ��ڣ�������¼�뺽����Ϣ��" << endl; cout << _STAR_ << endl; _PAC_; continue; }

		cout << "�����뺽�ߵķɻ��ͺţ��磺BOEING737-800 WINGLETS��" << endl; while (getline(cin, op), op.empty());
		if (op == "-1") { system("cls"); return; }
		else flight.planemodel = op;

		while (true) {
			cout << "�����뺽�ߵ���Ӫ���գ�0~6��(�����գ�0)" << endl;
			while (getline(cin, op), op.empty());
			if (op == "-1") { system("cls"); return; }
			int cnt = stoi(op);
			if (cnt >= 0 && cnt < 7) { flight.workday = cnt; break; }
			else cout << "������һ�� 0 �� 6 ֮������֣�" << endl;
		}

		flight.capacity[flight.allrating] = 0;
		rap(i, 0, flight.allrating) {
			//while (getline(cin, op), op.empty());
			while (true) {
				cout << "������" << i + 1 << "�Ȳ� ��λ�������磺" << 10 * i + 10 << "��" << endl;
				while (getline(cin, op), op.empty());
				if (op == "-1") { system("cls"); return; }
				int cnt = stoi(op);
				if (cnt >= 0 && cnt < INT_MAX) { flight.capacity[i] = cnt; break; }
				else cout << "������һ�� 0 �� " << INT_MAX << " ֮������֣�" << endl;
				//	cout << "������ " << i + 1 << "�Ȳ� ��λ�������磺" << 10 * i + 10 << "��" << endl;
			}
			flight.capacity[flight.allrating] += flight.capacity[i];
		}

		flight.Full_Ticket(); //������Ʊ
		flight.Add_NonStopFlight(); //���ֱ�ﺽ��
		allflights[flight.flightNO] = flight; Save(1);

		system("cls"); cout << _STAR_ << endl;
		cout << "\n���� " << flight.flightNO << " ����ӳɹ���" << endl;
		cout << "�Ƿ������ӣ� 1.�� 0.��" << endl;
		cout << _STAR_ << endl;
		while (true) {
			cout << "������ѡ��ǰ�������Լ���������";
			while (getline(cin, op), op.empty());
			if (op == "1") { system("cls"); break; }
			else if (op == "0") { system("cls"); return; }
			else cout << "����ѡ���������������룡" << endl;
		}
	}
}

void View_Infor(string it) {
	cout << _STAR_ << endl << endl;
	Time nowtime; nowtime.Display();
	cout << _STAR_ << endl;
	cout << "\t������ʹ�� �鿴" << it << "��Ϣ ����" << endl;
	cout << " 1.���ǽ�Ϊ����ʾ ����" << it << "����Ϣ��" << endl;
	cout << endl << _STAR_ << endl;
	if (it == "����") for (auto it : allflights) it.second.Display(true);
	else for (auto it : allusers) it.second.Display(true);
	cout << _STAR_ << endl; _PAC_;
}

void Query_Flight() {
	string op;
	while (true) {
		cout << _STAR_ << endl;
		Time nowtime; nowtime.Display();
		cout << _STAR_ << endl;
		cout << "\t  ���ѽ��� ������ѯ ϵͳ\n  ��ҳ��ɲ�ѯ���� ����� ����Ϣ��" << endl;
		cout << "1.���� ��ʼ�ء�Ŀ�ĵ� ���ɲ�ѯ���ֱ�ﺽ����Ϣ��" << endl;
		cout << "2.���� ֱ�ﺽ�� ��Ϣ������ѡ������ ���˷���" << endl;
		cout << "3.����-1�Է�����һ��˵�" << endl;
		cout << _STAR_ << endl;

		string _start, _end;
		cout << "������������ʼ�ص�(�磺����)��"; while (getline(cin, _start), _start.empty());
		if (_start == "-1") { system("cls"); return; }
		cout << "����������Ŀ�ĵص�(�磺����)��"; while (getline(cin, _end), _end.empty());
		if (_end == "-1") { system("cls"); return; }

		auto left = nonstopflight.lower_bound(pss(_start, _end));
		auto right = nonstopflight.upper_bound(pss(_start, _end));
		if (left == right) {
			cout << _STAR_ << endl;
			cout << " " << _start << " �� " << _end << " ֮������ֱ�ﺽ�ߣ�" << endl;
			cout << "\nϵͳ�ṩ ���˲�ѯ �����Ƿ���ҪΪ�������÷��� 1.��Ҫ 0.����Ҫ" << endl;
			cout << _STAR_ << endl;
			while (true) {
				cout << "������ѡ��ǰ�������Լ���������";
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
			cout << "\nΪ����ѯ������ ֱ�ﺽ�� ��Ϣ��" << endl;
			for (auto it = left; it != right; ++it) allflights[it->second].Display(false);
			cout << "���μ�����Ҫ��Ʊ�� ����� ����ʹ�� ��ƱԤ�� ���ܽ��й�Ʊ������" << endl;
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
		cout << "\t  ���ѽ���������Ʊϵͳ\n  ��ҳ����¼�� ����� ����ɹ�Ʊ������" << endl;
		cout << "1.�����庽���δ֪������ʹ�� ��ѯ���� ���ܻ�ȡ����ţ�" << endl;
		cout << "2.���� ��Ʊ���� ʱ���� �������� ���� ��Ʊ���� ���ʹ�� �򲹹��ܡ�" << endl;
		cout << "3.����-1�Է�����һ��˵�" << endl;
		cout << _STAR_ << endl;
		cout << "����������Ҫ��Ʊ�ĺ����(�磺CA1202)��"; while (getline(cin, op), op.empty());
		if (op == "-1") { system("cls"); return; }
		else if (!allflights.count(op)) {
			cout << _STAR_ << endl;
			cout << "����� " << op << " �����ڣ����������Ƿ���ȷ��" << endl;
			cout << _STAR_ << endl; _PAC_; continue;
		}
		else {
			Flight& foundit = allflights[op];
			cout << "\n�ú���������Ʊ�������£�" << endl;
			rap(i, 0, foundit.allrating) cout << "  " << i + 1 << "�Ȳգ�" << foundit.remaining[i].size() << "��" << endl;

			int _rating;
			while (true) {
				cout << "\n����������ҪԤ��Ʊ�Ĳյȼ����磺1����";
				while (getline(cin, op), op.empty());
				if (op == "-1") { system("cls"); return; }
				int cnt = stoi(op);
				if (cnt > 0 && cnt <= foundit.allrating) { _rating = cnt - 1; break; }
				else cout << "������һ�� 1 �� " << foundit.allrating << " ֮������֣�" << endl;
			}

			while (true) {
				cout << "����������ҪԤ��" << _rating + 1 << "�Ȳ�Ʊ���������磺2����";
				while (getline(cin, op), op.empty());
				if (op == "-1") { system("cls"); return; }
				int cnt = stoi(op);
				if (cnt >= 0) {
					if (cnt <= foundit.remaining[_rating].size()) {
						rap(i, 0, cnt) {
							Ticket ticket = foundit.Pop_Ticket(psi(user.account, _rating));
							user.Push_Ticket(ticket);
							cout << "\n��Ʊ�ɹ�, ��Ʊ��Ϣ���£�" << endl;
							ticket.Display();
						}
						cout << "����Ϊ��Ԥ��..." << endl;
					}
					else {
						cout << "��Ʊ�������㣬�Ƿ���ҪΪ���Ǽ� �ŶӺ� ��Ʊ��" << endl;
						cout << "1.��Ҫ 0.����Ҫ" << endl;
						while (true) {
							cout << "������ѡ��ǰ�������Լ���������";
							while (getline(cin, op), op.empty());
							if (op == "-1") { system("cls"); return; }
							else if (op == "1") {
								cout << "���Ĺ�Ʊ����" << foundit.flightNO << " " << _rating << "�Ȳ� " << cnt << "��" << endl;
								foundit.Alternate_Ticket(pii(_rating, cnt));
								cout << "����Ϊ���Ǽ��Ŷ�..." << endl; break;
							}
							else if (op == "0") break;
						}
					}

					Save(0); Save(1);
					cout << _STAR_ << endl; cout << "\t�����ɹ���" << endl;
					cout << _STAR_ << endl; _PAC_; return;
				}
				else //cout << "������һ�� 0 �� " << foundit.remaining[_rating].size() << " ֮������֣�" << endl;
					cout << "������һ����������������Ʊ�������ŶӺ򲹣�" << endl;
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
		cout << "\t  ���ѽ���������Ʊϵͳ\n  ��ҳ����¼�� �ѹ�Ʊ��� �������Ʊ������" << endl;
		cout << "1.ϵͳ��������� Ԥ��Ʊ �б������Ӧ ��� ���������Ʊ������" << endl;
		cout << "2.����-1�Է�����һ��˵�" << endl;
		cout << _STAR_ << endl;
		int size = user.preorder.size(), i = 1;
		if (!size) {
			cout << "��ʱ��û�����Ĺ�Ʊ��¼����ȥԤ��һ�Ż�Ʊ�ɣ�" << endl;
			cout << _STAR_ << endl; _PAC_; return;
		}
		cout << "���Ĺ�Ʊ��¼���£�" << endl;
		for (auto it : user.preorder) cout << "\n��Ʊ���" << i << "��" << endl, it.Display(), ++i;

		while (true) {
			cout << "����������Ҫ��Ʊ�Ŀ�Ʊ���(�磺1)��"; while (getline(cin, op), op.empty());
			if (op == "-1") { system("cls"); return; } int idx = stoi(op);
			if (idx > 0 && idx <= size) {
				Ticket ticket = user.Pop_Ticket(idx); //�˿���Ʊ
				allflights[ticket.flightNO].Push_Ticket(ticket); //��Ʊ����
				allflights[ticket.flightNO].Handle_Alternate(ticket.rating); //�����
				Save(0); Save(1);
				cout << "�˶��ɹ���" << endl;
				cout << _STAR_ << endl; _PAC_; return;
			}
			else cout << "������һ�� 1 �� " << size << " �����֣�" << endl;
		}
	}
}

bool Save(int i) {
	if (i >= filecount || i < 0) return false;
	ofstream file(filename[i]);
	if (!file.is_open()) { //������ļ�ʧ��
		cout << " �����ļ� " << filename[i] << " ��ʧ��!" << endl;
		cout << "����: �Ƿ���Ȩ��д���ļ���" << endl;
		exit(-1); //��ֹ�����׳��쳣ֵ
	}
	if (!i) for (auto it : allusers) it.second.Write(file);
	else if (i == 1) for (auto it : allflights) it.second.Write(file);
	file.close(); file.clear();//�ر��ļ���
	return true;
}

void Load() {
	cout << "���ݼ�����..." << endl;
	ifstream file;
	rap(i, 0, filecount) {
		file.open(filename[i]);
		if (!file.is_open()) { //������ļ�ʧ��
			if (Save(i));
			else {
				cout << " �����ļ� " << filename[i] << " ��ʧ��!" << endl;
				cout << "����: �Ƿ���Ȩ�޶�ȡ�ļ���" << endl;
				exit(-1); //��ֹ�����׳��쳣ֵ
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

		file.close(); file.clear();//�ر��ļ���
	}

	Initilaze(); //��ʼ��
	cout << "���ݼ�����ϣ�" << endl; system("cls");
}

void Initilaze() {
	//ֱ�ﺽ��
	nonstopflight.clear();
	for (auto it : allflights) nonstopflight.insert(make_pair(pss(it.second.start, it.second.end), it.first));
	//���˺���
	graph.Clear();
	for (auto it : allflights) graph.AddEdge(it.second.start, it.second.end, it.second.flightNO);
}


void Text() {
	//Flight("");

}