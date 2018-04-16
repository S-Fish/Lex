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
void testNFAconver(){
	char* ex="(a|b)*.c.d.e#" ;

	Operation op;

	Graph g(12);

	int S0=0, F0=0;

	op.calute(g, ex, S0, F0);
	
	printG(g);
	/*
	cout << "S0:" << S0 << endl;

	cout << "F0:" << F0 << endl;
	*/
}

int main(){

	testNFAconver();

	system("pause");
}