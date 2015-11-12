#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
using namespace std;
enum Tag
{
    NUM=256,REAL,ID,IF,ELSE,FOR,WHILE,DO,BREAK,CONTINUE,MINUS,AND,OR,EQ,NE,GE,LE,STR,TYPE,KEY,SHARP
};
const int k_w_len=7;
const int t_w_len=5;
string keywords[k_w_len]={"if","else","for","while","do","break","continue"};
string typewords[t_w_len]={"int","float","double","char","bool"};
vector<string> idwords;
vector<string> strwords;
int get_keyindex(string a)
{
    for(int i=0;i<k_w_len;i++)
    {
        if(keywords[i]==a)
            return i;
    }
    return -1;
}
int get_typeindex(string a)
{
    for(int i=0;i<t_w_len;i++)
    {
        if(typewords[i]==a)
            return i;
    }
    return -1;
}
int get_idindex(string a)
{
    int len=idwords.size();
    for(int i=0;i<len;i++)
    {
        if(idwords[i]==a)
            return i;
    }
    idwords.push_back(a);
    return len;
}
int get_strindex(string a)
{
    int len=strwords.size();
    for(int i=0;i<len;i++)
    {
        if(strwords[i]==a)
            return i;
    }
    strwords.push_back(a);
    return len;
}
class Token
{
    private:
        int tag;
    public:
        Token(){}
        Token(int i)
        {
            tag=i;
        }
        int get_tag()
        {
            return tag;
        }
        virtual int get_numvalue(){}
        virtual double get_realvalue(){}
        virtual int get_lexeme(){}
        virtual string get_lexeme_str(){}
};
class Num: public Token
{
    private:
        int value;
    public:
        Num(int t,int v):Token(t),value(v){};
        int get_numvalue()
        {
            return value;
        }
};
class Real: public Token
{
    private:
        double value;
    public:
        Real(int t,double v):Token(t),value(v){};
        double get_realvalue()
        {
            return value;
        }
};
class Word: public Token
{
    private:
        int lexeme;
    public:
        Word(int t,int v):Token(t),lexeme(v){};
        int get_lexeme()
        {
            return lexeme;
        }
        string get_lexeme_str()
        {
            if((this->get_tag())==ID) return idwords[lexeme];
            else if((this->get_tag())==TYPE) return typewords[lexeme];
            else if((this->get_tag())==KEY) return keywords[lexeme];
            else return strwords[lexeme];
        }
};
bool isletter(char c)
{
    if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c=='_'))
        return true;
    else return false;
}
bool isdigit(char c)
{
    if(c>='0'&&c<='9')
        return true;
    else return false;
}
string symbols=",;+-*/(){}[]";
bool issymbol(char c)
{
    for(int i=0;i<symbols.length();i++)
    {
        if(c==symbols[i])
            return true;
    }
    return false;
}
bool error=false;
string buffer="";
string tmp="";
vector<Token*> Token_list;
void show_token_list()
{
    int len=Token_list.size();
        for(int i=0;i<len;i++)
        {
            int tmp=Token_list[i]->get_tag();
            cout<<tmp<<"\t";
            if(tmp<NUM)
            {
                cout<<char(tmp)<<endl;
            }
            else if(tmp==NUM)
            {
                cout<<Token_list[i]->get_numvalue()<<endl;
            }
            else if(tmp==REAL)
            {
                cout<<Token_list[i]->get_realvalue()<<endl;
            }
            else if(tmp==ID||tmp==STR||tmp==KEY||tmp==TYPE)
            {
                cout<<Token_list[i]->get_lexeme_str()<<endl;
            }
            else cout<<endl;
        }
}

//Syntax analysis
int cur_token;
int cur_tag;
void expr();
void term();
void expr1();
void term1();
void factor();
void expr()
{
    term();
    if(error) return ;
    expr1();
    if(error) return ;
}
void term()
{
    factor();
    if(error) return ;
    term1();
    if(error) return ;
}
void expr1()
{
    if(cur_tag=='+'||cur_tag=='-')
    {
        cur_token++;
        cur_tag=Token_list[cur_token]->get_tag();
        term();
        if(error) return ;
        expr1();
        if(error) return ;
    }
}
void term1()
{
    if(cur_tag=='*'||cur_tag=='/')
    {
        cur_token++;
        cur_tag=Token_list[cur_token]->get_tag();
        factor();
        if(error) return ;
        term1();
        if(error) return ;
    }
}
void factor()
{
    if(!(cur_tag==NUM||cur_tag==REAL||cur_tag==ID))
    {
        if(cur_tag=='(')
        {
            cur_token++;
            cur_tag=Token_list[cur_token]->get_tag();
            expr();
            if(error) return ;
            if(cur_tag!=')')
            {
                error=true;
            }
        }
        else
        {
            error=true;
            return ;
        }
    }
    cur_token++;
    cur_tag=Token_list[cur_token]->get_tag();
}
int main()
{
    freopen("test.in","r",stdin);

    while(getline(cin,tmp))
    {
        buffer=buffer+tmp+"\n";
    }
    cout<<buffer<<endl;
    int buffer_size=buffer.length();
    int lexemeBegin=0,forword=0;
    while(lexemeBegin<buffer_size&&!error)
    {
        char c=buffer[forword];
        //cout<<"c=="<<c<<endl;
        while(c==' '||c=='\n'||c=='\t')
        {
            forword++;
            lexemeBegin++;
            if(forword>=buffer_size||lexemeBegin>=buffer_size)
                break;
            c=buffer[forword];
        }
        if(c=='<'||c=='>'||c=='='||c=='!')
        {
            forword++;
            if(forword<buffer_size&&buffer[forword]=='=')
            {
                    switch (c)
                    {
                                case '<':
                                    Token_list.push_back(new Token(LE));
                                    break;
                                case '>':
                                    Token_list.push_back(new Token(GE));
                                    break;
                                case '!':
                                    Token_list.push_back(new Token(NE));
                                    break;
                                case '=':
                                    Token_list.push_back(new Token(EQ));
                                    break;
                    }
                    forword++;
                    lexemeBegin+=2;
            }
            else
            {
                    Token_list.push_back(new Token(c));
                    lexemeBegin++;
            }
        }
        else if(isletter(c))
        {
            while(true)
            {
                forword++;
                if(forword>=buffer_size) break;
                c=buffer[forword];
                if(!(isletter(c)||isdigit(c))) break;
            }
            //cout<<"tokenlis push back="<<buffer.substr(lexemeBegin,forword-lexemeBegin)<<endl;
            //cout<<"now buffer[forword] is "<<buffer[forword]<<endl;
            string tmp=buffer.substr(lexemeBegin,forword-lexemeBegin);
            int index=get_keyindex(tmp);
            if(index!=-1)  Token_list.push_back(new Word(KEY,index));
            else
            {
                index=get_typeindex(tmp);
                if(index!=-1)   Token_list.push_back(new Word(TYPE,index));
                else
                    Token_list.push_back(new Word(ID,get_idindex(tmp)));
            }
            lexemeBegin=forword;
        }
        else if(isdigit(c))
        {
            //number
            //cout<<"number="<<c<<endl;
            bool endofinput=false;
            double ans=c-'0';
            while(true)
            {
                forword++;
                if(forword>=buffer_size)
                {
                    endofinput=true;
                    break;
                }
                c=buffer[forword];
                if(!isdigit(c))
                    break;
                ans=ans*10+c-'0';
            }
            if(endofinput)
            {
                Token_list.push_back(new Num(NUM,ans));
                lexemeBegin=forword;
            }
            else
            {
                if(c=='.')
                {
                    forword++;
                    if(forword>=buffer_size||!isdigit(buffer[forword]))
                        error=true;
                    else
                    {
                        double w=10;
                        ans+=(buffer[forword]-'0')/w;
                        while(true)
                        {
                            forword++;
                            w*=10;
                            if(forword>=buffer_size)
                            {
                                endofinput=true;
                                break;
                            }
                            c=buffer[forword];
                            if(!isdigit(c))
                                break;
                            ans=ans+(c-'0')/w;
                        }
                        if(endofinput)
                        {
                            Token_list.push_back(new Real(REAL,ans));
                            lexemeBegin=forword;
                        }
                        else
                        {
                            if(c=='e'||c=='E')
                            {
                                double ord=0;
                                forword++;
                                if(forword>=buffer_size)
                                {
                                    error=true;
                                }
                                else
                                {
                                    c=buffer[forword];
                                    bool ispos=true;
                                    if(c=='+'||c=='-')
                                    {
                                        if(c=='-')  ispos=false;
                                        forword++;
                                        if(forword>=buffer_size||!isdigit(buffer[forword]))
                                            error=true;
                                        else
                                        {
                                            c=buffer[forword];
                                            ord=c-'0';
                                        }
                                    }
                                    else if(!isdigit(c))
                                    {
                                        error=true;
                                    }
                                    else
                                    {
                                        ord=c-'0';
                                    }
                                    if(!error)
                                    {
                                        while(true)
                                        {
                                             forword++;
                                             if(forword>=buffer_size)   break;
                                             c=buffer[forword];
                                             if(!isdigit(c)) break;
                                             ord=ord*10+c-'0';
                                        }
                                        if(!ispos) ord=-ord;
                                        ans=ans*pow(10,ord);
                                        Token_list.push_back(new Real(REAL,ans));
                                        lexemeBegin=forword;
                                    }
                                }
                            }
                            else
                            {
                                Token_list.push_back(new Real(REAL,ans));
                                lexemeBegin=forword;
                            }
                        }
                    }
                }
                else if(c=='e'||c=='E')
                {
                    double ord=0;
                    forword++;
                    if(forword>=buffer_size)
                    {
                        error=true;
                    }
                    else
                    {
                        c=buffer[forword];
                        bool ispos=true;
                        if(c=='+'||c=='-')
                        {
                            if(c=='-')  ispos=false;
                            forword++;
                            if(forword>=buffer_size||!isdigit(buffer[forword]))
                                error=true;
                            else
                            {
                                c=buffer[forword];
                                ord=c-'0';
                            }
                        }
                        else if(!isdigit(c))
                        {
                            error=true;
                        }
                        else
                        {
                            ord=c-'0';
                        }
                        if(!error)
                        {
                            while(true)
                            {
                                 forword++;
                                 if(forword>=buffer_size)   break;
                                 c=buffer[forword];
                                 if(!isdigit(c)) break;
                                 ord=ord*10+c-'0';
                            }
                            if(!ispos) ord=-ord;
                            ans=ans*pow(10,ord);
                            Token_list.push_back(new Real(REAL,ans));
                            lexemeBegin=forword;
                        }
                    }
                }
                else
                {
                    Token_list.push_back(new Num(NUM,int(ans)));
                    lexemeBegin=forword;
                }
            }
        }
        else if(c=='\'')
        {
            if((forword+2>=buffer_size)||(buffer[forword+2]!='\''))
            {
                error=true;
            }
            else
            {
                Token_list.push_back(new Num(NUM,buffer[forword+1]));
                forword+=3;
                lexemeBegin+=3;
            }
        }
        else if(c=='"')
        {
            do
            {
                forword++;
                if(forword>=buffer_size)
                {
                    error=true;
                    break;
                }
                c=buffer[forword];
            }while(buffer[forword]!='"');
            if(!error)
            {
                Token_list.push_back(new Word(STR,get_strindex(buffer.substr(lexemeBegin+1,forword-lexemeBegin-1))));
                forword++;
                lexemeBegin=forword;
            }
        }
        else if(issymbol(c))
        {
            Token_list.push_back(new Token(c));
            forword++;
            lexemeBegin++;
        }
    }
    if(error)
    {
        cout<<"Lexical analysis error!"<<endl;
    }
    else
    {
        show_token_list();
        cout<<"Lexical analysis accepted!"<<endl;
        Token_list.push_back(new Token(SHARP));
        cur_token=0;
        cur_tag=Token_list[cur_token]->get_tag();
        expr();
        if(Token_list[cur_token]->get_tag()!=SHARP)
            error=true;
        if(error==false)
        {
            cout<<"Syntax analysis accepted!"<<endl;
        }
        else
        {
            cout<<"Syntax analysis error!"<<endl;
        }
    }
    return 0;
}
