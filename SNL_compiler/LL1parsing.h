#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#ifndef PS_DEC
#define PS_DEC
#include "PasingDec.h"
#endif
#define OUT_FLAG 0 // 是否输出分析过程信息
using namespace std;

struct Symbol
{
    int kind; // 0非终极符, 1终极符, 2弹栈, -1error
    NonTerminal n;
    Terminal t;
    TreeNode *father;
};

class LL1parsinger
{
private:
    // toke序列：词法分析得到的token
    vector<Token> tokenList;
    Token *currentToken;
    int INDEX = 0;
    // 符号栈：待认养孩子的符号
    vector<Symbol> symbolStack;
    // 结点栈：栈顶为当前应插入子节点的结点
    vector<TreeNode *> nodeStack;
    // LL1分析矩阵
    vector<vector<vector<Symbol> *>> LL1matrix;
    // 语法分析树根节点
    TreeNode *root = NULL;
    // 初始化LL1分析表
    void initLL1();
    // 读取token序列的第一个token为currentToken
    void readToken();
    // 处理当前符号栈栈顶， 终极符/非终极符/弹栈标志
    void popSymbolStack();
    // 将当前 Token序列 和 符号栈 中的第一个字符进行匹配，添加子节点
    void matchToken(Symbol t);
    // 根据当前Token序列最前Token堆导非终极符，并创建新节点、压栈，添加子节点
    void infer(NonTerminal n, Terminal t);
    // 弹出结点栈栈顶
    void popNodeStack();

public:
    // 将Token序列作为参数传递
    LL1parsinger(vector<Token> list);
    // 进行语法分析
    TreeNode *analyse();
};

LL1parsinger::LL1parsinger(vector<Token> list)
{
    tokenList.assign(list.begin(), list.end());
}

TreeNode *LL1parsinger::analyse()
{

    initLL1();

    // 创建根节点
    root = new TreeNode("Program");
    nodeStack.push_back(root);

    Symbol s = {0, Program, BEGIN};
    symbolStack.push_back(s);

    while (!symbolStack.empty())
    {
        popSymbolStack();
    }
    // 分析成功后不应有剩余Token，且结点栈中仅剩根节点
    readToken();
    if (OUT_FLAG)
        cout << "分析到 " << currentToken->line << " 行 " << getString(currentToken->wd.tok) << endl;
    if (OUT_FLAG)
        cout << "语法分析结束" << endl;
    if (currentToken->wd.str == "." && INDEX == (int)tokenList.size())
    {
        return nodeStack[0];
    }
    else
    {
        readToken();
        if (OUT_FLAG)
            cout << "程序有误 " << currentToken->line << " 行 " << currentToken->wd.str << "后为多余程序" << endl;
    }
    exit(1);
}

void LL1parsinger::popSymbolStack()
{
    Symbol temp = symbolStack[symbolStack.size() - 1];
    symbolStack.pop_back();
    if (OUT_FLAG)
        cout << endl
             << "处理栈顶 ";
    if (temp.kind == 0)
    { // 非终极符号
        if (OUT_FLAG)
            cout << "非终极符 " << getString(temp.n) << endl;
        infer(temp.n, tokenList[INDEX].wd.tok);
    }
    else if (temp.kind == 1)
    { // 终极符
        if (OUT_FLAG)
            cout << "终极符 " << getString(temp.t) << endl;
        matchToken(temp);
    }
    else if (temp.kind == 2)
    { // 弹结点栈
        if (OUT_FLAG)
            cout << "弹栈标志" << endl;
        if (OUT_FLAG)
            cout << "弹出 " << nodeStack[nodeStack.size() - 1]->name << endl;
        popNodeStack();
    }
}

void LL1parsinger::readToken()
{
    currentToken = &tokenList[INDEX++];
}

void LL1parsinger::matchToken(Symbol t)
{
    // 空字符不读Token，也会导致当前栈顶的父亲不认养自己
    if (t.t == NONE)
    {
        return;
    }

    readToken();
    if (OUT_FLAG)
        cout << "读到 " << currentToken->wd.str << " " << getString(currentToken->wd.tok) << endl;

    if (currentToken->wd.tok == t.t)
    {
        // 匹配则创建新结点，由于是终结符，不压入符号栈
        TreeNode *temp = new TreeNode(currentToken->wd.str);
        temp->tk = currentToken;
        temp->father = t.father;
        temp->father->addChild(temp);
        // nodeStack[nodeStack.size() - 1]->addChild(temp);
        if (OUT_FLAG)
            cout << t.father->name << "认养了" << temp->name << endl;
    }
    else
    {
        if (OUT_FLAG)
            cout << currentToken->line << "行 " << currentToken->wd.str << " 处应为" << getString(t.t) << endl;
        exit(1);
    }
}

void LL1parsinger::infer(NonTerminal n, Terminal t)
{

    // 在符号栈中压一个弹栈标志
    Symbol p;
    p.kind = 2;
    symbolStack.push_back(p);
    // cout << "压入弹栈标志" << endl;

    // 按照分析矩阵进行推导
    vector<Symbol> vs = *LL1matrix[n][t];
    if (OUT_FLAG)
        cout << "推导出" << endl;

    TreeNode *father = nodeStack[nodeStack.size() - 1]; // 保存当前结点栈栈顶，方便其儿子任阳
    for (int i = 0; i != (int)vs.size(); i++)
    {
        if (vs[i].kind == -1)
        { // 分析矩阵此处为错误标志
            cout << "程序有误 : " << getString(n) << " 的Predict集无 " << getString(t) << " " << tokenList[INDEX].wd.str << endl;
            // readToken();
            // cout << currentToken->line << " " << currentToken->wd.str << endl;
            // cout << "符号栈的第一个Token为 " << symbolStack[symbolStack.size() - 1].kind << endl;
            cout << "程序有误 " << currentToken->line << " 行 " << currentToken->wd.str << "后出现错误" << endl;
            exit(1);
        }
        if (vs[i].kind == 0)
        { // 非终极符
            if (OUT_FLAG)
                cout << " " << getString(vs[i].n);
            // 创建新结点，并压入结点栈
            TreeNode *temp = new TreeNode(getString(vs[i].n));
            temp->father = father;
            nodeStack.push_back(temp);
            if (OUT_FLAG)
                cout << " 创建了" << temp->name << "结点 其父亲是" << father->name << endl;
        }
        if (vs[i].kind == 1)
        { // 终极符
            if (OUT_FLAG)
                cout << " " << getString(vs[i].t);
        }
        // 不管是终极符还是非终极符都压入符号栈
        symbolStack.push_back(vs[i]);
        symbolStack[symbolStack.size() - 1].father = father;
    }
    if (OUT_FLAG)
        cout << endl;
}

void LL1parsinger::popNodeStack()
{
    // 根节点不应被弹出
    if (nodeStack.size() == 1)
    {
        return;
    }
    // 如果一个结点有孩子则让父亲认养自己
    if (OUT_FLAG)
        cout << nodeStack[nodeStack.size() - 1]->name << "找爸爸" << nodeStack[nodeStack.size() - 1]->father->name << endl;
    if (nodeStack[nodeStack.size() - 1]->haveChile())
    {
        if (OUT_FLAG)
            cout << nodeStack[nodeStack.size() - 1]->father->name << "认养了" << nodeStack[nodeStack.size() - 1]->name << endl;
        nodeStack[nodeStack.size() - 1]->father->addChild(nodeStack[nodeStack.size() - 1]);
    }
    else
    {
        if (OUT_FLAG)
            cout << nodeStack[nodeStack.size() - 1]->father->name << "不认养" << nodeStack[nodeStack.size() - 1]->name << endl;
    }
    nodeStack.pop_back();
}

void LL1parsinger::initLL1()
{
    // 重复利用的变量
    vector<Symbol> *temp = new vector<Symbol>;
    Symbol s;

    // 填入错误标志
    s = {-1, nullNonTerminal, nullTerminal};
    temp->push_back(s);
    for (int i = 0; i < 100; i++)
    {
        vector<vector<Symbol> *> it;
        for (int j = 0; j < 100; j++)
        {
            it.push_back(temp);
        }
        LL1matrix.push_back(it);
    }

    // 1、按照一个产生式逆序生成vector<Symbol>
    // 2、填入LL1matrix[产生式左部][predict]
    // 以下为程序自动生成--------------------------------------------

    temp = new vector<Symbol>;temp->push_back({0, ProgramBody, nullTerminal});temp->push_back({0, DeclarePart, nullTerminal});temp->push_back({0, ProgramHead, nullTerminal});LL1matrix[Program][PROGRAM] = temp;
temp = new vector<Symbol>;temp->push_back({0, ProgramName, nullTerminal});temp->push_back({1, nullNonTerminal, PROGRAM});LL1matrix[ProgramHead][PROGRAM] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, ID});LL1matrix[ProgramName][ID] = temp;
temp = new vector<Symbol>;temp->push_back({0, ProcDecpart, nullTerminal});temp->push_back({0, VarDecpart, nullTerminal});temp->push_back({0, TypeDecpart, nullTerminal});LL1matrix[DeclarePart][TYPE] = temp;LL1matrix[DeclarePart][VAR] = temp;LL1matrix[DeclarePart][BEGIN] = temp;LL1matrix[DeclarePart][PROCEDURE] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[TypeDecpart][VAR] = temp;LL1matrix[TypeDecpart][BEGIN] = temp;LL1matrix[TypeDecpart][PROCEDURE] = temp;
temp = new vector<Symbol>;temp->push_back({0, TypeDec, nullTerminal});LL1matrix[TypeDecpart][TYPE] = temp;
temp = new vector<Symbol>;temp->push_back({0, TypeDecList, nullTerminal});temp->push_back({1, nullNonTerminal, TYPE});LL1matrix[TypeDec][TYPE] = temp;
temp = new vector<Symbol>;temp->push_back({0, TypeDecMore, nullTerminal});temp->push_back({1, nullNonTerminal, SEMI});temp->push_back({0, TypeDef, nullTerminal});temp->push_back({1, nullNonTerminal, EQ});temp->push_back({0, TypeId, nullTerminal});LL1matrix[TypeDecList][ID] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[TypeDecMore][VAR] = temp;LL1matrix[TypeDecMore][BEGIN] = temp;LL1matrix[TypeDecMore][PROCEDURE] = temp;
temp = new vector<Symbol>;temp->push_back({0, TypeDecList, nullTerminal});LL1matrix[TypeDecMore][ID] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, ID});LL1matrix[TypeId][ID] = temp;
temp = new vector<Symbol>;temp->push_back({0, BaseType, nullTerminal});LL1matrix[TypeDef][CHAR] = temp;LL1matrix[TypeDef][INTEGER] = temp;
temp = new vector<Symbol>;temp->push_back({0, StructureType, nullTerminal});LL1matrix[TypeDef][RECORD] = temp;LL1matrix[TypeDef][ARRAY] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, ID});LL1matrix[TypeDef][ID] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, INTEGER});LL1matrix[BaseType][INTEGER] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, CHAR});LL1matrix[BaseType][CHAR] = temp;
temp = new vector<Symbol>;temp->push_back({0, ArrayType, nullTerminal});LL1matrix[StructureType][ARRAY] = temp;
temp = new vector<Symbol>;temp->push_back({0, RecType, nullTerminal});LL1matrix[StructureType][RECORD] = temp;
temp = new vector<Symbol>;temp->push_back({0, BaseType, nullTerminal});temp->push_back({1, nullNonTerminal, OF});temp->push_back({1, nullNonTerminal, RMIDPAREN});temp->push_back({0, Top, nullTerminal});temp->push_back({1, nullNonTerminal, UNDERANGE});temp->push_back({0, Low, nullTerminal});temp->push_back({1, nullNonTerminal, LMIDPAREN});temp->push_back({1, nullNonTerminal, ARRAY});LL1matrix[ArrayType][ARRAY] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, INTC});LL1matrix[Low][INTC] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, INTC});LL1matrix[Top][INTC] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, END});temp->push_back({0, FieldDecList, nullTerminal});temp->push_back({1, nullNonTerminal, RECORD});LL1matrix[RecType][RECORD] = temp;
temp = new vector<Symbol>;temp->push_back({0, FieldDecMore, nullTerminal});temp->push_back({1, nullNonTerminal, SEMI});temp->push_back({0, IdList, nullTerminal});temp->push_back({0, BaseType, nullTerminal});LL1matrix[FieldDecList][CHAR] = temp;LL1matrix[FieldDecList][INTEGER] = temp;
temp = new vector<Symbol>;temp->push_back({0, FieldDecMore, nullTerminal});temp->push_back({1, nullNonTerminal, SEMI});temp->push_back({0, IdList, nullTerminal});temp->push_back({0, ArrayType, nullTerminal});LL1matrix[FieldDecList][ARRAY] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[FieldDecMore][END] = temp;
temp = new vector<Symbol>;temp->push_back({0, FieldDecList, nullTerminal});LL1matrix[FieldDecMore][CHAR] = temp;LL1matrix[FieldDecMore][ARRAY] = temp;LL1matrix[FieldDecMore][INTEGER] = temp;
temp = new vector<Symbol>;temp->push_back({0, IdMore, nullTerminal});temp->push_back({1, nullNonTerminal, ID});LL1matrix[IdList][ID] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[IdMore][SEMI] = temp;
temp = new vector<Symbol>;temp->push_back({0, IdList, nullTerminal});temp->push_back({1, nullNonTerminal, COMMA});LL1matrix[IdMore][COMMA] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[VarDecpart][BEGIN] = temp;LL1matrix[VarDecpart][PROCEDURE] = temp;
temp = new vector<Symbol>;temp->push_back({0, VarDec, nullTerminal});LL1matrix[VarDecpart][VAR] = temp;
temp = new vector<Symbol>;temp->push_back({0, VarDecList, nullTerminal});temp->push_back({1, nullNonTerminal, VAR});LL1matrix[VarDec][VAR] = temp;
temp = new vector<Symbol>;temp->push_back({0, VarDecMore, nullTerminal});temp->push_back({1, nullNonTerminal, SEMI});temp->push_back({0, VarldList, nullTerminal});temp->push_back({0, TypeDef, nullTerminal});LL1matrix[VarDecList][CHAR] = temp;LL1matrix[VarDecList][RECORD] = temp;LL1matrix[VarDecList][ID] = temp;LL1matrix[VarDecList][ARRAY] = temp;LL1matrix[VarDecList][INTEGER] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[VarDecMore][BEGIN] = temp;LL1matrix[VarDecMore][PROCEDURE] = temp;
temp = new vector<Symbol>;temp->push_back({0, VarDecList, nullTerminal});LL1matrix[VarDecMore][CHAR] = temp;LL1matrix[VarDecMore][RECORD] = temp;LL1matrix[VarDecMore][ID] = temp;LL1matrix[VarDecMore][ARRAY] = temp;LL1matrix[VarDecMore][INTEGER] = temp;
temp = new vector<Symbol>;temp->push_back({0, VarldMore, nullTerminal});temp->push_back({1, nullNonTerminal, ID});LL1matrix[VarldList][ID] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[VarldMore][SEMI] = temp;
temp = new vector<Symbol>;temp->push_back({0, VarldList, nullTerminal});temp->push_back({1, nullNonTerminal, COMMA});LL1matrix[VarldMore][COMMA] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[ProcDecpart][BEGIN] = temp;
temp = new vector<Symbol>;temp->push_back({0, ProcDec, nullTerminal});LL1matrix[ProcDecpart][PROCEDURE] = temp;
temp = new vector<Symbol>;temp->push_back({0, ProcDecMore, nullTerminal});temp->push_back({0, ProcBody, nullTerminal});temp->push_back({0, ProcDecPart, nullTerminal});temp->push_back({1, nullNonTerminal, SEMI});temp->push_back({1, nullNonTerminal, RPAREN});temp->push_back({0, ParamList, nullTerminal});temp->push_back({1, nullNonTerminal, LPAREN});temp->push_back({0, ProcName, nullTerminal});temp->push_back({1, nullNonTerminal, PROCEDURE});LL1matrix[ProcDec][PROCEDURE] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[ProcDecMore][BEGIN] = temp;
temp = new vector<Symbol>;temp->push_back({0, ProcDec, nullTerminal});LL1matrix[ProcDecMore][PROCEDURE] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, ID});LL1matrix[ProcName][ID] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[ParamList][RPAREN] = temp;
temp = new vector<Symbol>;temp->push_back({0, ParamDecList, nullTerminal});LL1matrix[ParamList][CHAR] = temp;LL1matrix[ParamList][RECORD] = temp;LL1matrix[ParamList][VAR] = temp;LL1matrix[ParamList][ID] = temp;LL1matrix[ParamList][ARRAY] = temp;LL1matrix[ParamList][INTEGER] = temp;
temp = new vector<Symbol>;temp->push_back({0, ParamMore, nullTerminal});temp->push_back({0, Param, nullTerminal});LL1matrix[ParamDecList][CHAR] = temp;LL1matrix[ParamDecList][RECORD] = temp;LL1matrix[ParamDecList][VAR] = temp;LL1matrix[ParamDecList][ID] = temp;LL1matrix[ParamDecList][ARRAY] = temp;LL1matrix[ParamDecList][INTEGER] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[ParamMore][RPAREN] = temp;
temp = new vector<Symbol>;temp->push_back({0, ParamDecList, nullTerminal});temp->push_back({1, nullNonTerminal, SEMI});LL1matrix[ParamMore][SEMI] = temp;
temp = new vector<Symbol>;temp->push_back({0, FormList, nullTerminal});temp->push_back({0, TypeDef, nullTerminal});LL1matrix[Param][CHAR] = temp;LL1matrix[Param][RECORD] = temp;LL1matrix[Param][ID] = temp;LL1matrix[Param][ARRAY] = temp;LL1matrix[Param][INTEGER] = temp;
temp = new vector<Symbol>;temp->push_back({0, FormList, nullTerminal});temp->push_back({0, TypeDef, nullTerminal});temp->push_back({1, nullNonTerminal, VAR});LL1matrix[Param][VAR] = temp;
temp = new vector<Symbol>;temp->push_back({0, FidMore, nullTerminal});temp->push_back({1, nullNonTerminal, ID});LL1matrix[FormList][ID] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[FidMore][SEMI] = temp;LL1matrix[FidMore][RPAREN] = temp;
temp = new vector<Symbol>;temp->push_back({0, FormList, nullTerminal});temp->push_back({1, nullNonTerminal, COMMA});LL1matrix[FidMore][COMMA] = temp;
temp = new vector<Symbol>;temp->push_back({0, DeclarePart, nullTerminal});LL1matrix[ProcDecPart][TYPE] = temp;LL1matrix[ProcDecPart][VAR] = temp;LL1matrix[ProcDecPart][BEGIN] = temp;LL1matrix[ProcDecPart][PROCEDURE] = temp;
temp = new vector<Symbol>;temp->push_back({0, ProgramBody, nullTerminal});LL1matrix[ProcBody][BEGIN] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, END});temp->push_back({0, StmList, nullTerminal});temp->push_back({1, nullNonTerminal, BEGIN});LL1matrix[ProgramBody][BEGIN] = temp;
temp = new vector<Symbol>;temp->push_back({0, StmMore, nullTerminal});temp->push_back({0, Stm, nullTerminal});LL1matrix[StmList][RETURN] = temp;LL1matrix[StmList][WRITE] = temp;LL1matrix[StmList][ID] = temp;LL1matrix[StmList][WHILE] = temp;LL1matrix[StmList][IF] = temp;LL1matrix[StmList][READ] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[StmMore][END] = temp;LL1matrix[StmMore][FI] = temp;LL1matrix[StmMore][ELSE] = temp;LL1matrix[StmMore][ENDWH] = temp;
temp = new vector<Symbol>;temp->push_back({0, StmList, nullTerminal});temp->push_back({1, nullNonTerminal, SEMI});LL1matrix[StmMore][SEMI] = temp;
temp = new vector<Symbol>;temp->push_back({0, ConditionalStm, nullTerminal});LL1matrix[Stm][IF] = temp;
temp = new vector<Symbol>;temp->push_back({0, LoopStm, nullTerminal});LL1matrix[Stm][WHILE] = temp;
temp = new vector<Symbol>;temp->push_back({0, InputStm, nullTerminal});LL1matrix[Stm][READ] = temp;
temp = new vector<Symbol>;temp->push_back({0, OutputStm, nullTerminal});LL1matrix[Stm][WRITE] = temp;
temp = new vector<Symbol>;temp->push_back({0, ReturnStm, nullTerminal});LL1matrix[Stm][RETURN] = temp;
temp = new vector<Symbol>;temp->push_back({0, AssCall, nullTerminal});temp->push_back({1, nullNonTerminal, ID});LL1matrix[Stm][ID] = temp;
temp = new vector<Symbol>;temp->push_back({0, AssignmentRest, nullTerminal});LL1matrix[AssCall][SEMI] = temp;LL1matrix[AssCall][ELSE] = temp;LL1matrix[AssCall][ENDWH] = temp;LL1matrix[AssCall][DOT] = temp;LL1matrix[AssCall][FI] = temp;LL1matrix[AssCall][LMIDPAREN] = temp;LL1matrix[AssCall][END] = temp;LL1matrix[AssCall][ASSIGN] = temp;
temp = new vector<Symbol>;temp->push_back({0, CallStmRest, nullTerminal});LL1matrix[AssCall][LPAREN] = temp;
temp = new vector<Symbol>;temp->push_back({0, Exp, nullTerminal});temp->push_back({1, nullNonTerminal, ASSIGN});temp->push_back({0, VariMore, nullTerminal});LL1matrix[AssignmentRest][DOT] = temp;LL1matrix[AssignmentRest][LMIDPAREN] = temp;LL1matrix[AssignmentRest][ASSIGN] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, FI});temp->push_back({0, StmList, nullTerminal});temp->push_back({1, nullNonTerminal, ELSE});temp->push_back({0, StmList, nullTerminal});temp->push_back({1, nullNonTerminal, THEN});temp->push_back({0, RelExp, nullTerminal});temp->push_back({1, nullNonTerminal, IF});LL1matrix[ConditionalStm][IF] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, ENDWH});temp->push_back({0, StmList, nullTerminal});temp->push_back({1, nullNonTerminal, DO});temp->push_back({0, RelExp, nullTerminal});temp->push_back({1, nullNonTerminal, WHILE});LL1matrix[LoopStm][WHILE] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, RPAREN});temp->push_back({0, Invar, nullTerminal});temp->push_back({1, nullNonTerminal, LPAREN});temp->push_back({1, nullNonTerminal, READ});LL1matrix[InputStm][READ] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, ID});LL1matrix[Invar][ID] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, RPAREN});temp->push_back({0, Exp, nullTerminal});temp->push_back({1, nullNonTerminal, LPAREN});temp->push_back({1, nullNonTerminal, WRITE});LL1matrix[OutputStm][WRITE] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, RETURN});LL1matrix[ReturnStm][RETURN] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, RPAREN});temp->push_back({0, ActParamList, nullTerminal});temp->push_back({1, nullNonTerminal, LPAREN});LL1matrix[CallStmRest][LPAREN] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[ActParamList][RPAREN] = temp;
temp = new vector<Symbol>;temp->push_back({0, ActParamMore, nullTerminal});temp->push_back({0, Exp, nullTerminal});LL1matrix[ActParamList][INTC] = temp;LL1matrix[ActParamList][LPAREN] = temp;LL1matrix[ActParamList][ID] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[ActParamMore][RPAREN] = temp;
temp = new vector<Symbol>;temp->push_back({0, ActParamList, nullTerminal});temp->push_back({1, nullNonTerminal, COMMA});LL1matrix[ActParamMore][COMMA] = temp;
temp = new vector<Symbol>;temp->push_back({0, OtherRelE, nullTerminal});temp->push_back({0, Exp, nullTerminal});LL1matrix[RelExp][INTC] = temp;LL1matrix[RelExp][LPAREN] = temp;LL1matrix[RelExp][ID] = temp;
temp = new vector<Symbol>;temp->push_back({0, Exp, nullTerminal});temp->push_back({0, CmpOp, nullTerminal});LL1matrix[OtherRelE][LT] = temp;LL1matrix[OtherRelE][EQ] = temp;
temp = new vector<Symbol>;temp->push_back({0, OtherTerm, nullTerminal});temp->push_back({0, Term, nullTerminal});LL1matrix[Exp][INTC] = temp;LL1matrix[Exp][LPAREN] = temp;LL1matrix[Exp][ID] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[OtherTerm][THEN] = temp;LL1matrix[OtherTerm][SEMI] = temp;LL1matrix[OtherTerm][ELSE] = temp;LL1matrix[OtherTerm][DO] = temp;LL1matrix[OtherTerm][EQ] = temp;LL1matrix[OtherTerm][ENDWH] = temp;LL1matrix[OtherTerm][FI] = temp;LL1matrix[OtherTerm][COMMA] = temp;LL1matrix[OtherTerm][LT] = temp;LL1matrix[OtherTerm][END] = temp;LL1matrix[OtherTerm][RPAREN] = temp;LL1matrix[OtherTerm][RMIDPAREN] = temp;
temp = new vector<Symbol>;temp->push_back({0, Exp, nullTerminal});temp->push_back({0, AddOp, nullTerminal});LL1matrix[OtherTerm][PLUS] = temp;LL1matrix[OtherTerm][MINUS] = temp;
temp = new vector<Symbol>;temp->push_back({0, OtherFactor, nullTerminal});temp->push_back({0, Factor, nullTerminal});LL1matrix[Term][INTC] = temp;LL1matrix[Term][LPAREN] = temp;LL1matrix[Term][ID] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[OtherFactor][THEN] = temp;LL1matrix[OtherFactor][SEMI] = temp;LL1matrix[OtherFactor][ELSE] = temp;LL1matrix[OtherFactor][DO] = temp;LL1matrix[OtherFactor][EQ] = temp;LL1matrix[OtherFactor][ENDWH] = temp;LL1matrix[OtherFactor][FI] = temp;LL1matrix[OtherFactor][PLUS] = temp;LL1matrix[OtherFactor][COMMA] = temp;LL1matrix[OtherFactor][LT] = temp;LL1matrix[OtherFactor][MINUS] = temp;LL1matrix[OtherFactor][END] = temp;LL1matrix[OtherFactor][RPAREN] = temp;LL1matrix[OtherFactor][RMIDPAREN] = temp;
temp = new vector<Symbol>;temp->push_back({0, Term, nullTerminal});temp->push_back({0, MultOp, nullTerminal});LL1matrix[OtherFactor][TIMES] = temp;LL1matrix[OtherFactor][OVER] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, RPAREN});temp->push_back({0, Exp, nullTerminal});temp->push_back({1, nullNonTerminal, LPAREN});LL1matrix[Factor][LPAREN] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, INTC});LL1matrix[Factor][INTC] = temp;
temp = new vector<Symbol>;temp->push_back({0, Variable, nullTerminal});LL1matrix[Factor][ID] = temp;
temp = new vector<Symbol>;temp->push_back({0, VariMore, nullTerminal});temp->push_back({1, nullNonTerminal, ID});LL1matrix[Variable][ID] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[VariMore][TIMES] = temp;LL1matrix[VariMore][THEN] = temp;LL1matrix[VariMore][SEMI] = temp;LL1matrix[VariMore][PLUS] = temp;LL1matrix[VariMore][OVER] = temp;LL1matrix[VariMore][END] = temp;LL1matrix[VariMore][RMIDPAREN] = temp;LL1matrix[VariMore][ELSE] = temp;LL1matrix[VariMore][DO] = temp;LL1matrix[VariMore][EQ] = temp;LL1matrix[VariMore][ENDWH] = temp;LL1matrix[VariMore][FI] = temp;LL1matrix[VariMore][COMMA] = temp;LL1matrix[VariMore][LT] = temp;LL1matrix[VariMore][MINUS] = temp;LL1matrix[VariMore][RPAREN] = temp;LL1matrix[VariMore][ASSIGN] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, RMIDPAREN});temp->push_back({0, Exp, nullTerminal});temp->push_back({1, nullNonTerminal, LMIDPAREN});LL1matrix[VariMore][LMIDPAREN] = temp;
temp = new vector<Symbol>;temp->push_back({0, FieldVar, nullTerminal});temp->push_back({1, nullNonTerminal, DOT});LL1matrix[VariMore][DOT] = temp;
temp = new vector<Symbol>;temp->push_back({0, FieldVarMore, nullTerminal});temp->push_back({1, nullNonTerminal, ID});LL1matrix[FieldVar][ID] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, NONE});LL1matrix[FieldVarMore][TIMES] = temp;LL1matrix[FieldVarMore][THEN] = temp;LL1matrix[FieldVarMore][SEMI] = temp;LL1matrix[FieldVarMore][PLUS] = temp;LL1matrix[FieldVarMore][OVER] = temp;LL1matrix[FieldVarMore][END] = temp;LL1matrix[FieldVarMore][RMIDPAREN] = temp;LL1matrix[FieldVarMore][ELSE] = temp;LL1matrix[FieldVarMore][DO] = temp;LL1matrix[FieldVarMore][EQ] = temp;LL1matrix[FieldVarMore][ENDWH] = temp;LL1matrix[FieldVarMore][FI] = temp;LL1matrix[FieldVarMore][COMMA] = temp;LL1matrix[FieldVarMore][LT] = temp;LL1matrix[FieldVarMore][MINUS] = temp;LL1matrix[FieldVarMore][RPAREN] = temp;LL1matrix[FieldVarMore][ASSIGN] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, RMIDPAREN});temp->push_back({0, Exp, nullTerminal});temp->push_back({1, nullNonTerminal, LMIDPAREN});LL1matrix[FieldVarMore][LMIDPAREN] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, LT});LL1matrix[CmpOp][LT] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, EQ});LL1matrix[CmpOp][EQ] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, PLUS});LL1matrix[AddOp][PLUS] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, MINUS});LL1matrix[AddOp][MINUS] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, TIMES});LL1matrix[MultOp][TIMES] = temp;
temp = new vector<Symbol>;temp->push_back({1, nullNonTerminal, OVER});LL1matrix[MultOp][OVER] = temp;


    //-------------------------------------------------------------

    if (OUT_FLAG)
        cout << "LL1分析矩阵初始化完成" << endl;
}
