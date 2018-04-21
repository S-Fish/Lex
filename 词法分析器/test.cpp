#include"lex.h"
#include<iostream>




//正则表达需要输入完毕需要用#表示输入完毕
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

//测试第一个转化器
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