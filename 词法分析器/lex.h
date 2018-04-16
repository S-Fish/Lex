#ifndef LEX_H_
#define LEX_H_

#include<string>


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
	Node* nodes;//各个节点，采用的数组形式
	int newNode();//获得最最后一个节点的下一个，相当于分配
	bool extendNodes();//扩展nodes数组
	void insertArc(int index,Arc* a);//给index这个节点插入一条弧
	int getTail();
	Graph(int maxLength=100);
	~Graph();		//析构函数，主要是回收nodes和nodes的链表的内存空间

};


//考虑NFA，几个转化都是在这个类中实现的
class NFA
{
private:
	Graph g;//包括了状态S和mov函数
	int S0;//初始状态
	char* words;//字母表
	int wordLength;//对应字母表的数组长度
	int* F0;//终态
	int tail;//F0的实际个数
	int maxF0Length;//对应终态的数组长度
public:

	//无表达式子构造的就是一个空串到空串的状态机
	//无参的构造函数words长度为空
	NFA(char* words="\0",char *expression="#",int maxF0Length=100);
	
	//获得图
	Graph& getG();

	
	~NFA();//不用析构g了,g会被g的析构函数自动析构,主要是需要去析构allWords和F0
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
	int index=0;//解析到表达式的位置;
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

	char getNextChar(char* expression);//针对于正规式，和算术表达式不一样，对于正规是操作数和操作符都只会占一个字符位置

	bool isOp(const char a);//判断a是不是操作符，如果是则返回1不是则返回0

	MidResult conversion(Graph& g, char a);//将字符串转化一下，同时也需要在图上给他分配空间

public:
	Operation();
	~Operation();
	//计算一个正规式子，输出一个NFA,计算没有出错会返回1出错会返回0,同时也需要给出S0和F0，采用这种方法构造的终态只有一个
	bool calute(Graph& g,char* expression,int& S0,int& F0);
};



#endif