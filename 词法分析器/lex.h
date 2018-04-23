#ifndef LEX_H_
#define LEX_H_

#include<string>
#include<vector>
using namespace std;


//图的转移，主要是来表示
struct Arc{
	int nodeIndex;//下一个节点的下标，节点采用的是数组，所以采用这种形式
	char changeValue;//变化的值"\0"表示为空值
	Arc* nextArc = NULL;//表示下一个
};



//图的节点，这个表示一个个状态
struct  Node
{
	std::string value;//状态的值，但是不是用这个来寻找节点
	Arc* firstArc=NULL;//这个点的出度的边，采用的是链表方式
}; 

//采用class是为了内存的管理方向，可以实现自动调用析构函数
class Graph
{
private:
	int maxLength;//nodes的最大长度，node采用new的方式来管理，当最大的时候会采用扩充的形式
	int tail;	//现在节点用到的位置
public:
	void CopyGraph(const Graph& g);//拷贝构造函数
	void clearG();
	Node* nodes;//各个节点，采用的数组形式
	int newNode();//获得最最后一个节点的下一个，相当于分配
	bool extendNodes();//扩展nodes数组
	void insertArc(int index,Arc* a);//给index这个节点插入一条弧
	int getTail();
	Graph(int maxLength=100);
	Graph(const Graph& g);//拷贝构造函数
	~Graph();		//析构函数，主要是回收nodes和nodes的链表的内存空间

};
//栈的中间结果
struct MidResult
{
	int S0;
	int F0;
};


//运算写入一个类中
class Operation
{
private:
	int index = 0;//解析到表达式的位置;

	//运算符号的优先级,六个运算符（"(",")","#","|",".","*"）,pri[i][j],表示i的优先级是不是大于j，i在j的左边
	//1表示不需要操作，直接入栈，0表示需要进行操作
	bool pri[6][6];
	char ops[7];
	//展开运算式子，主要是用来将正规式子的一些特殊的含义字符展开，为只有上面的几个运算符合运算元素的字符串
	char* unfoldExpre(char* expression);


	int getopIndex(char op);//找到op的下标

	bool connectOp(Graph& g, MidResult& a, MidResult& b);//连接运算。,a=a。b

	bool andOp(Graph& g, MidResult& a, MidResult& b);//与运算|,a=a|b

	bool closureOp(Graph& g, MidResult& a);//闭包运算, a=a*

	char getNextChar(const char* expression);//针对于正规式，和算术表达式不一样，对于正规是操作数和操作符都只会占一个字符位置

	bool isOp(const char a);//判断a是不是操作符，如果是则返回1不是则返回0

	MidResult conversion(Graph& g, char a);//将字符串转化一下，同时也需要在图上给他分配空间

public:
	Operation();
	~Operation();
	//计算一个正规式子，输出一个NFA,计算没有出错会返回1出错会返回0,同时也需要给出S0和F0，采用这种方法构造的终态只有一个
	bool calute(Graph& g, const char* expression, int& S0, int& F0);
};


//考虑NFA，几个转化都是在这个类中实现的
class NFA
{
protected:


	Graph g;//包括了状态S和mov函数
	int S0;//初始状态
	char* words;//字母表
	int wordLength;//对应字母表的数组长度
	int* F0;//终态
	int tail;//F0的实际个数
	int maxF0Length;//对应终态的数组长度


	//判断elem是不是在v中，是则返回在v中的下标，否则返回-1
	int elemInVector(int elem, const vector<int>& v);
	int elemInVector(const vector<int>& elem, const vector<vector<int>>& v);

	//计算result=Smove（s,c）
	vector<int>& smove(int s, vector<int>& result, char c);


	//求空闭包
	vector<int>& NULLClose(vector<int>& states);


	


	//防止F0不够进行扩展
	bool extendsF0();

	//result=result+（result-c）
	vector<int>& combineVector(vector<int>& result, const vector<int>& c);


	

	
public:
	string name;
	int getF0(){
		return this->F0[0];
	}
	int getS0(){
		return this->S0;
	}

	//设置F0s，返回的是tail
	int setF0(vector<int>& F0S);
	//无表达式子构造的就是一个空串到空串的状态机
	//无参的构造函数words长度为空
	NFA(const char* name, const char* words = "\0", const char *expression = "#", int maxF0Length = 100);
	NFA(const NFA& nfa);//拷贝构造函数

	//获得图
	Graph& getG();

	//设置图，为了析构，不知道直接等于会不会直接将原来的Graph析构
	void setG(Graph& g);
	
	~NFA();//不用析构g了,g会被g的析构函数自动析构,主要是需要去析构allWords和F0
};


//DFA公有继承NFA，主要的目的是为了是程序的各个部分功能更加明确
//这个类主要完成的工作有
/*
	1.NFA到DFA的图的转化
	2.DFA的最小化
	3.DFA的模拟器
*/
class DFA:public NFA
{

private:

	int** DFAtable;//状态转化表
	int Length_of_S;//S的状态数目
	//DFA主要是在这三个函数中使用的节点
	void convertDFAG(vector<Node>& DFA);//用在构造函数中，在父类构造完后，子类调用获得DFA
	
	//将DFA化简,将简化的结果直接放到g中就行
	void simplify(vector<Node>& DFA);
	
	void setG(vector<Node>& DFA);//根据vector<Node>给G赋值,最小化后要使用的

	void setTable();//设置状态转化表

	int getCharOfWords(char a);//获得字符串在words里面的下标

	bool isInF0(int state);//判断states
public:

	

	void convertString(string& s);//将这个类转化为TXT格式

	//识别字符串，主要的对外服务
	/*这里的str不是针对一个完整的字符串，而是一个字符串流,由字符串的beginIndex位置开始,
	一直检测到不在这个DFA表达字母表(a+)中的或者接受对于某个节点没有这个字符串的边（1234，a1234）
	返回值为结束的位置下标，同时这个值会被存在result中
	*/
	bool simDFA(const char* str, int beginIndex, int& endIndex,char* result);
	//构造函数
	DFA(const char* name = "", const char* words = "\0", const char *expression = "#", int maxF0Length = 100);
	DFA(const char* name , const char* words,vector<int>& F0,int S0,int table[400][300],int Length_of_S);
	void getF0s(vector<int>& F0S);
	void getWords(vector<char>& Words);
	DFA(const DFA& dfa);
	~DFA();

};

enum CONSTRUCT_TYPE{
	PARSING_REGULAR,//这个是根据正规式来通过运算得到
	PARSING_CLASS,//这个不需要通过运算

};
//将所有的整合形成一个词法分析器
class LexicalAnalyzer
{
public:
	//注意，这里面需要读取一个文件里面的为各个定义,默认是通过正规式来解析
	LexicalAnalyzer(string path = "", CONSTRUCT_TYPE type = PARSING_REGULAR);
	~LexicalAnalyzer();

	//将xx.c文件转化为记号流，保存在xxMark.txt中
	void getMarkStream(string cpath="");

private:

	//解析正规式的构造函数选项入口

	void parsingRegular(string path);

	//解析类的类文件构造函数入口
	void parsingClass(string path);



	//分析str，同时将str分为两部分，一个部分为表达式，一个部分为名字，最后在表达式上面获得字母表
	//name=expression(包括#)，所有的char*最后都是以\0结束
	//返回为1则表示正确分析成功，返回为0则表示分析失败
	bool analyString(const char* str, char* expression, char* name, char* words);

	//将表达式展开，主要需要处理的有.显现，A1-An转化为A1|A2|A3|A4....|An
	//暂时未实现，请期待
	void extendsExpression(const char*expression,char* extendsExp);

	//在表达式上面获取words
	void analyWords(char* expression,char* words);

	vector<DFA> wordsDefine;
};



#endif