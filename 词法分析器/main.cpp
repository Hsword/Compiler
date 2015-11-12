#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
typedef long long LL;
bool isnum(char c)
{
    if(c>='0'&&c<='1') return true;
    else return false;
}
bool isapha(char c)
{
    if((c>='a'&&c<='z')||(c>='A'&&c<='Z')) return true;
    else return false;
}
const int maxn=100;
struct Token
{
    int id;
    string ptr;
};
Token token[maxn];
string key[50]={"int","main","void","if","else","char",">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")","[","]"};
int num_key=24;
string str[50];
int num_str=0;
int num_token=0;
int get_id(string tmp)
{
    //cout<<"***   "<<tmp<<endl;
    for(int i=0;i<num_key;i++)
    {
        if(tmp==key[i])
            return i+4;
    }
    bool in_list=false;
    for(int i=0;i<num_str;i++)
    {
        if(tmp==str[i])
            in_list=true;
    }
    if(!in_list)
        str[num_str++]=tmp;
    return 0;
}
Token get_token(string &tmp)
{
    Token ans;
    if(isnum(tmp[0]))
        ans.id=3;
    else if(tmp[0]=='\'')
        ans.id=1;
    else if(tmp[0]=='"')
        ans.id=2;
    else
        ans.id=get_id(tmp);
    //ans.ptr=(char*)tmp.c_str();
    ans.ptr=tmp;
    return ans;
}
void Clear(string& tmp)
{
    if(tmp!="")
                token[num_token++]=get_token(tmp);
    tmp="";
}
int main()
{
    char c;
    string tmp;
    tmp.clear();
    bool ok=true;
    bool have_read=false;
    while(true)
    {
        if(have_read)
        {
            have_read=false;
        }
        else
        {
            if(scanf("%c",&c)==EOF) break;
        }
        if(c==' '||c=='\n'||c=='\t')
        {
            Clear(tmp);
            while(c==' '||c=='\n'||c=='\t')
            {
                c=getchar();
            }
        }
        if(isalpha(c)||isnum(c))
            tmp.append(1,c);
        else
        {
            Clear(tmp);
            tmp.append(1,c);
            if(c=='\'')
            {
                if(scanf("%c",&c)==EOF) {ok=false;break;}
                tmp.append(1,c);
                if(scanf("%c",&c)==EOF||c!='\'') {ok=false;break;}
                tmp.append(1,c);
                Clear(tmp);
            }
            else if(c=='"')
            {
                while((scanf("%c",&c)!=EOF)&&(c!='"'))
                    tmp.append(1,c);
                if(c!='"') {ok=false;break;}
                tmp.append(1,c);
                Clear(tmp);
            }
            else if(c=='>'||c=='<'||c=='=')
            {
                if(scanf("%c",&c)==EOF)
                    Clear(tmp);
                else
                {
                    if(c=='=')
                    {
                        tmp.append(1,c);
                        Clear(tmp);
                    }
                    else
                    {
                        Clear(tmp);
                        have_read=true;
                    }
                }
            }
            else
            {
                Clear(tmp);
            }
        }
    }
    if(!ok)
        cout<<"error"<<endl;
    else
    {
        for(int i=0;i<num_token;i++)
        {
            printf("<%d>\t",token[i].id);
            cout<<token[i].ptr<<endl;
        }
    }
    return 0;
}
