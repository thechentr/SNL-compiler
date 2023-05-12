/*
    语法分析所需要的必要声明：非终极符 终极符 树结构
*/
// 语法分析树的输出文件
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
using namespace std;

//--------------------------------枚举终极符号、非终极符号(由enmu()生成)

typedef enum {Program, 
ReturnStm, 
VarldMore, 
ProcDecPart, 
FieldVarMore, 
StmMore, 
FormList, 
MultOp, 
ProgramHead, 
VariMore, 
AssCall, 
OtherTerm, 
OtherRelE, 
DeclarePart, 
Top, 
ConditionalStm, 
BaseType, 
TypeDec, 
VarldList, 
IdList, 
RecType, 
Stm, 
ArrayType, 
ActParamList, 
ProgramName, 
Invar, 
VarDecMore, 
ProcDecpart, 
ParamDecList, 
FidMore, 
TypeDef, 
nullNonTerminal, 
Variable, 
ParamList, 
IdMore, 
Factor, 
ProcDec, 
OtherFactor, 
VarDec, 
StmList, 
TypeDecpart, 
ProcDecMore, 
Exp, 
VarDecpart, 
AssignmentRest, 
VarDecList, 
ParamMore, 
CallStmRest, 
InputStm, 
CmpOp, 
AddOp, 
ProgramBody, 
TypeDecMore, 
Param, 
LoopStm, 
OutputStm, 
Term, 
FieldVar, 
FieldDecList, 
ProcBody, 
ActParamMore, 
FieldDecMore, 
TypeId, 
Low, 
ProcName, 
StructureType, 
RelExp, 
TypeDecList, 
}NonTerminal;

typedef enum {TIMES, 
SEMI, 
THEN, 
PROCEDURE, 
PROGRAM, 
WHILE, 
IF, 
nullTerminal, 
RETURN, 
BEGIN, 
END, 
OF, 
COMMA, 
ARRAY, 
INTEGER, 
LT, 
TYPE, 
RPAREN, 
ASSIGN, 
CHAR, 
UNDERANGE, 
WRITE, 
ID, 
PLUS, 
OVER, 
READ, 
NONE, 
LMIDPAREN, 
RECORD, 
INTC, 
LPAREN, 
RMIDPAREN, 
ELSE, 
DO, 
EQ, 
ENDWH, 
DOT, 
FI, 
VAR, 
MINUS, 
}Terminal;


//------------------------------------------------------------------


// 完成终结符、非终极符号 枚举类型->字符串 的转换
//---------------------------------------------------(由getStr()生成)

string getString(Terminal t){if (t == -1)return NULL;else if (t == TIMES) return "TIMES";
else if (t == SEMI) return "SEMI";
else if (t == THEN) return "THEN";
else if (t == PROCEDURE) return "PROCEDURE";
else if (t == PROGRAM) return "PROGRAM";
else if (t == WHILE) return "WHILE";
else if (t == IF) return "IF";
else if (t == nullTerminal) return "nullTerminal";
else if (t == RETURN) return "RETURN";
else if (t == BEGIN) return "BEGIN";
else if (t == END) return "END";
else if (t == OF) return "OF";
else if (t == COMMA) return "COMMA";
else if (t == ARRAY) return "ARRAY";
else if (t == INTEGER) return "INTEGER";
else if (t == LT) return "LT";
else if (t == TYPE) return "TYPE";
else if (t == RPAREN) return "RPAREN";
else if (t == ASSIGN) return "ASSIGN";
else if (t == CHAR) return "CHAR";
else if (t == UNDERANGE) return "UNDERANGE";
else if (t == WRITE) return "WRITE";
else if (t == ID) return "ID";
else if (t == PLUS) return "PLUS";
else if (t == OVER) return "OVER";
else if (t == READ) return "READ";
else if (t == NONE) return "NONE";
else if (t == LMIDPAREN) return "LMIDPAREN";
else if (t == RECORD) return "RECORD";
else if (t == INTC) return "INTC";
else if (t == LPAREN) return "LPAREN";
else if (t == RMIDPAREN) return "RMIDPAREN";
else if (t == ELSE) return "ELSE";
else if (t == DO) return "DO";
else if (t == EQ) return "EQ";
else if (t == ENDWH) return "ENDWH";
else if (t == DOT) return "DOT";
else if (t == FI) return "FI";
else if (t == VAR) return "VAR";
else if (t == MINUS) return "MINUS";
return NULL;}

string getString(NonTerminal t){if (t == -1)return NULL;else if (t == Program) return "Program";
else if (t == ReturnStm) return "ReturnStm";
else if (t == VarldMore) return "VarldMore";
else if (t == ProcDecPart) return "ProcDecPart";
else if (t == FieldVarMore) return "FieldVarMore";
else if (t == StmMore) return "StmMore";
else if (t == FormList) return "FormList";
else if (t == MultOp) return "MultOp";
else if (t == ProgramHead) return "ProgramHead";
else if (t == VariMore) return "VariMore";
else if (t == AssCall) return "AssCall";
else if (t == OtherTerm) return "OtherTerm";
else if (t == OtherRelE) return "OtherRelE";
else if (t == DeclarePart) return "DeclarePart";
else if (t == Top) return "Top";
else if (t == ConditionalStm) return "ConditionalStm";
else if (t == BaseType) return "BaseType";
else if (t == TypeDec) return "TypeDec";
else if (t == VarldList) return "VarldList";
else if (t == IdList) return "IdList";
else if (t == RecType) return "RecType";
else if (t == Stm) return "Stm";
else if (t == ArrayType) return "ArrayType";
else if (t == ActParamList) return "ActParamList";
else if (t == ProgramName) return "ProgramName";
else if (t == Invar) return "Invar";
else if (t == VarDecMore) return "VarDecMore";
else if (t == ProcDecpart) return "ProcDecpart";
else if (t == ParamDecList) return "ParamDecList";
else if (t == FidMore) return "FidMore";
else if (t == TypeDef) return "TypeDef";
else if (t == nullNonTerminal) return "nullNonTerminal";
else if (t == Variable) return "Variable";
else if (t == ParamList) return "ParamList";
else if (t == IdMore) return "IdMore";
else if (t == Factor) return "Factor";
else if (t == ProcDec) return "ProcDec";
else if (t == OtherFactor) return "OtherFactor";
else if (t == VarDec) return "VarDec";
else if (t == StmList) return "StmList";
else if (t == TypeDecpart) return "TypeDecpart";
else if (t == ProcDecMore) return "ProcDecMore";
else if (t == Exp) return "Exp";
else if (t == VarDecpart) return "VarDecpart";
else if (t == AssignmentRest) return "AssignmentRest";
else if (t == VarDecList) return "VarDecList";
else if (t == ParamMore) return "ParamMore";
else if (t == CallStmRest) return "CallStmRest";
else if (t == InputStm) return "InputStm";
else if (t == CmpOp) return "CmpOp";
else if (t == AddOp) return "AddOp";
else if (t == ProgramBody) return "ProgramBody";
else if (t == TypeDecMore) return "TypeDecMore";
else if (t == Param) return "Param";
else if (t == LoopStm) return "LoopStm";
else if (t == OutputStm) return "OutputStm";
else if (t == Term) return "Term";
else if (t == FieldVar) return "FieldVar";
else if (t == FieldDecList) return "FieldDecList";
else if (t == ProcBody) return "ProcBody";
else if (t == ActParamMore) return "ActParamMore";
else if (t == FieldDecMore) return "FieldDecMore";
else if (t == TypeId) return "TypeId";
else if (t == Low) return "Low";
else if (t == ProcName) return "ProcName";
else if (t == StructureType) return "StructureType";
else if (t == RelExp) return "RelExp";
else if (t == TypeDecList) return "TypeDecList";
return NULL;}


//---------------------------------------------------------------

// 保存终结符类型和具体单词
struct Word
{
    string str;
    Terminal tok; // 终结符类型
};

// token : 行号、单词
struct Token
{
    int line;
    struct Word wd;
};

// 树结点
class TreeNode
{
public:
    string name; // 节点名
    Token *tk;   // 终结符结点tk为token， 非终结符结点为NULL
    TreeNode *father;
    TreeNode *child[32];
    int childIndex;
    TreeNode(string nm)
    {
        name = nm;
        tk = NULL;
        father = NULL;
        for (int i = 0; i < 31; i++)
        {
            child[i] = NULL;
        }
        childIndex = 0;
    }
    // 向该结点中添加一个子节点，如果结点值为空或结点没有孩子就不添加
    void addChild(TreeNode *t)
    {
        // 非终极符号的 t->tk 为 NULL 终极符号肯定接
        if (t->tk == NULL && !t->haveChile())
        {
            // cout << t->name << "无孩子 被淘汰" << endl;
            return;
        }
        t->father = this;
        child[childIndex] = t;
        childIndex++;
    }
    // 判断该结点是否有孩子
    bool haveChile()
    {
        return childIndex > 0;
    }
};

class Tree
{
private:
    TreeNode *root; // 根节点
    void _print(TreeNode *t, string branch);

public:
    Tree(TreeNode *r);
    TreeNode *getRoot();
    // 打印语法树并存储到output中
    void print(string output);
};

Tree::Tree(TreeNode *r)
{
    root = r;
}

TreeNode *Tree::getRoot()
{
    return root;
}

void Tree::print(string output = "")
{

    cout << "语法树为" << endl;
    string branch = "";
    _print(root, branch);
    if (output != "")
    {
        freopen(output.c_str(), "w", stdout);
        _print(root, branch);
        freopen("CON", "w", stdout);
    }
}

void Tree::_print(TreeNode *t, string branch)
{
    // 打印本结点前的树杈
    cout << branch;
    // 打印本结点
    bool last = 1;
    if (t->father != NULL)
    {
        // 本结点是否为其父节点的最后一个结点
        last = (t == t->father->child[t->father->childIndex - 1]);
        if (last)
        {
            cout << "└";
        }
        else
        {
            cout << "├";
        }
    }

    cout << t->name << endl;

    // 打印其儿子结点
    string temp = branch;
    if (last)
        temp += " ";
    else
        temp += "│";
    // 按照当前结点名称长度在其子节点前加空格
    for (int i = 0; i < (int)t->name.size() / 2; i++)
        temp += " ";

    // 前序遍历子节点
    for (int i = 0; i < t->childIndex; i++)
    {
        _print(t->child[i], temp);
    }
}

// 以下函数本应在词法分析中，因负责词法分析部分的同学与我没有统一好接口而额外添加
// 目的是词法分析生成的token序列转换为语法分析需要的格式
//-------------------------------------------------------------

// 将词法分析部分同学写的语法信息和语义信息转换成接口格式
struct Word convertToWD(string yf, string yy, bool out = 0)
{
    // 并提前声明一些固定终结符，便于直接使用
    Word reservedWords[21] =
        {
            {"program", PROGRAM},
            {"type", TYPE},
            {"var", VAR},
            {"procedure", PROCEDURE},
            {"begin", BEGIN},
            {"end", END},
            {"array", ARRAY},
            {"of", OF},
            {"record", RECORD},
            {"if", IF},
            {"then", THEN},
            {"else", ELSE},
            {"fi", FI},
            {"while", WHILE},
            {"do", DO},
            {"endwh", ENDWH},
            {"read", READ},
            {"write", WRITE},
            {"return", RETURN},
            {"integer", INTEGER},
            {"char", CHAR}},
         // 单分界符
        delimiter[11] = {
            {";", SEMI},
            {"(", LPAREN},
            {")", RPAREN},
            {"]", RMIDPAREN},
            {"[", LMIDPAREN},
            {"=", EQ},
            {"+", PLUS},
            {"-", MINUS},
            {"*", TIMES},
            {"/", OVER},
            {"<", LT}};

    Word re;
    re.str = yy;
    if (yf == "保留字")
    {
        for (int i = 0; i < 21; i++)
        {
            if (reservedWords[i].str == yy)
            {
                if (out)
                    cout << "检测到 保留字 " << reservedWords[i].str << endl;
                re = reservedWords[i];
                return re;
            }
        }
    }
    if (yf == "标识符")
    {
        re.tok = ID;
        if (out)
            cout << "检测到 标识符 " << re.str << endl;
        return re;
    }
    if (yf == "单分界符")
    {
        for (int i = 0; i < 11; i++)
        {
            if (delimiter[i].str == yy)
            {
                if (out)
                    cout << "检测到 单分界符 " << delimiter[i].str << endl;
                re = delimiter[i];
                return re;
            }
        }
    }
    if (yf == "双分界符")
    {
        re.tok = ASSIGN;
        if (out)
            cout << "检测到 双分界符 " << re.str << endl;
        return re;
    }
    if (yf == "字符标志状态")
    {
        re.tok = COMMA;
        if (out)
            cout << "检测到 字符标志状态 " << re.str << endl;
        return re;
    }
    if (yf == "无符号整数")
    {
        re.tok = INTC;
        if (out)
            cout << "检测到 无符号整数 " << re.str << endl;
        return re;
    }
    if (yf == "程序结束标志")
    {
        re.tok = DOT;
        if (out)
            cout << "检测到 程序结束标志 " << re.str << endl;
        return re;
    }
    if (yf == "数组下标")
    {
        re.tok = UNDERANGE;
        if (out)
            cout << "检测到 数组下标 " << re.str << endl;
        return re;
    }
    if (yf == "注释")
    {
        re.tok = nullTerminal;
        return re;
    }
    cout << "未能识别 " << yf << " " << yy << endl;
    exit(1);
}

// 将所有token转换为tokenList
typedef pair<int, pair<string, string>> PSS; // 存储单个Token
vector<Token> tokenToTokenlist(vector<PSS> token)
{
    int N = token.size();
    cout << "共计" << N << "个token" << endl;
    vector<Token> tList;
    for (int i = 0; i < N; i++)
    {
        Token temp;
        // temp.index = i;
        temp.line = token[i].first;
        temp.wd = convertToWD(token[i].second.first, token[i].second.second);
        if (temp.wd.tok == nullTerminal)
        {
            cout << "跳过注释" << endl;
            continue;
        }
        tList.push_back(temp);
    }
    return tList;
}