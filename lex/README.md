# lex部分说明  
  
## lex是什么
lex的输入为1个.lex文本文件，声明了词法规则和对词法分析器的要求。  
输出为若干个.c(.cpp,.h,.hpp等)文件，可编译生成一个可执行文件，为符合输入的.lex要求的扫描器。  
因此lex所需实现的功能应包括：读取并理解.lex文件->识别词法规则->构建DFA状态图->根据DFA状态图输出代码。  
  
## 自定义lex规则
使用lex时，通过命令行参数给出输入文件路径。  
输出文件输出至lex所在目录下的out文件夹，输出为3个文件：main.cpp, mxf_Scanner.hpp, mxf_Scanner.cpp  
对输入.lex文件的要求参照下一部分的简化版要求。
由lex定义而非由使用lex的使用者定义，可在生成的Scanner使用的变量/操作如下：  
* class mxf_Scanner
* mxf_Scanner类型全局变量mxf_Scanner
* 扫描状态#define INITIAL 0
* 切换扫描状态函数#define BEGIN(state)
  
## .lex文件规则(简化)
### 第一部分 全局声明
该部分由两个小部分组成：  
* 第一小部分为c全局声明，该部分的所有内容会被复制至输出文件main.cpp的开头处。  
c全局声明写在%{...%}内，可以不加。  
* 第二小部分为lex声明，可声明符号和状态，每个声明一行。  
符号声明先是由大/小写字母和下划线组成的符号名，使用空格或tab隔开后是该符号的正规式  
状态声明为%x <状态名>
* lex声明部分各行行尾不允许出现多余的空格

全局部分由%%结尾

例：  
```
  %{
  int wordCounter;
  %}
  %x COMMENT
  nondigit    ([_A-Za-z])
  digit       ([0-9])
  integer     ({digit}+)
  identifier  ({nondigit}({nondigit}|{digit})*)
  blank_chars ([ \f\r\t\v]+)
  %%
```
### 第二部分 符号及对应操作规则
* 每行为一个所需识别的标记以及其对应的操作，操作使用{}包括，为简化lex，不允许在{}内部出现其他的}  
* 已声明的标识符用{}表示  
* \n以外的未声明的标识符用""表示  
* .表示匹配任何\n以外的字符  
* 可以使用```<状态名>标识符```的方法表示在特定状态下需要进行的识别  
简单来说，仅存在5中表示:```{已声明标记},"未声明标记",\n,.,<模式名>前四种标记```  
例：  
```
{identifier}    { /*do something*/ }
{integer}       { /*do something*/ }
{blank_chars}   { /*do nothing*/ }
\n            { /*do nothing*/ }
"//"          { BEGIN COMMENT; }
.             { /*do something*/ }
<COMMENT>\n   { BEGIN INITIAL; }
<COMMENT>.    { /*do nothing*/ }
%% 
```
### 第三部分 C代码
该部分的内容应完整复制至main.cpp文件的末尾  
  
## 正规式书写规则（简化）
* ()标识优先级
* \[]方括号内可包含若干个字符，匹配括号内任意一个字符
* {}大括号内为已经全局声明过的标记
* \[]与{}不可自行嵌套、互相嵌套或交叉
* -指定范围
* \*附加在上述限定符之后，表示匹配0或多个限定符内的模式
* +附加在上述限定符之后，表示匹配1或多个限定符内的模式
* \\转义字符，也可用于表示该规则内出现符号的原符号
* |左右两个模式的两模式的或
* 上述符号非转义时，-,\\只能出现在\[]内部，(,),{,},\[,],+,\*,|不能出现在\[]内部，+,\*只能出现在],),}之后，|前必须为+,\*,),],}，|后必须为(,\[,{
* 每个正规式的最后一个字符(换行符之前)必须是)
