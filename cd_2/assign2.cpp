#include<iostream>
#include<cstdlib>
#include<string.h>
#include<fstream>
#include<vector>

using namespace std;
vector<char> mstack;
int mapping(char ch);
char comp(char x,char y);
void display(vector<char> v){    for(int i=0;i< v.size();++i){cout<<v[i];}    cout<<endl;   }
void reduce(int cur)
{
    if(mstack[cur]== '*' || mstack[cur] == '+' || mstack[cur] == '-')
    {
        if(mstack[cur] == '*') {cout<<mstack[cur-1] <<"*"<< mstack[cur+1]<<"    : "<<endl;   mstack[cur-1] = mstack[cur-1] * mstack[cur+1]; }
        else if(mstack[cur] == '+') mstack[cur-1] = mstack[cur-1] + mstack[cur+1];
        else if(mstack[cur] == '-') mstack[cur-1] = mstack[cur-1] - mstack[cur+1];
    }

}
string skipwhite(string );
char precedence[3][3];

int main(){
//input from file
        string input;
        char ch;
        int i=0,j=0;


		ifstream file("ip",ios::in);
        if (file.good())
        {
            string str;
            while(getline(file, str))
            {   int ctr=0;
                while(ctr<5)
                {
                    ch =str[ctr];
                    if(ch=='-'||ch=='+'||ch=='*'){
                           if(ctr==0){ i=mapping(ch);}
                           if(ctr==2){ j=mapping(ch);}
                       }
                    if(ctr==4){ precedence[i][j] = ch; }
                    ++ctr;
                }
            }
        }
        file.close();

        string exp;
        cout<<"Enter your Expression : ";
        cin>>std::ws;
        getline(cin,exp);  /**implement skipwhite*/

        string exp2=exp;
        ///Parsing
        int cur=0,ipcur=0;
        vector<int> prevcur;
        prevcur.push_back(0);
        mstack.push_back('$');
        display(mstack);
        while(1){
                if(comp(mstack[cur],exp2[ipcur]) == '<') {
                                                          mstack.push_back(exp2[ipcur]);
                                                           ++ipcur;
                                                           prevcur.push_back(cur);
                                                           cur=mstack.back();
                                                           display(mstack);}  ///shifting

                else if(comp(mstack[cur],exp2[ipcur]) == '>') {
                                                            reduce(cur);
                                                          cur = prevcur.back();
                                                          prevcur.pop_back();
                                                        display(mstack);
                                                        }  ///Reduce
                else if(comp(mstack[cur],exp2[ipcur]) == '!')
                {
                    cout<<"Incorrect input"<<endl;

                }

                else if(comp(mstack[cur],exp2[ipcur]) == '#')
                    display(mstack);
                    exit(0);
                }


display(mstack);

return 0;
}

int mapping(char ch){
    if(ch=='-'){return 0;}
    else if(ch=='+'){return 1;}
    return 2;
}

char comp(char x,char y){
    if(x == '$' && y == '$') {return '#';}  //end of parsing
    else if(x == '$') return '<';
    else if( y == '$' ) return '>';

    else if((x=='-'||x=='+'||x=='*')&&(y=='-'||y=='+'||y=='*')){
       return precedence[mapping(x)][mapping(y)];
    }
    else if(x=='-'||x=='+'||x=='*') return '<';
    else if(y=='-'||y=='+'||y=='*') return '>';
    else return '!'; ///Error case where both are identifiers

}


   //cout<<precedence[0][0]<<"   "<<endl;
/*
    for(int p=0;p<3;++p){
         for(int q=0;q<3;++q){
            cout<<precedence[p][q]<<"   ";
    }
    cout<<endl;
    }
*/
