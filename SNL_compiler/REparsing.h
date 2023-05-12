#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <vector>
#ifndef PS_DEC
#define PS_DEC
#include "PasingDec.h"
#endif
using namespace std;

class RDparsinger
{
private:
    // token表
    vector<Token> tokenList;
    // 建立语法树时使用
    Token *currentToken;
    // currentToken = tokenList [ INDEX ]
    int INDEX;
    // 读取Token序列中的下一个Token，Match或输出错误时使用
    void readToken();
    // 对Token序列中的下一个Token进行匹配
    TreeNode *matchToken(Terminal tok);
    // 以下为各个产生式的子程序------------------(rdDec()生成)

    TreeNode* _Program();
TreeNode* _ReturnStm();
TreeNode* _VarldMore();
TreeNode* _ProcDecPart();
TreeNode* _FieldVarMore();
TreeNode* _StmMore();
TreeNode* _FormList();
TreeNode* _MultOp();
TreeNode* _ProgramHead();
TreeNode* _VariMore();
TreeNode* _AssCall();
TreeNode* _OtherTerm();
TreeNode* _OtherRelE();
TreeNode* _DeclarePart();
TreeNode* _Top();
TreeNode* _ConditionalStm();
TreeNode* _BaseType();
TreeNode* _TypeDec();
TreeNode* _VarldList();
TreeNode* _IdList();
TreeNode* _RecType();
TreeNode* _Stm();
TreeNode* _ArrayType();
TreeNode* _ActParamList();
TreeNode* _ProgramName();
TreeNode* _Invar();
TreeNode* _VarDecMore();
TreeNode* _ProcDecpart();
TreeNode* _ParamDecList();
TreeNode* _FidMore();
TreeNode* _TypeDef();
TreeNode* _Variable();
TreeNode* _ParamList();
TreeNode* _IdMore();
TreeNode* _Factor();
TreeNode* _ProcDec();
TreeNode* _OtherFactor();
TreeNode* _VarDec();
TreeNode* _StmList();
TreeNode* _TypeDecpart();
TreeNode* _ProcDecMore();
TreeNode* _Exp();
TreeNode* _VarDecpart();
TreeNode* _AssignmentRest();
TreeNode* _VarDecList();
TreeNode* _ParamMore();
TreeNode* _CallStmRest();
TreeNode* _InputStm();
TreeNode* _CmpOp();
TreeNode* _AddOp();
TreeNode* _ProgramBody();
TreeNode* _TypeDecMore();
TreeNode* _Param();
TreeNode* _LoopStm();
TreeNode* _OutputStm();
TreeNode* _Term();
TreeNode* _FieldVar();
TreeNode* _FieldDecList();
TreeNode* _ProcBody();
TreeNode* _ActParamMore();
TreeNode* _FieldDecMore();
TreeNode* _TypeId();
TreeNode* _Low();
TreeNode* _ProcName();
TreeNode* _StructureType();
TreeNode* _RelExp();
TreeNode* _TypeDecList();


    // ----------------------------------------
    void printErrorMsg(string reason);
    // 当前待处理token属于当前非终极符predict集，报错
    void unExcept();

public:
    RDparsinger(vector<Token> list); // 将Token序列作为参数传递
    TreeNode *analyse();
};

RDparsinger::RDparsinger(vector<Token> list)
{

    tokenList.assign(list.begin(), list.end());
}

void RDparsinger::unExcept()
{
    cout << tokenList[INDEX].line << " 行 " << tokenList[INDEX].wd.str << "处有错误！" << endl;
    exit(1);
}

TreeNode *RDparsinger::analyse()
{
    INDEX = 0;
    readToken();
    TreeNode *re = _Program();
    cout << INDEX << endl;
    return re;
}

void RDparsinger::readToken()
{
    currentToken = &tokenList[INDEX++];
}

TreeNode *RDparsinger::matchToken(Terminal tok)
{

    if (currentToken->wd.tok == tok)
    {
        TreeNode *t = new TreeNode(currentToken->wd.str);
        t->tk = currentToken;
        // cout << "match   " << currentToken->wd.str << endl;
        readToken();
        return t;
    }
    else
    {
        cout << currentToken->line << " 行 " << getString(currentToken->wd.tok) << " 处应为" << getString(tok) << endl;
        exit(1);
    }
    exit(2);
    return NULL;
}

//----------------------------------------------------------各非终极符的处理程序(由rdFuc()生成)

TreeNode* RDparsinger::_Program(){TreeNode* t = new TreeNode("Program");if(currentToken->wd.tok == PROGRAM){t->addChild(_ProgramHead());t->addChild(_DeclarePart());t->addChild(_ProgramBody());return t;}unExcept();return t;}
TreeNode* RDparsinger::_ReturnStm(){TreeNode* t = new TreeNode("ReturnStm");if(currentToken->wd.tok == RETURN){t->addChild(matchToken(RETURN));return t;}unExcept();return t;}
TreeNode* RDparsinger::_VarldMore(){TreeNode* t = new TreeNode("VarldMore");if(currentToken->wd.tok == SEMI){return t;}if(currentToken->wd.tok == COMMA){t->addChild(matchToken(COMMA));t->addChild(_VarldList());return t;}unExcept();return t;}
TreeNode* RDparsinger::_ProcDecPart(){TreeNode* t = new TreeNode("ProcDecPart");if(currentToken->wd.tok == TYPE||currentToken->wd.tok == VAR||currentToken->wd.tok == BEGIN||currentToken->wd.tok == PROCEDURE){t->addChild(_DeclarePart());return t;}unExcept();return t;}
TreeNode* RDparsinger::_FieldVarMore(){TreeNode* t = new TreeNode("FieldVarMore");if(currentToken->wd.tok == TIMES||currentToken->wd.tok == THEN||currentToken->wd.tok == SEMI||currentToken->wd.tok == PLUS||currentToken->wd.tok == OVER||currentToken->wd.tok == END||currentToken->wd.tok == RMIDPAREN||currentToken->wd.tok == ELSE||currentToken->wd.tok == DO||currentToken->wd.tok == EQ||currentToken->wd.tok == ENDWH||currentToken->wd.tok == FI||currentToken->wd.tok == COMMA||currentToken->wd.tok == LT||currentToken->wd.tok == MINUS||currentToken->wd.tok == RPAREN||currentToken->wd.tok == ASSIGN){return t;}if(currentToken->wd.tok == LMIDPAREN){t->addChild(matchToken(LMIDPAREN));t->addChild(_Exp());t->addChild(matchToken(RMIDPAREN));return t;}unExcept();return t;}
TreeNode* RDparsinger::_StmMore(){TreeNode* t = new TreeNode("StmMore");if(currentToken->wd.tok == END||currentToken->wd.tok == FI||currentToken->wd.tok == ELSE||currentToken->wd.tok == ENDWH){return t;}if(currentToken->wd.tok == SEMI){t->addChild(matchToken(SEMI));t->addChild(_StmList());return t;}unExcept();return t;}
TreeNode* RDparsinger::_FormList(){TreeNode* t = new TreeNode("FormList");if(currentToken->wd.tok == ID){t->addChild(matchToken(ID));t->addChild(_FidMore());return t;}unExcept();return t;}
TreeNode* RDparsinger::_MultOp(){TreeNode* t = new TreeNode("MultOp");if(currentToken->wd.tok == TIMES){t->addChild(matchToken(TIMES));return t;}if(currentToken->wd.tok == OVER){t->addChild(matchToken(OVER));return t;}unExcept();return t;}
TreeNode* RDparsinger::_ProgramHead(){TreeNode* t = new TreeNode("ProgramHead");if(currentToken->wd.tok == PROGRAM){t->addChild(matchToken(PROGRAM));t->addChild(_ProgramName());return t;}unExcept();return t;}
TreeNode* RDparsinger::_VariMore(){TreeNode* t = new TreeNode("VariMore");if(currentToken->wd.tok == TIMES||currentToken->wd.tok == THEN||currentToken->wd.tok == SEMI||currentToken->wd.tok == PLUS||currentToken->wd.tok == OVER||currentToken->wd.tok == END||currentToken->wd.tok == RMIDPAREN||currentToken->wd.tok == ELSE||currentToken->wd.tok == DO||currentToken->wd.tok == EQ||currentToken->wd.tok == ENDWH||currentToken->wd.tok == FI||currentToken->wd.tok == COMMA||currentToken->wd.tok == LT||currentToken->wd.tok == MINUS||currentToken->wd.tok == RPAREN||currentToken->wd.tok == ASSIGN){return t;}if(currentToken->wd.tok == LMIDPAREN){t->addChild(matchToken(LMIDPAREN));t->addChild(_Exp());t->addChild(matchToken(RMIDPAREN));return t;}if(currentToken->wd.tok == DOT){t->addChild(matchToken(DOT));t->addChild(_FieldVar());return t;}unExcept();return t;}
TreeNode* RDparsinger::_AssCall(){TreeNode* t = new TreeNode("AssCall");if(currentToken->wd.tok == SEMI||currentToken->wd.tok == ELSE||currentToken->wd.tok == ENDWH||currentToken->wd.tok == DOT||currentToken->wd.tok == FI||currentToken->wd.tok == LMIDPAREN||currentToken->wd.tok == END||currentToken->wd.tok == ASSIGN){t->addChild(_AssignmentRest());return t;}if(currentToken->wd.tok == LPAREN){t->addChild(_CallStmRest());return t;}unExcept();return t;}
TreeNode* RDparsinger::_OtherTerm(){TreeNode* t = new TreeNode("OtherTerm");if(currentToken->wd.tok == THEN||currentToken->wd.tok == SEMI||currentToken->wd.tok == ELSE||currentToken->wd.tok == DO||currentToken->wd.tok == EQ||currentToken->wd.tok == ENDWH||currentToken->wd.tok == FI||currentToken->wd.tok == COMMA||currentToken->wd.tok == LT||currentToken->wd.tok == END||currentToken->wd.tok == RPAREN||currentToken->wd.tok == RMIDPAREN){return t;}if(currentToken->wd.tok == PLUS||currentToken->wd.tok == MINUS){t->addChild(_AddOp());t->addChild(_Exp());return t;}unExcept();return t;}
TreeNode* RDparsinger::_OtherRelE(){TreeNode* t = new TreeNode("OtherRelE");if(currentToken->wd.tok == LT||currentToken->wd.tok == EQ){t->addChild(_CmpOp());t->addChild(_Exp());return t;}unExcept();return t;}
TreeNode* RDparsinger::_DeclarePart(){TreeNode* t = new TreeNode("DeclarePart");if(currentToken->wd.tok == TYPE||currentToken->wd.tok == VAR||currentToken->wd.tok == BEGIN||currentToken->wd.tok == PROCEDURE){t->addChild(_TypeDecpart());t->addChild(_VarDecpart());t->addChild(_ProcDecpart());return t;}unExcept();return t;}
TreeNode* RDparsinger::_Top(){TreeNode* t = new TreeNode("Top");if(currentToken->wd.tok == INTC){t->addChild(matchToken(INTC));return t;}unExcept();return t;}
TreeNode* RDparsinger::_ConditionalStm(){TreeNode* t = new TreeNode("ConditionalStm");if(currentToken->wd.tok == IF){t->addChild(matchToken(IF));t->addChild(_RelExp());t->addChild(matchToken(THEN));t->addChild(_StmList());t->addChild(matchToken(ELSE));t->addChild(_StmList());t->addChild(matchToken(FI));return t;}unExcept();return t;}
TreeNode* RDparsinger::_BaseType(){TreeNode* t = new TreeNode("BaseType");if(currentToken->wd.tok == INTEGER){t->addChild(matchToken(INTEGER));return t;}if(currentToken->wd.tok == CHAR){t->addChild(matchToken(CHAR));return t;}unExcept();return t;}
TreeNode* RDparsinger::_TypeDec(){TreeNode* t = new TreeNode("TypeDec");if(currentToken->wd.tok == TYPE){t->addChild(matchToken(TYPE));t->addChild(_TypeDecList());return t;}unExcept();return t;}
TreeNode* RDparsinger::_VarldList(){TreeNode* t = new TreeNode("VarldList");if(currentToken->wd.tok == ID){t->addChild(matchToken(ID));t->addChild(_VarldMore());return t;}unExcept();return t;}
TreeNode* RDparsinger::_IdList(){TreeNode* t = new TreeNode("IdList");if(currentToken->wd.tok == ID){t->addChild(matchToken(ID));t->addChild(_IdMore());return t;}unExcept();return t;}
TreeNode* RDparsinger::_RecType(){TreeNode* t = new TreeNode("RecType");if(currentToken->wd.tok == RECORD){t->addChild(matchToken(RECORD));t->addChild(_FieldDecList());t->addChild(matchToken(END));return t;}unExcept();return t;}
TreeNode* RDparsinger::_Stm(){TreeNode* t = new TreeNode("Stm");if(currentToken->wd.tok == IF){t->addChild(_ConditionalStm());return t;}if(currentToken->wd.tok == WHILE){t->addChild(_LoopStm());return t;}if(currentToken->wd.tok == READ){t->addChild(_InputStm());return t;}if(currentToken->wd.tok == WRITE){t->addChild(_OutputStm());return t;}if(currentToken->wd.tok == RETURN){t->addChild(_ReturnStm());return t;}if(currentToken->wd.tok == ID){t->addChild(matchToken(ID));t->addChild(_AssCall());return t;}unExcept();return t;}
TreeNode* RDparsinger::_ArrayType(){TreeNode* t = new TreeNode("ArrayType");if(currentToken->wd.tok == ARRAY){t->addChild(matchToken(ARRAY));t->addChild(matchToken(LMIDPAREN));t->addChild(_Low());t->addChild(matchToken(UNDERANGE));t->addChild(_Top());t->addChild(matchToken(RMIDPAREN));t->addChild(matchToken(OF));t->addChild(_BaseType());return t;}unExcept();return t;}
TreeNode* RDparsinger::_ActParamList(){TreeNode* t = new TreeNode("ActParamList");if(currentToken->wd.tok == RPAREN){return t;}if(currentToken->wd.tok == INTC||currentToken->wd.tok == LPAREN||currentToken->wd.tok == ID){t->addChild(_Exp());t->addChild(_ActParamMore());return t;}unExcept();return t;}
TreeNode* RDparsinger::_ProgramName(){TreeNode* t = new TreeNode("ProgramName");if(currentToken->wd.tok == ID){t->addChild(matchToken(ID));return t;}unExcept();return t;}
TreeNode* RDparsinger::_Invar(){TreeNode* t = new TreeNode("Invar");if(currentToken->wd.tok == ID){t->addChild(matchToken(ID));return t;}unExcept();return t;}
TreeNode* RDparsinger::_VarDecMore(){TreeNode* t = new TreeNode("VarDecMore");if(currentToken->wd.tok == BEGIN||currentToken->wd.tok == PROCEDURE){return t;}if(currentToken->wd.tok == CHAR||currentToken->wd.tok == RECORD||currentToken->wd.tok == ID||currentToken->wd.tok == ARRAY||currentToken->wd.tok == INTEGER){t->addChild(_VarDecList());return t;}unExcept();return t;}
TreeNode* RDparsinger::_ProcDecpart(){TreeNode* t = new TreeNode("ProcDecpart");if(currentToken->wd.tok == BEGIN){return t;}if(currentToken->wd.tok == PROCEDURE){t->addChild(_ProcDec());return t;}unExcept();return t;}
TreeNode* RDparsinger::_ParamDecList(){TreeNode* t = new TreeNode("ParamDecList");if(currentToken->wd.tok == CHAR||currentToken->wd.tok == RECORD||currentToken->wd.tok == VAR||currentToken->wd.tok == ID||currentToken->wd.tok == ARRAY||currentToken->wd.tok == INTEGER){t->addChild(_Param());t->addChild(_ParamMore());return t;}unExcept();return t;}
TreeNode* RDparsinger::_FidMore(){TreeNode* t = new TreeNode("FidMore");if(currentToken->wd.tok == SEMI||currentToken->wd.tok == RPAREN){return t;}if(currentToken->wd.tok == COMMA){t->addChild(matchToken(COMMA));t->addChild(_FormList());return t;}unExcept();return t;}
TreeNode* RDparsinger::_TypeDef(){TreeNode* t = new TreeNode("TypeDef");if(currentToken->wd.tok == CHAR||currentToken->wd.tok == INTEGER){t->addChild(_BaseType());return t;}if(currentToken->wd.tok == RECORD||currentToken->wd.tok == ARRAY){t->addChild(_StructureType());return t;}if(currentToken->wd.tok == ID){t->addChild(matchToken(ID));return t;}unExcept();return t;}
TreeNode* RDparsinger::_Variable(){TreeNode* t = new TreeNode("Variable");if(currentToken->wd.tok == ID){t->addChild(matchToken(ID));t->addChild(_VariMore());return t;}unExcept();return t;}
TreeNode* RDparsinger::_ParamList(){TreeNode* t = new TreeNode("ParamList");if(currentToken->wd.tok == RPAREN){return t;}if(currentToken->wd.tok == CHAR||currentToken->wd.tok == RECORD||currentToken->wd.tok == VAR||currentToken->wd.tok == ID||currentToken->wd.tok == ARRAY||currentToken->wd.tok == INTEGER){t->addChild(_ParamDecList());return t;}unExcept();return t;}
TreeNode* RDparsinger::_IdMore(){TreeNode* t = new TreeNode("IdMore");if(currentToken->wd.tok == SEMI){return t;}if(currentToken->wd.tok == COMMA){t->addChild(matchToken(COMMA));t->addChild(_IdList());return t;}unExcept();return t;}
TreeNode* RDparsinger::_Factor(){TreeNode* t = new TreeNode("Factor");if(currentToken->wd.tok == LPAREN){t->addChild(matchToken(LPAREN));t->addChild(_Exp());t->addChild(matchToken(RPAREN));return t;}if(currentToken->wd.tok == INTC){t->addChild(matchToken(INTC));return t;}if(currentToken->wd.tok == ID){t->addChild(_Variable());return t;}unExcept();return t;}
TreeNode* RDparsinger::_ProcDec(){TreeNode* t = new TreeNode("ProcDec");if(currentToken->wd.tok == PROCEDURE){t->addChild(matchToken(PROCEDURE));t->addChild(_ProcName());t->addChild(matchToken(LPAREN));t->addChild(_ParamList());t->addChild(matchToken(RPAREN));t->addChild(matchToken(SEMI));t->addChild(_ProcDecPart());t->addChild(_ProcBody());t->addChild(_ProcDecMore());return t;}unExcept();return t;}
TreeNode* RDparsinger::_OtherFactor(){TreeNode* t = new TreeNode("OtherFactor");if(currentToken->wd.tok == THEN||currentToken->wd.tok == SEMI||currentToken->wd.tok == ELSE||currentToken->wd.tok == DO||currentToken->wd.tok == EQ||currentToken->wd.tok == ENDWH||currentToken->wd.tok == FI||currentToken->wd.tok == PLUS||currentToken->wd.tok == COMMA||currentToken->wd.tok == LT||currentToken->wd.tok == MINUS||currentToken->wd.tok == END||currentToken->wd.tok == RPAREN||currentToken->wd.tok == RMIDPAREN){return t;}if(currentToken->wd.tok == TIMES||currentToken->wd.tok == OVER){t->addChild(_MultOp());t->addChild(_Term());return t;}unExcept();return t;}
TreeNode* RDparsinger::_VarDec(){TreeNode* t = new TreeNode("VarDec");if(currentToken->wd.tok == VAR){t->addChild(matchToken(VAR));t->addChild(_VarDecList());return t;}unExcept();return t;}
TreeNode* RDparsinger::_StmList(){TreeNode* t = new TreeNode("StmList");if(currentToken->wd.tok == RETURN||currentToken->wd.tok == WRITE||currentToken->wd.tok == ID||currentToken->wd.tok == WHILE||currentToken->wd.tok == IF||currentToken->wd.tok == READ){t->addChild(_Stm());t->addChild(_StmMore());return t;}unExcept();return t;}
TreeNode* RDparsinger::_TypeDecpart(){TreeNode* t = new TreeNode("TypeDecpart");if(currentToken->wd.tok == VAR||currentToken->wd.tok == BEGIN||currentToken->wd.tok == PROCEDURE){return t;}if(currentToken->wd.tok == TYPE){t->addChild(_TypeDec());return t;}unExcept();return t;}
TreeNode* RDparsinger::_ProcDecMore(){TreeNode* t = new TreeNode("ProcDecMore");if(currentToken->wd.tok == BEGIN){return t;}if(currentToken->wd.tok == PROCEDURE){t->addChild(_ProcDec());return t;}unExcept();return t;}
TreeNode* RDparsinger::_Exp(){TreeNode* t = new TreeNode("Exp");if(currentToken->wd.tok == INTC||currentToken->wd.tok == LPAREN||currentToken->wd.tok == ID){t->addChild(_Term());t->addChild(_OtherTerm());return t;}unExcept();return t;}
TreeNode* RDparsinger::_VarDecpart(){TreeNode* t = new TreeNode("VarDecpart");if(currentToken->wd.tok == BEGIN||currentToken->wd.tok == PROCEDURE){return t;}if(currentToken->wd.tok == VAR){t->addChild(_VarDec());return t;}unExcept();return t;}
TreeNode* RDparsinger::_AssignmentRest(){TreeNode* t = new TreeNode("AssignmentRest");if(currentToken->wd.tok == DOT||currentToken->wd.tok == LMIDPAREN||currentToken->wd.tok == ASSIGN){t->addChild(_VariMore());t->addChild(matchToken(ASSIGN));t->addChild(_Exp());return t;}unExcept();return t;}
TreeNode* RDparsinger::_VarDecList(){TreeNode* t = new TreeNode("VarDecList");if(currentToken->wd.tok == CHAR||currentToken->wd.tok == RECORD||currentToken->wd.tok == ID||currentToken->wd.tok == ARRAY||currentToken->wd.tok == INTEGER){t->addChild(_TypeDef());t->addChild(_VarldList());t->addChild(matchToken(SEMI));t->addChild(_VarDecMore());return t;}unExcept();return t;}
TreeNode* RDparsinger::_ParamMore(){TreeNode* t = new TreeNode("ParamMore");if(currentToken->wd.tok == RPAREN){return t;}if(currentToken->wd.tok == SEMI){t->addChild(matchToken(SEMI));t->addChild(_ParamDecList());return t;}unExcept();return t;}
TreeNode* RDparsinger::_CallStmRest(){TreeNode* t = new TreeNode("CallStmRest");if(currentToken->wd.tok == LPAREN){t->addChild(matchToken(LPAREN));t->addChild(_ActParamList());t->addChild(matchToken(RPAREN));return t;}unExcept();return t;}
TreeNode* RDparsinger::_InputStm(){TreeNode* t = new TreeNode("InputStm");if(currentToken->wd.tok == READ){t->addChild(matchToken(READ));t->addChild(matchToken(LPAREN));t->addChild(_Invar());t->addChild(matchToken(RPAREN));return t;}unExcept();return t;}
TreeNode* RDparsinger::_CmpOp(){TreeNode* t = new TreeNode("CmpOp");if(currentToken->wd.tok == LT){t->addChild(matchToken(LT));return t;}if(currentToken->wd.tok == EQ){t->addChild(matchToken(EQ));return t;}unExcept();return t;}
TreeNode* RDparsinger::_AddOp(){TreeNode* t = new TreeNode("AddOp");if(currentToken->wd.tok == PLUS){t->addChild(matchToken(PLUS));return t;}if(currentToken->wd.tok == MINUS){t->addChild(matchToken(MINUS));return t;}unExcept();return t;}
TreeNode* RDparsinger::_ProgramBody(){TreeNode* t = new TreeNode("ProgramBody");if(currentToken->wd.tok == BEGIN){t->addChild(matchToken(BEGIN));t->addChild(_StmList());t->addChild(matchToken(END));return t;}unExcept();return t;}
TreeNode* RDparsinger::_TypeDecMore(){TreeNode* t = new TreeNode("TypeDecMore");if(currentToken->wd.tok == VAR||currentToken->wd.tok == BEGIN||currentToken->wd.tok == PROCEDURE){return t;}if(currentToken->wd.tok == ID){t->addChild(_TypeDecList());return t;}unExcept();return t;}
TreeNode* RDparsinger::_Param(){TreeNode* t = new TreeNode("Param");if(currentToken->wd.tok == CHAR||currentToken->wd.tok == RECORD||currentToken->wd.tok == ID||currentToken->wd.tok == ARRAY||currentToken->wd.tok == INTEGER){t->addChild(_TypeDef());t->addChild(_FormList());return t;}if(currentToken->wd.tok == VAR){t->addChild(matchToken(VAR));t->addChild(_TypeDef());t->addChild(_FormList());return t;}unExcept();return t;}
TreeNode* RDparsinger::_LoopStm(){TreeNode* t = new TreeNode("LoopStm");if(currentToken->wd.tok == WHILE){t->addChild(matchToken(WHILE));t->addChild(_RelExp());t->addChild(matchToken(DO));t->addChild(_StmList());t->addChild(matchToken(ENDWH));return t;}unExcept();return t;}
TreeNode* RDparsinger::_OutputStm(){TreeNode* t = new TreeNode("OutputStm");if(currentToken->wd.tok == WRITE){t->addChild(matchToken(WRITE));t->addChild(matchToken(LPAREN));t->addChild(_Exp());t->addChild(matchToken(RPAREN));return t;}unExcept();return t;}
TreeNode* RDparsinger::_Term(){TreeNode* t = new TreeNode("Term");if(currentToken->wd.tok == INTC||currentToken->wd.tok == LPAREN||currentToken->wd.tok == ID){t->addChild(_Factor());t->addChild(_OtherFactor());return t;}unExcept();return t;}
TreeNode* RDparsinger::_FieldVar(){TreeNode* t = new TreeNode("FieldVar");if(currentToken->wd.tok == ID){t->addChild(matchToken(ID));t->addChild(_FieldVarMore());return t;}unExcept();return t;}
TreeNode* RDparsinger::_FieldDecList(){TreeNode* t = new TreeNode("FieldDecList");if(currentToken->wd.tok == CHAR||currentToken->wd.tok == INTEGER){t->addChild(_BaseType());t->addChild(_IdList());t->addChild(matchToken(SEMI));t->addChild(_FieldDecMore());return t;}if(currentToken->wd.tok == ARRAY){t->addChild(_ArrayType());t->addChild(_IdList());t->addChild(matchToken(SEMI));t->addChild(_FieldDecMore());return t;}unExcept();return t;}
TreeNode* RDparsinger::_ProcBody(){TreeNode* t = new TreeNode("ProcBody");if(currentToken->wd.tok == BEGIN){t->addChild(_ProgramBody());return t;}unExcept();return t;}
TreeNode* RDparsinger::_ActParamMore(){TreeNode* t = new TreeNode("ActParamMore");if(currentToken->wd.tok == RPAREN){return t;}if(currentToken->wd.tok == COMMA){t->addChild(matchToken(COMMA));t->addChild(_ActParamList());return t;}unExcept();return t;}
TreeNode* RDparsinger::_FieldDecMore(){TreeNode* t = new TreeNode("FieldDecMore");if(currentToken->wd.tok == END){return t;}if(currentToken->wd.tok == CHAR||currentToken->wd.tok == ARRAY||currentToken->wd.tok == INTEGER){t->addChild(_FieldDecList());return t;}unExcept();return t;}
TreeNode* RDparsinger::_TypeId(){TreeNode* t = new TreeNode("TypeId");if(currentToken->wd.tok == ID){t->addChild(matchToken(ID));return t;}unExcept();return t;}
TreeNode* RDparsinger::_Low(){TreeNode* t = new TreeNode("Low");if(currentToken->wd.tok == INTC){t->addChild(matchToken(INTC));return t;}unExcept();return t;}
TreeNode* RDparsinger::_ProcName(){TreeNode* t = new TreeNode("ProcName");if(currentToken->wd.tok == ID){t->addChild(matchToken(ID));return t;}unExcept();return t;}
TreeNode* RDparsinger::_StructureType(){TreeNode* t = new TreeNode("StructureType");if(currentToken->wd.tok == ARRAY){t->addChild(_ArrayType());return t;}if(currentToken->wd.tok == RECORD){t->addChild(_RecType());return t;}unExcept();return t;}
TreeNode* RDparsinger::_RelExp(){TreeNode* t = new TreeNode("RelExp");if(currentToken->wd.tok == INTC||currentToken->wd.tok == LPAREN||currentToken->wd.tok == ID){t->addChild(_Exp());t->addChild(_OtherRelE());return t;}unExcept();return t;}
TreeNode* RDparsinger::_TypeDecList(){TreeNode* t = new TreeNode("TypeDecList");if(currentToken->wd.tok == ID){t->addChild(_TypeId());t->addChild(matchToken(EQ));t->addChild(_TypeDef());t->addChild(matchToken(SEMI));t->addChild(_TypeDecMore());return t;}unExcept();return t;}


//-------------------------------------------------------------------------------
