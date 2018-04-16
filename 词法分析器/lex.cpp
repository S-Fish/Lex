#include"lex.h"
#include<stack>
using namespace std;

//Graph类，没有考虑未分配成功的情况
bool Graph::extendNodes(){
	bool result=0;
	if (this->tail >= this->maxLength){
		
		Node* newNodes;
		
		newNodes = new Node[2*this->maxLength];//扩展一下
		
		//原来数据转移一下
		for (int i = 0; i < maxLength; i++){
			newNodes[i] = this->nodes[i];
		}

		//回收原来的空间
		delete[] this->nodes;

		//重新分配一下空间
		this->nodes = newNodes;
		
		//长度改变一下
		this->maxLength = 2 * this->maxLength;

		result = 1;
	}
	return result;
}

int Graph::newNode(){
	int a = this->tail;
	this->tail++;
	extendNodes();//判断是不是需要扩展
	return a;
}

int Graph::getTail(){
	return this->tail;
}

Graph::Graph(int maxLength ){
	this->tail = 0;
	this->maxLength = maxLength;
	this->nodes = new Node[this->maxLength];
	//开始的是一个空的需要进行一些操作
	int S0 = newNode();
	int F0 = newNode();
	Arc* arc = new Arc;
	arc->changeValue = '\0';
	arc->nextArc = NULL;
	arc->nodeIndex = F0;
	this->nodes[S0].firstArc = arc;
	this->nodes[F0].firstArc = NULL;

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

	delete[] this->nodes;

	printf("节点回收完成\n");


}

void Graph::insertArc(int index,Arc* arc){
	Arc * p = nodes[index].firstArc;

	//这个节点没有边
	if (p == NULL){
		nodes[index].firstArc = arc;
		return;
	}

	while (p->nextArc!=NULL)
	{
		p = p->nextArc;
	}
	p->nextArc = arc;


}


//Operation类，主要完成正规式子到图的转化

//将s进行初始化!!!
void init0Arc(Arc* s,int nextNode){
	s->changeValue = '\0';
	s->nodeIndex = nextNode;
	s->nextArc = NULL;
}


Operation::Operation()
{
	this->index = 0;
	bool a[6][6] = { 
	{ 1, 0, 0, 1, 1, 1 },
	{ 0, 0, 0, 0, 0, 0 },
	{ 1, 0, 0, 1, 1, 1 },
	{ 1, 0, 0, 0, 1, 1 },
	{ 1, 0, 0, 1, 1, 1 },
	{ 0, 0, 0, 0, 0, 0 },
	};
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 6; j++){
			this->pri[i][j] = a[i][j];
		}
	}

	char op[7] = { '(', ')', '#', '|', '.', '*' };
	for (int i = 0; i < 6; i++){
		ops[i] = op[i];
	}
}
Operation::~Operation()
{
}


bool Operation::calute(Graph& g, char* expression, int& S0, int& F0){
	//初始化一下,初始的结果,空的表现
	S0 = 0;
	F0 = 1;
	
	//操作符栈创建
	stack<char> op;
	op.push('#');//表示开始

	//操作数的栈
	stack<MidResult> opn;
	MidResult a;
	a.S0 = S0;
	a.F0 = F0;
	opn.push(a);

	while (!op.empty())
	{
		//表示当前的下标大于expression的下标需要退出
		if (this->index >= strlen(expression)){
			return 0;
		}	
		char c = this->getNextChar(expression);//获得一个数据

		if (this->isOp(c)){//判断c是不是操作符
			//是操作符的操作
			char opa = '\0';
			opa=op.top();
			int aindex = getopIndex(opa);
			if (aindex == -1) return 0;//说明a这个操作符不存在
			int cindex = getopIndex(c);//这两个是为了查找优先级
			if (cindex == -1) return 0;//说明c这个操作符不存在

			//表示需要进行计算，以及操作符的出栈
			if (this->pri[aindex][cindex] == 0){

				MidResult a, b;

				//看看aop的是什么样的
				switch (opa)
				{
				case '('://'（’运算表示c一定是一个）这个需要出栈，并且需要将index向后‘）’给消去，index需要自增
					if (c != ')'){ return 0; }//不是‘）’是出错了
					op.pop();//将‘（’弹出
					this->index++;//后面有自减所以这里要自增
					break;
				case '#'://表示是终结，到这一步结果应该就应该已经出来了
					if (c != '#'){ return 0; }//不是‘#’是出错了
					op.pop();//将‘#’弹出，这个时候op是空的
					break;
				case '|':
					b = opn.top();
					opn.pop();
					a = opn.top();
					opn.pop();				//因为是栈所以b在前面a在后面
					this->andOp(g, a, b);
					
					opn.push(a);//计算结果再压栈一下
					op.pop();//栈中的'|'运算完成所以需要出栈
					break;
				case '*':
					a = opn.top();
					opn.pop();

					this->closureOp(g, a);
					opn.push(a);
					op.pop();//		*运算完成需要出栈
					break;

				case '.':
					b = opn.top();
					opn.pop();
					a = opn.top();
					opn.pop();				//因为是栈所以b在前面a在后面

					this->connectOp(g, a, b);//和|有点类似


					opn.push(a);//计算结果再压栈一下
					op.pop();//栈中的'.'运算完成所以需要出栈
					break;

				default:
					break;
				}

				this->index--;//下一次还要继续去这个c所以需要先自减
			}
			else//表示直接压栈就行,操作比较简单
			{
				op.push(c);
			}
		}
		else
		{
			//是操作数的操作,先将字符转化一下
			MidResult t = this->conversion(g, c);//获得一个操作数，并且将它直接压入栈中
			opn.push(t);
		}
	}
	this->index = 0;//下一次的计算需要将这个下标重置,
	S0 = opn.top().S0;
	F0 = opn.top().F0;
	return 1;
}

char Operation::getNextChar(char* expression){
	
	char c = expression[index];
	index++;
	return c;
}

bool Operation::isOp(const char a){
	
	bool result = 0;
	
	int index = this->getopIndex(a);

	if (index == -1) result = 0;
	else
	{
		result = 1;
	}

	return result;

}

int  Operation::getopIndex(char op){
	int index = -1;
	for (int i = 0; i < 7; i++){
		if (op == this->ops[i]) index = i;
	}
	return index;
}

MidResult Operation::conversion(Graph& g, char a){
	MidResult t;
	t.S0 = g.newNode();
	t.F0 = g.newNode();

	//对G进行修改
	Arc * arc;
	arc = new Arc;
	arc->nextArc = NULL;
	arc->nodeIndex = t.F0;
	arc->changeValue =a;
	g.nodes[t.S0].firstArc = arc;
	g.nodes[t.F0].firstArc = NULL;
	//----在G上面创建了一个

	return t;
}

bool Operation::andOp(Graph& g, MidResult& a, MidResult& b){
	bool result = 1;
	int S0 = g.newNode();
	int F0 = g.newNode();//分配新的节点
	
	
	//进行与操作的具体
	//S0的改变
	Arc * t;
	t = new Arc;
	init0Arc(t, a.S0);
	g.insertArc(S0, t);

	t = new Arc;
	init0Arc(t, b.S0);
	g.insertArc(S0, t);


	//F0的改变
	t = new Arc;
	init0Arc(t, F0);
	g.insertArc(a.F0, t);

	t = new Arc;
	init0Arc(t, F0);
	g.insertArc(b.F0, t);

	a.F0 = F0;
	a.S0 = S0;


	return result;

}

bool Operation::closureOp(Graph& g, MidResult& a){
	bool result = 1;
	int S0 = g.newNode();
	int F0 = g.newNode();

	//需要生成四条边
	Arc* t;
	t = new Arc;
	init0Arc(t, a.S0);
	g.insertArc(S0, t);
	
	t = new Arc;
	init0Arc(t, F0);
	g.insertArc(S0, t);

	t = new Arc;
	init0Arc(t, S0);
	g.insertArc(a.F0, t);
	
	t = new Arc;
	init0Arc(t, F0);
	g.insertArc(a.F0, t);

	//结果改变一下
	a.S0 = S0;
	a.F0 = F0;
	

	return result;
}

bool Operation::connectOp(Graph& g, MidResult& a, MidResult& b){
	bool result = 1;

	//只要创建一个新的弧就行，不需要去申请新的节点
	Arc* t;
	t = new Arc;
	init0Arc(t, b.S0);
	g.insertArc(a.F0, t);

	a.S0 = a.S0;
	a.F0 = b.F0;


	return result;

}



//NFA类
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
