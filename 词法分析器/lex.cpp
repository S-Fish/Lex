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







//Graph�࣬û�п���δ����ɹ������
bool Graph::extendNodes(){
	bool result=0;
	if (this->tail >= this->maxLength){
		
		Node* newNodes;
		
		newNodes = new Node[2*this->maxLength];//��չһ��
		
		//ԭ������ת��һ��
		for (int i = 0; i < maxLength; i++){
			newNodes[i] = this->nodes[i];
		}

		//����ԭ���Ŀռ�
		delete[] this->nodes;

		//���·���һ�¿ռ�
		this->nodes = newNodes;
		
		//���ȸı�һ��
		this->maxLength = 2 * this->maxLength;

		result = 1;
	}
	return result;
}

int Graph::newNode(){
	int a = this->tail;
	this->tail++;
	extendNodes();//�ж��ǲ�����Ҫ��չ
	return a;
}

int Graph::getTail(){
	return this->tail;
}

void Graph::clearG(){
	
	for(int a = 0; a < this->tail; a++){
		//���ԭ�����л���Ҫ�ѻ����ڴ�ռ����
		Arc* p = nodes[a].firstArc;
		//���жϽڵ���û�бߣ��б߲Ŵ���
		if (p != NULL){
			while (p->nextArc != NULL){
				Arc* temp = p->nextArc;
				delete p;
				p = temp;
			}
			delete p;//����whileѭ������һ��
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
	//�ڵ�ı���Ҫ���·���
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
	//��ʼ����һ���յ���Ҫ����һЩ����
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

	//���ձ�
	for (int i = 0; i < tail; i++){
		Node t = this->nodes[i];
		//���վ���t�ı�
		Arc* a = t.firstArc;
		//�ж�����ڵ��ǲ���û�л�
		if (a != NULL){

			//����a->next=NULL��ʱ������������Ҫ�������һ��delete
			while (a->nextArc != NULL){
				Arc* t = a->nextArc;
				delete a;
				a = t;
			}
			delete a;
		}
		//printf("%d�ڵ�Ļ�������\n", i);
	}
	//printf("���������\n");

	delete[] this->nodes;

	//printf("�ڵ�������\n");


}

void Graph::insertArc(int index,Arc* arc){
	Arc * p = nodes[index].firstArc;

	//����ڵ�û�б�
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


//Operation�࣬��Ҫ�������ʽ�ӵ�ͼ��ת��

//��s���г�ʼ��!!!
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
	//��ʼ��һ��,��ʼ�Ľ��,�յı���
	S0 = 0;
	F0 = 1;
	
	//������ջ����
	stack<char> op;
	op.push('#');//��ʾ��ʼ

	//��������ջ
	stack<MidResult> opn;
	MidResult a;
	a.S0 = S0;
	a.F0 = F0;
	opn.push(a);

	while (!op.empty())
	{
		//��ʾ��ǰ���±����expression���±���Ҫ�˳�
		if (this->index >= strlen(expression)){
			return 0;
		}	
		char c = this->getNextChar(expression);//���һ������
		//if (c == '@');//c��һ��ת���ַ������һ���ַ�һ���ǲ���������
		if (c!='@'&&this->isOp(c)){//�ж�c�ǲ��ǲ�����
			//�ǲ������Ĳ���
			char opa = '\0';
			opa=op.top();
			int aindex = getopIndex(opa);
			if (aindex == -1) return 0;//˵��a���������������
			int cindex = getopIndex(c);//��������Ϊ�˲������ȼ�
			if (cindex == -1) return 0;//˵��c���������������

			//��ʾ��Ҫ���м��㣬�Լ��������ĳ�ջ
			if (this->pri[aindex][cindex] == 0){

				MidResult a, b;

				//����aop����ʲô����
				switch (opa)
				{
				case '('://'���������ʾcһ����һ���������Ҫ��ջ��������Ҫ��index��󡮣�������ȥ��index��Ҫ����
					if (c != ')'){ return 0; }//���ǡ������ǳ�����
					op.pop();//������������
					this->index++;//�������Լ���������Ҫ����
					break;
				case '#'://��ʾ���սᣬ����һ�����Ӧ�þ�Ӧ���Ѿ�������
					if (c != '#'){ return 0; }//���ǡ�#���ǳ�����
					op.pop();//����#�����������ʱ��op�ǿյ�
					break;
				case '|':
					b = opn.top();
					opn.pop();
					a = opn.top();
					opn.pop();				//��Ϊ��ջ����b��ǰ��a�ں���
					this->andOp(g, a, b);
					
					opn.push(a);//��������ѹջһ��
					op.pop();//ջ�е�'|'�������������Ҫ��ջ
					break;
				case '*':
					a = opn.top();
					opn.pop();

					this->closureOp(g, a);
					opn.push(a);
					op.pop();//		*���������Ҫ��ջ
					break;

				case '.':
					b = opn.top();
					opn.pop();
					a = opn.top();
					opn.pop();				//��Ϊ��ջ����b��ǰ��a�ں���

					this->connectOp(g, a, b);//��|�е�����


					opn.push(a);//��������ѹջһ��
					op.pop();//ջ�е�'.'�������������Ҫ��ջ
					break;

				default:
					break;
				}

				this->index--;//��һ�λ�Ҫ����ȥ���c������Ҫ���Լ�
			}
			else//��ʾֱ��ѹջ����,�����Ƚϼ�
			{
				op.push(c);
			}
		}
		else
		{
			//�ǲ������Ĳ���,�Ƚ��ַ�ת��һ��

			//�����ǲ���ת���ַ�������Ҫ�����ת��һ��
			if (c == '@') c = this->getNextChar(expression);
			MidResult t = this->conversion(g, c);//���һ�������������ҽ���ֱ��ѹ��ջ��
			opn.push(t);
		}
	}
	this->index = 0;//��һ�εļ�����Ҫ������±�����,
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

	//��G�����޸�
	Arc * arc;
	arc = new Arc;
	arc->nextArc = NULL;
	arc->nodeIndex = t.F0;
	arc->changeValue =a;
	g.nodes[t.S0].firstArc = arc;
	g.nodes[t.F0].firstArc = NULL;
	//----��G���洴����һ��

	return t;
}

bool Operation::andOp(Graph& g, MidResult& a, MidResult& b){
	bool result = 1;
	int S0 = g.newNode();
	int F0 = g.newNode();//�����µĽڵ�
	
	
	//����������ľ���
	//S0�ĸı�
	Arc * t;
	t = new Arc;
	init0Arc(t, a.S0);
	g.insertArc(S0, t);

	t = new Arc;
	init0Arc(t, b.S0);
	g.insertArc(S0, t);


	//F0�ĸı�
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

	//��Ҫ����������
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

	//����ı�һ��
	a.S0 = S0;
	a.F0 = F0;
	

	return result;
}

bool Operation::connectOp(Graph& g, MidResult& a, MidResult& b){
	bool result = 1;

	//ֻҪ����һ���µĻ����У�����Ҫȥ�����µĽڵ�
	Arc* t;
	t = new Arc;
	init0Arc(t, b.S0);
	g.insertArc(a.F0, t);

	a.S0 = a.S0;
	a.F0 = b.F0;


	return result;

}



//NFA��
NFA::NFA(const char* name, const char* words, const char *expression, int maxF0Length) {
	
	this->F0 = new int[maxF0Length];
	Operation op;
	op.calute(this->g, expression, this->S0, this->F0[0]);//����G
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
	//printf("NFA���ճɹ�\n");
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

//result=result+��result-c��
vector<int>&  NFA::combineVector(vector<int>& result, const vector<int>& c){
	
	for (int i = 0; i < c.size(); i++){

		//i��C�в���result����Ҫ��ѹ��
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

	//����ת��
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

	vector<vector<int>> states;//����������״̬���������������
	
	//����S0;
	vector<int> t;
	t.push_back(S0);
	NULLClose(t);
	states.push_back(t);
	
	Node s0;
	s0.firstArc = NULL;
	DFA.push_back(s0);

	//��states����һ�飬���size�᲻�ϵ�����
	for (int i = 0; i < states.size(); i++){

		cout << i<<endl;//�ȴ�ʱ�䣬��Ҫ�Ƿ�ֹ��ȵĲ��ͷ�
		const vector<int> lastS = states.at(i);//��һ��״̬����
		
		for (int j = 0; j < this->wordLength; j++){
			
			//��dfag��mov������mov(i,words[j])
			vector<int> result;
			for (int index = 0; index < lastS.size(); index++){
				vector<int> t;
				smove(lastS.at(index),t, this->words[j]);
				combineVector(result, t);
			}
			NULLClose(result);
			/*
				1. ���ж�result�ǲ��ǿյģ��ǿյ�����治����
				2. �ҵ�result��states��λ�ã�λ�ô�t�У�
				3. result�������states�����ʾ�����״̬���states�У�λ�ô�t�У�
				4. ����t= mov(i,words[j]),��������뵽dfag��ȥ
			*/
			if (result.size() != 0){
				int insertIndex = elemInVector(result, states);
				if (insertIndex == -1){
					insertIndex = states.size();
					states.push_back(result);

					Node node;
					DFA.push_back(node);
					//dfag.newNode();//��dfag����newһ���ڵ㣬�ڵ���Ҫ��result��Ӧ��λ����ͬ
				}

				//����insertIndex=mov��i��words[j]);
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


//ʶ���ַ�������Ҫ�Ķ������
/*�����str�������һ���������ַ���������һ���ַ�����,���ַ�����beginIndexλ�ÿ�ʼ,
һֱ��⵽�������DFA�����ĸ��(a+)�еĻ��߽��ܶ���ĳ���ڵ�û������ַ����ıߣ�1234��a1234��
����ֵbool,Ϊ1���ʾ���DFA���ܣ�Ϊ0���ʾ���DFA�����ܣ�
ͬʱ���ֵ�ᱻ����result��
*/
bool DFA::simDFA(const char* str, int beginIndex,int& endIndex ,char* result){
	
	endIndex = beginIndex;
	//result = "";
	int i = 0;
	int charIndex = getCharOfWords(str[endIndex]);
	int state = this->S0;
	int charlen = strlen(str);
	while (	endIndex!=charlen&&	//����ַ����ڽ�β
			charIndex!=-1&&    //�����ʾ����������ַ���
			this->DFAtable[state][charIndex]!=-1)//���״̬û����������ַ�
	{
		state = DFAtable[state][charIndex];
		result[i] = str[endIndex];
		i++;
		endIndex++;
		charIndex = getCharOfWords(str[endIndex]);
	}
	result[i] = '\0';

	//���DFA������������ܾ�
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
//��DFA����,���򻯵Ľ��ֱ�ӷŵ�g�о���
void NFA::simplify(vector<Node>& DFA){
	vector<vector<int>> states;//��Ż��ֵ�
	vector<int> benginA;		//��ȥ��̬��
	vector<int> benginB;		//������̬��
	for (int i = 0; i < tail; i++){
		benginB.push_back(this->F0[i]);	//��̬�������
	}
	for (int i = 0; i < DFA.size(); i++){
		if (elemInVector(i, benginB) == -1){
			benginA.push_back(i);
		}
	}
	states.push_back(benginA);
	states.push_back(benginB);
	//---------��ʼ���Ĺ����ͷ������-------//






}


void getFinalyStates(vector<vector<int>>& states){
	int length = states.size();

	//��ÿһ��states���б���
	for (int i = 0; i < length; i++){
		//���з���
		vector<vector<int>> tmp;
		//��ӵ�states��β������
		for (int j = 0; j < tmp.size(); j++){
			states.push_back(tmp.size);
		}
	}
	//��ǰ���statesɾ����

	//�ж���û���µĶ������֣�������򳤶�һ���ı���
	if (states.size() != length){
		getFinalyStates(states);
	}


}
*/



//LexicalAnalyzer


LexicalAnalyzer::~LexicalAnalyzer(){}
bool LexicalAnalyzer::analyString(const char* str, char* expression, char* name, char* words){
	
	unsigned indexE=0;//�Ⱥŵ��±�
	while (indexE < strlen(str) && str[indexE] != '='){
		indexE++;
	}
	//˵�����ʽ��û�еȺţ��������ʽ�Ӳ����Ϲ淶
	if (indexE == 0||indexE==strlen(str)) return 0;

	//��������ַ���
	for (int i = 0; i < indexE; i++){
		name[i] = str[i];
	}
	name[indexE] = '\0';

	//��ñ��ʽ�ַ���
	int exIndex = 0;
	for (int i = indexE + 1; i < strlen(str); i++){
		expression[exIndex] = str[i];
		exIndex++;
	}
	expression[exIndex] = '\0';

	//��չ�������ʽ����ʱ��û��ʵ��
	extendsExpression(expression, expression);

	//�����ĸ��
	analyWords(expression, words);

	return 1;

 }


//length���ȵ��ַ�����array������û��c,�з���1û�з���0
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
	//ע���
	//1.ת���ַ�@��ע���
	//2.�������| . * ( )����;
	int exIndex = 0;//ָ����ʽ��
	int wordIndex = 0;//��ʾ�����ж��ٵ�word��
	char op[] = { '|', '.', '*', '(', ')', '#' };
	while (exIndex < strlen(expression)){
		
		char c = expression[exIndex];
		
		//�������op��������ĸ
		if (!isInArray(op, 6, c)){
			//ת���ַ�����
			if (c == '@'){
				exIndex++;
				c = expression[exIndex];
			}
			//�ж�c�ڲ�����ĸ����
			if (!isInArray(words, wordIndex, c)){
				//������ĸ����ӵ���ĸ����
				words[wordIndex] = c;
				wordIndex++;
			}

		}
	
		exIndex++;
	}
	//������ʶһ��
	words[wordIndex] = '\0';

}

void LexicalAnalyzer::extendsExpression(const char*expression, char* extendsExp){}




//���캯�����Թ����ļ����ݵĸ�ʽҪ��ÿһ���������Ҫ�û��н�����
LexicalAnalyzer::LexicalAnalyzer(string path){
	//һ��һ�еĹ���
	ifstream inFile(path.c_str());
	string regular;
	char expression[500];//���ʽ��ʱ���λ��
	char name[100];		//���ִ��
	char words[400];	//��ĸ����
	if (!inFile.is_open()){ 
		cout << path<<"open fail"<<endl;

		return; 
	}
	int line = 1;

	while (getline(inFile,regular)){
		if (analyString(regular.c_str(), expression, name, words)){
			DFA tmp(name, words, expression);

			//��֪�����ֱ�ӵ��ÿ���������ͬʱ��һ��whileѭ������ʱ������tmp����������
			this->wordsDefine.push_back(tmp);
		
		}
		else
		{

			cout <<line<<"line: "<< regular <<"����������ʽ�淶"<< endl;
		}
		line++;
	}

	inFile.close();
}

//��array��Ѱ�����
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


//�ж�c�ǲ�����a-z,A-Z,0-9֮�е��ַ���,�ھͷ���1�������򷵻�0����Ҫ��Ϊ�˴�����
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

//����ӿ�
void LexicalAnalyzer::getMarkStream(string path){
	//һ��һ�еķ���
	//���ļ�
	ifstream inread(path);
	if (!inread.is_open()){
		cout << path << " open fail" << endl;

		return;
	}
	int line = 1;//����������,ͬʱҲ��¼λ��
	int beginIndex;
	int* endIndex;
	endIndex = new int[this->wordsDefine.size()]();

	char** value;
	value = new char*[this->wordsDefine.size()];
	for (int i = 0; i < wordsDefine.size(); i++){
		value[i] = new char[300];
	}


	string lines;
	


	//���ÿһ�н��з���
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

			
			//�����ո���Ʊ���ţ���Ϊ�ؼ��ֺ�ID�����־��ǿ��ո���ʵ�ֵ�������Ҫ���ÿո�������
			while (code[endIndex[nextBegin]] == ' ' || code[endIndex[nextBegin]] == '\t'){
				endIndex[nextBegin]++;
			}

			//����һ���޷�ʶ����ַ������ᱻ����
			if (endIndex[nextBegin] == beginIndex){
				
				cout << "error: line " <<line<<" "<<code[endIndex[nextBegin]] ;
				endIndex[nextBegin]++;//ƫ��һ��λ��
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