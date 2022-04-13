#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <windows.h>
#include <winuser.h>
#include <thread>
#include <vector>

//if not rounded : fpml+s1.len+s2.len=float point max length
//#define fpml 100 //now editable in runtime
#define dbg false
#define floatExp true
#define enableComputingBreaker true
#define sqrtDbg false
//#define expShorten false

/*
this program can implement big decimal compute +, -, *, /, !, ^  ,
short compute also included, like 1+1=2 then *9=18,
you can ignore first value to use last result.
It can implement +,-,*,/,^(exp should be integer),!(should be integer).
If you got r in last round, you can enter a expression with one parameter loss,
like 1/,*9,!,2^,^2 , it will replace r to that blank parameter.
enter cpy to copy r into clipboard.
enter exp to represent r in scientific notation
you can also enter fpml to change fpml(float point max length) in runtime
(too high fpml can lead to crash, due to memory insufficient)
enter clear to clean the screen
*/
//Huang-Ku Yeh
using namespace std;
string add(string,string);
string sub(string,string);
string mul(string,string);
string div(string,string);
string divForSqrt(string,string,int);
void stringReverse(string&);
bool fullStringGreaterOrEqul(string,string);
string expo(string,string);
string shortExp(string);
string fac(string);
string preCut(string);
string postCut(string);
string cut(string);
bool isZero(string);
string sqrt(string);
string intToStr(int);
double stringToDouble(string);
string mod(string,string);
string cutForSqrt(string);
void computing();
string getSqrtApproximation(string);
string sqrtForExpo(string);
string nthRoot(string,string);
int getExp(string);

void toClipboard(const std::string &s);//from http://www.cplusplus.com/forum/general/48837/

bool rounded=false;

int fpml=100;

bool stillComputing=false;

bool globalErrorFlag=false;

unsigned long long llrand() {
    unsigned long long r = 0;

    for (int i = 0; i < 5; ++i) {
        r = (r << 15) | (rand() & 0x7FFF);
    }

    return r & 0xFFFFFFFFFFFFFFFFULL;
}

void tst()
{
    string bf;
    string ps;
    char buffer[1000];
    unsigned long long int lt;
    while(true)
    {
        lt=llrand();
        //cout<<lt<<endl;
        sprintf(buffer,"%llu",lt);
        ps=string(buffer);
        //cout<<ps<<endl;
        bf=ps;
        //cout<<bf<<endl;
        ps=expo(ps,"2");
        ps=sqrt(ps);
        if(ps!=bf||globalErrorFlag)
        {
            globalErrorFlag=false;
            cout<<"!";
            cout<<ps<<endl;
            cout<<bf<<endl;
            system("pause");
        }
        else cout<<".";
    }
}

int main()
{
    /*cout<<DBL_MAX<<endl;
    return 0;*/
    //cout<<expo("1.00000001","10")<<endl;
    /*string a,b;
    while(cin>>a>>b)
        cout<<nthRoot(a,b);
    //tst();


    /*string g="1";
    for(int i=1;i<63;i++)
    {
        g=mul(g,"2");
        g=add(g,"1");
    }
    cout<<g<<endl;
    //system("pause");
    long double bb;
    stringstream sss;
    sss<<g;
    sss>>bb;
    printf("%.50lf\n",sqrt(bb));
    //cout<<LDBL_DIG<<endl;
    system("pause");*/
    /*string s;
    while(cin>>s)
    {
        cout<<sqrt(s)<<endl;
    }*/
    if(!dbg)
    {
        cout<<"This program can implement big decimal compute over all float point or integer,\n";
        cout<<"with +, -, *, /, !(factorial), ^(only integer exp), @(sqrt), %(mod, not only integer), #(nth root) 8 operator\n";
        cout<<"e.g:-1.5+2 or -1.5^-2 or 7! or 9@ or .8/6 or -36.1%11.9\n";
    }
    stringstream ss;
    string str;
    string v1,v2,r="0";
    bool isV1Signed,isV2Signed;
    bool signFlag;
    char arithType=0;
    int buf;
    while(getline(cin,str))
    {
        if(str=="fpml")
        {
            //cout<<"enter to change"<<endl;
            cin>>fpml;
            if(fpml<0)fpml=0;
            cout<<"fpml="<<fpml<<endl<<endl;
            continue;
        }
        if(str=="help")
        {
            cout<<"This program can implement big decimal compute over all float point or integer,\n";
            cout<<"with +, -, *, /, !(factorial), ^(only integer exp), @(sqrt), %(mod, not only integer), #(nth root) 8 operator\n";
            cout<<"e.g:-1.5+2 or -1.5^-2 or 7! or 9@ or .8/6 or -36.1%11.9\n";
            continue;
        }
        if(str.find("fpml")!=string::npos)
        {
            str.erase(0,4);
            for(int i=0;i<str.length();i++)
            {
                if(str[i]<'0'||str[i]>'9')
                {
                    str.erase(0,1);
                    i--;
                }
                else break;
            }
            fpml=0;
            for(int i=0;i<str.length();i++)
            {
                fpml*=10;
                fpml+=str[i]-'0';
            }
            if(fpml<0)fpml=0;
            cout<<"fpml="<<fpml<<endl<<endl;
            continue;
        }
        rounded=false;
        if(str=="cpy"||str=="CPY")//copy last r to clipboard
        {
            toClipboard(r);
            cout<<r<<endl<<endl;
            continue;
        }
        if(str=="clear"||str=="CLEAR"||str=="cls"||str=="CLS")
        {
            system("cls");
            continue;
        }
        if(str=="exp"||str=="EXP")//if enter exp, it will return short exp result of r
        {
            cout<<shortExp(r)<<endl<<endl;
            continue;
        }
        if(str.length()==0)
        {
            //from https://stackoverflow.com/questions/4895541/how-to-go-to-the-previous-line-in-a-c-code
            //cause some problem with multi-line r
            /*CONSOLE_SCREEN_BUFFER_INFO coninfo;
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            GetConsoleScreenBufferInfo(hConsole, &coninfo);
            coninfo.dwCursorPosition.Y -= 1;    // move up one line
            SetConsoleCursorPosition(hConsole, coninfo.dwCursorPosition);*/
            //////
            str=(r+arithType+v2);
            cout<<str<<endl;
        }
        for(buf=0;buf<str.length();buf++)
        {
            if(str[buf]=='+'||str[buf]=='-'||str[buf]=='*'||str[buf]=='/'||str[buf]=='^'||str[buf]=='!'||str[buf]=='@'||str[buf]=='%'||str[buf]=='#')break;
        }
        //cout<<"buf="<<buf<<endl;
        if(buf>=str.length())
        {
            for(buf=0;buf<str.length();buf++)
            {
                if(str[buf]>'9'||str[buf]<'0')break;
            }
            if(buf==str.length())
            {
                //cout<<"Operator needed"<<endl;

                //r=add("0",str);
                r=cut(str);
                if(r=="-0") r="0";

                cout<<r<<endl<<endl;
                continue;
            }
        }
        if(str[buf]=='!'||str[buf]=='@') str.append("0");
        if(buf==0)
        {
            if(str[buf]=='-')
            {
                int tmp=1;
                for(tmp=1;tmp<str.length();tmp++)
                {
                    if(str[tmp]=='+'||str[tmp]=='-'||str[tmp]=='*'||str[tmp]=='/'||str[tmp]=='^'||str[tmp]=='!'||str[tmp]=='@'||str[tmp]=='%'||str[tmp]=='#')break;
                }
                if(tmp>=str.length())
                {
                    str.insert(0,r);
                    for(buf=1;buf<str.length();buf++)
                    {
                        if(str[buf]=='+'||str[buf]=='-'||str[buf]=='*'||str[buf]=='/'||str[buf]=='^'||str[buf]=='!'||str[buf]=='@'||str[buf]=='%'||str[buf]=='#')break;
                    }
                }
                else
                {
                    buf=tmp;
                }
            }
            else
            {
                str.insert(0,r);
                for(buf=1;buf<str.length();buf++)
                {
                    if(str[buf]=='+'||str[buf]=='-'||str[buf]=='*'||str[buf]=='/'||str[buf]=='^'||str[buf]=='!'||str[buf]=='@'||str[buf]=='%'||str[buf]=='#')break;
                }
            }
        }
        if((buf==str.length()-1)) str.append(r);//deal with like 1/
        //cout<<"str="<<str<<endl;
        str.insert(buf,1,' ');
        str.insert(buf+2,1,' ');
        //cout<<"str="<<str<<endl;
        ss.str(str);
        //cout<<"ss.str()="<<ss.str()<<endl;
        ss>>v1>>arithType>>v2;
        /*cout<<"v1="<<v1<<endl;
        cout<<"v2="<<v2<<endl;
        cout<<"ari="<<arithType<<endl;*/
        if(v1[0]=='-')
        {
            isV1Signed=true;
            v1.erase(0,1);
        }
        else isV1Signed=false;
        if(v2[0]=='-')
        {
            isV2Signed=true;
            v2.erase(0,1);
        }
        else isV2Signed=false;
        /*cout<<"v1="<<v1<<endl;
        cout<<"v2="<<v2<<endl;
        cout<<"v1s="<<isV1Signed<<endl;
        cout<<"v2s="<<isV2Signed<<endl;*/
        switch(arithType)
        {
            case '+':
                if(!isV1Signed&&!isV2Signed) r=add(v1,v2);
                else if(isV1Signed&&isV2Signed) r="-"+add(v1,v2);
                else if(isV1Signed&&!isV2Signed)
                {
                    if(fullStringGreaterOrEqul(v2,v1)) r=sub(v2,v1);
                    else r="-"+sub(v1,v2);
                }
                else if(!isV1Signed&&isV2Signed)
                {
                    if(fullStringGreaterOrEqul(v1,v2)) r=sub(v1,v2);
                    else r="-"+sub(v2,v1);
                }
                break;
            case '-':
                if(!isV1Signed&&!isV2Signed)
                {
                    if(fullStringGreaterOrEqul(v1,v2)) r=sub(v1,v2);
                    else r="-"+sub(v2,v1);
                }
                else if(isV1Signed&&isV2Signed)
                {
                    if(fullStringGreaterOrEqul(v2,v1)) r=sub(v1,v2);
                    r="-"+add(v1,v2);
                }
                else if(isV1Signed&&!isV2Signed) r="-"+add(v1,v2);
                else if(!isV1Signed&&isV2Signed) r=add(v1,v2);
                break;
            case '*':
                r=mul(v1,v2);
                if((isV1Signed&&!isV2Signed) || (!isV1Signed&&isV2Signed)) r.insert(0,1,'-');
                break;
            case '/':
                r=div(v1,v2);
                if((isV1Signed&&!isV2Signed) || (!isV1Signed&&isV2Signed)) r.insert(0,1,'-');
                break;
            case '^':
                if(isV2Signed) r=expo(v1,"-"+v2);
                else r=expo(v1,v2);
                if(isV1Signed&&((v2[v2.length()-1]-'0')%2==1)) r.insert(0,1,'-');
                break;
            case '!':
                r=fac(v1);
                //if(isV1Signed) r.insert(0,1,'-'); //if neg factorial define as 0
                break;
            case '@':
                if(isV1Signed)r="0";
                else r=sqrt(v1);
                break;
            case '%':
                if(isV1Signed&&!isV2Signed)
                {
                    r=sub(v2,mod(v1,v2));
                }
                else if(!isV1Signed&&isV2Signed)
                {
                    r="-"+sub(v2,mod(v1,v2));
                }
                else if(!isV1Signed&&!isV2Signed)
                {
                    r=mod(v1,v2);
                }
                else if(isV1Signed&&isV2Signed)
                {
                    r="-"+mod(v1,v2);
                }
                break;
            case '#':
                if(!isV1Signed&&!isV2Signed)r=nthRoot(v1,v2);
                else r="0";
                break;
            default:
                cout<<"Operator invalid"<<endl<<endl;
                cout<<arithType<<endl;
                continue;
        }
        //if(expShorten) r=shortExp(r);
        if(r=="-0") r="0";
        //cout<<"tmpr:"<<r<<endl;
        r=cut(r);
        cout<<r;
        if(rounded)cout<<" (Rounded)";
        cout<<endl<<endl;
        ss.str("");
        ss.clear();
        isV1Signed=isV2Signed=false;

    }
    return 0;
}

bool fullStringGreaterOrEqul(string s1,string s2)
{
    /*cout<<"grt"<<endl;
    cout<<"s1="<<s1<<endl;
    cout<<"s2="<<s2<<endl;
    cout<<"-----"<<endl;*/
    int buf=0,j;
    for(j=s1.length()-1;j>=0;j--) if(s1[j]=='.')break;
    if(j!=-1)buf=s1.length()-j-1;
    //cout<<"buf="<<buf<<endl;
    for(j=s2.length()-1;j>=0;j--) if(s2[j]=='.')break;
    if(j!=-1)buf-=(s2.length()-j-1);
    //cout<<"buf="<<buf<<endl;
    if(buf>0) s2.append(buf,'0');
    else if(buf<0) s1.append(-buf,'0');

    for(int i=0;i<s1.length();i++)
    {
        if(s1[i]=='.')
        {
            s1.erase(i,1);
            break;
        }
    }
    for(int i=0;i<s2.length();i++)
    {
        if(s2[i]=='.')
        {
            s2.erase(i,1);
            break;
        }
    }

    /*cout<<"!!!!grt!"<<endl;
    cout<<"s1="<<s1<<endl;
    cout<<"s2="<<s2<<endl;
    cout<<"-----"<<endl;*/

    if(s1.length()>s2.length()) return true;
    else if(s2.length()>s1.length()) return false;
    else
    {
        for(int i=0;i<s1.length();i++)
        {
            if(s1[i]>s2[i]) return true;
            else if(s2[i]>s1[i]) return false;
        }
        return true;
    }
}

void stringReverse(string &str)
{
    for(int i=0,temp=str.length();i<(temp/2);i++)
    {
        swap(str[i],str[temp-1-i]);
    }
}

string add(string s1,string s2)
{
    string f1,f2;
    bool isFs1,isFs2;
    int i;

    for(i=0;i<s1.length();i++) if(s1[i]=='.')break;
    if(i<s1.length())
    {
        f1=s1.substr(s1.find("."));
        f1.erase(0,1);
        s1=s1.substr(0,i);
        isFs1=true;
    }
    for(i=0;i<s2.length();i++) if(s2[i]=='.')break;
    if(i<s2.length())
    {
        f2=s2.substr(s2.find("."));
        f2.erase(0,1);
        s2=s2.substr(0,i);
        isFs2=true;
    }

    string rf(max(f1.length(),f2.length()),'0');
    //cout<<"------------"<<endl;
    //cout<<s1<<endl;
    stringReverse(s1);
    //cout<<s1<<endl;
    //cout<<s2<<endl;
    stringReverse(s2);
    //cout<<s2<<endl;
    string r(max(s1.length(),s2.length())+1,'0');

    //integer
    for(i=0;i<s1.length()&&i<s2.length();i++)
        r[i]=s1[i]+s2[i]-'0';
    for(;i<s1.length();i++)
        r[i]=s1[i];
    for(;i<s2.length();i++)
        r[i]=s2[i];

    //float point
    if(isFs1&&isFs2)
    {
        for(i=0;i<f1.length()&&i<f2.length();i++)
            rf[i]=f1[i]+f2[i]-'0';
        for(;i<f1.length();i++)
            rf[i]=f1[i];
        for(;i<f2.length();i++)
            rf[i]=f2[i];
        for(i=rf.length()-1;i>=1;i--)
        {
            if(rf[i]>'9')
            {
                rf[i-1]+=(rf[i]-'0')/10;
                rf[i]=(rf[i]-'0')%10+'0';
            }
        }
        if(rf[0]>'9')
        {
            rf[0]=(rf[0]-'0')%10+'0';
            r[0]++;
        }
    }
    else if(isFs1)rf=f1;
    else if(isFs2)rf=f2;
    else rf="";

    for(int j=rf.length()-1;j>=0;j--)
    {
        if(rf[j]=='0')rf=rf.substr(0,j);
        else break;
    }
    if(rf.length()==0)isFs1=isFs2=false;
    //cout<<r<<endl;
    for(i=0;i<r.length();i++)
    {
        if(r[i]>'9')
        {
            r[i+1]+=(r[i]-'0')/10;
            r[i]=(r[i]-'0')%10+'0';
        }
    }

    //cout<<r<<endl;
    stringReverse(r);
    if(r[0]=='0')r=r.substr(1,r.length()-1);
    if(r.length()==0)r="0";
    if(isFs1||isFs2)r=r+"."+rf;
    //if(expShorten) r=shortExp(r);
    //r=cut(r);
    return r;
}

//should be always be greater than 0
string sub(string s1,string s2)
{
    string f1,f2;
    bool isFs1=false,isFs2=false;
    int i;

    for(i=0;i<s1.length();i++)
        if(s1[i]=='.')break;
    if(i<s1.length())
    {
        f1=s1.substr(s1.find("."));
        f1.erase(0,1);
        s1=s1.substr(0,i);
        isFs1=true;
    }
    for(i=0;i<s2.length();i++)
        if(s2[i]=='.')break;
    if(i<s2.length())
    {
        f2=s2.substr(s2.find("."));
        f2.erase(0,1);
        s2=s2.substr(0,i);
        isFs2=true;
    }

    /*cout<<"s1="<<s1<<endl;
    cout<<"s2="<<s2<<endl;
    cout<<"f1="<<f1<<endl;
    cout<<"f1="<<f2<<endl;*/

    string rf(max(f1.length(),f2.length()),'0');
    stringReverse(s1);
    stringReverse(s2);
    string r(max(s1.length(),s2.length()),'0');

    //integer
    for(i=0;i<s2.length();i++)
        r[i]=s1[i]-s2[i]+'0';
    for(;i<s1.length();i++)
        r[i]=s1[i];

    //cout<<"r="<<r<<endl;
    //float point
    if(isFs1&&isFs2)
    {
        for(i=0;i<f1.length()&&i<f2.length();i++)
            rf[i]=f1[i]-f2[i]+'0';
        for(;i<f1.length();i++)
            rf[i]=f1[i];
        for(;i<f2.length();i++)
            rf[i]-=(f2[i]-'0');
        for(i=rf.length()-1;i>=1;i--)
        {
            if(rf[i]<'0')
            {
                rf[i-1]--;
                rf[i]+=10;
            }
        }
        if(rf[0]<'0')
        {
            rf[0]+=10;
            r[0]--;
        }
        //cout<<"rf="<<rf<<endl;
    }
    else if(isFs1)rf=f1;
    else if(isFs2)
    {
        for(i=f2.length()-1;i>=0;i--)
        {
            if(f2[i]!='0')break;
        }
        if(i==-1)rf="";
        else
        {
            r[0]--;
            rf[0]='9'+1-f2[0]+'0';
            for(i=1;i<f2.length();i++)
            {
                rf[i]='0'+'0'-f2[i];
            }
            for(i=f2.length()-1;i>0;i--)
            {
                if(rf[i]<'0')
                {
                    rf[i-1]--;
                    rf[i]+=10;
                }
            }
        }

    }
    else rf="";

    //cout<<"r="<<r<<endl;

    for(int j=rf.length()-1;j>=0;j--)
    {
        if(rf[j]=='0')rf=rf.substr(0,j);
        else break;
    }

    //cout<<"r="<<r<<endl;

    if(rf.length()==0)isFs1=isFs2=false;
    for(i=0;i<r.length();i++)
    {
        if(r[i]<'0')
        {
            r[i+1]--;
            r[i]+=10;
        }
    }

    //cout<<"r="<<r<<endl;

    //cout<<"!!!"<<rf[0]-'0'<<endl;
    stringReverse(r);
    if(isFs1||isFs2)r=r+"."+rf;
    //if(expShorten) r=shortExp(r);
    //r=cut(r);
    return r;
}

string mul(string s1,string s2)
{
    /*cout<<s1<<endl;
    cout<<s2<<endl;*/
    int f1Idx=0,f2Idx=0,buf=0;
    for(int i=0;i<s1.length();i++)
    {
        if(s1[i]=='.')
        {
            f1Idx=s1.length()-i-1;
            s1.erase(i,1);
            break;
        }
    }
    for(int i=0;i<s2.length();i++)
    {
        if(s2[i]=='.')
        {
            f2Idx=s2.length()-i-1;
            s2.erase(i,1);
            break;
        }
    }

    stringReverse(s1);
    stringReverse(s2);

    for(int i=0;i<s1.length();i++)s1[i]-='0';
    for(int i=0;i<s2.length();i++)s2[i]-='0';

    string r(s1.length()+s2.length()+2,0);
    for(int i=0;i<s1.length();i++)
    {
        for(int j=0;j<s2.length();j++)
        {
            r[i+j]+=s1[i]*s2[j];
            if(r[i+j]>9)
            {
                r[i+j+1]+=r[i+j]/10;
                r[i+j]%=10;
            }
        }
    }

    /*for(int i=0;i<r.length();i++)
    {
        if(r[i]>9)
        {
            r[i+1]+=r[i]/10;
            r[i]%=10;
        }
    }*/

    /*for(int i=0;i<r.length();i++)
    {
        printf("%d ",r[i]);
    }
    cout<<endl;*/
    /*for(int i=0;i<s1.length();i++)s1[i]+='0';
    cout<<"r="<<r<<endl;
    for(int i=0;i<s1.length();i++)s1[i]-='0';*/

    r.insert(f1Idx+f2Idx,1,'.'-'0');
    stringReverse(r);

    buf=0;
    for(int i=r.length()-1;i>=0;i--)
    {
        if(r[i]==0)buf++;
        else break;
    }
    if(buf!=0)r=r.substr(0,r.length()-buf);
    if(r[r.length()-1]==('.'-'0'))r=r.substr(0,r.length()-1);

    buf=0;
    for(int i=0;i<r.length();i++)
    {
        if(r[i]==0)buf++;
        else break;
    }
    if(buf!=0)r=r.substr(buf,r.length()-buf);
    if(r.length()==0) return "0";

    for(int i=0;i<r.length();i++)r[i]+='0';
    if(r[0]=='.')r.insert(0,1,'0');
    //cout<<"r="<<r<<endl;

    //stringReverse(r);
    //if(expShorten) r=shortExp(r);
    //r=cut(r);
    //cout<<"before return:"<<r<<endl;
    return r;
}

bool divStringGreaterThan(string s1,string s2)
{
    for(int i=0;i<s1.length();i++)
    {
        if(i>s2.length()) return true;
        if(s1[i]>s2[i]) return true;
        else if(s2[i]>s1[i]) return false;
    }
    if(s1.length()==s2.length()) return true;
    for(int i=s2.length()-1;i>=0;i--)
    {
        if(s2[i]!='0') return false;
    }
    return true;
}

string div(string s1,string s2)
{
    if(isZero(s2))return "0";
    //cout<<"a";
    /*cout<<"s1="<<s1<<"!"<<endl;
    cout<<"s2="<<s2<<"!"<<endl;
    system("pause");*/
    int fIdx=0,buf=0;
    int pos;
    pos=s1.find('.');
    if(pos!=string::npos)
    {
        fIdx-=s1.length()-pos-1;
        s1.erase(pos,1);
    }
    pos=s2.find('.');
    if(pos!=string::npos)
    {
        fIdx+=s2.length()-pos-1;
        s2.erase(pos,1);
    }
    //cout<<"b";
    /*
    for(int i=0;i<s1.length();i++)
    {
        if(s1[i]=='.')
        {
            fIdx-=s1.length()-i-1;
            s1.erase(i,1);
            break;
        }
    }
    for(int i=0;i<s2.length();i++)
    {
        if(s2[i]=='.')
        {
            fIdx+=s2.length()-i-1;
            s2.erase(i,1);
            break;
        }
    }*/
    int bufIdxForZero;
    for(bufIdxForZero=0;bufIdxForZero<s2.length();bufIdxForZero++)
    {
        if(s2[bufIdxForZero]!='0')break;
    }
    if(bufIdxForZero!=0)
    {
        s2.erase(0,bufIdxForZero);
        //s1.append(bufIdxForZero,'0');
    }
    //cout<<"c";
    /*cout<<"s1="<<s1<<"!"<<endl;
    cout<<"s2="<<s2<<"!"<<endl;
    system("pause");*/

    string r,bs,bs2;
    bs2=s2;
    bs=s1;
    bs.append(s2.length()*2,'0');
    //for(int i=0;i<buf.length();i++)buf[i]-='0';
    buf=0;
    int i,lenTest;
    bool isGreater;
    for(i=0;i<bs.length();i++)
    {
        //fpml
        if(isZero(bs))break;
        if(i>(s1.length()+s2.length()+fpml))
        {
            //cout<<"!"<<endl;
            rounded=true;
            break;
        }
        buf=0;
        isGreater=false;
        if(divStringGreaterThan(bs,bs2)) isGreater=true;
        while(divStringGreaterThan(bs,bs2))
        {
            lenTest=bs2.length()-bs.length();
            if(lenTest>0)
            {
                bs.append(lenTest,'0');
            }
            /*cout<<"now  bs="<<bs<<endl;
            cout<<"now bs2="<<bs2<<endl;
            cout<<"buf="<<buf<<endl;*/
            buf++;
            //cout<<"(";
            bs=sub(bs.substr(0,bs2.length()),bs2)+bs.substr(bs2.length(),bs.length()-bs2.length());
            //cout<<")";
            /*cout<<"!now  bs="<<bs<<endl;
            cout<<"!now bs2="<<bs2<<endl;
            cout<<"!buf="<<buf<<endl;*/
        }
        //cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
        if(isGreater)bs+="0";
        bs2.insert(0,1,'0');
        r+="0";
        r[r.length()-1]+=buf;
        //cout<<"!!!!!now r=!!!!!"<<r<<endl;
    }
    //cout<<"d";
    buf=s1.length()-s2.length()+fIdx+1;
    /*buf-=s2.length();
    buf+=fIdx;
    buf++;*/
    //cout<<"??"<<buf<<endl;
    if(buf<0)
    {
        r.insert(0,(1-buf),'0');
        /*for(i=buf;i<=0;i++)
        {
            r.insert(0,1,'0');
        }*/
        r.insert(1,1,'.');
    }
    else
    {
        if(r.length()<buf) r.append((buf)-r.length(),'0');
        else r.insert(buf,1,'.');
    }
    for(i=0;i<r.length();i++)
    {
        if(r[i]!='0')break;
    }
    if(i!=0)
    {
        r.erase(0,i);
    }
    if(r[0]=='.')r.insert(0,1,'0');
    //cout<<"i="<<i<<endl;
    //cout<<"fIdx="<<fIdx<<endl;
    /////////////////////////////////////////////////
    //if(expShorten) r=shortExp(r);
    ////////////////////////////////////////////
    //r=cut(r);
    //cout<<"e"<<endl;
    return r;
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!expo with dec is not completed(error too high)
string expo(string s,string n)
{
    vector<string> v;
    bool isDec=false;
    int dotIdx=n.find('.');
    string nf;
    /*for(int i=0;i<n.length();i++)
    {
        if(n[i]=='.')return "0";
    }*/
    if(dotIdx!=string::npos)
    {
        isDec=true;
        nf=n.substr(dotIdx,n.length()-dotIdx);
        nf.insert(0,1,'0');
        n=n.substr(0,dotIdx);
    }
    //cout<<"nf="<<nf<<endl;

    int e=0;
    bool isSigned=false;
    //stringReverse(n);
    if(n[0]=='-') isSigned=true;
    else e=n[0]-'0';
    for(int i=1;i<n.length();i++)
    {
        e*=10;
        e+=n[i]-'0';
    }

    string r,rf="1";
    //cout<<"s="<<s<<endl;
    //cout<<"e="<<e<<endl;
    int temp;
    int maxE;
    if(e==0) r="1";
    maxE=1;

    r=s;
    v.push_back(s);
    for(int i=2;i<=e;)
    {
        if(i<=e)
        {
            r=mul(r,r);
            v.push_back(r);
            maxE*=2;
        }
        i*=i;
        //r=mul(s,r);
    }
    temp=maxE;
    r="1";
    for(int i=v.size()-1;i>=0;i--)
    {
        temp=maxE;
        if(temp<=e)
        {
            /*cout<<"t:"<<temp<<" e:"<<e<<endl;
            for(int k=0;k<v.size();k++)
            {
                cout<<v[k]<<" ";
            }
            cout<<endl;*/
            e-=temp;
            r=mul(r,v[i]);
            i++;
            //cout<<"r="<<r<<" i="<<i<<endl;
            continue;
        }
        else maxE/=2;
    }
    if(isDec)
    {
        //cout<<"?";
        string bufStr=nf;
        string acString="1";
        string bufAcString=s;
        string buf;
        int len=s.length()+n.length()+sqrt(fpml);
        double tempForLen=1;
        double finalLen=(s.length()-s.find('.')-1)/(ceil(tempForLen/stringToDouble(nf)))+s.find('.')+1;
        int incCount=0;
        for(int step=0;step<=fpml||(incCount<sqrt(fpml));step++)
        {
            /*cout<<"bufStr="<<bufStr<<endl;
            cout<<"acString="<<acString<<endl;
            cout<<"bufAcstring="<<bufAcString<<endl;*/
            if(fullStringGreaterOrEqul(bufStr,acString))
            {
                bufStr=sub(bufStr,acString);
                rf=mul(rf,bufAcString);
                //rf=cutForSqrt(rf);
                rf=cut(rf);
                //cout<<"rf now="<<rf<<"-"<<endl;
                //cout<<"rf="<<rf<<endl;
                incCount++;
            }
            else
            {
                acString=div(acString,"2");
                buf=bufAcString;
                bufAcString=sqrtForExpo(bufAcString);
                if(buf==bufAcString) break;
                //if(bufAcString=="1") break;
                /*for(int i=2;i<bufAcString.length();i++)
                {
                    if(bufAcString[i]!='0')
                    {
                        len=bufAcString.length()-i;
                        break;
                    }
                }
                len+=sqrt(fpml)>bufAcString.length()?sqrt(fpml):bufAcString.length();*/
                //cout<<"len="<<len<<endl;
                //cout<<"@"<<acString<<endl;
                //cout<<"|"<<bufAcString<<endl;
                //bufAcString=cutForSqrt(bufAcString);
                bufAcString=bufAcString.substr(0,len>bufAcString.length()?bufAcString.length():len);
                //cout<<":"<<bufAcString<<endl;
            }
        }
        //cout<<"~rf="<<rf<<endl;
        //rf=rf.substr(0,finalLen);
        //cout<<"lest="<<bufStr<<endl;
        //cout<<"r="<<r<<endl;
        //cout<<"rf="<<rf<<endl;
    }

    if(!isSigned)
    {
        if(isDec) r=mul(r,rf);
        return r;
    }
    else
    {
        if(isDec) return div("1",mul(r,rf));
        return div("1",r);
    }
}

string shortExp(string r)
{
    bool neg=false;
    string bufExp;
    if(r[0]=='-')
    {
        neg=true;
        r.erase(0,1);
    }
    int j;
    for(j=0;j<r.length();j++)
    {
        if(r[j]=='.') break;
    }
    //only 1 digit
    if(j==r.length()&&r.length()==1)
    {
        if(neg) return ("-"+r);
        else return r;
    }
    //not found '.' means integer
    if(j==r.length())
    {
        int temp=r.length()-1;
        int k;
        for(k=r.length()-1;k>=0;k--)
        {
            if(r[k]!='0')break;
        }
        if(k!=r.length()-1) r.erase(k+1,r.length()-k);

        string bufExpStrRev="";
        while(temp>0)
        {
            //bufExp.insert(bufExp.length()-1,1,'0'+temp%10);
            //bufExp.append(1,'0'+temp%10);
            bufExpStrRev.append(1,'0'+temp%10);
            temp/=10;
        }
        stringReverse(bufExpStrRev);
        bufExp=bufExpStrRev;
        if(r.length()>=2)r.insert(1,1,'.');
        r.append(1,'e');
        r.append(bufExp);
        if(neg) return ("-"+r);
        else return r;
    }

    /*for(k=r.length()-1;k>=0;k--)
    {
        if(r[k]!='0')break;
    }
    if(k!=r.length()-1) r.erase(k+1,r.length()-k);*/

    // 0.xxx
    if(r[0]=='0'&&r[1]=='.')
    {
        for(j=2;j<r.length();j++)
        {
            if(r[j]!='0')break;
        }
        r.erase(0,j);
        if(r.length()>=2) r.insert(1,1,'.');
        r.append(1,'e');
        r.append(1,'-');
        j--;
        while(j>0)
        {
            bufExp.insert(0,1,'0'+j%10);
            j/=10;
        }
        r.append(bufExp);
        if(neg) return ("-"+r);
        else return r;
    }
    //lest, that is like xxxx.xxx
    r.erase(j,1);
    r.insert(1,1,'.');

    j--;
    if(j>0)
    {
        while(j>0)
        {
            bufExp.insert(0,1,'0'+j%10);
            j/=10;
        }
        r.append(1,'e');
        r.append(bufExp);
    }
    if(neg) return ("-"+r);
    else return r;
}

string preCut(string s)
{
    bool isSigned=false;
    if(s[0]=='-')
    {
        isSigned=true;
        s.erase(0,1);
    }
    int i;
    int dotPos=0;
    for(i=0;i<s.length();i++)
    {
        if(s[i]=='.')
        {
            dotPos=i;
            break;
        }
        if(s[i]!='0')
        {
            break;
        }
    }
    if(dotPos>1)
    {
        s.erase(0,dotPos);
    }
    else if(dotPos==0)
    {
        if(i!=0)s.erase(0,i);
    }
    if(s[0]=='.')s.insert(0,1,'0');
    if(isSigned)s.insert(0,1,'-');
    if(s.length()==0)return "0";
    return s;
}

string postCut(string s)
{
    int idx;
    int dotPos=0;
    for(int i=s.length()-1;i>=0;i--)
    {
        if(s[i]=='.')
        {
            dotPos=i;
            break;
        }
    }
    for(idx=s.length()-1;idx>=0;idx--)
    {
        if(s[idx]!='0')break;
    }
    if(dotPos!=0&&idx!=s.length()-1)
    {
        s.erase(idx+1,s.length()-idx);
    }
    if(s[s.length()-1]=='.')s.erase(s.length()-1,1);
    return s;
}

string cut(string s)
{
    s=preCut(s);
    s=postCut(s);
    return s;
}

string fac(string s)
{
    //cout<<"s="<<s<<endl;
    for(int i=0;i<s.length();i++)
    {
        if(s[i]=='.')return "0";
    }
    string r="1";
    while(!isZero(s))
    {
        r=mul(r,s);
        s=sub(s,"1");
        //s=cut(s);
    }
    return r;
}

bool isZero(string s)
{
    for(int i=0;i<s.length();i++)
    {
        if(s[i]>='1'&&s[i]<='9')return false;
    }
    return true;
}

string sqrt(string m)
{
    m=cut(m);
    //if(fullStringGreaterOrEqul("0",m)) return "0";
    if(m=="0") return "0";
    /*int temp=fpml;
    fpml=10;*/
    //string i="0";
    string x1,x2;
    /*while(fullStringGreaterOrEqul(m,mul(i,i)))
    {
        i=add(i,fullStringGreaterOrEqul(mul(m,"0.01"),"0.01")?mul(m,"0.01"):"0.01");
        //cout<<"!";
    }*/
    x1=getSqrtApproximation(m);
    if(sqrtDbg)cout<<"x1="<<x1<<endl;
    //string temp="2";
    int k;
    /*for(k=2;true;k++)
    {
        temp=mul(temp,"2");
        if(fullStringGreaterOrEqul(temp,m))break;
    }*/
    k=m.length()*3;
    k=sqrt(k);
    //k/=2;
    //k+=10;
    if(k<10)k=10;
    if(sqrtDbg)cout<<"k="<<k<<endl;
    //system("pause");
    int p=0;
    //x1="1000000000000000000000000000000000000000000000000000";
    string tmpCutStr;
    for(p=0;p<k;p++)
    {
        x2=m;
        x2=divForSqrt(x2,x1,m.length()>sqrt(fpml)?m.length():sqrt(fpml));
        x2=cut(x2);
        x2=add(x2,x1);
        x2=divForSqrt(x2,"2",m.length()>sqrt(fpml)?m.length():sqrt(fpml));
        x2=cut(x2);
        tmpCutStr=cutForSqrt(x2);
        tmpCutStr=cut(tmpCutStr);
        if(expo(tmpCutStr,"2")==m)
        {
            x2=tmpCutStr;
            break;
        }
        if(x1==x2)break;
        //if(sqrtDbg)cout<<"x1="<<x1<<endl;
        x1=x2;
        if(sqrtDbg)cout<<"x2="<<x2<<" p="<<p<<endl;

        //if(x2.length()>fpml)break;
    }
    if(expo(x2,"2")==m)return x2;
    globalErrorFlag=true;
    string appR=x2;
    //bool hasApp=false;
    if(enableComputingBreaker)
    {
        //hasApp=true;
        cout<<"Approximation: "<<x2<<endl;
        cout<<"Computing(press esc to break)";
    }
    x1=x2;
    thread r;
    if(enableComputingBreaker)
    {
        stillComputing=true;
        r=thread(computing);
    }
    if(sqrtDbg)cout<<"k="<<k<<endl;
    for(int j=0;j<(k>100?sqrt(k):10);j++)
    {
        x2=m;
        x2=div(x2,x1);
        x2=cut(x2);
        x2=add(x2,x1);
        x2=div(x2,"2");
        x2=cut(x2);
        if(x1==x2)break;
        //x2=cutForSqrt(x2);
        x1=x2;
        if(enableComputingBreaker&&!stillComputing)
        {
            x2=appR;
            r.join();
            break;
        }
        if(sqrtDbg)cout<<"?"<<x2<<endl;
        //if(x2.length()>fpml)break;
    }
    if(x2.length()>fpml)rounded=true;
    //else rounded=false;
    //fpml=temp;
    if(enableComputingBreaker)
    {
        if(stillComputing)
        {
            stillComputing=false;
            r.join();
        }
        else stillComputing=false;
        cout<<endl;
    }
    return x2;
}

/*
Disable round function,
to prevent error positive judge.
*/
string divForSqrt(string s1,string s2,int fm)
{
    if(isZero(s2))return "0";
    /*cout<<"s1="<<s1<<"!"<<endl;
    cout<<"s2="<<s2<<"!"<<endl;
    system("pause");*/
    int fIdx=0,buf=0;
    for(int i=0;i<s1.length();i++)
    {
        if(s1[i]=='.')
        {
            fIdx-=s1.length()-i-1;
            s1.erase(i,1);
            break;
        }
    }
    for(int i=0;i<s2.length();i++)
    {
        if(s2[i]=='.')
        {
            fIdx+=s2.length()-i-1;
            s2.erase(i,1);
            break;
        }
    }
    int bufIdxForZero;
    for(bufIdxForZero=0;bufIdxForZero<s2.length();bufIdxForZero++)
    {
        if(s2[bufIdxForZero]!='0')break;
    }
    if(bufIdxForZero!=0)
    {
        s2.erase(0,bufIdxForZero);
        //s1.append(bufIdxForZero,'0');
    }
    /*cout<<"s1="<<s1<<"!"<<endl;
    cout<<"s2="<<s2<<"!"<<endl;
    system("pause");*/

    string r,bs,bs2;
    bs2=s2;
    bs=s1;
    bs.append(s2.length()*2,'0');
    //for(int i=0;i<buf.length();i++)buf[i]-='0';
    buf=0;
    int i,lenTest;
    bool isGreater;
    for(i=0;i<bs.length();i++)
    {
        //fpml
        if(isZero(bs))break;
        if(i>((s1.length()+s2.length())>fm*2?fm*2:(s1.length()+s2.length())))
        {
            //cout<<"!"<<endl;
            //rounded=true;
            break;
        }
        buf=0;
        isGreater=false;
        while(divStringGreaterThan(bs,bs2))
        {
            lenTest=bs.length()-bs2.length();
            if(lenTest<0)
            {
                bs.append(-lenTest,'0');
            }
            /*cout<<"now  bs="<<bs<<endl;
            cout<<"now bs2="<<bs2<<endl;
            cout<<"buf="<<buf<<endl;*/
            buf++;
            bs=sub(bs.substr(0,bs2.length()),bs2)+bs.substr(bs2.length(),bs.length()-bs2.length());
            /*cout<<"!now  bs="<<bs<<endl;
            cout<<"!now bs2="<<bs2<<endl;
            cout<<"!buf="<<buf<<endl;*/
            isGreater=true;
        }
        //cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
        if(isGreater)bs+="0";
        bs2.insert(0,1,'0');
        r+="0";
        r[r.length()-1]+=buf;
        //cout<<"!!!!!now r=!!!!!"<<r<<endl;
    }
    buf=s1.length();
    buf-=s2.length();
    buf+=fIdx;
    buf++;
    //cout<<"??"<<buf<<endl;
    if(buf<0)
    {
        for(i=buf;i<=0;i++)
        {
            r.insert(0,1,'0');
        }
        r.insert(1,1,'.');
    }
    else
    {
        if(r.length()<s1.length()-s2.length()+1+fIdx) r.append((s1.length()-s2.length()+1+fIdx)-r.length(),'0');
        else r.insert(s1.length()-s2.length()+1+fIdx,1,'.');
    }
    for(i=0;i<r.length();i++)
    {
        if(r[i]!='0')break;
    }
    if(i!=0)
    {
        r.erase(0,i);
    }
    if(r[0]=='.')r.insert(0,1,'0');
    //cout<<"i="<<i<<endl;
    //cout<<"fIdx="<<fIdx<<endl;
    /////////////////////////////////////////////////
    //if(expShorten) r=shortExp(r);
    ////////////////////////////////////////////
    //r=cut(r);
    return r;
}

string intToStr(int n)
{
    stringstream ss;
    ss<<n;
    return ss.str();
}

double stringToDouble(string s)
{
    double r;
    stringstream ss;
    ss<<s;
    ss>>r;
    return r;
}


string mod(string s1,string s2)
{
    string tmp=div(s1,s2);
    //cout<<"tmp="<<tmp<<endl;
    rounded=false;
    tmp=tmp.substr(0,tmp.find('.'));
    //cout<<"tmp!!="<<tmp<<endl;
    return cut(sub(s1,mul(tmp,s2)));
}

string cutForSqrt(string s)
{
    int counter=0;
    int dotPos=s.find('.');
    /*if(true)
    {
        s=s.substr(0,dotPos-1);
        dotPos=s.find('.');
    }*/
    if(dotPos!=string::npos)
    {
        /*if(s.length()-dotPos>100)
        {
            s=s.substr(0,dotPos+100);
            dotPos=s.find('.');
        }*/
        /*for(int i=s.length()-1;i>=dotPos;i--)
        {
            if(s[i]=='9')counter++;
            else
            {
                if(counter>=20)
                {
                    cout<<"*";
                    if(s[i]!='.')
                    {
                        s[i]++;
                        s=s.substr(0,i-1);
                        return s;
                    }
                    else return s.substr(0,i-2);
                }
                else counter=0;
            }
        }
        counter=0;*/
        for(int i=s.length()-1;i>dotPos;i--)
        {
            if(s[i]=='9')counter++;
            else counter=0;
            if(counter>=6)
            {
                //cout<<"~";
                s=s.substr(0,i);
                if(s[s.length()-1]=='.')
                {
                    s[s.length()-2]++;
                    s.erase(s.length()-1,1);
                }
                else
                {
                    s[s.length()-1]++;
                }
                for(int j=s.length();j>=0;j--)
                {
                    if(s[j]!='.')
                    {
                        if(s[j]>'9')
                        {
                            s[j]-=10;
                            if(s[j-1]!='.')s[j-1]++;
                            else s[j-2]++;
                        }
                    }
                }
            }
        }

        for(int i=s.length()-1;i>dotPos;i--)
        {
            if(s[i]=='0')counter++;
            else counter=0;
            if(counter>=6)
            {
                //cout<<"~";
                s=s.substr(0,i);
                if(s[s.length()-1]=='.')
                {
                    s.erase(s.length()-1,1);
                }
                return s;
            }
        }
    }
    /*bool stillNine=true;
    for(int i=s.length()-1;i>=0;i--)
    {
        if(s[i]!='9')
        {
            stillNine=false;
            break;
        }
    }
    if(stillNine)return s.substr(0,dotPos-1);*/
    return s;
}

void computing()
{
    int i;
    int stat=0;
    while(stillComputing)
    {
        i++;
        if(i>=5)
        {
            if(stat==3)
            {
                cout<<"\b\b\b";
                cout<<"   ";
                cout<<"\b\b\b";
                stat=0;
            }
            else
            {
                cout<<".";
                stat++;
            }
            i=0;
        }
        if(GetKeyState(VK_ESCAPE)&0x8000)
        {
            stillComputing=false;
            return;
        }
        Sleep(100);
    }
    return;
}

string getSqrtApproximation(string m)
{
    int dotPos=0;
    for(int i=0;i<m.length();i++)
    {
        if(m[i]=='.')
        {
            dotPos=i;
            break;
        }
    }
    if(dotPos==0)
    {
        return expo("10",intToStr(m.length()/2));
    }
    else
    {
        if(dotPos==1&&m[0]=='0')
        {
            for(int j=2;j<m.length();j++)
            {
                if(m[j]!='0')
                {
                    dotPos=j;
                    break;
                }
            }
            return expo("10","-"+intToStr((dotPos+1)/2));
        }
        else
        {
            return expo("10",intToStr(dotPos/2));
        }
    }

    /*string i="0";
    while(fullStringGreaterOrEqul(m,mul(i,i)))
    {
        i=add(i,mul("0.01",m));
    }
    string d=sub(m,mul(i,i));
    string p=div(d,mul("2",i));
    string a=add(i,p);
    return sub(a,div(mul(p,p),mul("2",a)));*/
}


void toClipboard(const std::string &s)
{
    HWND hwnd=GetActiveWindow();
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg=GlobalAlloc(GMEM_MOVEABLE,s.size()+1);
	if (!hg){
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg),s.c_str(),s.size()+1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT,hg);
	CloseClipboard();
	GlobalFree(hg);
}

string sqrtForExpo(string m)
{
    m=cut(m);
    //if(fullStringGreaterOrEqul("0",m)) return "0";
    if(m=="0") return "0";
    /*int temp=fpml;
    fpml=10;*/
    //string i="0";
    string x1,x2;
    /*while(fullStringGreaterOrEqul(m,mul(i,i)))
    {
        i=add(i,fullStringGreaterOrEqul(mul(m,"0.01"),"0.01")?mul(m,"0.01"):"0.01");
        //cout<<"!";
    }*/
    x1=getSqrtApproximation(m);
    if(sqrtDbg)cout<<"m="<<m<<endl;
    if(sqrtDbg)cout<<"x1="<<x1<<endl;
    //string temp="2";
    int k,h=0;
    /*for(k=2;true;k++)
    {
        temp=mul(temp,"2");
        if(fullStringGreaterOrEqul(temp,m))break;
    }*/
    for(int i=2;i<m.length();i++)
    {
        if(m[i]!='0')
        {
            h=i-1;
            break;
        }
    }
    h+=sqrt(fpml);
    h+=m.length();

    k=m.length()*3;
    k=sqrt(k);
    //k=m.length();//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //k/=2;
    //k+=10;
    if(k<10)k=10;
    if(sqrtDbg)cout<<"k="<<k<<endl;
    if(sqrtDbg)cout<<"h="<<h<<endl;
    //system("pause");
    int p=0;
    //x1="1000000000000000000000000000000000000000000000000000";
    string tmpCutStr;
    for(p=0;p<k;p++)
    {
        x2=m;
        x2=divForSqrt(x2,x1,h);
        x2=cut(x2);
        x2=add(x2,x1);
        x2=divForSqrt(x2,"2",h);
        x2=cut(x2);
        tmpCutStr=cutForSqrt(x2);
        tmpCutStr=cut(tmpCutStr);
        if(expo(tmpCutStr,"2")==m)
        {
            x2=tmpCutStr;
            break;
        }
        if(x1==x2)break;
        //if(sqrtDbg)cout<<"x1="<<x1<<endl;
        x1=x2;
        if(sqrtDbg)cout<<"x2="<<x2<<" p="<<p<<endl;

        //if(x2.length()>fpml)break;
    }
    return x2;
}

string nthRoot(string s,string e)
{
    string r="1",temp="0";
    string r1,r2,r4,r5,r6;
    int len=(s.length()+sqrt(fpml));
    int exp=getExp(s);
    int finalLen;
    //cout<<exp<<endl;
    exp/=stringToDouble(e);
    //exp+=1;
    //cout<<exp<<'!'<<endl;
    if(exp>0)
    {
        r.append(exp,'0');
    }
    else if(exp<0)
    {
        exp*=-1;
        r.insert(0,exp,'0');
        r.insert(1,1,'.');
    }
    for(int i=2;i<=9;i++)
    {
        temp=mul(intToStr(i),r);
        if(fullStringGreaterOrEqul(expo(temp,e),s))
        {
            r=temp;
            break;
        }
    }
    finalLen=r.length()+sqrt(fpml);
    if(sqrtDbg)
    {
        cout<<"r len"<<r.length()<<endl;
        cout<<"r="<<r<<endl;
    }

    //r1=r;
    for(int i=0;i<fpml*sqrt(stringToDouble(e));i++)
    {
        //r=div(add(mul(sub(e,"1"),r),div(s,expo(r,sub(e,"1")))),e);
        //cout<<"a";
        r1=sub(e,"1");
        r1=cut(r1);
        if(sqrtDbg) cout<<"r1="<<r1<<endl;
        //cout<<"b";
        r2=mul(r1,r);
        if(sqrtDbg) cout<<"r2="<<r2<<endl;
        //cout<<"c";
        //r3=r1;
        //cout<<"d";
        if(sqrtDbg) cout<<"!!!!!"<<r<<":::"<<r1<<endl;
        r4=expo(r,r1);
        if(sqrtDbg) cout<<"r4="<<r4<<endl;
        //r4=r4.substr(0,len<r4.length()?len:r4.length());
        //r4=r4.substr(0,finalLen<r4.length()?finalLen:r4.length());
        //cout<<"e";
        r5=div(s,r4);
        //r5=divForSqrt(s,r4,s.length()>sqrt(fpml)?s.length():sqrt(fpml));
        //cout<<"f";
        r6=add(r2,r5);
        //cout<<"g";
        r=div(r6,e);
        //cout<<"h"<<endl;
        //r=r7;
        if(sqrtDbg) cout<<"r="<<r<<" : ";
        r=r.substr(0,len<r.length()?len:r.length());
        r=cut(r);
        if(sqrtDbg) cout<<r<<endl;
        if(temp==r) break;
        temp=r;
    }
    if(enableComputingBreaker)
    {
        //hasApp=true;
        cout<<"Approximation: "<<r<<endl;
        cout<<"Computing(press esc to break)";
    }
    thread t;
    if(enableComputingBreaker)
    {
        stillComputing=true;
        t=thread(computing);
    }
    if(cut(expo(r,e))!=s)
    {
        int k=s.length()*3;
        k=sqrt(k);
        if(k<10)k=10;
        for(int i=0;i<k;i++)
        {
            r=div(add(mul(sub(e,"1"),r),div(s,expo(r,sub(e,"1")))),e);
            r=cut(r);
            //cout<<r<<endl;
            if(r.length()>s.length()+fpml*sqrt(s.length()))
            {
                if(enableComputingBreaker&&!stillComputing)
                {
                    t.join();
                }
                break;
            }
        }
    }
    else rounded=false;
    //cout<<"!"<<endl;
    r=cut(r);
    if(enableComputingBreaker)
    {
        if(stillComputing)
        {
            stillComputing=false;
            t.join();
        }
        else stillComputing=false;
        cout<<endl;
    }
    return r;
    //return r.substr(0,len<r.length()?len:r.length());
}

int getExp(string s)
{
    //if(s[0]=='-') return 0;
    int pos=s.find('.');
    if(pos==string::npos)
    {
        if(isZero(s)) return 0;
        //cout<<"%1";
        return s.length()-1;
    }
    else
    {
        if(pos==1&&s[0]!='0')
        {
            return 0;
        }
        for(int i=pos+1;i<s.length();i++)
        {
            //cout<<"%2";
            if(s[i]!='0') return pos-i;
        }
        return 0;
    }
}

string expoForRoot(string s,string n,int len)
{
    vector<string> v;
    int e=0;
    for(int i=0;i<n.length();i++)
    {
        e*=10;
        e+=n[i]-'0';
    }

    string r;
    //cout<<"s="<<s<<endl;
    //cout<<"e="<<e<<endl;
    int temp;
    int maxE;
    //if(e==0) r="1";
    maxE=1;

    r=s;
    v.push_back(s);
    for(int i=2;i<=e;)
    {
        if(i<=e)
        {
            r=mul(r,r);
            v.push_back(r);
            maxE*=2;
        }
        i*=i;
        //r=mul(s,r);
    }
    temp=maxE;
    r="1";
    for(int i=v.size()-1;i>=0;i--)
    {
        temp=maxE;
        if(temp<=e)
        {
            /*cout<<"t:"<<temp<<" e:"<<e<<endl;
            for(int k=0;k<v.size();k++)
            {
                cout<<v[k]<<" ";
            }
            cout<<endl;*/
            e-=temp;
            r=mul(r,v[i]);
            i++;
            //cout<<"r="<<r<<" i="<<i<<endl;
            continue;
        }
        else maxE/=2;
    }
    return r;
}
