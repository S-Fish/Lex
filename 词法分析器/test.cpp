#include"lex.h"
#include<iostream>




//��������Ҫ���������Ҫ��#��ʾ�������

//��ʹ�ú��������ַ���ΪԪ�ر��Ԫ��ʱ����Ҫʹ��ת���ַ�@

//���������ʽ��Ӧ�������� name=��...���塿#
//�����׶β�ʹ�ü򻯵���ʽ������a.b��.����ʡ��
//������ʱ��;������ڼ򻯲��ּ���һЩ
//Ԫ�ر����Դ�Ǹ�������ʽ��Ԫ������
using namespace std;


//���Ե�һ��ת����
void testNFAconver(Graph& g){
	char* ex="(@||@*)*.@|.@*.@*#" ;

	DFA dfa("key","|*",ex);
	//vector<Node> DFA;
	//nfa.convertDFAG(DFA);
	
	//printG(nfa.getG());
	//g = nfa.getG();
	//printG(dfa.getG());
	
	char* sss = "||*|*t|**;";
	int length = strlen(sss);
	int benginIndex = 0;
	while (benginIndex<length)
	{
		int endlIndex = 0;
		char u[100]="";
		bool sb = dfa.simDFA(sss, benginIndex, endlIndex, u);
		cout <<sb<<" "<< u<<" "<<endlIndex<<endl;
		if (sb == 0){
			cout <<"eorr"<<sss[endlIndex]<<endl;
			endlIndex++;
		}
		benginIndex = endlIndex;

	}
	
	

}

//���Դʷ�������
void testLexcial(){
	LexicalAnalyzer l("test.txt");
	l.getMarkStream("s.txt");//����12345A��ʶ��Ϊ����һ��������<number,12345> һ����<id,A>;
}

int main(){

	//Graph g;

	//testNFAconver(g);

	testLexcial();
	cout << "ssss" << endl;

	system("pause");
}