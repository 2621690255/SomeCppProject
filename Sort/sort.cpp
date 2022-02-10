#include <iostream> //C++ 流输入输出库
#include <cstdio> //C 标准输入输出库
#include <cstdlib> //C 标准函数库
#include <cstring> //C 字符串函数库
#include <string> //C++ string类库
#include <vector> //C++ vector向量库
#include <stack> //C++ stack栈库
#include <algorithm> //C++ 常用算法库

//宏定义部分
#define rap(i, a, b) for(int i = (a); i < (b); ++i)
#define rep(i, a, b) for(int i = (a); i >= (b); --i)
#define _PAC_ system("pause"), system("cls")
#define _WHICH_(i) cout << _STAR_ << endl << "当前演示：" << sortname[(i - 1)] << "算法" <<endl
#define _CASE_ cout << "第 " << (++_case) << " 趟排序结果：" << endl
#define _STAR_ "****************************************"

// 9 554 56 12 44 78 65 1 44 76
//20 15 65 1 44 5 8 5456 12 31 5 565 5 5 6 5451 210 12 21 89  20
using namespace std;

typedef pair<int, int> pii; //重命名pair, double存数， int存下标
const string sortname[] = { "冒泡排序 | 稳定", "简单选择排序 | 不稳定", "直接插入排序 | 稳定", "折半插入排序 |稳定",
"希尔排序 | 不稳定 ", "快速排序（递归版）| 不稳定", "快速排序（非递归版） | 不稳定", "堆排序 | 不稳定" };


int _case; //第_case趟
pii cmp_move; //比较次数，移动次数
vector<pii> num, vec; //原始数组, 待排序数组
stack<pii> st; //快排非递归用栈

int Input(); //输入数组
void Output(vector<pii>&); //输出数组
int Judge_Stable(); //判断稳定性
void Result(); //得出结论

void Bubble_Sort(); //冒泡排序
void Simple_Selection_Sort(); //简单选择排序
void Direct_Insertion_Sort(); //直接插入排序
void Binary_Insertion_Sort(); //折半插入排序
void Shell_Sort(); //希尔排序
void QuickSort_Recursive(); //快速排序（递归版）
void QuickSort_NonRecursive(); //快速排序（非递归版）
void Heap_Sort(); //堆排序

int Partition(int, int); //快排用 划分函数
void HeapAdjust(int, int); //堆排序用 堆（大顶堆）调整

int main() {
	system("color 30"); //换个背景颜色
	while (true) {
		cout << _STAR_ << endl;
		cout << "   欢迎使用本内部排序模拟算法系统!" << endl;
		cout << "\t1.冒泡排序" << endl;
		cout << "\t2.简单选择排序" << endl;
		cout << "\t3.直接插入排序" << endl;
		cout << "\t4.折半插入排序" << endl;
		cout << "\t5.希尔排序" << endl;
		cout << "\t6.快速排序（递归版）" << endl;
		cout << "\t7.快速排序（非递归版）" << endl;
		cout << "\t8.堆排序" << endl;
		cout << "\t0.退出系统" << endl;
		cout << _STAR_ << endl;
		cout << "请输入选项前的数字以继续操作："; int op; cin >> op;
		switch (op)
		{
		case 1: system("cls"); Bubble_Sort(); break;
		case 2: system("cls"); Simple_Selection_Sort(); break;
		case 3: system("cls"); Direct_Insertion_Sort(); break;
		case 4: system("cls"); Binary_Insertion_Sort(); break;
		case 5: system("cls"); Shell_Sort(); break;
		case 6: system("cls"); QuickSort_Recursive(); break;
		case 7: system("cls"); QuickSort_NonRecursive(); break;
		case 8: system("cls"); Heap_Sort(); break;
		case 0: cout << "退出系统成功！" << endl; return 0;
		default:
			cout << "输入选项有误，请检查后重新输入！ OvO" << endl;
			cout << _STAR_ << endl; _PAC_;
			break;
		}
	}
	return 0;
}

int Input() {
	//全局变量初始化
	vec.clear(); num.clear(); cmp_move = pii(0, 0); _case = 0; while (!st.empty()) st.pop();
	cout << _STAR_ << endl;
	cout << "请输入一个正整数表示待排序数的总数量（输入0返回主菜单）：";
	int n; cin >> n; if (n <= 0) { system("cls"); return 0; }
	cout << "请依次输入" << n << "个整数（注意是整数哦 OvO），以空格或回车隔开!" << endl;
	double x; rap(i, 0, n) cin >> x, vec.push_back(pii(x, i)), num.push_back(vec[i]);
	cout << _STAR_ << endl;
	return n;
}

void OutPut(vector<pii>& vec) {
	//数值（下标）
	for (auto it : vec) cout << it.first << "(" << it.second << ") "; cout << endl;
}

int Judge_Stable() {
	//有相同的数，且下标非升序，则表示不稳定
	int n = vec.size(); bool same = false;
	rap(i, 1, n)
		if (vec[i - 1].first == vec[i].first) {
			same = true;
			if (vec[i].second < vec[i - 1].second) return 0;
		}
	return (same ? 1 : -1);
}

void Result() {
	cout << _STAR_ << endl;
	cout << "原始数组：" << endl; OutPut(num);
	cout << "升序数组：" << endl; OutPut(vec);
	cout << "比较次数：" << cmp_move.first << endl;
	cout << "移动次数：" << cmp_move.second << endl;
	cout << "稳定性(仅代表本次排序结果)："; int Stable = Judge_Stable();
	if (Stable == -1) cout << "数组内无相同元素，无法判断";
	else if (Stable) cout << "稳定";
	else cout << "不稳定";
	cout << endl << _STAR_ << endl; _PAC_;
}

void Bubble_Sort() {
	_WHICH_(1); int n = Input(); if (!n) return;
	rap(i, 0, n - 1) { //n - 1趟
		rap(j, 0, n - 1 - i) { //相邻两位比较
			++cmp_move.first;
			if (vec[j].first > vec[j + 1].first) swap(vec[j], vec[j + 1]), ++cmp_move.second;
		}
		_CASE_; OutPut(vec);
	}
	Result();
}

void Simple_Selection_Sort() {
	_WHICH_(2); int n = Input(); if (!n) return;
	rap(i, 0, n) { //n趟
		pii choose = pii(vec[i].first, i);
		rap(j, i + 1, n) { //从未排好序的 n - i位选最小值
			++cmp_move.first;
			if (vec[j].first < choose.first) choose = pii(vec[j].first, j);
		}
		swap(vec[i], vec[choose.second]);
		_CASE_; OutPut(vec);
		++cmp_move.second;
	}
	Result();
}

void Direct_Insertion_Sort() {
	_WHICH_(3); int n = Input(); if (!n) return;
	rap(i, 1, n) { //n - 1趟
		if (vec[i].first < vec[i - 1].first) { //前面排好序的是升序的数组
			pii target = vec[i]; int j;
			for (j = i - 1; j >= 0; --j) { //从当前位往前找到第一个大于等于target的位置
				++cmp_move.first;
				if (target.first < vec[j].first) vec[j + 1] = vec[j];
				else break;
			}
			vec[j + 1] = target; ++cmp_move.second;
		}
		else { ++cmp_move.first; continue; }
		_CASE_; OutPut(vec);
	}
	Result();
}

void Binary_Insertion_Sort() {
	//因为每一趟插入排序时，前面的数组都是有序的，因此，可以采用二分的方法找位置
	_WHICH_(4); int n = Input(); if (!n) return;
	rap(i, 1, n) {
		int left = 0, right = i - 1;
		pii target = vec[i];
		while (left <= right) {
			int mid = (left + right) >> 1;
			++cmp_move.first;
			if (vec[mid].first > target.first) right = mid - 1;
			else left = mid + 1;
		}
		rep(j, i - 1, left) vec[j + 1] = vec[j];
		vec[left] = target; ++cmp_move.second;
		_CASE_; OutPut(vec);
	}
	Result();
}

void Shell_Sort() {
	//一种优化过的插入排序,逻辑分组进行插入排序
	_WHICH_(5); int n = Input(); if (!n) return;
	for (int dk = n / 2; dk > 0; dk /= 2) { //希尔基数 dk[0] = n / 2, dk[1] = dk[0] / 2, ... , dk[n] = dk[n - 1] / 2; 
		rap(i, dk, n) {
			if (vec[i].first < vec[i - dk].first) {
				pii target = vec[i]; int j;
				for (j = i - dk; j >= 0; j -= dk) {
					++cmp_move.first;
					if (vec[j].first > target.first) vec[j + dk] = vec[j];
					else break;
				}
				vec[j + dk] = target; ++cmp_move.second;
			}
			else { ++cmp_move.first; continue; }
		}
		_CASE_; OutPut(vec);
	}
	Result();
}

int Partition(int low, int high) { //快排划分用
	int left = low, right = high; //左右哨兵
	pii tmp = vec[left]; //基准数
	while (left < right) {
		while (right > left) { //右边第一个小于基准数的值
			++cmp_move.first;
			if (vec[right].first >= tmp.first) --right;
			else break;
		}
		if (left < right) vec[left++] = vec[right], ++cmp_move.second;
		while (left < right) { //左边第一个大于基准数的值
			++cmp_move.first;
			if (vec[left].first <= tmp.first) ++left;
			else break;
		}
		if (right > left) vec[right--] = vec[left], ++cmp_move.second;
	}
	vec[left] = tmp; if (left != low) ++cmp_move.second;
	_CASE_; OutPut(vec);
	return left;
}

void QuickSort(int low, int high) {
	if (low >= high) return;
	int mid = Partition(low, high); //当前区间调整成大致有序
	QuickSort(low, mid - 1); //递归左子区 间
	QuickSort(mid + 1, high); //递归右子区间
}

void QuickSort_Recursive() {
	_WHICH_(6); int n = Input(); if (!n) return;
	QuickSort(0, n - 1);
	Result();
}

void QuickSort_NonRecursive() {
	_WHICH_(7); int n = Input(); if (!n) return;
	st.push(pii(0, n - 1)); //传入参数，待排序区间开始下标，结束下标
	while (!st.empty()) {
		int low = st.top().first, high = st.top().second; st.pop();
		int mid = Partition(low, high);
		//以栈结构代替递归操作
		if (low < mid - 1) st.push(pii(low, mid - 1));
		if (mid + 1 < high) st.push(pii(mid + 1, high));
	}
	Result();
}

void HeapAdjust(int n, int idx) {
	//左右儿子下标
	int lson = idx * 2 + 1, rson = idx * 2 + 2;
	int MaxIdx = idx; //最大值下标
	if (lson < n) {
		++cmp_move.first;
		if (vec[lson].first > vec[MaxIdx].first) MaxIdx = lson;
	}
	if (rson < n) {
		++cmp_move.first;
		if (vec[rson].first > vec[MaxIdx].first) MaxIdx = rson;
	}
	//如果最大值是儿子，则向下递归处理对应子树
	if (MaxIdx != idx) {
		swap(vec[MaxIdx], vec[idx]); ++cmp_move.second;
		HeapAdjust(n, MaxIdx);
	}
}

void Heap_Sort() {
	_WHICH_(8); int n = Input(); if (!n) return;
	//从第一个非叶子结点下标（n / 2 - 1）开始，向上处理到根结点， 构建一个大顶堆
	rep(i, n / 2 - 1, 0) HeapAdjust(n, i), _CASE_, OutPut(vec);
	//依次处理规模位(n - 1, n - 2, ..., 1)个的堆，每次将最大值沉入堆底
	rep(i, n - 1, 1) swap(vec[0], vec[i]), ++cmp_move.second, HeapAdjust(i, 0), _CASE_, OutPut(vec);
	Result();
}
