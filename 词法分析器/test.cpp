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
	char* ex="(a|b)*.c.d.e#" ;

	NFA nfa("abcde",ex);
	
	//printG(nfa.getG());
	g = nfa.getG();
	/*
	cout << "S0:" << S0 << endl;

	cout << "F0:" << F0 << endl;
	*/
}

int main(){

	Graph g;

	testNFAconver(g);
	printG(g);

	cout << "ssss" << endl;

	system("pause");
}