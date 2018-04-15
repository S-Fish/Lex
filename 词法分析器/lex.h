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
	Graph(int maxLength=100);
	~Graph();		//析构函数，主要是回收nodes和nodes的链表的内存空间

};
Graph::Graph(int maxLength = 100){
	this->tail = 0;
	this->maxLength = maxLength;
}
Graph::~Graph(){
	
	//回收边
	for (int i = 0; i < tail; i++){
		Node t = this->nodes[i];
		//回收具体t的边
		Arc* a = t.firstArc;
		//判断这个节点是不是没有弧
		if (a != NULL){
			
			//会在a->next=NULL的时候跳出所以需要额外进行一次delete
			while (a->nextArc != NULL){
				Arc* t = a->nextArc;
				delete a;
				a = t;
			}
			delete a;
		}
		printf("%d节点的弧被回收\n", i);
	}
	printf("弧回收完成");

	delete this->nodes;

	printf("节点回收完成");


}


//考虑NFA，几个转化都是在这个类中实现的
class NFA
{
private:
	Graph g;//包括了状态S和mov函数
	int S0;//初始状态
	char* allWord;//字母表
	int wordLength;//对应字母表的数组长度
	int* F0;//终态
	int F0Length;//对应终态的数组长度
public:

	NFA(Graph& g,int S0,char* allWord,int wordLength,int* F0,int F0Length);

	~NFA();//不用析构g了,g会被g的析构函数自动析构,主要是需要去析构allWords和F0
};
NFA::NFA(Graph& g, int S0, char* allWord, int wordLength, int* F0, int F0Length){
	this->allWord = allWord;
	this->g = g;
	this->S0 = S0;
	this->wordLength = wordLength;
	this->F0Length = F0Length;
	this->allWord = new char[this->wordLength];
	for (int i = 0; i < wordLength; i++){
		this->allWord[i] = allWord[i];
	}


	this->F0 = new int[this->F0Length];
	for (int i = 0; i < F0Length; i++){
		this->F0[i] = F0[i];
	}
}
NFA::~NFA(){
	delete this->F0;
	delete this->allWord;
	printf("NFA回收成功");
}

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
	bool pri[6][6];//运算符号的优先级,六个运算符（"(",")","#","|",".","*"）;

	//展开运算式子，主要是用来将正规式子的一些特殊的含义字符展开，为只有上面的几个运算符合运算元素的字符串
	char* unfoldExpre(char* expression);


public:
	Operation();
	~Operation();
	NFA calute(char* expression, int expLength);//计算一个正规式子，输出一个NFA
};

Operation::Operation()
{
	this->index = 0;
}

Operation::~Operation()
{
}


#endif