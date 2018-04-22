#include"lex.h"
#include<stack>
#include<iostream>
#include<fstream>

using namespace std;

void init0Arc(Arc* s, int nextNode, char change = '\0');





void printG(Graph& g){

	for (int i = 0; i < g.getTail(); i++){
		cout << "Node:" << i << endl << "Arc: ";


		Arc* t = g.nodes[i].firstArc;
		while (t != NULL){
			if (t->changeValue == '\0') cout << "@ "<< t->nodeIndex << " ";
			else
			{
				cout << t->changeValue << " " << t->nodeIndex << " ";
			}
			t = t->nextArc;
		}

		cout << endl;

	}

}







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

void Graph::clearG(){
	
	for(int a = 0; a < this->tail; a++){
		//如果原来的有弧，要把弧的内存空间回收
		Arc* p = nodes[a].firstArc;
		//先判断节点有没有边，有边才处理
		if (p != NULL){
			while (p->nextArc != NULL){
				Arc* temp = p->nextArc;
				delete p;
				p = temp;
			}
			delete p;//跳出while循环还差一个
		}
		nodes[a].firstArc = NULL;
	}
	this->tail = 0;
}

Graph::Graph(const Graph& g){
	this->CopyGraph(g);
}


void Graph::CopyGraph(const Graph& g){
	this->tail = g.tail;
	this->maxLength = g.maxLength;
	this->nodes = new Node[g.maxLength];
	for (int i = 0; i < g.tail; i++){
		this->nodes[i] = g.nodes[i];
		this->nodes[i].firstArc = NULL;
	}
	//节点的边需要重新分配
	for (int i = 0; i < g.tail; i++){
		Arc * p = g.nodes[i].firstArc;
		while (p != NULL){

			Arc* t = new Arc;
			init0Arc(t, p->nodeIndex, p->changeValue);
			this->insertArc(i, t);
			p = p->nextArc;
		}
	}
}

Graph::Graph(int maxLength ){
	this->tail = 0;
	this->maxLength = maxLength;
	this->nodes = new Node[this->maxLength];
	//开始的是一个空的需要进行一些操作
	int S0 = newNode();
	int F0 = newNode();
	/*Arc* arc = new Arc;
	arc->changeValue = '\0';
	arc->nextArc = NULL;
	arc->nodeIndex = F0;
	*/
	this->nodes[S0].firstArc = NULL;
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
		//printf("%d节点的弧被回收\n", i);
	}
	//printf("弧回收完成\n");

	delete[] this->nodes;

	//printf("节点回收完成\n");


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
void init0Arc(Arc* s,int nextNode,char change){
	s->changeValue = change;
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
	{ 1, 0, 0, 0, 0, 1 },
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


bool Operation::calute(Graph& g, const char* expression, int& S0, int& F0){
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
		//if (c == '@');//c是一个转义字符下面的一个字符一定是操作数！！
		if (c!='@'&&this->isOp(c)){//判断c是不是操作符
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

			//看看是不是转义字符是则需要将其给转义一下
			if (c == '@') c = this->getNextChar(expression);
			MidResult t = this->conversion(g, c);//获得一个操作数，并且将它直接压入栈中
			opn.push(t);
		}
	}
	this->index = 0;//下一次的计算需要将这个下标重置,
	S0 = opn.top().S0;
	F0 = opn.top().F0;
	return 1;
}

char Operation::getNextChar(const char* expression){
	
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
NFA::NFA(const char* name, const char* words, const char *expression, int maxF0Length) {
	
	this->F0 = new int[maxF0Length];
	Operation op;
	op.calute(this->g, expression, this->S0, this->F0[0]);//计算G
	this->tail = 1;
	this->maxF0Length = maxF0Length;
	this->wordLength = strlen(words);
	
	this->words = new char[strlen(words)];
	
	for (unsigned i = 0; i < strlen(words); i++){
		this->words[i] = words[i];
	}
	this->name = name;


}
Graph& NFA::getG(){
	return this->g;
}
NFA::~NFA(){
	delete[] this->F0;
	delete[] this->words;
	//printf("NFA回收成功\n");
}

NFA::NFA(const NFA& nfa){
	
	this->g.CopyGraph(nfa.g);
	this->maxF0Length = nfa.maxF0Length;
	this->name = string(nfa.name);
	this->S0 = nfa.S0;
	this->tail = nfa.tail;
	this->wordLength = nfa.wordLength;

	this->F0 = new int[this->maxF0Length];
	for (int i = 0; i < this->tail; i++){
		this->F0[i] = nfa.F0[i];
	}

	this->words = new char[nfa.wordLength];
	for (int i = 0; i < this->wordLength; i++){
		this->words[i] = nfa.words[i];
	}

}

int NFA::elemInVector(int elem, const vector<int>& v){
	int index = -1;

	for (int i = 0; i < v.size(); i++){
		if (elem == v.at(i)) {
			index = i;
			break;
		}
	}

	return index;
}

int NFA::elemInVector(const vector<int>& elem, const vector<vector<int>>& v){
	int index = -1;

	for (int i = 0; i < v.size(); i++){
		bool flag = 1;
		for (int j = 0; j < elem.size(); j++){
			if (elemInVector(elem.at(j), v.at(i)) == -1){
				flag = 0;
				break;
			}
		}
		if (flag == 1){
			index = i;
			break;
		}	
	}

	return index;
}

vector<int>& NFA::smove(int s, vector<int>& result, char c){
	Node node = g.nodes[s];
	Arc* p = node.firstArc;
	while (p!=NULL)
	{
		if (p->changeValue == c){
			result.push_back(p->nodeIndex);
		}
		p = p->nextArc;
	}

	return result;
}

vector<int>& NFA::NULLClose(vector<int>& states){

	for (int i = 0; i < states.size(); i++){
		
		Arc* p = g.nodes[states.at(i)].firstArc;

		while (p != NULL){
			if (p->changeValue == '\0'){
				if (elemInVector(p->nodeIndex, states) == -1){
					states.push_back(p->nodeIndex);
				}
			}
		
			p = p->nextArc;
		}
	
	}
	return states;

}

//result=result+（result-c）
vector<int>&  NFA::combineVector(vector<int>& result, const vector<int>& c){
	
	for (int i = 0; i < c.size(); i++){

		//i在C中不在result中需要被压入
		if (elemInVector(c.at(i), result) == -1){
		
			result.push_back(c.at(i));
			
		}
	}
	return result;
}


int NFA::setF0(vector<int>& F0S){
	
	
	for (this->tail = 0; this->tail < F0S.size(); this->tail++){
		extendsF0();
		F0[tail] = F0S.at(tail);
	}
	
	return this->tail;

}

bool NFA::extendsF0(){
	bool result = 0;

	//进行转移
	if (this->tail >= this->maxF0Length){
		
		int * t = new int[maxF0Length * 2];

		for (int i = 0; i < this->tail; i++){
			t[i] = this->F0[i];
		}

		delete[] this->F0;
		this->F0 = t;
		this->maxF0Length = maxF0Length * 2;
		result = 1;

	}

	return result;
}







//DFA
void DFA::convertDFAG(vector<Node>& DFA){

	vector<vector<int>> states;//后来的所有状态集都会在这个里面
	
	//计算S0;
	vector<int> t;
	t.push_back(S0);
	NULLClose(t);
	states.push_back(t);
	
	Node s0;
	s0.firstArc = NULL;
	DFA.push_back(s0);

	//将states遍历一遍，这个size会不断的增加
	for (int i = 0; i < states.size(); i++){

		cout << i<<endl;//等待时间，主要是防止你等的不耐烦
		const vector<int> lastS = states.at(i);//上一个状态集合
		
		for (int j = 0; j < this->wordLength; j++){
			
			//求dfag的mov函数，mov(i,words[j])
			vector<int> result;
			for (int index = 0; index < lastS.size(); index++){
				vector<int> t;
				smove(lastS.at(index),t, this->words[j]);
				combineVector(result, t);
			}
			NULLClose(result);
			/*
				1. 先判断result是不是空的，是空的则后面不用做
				2. 找到result在states的位置（位置存t中）
				3. result如果不在states中则表示将这个状态添加states中（位置存t中）
				4. 存在t= mov(i,words[j]),将这个加入到dfag中去
			*/
			if (result.size() != 0){
				int insertIndex = elemInVector(result, states);
				if (insertIndex == -1){
					insertIndex = states.size();
					states.push_back(result);

					Node node;
					DFA.push_back(node);
					//dfag.newNode();//在dfag上面new一个节点，节点需要好result对应的位置相同
				}

				//出现insertIndex=mov（i，words[j]);
				Arc* arc = new Arc;
				init0Arc(arc, insertIndex, words[j]);

				Arc* p = DFA.at(i).firstArc;
				if (p == NULL){
					DFA.at(i).firstArc = arc;
					continue;
				}

				while (p->nextArc != NULL)
				{
					p = p->nextArc;
				}
				p->nextArc = arc;
				
			}
		}
			
	}

	this->S0 = 0;
	vector<int> F0temp;
	for (int i = 0; i < states.size(); i++){
		
		for (int j = 0; j < tail; j++){
			
			if (elemInVector(F0[j], states.at(i)) != -1){
				F0temp.push_back(i);
			}
		}
	
	}

	setF0(F0temp);
	
	
}

void DFA::setG(vector<Node>& DFA){
	g.clearG();
	int index = 0;
	for (int i = 0; i < DFA.size(); i++){
		index = g.newNode();
		g.nodes[index] = DFA[i];
	}

}

void DFA::simplify(vector<Node>& DFA){
}

int DFA::getCharOfWords(char a){
	int index = -1;

	for (int i = 0; i < this->wordLength; i++){
		if (a == this->words[i]) { 
			index = i;
			break;
		}
	}
	return index;
}

void DFA::setTable(){
	this->DFAtable = new int*[g.getTail()];
	for (int i = 0; i < g.getTail(); i++){
		this->DFAtable[i] = new int[this->wordLength];
		fill(this->DFAtable[i], this->DFAtable[i] + this->wordLength, -1);
	}

	for (int i = 0; i < g.getTail(); i++){
		Arc* p = g.nodes[i].firstArc;
		while (p!=NULL)
		{
			int index = getCharOfWords(p->changeValue);
			this->DFAtable[i][index] = p->nodeIndex;
			p = p->nextArc;
		}	
	}

}


//识别字符串，主要的对外服务
/*这里的str不是针对一个完整的字符串，而是一个字符串流,由字符串的beginIndex位置开始,
一直检测到不在这个DFA表达字母表(a+)中的或者接受对于某个节点没有这个字符串的边（1234，a1234）
返回值bool,为1则表示这个DFA接受，为0则表示这个DFA不接受，
同时这个值会被存在result中
*/
bool DFA::simDFA(const char* str, int beginIndex,int& endIndex ,char* result){
	
	endIndex = beginIndex;
	//result = "";
	int i = 0;
	int charIndex = getCharOfWords(str[endIndex]);
	int state = this->S0;
	int charlen = strlen(str);
	while (	endIndex!=charlen&&	//这个字符串在结尾
			charIndex!=-1&&    //这个表示不存在这个字符串
			this->DFAtable[state][charIndex]!=-1)//这个状态没法接受这个字符
	{
		state = DFAtable[state][charIndex];
		result[i] = str[endIndex];
		i++;
		endIndex++;
		charIndex = getCharOfWords(str[endIndex]);
	}
	result[i] = '\0';

	//这个DFA不接受这个，拒绝
	if (isInF0(state) == 0){
		endIndex = beginIndex;
	}
	return isInF0(state);
	
}

bool DFA::isInF0(int state){
	bool result = 0;
	for (int i = 0; i < this->tail; i++){
	
		if (F0[i] == state){
			result = 1;
			break;
		}
	}
	return result;
}


DFA::DFA(const char* name, const char* words, const char *expression, const int maxF0Length) :NFA(name, words, expression, maxF0Length){
	
	//printG(this->g);
	vector<Node> t;
	this->convertDFAG(t);
	this->simplify(t);
	this->setG(t);
	this->setTable();

	
	/*
	for (int i = 0; i < this->g.getTail(); i++){
		for (int j = 0; j < this->wordLength; j++){
			cout << this->DFAtable[i][j]<<" ";
		}
		cout << endl;
	}
	*/
}

DFA::~DFA(){
	
	for (int i = 0; i < g.getTail(); i++){
		delete[] this->DFAtable[i];
	}
	delete[] this->DFAtable;

}
DFA::DFA(const DFA& dfa) :NFA(dfa){

	vector<Node> t;
	this->convertDFAG(t);
	this->simplify(t);
	this->setG(t);
	this->setTable();
	//cout << "copy DFA\n";
}

/*
//将DFA化简,将简化的结果直接放到g中就行
void NFA::simplify(vector<Node>& DFA){
	vector<vector<int>> states;//存放划分的
	vector<int> benginA;		//除去终态的
	vector<int> benginB;		//所有终态的
	for (int i = 0; i < tail; i++){
		benginB.push_back(this->F0[i]);	//终态核算完毕
	}
	for (int i = 0; i < DFA.size(); i++){
		if (elemInVector(i, benginB) == -1){
			benginA.push_back(i);
		}
	}
	states.push_back(benginA);
	states.push_back(benginB);
	//---------初始化的工作和分组完成-------//






}


void getFinalyStates(vector<vector<int>>& states){
	int length = states.size();

	//对每一个states进行遍历
	for (int i = 0; i < length; i++){
		//进行分组
		vector<vector<int>> tmp;
		//添加到states的尾巴上面
		for (int j = 0; j < tmp.size(); j++){
			states.push_back(tmp.size);
		}
	}
	//将前面的states删除掉

	//判断有没有新的东西出现，如果有则长度一定改变了
	if (states.size() != length){
		getFinalyStates(states);
	}


}
*/



//LexicalAnalyzer


LexicalAnalyzer::~LexicalAnalyzer(){}
bool LexicalAnalyzer::analyString(const char* str, char* expression, char* name, char* words){
	
	unsigned indexE=0;//等号的下标
	while (indexE < strlen(str) && str[indexE] != '='){
		indexE++;
	}
	//说明这个式子没有等号，这个正规式子不符合规范
	if (indexE == 0||indexE==strlen(str)) return 0;

	//获得名字字符串
	for (int i = 0; i < indexE; i++){
		name[i] = str[i];
	}
	name[indexE] = '\0';

	//获得表达式字符串
	int exIndex = 0;
	for (int i = indexE + 1; i < strlen(str); i++){
		expression[exIndex] = str[i];
		exIndex++;
	}
	expression[exIndex] = '\0';

	//扩展解析表达式，暂时还没有实现
	extendsExpression(expression, expression);

	//获得字母表
	analyWords(expression, words);

	return 1;

 }


//length长度的字符数组array里面有没有c,有返回1没有返回0
bool isInArray(char* array,int length, char c){
	bool result = 0;

	for (int i = 0; i < length; i++){
		if (c == array[i]){
			result = 1;
			break;
		}
	}

	return result;
}

void LexicalAnalyzer::analyWords(char* expression, char* words){
	//注意点
	//1.转义字符@的注意点
	//2.运算符号| . * ( )区别;
	int exIndex = 0;//指向表达式的
	int wordIndex = 0;//表示现在有多少的word了
	char op[] = { '|', '.', '*', '(', ')', '#' };
	while (exIndex < strlen(expression)){
		
		char c = expression[exIndex];
		
		//如果不在op中则是字母
		if (!isInArray(op, 6, c)){
			//转义字符处理
			if (c == '@'){
				exIndex++;
				c = expression[exIndex];
			}
			//判断c在不在字母表中
			if (!isInArray(words, wordIndex, c)){
				//不在字母就添加到字母表中
				words[wordIndex] = c;
				wordIndex++;
			}

		}
	
		exIndex++;
	}
	//结束标识一下
	words[wordIndex] = '\0';

}

void LexicalAnalyzer::extendsExpression(const char*expression, char* extendsExp){}




//构造函数，对构造文件内容的格式要求每一个定义结束要用换行结束，
LexicalAnalyzer::LexicalAnalyzer(string path){
	//一行一行的构造
	ifstream inFile(path.c_str());
	string regular;
	char expression[500];//表达式临时存放位置
	char name[100];		//名字存放
	char words[400];	//字母表存放
	if (!inFile.is_open()){ 
		cout << path<<"open fail"<<endl;

		return; 
	}
	int line = 1;

	while (getline(inFile,regular)){
		if (analyString(regular.c_str(), expression, name, words)){
			DFA tmp(name, words, expression);

			//不知道会会直接调用拷贝函数，同时在一个while循环结束时候会调用tmp的析构函数
			this->wordsDefine.push_back(tmp);
		
		}
		else
		{

			cout <<line<<"line: "<< regular <<"不符合正规式规范"<< endl;
		}
		line++;
	}

	inFile.close();
}

//在array中寻找最大
int findMaxIndex(int* array,int length){
	int index = 0;
	int max = array[0];
	for (int i = 1; i < length; i++){
		if (max < array[i]){
			index = i;
			max = array[i];
		}
	}
	return index;
}


//判断c是不是在a-z,A-Z,0-9之中的字符串,在就返回1，不在则返回0，主要是为了错误处理
bool isInIDE(char c){
	//char * s = "abcdefghigklmnopqrstuvwxyzABCDEFGHIGKLMNOPQRSTUVWXYZ0123456789";
	bool result = 1;

	//
	if (c < '0') result = 0;

	if (c > '9'&&c < 'A') result = 0;

	if (c > 'Z'&&c < 'a') result = 0;

	if (c > 'z') result = 0;
	
	return result;
}

//对外接口
void LexicalAnalyzer::getMarkStream(string path){
	//一行一行的分析
	//读文件
	ifstream inread(path);
	if (!inread.is_open()){
		cout << path << " open fail" << endl;

		return;
	}
	int line = 1;//错误标记作用,同时也记录位置
	int beginIndex;
	int* endIndex;
	endIndex = new int[this->wordsDefine.size()]();

	char** value;
	value = new char*[this->wordsDefine.size()];
	for (int i = 0; i < wordsDefine.size(); i++){
		value[i] = new char[300];
	}


	string lines;
	


	//针对每一行进行分析
	while (getline(inread,lines))
	{
		const char* code = lines.c_str();
		beginIndex = 0;

		while (beginIndex<strlen(code))
		{
			for (unsigned i = 0; i < wordsDefine.size(); i++){
				wordsDefine[i].simDFA(code, beginIndex, endIndex[i], value[i]);
			}
			int nextBegin = findMaxIndex(endIndex, this->wordsDefine.size());
			if (endIndex[nextBegin] != beginIndex)cout << "(line: " << line << ")" << "<" << wordsDefine.at(nextBegin).name << "," << value[nextBegin] << ")\n";

			
			//除掉空格和制表符号，因为关键字和ID的区分就是靠空格来实现的所有需要利用空格来区分
			while (code[endIndex[nextBegin]] == ' ' || code[endIndex[nextBegin]] == '\t'){
				endIndex[nextBegin]++;
			}

			//出现一个无法识别的字符，将会被报错
			if (endIndex[nextBegin] == beginIndex){
				
				cout << "error: line " <<line<<" "<<code[endIndex[nextBegin]] ;
				endIndex[nextBegin]++;//偏移一个位置
				while (isInIDE(code[endIndex[nextBegin]]))
				{
					cout << code[endIndex[nextBegin]];
					endIndex[nextBegin]++;
				}
				cout << endl;
			}
			
			beginIndex = endIndex[nextBegin];
			
		}


		line++;
	}


	for (int i = 0; i < this->wordsDefine.size(); i++){
		delete[] value[i];
	}
	delete[] value;
	delete[] endIndex;
	inread.close();
}