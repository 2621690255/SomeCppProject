#include <iostream> //C++ 流输入输出库
#include <cstdio> //C 标准输入输出库
#include <cstdlib> //C 标准函数库
#include <cstring> //C 字符串函数库
#include <string> //C++ string类库
#include <queue> //C++ queue队列库
#include <vector> //C++vector向量库
#include <map> //C++ map映射库
#include <algorithm> //C++ 常用算法库

//宏定义部分
#define rap(i, a, b) for(int i = (a); i < (b); ++i)
#define rep(i, a, b) for(int i = (a); i >= (b); --i)
#define _STAR_ "****************************************"

using namespace std;

//常量区
const int inf = 0x3f3f3f3f; //用作正无穷
const double eps = 1e-8; //浮点数误差 0

typedef pair<int, int> pii; //重命名pair型变量

//优先队列用的结构体
struct node {
	int CityID, RoadID; //到达城市的编号 路径编号
	double first, second; //第一关键字 第二关键字(路程或时间)
	node() {}
	node(int r, int c, double f, double s) {
		RoadID = r; CityID = c; first = f; second = s;
	}
	bool operator<(const node& other)const {
		if (fabs(other.first - first) < eps) return other.second < second; //浮点数相等判断
		return other.first < first; //权值小的出现在堆顶
	}
};

//存边的结构体
struct Edge_Node {
	string  Start, End; //起始地 目的地
	double distance, speed, time; //距离 速度 耗时
	static int road_cnt; //道路总数
	int ID; //道路编号
	Edge_Node() {}
	Edge_Node(string s, string e, double d, double sp) {
		Start = s; End = e; distance = d; speed = sp;
		time = d / sp; //计算耗时 距离/速度
		ID = ++road_cnt; //道路自动编号并+1
	}
};

//全局变量区
int Edge_Node::road_cnt = 0; //总道路数量
vector<vector<node> > graph; //地图
vector<Edge_Node> V; //边集合
vector<pii> vec; //输出路径时，保留路径用
map<string, int> city_id; //城市名对应城市编号
map<int, string> id_city; //城市编号对应城市名
vector<double> cost[2]; //第一、二关键字权值和 cost[0][i]则表示 到达城市i所需的第一关键字的花费
vector<pii> path; //保存到达每个城市的路径 pair<int, int> (上一个城市编号，选择的路径编号)
priority_queue<node> Q; //优先队列 dijkstra算法堆优化
int city_cnt = 0; //总城市数量

//菜单函数区
void CreatMap_Menu(); //新增路线
void LookMap_Menu(); //查看地图
void Navigation_Menu(); //导航系统
void Helper_Menu(); //使用说明

//功能区
void Add_City(string); //新增城市
void dijkstra(int); //核心的寻路算法
void OutPath(int); //输出路径

int main() {
	system("color 30"); //换个背景颜色
	Helper_Menu();
	while (true) {
		cout << _STAR_ << endl;
		cout << "欢迎使用本导航系统进行城市道路咨询!" << endl;
		cout << "请输入选项前的数字以继续操作：" << endl;
		cout << "\t1.创建地图" << endl;
		cout << "\t2.查看地图" << endl;
		cout << "\t3.开始导航" << endl;
		cout << "\t4.帮助手册" << endl;
		cout << "\t0.退出系统" << endl;
		cout << _STAR_ << endl;
		int op; cin >> op;
		switch (op)
		{
			//创建地图
		case 1: system("cls"); CreatMap_Menu();  break;
			//查看地图
		case 2: system("cls"); LookMap_Menu(); break;
			//开始导航
		case 3: system("cls"); Navigation_Menu(); break;
			//帮助手册
		case 4: system("cls"); Helper_Menu(); break;
			//退出系统
		case 0: cout << "退出系统成功！" << endl; return 0;
		default:
			cout << "输入选项有误，请检查后重新输入！ OvO" << endl; cout << _STAR_ << endl;
			system("pause"); system("cls");
			break;
		}
	}
	return 0;
}

//新增路线
void CreatMap_Menu() {
	string u, v; double distance, speed;
	cout << _STAR_ << endl;
	cout << "示例地图输入格式：" << endl;
	cout << "郑州 上海 1 0.05" << endl;
	cout << "郑州 北京 2 0.1" << endl;
	cout << "上海 株洲 2 0.066666666666" << endl;
	cout << "郑州 株洲 4 0.4" << endl;
	cout << "北京 株洲 1 0.05" << endl;
	cout << "-1" << endl;
	cout << _STAR_ << endl;
	cout << "请依次输入一条路线的 起始地名称、目的地名称、路程(千米 km)、平均行驶速度(千米/时 km/h)" << endl;
	cout << "(注： 单行输入-1退出创建地图)" << endl;
	while (cin >> u) {
		if (u == "-1") break;
		cin >> v >> distance >> speed;
		V.push_back(Edge_Node(u, v, distance, speed)); //加边
		//只有城市名第一次出现时，才需要新增城市
		if (!city_id.count(u)) Add_City(u);
		if (!city_id.count(v)) Add_City(v);
	}
	cout << _STAR_ << endl;
	cout << "地图创建成功! 快去试试其他功能吧 OvO" << endl;
	system("pause"); system("cls");
}

//查看地图
void LookMap_Menu() {
	cout << _STAR_ << endl;
	rep(i, city_id.size() - 1, 0) graph[i].clear(); //初始化图
	//遍历边集合，将每条边构建出一个邻接表
	for (auto it : V) graph[city_id[it.Start]].push_back(node(it.ID, city_id[it.End], it.distance, it.time));
	cout << "您所创建的地图共有 城市" << city_id.size() << "座, 道路" << V.size() << "条" << endl;
	if (city_id.size()) cout << "详情如下：\n" << endl;
	for (auto it : city_id) {
		cout << " 城市\"" << it.first << "\"共有道路 " << graph[it.second].size() << "条：" << endl;
		for (auto jt : graph[it.second])
			cout << "   道路" << jt.RoadID << ": " << "目的地 " << id_city[jt.CityID] << " 距离" << jt.first << "千米 耗时" << jt.second << "小时" << endl;
		cout << endl;
	}
	cout << _STAR_ << endl;
	cout << "查询完毕! 就差导航没玩过了吧 OvO" << endl;
	system("pause"); system("cls");
}

//导航系统
void Navigation_Menu() {
	string a, b;
	cout << _STAR_ << endl;
	cout << "若使用示例地图，则可输入下列任意一行：" << endl;
	cout << "郑州 株洲" << endl;
	cout << "郑州 北京" << endl;
	cout << "上海 株洲" << endl;
	cout << _STAR_ << endl;
	cout << "请输入需要导航的 起始地名称，目的地名称" << endl;
	cin >> a >> b; //cout << a << " " << b << endl;
	int FindCityA = city_id.count(a), FindCityB = city_id.count(b);
	//判断两城市是否存在
	if (FindCityA && FindCityB) {
		cout << _STAR_ << endl;
		cout << "请输入您需要的导航方式：" << endl;
		cout << "0、最短路程\t1、最短耗时" << endl;
		int op; cin >> op;
		rep(i, city_id.size() - 1, 0) graph[i].clear(); //初始化图
		//根据指令不同，选择路径或时间做第一关键字 构建对应邻接表
		if (!op) for (auto it : V) graph[city_id[it.Start]].push_back(node(it.ID, city_id[it.End], it.distance, it.time));
		else  for (auto it : V) graph[city_id[it.Start]].push_back(node(it.ID, city_id[it.End], it.time, it.distance));
		cout << _STAR_ << endl;
		dijkstra(city_id[a]); //寻路
		OutPath(city_id[b]); //输出路径
		cout << "该路径预计行驶：" << cost[op][city_id[b]] << " 千米(km)" << endl;
		cout << "该路径预计耗时：" << cost[!op][city_id[b]] << " 小时(h)" << endl;
		cout << _STAR_ << endl;
		cout << "什么，你不相信我？ 要不你去手工算一遍吧 OvO" << endl;
	}
	else {
		cout << "城市";
		if (!FindCityA) cout << "\"" << a << "\"";
		if (!FindCityB) cout << "\"" << b << "\"";
		cout << "还不在地图里哦，去主菜单新增这条路径吧 OvO" << endl;
		cout << _STAR_ << endl;
	}
	system("pause"); system("cls");
}

//使用说明
void Helper_Menu() {
	cout << _STAR_ << _STAR_ << endl;
	cout << " 使用系统前，请先仔细阅读本使用手册 OvO" << endl;
	cout << "  一.创建地图相关：" << endl;
	cout << "\t1.在主菜单界面，输入数字1即可创建地图，按格式输入即可添加路线。" << endl;
	cout << "\t2.路程和速度均须大于0，生活常识！(系统内部寻路采用dijkstra算法，不支持负权)。" << endl;
	cout << "\t3.在中途不退出系统的情况下，您可以再次输入1来继续添加路线（图保留到程序结束）。" << endl;
	cout << "  二.其他：" << endl;
	cout << "\t1.请严格按格式输入，非法输入可能会产生一些莫名其妙的问题（俗称BUG） Orz" << endl;
	cout << _STAR_ << _STAR_ << endl;
	system("pause"); system("cls");
}

//新增城市
void Add_City(string u) {
	city_id[u] = city_cnt; id_city[city_cnt] = u; //城市名与编号相对应
	path.push_back(pii(-1, -1)); //初始化路径 保留结点的父节点
	cost[0].push_back(inf); cost[1].push_back(inf); //初始化权值花费 均为正无穷
	vector<node> vec; graph.push_back(vec); //为图的邻接表新增一行
	++city_cnt; //城市总数+1
}

//核心的寻路算法
void dijkstra(int s) {
	//初始化 城市s 到达 城市j 的第一、二关键字花费
	rap(i, 0, 2) rep(j, cost[i].size() - 1, 0) cost[i][j] = inf;
	while (!Q.empty()) Q.pop(); //队列清空
	cost[0][s] = cost[1][s] = 0; path[s] = pii(-1, -1); //起点初始化
	Q.push(node(0, s, 0, 0)); //将起点加入堆
	//dijkstra算法 + 堆优化
	while (!Q.empty()) {
		//获取关键字最小的结点
		node tmp = Q.top(); Q.pop();
		//遍历到达城市（编号）tmp.CityID 的邻接表， 
		for (auto it : graph[tmp.CityID]) {
			//第一关键字更优 直接更新
			//cost[0][it.CityID] 先前到达城市（编号） it.CityID 的第一关键字花费
			//tmp.first 目前到达城市（编号）tmp.CityID 的第一关键字权值花费
			//it.first 下一个到达城市（编号）it.CityID 的第一关键字权值花费
			if (cost[0][it.CityID] > tmp.first + it.first) {
				cost[0][it.CityID] = tmp.first + it.first; //更新第一关键字
				cost[1][it.CityID] = tmp.second + it.second; //更新第二关键字
				path[it.CityID] = pii(tmp.CityID, it.RoadID); //更新路径
				Q.push(node(it.RoadID, it.CityID, cost[0][it.CityID], cost[1][it.CityID])); //将新增结点压入堆中
			}
			else if (cost[0][it.CityID] < tmp.first + it.first);
			//第一关键字相等 按第二关键字更新
			else {
				if (cost[1][it.CityID] > tmp.second + it.second) {
					cost[1][it.CityID] = tmp.second + it.second; //更新第二关键字
					path[it.CityID] = pii(tmp.CityID, it.RoadID); //更新路径
					Q.push(node(it.RoadID, it.CityID, cost[0][it.CityID], cost[1][it.CityID])); //将新增结点压入堆中
				}
			}
		}
	}
}

//输出路径
void OutPath(int x) {
	int s = x; //起点城市s 终点城市x
	cout << "导航成功，具体路径为：\n" << endl;
	vec.clear();
	//从终点城市x 一直回溯到 起点城市s 并保留路径
	while (path[s].first != -1) vec.push_back(path[s]), s = path[s].first;
	//按相反的顺序输出路径
	for (auto it = vec.rbegin(); it != vec.rend(); ++it) cout << id_city[it->first], cout << " =道路" << it->second << "=》";
	cout << id_city[x] << endl << endl; //输出终点城市
}