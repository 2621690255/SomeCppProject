#include <iostream> //C++ �����������
#include <fstream> //C++ �ļ������������
#include <cstdio> //C ��׼���������
#include <cstdlib> //C ��׼������
#include <cstring> //C �ַ���������
#include <string> //C++ string���
#include <map> //C++ mapӳ���
#include <queue> //C++ queue���п�
#include <algorithm> //C++ �����㷨��

using namespace std;

/********************** Huffman����� **********************/
//Huffman���������
struct node {
	string s; //�ַ����浥���ַ��������ģ�
	int weight; //��¼��ǰ����Ȩֵ
	node* Left, * Right; //������ָ�룬������ָ��
};
//�Զ��������ȼ����������ȶ��У�
class Compare_Huffman_node {
public:
	bool operator()(node*& a, node*& b)const {
		return b->weight < a->weight; //С����
	}
};
typedef struct node* HuffmanTree, * HuffmanNode; //��������ָ�룬�����������ָ��

/********************** ȫ�ֱ��� **********************/
map<string, int> FrequencyTable; //�ַ�Ƶ�ʱ�
map<string, string> CodeTable; //�����������
priority_queue<HuffmanNode, vector<HuffmanNode>, Compare_Huffman_node> Q; //���ȶ��У�ȡȨֵ��С���Ķ�
string Source_s, ZeroOne_s, Decoding_s; //�洢 Դ�ļ�, �����������ļ��������ļ� ���ַ���
string CodeTable_s; //�洢 ����� ���ַ���
const int C_Bit = 8; //C_Bitλѹ��

/********************** �˵��� **********************/
void Encryption_Menu(); //����ѡ��˵�
void Decryption_Menu(); //����ѡ��˵�
void Compare_Menu(); //�Ƚ��ļ��˵�

/********************** �ļ����� **********************/
string Open_File(string, void(*p)(ifstream&)); //���ļ��� ����������������ƣ������ַ��ĺ���ָ��
void Out_File(string, void(*p)(ofstream&)); //����ļ�������������������ƣ�д���ַ��ĺ���ָ��
void Save_Encrypted(); //��������ļ�
void Load_Encrypted(); //���ؼ����ļ�
size_t Get_FileSize(const string&); //��ȡ�ļ���С

/********************** Huffman�� **********************/
HuffmanTree Build_Huffman(); //�����������
HuffmanTree Rebuild_Huffman(); //�ع���������

/********************** ����д���Ӻ��� **********************/
void Read_Source(ifstream&); //����ԭʼ�ļ������������ifstream��
void Read_Decoding(ifstream&);//��������ļ�
void Read_Encrypted(fstream&); //��������ļ������������ifstream��
void Read_CodeTable(ifstream&); //����������ļ������������ifstream��
void Write_ZeroOne(ofstream&); //�����01�ļ�
void Write_Decoding(ofstream&); //��������ļ�
void Write_CodeTable(ofstream&); //���������ļ�
void Write_Encrypted(fstream&); //��������ļ�

/********************** ���ܽ��� **********************/
void Coding(HuffmanTree, string); //��ȡ�������������������������õĹ������������ڴ������ַ���
void Encryption(); //��ԭʼ�ļ����й���������
void Decryption(HuffmanTree); //�Լ����ļ����롣�����������������

//����ȫ�ֱ���
void init() {
	FrequencyTable.clear(); CodeTable.clear();
	while (!Q.empty()) Q.pop();
	Source_s.clear(), ZeroOne_s.clear(), Decoding_s.clear();
	CodeTable_s.clear();
}
int main() {
	system("color 30"); //����������ɫ
	while (true) {
		cout << "��ӭʹ�ñ�ϵͳ�����ļ�����ѹ��/���뻹ԭ!" << endl;
		cout << "������ѡ��ǰ�������Լ���������" << endl;
		cout << "\t1.�ļ�����" << endl;
		cout << "\t2.�ļ�����" << endl;
		cout << "\t3.�ļ��Ƚ�" << endl;
		cout << "\t0.�˳�ϵͳ" << endl;
		int op; cin >> op;
		switch (op)
		{
			//�ļ�����
		case 1: system("cls"); Encryption_Menu();  break;
			//�ļ�����
		case 2: system("cls"); Decryption_Menu(); break;
			//�ļ��Ƚ�
		case 3: system("cls"); Compare_Menu(); break;
			//�˳�ϵͳ
		case 0: cout << "�˳�ϵͳ�ɹ���" << endl; return 0;
		default:
			cout << "����ѡ������������������룡" << endl;
			system("pause"); system("cls");
			break;
		}
	}
}

/********************** �˵��� **********************/
//����ѡ��˵�
void Encryption_Menu() {
	Open_File("ԭʼ", Read_Source); //��ԭʼ�ļ�
	cout << "��ԭʼ�ļ��ɹ������ڹ���Huffman��..." << endl;
	HuffmanTree T = Build_Huffman(); //����Ƶ�ʱ����������
	cout << "Huffman������ɹ������ڽ���Huffman����..." << endl;
	Coding(T, ""); //���ݹ���������ȡ����������
	cout << "����ɹ������ڼ���..." << endl;
	Encryption(); //��ԭʼ�ļ����й���������
	cout << "���ܳɹ���" << endl;
	cout << "********************************************************************\n" << endl;
	Save_Encrypted(); //ѹ������������ļ�
	cout << "�����ļ�������ɣ�\n" << endl;
	//Out_File("01", Write_ZeroOne); //���01�ļ�
	Out_File("�����", Write_CodeTable); //���������ļ�
	cout << "������ɣ����μ�������ļ����ļ���ַ�Լ�������ַ��" << endl;
	system("pause"); system("cls");
	init(); //����ȫ�ֱ���
}
//����ѡ��˵�
void Decryption_Menu() {
	Load_Encrypted(); //���ز���ѹ�������ļ�
	cout << "�����ļ���ѹ���ɹ���\n" << endl;
	Open_File("�����", Read_CodeTable);
	cout << "����������ϣ������ع�Huffman��..." << endl;
	HuffmanTree T = Rebuild_Huffman(); //����������ع���������
	cout << "Huffman���ع���ϣ����ڶԼ����ļ����н���..." << endl;
	Decryption(T); //�Լ����ļ����н���
	cout << "����ɹ���" << endl;
	cout << "********************************************************************\n" << endl;
	Out_File("����", Write_Decoding); //��������ļ�
	cout << "������ɣ����μ�������Ľ����ļ���ַ��" << endl;
	system("pause"); system("cls");
	init(); //����ȫ�ֱ���
}
//�Ƚ��ļ��˵�
void Compare_Menu() {
	Source_s.clear(); string file_name = Open_File("ԭʼ", Read_Source);
	size_t Source_size = Get_FileSize(file_name);
	cout << "\n------------------------------------------- ԭʼ�ļ� -------------------------------------------\n" << endl;
	cout << Source_s;
	cout << "\n------------------------------------------- ԭʼ�ļ� -------------------------------------------\n" << endl;
	Decoding_s.clear(); file_name = Open_File("����", Read_Decoding);
	size_t Decoding_size = Get_FileSize(file_name);
	cout << "\n------------------------------------------- �����ļ� -------------------------------------------\n" << endl;
	cout << Decoding_s;
	cout << "\n------------------------------------------- �����ļ� -------------------------------------------\n" << endl;

	cout << "*********************************************************************************" << endl;
	cout << "\tԭʼ�ļ���С��" << Source_size << " �ֽ�" << endl;
	cout << "\t�����ļ���С��" << Decoding_size << " �ֽ�" << endl;
	if (Source_s == Decoding_s) cout << "\t���ȶԣ�ԭʼ�ļ��������ļ�һ�£�" << endl;
	else cout << "\t���ȶԣ�ԭʼ�ļ��������ļ���һ�£�" << endl;
	cout << "*********************************************************************************\n" << endl;

	cout << "\t��������" << "����" << "�ļ���·�����ļ���: " << endl;
	cout << "ʾ����" << "C:\\Users\\lgc2621690255\\source\\repos\\Project1\\Project1\\" << "����" << "�ļ�.Huffman" << endl;
	file_name; cin >> file_name; size_t Encryption_size = Get_FileSize(file_name);
	cout << "*********************************************************************************" << endl;
	cout << "\tԭʼ�ļ���С��" << Source_size << " �ֽ�" << endl;
	cout << "\t�����ļ���С��" << Encryption_size << " �ֽ�" << endl;
	cout << "\tѹ����Ϊ��" << (1.0 - (double)Encryption_size / (double)Source_size) * 100 << "%" << endl;
	cout << "*********************************************************************************\n" << endl;
	system("pause"); system("cls");
	init(); //����ȫ�ֱ���
}
/********************** �˵��� **********************/


/********************** �ļ����� **********************/
//���ļ�
string Open_File(string op, void(*read)(ifstream&)) {
	ifstream infile; //�ļ�������
	cout << "\t������" << op << "�ļ��Ĵ��·�����ļ���: " << endl;
	cout << "ʾ����" << "C:\\Users\\lgc2621690255\\source\\repos\\Project1\\Project1\\" << op << "�ļ�.txt" << endl;
	string file_name; cin >> file_name;
	infile.open(file_name); //��ָ���ļ�
	if (!infile.is_open()) { //������ļ�ʧ��
		cout << "�ļ� " << file_name << " ��ʧ��!" << endl;
		cout << "����: 1.�ļ��Ƿ���� 2. ·���Ƿ���ȷ 3.�Ƿ���Ȩ�޷��ʸ��ļ� / �ļ���" << endl;
		exit(-1); //��ֹ�����׳��쳣ֵ
	}
	read(infile); //���ļ��������ַ�
	infile.close(); //�ر��ļ���
	cout << endl; //�������ۣ���ʵ������
	return file_name;
}
//����ļ����������������/�����ļ��ַ�����"����"/"����"
void Out_File(string op, void(*write)(ofstream&)) {
	ofstream outfile; //�ļ������
	cout << "\t��������" << op << "�ļ���·�����ļ���: " << endl;
	cout << "ʾ����" << "C:\\Users\\lgc2621690255\\source\\repos\\Project1\\Project1\\" << op << "�ļ�.txt" << endl;
	string file_name; cin >> file_name;
	outfile.open(file_name); //��ָ���ļ�
	if (!outfile.is_open()) { //������ļ�ʧ��
		cout << "�ļ� " << file_name << " ��ʧ��!" << endl;
		cout << "����: 1.�ļ��Ƿ��Ѵ��� 2. ·���Ƿ���ȷ 3.�Ƿ���Ȩ�޷��ʸ��ļ� / �ļ���" << endl;
		exit(-1); //��ֹ�����׳��쳣ֵ
	}
	write(outfile);
	outfile.close(); //�ر��ļ��� 
	cout << endl; //�������ۣ���ʵ������
}
//��������ļ�
void Save_Encrypted() {
	fstream file; //�ļ������
	cout << "\t��������" << "����" << "�ļ���·�����ļ���: " << endl;
	cout << "ʾ����" << "C:\\Users\\lgc2621690255\\source\\repos\\Project1\\Project1\\" << "����" << "�ļ�.Huffman" << endl;
	string file_name; cin >> file_name;
	file.open(file_name, ios::binary | ios::out); //��ָ���ļ�
	if (!file) { //������ļ�ʧ��
		cout << "�ļ� " << file_name << " ��ʧ��!" << endl;
		cout << "����: 1.�ļ��Ƿ��Ѵ��� 2. ·���Ƿ���ȷ 3.�Ƿ���Ȩ�޷��ʸ��ļ� / �ļ���" << endl;
		exit(-1); //��ֹ�����׳��쳣ֵ
	}
	Write_Encrypted(file);
	file.close(); file.clear();//�ر��ļ��� 
}
//���ؼ����ļ�
void Load_Encrypted() {
	fstream file; //�ļ�������
	cout << "\t������" << "����" << "�ļ��Ĵ��·�����ļ���: " << endl;
	cout << "ʾ����" << "C:\\Users\\lgc2621690255\\source\\repos\\Project1\\Project1\\" << "����" << "�ļ�.Huffman" << endl;
	string file_name; cin >> file_name;
	file.open(file_name, ifstream::in | ios::binary); //��ָ���ļ�
	if (!file) { //������ļ�ʧ��
		cout << "�ļ� " << file_name << " ��ʧ��!" << endl;
		cout << "����: 1.�ļ��Ƿ���� 2. ·���Ƿ���ȷ 3.�Ƿ���Ȩ�޷��ʸ��ļ� / �ļ���" << endl;
		exit(-1); //��ֹ�����׳��쳣ֵ
	}
	Read_Encrypted(file);
	file.close(); file.clear();//�ر��ļ���
}
//��ȡ�ļ���С
size_t Get_FileSize(const string& file_name) {
	ifstream in(file_name.c_str());
	in.seekg(0, ios::end);
	size_t size = in.tellg();
	in.close();
	return size; //��λ�ǣ�byte
}

/********************** �ļ����� **********************/


/********************** Huffman�� **********************/
//�����������
HuffmanTree Build_Huffman() {
	HuffmanTree T; T = new node; //�½�һ�Ź�������
	T->s = ""; T->weight = 0; T->Right = T->Left = NULL; //����ֵ
	while (Q.size() > 1) { //�����ȶ��У�ɭ�֣���Ԫ������ > 1
		T = new node; //�½�һ�Ź�������
		T->Left = Q.top(), Q.pop(); //������Ϊɭ����Ȩֵ��С����
		T->Right = Q.top(), Q.pop(); //������Ϊɭ����Ȩֵ��С����
		T->weight = T->Left->weight + T->Right->weight; //��ǰ����ȨֵΪ��������Ȩֵ֮��
		Q.push(T); //���¹���������²������ȶ�����
	}
	T = Q.top(), Q.pop(); //ɭ���ڽ����һ�Ź�������
	return T;
}
//�ع�Huffman��
HuffmanTree Rebuild_Huffman() {
	HuffmanTree T, _T; T = new node; //�½�һ�Ź�������
	T->s = ""; T->weight = 0; T->Right = T->Left = NULL; //����ֵ
	_T = T;
	for (auto it : CodeTable) {
		int size = it.first.size();
		T = _T;
		for (int i = 0; i < size; ++i) {
			HuffmanNode t; t = new node; //�½�һ�Ź�������
			t->s = ""; t->weight = 0; t->Right = t->Left = NULL; //����ֵ

			if (it.first[i] == '0') {
				if (T->Left == NULL) T->Left = t;
				T = T->Left;
			}
			else {
				if (T->Right == NULL) T->Right = t;
				T = T->Right;
			}

			if (i == size - 1) T->s = it.second;
		}
	}
	return _T;
}
/********************** Huffman�� **********************/


/********************** ����д���Ӻ��� **********************/
void Write_ZeroOne(ofstream& out) { out << ZeroOne_s; } //���01�ļ�
void Write_Decoding(ofstream& out) { out << Decoding_s; } //��������ļ�
 //���������ļ�
void Write_CodeTable(ofstream& out) {
	CodeTable_s.clear(); for (auto it : CodeTable) CodeTable_s += it.first + " " + it.second + "\n";
	out << CodeTable_s;
}
//��������ļ�
void Write_Encrypted(fstream& file) {
	int size = ZeroOne_s.size();
	unsigned char last_bit = (unsigned char)(size % C_Bit + '0'); file.write((char*)&last_bit, sizeof(last_bit));
	for (int i = 0; i < size; i += C_Bit) {
		int res = 0;
		for (int j = 0; i + j < size && j < C_Bit; ++j) res = res * 2 + (ZeroOne_s[i + j] - '0');
		unsigned char ch = res;
		file.write((char*)&ch, sizeof(ch));
	}
}
//�������ݡ����������ifstream��
void Read_Source(ifstream& in) {
	char ch; string s;
	while (in.peek() != EOF) { //���ļ��������ļ���βʱ
		ch = in.get(); s = ch; //һ��һ���ַ�����
		if (ch > 127 || ch < 0) { //�����ASCII����ַ��������ַ���
			ch = in.get(); s = s + ch;
		}
		++FrequencyTable[s]; //�ַ� s Ƶ�� + 1
		Source_s = Source_s + s; //ԭʼ�ļ��ַ���
	}
	//�����ַ�Ƶ�ʱ�
	for (auto it : FrequencyTable) {
		HuffmanNode t = new node; //�½�һ�����������
		t->s = it.first; t->weight = it.second; t->Left = NULL; t->Right = NULL; //Ϊ�丳ֵ
		Q.push(t); //���뵽���ȶ��У��ѣ���
	}
}
//��������ļ�
void Read_Decoding(ifstream& in) {
	char ch; string s;
	while (in.peek() != EOF) { //���ļ��������ļ���βʱ
		ch = in.get(); s = ch; //һ��һ���ַ�����
		if (ch > 127 || ch < 0) { //�����ASCII����ַ��������ַ���
			ch = in.get(); s = s + ch;
		}
		Decoding_s = Decoding_s + s; //ԭʼ�ļ��ַ���
	}
}
//��������ļ������������ifstream��
void Read_Encrypted(fstream& file) {
	unsigned char ch; int last_bit = -1;
	//���ζ�ȡ�������ļ��е�unsigned charֵ���뵽����i,�����
	while (file.read((char*)&ch, sizeof(ch))) {
		if (last_bit == -1) { last_bit = ch - '0'; continue; }
		int t = ch;
		string s;
		for (int i = 0; i < C_Bit; ++i) {
			s = s + (char)((t & 1) + '0'); t >>= 1;
		}
		reverse(s.begin(), s.end());
		ZeroOne_s = ZeroOne_s + s;
	}
	if (last_bit != 0) {
		int size = ZeroOne_s.size();
		string t; for (int i = size - last_bit; i < size; ++i) t = t + ZeroOne_s[i];
		for (int i = 0; i < C_Bit; ++i) ZeroOne_s.pop_back();
		ZeroOne_s = ZeroOne_s + t;
	}
	//cout << ZeroOne_s << endl;
}
//���������
void Read_CodeTable(ifstream& in) {
	char ch; string s;
	while (in.peek() != EOF) { //���ļ��������ļ���βʱ
		ch = in.get(); s = ch; //һ��һ���ַ�����
		if (ch > 127 || ch < 0) { //�����ASCII����ַ��������ַ���
			ch = in.get(); s = s + ch;
		}
		string coding; in >> coding; in.get();
		CodeTable[coding] = s;
	}
	//for (auto it : CodeTable) cout << it.first << " " << it.second << endl;
}
/********************** ����д���Ӻ��� **********************/


/********************** ���ܽ��� **********************/
//��ȡ�������������������������õĹ������������ڴ������ַ���
void Coding(HuffmanTree T, string s) {
	if (T->Left == NULL && T->Right == NULL) { //���ߵ���Ҷ�ӽ��
		CodeTable[T->s] = s; //�洢�ý�����ַ���01����
		return;
	}
	Coding(T->Left, s = s + "0"); //����ߣ����� + 0
	s.pop_back(); //����ʱ ���볤�� - 1
	Coding(T->Right, s = s + "1"); //�ұ��� ���� + 1
	s.pop_back(); //����ʱ ���볤�� - 1
}
//��ԭʼ�ļ�����
void Encryption() {
	int size = Source_s.size(); string s; //��ȡԭʼ�ļ��ĳ���
	for (int i = 0; i < size; ++i) {
		s = Source_s[i]; //��������ַ�

		if (Source_s[i] > 127 || Source_s[i] < 0) { //�����ASCII����ַ��������ַ���
			++i; s = s + Source_s[i];
		}
		ZeroOne_s = ZeroOne_s + CodeTable[s]; //��������� �ַ���Ӧ�� 01���� ��������ַ���
	}
}
//�Լ����ļ����롣��������������õĹ�������
void Decryption(HuffmanNode T) {
	int i = 0, size = ZeroOne_s.size(); //��ȡ�����ļ��ĳ���
	HuffmanNode _T = T; //�洢���������ĸ����
	while (i < size) {
		int cnt = 0; T = _T; //���ñ���
		while (T->Left != NULL && T->Right != NULL) { //��û�ߵ�Ҷ�ӽ��ʱ
			if (ZeroOne_s[i + cnt] == '0') T = T->Left; //�� 0 ������������
			else if (ZeroOne_s[i + cnt] == '1') T = T->Right; //�� 1 ������������
			++cnt; //���α���ĳ��� + 1
		}
		Decoding_s = Decoding_s + T->s; //�����������ַ�����
		i += cnt; //���߳���i ���� ���α���ĳ��� cnt
	}
}
/********************** ���ܽ��� **********************/