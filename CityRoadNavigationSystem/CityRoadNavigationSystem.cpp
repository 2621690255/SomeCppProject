#include <iostream> //C++ �����������
#include <cstdio> //C ��׼���������
#include <cstdlib> //C ��׼������
#include <cstring> //C �ַ���������
#include <string> //C++ string���
#include <queue> //C++ queue���п�
#include <vector> //C++vector������
#include <map> //C++ mapӳ���
#include <algorithm> //C++ �����㷨��

//�궨�岿��
#define rap(i, a, b) for(int i = (a); i < (b); ++i)
#define rep(i, a, b) for(int i = (a); i >= (b); --i)
#define _STAR_ "****************************************"

using namespace std;

//������
const int inf = 0x3f3f3f3f; //����������
const double eps = 1e-8; //��������� 0

typedef pair<int, int> pii; //������pair�ͱ���

//���ȶ����õĽṹ��
struct node {
	int CityID, RoadID; //������еı�� ·�����
	double first, second; //��һ�ؼ��� �ڶ��ؼ���(·�̻�ʱ��)
	node() {}
	node(int r, int c, double f, double s) {
		RoadID = r; CityID = c; first = f; second = s;
	}
	bool operator<(const node& other)const {
		if (fabs(other.first - first) < eps) return other.second < second; //����������ж�
		return other.first < first; //ȨֵС�ĳ����ڶѶ�
	}
};

//��ߵĽṹ��
struct Edge_Node {
	string  Start, End; //��ʼ�� Ŀ�ĵ�
	double distance, speed, time; //���� �ٶ� ��ʱ
	static int road_cnt; //��·����
	int ID; //��·���
	Edge_Node() {}
	Edge_Node(string s, string e, double d, double sp) {
		Start = s; End = e; distance = d; speed = sp;
		time = d / sp; //�����ʱ ����/�ٶ�
		ID = ++road_cnt; //��·�Զ���Ų�+1
	}
};

//ȫ�ֱ�����
int Edge_Node::road_cnt = 0; //�ܵ�·����
vector<vector<node> > graph; //��ͼ
vector<Edge_Node> V; //�߼���
vector<pii> vec; //���·��ʱ������·����
map<string, int> city_id; //��������Ӧ���б��
map<int, string> id_city; //���б�Ŷ�Ӧ������
vector<double> cost[2]; //��һ�����ؼ���Ȩֵ�� cost[0][i]���ʾ �������i����ĵ�һ�ؼ��ֵĻ���
vector<pii> path; //���浽��ÿ�����е�·�� pair<int, int> (��һ�����б�ţ�ѡ���·�����)
priority_queue<node> Q; //���ȶ��� dijkstra�㷨���Ż�
int city_cnt = 0; //�ܳ�������

//�˵�������
void CreatMap_Menu(); //����·��
void LookMap_Menu(); //�鿴��ͼ
void Navigation_Menu(); //����ϵͳ
void Helper_Menu(); //ʹ��˵��

//������
void Add_City(string); //��������
void dijkstra(int); //���ĵ�Ѱ·�㷨
void OutPath(int); //���·��

int main() {
	system("color 30"); //����������ɫ
	Helper_Menu();
	while (true) {
		cout << _STAR_ << endl;
		cout << "��ӭʹ�ñ�����ϵͳ���г��е�·��ѯ!" << endl;
		cout << "������ѡ��ǰ�������Լ���������" << endl;
		cout << "\t1.������ͼ" << endl;
		cout << "\t2.�鿴��ͼ" << endl;
		cout << "\t3.��ʼ����" << endl;
		cout << "\t4.�����ֲ�" << endl;
		cout << "\t0.�˳�ϵͳ" << endl;
		cout << _STAR_ << endl;
		int op; cin >> op;
		switch (op)
		{
			//������ͼ
		case 1: system("cls"); CreatMap_Menu();  break;
			//�鿴��ͼ
		case 2: system("cls"); LookMap_Menu(); break;
			//��ʼ����
		case 3: system("cls"); Navigation_Menu(); break;
			//�����ֲ�
		case 4: system("cls"); Helper_Menu(); break;
			//�˳�ϵͳ
		case 0: cout << "�˳�ϵͳ�ɹ���" << endl; return 0;
		default:
			cout << "����ѡ������������������룡 OvO" << endl; cout << _STAR_ << endl;
			system("pause"); system("cls");
			break;
		}
	}
	return 0;
}

//����·��
void CreatMap_Menu() {
	string u, v; double distance, speed;
	cout << _STAR_ << endl;
	cout << "ʾ����ͼ�����ʽ��" << endl;
	cout << "֣�� �Ϻ� 1 0.05" << endl;
	cout << "֣�� ���� 2 0.1" << endl;
	cout << "�Ϻ� ���� 2 0.066666666666" << endl;
	cout << "֣�� ���� 4 0.4" << endl;
	cout << "���� ���� 1 0.05" << endl;
	cout << "-1" << endl;
	cout << _STAR_ << endl;
	cout << "����������һ��·�ߵ� ��ʼ�����ơ�Ŀ�ĵ����ơ�·��(ǧ�� km)��ƽ����ʻ�ٶ�(ǧ��/ʱ km/h)" << endl;
	cout << "(ע�� ��������-1�˳�������ͼ)" << endl;
	while (cin >> u) {
		if (u == "-1") break;
		cin >> v >> distance >> speed;
		V.push_back(Edge_Node(u, v, distance, speed)); //�ӱ�
		//ֻ�г�������һ�γ���ʱ������Ҫ��������
		if (!city_id.count(u)) Add_City(u);
		if (!city_id.count(v)) Add_City(v);
	}
	cout << _STAR_ << endl;
	cout << "��ͼ�����ɹ�! ��ȥ�����������ܰ� OvO" << endl;
	system("pause"); system("cls");
}

//�鿴��ͼ
void LookMap_Menu() {
	cout << _STAR_ << endl;
	rep(i, city_id.size() - 1, 0) graph[i].clear(); //��ʼ��ͼ
	//�����߼��ϣ���ÿ���߹�����һ���ڽӱ�
	for (auto it : V) graph[city_id[it.Start]].push_back(node(it.ID, city_id[it.End], it.distance, it.time));
	cout << "���������ĵ�ͼ���� ����" << city_id.size() << "��, ��·" << V.size() << "��" << endl;
	if (city_id.size()) cout << "�������£�\n" << endl;
	for (auto it : city_id) {
		cout << " ����\"" << it.first << "\"���е�· " << graph[it.second].size() << "����" << endl;
		for (auto jt : graph[it.second])
			cout << "   ��·" << jt.RoadID << ": " << "Ŀ�ĵ� " << id_city[jt.CityID] << " ����" << jt.first << "ǧ�� ��ʱ" << jt.second << "Сʱ" << endl;
		cout << endl;
	}
	cout << _STAR_ << endl;
	cout << "��ѯ���! �Ͳ��û����˰� OvO" << endl;
	system("pause"); system("cls");
}

//����ϵͳ
void Navigation_Menu() {
	string a, b;
	cout << _STAR_ << endl;
	cout << "��ʹ��ʾ����ͼ�����������������һ�У�" << endl;
	cout << "֣�� ����" << endl;
	cout << "֣�� ����" << endl;
	cout << "�Ϻ� ����" << endl;
	cout << _STAR_ << endl;
	cout << "��������Ҫ������ ��ʼ�����ƣ�Ŀ�ĵ�����" << endl;
	cin >> a >> b; //cout << a << " " << b << endl;
	int FindCityA = city_id.count(a), FindCityB = city_id.count(b);
	//�ж��������Ƿ����
	if (FindCityA && FindCityB) {
		cout << _STAR_ << endl;
		cout << "����������Ҫ�ĵ�����ʽ��" << endl;
		cout << "0�����·��\t1����̺�ʱ" << endl;
		int op; cin >> op;
		rep(i, city_id.size() - 1, 0) graph[i].clear(); //��ʼ��ͼ
		//����ָ�ͬ��ѡ��·����ʱ������һ�ؼ��� ������Ӧ�ڽӱ�
		if (!op) for (auto it : V) graph[city_id[it.Start]].push_back(node(it.ID, city_id[it.End], it.distance, it.time));
		else  for (auto it : V) graph[city_id[it.Start]].push_back(node(it.ID, city_id[it.End], it.time, it.distance));
		cout << _STAR_ << endl;
		dijkstra(city_id[a]); //Ѱ·
		OutPath(city_id[b]); //���·��
		cout << "��·��Ԥ����ʻ��" << cost[op][city_id[b]] << " ǧ��(km)" << endl;
		cout << "��·��Ԥ�ƺ�ʱ��" << cost[!op][city_id[b]] << " Сʱ(h)" << endl;
		cout << _STAR_ << endl;
		cout << "ʲô���㲻�����ң� Ҫ����ȥ�ֹ���һ��� OvO" << endl;
	}
	else {
		cout << "����";
		if (!FindCityA) cout << "\"" << a << "\"";
		if (!FindCityB) cout << "\"" << b << "\"";
		cout << "�����ڵ�ͼ��Ŷ��ȥ���˵���������·���� OvO" << endl;
		cout << _STAR_ << endl;
	}
	system("pause"); system("cls");
}

//ʹ��˵��
void Helper_Menu() {
	cout << _STAR_ << _STAR_ << endl;
	cout << " ʹ��ϵͳǰ��������ϸ�Ķ���ʹ���ֲ� OvO" << endl;
	cout << "  һ.������ͼ��أ�" << endl;
	cout << "\t1.�����˵����棬��������1���ɴ�����ͼ������ʽ���뼴�����·�ߡ�" << endl;
	cout << "\t2.·�̺��ٶȾ������0�����ʶ��(ϵͳ�ڲ�Ѱ·����dijkstra�㷨����֧�ָ�Ȩ)��" << endl;
	cout << "\t3.����;���˳�ϵͳ������£��������ٴ�����1���������·�ߣ�ͼ�����������������" << endl;
	cout << "  ��.������" << endl;
	cout << "\t1.���ϸ񰴸�ʽ���룬�Ƿ�������ܻ����һЩĪ����������⣨�׳�BUG�� Orz" << endl;
	cout << _STAR_ << _STAR_ << endl;
	system("pause"); system("cls");
}

//��������
void Add_City(string u) {
	city_id[u] = city_cnt; id_city[city_cnt] = u; //�������������Ӧ
	path.push_back(pii(-1, -1)); //��ʼ��·�� �������ĸ��ڵ�
	cost[0].push_back(inf); cost[1].push_back(inf); //��ʼ��Ȩֵ���� ��Ϊ������
	vector<node> vec; graph.push_back(vec); //Ϊͼ���ڽӱ�����һ��
	++city_cnt; //��������+1
}

//���ĵ�Ѱ·�㷨
void dijkstra(int s) {
	//��ʼ�� ����s ���� ����j �ĵ�һ�����ؼ��ֻ���
	rap(i, 0, 2) rep(j, cost[i].size() - 1, 0) cost[i][j] = inf;
	while (!Q.empty()) Q.pop(); //�������
	cost[0][s] = cost[1][s] = 0; path[s] = pii(-1, -1); //����ʼ��
	Q.push(node(0, s, 0, 0)); //���������
	//dijkstra�㷨 + ���Ż�
	while (!Q.empty()) {
		//��ȡ�ؼ�����С�Ľ��
		node tmp = Q.top(); Q.pop();
		//����������У���ţ�tmp.CityID ���ڽӱ� 
		for (auto it : graph[tmp.CityID]) {
			//��һ�ؼ��ָ��� ֱ�Ӹ���
			//cost[0][it.CityID] ��ǰ������У���ţ� it.CityID �ĵ�һ�ؼ��ֻ���
			//tmp.first Ŀǰ������У���ţ�tmp.CityID �ĵ�һ�ؼ���Ȩֵ����
			//it.first ��һ��������У���ţ�it.CityID �ĵ�һ�ؼ���Ȩֵ����
			if (cost[0][it.CityID] > tmp.first + it.first) {
				cost[0][it.CityID] = tmp.first + it.first; //���µ�һ�ؼ���
				cost[1][it.CityID] = tmp.second + it.second; //���µڶ��ؼ���
				path[it.CityID] = pii(tmp.CityID, it.RoadID); //����·��
				Q.push(node(it.RoadID, it.CityID, cost[0][it.CityID], cost[1][it.CityID])); //���������ѹ�����
			}
			else if (cost[0][it.CityID] < tmp.first + it.first);
			//��һ�ؼ������ ���ڶ��ؼ��ָ���
			else {
				if (cost[1][it.CityID] > tmp.second + it.second) {
					cost[1][it.CityID] = tmp.second + it.second; //���µڶ��ؼ���
					path[it.CityID] = pii(tmp.CityID, it.RoadID); //����·��
					Q.push(node(it.RoadID, it.CityID, cost[0][it.CityID], cost[1][it.CityID])); //���������ѹ�����
				}
			}
		}
	}
}

//���·��
void OutPath(int x) {
	int s = x; //������s �յ����x
	cout << "�����ɹ�������·��Ϊ��\n" << endl;
	vec.clear();
	//���յ����x һֱ���ݵ� ������s ������·��
	while (path[s].first != -1) vec.push_back(path[s]), s = path[s].first;
	//���෴��˳�����·��
	for (auto it = vec.rbegin(); it != vec.rend(); ++it) cout << id_city[it->first], cout << " =��·" << it->second << "=��";
	cout << id_city[x] << endl << endl; //����յ����
}