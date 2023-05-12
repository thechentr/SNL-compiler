import copy
import os

NonTerminal = set()  # 非终极符集合
Terminal = set()  # 终极符集合
gens = []  # 产生式集合


def readGens():
    with open(".\\src\\SNL语言语法产生式.txt", "r") as f:
        lines = f.read().split('\n')

    for line in lines:
        symbols = line.split()
        # 分割后为(序号， 产生式左部, [产生式右部], [predict集(待填写)])
        gens.append((int(symbols[0][:-1]), symbols[1], symbols[3:], []))

    # 统计非终极符号和终极符
    for gen in gens:
        symbols = [gen[1]] + gen[2]  # 左部加右部
        for symbol in symbols:
            if symbol == '::=':
                continue
            if symbol[0] != '@':
                NonTerminal.add(symbol)
            else:
                Terminal.add(symbol[1:])


def enum() -> str:
    """
    枚举所有终极符和非终极符，
    :return: 枚举代码
    """
    # 枚举所有非终极符
    code = ""
    enumNT = "typedef enum {"
    for n in NonTerminal:
        enumNT += n + ", \n"
    enumNT += "}NonTerminal;"
    code += enumNT + '\n' + '\n'

    # 枚举所有终极符
    enumT = "typedef enum {"
    for n in Terminal:
        enumT += n + ", \n"
    enumT += "}Terminal;"
    code += enumT + '\n'
    return code


def getStr() -> str:
    """
    定义（非）终极符  枚举类型->字符串 的转换函数
    :return: 函数代码
    """
    code = ""
    # 完成终结符和非终结符 枚举类型->字符串 的转换
    get_str = "string getString(Terminal t){if (t == -1)return NULL;"
    for n in Terminal:
        get_str += f"else if (t == {n}) return \"{n}\";\n"
    get_str += "return NULL;}"
    code += get_str + '\n' + '\n'

    get_str = "string getString(NonTerminal t){if (t == -1)return NULL;"
    for n in NonTerminal:
        if n:
            get_str += f"else if (t == {n}) return \"{n}\";\n"
    get_str += "return NULL;}"
    code += get_str + '\n'
    return code


# ll1matrix分析矩阵构造
def LL1matrix() -> str:
    recode = ""
    for gen in gens:
        code = "temp = new vector<Symbol>;"
        gen[2].reverse()
        for s in gen[2]:
            code += "temp->push_back("
            if s[0] == "@":
                code += f"{{1, nullNonTerminal, {s[1:]}}}"
                # code += f"s.kind = 1;s.t = {s[1:]};"
            else:
                code += f"{{0, {s}, nullTerminal}}"
                # code += f"s.kind = 0;s.n = {s};"
            code += ");"
        gen[2].reverse()
        for p in gen[3]:
            code += f"LL1matrix[{gen[1]}][{p}] = temp;"
        recode += code + '\n'
    return recode


# 递归下降函数声明
def rdDec():
    dec = ""
    for nonTerminal in NonTerminal:
        if nonTerminal == "nullNonTerminal":
            continue
        dec += f"TreeNode* _{nonTerminal}();\n"
    return dec


# 递归下降函数定义
def rdFuc():
    code = ""
    for nonTerminal in NonTerminal:
        if nonTerminal == "nullNonTerminal":
            continue
        func = f"TreeNode* RDparsinger::_{nonTerminal}(){{TreeNode* t = new TreeNode(\"{nonTerminal}\");"

        # 根据产生式和predict写函数
        for gen in gens:
            if gen[1] == nonTerminal:
                func += "if("
                for p in gen[3]:
                    if func[-1] != '(':
                        func += "||"
                    func += f"currentToken->wd.tok == {p}"
                func += "){"
                for s in gen[2]:
                    if s[0] == "@":
                        if s != "@NONE":
                            func += f"t->addChild(matchToken({s[1:]}));"
                    else:
                        func += f"t->addChild(_{s}());"
                func += "return t;}"
        func += "unExcept();return t;}\n"
        code += func
    return code


def Predict():
    # 求每个符号的first集
    first = dict()
    for n in NonTerminal:
        first[n] = set()

    while True:
        # 此处需要深拷贝
        temp = copy.deepcopy(first)
        for n in NonTerminal:
            for gen in gens:
                # 寻找以该终结符为左部的产生式
                if gen[1] == n:
                    # 遍历当前产生式的右部求左部的first集
                    for x in gen[2]:
                        if x[0] == '@':
                            # 是终极符则加入first集合后跳出
                            first[n].add(x[1:])
                            break
                        else:
                            # 是非终极符则将该符号的first集加入first集合，推导不出空则继续推导
                            first[n] = first[n].union(first[x])
                            if "NONE" not in first[x]:
                                break
        if first == temp:
            break

    # 求每个符号的follow集
    follow = dict()
    for n in NonTerminal:
        follow[n] = set()

    while True:
        # 此处需要深拷贝
        temp = copy.deepcopy(follow)
        for n in NonTerminal:
            for gen in gens:
                # 遍历产生式右部寻找当前非终极符的身影
                for i in range(len(gen[2])):
                    if gen[2][i] == n:
                        # 从该非终结符后一位开始找
                        it = i + 1
                        while it != len(gen[2]):
                            if gen[2][it][0] == '@':  # 是终结符就加入后退出
                                follow[n].add(gen[2][it][1:])
                                break
                            else:  # 是非终结符就将其first集合加入
                                follow[n] = follow[n].union(first[gen[2][it]])
                                if "NONE" not in first[gen[2][it]]:
                                    break
                                else:
                                    follow[n].remove("NONE")
                            it += 1
                        if it == len(gen[2]):  # 遍历到最后则将左部的follow集合加入
                            follow[n] = follow[n].union(follow[gen[1]])
        if follow == temp:
            break

    # 求每个产生式的Predict集
    for gen in gens:
        pre = set()
        i = 0
        # 遍历产生式右部
        while i != len(gen[2]):
            # 终极符
            if gen[2][i] == "@NONE":
                i += 1
                continue
            if gen[2][i][0] == '@':
                pre.add(gen[2][i][1:])
                break
            # 非终极符
            pre = pre.union(first[gen[2][i]])
            if "NONE" in first[gen[2][i]]:
                pre.remove("NONE")
            else:
                break
            i += 1
        if i == len(gen[2]):
            pre = pre.union(follow[gen[1]])
        # 将元素都添加到当前产生式的predict集中
        for p in pre:
            gen[3].append(p)


if __name__ == '__main__':

    readGens()

    Predict()

    print(f"共{len(gens)}条产生式\n")

    print(f"非终极符：{NonTerminal}\n共{len(NonTerminal)}个非终极符\n")

    print(f"终极符：{Terminal}\n共{len(Terminal)}个终极符\n")

    NonTerminal.add("nullNonTerminal")
    Terminal.add("nullTerminal")

    # 创建文件夹
    if not os.path.exists("..\\SNL_compiler"):
        os.mkdir("..\\SNL_compiler")

    # 生成 LL1parsing.h---------------------------------------------------------
    with open(".\\src\\LL1parsing", "r", encoding="UTF-8") as f:
        content = f.read()
    content = content.replace("![LL1matrix]", LL1matrix())
    with open("..\\SNL_compiler\\LL1parsing.h", "w", encoding="UTF-8") as f:
        f.write(content)
    print("\tLL1parsing.h 生成成功 ！")

    # 生成 PasingDec.h-----------------------------------------------------------
    with open(".\\src\\PasingDec", "r", encoding="UTF-8") as f:
        content = f.read()
    content = content.replace("![getStr]", getStr())
    content = content.replace("![enum]", enum())
    with open("..\\SNL_compiler\\PasingDec.h", "w", encoding="UTF-8") as f:
        f.write(content)
    print("\tPasingDec.h 生成成功 ！")

    # 生成 REparsing.h-----------------------------------------------------------
    with open(".\\src\\REparsing", "r", encoding="UTF-8") as f:
        content = f.read()
    content = content.replace("![rdDec]", rdDec())
    content = content.replace("![rdFuc]", rdFuc())
    with open("..\\SNL_compiler\\REparsing.h", "w", encoding="UTF-8") as f:
        f.write(content)
        print("\tREparsing.h 生成成功 ！")

    # 生成 Word.h---------------------------------------------------------------
    with open(".\\src\\Word", "r", encoding="UTF-8") as f:
        content = f.read()
    with open("..\\SNL_compiler\\Word.h", "w", encoding="UTF-8") as f:
        f.write(content)  # Word.h是事先写好的不需要修改
        print("\tWord.h 生成成功 ！")

    # 生成 analyse.cpp---------------------------------------------------------------
    with open(".\\src\\analyse", "r", encoding="UTF-8") as f:
        content = f.read()
    with open("..\\SNL_compiler\\analyse.cpp", "w", encoding="UTF-8") as f:
        f.write(content)  # analyse.cpp是事先写好的不需要修改
        print("\tanalyse.cpp 生成成功 ！")


    print("\nSNL语法分析程序生成完毕，请编译后运行 SNL_compiler/analyse.cpp")