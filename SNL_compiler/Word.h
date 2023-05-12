#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<algorithm>
#include<cstring>
#include <stdlib.h>
#include<stdio.h>
#include <cstdio>
#include <stack>
#include <fstream>
#include<iomanip>
using namespace std;
typedef pair<int,pair<string, string>> PSS;//存储单个Token 
bool is_num(char ch) {
	if (ch >= '0' && ch <= '9')return true;
	return false;
}
bool is_letter(char ch) {
	if (ch >= 'a' && ch <= 'z')return true;
	if (ch >= 'A' && ch <= 'Z')return true;
	return false;
}
bool is_other(char ch) {
	if (is_num(ch) || is_letter(ch))return false;
	return true;
}
bool is_delimiter(char ch) {
	char word[] = { '+','-','*','/','(',')',';','[',']','=','<' };
	for (auto x : word) {
		if (x == ch)return true;
	}
	return false;
}
bool judge(char ch) {
	char word[] = { '+','-','*','/','(',')',';','[',']','=','<',':','{','.',',',' ','\n','\t'};
	for (auto x : word) {
		if (x == ch)return true;
	}
	return false;
}
void printToken(PSS& token1) {
	int line = token1.first;
	auto item = token1.second;
	//cout << setw(15) <<"行数" << setw(15) << "" << setw(15) << "" << endl;
	cout <<setw(15)<<line <<setw(15) << item.first <<setw(15)<<item.second<< endl;

}//显示一个单词 
bool  reservedLookup(string s) {
	string Keyword[] = { "program","procedure","type","var","then","else","fi","do",
"endwh","array","of","record","write","return","integer","char","if","EOF","read","while","end","begin"};
	for (auto x : Keyword) {
		if (x == s)return true;
	}
	return false;
};//查找保留字
bool getTokenlist(FILE* fp,vector<PSS> &token)//取得当前行所有的token序列
{	
	char cp = fgetc(fp);
	int line = 1;
	while (cp!=EOF){
		while (cp == ' ' || cp == '\t' || cp == '\n')
		{
			if (cp == '\n')line++;
			cp = fgetc(fp);
		}//去掉所有的空格，制表符，换行符
		if (!is_letter(cp) && !is_num(cp) && !judge(cp)&&cp!=EOF) {
			cout<< line << ":错误，无法以'"<<cp<<"'字符开头，请规范代码" <<"\n"<< endl;

			return false;
		}
		if (is_num(cp)) {//数字
			string val;
			//val.push_back(cp);
			while (is_num(cp)) {
				val.push_back(cp);
				cp = fgetc(fp);

			}
			ungetc(cp, fp);
			cp = val.back();
			PSS ans = { line,{ "无符号整数",val } };
			//printToken(ans);
			token.push_back(ans);
		}
		if (is_letter(cp)) {//字母
			string val;
			while (is_letter(cp) || is_num(cp)) {
				val.push_back(cp);
				cp = fgetc(fp);
			}
			ungetc(cp, fp);
			PSS ans;
			if (reservedLookup(val)) {
				ans = { line,{ "保留字",val } };
			}
			else {
				ans = { line,{ "标识符",val } };
			}
			
			//printToken(ans);
			token.push_back(ans);
			cp = val.back();
		}
			if (is_delimiter(cp)) {//分界符
				string tmp;
				tmp += cp;
				PSS a_token = { line,{ "单分界符",tmp } };
				//printToken(a_token);
				token.push_back(a_token);
			}
			if (cp == ':') {//:=双分界符
				string tmp;
				tmp += cp;
				cp = fgetc(fp);
				if (cp == '=') {
					tmp += cp;
					PSS to = { line,{ "双分界符",tmp } };
					//printToken(to);
					token.push_back(to);
				}
				else {
					ungetc(cp, fp);
					cp = ':';
					cout << line <<":错误,你是不是忘写了个等号呀，兄弟"<< endl;
					return false;
				}
			}
			if (cp == '{') {//注释
				cp = fgetc(fp);
				while (cp != '}') {
					if (cp == '\n')line++;
					cp = fgetc(fp);
					
				}
				if (cp != '}')return false;
				PSS to = { line,{ "注释","{}"} };
				//printToken(to);
				token.push_back(to);
			}
			if (cp == '.')//数组下标
			{
				cp = fgetc(fp);
				if (cp == '.') {
					PSS to = { line,{ "数组下标",".." } };
					//printToken(to);
					token.push_back(to);
				}
				else {
					PSS to = { line,{ "程序结束标志","."} };
					if (cp != ' '&&cp!='\n'&&cp!=EOF) {
						cout << line << ":错误，兄弟，你数组下标是不是忘了一个点呀" << endl;
						return false;
					}
					else {
						ungetc(cp, fp);
						cp = '.';
						//printToken(to);
						token.push_back(to);
					}
				}
			}
			if (cp == ',') {//字符标志状态
				cp = fgetc(fp);
				if (is_num(cp) || is_letter(cp)) {
					ungetc(cp, fp);
					cp = ',';
					PSS to = { line,{ "字符标志状态","," } };
					//printToken(to);
					token.push_back(to);
				}
				else {
					ungetc(cp, fp);
					cp = ',';
					cout << line <<":错误，','是字符标志状态，后面应该是字母或数字"<< endl;
					return false;
				}
			}
			
			cp = fgetc(fp);
		}
	return true;
}
void printTokenlist(vector<PSS>& to) {
	cout << setw(15) << "行数" << setw(15) << "词法信息" << setw(15) << "语义信息" << endl;
	for (auto a = to.begin(); a != to.end(); a++) {
		printToken(*a);
	}
}//显示词法分析的结果

vector<PSS> cffx() {
	vector<PSS> token;
	FILE* fp;
	if ((fp=fopen("input.txt", "r")) == NULL)
	{
		printf("文件打开失败\n");
		exit(1);
	}
	bool flag = getTokenlist(fp,token);
	fclose(fp);
	if(flag==false){
		cout << "词法分析失败，程序编写有错误，请重新编程" << endl;
        exit(1);
	}
	else {
		printTokenlist(token);
	}
	return token;
}