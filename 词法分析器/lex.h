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
	Graph(int maxLength=100);
	~Graph();		//������������Ҫ�ǻ���nodes��nodes��������ڴ�ռ�

};
Graph::Graph(int maxLength = 100){
	this->tail = 0;
	this->maxLength = maxLength;
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
		printf("%d�ڵ�Ļ�������\n", i);
	}
	printf("���������");

	delete this->nodes;

	printf("�ڵ�������");


}


//����NFA������ת���������������ʵ�ֵ�
class NFA
{
private:
	Graph g;//������״̬S��mov����
	int S0;//��ʼ״̬
	char* allWord;//��ĸ��
	int wordLength;//��Ӧ��ĸ������鳤��
	int* F0;//��̬
	int F0Length;//��Ӧ��̬�����鳤��
public:

	NFA(Graph& g,int S0,char* allWord,int wordLength,int* F0,int F0Length);

	~NFA();//��������g��,g�ᱻg�����������Զ�����,��Ҫ����Ҫȥ����allWords��F0
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
	printf("NFA���ճɹ�");
}

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
	bool pri[6][6];//������ŵ����ȼ�,�����������"(",")","#","|",".","*"��;

	//չ������ʽ�ӣ���Ҫ������������ʽ�ӵ�һЩ����ĺ����ַ�չ����Ϊֻ������ļ��������������Ԫ�ص��ַ���
	char* unfoldExpre(char* expression);


public:
	Operation();
	~Operation();
	NFA calute(char* expression, int expLength);//����һ������ʽ�ӣ����һ��NFA
};

Operation::Operation()
{
	this->index = 0;
}

Operation::~Operation()
{
}


#endif