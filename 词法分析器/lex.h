#ifndef LEX_H_
#define LEX_H_

#include<string>


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
	Node* nodes;//�����ڵ㣬���õ�������ʽ
	int newNode();//��������һ���ڵ����һ�����൱�ڷ���
	bool extendNodes();//��չnodes����
	void insertArc(int index,Arc* a);//��index����ڵ����һ����
	int getTail();
	Graph(int maxLength=100);
	~Graph();		//������������Ҫ�ǻ���nodes��nodes��������ڴ�ռ�

};


//����NFA������ת���������������ʵ�ֵ�
class NFA
{
private:
	Graph g;//������״̬S��mov����
	int S0;//��ʼ״̬
	char* words;//��ĸ��
	int wordLength;//��Ӧ��ĸ������鳤��
	int* F0;//��̬
	int tail;//F0��ʵ�ʸ���
	int maxF0Length;//��Ӧ��̬�����鳤��
public:

	//�ޱ��ʽ�ӹ���ľ���һ���մ����մ���״̬��
	//�޲εĹ��캯��words����Ϊ��
	NFA(char* words="\0",char *expression="#",int maxF0Length=100);
	
	//���ͼ
	Graph& getG();

	
	~NFA();//��������g��,g�ᱻg�����������Զ�����,��Ҫ����Ҫȥ����allWords��F0
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
	int index=0;//���������ʽ��λ��;
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

	char getNextChar(char* expression);//���������ʽ�����������ʽ��һ�������������ǲ������Ͳ�������ֻ��ռһ���ַ�λ��

	bool isOp(const char a);//�ж�a�ǲ��ǲ�������������򷵻�1�����򷵻�0

	MidResult conversion(Graph& g, char a);//���ַ���ת��һ�£�ͬʱҲ��Ҫ��ͼ�ϸ�������ռ�

public:
	Operation();
	~Operation();
	//����һ������ʽ�ӣ����һ��NFA,����û�г���᷵��1����᷵��0,ͬʱҲ��Ҫ����S0��F0���������ַ����������ֻ̬��һ��
	bool calute(Graph& g,char* expression,int& S0,int& F0);
};



#endif