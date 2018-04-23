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
	LexicalAnalyzer l("classFile\\testclass.txt", PARSING_CLASS);
	l.getMarkStream("s.txt");//����12345A��ʶ��Ϊ����һ��������<number,12345> һ����<id,A>;
}



//��������
int main(){
	testLexcial();
	system("pause");
	return 0;
}









//��һ��������DFA�ļ���������Class�ļ�Ҳ����������ʽ��
//�ڶ�����Դ�����ļ�
//��������ģʽ
	/*ģʽ�����֣�
	  1.compileRegularToClass:��������ʽ�ļ����ɶ�Ӧ��Class�ļ�
	  2.compileRegularToMarkFlow:��������ʽ�ļ����ɶ�Ӧ��Class�ļ������ҽ�����.c�ļ�ת��Ϊת��Ϊ�Ǻ���
	  3.LoadClassToMarkFlow:����Class�ļ���Ȼ��.c��Դ����ת��Ϊ�Ǻ���
	*/

//�����ļ�������
/*
	1.����ʽ���ļ�
	2.����ʽ��Class�ļ�
	3.Դ�ļ�
*/

//����ļ�������
/*
	1.�Ǻ����ļ�;//������markStreamĿ¼��
	2.����ʽ��Class�ļ�;//������classFileĿ¼��
*/
int finallymain(int argc, char *argv[]){


	if (argc != 4) return 1;//��������

	string normalFile = argv[1];//����ʽ�ļ�
	string sourceFile = argv[2];//Դ�����ļ�
	string model = argv[3];		//ģʽ

	if (strcmp(model.c_str(),"compileRegularToClass")==0){
		LexicalAnalyzer lex(normalFile,PARSING_REGULAR);//ͨ���������ж�
		return 0;
	}
	if (strcmp(model.c_str(), "compileRegularToMarkFlow") == 0){
		LexicalAnalyzer lex(normalFile, PARSING_REGULAR);//ͨ���������ж�
		lex.getMarkStream(sourceFile);
		return 0;
	}
	if (strcmp(model.c_str(), "LoadClassToMarkFlow") == 0){
		LexicalAnalyzer lex(normalFile, PARSING_REGULAR);//ͨ��Class���ж�
		lex.getMarkStream(sourceFile);
		return 0;
	}

	//system("pause");
	return 0;
}