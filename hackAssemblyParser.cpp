//#include <iostream>
//#include <fstream>
//#include <cstdlib>
//#include <cstring>
//#include <iomanip>
//#include <sstream>
#include <bits/stdc++.h>
using namespace std;
class HackAssembler{
    int instructionCount, varCount;
    ifstream read;
    ofstream writef;
    char bcode[16];
    void lineProcessor(istringstream &iss);
    void symbolTableInitializer(istringstream &iss1);
    string Ainstruct(string ins);
    string Cinstruct(const string &ins);
    string dest(const string& );
    string comp(const string&);
    string jump(const string&);
    string iToBinary(int a);
    map<string,int> st;
public:
    HackAssembler(char *);
    void parser();
    int getInstructionCount(){return instructionCount;};
};
int main(int argc, char *argv[])
{
    if(argc < 1){
        cout<<"Need to give a filename"<<endl;
        exit(1);
    }
    HackAssembler fobj(argv[1]);
    fobj.parser();
    cout<<fobj.getInstructionCount();
    return 0;
}
HackAssembler :: HackAssembler(char * filename)
:instructionCount(-1),varCount(16)    //what if we get the LABEL at the start of the program just add the next instruction to it
{
    read.open(filename);
    if(!read){
        cout<<"Error opening "<<filename<<endl;
        exit(1);
    }
    int i;
char anotherfile[strlen(filename)+2];
    for(i =0; filename[i]!='.' ; i++)
    {
        anotherfile[i]=filename[i];
    }
    anotherfile[i]='.';
    anotherfile[i+1]='h';
    anotherfile[i+2]='a';
    anotherfile[i+3]='c';
    anotherfile[i+4]='k';
    anotherfile[i+5]='\0';

    writef.open(anotherfile);
    if(!writef){
        cout<<"Error opening "<<anotherfile<<endl;
        exit(1);
    }
    /**Initializing symbol table*/
    st["SP"] = 0;
    st["LCL"] = 1;
    st["ARG"] = 2;
    st["THIS"] = 3;
    st["THAT"] = 4;
    st["R0"] = 0;
    st["R1"] = 1;
    st["R2"] = 2;
    st["R3"] = 3;
    st["R4"] = 4;
    st["R5"] = 5;
    st["R6"] = 6;
    st["R7"] = 7;
    st["R8"] = 8;
    st["R9"] = 9;
    st["R10"] = 10;
    st["R11"] = 11;
    st["R12"] = 12;
    st["R13"] = 13;
    st["R14"] = 14;
    st["R15"] = 15;
    st["SCREEN"] = 16384;
    st["KBD"] = 24576;
}
void HackAssembler::parser(){
      string line;
      //char linearr[100];iss>>ws;
      while(getline( read , line))
      {
          //cout<<"I have more lines"<<endl;
          if(line.empty()){
            continue;
          }
          istringstream iss1(line);
          symbolTableInitializer(iss1);//method to process the line;
      }
/**Error here in reading the file again from start the seekg function is not working*/

        //cout<<read.tellg()<<endl;
        read.clear();//it clears all the error flags like eof
        read.seekg(0, ios::beg);
        cout<<read.tellg()<<endl;
      while(getline( read , line))
      {
          //cout<<"I have more lines"<<endl;
          if(line.empty()){
            continue;
          }
          istringstream iss(line);
          lineProcessor(iss);//method to process the line;
      }
}
void HackAssembler::symbolTableInitializer(istringstream & iss){
    char c;
    string info;
    string result;
    iss>>c;
    if(c == '/'){
            //cout<<"got a comment in first pass"<<endl;
        return;
    }
    if(c == '@'){
            //cout<<"got the A instruction in first pass"<<endl;
        instructionCount++;
    }
    else if(c=='('){
        //we got a LABEL convert it according to instructionCount
        while(true){
            //cout<<"adding char into A-info\n"<<endl;
            iss>>c;
            if(c==')'||iss.eof())
                break;
            info.push_back(c);
        }
        if(st.find(info)==st.end()){
            st.insert(pair<string, int>(info, instructionCount+1));
            map <string, int>::iterator p = st.find(info);
            if(p != st.end())
                cout<<info<<"added = "<<st[info]<<endl;
        }
    }
    else{
        //cout<<"got a C instruction in first pass"<<endl;
        instructionCount++;
    }
}
void HackAssembler::lineProcessor(istringstream& iss){
    char c;
    string info;
    string result;
    iss>>c;
    if(c == '/'){
            cout<<"got a comment in second pass"<<endl;
        return;
    }
    else if(c == '@'){
            cout<<"got a Ainstruction in second pass"<<endl;
        while(true){
            //cout<<"adding char into A-info\n"<<endl;
            iss>>c;
            if(c=='\\'||iss.eof())
                break;
            info.push_back(c);
        }
        result = Ainstruct(info);
        //instructionCount++;
    }
    else if(c == '('){
                cout<<"Got a LABEL in second pass"<<endl;
                return;
            }
    else{
        cout<<"got a Cinstruction in second pass"<<endl;
        iss.seekg(0,ios::beg);
        while(true){
            iss>>c;
            //cout<<"adding char into C-info\n";
            if(c=='\\'||iss.eof())
                break;
            info.push_back(c);
        }
        //cout<<"scanned C instruction "<<info<<endl;
        result=Cinstruct(info);
        //instructionCount++;
    }
    writef<<result<<endl;
}
string HackAssembler ::Ainstruct(string info){
        /*const char* a= info.c_str();
        int t;
        try{
            t = atoi(a);
        }
        catch(...){
            //it's not convertible*/
            /**write map function here search in map and do the calculations*/
          int t;
            if(st.find(info) == st.end()){
                st.insert(pair<string, int>(info, varCount));
                map <string, int>::iterator p = st.find(info);
            //if(p != st.end())
                //cout<<info<<"added = "<<st[info]<<endl;

                t= varCount;
                varCount++;
            }
            else{
                t = st[info];
            }

        return iToBinary(t);
}

string HackAssembler ::iToBinary(int a){

char *bin= new char[17];
    int i;
    for(i =0; i<16 ; i++){
        if(a&1<<i)
            bin[15-i]= '1';
        else
            bin[15-i]= '0';
    }
    bin[i]='\0';
    string result = bin;
    delete[] bin;
    return result;
}
string HackAssembler ::Cinstruct(const string &info){
    string part;
    string c;
    string d;
    string j;
    int i=0;
    while(true){
        if(info[i]=='='|| info[i]==';'||info[i]=='\0'){
            break;
        }
        part+=info[i];
        i++;

    }
    if(info[i]=='='){
        i++;
        //cout<<"dest part"<<part<<endl;
        d=dest(part);
        part.erase();
        while(true){
            if(info[i]==';'||info[i]=='\0'){
                break;
            }
            part.push_back(info[i]);
            i++;
        }
            if(info[i]==';'){
                i++;
                //cout<<"comp part "<<part<<endl;
                c = comp(part);
                part.erase();
                while(info[i]!='\0'){
                    part.push_back(info[i]);
                    i++;
                }
                //cout<<"jump part "<<part<<endl;
                j = jump(part);

        }
            else if(info[i]=='\0'){
                //cout<<"comp part "<<part<<endl;
                c= comp(part);
                //no jump;
                j = "000";
            }

    }
    else if(info[i]==';'){
                i++;
                //cout<<"comp part"<<part<<endl;
                c = comp(part);
                part.erase();
                while(info[i]!='\0'){
                    part.push_back(info[i]);
                    i++;
                }
                j = jump(part);
                //since no destination
                d = "000";
    }
    else if(info[i]=='\0'){
        //here neither destination nor jump
       // cout<<"comp part"<<part<<endl;
        c= comp(part);
        d="000";
        j="000";

    }
    else{
        cout<<"something wrong with reading Cinstruction\n";
        exit(-1);
    }
    cout<<"c= "<<c<<" d= "<<d<<" j= "<<j<<endl;
return "111"+c+d+j;
}
string HackAssembler::dest(const string & part){
    string result;
    if(part=="M"){
        result ="001";
    }
    else if(part=="D"){
        result.push_back('0');
        result.push_back('1');
        result.push_back('0');
    }
    else if(part=="MD"){
        result.push_back('0');
        result.push_back('1');
        result.push_back('1');
    }
    else if(part=="A"){
        result.push_back('1');
        result.push_back('0');
        result.push_back('0');
    }
    else if(part=="AM"){
        result.push_back('1');
        result.push_back('0');
        result.push_back('1');
    }
    else if(part=="AD"){
        result.push_back('1');
        result.push_back('1');
        result.push_back('0');
    }
    else if(part=="AMD"){
        result.push_back('1');
        result.push_back('1');
        result.push_back('1');
    }
    cout<<"dest part done"<<part<<" = "<<result<<endl;
return result;
}
string HackAssembler::jump(const string & part){
    string result;
    if(part == "JGT"){
        result.push_back('0');
        result.push_back('0');
        result.push_back('1');
    }
    else if(part=="JEQ"){
        result.push_back('0');
        result.push_back('1');
        result.push_back('0');
    }
    else if(part=="JGE"){
        result.push_back('0');
        result.push_back('1');
        result.push_back('1');
    }
    else if(part=="JLT"){
        result.push_back('1');
        result.push_back('0');
        result.push_back('0');
    }
    else if(part=="JNE"){
        result.push_back('1');
        result.push_back('0');
        result.push_back('1');
    }
    else if(part=="JLE"){
        result.push_back('1');
        result.push_back('1');
        result.push_back('0');
    }
    else if(part=="JMP"){
        result.push_back('1');
        result.push_back('1');
        result.push_back('1');
    }
    cout<<"jump part done"<<part<<" = "<<result<<endl;
return result;

}
string HackAssembler::comp(const string & part){
    string result;
    //all instruction with a=0 refer nand2tetris
    if(part=="0"){
        result ="0101010";
    }
    else if(part=="1"){
        result ="0111111";
    }
    else if(part == "-1"){
        result = "111010";
    }
    else if(part =="D"){
        result = "0001100";
    }
    else if(part =="A"){
        result ="0110000";
    }
    else if(part =="M"){
        result ="1110000";
    }
    else if(part =="!D"){
        result ="0001101";
    }
    else if(part =="!A"){
        result ="0110001";
    }
    else if(part =="!M"){
        result ="1110001";
    }
    else if(part =="-D"){
        result ="0001111";
    }
    else if(part =="-A"){
        result ="0110011";
    }
    else if(part =="-M"){
        result = "1110011";
    }
    else if(part =="D+1"){
        result = "0011111";
    }
    else if(part =="A+1"){
        result = "0110111";
    }
    else if(part =="M+1"){
        result = "1110111";
    }
    else if(part =="D-1"){
        result = "0001110";
    }
    else if(part =="A-1"){
        result = "0110010";
    }
    else if(part =="M-1"){
        result = "1110010";
    }
    else if(part =="D+A"){
        result = "0000010";
    }
    else if(part =="D+M"){
        result = "1000010";
    }
    else if(part =="D-A"){
        result = "0010011";
    }
    else if(part =="D-M"){
        result = "1010011";
    }
    else if(part =="A-D"){
        result = "0000111";
    }
    else if(part =="M-D"){
        result = "1000111";
    }
    else if(part =="D&A"){
        result = "0000000";
    }
    else if(part =="D&M"){
        result = "1000000";
    }
    else if(part =="D|A"){
        result = "0010101";
    }
    else if(part =="D|M"){
        result = "1010101";
    }
    return result;
}

