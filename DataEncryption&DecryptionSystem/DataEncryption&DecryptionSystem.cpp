#include <iostream> //C++ 流输入输出库
#include <fstream> //C++ 文件流输入输出库
#include <cstdio> //C 标准输入输出库
#include <cstdlib> //C 标准函数库
#include <cstring> //C 字符串函数库
#include <string> //C++ string类库
#include <map> //C++ map映射库
#include <queue> //C++ queue队列库
#include <algorithm> //C++ 常用算法库

using namespace std;

/********************** Huffman树结点 **********************/
//Huffman树单个结点
struct node {
	string s; //字符串存单个字符（含中文）
	int weight; //记录当前结点的权值
	node* Left, * Right; //左子树指针，右子树指针
};
//自定义结点优先级（用于优先队列）
class Compare_Huffman_node {
public:
	bool operator()(node*& a, node*& b)const {
		return b->weight < a->weight; //小顶堆
	}
};
typedef struct node* HuffmanTree, * HuffmanNode; //哈夫曼树指针，哈夫曼树结点指针

/********************** 全局变量 **********************/
map<string, int> FrequencyTable; //字符频率表
map<string, string> CodeTable; //哈夫曼编码表
priority_queue<HuffmanNode, vector<HuffmanNode>, Compare_Huffman_node> Q; //优先队列，取权值最小结点的堆
string Source_s, ZeroOne_s, Decoding_s; //存储 源文件, 哈夫曼编码文件，解码文件 的字符串
string CodeTable_s; //存储 译码表 的字符串
const int C_Bit = 8; //C_Bit位压缩

/********************** 菜单栏 **********************/
void Encryption_Menu(); //加密选项菜单
void Decryption_Menu(); //解码选项菜单
void Compare_Menu(); //比较文件菜单

/********************** 文件操作 **********************/
string Open_File(string, void(*p)(ifstream&)); //打开文件。 传入参数：操作名称，读入字符的函数指针
void Out_File(string, void(*p)(ofstream&)); //输出文件，传入参数：操作名称，写入字符的函数指针
void Save_Encrypted(); //保存加密文件
void Load_Encrypted(); //加载加密文件
size_t Get_FileSize(const string&); //获取文件大小

/********************** Huffman树 **********************/
HuffmanTree Build_Huffman(); //构造哈夫曼树
HuffmanTree Rebuild_Huffman(); //重构哈夫曼树

/********************** 读出写入子函数 **********************/
void Read_Source(ifstream&); //读入原始文件。传入参数：ifstream流
void Read_Decoding(ifstream&);//读入解码文件
void Read_Encrypted(fstream&); //读入加密文件。传入参数：ifstream流
void Read_CodeTable(ifstream&); //读入译码表文件。传入参数：ifstream流
void Write_ZeroOne(ofstream&); //输出出01文件
void Write_Decoding(ofstream&); //输出解码文件
void Write_CodeTable(ofstream&); //输出译码表文件
void Write_Encrypted(fstream&); //输出加密文件

/********************** 加密解码 **********************/
void Coding(HuffmanTree, string); //获取哈夫曼编码表。传入参数：构建好的哈夫曼树，用于存编码的字符串
void Encryption(); //对原始文件进行哈夫曼加密
void Decryption(HuffmanTree); //对加密文件解码。传入参数：哈夫曼树

//重置全局变量
void init() {
	FrequencyTable.clear(); CodeTable.clear();
	while (!Q.empty()) Q.pop();
	Source_s.clear(), ZeroOne_s.clear(), Decoding_s.clear();
	CodeTable_s.clear();
}
int main() {
	system("color 30"); //换个背景颜色
	while (true) {
		cout << "欢迎使用本系统进行文件加密压缩/解码还原!" << endl;
		cout << "请输入选项前的数字以继续操作：" << endl;
		cout << "\t1.文件加密" << endl;
		cout << "\t2.文件解码" << endl;
		cout << "\t3.文件比较" << endl;
		cout << "\t0.退出系统" << endl;
		int op; cin >> op;
		switch (op)
		{
			//文件加密
		case 1: system("cls"); Encryption_Menu();  break;
			//文件解码
		case 2: system("cls"); Decryption_Menu(); break;
			//文件比较
		case 3: system("cls"); Compare_Menu(); break;
			//退出系统
		case 0: cout << "退出系统成功！" << endl; return 0;
		default:
			cout << "输入选项有误，请检查后重新输入！" << endl;
			system("pause"); system("cls");
			break;
		}
	}
}

/********************** 菜单栏 **********************/
//加密选项菜单
void Encryption_Menu() {
	Open_File("原始", Read_Source); //打开原始文件
	cout << "打开原始文件成功，正在构造Huffman树..." << endl;
	HuffmanTree T = Build_Huffman(); //根据频率表构造哈夫曼树
	cout << "Huffman树构造成功，正在进行Huffman编码..." << endl;
	Coding(T, ""); //根据哈夫曼树获取哈夫曼编码
	cout << "编码成功，正在加密..." << endl;
	Encryption(); //对原始文件进行哈夫曼加密
	cout << "加密成功！" << endl;
	cout << "********************************************************************\n" << endl;
	Save_Encrypted(); //压缩并保存加密文件
	cout << "加密文件保存完成！\n" << endl;
	//Out_File("01", Write_ZeroOne); //输出01文件
	Out_File("译码表", Write_CodeTable); //输出译码表文件
	cout << "操作完成！请牢记您输入的加密文件地址以及译码表地址！" << endl;
	system("pause"); system("cls");
	init(); //重置全局变量
}
//解码选项菜单
void Decryption_Menu() {
	Load_Encrypted(); //加载并解压缩加密文件
	cout << "加密文件解压缩成功！\n" << endl;
	Open_File("译码表", Read_CodeTable);
	cout << "译码表加载完毕，正在重构Huffman树..." << endl;
	HuffmanTree T = Rebuild_Huffman(); //根据译码表重构哈夫曼树
	cout << "Huffman树重构完毕，正在对加密文件进行解码..." << endl;
	Decryption(T); //对加密文件进行解码
	cout << "解码成功！" << endl;
	cout << "********************************************************************\n" << endl;
	Out_File("解码", Write_Decoding); //输出解码文件
	cout << "操作完成！请牢记您输入的解码文件地址！" << endl;
	system("pause"); system("cls");
	init(); //重置全局变量
}
//比较文件菜单
void Compare_Menu() {
	Source_s.clear(); string file_name = Open_File("原始", Read_Source);
	size_t Source_size = Get_FileSize(file_name);
	cout << "\n------------------------------------------- 原始文件 -------------------------------------------\n" << endl;
	cout << Source_s;
	cout << "\n------------------------------------------- 原始文件 -------------------------------------------\n" << endl;
	Decoding_s.clear(); file_name = Open_File("解码", Read_Decoding);
	size_t Decoding_size = Get_FileSize(file_name);
	cout << "\n------------------------------------------- 解码文件 -------------------------------------------\n" << endl;
	cout << Decoding_s;
	cout << "\n------------------------------------------- 解码文件 -------------------------------------------\n" << endl;

	cout << "*********************************************************************************" << endl;
	cout << "\t原始文件大小：" << Source_size << " 字节" << endl;
	cout << "\t解码文件大小：" << Decoding_size << " 字节" << endl;
	if (Source_s == Decoding_s) cout << "\t经比对，原始文件与译码文件一致！" << endl;
	else cout << "\t经比对，原始文件与译码文件不一致！" << endl;
	cout << "*********************************************************************************\n" << endl;

	cout << "\t请输入存放" << "加密" << "文件的路径及文件名: " << endl;
	cout << "示例：" << "C:\\Users\\lgc2621690255\\source\\repos\\Project1\\Project1\\" << "加密" << "文件.Huffman" << endl;
	file_name; cin >> file_name; size_t Encryption_size = Get_FileSize(file_name);
	cout << "*********************************************************************************" << endl;
	cout << "\t原始文件大小：" << Source_size << " 字节" << endl;
	cout << "\t加密文件大小：" << Encryption_size << " 字节" << endl;
	cout << "\t压缩比为：" << (1.0 - (double)Encryption_size / (double)Source_size) * 100 << "%" << endl;
	cout << "*********************************************************************************\n" << endl;
	system("pause"); system("cls");
	init(); //重置全局变量
}
/********************** 菜单栏 **********************/


/********************** 文件操作 **********************/
//打开文件
string Open_File(string op, void(*read)(ifstream&)) {
	ifstream infile; //文件输入流
	cout << "\t请输入" << op << "文件的存放路径及文件名: " << endl;
	cout << "示例：" << "C:\\Users\\lgc2621690255\\source\\repos\\Project1\\Project1\\" << op << "文件.txt" << endl;
	string file_name; cin >> file_name;
	infile.open(file_name); //打开指定文件
	if (!infile.is_open()) { //如果打开文件失败
		cout << "文件 " << file_name << " 打开失败!" << endl;
		cout << "请检查: 1.文件是否存在 2. 路径是否正确 3.是否无权限访问该文件 / 文件夹" << endl;
		exit(-1); //终止程序，抛出异常值
	}
	read(infile); //从文件流读入字符
	infile.close(); //关闭文件流
	cout << endl; //纯属美观，无实际意义
	return file_name;
}
//输出文件，传入参数：加密/解码文件字符串，"加密"/"解码"
void Out_File(string op, void(*write)(ofstream&)) {
	ofstream outfile; //文件输出流
	cout << "\t请输入存放" << op << "文件的路径及文件名: " << endl;
	cout << "示例：" << "C:\\Users\\lgc2621690255\\source\\repos\\Project1\\Project1\\" << op << "文件.txt" << endl;
	string file_name; cin >> file_name;
	outfile.open(file_name); //打开指定文件
	if (!outfile.is_open()) { //如果打开文件失败
		cout << "文件 " << file_name << " 打开失败!" << endl;
		cout << "请检查: 1.文件是否已存在 2. 路径是否正确 3.是否无权限访问该文件 / 文件夹" << endl;
		exit(-1); //终止程序，抛出异常值
	}
	write(outfile);
	outfile.close(); //关闭文件流 
	cout << endl; //纯属美观，无实际意义
}
//保存加密文件
void Save_Encrypted() {
	fstream file; //文件输出流
	cout << "\t请输入存放" << "加密" << "文件的路径及文件名: " << endl;
	cout << "示例：" << "C:\\Users\\lgc2621690255\\source\\repos\\Project1\\Project1\\" << "加密" << "文件.Huffman" << endl;
	string file_name; cin >> file_name;
	file.open(file_name, ios::binary | ios::out); //打开指定文件
	if (!file) { //如果打开文件失败
		cout << "文件 " << file_name << " 打开失败!" << endl;
		cout << "请检查: 1.文件是否已存在 2. 路径是否正确 3.是否无权限访问该文件 / 文件夹" << endl;
		exit(-1); //终止程序，抛出异常值
	}
	Write_Encrypted(file);
	file.close(); file.clear();//关闭文件流 
}
//加载加密文件
void Load_Encrypted() {
	fstream file; //文件输入流
	cout << "\t请输入" << "加密" << "文件的存放路径及文件名: " << endl;
	cout << "示例：" << "C:\\Users\\lgc2621690255\\source\\repos\\Project1\\Project1\\" << "加密" << "文件.Huffman" << endl;
	string file_name; cin >> file_name;
	file.open(file_name, ifstream::in | ios::binary); //打开指定文件
	if (!file) { //如果打开文件失败
		cout << "文件 " << file_name << " 打开失败!" << endl;
		cout << "请检查: 1.文件是否存在 2. 路径是否正确 3.是否无权限访问该文件 / 文件夹" << endl;
		exit(-1); //终止程序，抛出异常值
	}
	Read_Encrypted(file);
	file.close(); file.clear();//关闭文件流
}
//获取文件大小
size_t Get_FileSize(const string& file_name) {
	ifstream in(file_name.c_str());
	in.seekg(0, ios::end);
	size_t size = in.tellg();
	in.close();
	return size; //单位是：byte
}

/********************** 文件操作 **********************/


/********************** Huffman树 **********************/
//构造哈夫曼树
HuffmanTree Build_Huffman() {
	HuffmanTree T; T = new node; //新建一颗哈夫曼树
	T->s = ""; T->weight = 0; T->Right = T->Left = NULL; //赋初值
	while (Q.size() > 1) { //当优先队列（森林）内元素数量 > 1
		T = new node; //新建一颗哈夫曼树
		T->Left = Q.top(), Q.pop(); //左子树为森林内权值最小的树
		T->Right = Q.top(), Q.pop(); //右子树为森林内权值次小的树
		T->weight = T->Left->weight + T->Right->weight; //当前结点的权值为左右子树权值之和
		Q.push(T); //将新构造的树重新插入优先队列中
	}
	T = Q.top(), Q.pop(); //森林内仅存的一颗哈夫曼树
	return T;
}
//重构Huffman树
HuffmanTree Rebuild_Huffman() {
	HuffmanTree T, _T; T = new node; //新建一颗哈夫曼树
	T->s = ""; T->weight = 0; T->Right = T->Left = NULL; //赋初值
	_T = T;
	for (auto it : CodeTable) {
		int size = it.first.size();
		T = _T;
		for (int i = 0; i < size; ++i) {
			HuffmanNode t; t = new node; //新建一颗哈夫曼树
			t->s = ""; t->weight = 0; t->Right = t->Left = NULL; //赋初值

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
/********************** Huffman树 **********************/


/********************** 读出写入子函数 **********************/
void Write_ZeroOne(ofstream& out) { out << ZeroOne_s; } //输出01文件
void Write_Decoding(ofstream& out) { out << Decoding_s; } //输出解码文件
 //输出译码表文件
void Write_CodeTable(ofstream& out) {
	CodeTable_s.clear(); for (auto it : CodeTable) CodeTable_s += it.first + " " + it.second + "\n";
	out << CodeTable_s;
}
//输出加密文件
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
//读入数据。传入参数：ifstream流
void Read_Source(ifstream& in) {
	char ch; string s;
	while (in.peek() != EOF) { //当文件流不到文件结尾时
		ch = in.get(); s = ch; //一个一个字符读入
		if (ch > 127 || ch < 0) { //处理非ASCII码的字符（中文字符）
			ch = in.get(); s = s + ch;
		}
		++FrequencyTable[s]; //字符 s 频率 + 1
		Source_s = Source_s + s; //原始文件字符串
	}
	//遍历字符频率表
	for (auto it : FrequencyTable) {
		HuffmanNode t = new node; //新建一个哈夫曼结点
		t->s = it.first; t->weight = it.second; t->Left = NULL; t->Right = NULL; //为其赋值
		Q.push(t); //加入到优先队列（堆）中
	}
}
//读入解码文件
void Read_Decoding(ifstream& in) {
	char ch; string s;
	while (in.peek() != EOF) { //当文件流不到文件结尾时
		ch = in.get(); s = ch; //一个一个字符读入
		if (ch > 127 || ch < 0) { //处理非ASCII码的字符（中文字符）
			ch = in.get(); s = s + ch;
		}
		Decoding_s = Decoding_s + s; //原始文件字符串
	}
}
//读入加密文件。传入参数：ifstream流
void Read_Encrypted(fstream& file) {
	unsigned char ch; int last_bit = -1;
	//依次读取二进制文件中的unsigned char值读入到变量i,并输出
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
//读入译码表
void Read_CodeTable(ifstream& in) {
	char ch; string s;
	while (in.peek() != EOF) { //当文件流不到文件结尾时
		ch = in.get(); s = ch; //一个一个字符读入
		if (ch > 127 || ch < 0) { //处理非ASCII码的字符（中文字符）
			ch = in.get(); s = s + ch;
		}
		string coding; in >> coding; in.get();
		CodeTable[coding] = s;
	}
	//for (auto it : CodeTable) cout << it.first << " " << it.second << endl;
}
/********************** 读出写入子函数 **********************/


/********************** 加密解码 **********************/
//获取哈夫曼编码表。传入参数：构建好的哈夫曼树，用于存编码的字符串
void Coding(HuffmanTree T, string s) {
	if (T->Left == NULL && T->Right == NULL) { //当走到了叶子结点
		CodeTable[T->s] = s; //存储该结点内字符的01编码
		return;
	}
	Coding(T->Left, s = s + "0"); //左边走，编码 + 0
	s.pop_back(); //回溯时 编码长度 - 1
	Coding(T->Right, s = s + "1"); //右边走 编码 + 1
	s.pop_back(); //回溯时 编码长度 - 1
}
//对原始文件加密
void Encryption() {
	int size = Source_s.size(); string s; //获取原始文件的长度
	for (int i = 0; i < size; ++i) {
		s = Source_s[i]; //逐个处理字符

		if (Source_s[i] > 127 || Source_s[i] < 0) { //处理非ASCII码的字符（中文字符）
			++i; s = s + Source_s[i];
		}
		ZeroOne_s = ZeroOne_s + CodeTable[s]; //将编码表中 字符对应的 01编码 存入加密字符串
	}
}
//对加密文件解码。传入参数：加密用的哈夫曼树
void Decryption(HuffmanNode T) {
	int i = 0, size = ZeroOne_s.size(); //获取加密文件的长度
	HuffmanNode _T = T; //存储哈夫曼树的根结点
	while (i < size) {
		int cnt = 0; T = _T; //重置变量
		while (T->Left != NULL && T->Right != NULL) { //当没走到叶子结点时
			if (ZeroOne_s[i + cnt] == '0') T = T->Left; //是 0 则向左子树走
			else if (ZeroOne_s[i + cnt] == '1') T = T->Right; //是 1 则向左子树走
			++cnt; //本次编码的长度 + 1
		}
		Decoding_s = Decoding_s + T->s; //结果存入解码字符串中
		i += cnt; //已走长度i 加上 本次编码的长度 cnt
	}
}
/********************** 加密解码 **********************/