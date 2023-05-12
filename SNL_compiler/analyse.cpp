#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include<vector>
#include "Word.h"
#include "LL1parsing.h"
#include "REparsing.h"
using namespace std;



int main() {
    // 将所有token转换为tokenList（token序列的另一种表示）
    vector<Token> tokenList = tokenToTokenlist(cffx());
    cout << "共计" << tokenList.size() << "个有效token" << endl;

    // 用哪个就把那个取消注释，把另一个注释掉
    
    // 实例化LL1语法分析器
    // LL1parsinger* analyser = new LL1parsinger(tokenList);

    // 实例化递归下降语法分析器
    RDparsinger *analyser = new RDparsinger(tokenList);

    // 用语法分析器构造语法树
    Tree *tree = new Tree(analyser->analyse());
    tree->print();


    cout << endl << "分析完成" << endl;
}
