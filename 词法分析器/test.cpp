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
	LexicalAnalyzer l("classFile\\testclass.txt", PARSING_CLASS);
	l.getMarkStream("s.txt");//对于12345A会识别为两个一个是数字<number,12345> 一个是<id,A>;
}



//调试作用
int main(){
	testLexcial();
	system("pause");
	return 0;
}









//第一个参数是DFA文件，可以是Class文件也可以是正规式子
//第二个是源程序文件
//第三个是模式
	/*模式有三种：
	  1.compileRegularToClass:编译正规式文件生成对应的Class文件
	  2.compileRegularToMarkFlow:编译正规式文件生成对应的Class文件，并且将给的.c文件转化为转化为记号流
	  3.LoadClassToMarkFlow:加载Class文件，然后将.c的源程序转化为记号流
	*/

//输入文件有三类
/*
	1.正规式子文件
	2.正规式的Class文件
	3.源文件
*/

//输出文件有两类
/*
	1.记号流文件;//保存在markStream目录中
	2.正规式的Class文件;//保存在classFile目录中
*/
int finallymain(int argc, char *argv[]){


	if (argc != 4) return 1;//参数不对

	string normalFile = argv[1];//正规式文件
	string sourceFile = argv[2];//源程序文件
	string model = argv[3];		//模式

	if (strcmp(model.c_str(),"compileRegularToClass")==0){
		LexicalAnalyzer lex(normalFile,PARSING_REGULAR);//通过正规来判断
		return 0;
	}
	if (strcmp(model.c_str(), "compileRegularToMarkFlow") == 0){
		LexicalAnalyzer lex(normalFile, PARSING_REGULAR);//通过正规来判断
		lex.getMarkStream(sourceFile);
		return 0;
	}
	if (strcmp(model.c_str(), "LoadClassToMarkFlow") == 0){
		LexicalAnalyzer lex(normalFile, PARSING_REGULAR);//通过Class来判断
		lex.getMarkStream(sourceFile);
		return 0;
	}

	//system("pause");
	return 0;
}