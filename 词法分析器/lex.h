#ifndef LEX_H_
#define LEX_H_

#include<string>
#include<vector>
using namespace std;


//ͼ��ת�ƣ���Ҫ������ʾ
struct Arc{
	int nodeIndex;//��һ���ڵ���±꣬�ڵ���õ������飬���Բ���������ʽ
	char changeValue;//�仯��ֵ"\0"��ʾΪ��ֵ
	Arc* nextArc = NULL;//��ʾ��һ��
};



//ͼ�Ľڵ㣬�����ʾһ����״̬
struct  Node
{
	std::string value;//״̬��ֵ�����ǲ����������Ѱ�ҽڵ�
	Arc* firstArc=NULL;//�����ĳ��ȵıߣ����õ�������ʽ
}; 

//����class��Ϊ���ڴ�Ĺ����򣬿���ʵ���Զ�������������
class Graph
{
private:
	int maxLength;//nodes����󳤶ȣ�node����new�ķ�ʽ������������ʱ�������������ʽ
	int tail;	//���ڽڵ��õ���λ��
public:
	void CopyGraph(const Graph& g);//�������캯��
	void clearG();
	Node* nodes;//�����ڵ㣬���õ�������ʽ
	int newNode();//��������һ���ڵ����һ�����൱�ڷ���
	bool extendNodes();//��չnodes����
	void insertArc(int index,Arc* a);//��index����ڵ����һ����
	int getTail();
	Graph(int maxLength=100);
	Graph(const Graph& g);//�������캯��
	~Graph();		//������������Ҫ�ǻ���nodes��nodes��������ڴ�ռ�

};
//ջ���м���
struct MidResult
{
	int S0;
	int F0;
};


//����д��һ������
class Operation
{
private:
	int index = 0;//���������ʽ��λ��;

	//������ŵ����ȼ�,�����������"(",")","#","|",".","*"��,pri[i][j],��ʾi�����ȼ��ǲ��Ǵ���j��i��j�����
	//1��ʾ����Ҫ������ֱ����ջ��0��ʾ��Ҫ���в���
	bool pri[6][6];
	char ops[7];
	//չ������ʽ�ӣ���Ҫ������������ʽ�ӵ�һЩ����ĺ����ַ�չ����Ϊֻ������ļ��������������Ԫ�ص��ַ���
	char* unfoldExpre(char* expression);


	int getopIndex(char op);//�ҵ�op���±�

	bool connectOp(Graph& g, MidResult& a, MidResult& b);//�������㡣,a=a��b

	bool andOp(Graph& g, MidResult& a, MidResult& b);//������|,a=a|b

	bool closureOp(Graph& g, MidResult& a);//�հ�����, a=a*

	char getNextChar(const char* expression);//���������ʽ�����������ʽ��һ�������������ǲ������Ͳ�������ֻ��ռһ���ַ�λ��

	bool isOp(const char a);//�ж�a�ǲ��ǲ�������������򷵻�1�����򷵻�0

	MidResult conversion(Graph& g, char a);//���ַ���ת��һ�£�ͬʱҲ��Ҫ��ͼ�ϸ�������ռ�

public:
	Operation();
	~Operation();
	//����һ������ʽ�ӣ����һ��NFA,����û�г���᷵��1����᷵��0,ͬʱҲ��Ҫ����S0��F0���������ַ����������ֻ̬��һ��
	bool calute(Graph& g, const char* expression, int& S0, int& F0);
};


//����NFA������ת���������������ʵ�ֵ�
class NFA
{
protected:


	Graph g;//������״̬S��mov����
	int S0;//��ʼ״̬
	char* words;//��ĸ��
	int wordLength;//��Ӧ��ĸ������鳤��
	int* F0;//��̬
	int tail;//F0��ʵ�ʸ���
	int maxF0Length;//��Ӧ��̬�����鳤��


	//�ж�elem�ǲ�����v�У����򷵻���v�е��±꣬���򷵻�-1
	int elemInVector(int elem, const vector<int>& v);
	int elemInVector(const vector<int>& elem, const vector<vector<int>>& v);

	//����result=Smove��s,c��
	vector<int>& smove(int s, vector<int>& result, char c);


	//��ձհ�
	vector<int>& NULLClose(vector<int>& states);


	//����F0s�����ص���tail
	int setF0(vector<int>& F0S);


	//��ֹF0����������չ
	bool extendsF0();

	//result=result+��result-c��
	vector<int>& combineVector(vector<int>& result, const vector<int>& c);


	

	
public:
	string name;
	int getF0(){
		return this->F0[0];
	}
	int getS0(){
		return this->S0;
	}


	//�ޱ��ʽ�ӹ���ľ���һ���մ����մ���״̬��
	//�޲εĹ��캯��words����Ϊ��
	NFA(const char* name, const char* words = "\0", const char *expression = "#", int maxF0Length = 100);
	NFA(const NFA& nfa);//�������캯��

	//���ͼ
	Graph& getG();

	//����ͼ��Ϊ����������֪��ֱ�ӵ��ڻ᲻��ֱ�ӽ�ԭ����Graph����
	void setG(Graph& g);
	
	~NFA();//��������g��,g�ᱻg�����������Զ�����,��Ҫ����Ҫȥ����allWords��F0
};


//DFA���м̳�NFA����Ҫ��Ŀ����Ϊ���ǳ���ĸ������ֹ��ܸ�����ȷ
//�������Ҫ��ɵĹ�����
/*
	1.NFA��DFA��ͼ��ת��
	2.DFA����С��
	3.DFA��ģ����
*/
class DFA:public NFA
{

private:
	int** DFAtable;//״̬ת����

	//DFA��Ҫ����������������ʹ�õĽڵ�
	void convertDFAG(vector<Node>& DFA);//���ڹ��캯���У��ڸ��๹�����������û��DFA
	
	//��DFA����,���򻯵Ľ��ֱ�ӷŵ�g�о���
	void simplify(vector<Node>& DFA);
	
	void setG(vector<Node>& DFA);//����vector<Node>��G��ֵ,��С����Ҫʹ�õ�

	void setTable();//����״̬ת����

	int getCharOfWords(char a);//����ַ�����words������±�

	bool isInF0(int state);//�ж�states
public:

	//ʶ���ַ�������Ҫ�Ķ������
	/*�����str�������һ���������ַ���������һ���ַ�����,���ַ�����beginIndexλ�ÿ�ʼ,
	һֱ��⵽�������DFA�����ĸ��(a+)�еĻ��߽��ܶ���ĳ���ڵ�û������ַ����ıߣ�1234��a1234��
	����ֵΪ������λ���±꣬ͬʱ���ֵ�ᱻ����result��
	*/
	bool simDFA(const char* str, int beginIndex, int& endIndex,char* result);
	//���캯��
	DFA(const char* name = "", const char* words = "\0", const char *expression = "#", int maxF0Length = 100);
	DFA(const DFA& dfa);
	~DFA();

};


//�����е������γ�һ���ʷ�������
class LexicalAnalyzer
{
public:
	//ע�⣬��������Ҫ��ȡһ���ļ������Ϊ��������
	LexicalAnalyzer(string path="");
	~LexicalAnalyzer();

	//��xx.c�ļ�ת��Ϊ�Ǻ�����������xxMark.txt��
	void getMarkStream(string cpath="");

private:
	//����str��ͬʱ��str��Ϊ�����֣�һ������Ϊ���ʽ��һ������Ϊ���֣�����ڱ��ʽ��������ĸ��
	//name=expression(����#)�����е�char*�������\0����
	//����Ϊ1���ʾ��ȷ�����ɹ�������Ϊ0���ʾ����ʧ��
	bool analyString(const char* str, char* expression, char* name, char* words);

	//�����ʽչ������Ҫ��Ҫ�������.���֣�A1-Anת��ΪA1|A2|A3|A4....|An
	//��ʱδʵ�֣����ڴ�
	void extendsExpression(const char*expression,char* extendsExp);

	//�ڱ��ʽ�����ȡwords
	void analyWords(char* expression,char* words);

	vector<DFA> wordsDefine;
};



#endif