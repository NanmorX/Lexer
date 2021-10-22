// 10.10 先把所有情况都处理好，之后再优化
// 文件处理
// 图形化
// 错误处理！！！！
// （再说，要增加c语言的各类关键字
// 预处理：空格？？和注释
// 数字只考虑int！！！ 应该差不多了
#define nullptr NULL
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
using namespace std;

string IdentifierStr;
int Num_Id = 0;
double NumVal;
vector <string> Identify;

string KeyWord[6] = {
    "int", "void", "if", "else", "while", "return",
};

char Sign[18] = {
    '=', '+', '-', '*', '/', '=', '>', '>', '<', '<',
    '!', ';', ',', '(', ')', '{', '}', '#'
};

int inKeyWord(string target)
{
    for(int i = 0; i < 6; i++)
    {
        if(target == KeyWord[i])
            return i + 1;
    }
    return 0;
}

int inSign(char target)
{
    for(int i = 0; i < 18; i++)
    {
        if(target == Sign[i])
            return i + 9;
    }
    return 0;
}

// gettok - Return the next token from standard input
static int gettok()
{
    static int LastChar = ' ';

    while(isspace(LastChar))
    {
        LastChar = getchar();
    }

    if(isalpha(LastChar))
    {
        IdentifierStr = LastChar;
        while(isalnum(LastChar = getchar()))
            IdentifierStr += LastChar;

        int t = inKeyWord(IdentifierStr);
        if(t)
        {
            cout<<"($"<<IdentifierStr<<", -)"<<endl;
            return t;
        }
        
        if(find(Identify.begin(), Identify.end(), IdentifierStr) == Identify.end())
        {
            Num_Id++;
            Identify.push_back(IdentifierStr);
            cout<<"("<<IdentifierStr<<", "<<Num_Id<<")"<<endl;
        }
        else
        {
            int num = find(Identify.begin(), Identify.end(), IdentifierStr) - Identify.begin() + 1;
            cout<<"("<<IdentifierStr<<", "<<num<<")"<<endl;
        }
        return 7;
    }
    // 不考虑.d形式
    else if(isdigit(LastChar))
    {
        string NumStr;
        NumStr += LastChar;
        if(LastChar != '0')
        {
            LastChar = getchar();
            while(isdigit(LastChar))
            {
                NumStr += LastChar;
                LastChar = getchar();
            }
        }
        else
        {
            LastChar = getchar();
            if(isdigit(LastChar))
                return -1;
        }

        NumVal = strtod(NumStr.c_str(), nullptr);
        cout<<"("<<NumVal<<", -)"<<endl;
        return 8;
    }
    // 先特殊处理=和==， <和<=, >和>=, != 
    else if(LastChar == '!')
    {
        LastChar = getchar();
        cout<<"($!=, -)"<<endl;
        LastChar = getchar();
        return 19;
    }
    else if(LastChar == '=')
    {
        LastChar = getchar();
        if(LastChar == '=')
        {
            cout<<"($==, -)"<<endl;
            LastChar = getchar();
            return 14;
        }
        else
        {
            cout<<"($=, -)"<<endl;
            return 9;
        }
    }
    else if(LastChar == '<')
    {
        LastChar = getchar();
        if(LastChar == '=')
        {
            cout<<"($<=, -)"<<endl;
            LastChar = getchar();
            return 18;
        }
        else
        {
            cout<<"(<, -)"<<endl;
            return 17;
        }
    }
    else if(LastChar == '>')
    {
        LastChar = getchar();
        if(LastChar == '=')
        {
            cout<<"($>=, -)"<<endl;
            LastChar = getchar();
            return 16;
        }
        else
        {
            cout<<"($>, -)"<<endl;
            return 15;
        }
    }
    else if(inSign(LastChar))
    {
        int t = inSign(LastChar);
        cout<<"($"<<(char)LastChar<<", -)"<<endl;
        LastChar = getchar();
        return t;
    }
    else if(LastChar == EOF)
    {
        return -1;
    }

    return 0;
}

int main()
{
    // // open txt file
    // ifstream infile;
    // infile.open("code.txt");
    // assert(infile.is_open());
    int res = 0;
    while(res>-1)
    {
        res = gettok();
        cout<<res<<endl;
    }
    system("pause");

    // infile.close();

    return 0;
}
