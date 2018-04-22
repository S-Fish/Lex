#include"lex.h"
#include<iostream>




//正则表达需要输入完毕需要用#表示输入完毕

//当使用含有特殊字符作为元素表的元素时候需要使用转义字符@

//输入的正规式子应该是这样 name=【...定义】#
//初级阶段不使用简化的形式，而且a.b的.不能省略
//将来有时间和精力，在简化部分加上一些
//元素表的来源是根据正规式中元素来的
using namespace std;


//测试第一个转化器
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

//测试词法分析器
void testLexcial(){
	LexicalAnalyzer l("test.txt");
	l.getMarkStream("s.txt");//对于12345A会识别为两个一个是数字<number,12345> 一个是<id,A>;
}

int main(){

	//Graph g;

	//testNFAconver(g);

	testLexcial();
	cout << "ssss" << endl;

	system("pause");
}