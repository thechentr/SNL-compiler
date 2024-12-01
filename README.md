# SNL Compiler  
This project includes:

* Lexical analysis program for the SNL language written in C
* Recursive descent parser for the SNL language written in C
* LL(1) parser for the SNL language written in C

Written in Python:

* Program to compute the Predict set for any language production rule
* Program to automatically generate the three C programs above for any language's production rules

Additionally:

* A txt document containing 104 production rules for the SNL language

For detailed information, please refer to my blog:  
[中文版](https://thechentr.github.io/2023/05/13/3_SNL%E8%AF%AD%E8%A8%80%E7%BC%96%E8%AF%91%E5%99%A8%EF%BC%88%E8%AF%AD%E6%B3%95%E5%88%86%E6%9E%90%E9%83%A8%E5%88%86%EF%BC%89/)  
[English version](https://thechentr.github.io/2024/09/12/20_SNL/)

In addition, this project provides test samples and executable program files. You can directly run `SNL_compiler\analyse.exe` to see the program's functionality.

For the automatic generation program, you can delete `SNL_compiler` and run `SNL Syntax Analyzer Code Generator.py` to regenerate the C language code. (This will regenerate a version of `SNL_compiler` identical to the previous one, but without the test samples and executable program files. You will need to recompile it.)
