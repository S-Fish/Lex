#include"lex.h"
#include<iostream>




//��������Ҫ���������Ҫ��#��ʾ�������
using namespace std;

void printG(Graph& g){

	for (int i = 0; i < g.getTail(); i++){
		cout << "Node:"<<i<<endl<<"Arc: ";
		

		Arc* t = g.nodes[i].firstArc;
		while (t != NULL){
			if (t->changeValue == '\0') cout << "@ ";
			else
			{
				cout << t->changeValue << " ";
			}
			t = t->nextArc;
		}

		cout << endl;
	
	}

}

//���Ե�һ��ת����
void testNFAconver(Graph& g){
	char* ex="(a|b)*.a.b.b#" ;

	NFA nfa("ab",ex);
	//vector<Node> DFA;
	//nfa.convertDFAG(DFA);
	
	//printG(nfa.getG());
	//g = nfa.getG();
	printG(nfa.getG());
	
	
}

int main(){

	Graph g;

	testNFAconver(g);


	cout << "ssss" << endl;

	system("pause");
}