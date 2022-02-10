#include <iostream> //C++ �����������
#include <cstdio> //C ��׼���������
#include <cstdlib> //C ��׼������
#include <cstring> //C �ַ���������
#include <string> //C++ string���
#include <vector> //C++ vector������
#include <stack> //C++ stackջ��
#include <algorithm> //C++ �����㷨��

//�궨�岿��
#define rap(i, a, b) for(int i = (a); i < (b); ++i)
#define rep(i, a, b) for(int i = (a); i >= (b); --i)
#define _PAC_ system("pause"), system("cls")
#define _WHICH_(i) cout << _STAR_ << endl << "��ǰ��ʾ��" << sortname[(i - 1)] << "�㷨" <<endl
#define _CASE_ cout << "�� " << (++_case) << " ����������" << endl
#define _STAR_ "****************************************"

// 9 554 56 12 44 78 65 1 44 76
//20 15 65 1 44 5 8 5456 12 31 5 565 5 5 6 5451 210 12 21 89  20
using namespace std;

typedef pair<int, int> pii; //������pair, double������ int���±�
const string sortname[] = { "ð������ | �ȶ�", "��ѡ������ | ���ȶ�", "ֱ�Ӳ������� | �ȶ�", "�۰�������� |�ȶ�",
"ϣ������ | ���ȶ� ", "�������򣨵ݹ�棩| ���ȶ�", "�������򣨷ǵݹ�棩 | ���ȶ�", "������ | ���ȶ�" };


int _case; //��_case��
pii cmp_move; //�Ƚϴ������ƶ�����
vector<pii> num, vec; //ԭʼ����, ����������
stack<pii> st; //���ŷǵݹ���ջ

int Input(); //��������
void Output(vector<pii>&); //�������
int Judge_Stable(); //�ж��ȶ���
void Result(); //�ó�����

void Bubble_Sort(); //ð������
void Simple_Selection_Sort(); //��ѡ������
void Direct_Insertion_Sort(); //ֱ�Ӳ�������
void Binary_Insertion_Sort(); //�۰��������
void Shell_Sort(); //ϣ������
void QuickSort_Recursive(); //�������򣨵ݹ�棩
void QuickSort_NonRecursive(); //�������򣨷ǵݹ�棩
void Heap_Sort(); //������

int Partition(int, int); //������ ���ֺ���
void HeapAdjust(int, int); //�������� �ѣ��󶥶ѣ�����

int main() {
	system("color 30"); //����������ɫ
	while (true) {
		cout << _STAR_ << endl;
		cout << "   ��ӭʹ�ñ��ڲ�����ģ���㷨ϵͳ!" << endl;
		cout << "\t1.ð������" << endl;
		cout << "\t2.��ѡ������" << endl;
		cout << "\t3.ֱ�Ӳ�������" << endl;
		cout << "\t4.�۰��������" << endl;
		cout << "\t5.ϣ������" << endl;
		cout << "\t6.�������򣨵ݹ�棩" << endl;
		cout << "\t7.�������򣨷ǵݹ�棩" << endl;
		cout << "\t8.������" << endl;
		cout << "\t0.�˳�ϵͳ" << endl;
		cout << _STAR_ << endl;
		cout << "������ѡ��ǰ�������Լ���������"; int op; cin >> op;
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
		case 0: cout << "�˳�ϵͳ�ɹ���" << endl; return 0;
		default:
			cout << "����ѡ������������������룡 OvO" << endl;
			cout << _STAR_ << endl; _PAC_;
			break;
		}
	}
	return 0;
}

int Input() {
	//ȫ�ֱ�����ʼ��
	vec.clear(); num.clear(); cmp_move = pii(0, 0); _case = 0; while (!st.empty()) st.pop();
	cout << _STAR_ << endl;
	cout << "������һ����������ʾ����������������������0�������˵�����";
	int n; cin >> n; if (n <= 0) { system("cls"); return 0; }
	cout << "����������" << n << "��������ע��������Ŷ OvO�����Կո��س�����!" << endl;
	double x; rap(i, 0, n) cin >> x, vec.push_back(pii(x, i)), num.push_back(vec[i]);
	cout << _STAR_ << endl;
	return n;
}

void OutPut(vector<pii>& vec) {
	//��ֵ���±꣩
	for (auto it : vec) cout << it.first << "(" << it.second << ") "; cout << endl;
}

int Judge_Stable() {
	//����ͬ���������±���������ʾ���ȶ�
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
	cout << "ԭʼ���飺" << endl; OutPut(num);
	cout << "�������飺" << endl; OutPut(vec);
	cout << "�Ƚϴ�����" << cmp_move.first << endl;
	cout << "�ƶ�������" << cmp_move.second << endl;
	cout << "�ȶ���(��������������)��"; int Stable = Judge_Stable();
	if (Stable == -1) cout << "����������ͬԪ�أ��޷��ж�";
	else if (Stable) cout << "�ȶ�";
	else cout << "���ȶ�";
	cout << endl << _STAR_ << endl; _PAC_;
}

void Bubble_Sort() {
	_WHICH_(1); int n = Input(); if (!n) return;
	rap(i, 0, n - 1) { //n - 1��
		rap(j, 0, n - 1 - i) { //������λ�Ƚ�
			++cmp_move.first;
			if (vec[j].first > vec[j + 1].first) swap(vec[j], vec[j + 1]), ++cmp_move.second;
		}
		_CASE_; OutPut(vec);
	}
	Result();
}

void Simple_Selection_Sort() {
	_WHICH_(2); int n = Input(); if (!n) return;
	rap(i, 0, n) { //n��
		pii choose = pii(vec[i].first, i);
		rap(j, i + 1, n) { //��δ�ź���� n - iλѡ��Сֵ
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
	rap(i, 1, n) { //n - 1��
		if (vec[i].first < vec[i - 1].first) { //ǰ���ź���������������
			pii target = vec[i]; int j;
			for (j = i - 1; j >= 0; --j) { //�ӵ�ǰλ��ǰ�ҵ���һ�����ڵ���target��λ��
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
	//��Ϊÿһ�˲�������ʱ��ǰ������鶼������ģ���ˣ����Բ��ö��ֵķ�����λ��
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
	//һ���Ż����Ĳ�������,�߼�������в�������
	_WHICH_(5); int n = Input(); if (!n) return;
	for (int dk = n / 2; dk > 0; dk /= 2) { //ϣ������ dk[0] = n / 2, dk[1] = dk[0] / 2, ... , dk[n] = dk[n - 1] / 2; 
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

int Partition(int low, int high) { //���Ż�����
	int left = low, right = high; //�����ڱ�
	pii tmp = vec[left]; //��׼��
	while (left < right) {
		while (right > left) { //�ұߵ�һ��С�ڻ�׼����ֵ
			++cmp_move.first;
			if (vec[right].first >= tmp.first) --right;
			else break;
		}
		if (left < right) vec[left++] = vec[right], ++cmp_move.second;
		while (left < right) { //��ߵ�һ�����ڻ�׼����ֵ
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
	int mid = Partition(low, high); //��ǰ��������ɴ�������
	QuickSort(low, mid - 1); //�ݹ������� ��
	QuickSort(mid + 1, high); //�ݹ���������
}

void QuickSort_Recursive() {
	_WHICH_(6); int n = Input(); if (!n) return;
	QuickSort(0, n - 1);
	Result();
}

void QuickSort_NonRecursive() {
	_WHICH_(7); int n = Input(); if (!n) return;
	st.push(pii(0, n - 1)); //������������������俪ʼ�±꣬�����±�
	while (!st.empty()) {
		int low = st.top().first, high = st.top().second; st.pop();
		int mid = Partition(low, high);
		//��ջ�ṹ����ݹ����
		if (low < mid - 1) st.push(pii(low, mid - 1));
		if (mid + 1 < high) st.push(pii(mid + 1, high));
	}
	Result();
}

void HeapAdjust(int n, int idx) {
	//���Ҷ����±�
	int lson = idx * 2 + 1, rson = idx * 2 + 2;
	int MaxIdx = idx; //���ֵ�±�
	if (lson < n) {
		++cmp_move.first;
		if (vec[lson].first > vec[MaxIdx].first) MaxIdx = lson;
	}
	if (rson < n) {
		++cmp_move.first;
		if (vec[rson].first > vec[MaxIdx].first) MaxIdx = rson;
	}
	//������ֵ�Ƕ��ӣ������µݹ鴦���Ӧ����
	if (MaxIdx != idx) {
		swap(vec[MaxIdx], vec[idx]); ++cmp_move.second;
		HeapAdjust(n, MaxIdx);
	}
}

void Heap_Sort() {
	_WHICH_(8); int n = Input(); if (!n) return;
	//�ӵ�һ����Ҷ�ӽ���±꣨n / 2 - 1����ʼ�����ϴ�������㣬 ����һ���󶥶�
	rep(i, n / 2 - 1, 0) HeapAdjust(n, i), _CASE_, OutPut(vec);
	//���δ����ģλ(n - 1, n - 2, ..., 1)���Ķѣ�ÿ�ν����ֵ����ѵ�
	rep(i, n - 1, 1) swap(vec[0], vec[i]), ++cmp_move.second, HeapAdjust(i, 0), _CASE_, OutPut(vec);
	Result();
}
