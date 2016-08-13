/* SIMPLE INSTRUCTIONAL COMPILER (SIC) [Complete Version]
   Source Language: A Small C-like Language
   Object Language: A Suitably Defined Intermediate Language
   Implementation Language: C++
*/


// Header Files
#include<fstream>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#include<ctype.h>


// Structure Definitions
// A Token
struct token
{int type;
 int val;
};

// A Symbol Table Entry
struct st_entry
{char name[9];
 int v_f; //0:variable name, 1:function name
 int address; //For function names only, denotes starting address of the function in object program
};

// Symbol Table
struct symbol_table
{st_entry list[100];
};

// Temporary Stack
struct temp_stack
{int A[100];
 int top;
};


// Global Variables
char FILENAME[13]; //Name of source file
ifstream SOURCE_FILE; //Source file input stream
ifstream HEADER_FILE; //Header file input stream
ofstream ASM_FILE; //Assembly file output stream
ofstream BIN_FILE; //Binary file output stream
int LINE_NO; //Nine number in source program
int NEXT_ADDR; //Next address in object program for which code is to be generated
int FUNCTION_ADDR; //Address of function to be called
symbol_table ST1; //Symbol table
int ST_SIZE; //Symbol table size
int ST_MODE; //0:insert, 1:inquire
int NO_ID; //Number of identifiers currently active in symbol table
int NO_FUNC; // Number of functions defined
int P_COUNT; //Number of parameters in function call
temp_stack TS; //Stack to reverse parameters before function call
token SAVED1,SAVED2; //Two backup tokens
int ERROR_CATEGORY; //Specification of error


// Function Prototypes
// Phases and Modules
token lexical_analyzer(int);
void syntax_analyzer();
void intermediate_code_generator(char,char=0);
void backpatch(int,int);
int st_insert_inquire(char[],int);
void error_handler(int);
// Service Routines
void about(); //About the SIC Compiler
void pushback(ifstream*,int=1); //Push n characters back into source or header file (default:n=1)
token next(int); //Fetch next token in source program
void save(token); //Save token for future use
void ts_init(); //Initialize temporary stack
int ts_empty(); //Check if temporary stack empty
void ts_push(int); //Push element on temporary stack
int ts_pop(); //Pop element from temporary stack
char convert_to_asm(char,char); //Convert two integers into 1 byte of asm instruction
void itoa(char[],unsigned char); //Convert integer into a string of hexadecimal digits


// The Main Program
void main(int argc,char *args[])
{int i;
 int len; //Length of name of source file
 char asm_filename[13],bin_filename[13]; //Names of asm and binary files

 // Checking Number of Arguments to main()
 if(argc==1)
	{about();
	 return;
	}
 if(argc>2)
	{error_handler(100);
	 return;
	}

 // Checking Name of Source File
 strcpy(FILENAME,args[1]);
 len=strlen(FILENAME);
 for(i=0;i<len-2;i++)
	if((!isalnum(FILENAME[i]))&&(FILENAME[i]!='_')&&(FILENAME[i]!='.'))
		{error_handler(101);
		 return;
		}
 if((FILENAME[len-2]!='.')||((FILENAME[len-1]!='C')&&(FILENAME[len-1]!='c')))
	{error_handler(102);
	 return;
	}

 // Creating Names for asm and Binary Files
 strcpy(asm_filename,FILENAME);
 len=strlen(asm_filename)+2;
 asm_filename[len-3]='A';
 asm_filename[len-2]='S';
 asm_filename[len-1]='M';
 asm_filename[len]='\0';
 strcpy(bin_filename,FILENAME);
 len=strlen(bin_filename)+2;
 bin_filename[len-3]='B';
 bin_filename[len-2]='I';
 bin_filename[len-1]='N';
 bin_filename[len]='\0';

 // Opening Files
 SOURCE_FILE.open(FILENAME,ios::in);
 if(!SOURCE_FILE)
	{error_handler(103);
	 return;
	}
 ASM_FILE.open(asm_filename,ios::out);
 BIN_FILE.open(bin_filename,ios::out);

 // Initializations
 LINE_NO=1;
 NEXT_ADDR=0;
 ST_SIZE=0;
 ST_MODE=1;
 NO_FUNC=0;
 ts_init();
 SAVED1.type=SAVED2.type=-1;

 // Invoking Syntax Analyzer which will Invoke Other Phases and Modules
 syntax_analyzer();

 // Closing Files
 SOURCE_FILE.close();
 ASM_FILE.close();
 BIN_FILE.close();

 getch(); //Code to help debugging
}


// Lexical Analyzer Phase
token lexical_analyzer(int mode)
/* mode: [1] variable name (normal) mode,
	 [2] function name mode,
	 [3] asm mode,
     and [4] error handling mode.
*/
{char ch; //A character in the source program
 char ch1; //Another character in the source program
 int i;
 static int header_mode=1; //0:no more header file expected, 1:header file expected, 2:header file being processed
 char directive[8]; //Name of preprocessor directive (e.g. include)
 char header_file_name[13]; //Name of header file currently being processed
 int comment_mode; //What is being processed? 0:executable code, 1:single line comment, 2:multiple line comment
 int flag; //Has a token been detected? 0:no, 1:yes
 int state; //State in NFA
 static int k_i=0; //Which branch of NFA is being traversed? 0:keyword, 1:identifier
 char identifier_name[9]; //Name of identifier detected
 int identifier_name_length; //Length of name of identifier detected
 long numeric_literal; //Value of numeric literal detected
 token t; //Token to be returned
 ifstream *infile; //Current input file stream (source file or any of the header files)

 //Choose Input File
 if(header_mode==2)
	infile=&HEADER_FILE;
 else
	infile=&SOURCE_FILE;

 // Error Handling Mode
 if(mode==4)
	{// Handling Lexical Errors
	 if(ERROR_CATEGORY==3)
		{do{infile->get(ch);
		    if(toascii(ch)==127)
			if(header_mode==2)
				{header_mode=1;
				 infile->close();
				 infile=&SOURCE_FILE;
				}
			else
				error_handler(304);
		   }while(isalnum(ch));
		}
	 // Handling Parsing Errors
	 if(ERROR_CATEGORY==4)
		{do{infile->get(ch);
		    if(toascii(ch)==127)
			if(header_mode==2)
				{header_mode=1;
				 infile->close();
				 infile=&SOURCE_FILE;
				}
			else
				error_handler(304);
		   }while((ch!=';')&&(ch!='{')&&(ch!='}'));
		}
	 t.type=100;
	 return t;
	}

 // Other Modes
 if(mode==3)
	state=91; //asm mode
 else
	state=0; //variable name (normal) mode or function name mode
 comment_mode=0;
 t.type=0;
 t.val=0;
 flag=0;

 // Loop to Read a Token Character-by-Character
 while(!flag)
	{// Reading a Character
	 if((state!=4)&&(state!=9)&&(state!=14)&&(state!=23)&&(state!=26)
	   &&(state!=28)&&(state!=30)&&(state!=35)&&(state!=39)&&(state!=46)
	   &&(state!=52)&&(state!=54)&&(state!=56)&&(state!=58)&&(state!=60)
	   &&(state!=61)&&(state!=63)&&(state!=64)&&(state!=65)&&(state!=66)
	   &&(state!=67)&&(state!=68)&&(state!=70)&&(state!=71)&&(state!=73)
	   &&(state!=75)&&(state!=77)&&(state!=78)&&(state!=80)&&(state!=81)
	   &&(state!=83)&&(state!=84)&&(state!=85)&&(state!=86)&&(state!=87)
	   &&(state!=88)&&(state!=89)&&(state!=90))
		infile->get(ch);
	   else
		ch='.';

	 // Handling Preprocessor Directives
	 if((header_mode==1)&&(ch=='#'))
		{do{infile->get(ch1);
		   }while((ch1==' ')||(ch1=='\t'));
		 directive[0]=ch1;
		 i=1;
		 while(i<7)
			{infile->get(ch1);
			 directive[i]=ch1;
			 i++;
			}
		 directive[i]='\0';
		 if((i!=7)||(strcmp(directive,"include")))
			{error_handler(201);
			}
		 do{infile->get(ch1);
		   }while((ch1==' ')||(ch1=='\t'));
		 if(ch1!='<')
			{error_handler(201);
			}
		 do{infile->get(ch1);
		   }while((ch1==' ')||(ch1=='\t'));
		 header_file_name[0]=ch1;
		 i=1;
		 while(1)
			{infile->get(ch1);
			 if(ch1=='\n')
				{LINE_NO++;
				 error_handler(201);
				 break;
				}
			 if((ch1==' ')||(ch1=='\t'))
				break;
			 if(ch1=='>')
				{pushback(infile);
				 break;
				}
			 header_file_name[i]=ch1;
			 i++;
			}
		 header_file_name[i]='\0';
		 do{infile->get(ch1);
		   }while((ch1==' ')||(ch1=='\t'));
		 if(ch1!='>')
			{error_handler(201);
			}
		 if(i>12)
			{error_handler(202);
			}
		 if((header_file_name[i-2]!='.')||(header_file_name[i-1]!='h'))
			{error_handler(203);
			}
		 do{infile->get(ch1);
		   }while(ch1!='\n');
		 LINE_NO++;
		 HEADER_FILE.open(header_file_name,ios::in);
		 if(HEADER_FILE)
			{header_mode=2;
			 flag=1;
			 break;
			}
		 else
			{error_handler(204);
			}
		}

	 // Handling End of Files
	 if(toascii(ch)==127)
		{if(header_mode==0)
			{if(comment_mode==2)
				{error_handler(200);
				}
			 flag=1;
			 break;
			}
		 else
			if(header_mode==2)
				{header_mode=1;
				 infile->close();
				 infile=&SOURCE_FILE;
				}
		 ch=' ';
		}

	 // Handling Whitespaces
	 if(ch=='\t')
		ch=' ';
	 if(ch=='\n')
		{ch=' ';
		 if(header_mode!=2)
			LINE_NO++;
		 if(comment_mode==1)
			comment_mode=0;
		}

	 // Handling Comments
	 if((comment_mode==0)&&(ch=='/'))
		{infile->get(ch1);
		 if(ch1=='/')
			{comment_mode=1;
			 continue;
			}
		 else
			if(ch1=='*')
				{comment_mode=2;
				 continue;
				}
			else
				pushback(infile);
		}
	 if((comment_mode==2)&&(ch=='*'))
		{infile->get(ch1);
		 if(ch1=='/')
			{comment_mode=0;
			 continue;
			}
		}
	 if((comment_mode==1)||(comment_mode==2))
		continue;

	 // Actual Lexical Analysis
	 switch(state)
		{case  0:if(ch==' ') //Space
				break;
			 if((k_i)&&((isalpha(ch))||(ch=='_'))) //Try to detect identifier
				{identifier_name[0]=ch;
				 identifier_name_length=1;
				 if(mode==1)
					state=53;
				 else
					state=55;
				 break;
				}
			 if(isdigit(ch)) //Try to detect numeric literal
				{state=57;
				 numeric_literal=ch-'0';
				 break;
				}
			 switch(ch) //Try to detect keyword
				{case 'a':state=1;
					  break;
				 case 'c':state=5;
					  break;
				 case 'e':state=10;
					  break;
				 case 'f':state=15;
					  break;
				 case 'i':state=24;
					  break;
				 case 'm':state=31;
					  break;
				 case 'o':state=36;
					  break;
				 case 'r':state=40;
					  break;
				 case 'w':state=47;
					  break;
				 case '+':state=59;
					  break;
				 case '-':state=62;
					  break;
				 case '^':state=65;
					  break;
				 case '*':state=66;
					  break;
				 case '/':state=67;
					  break;
				 case '%':state=68;
					  break;
				 case '!':state=69;
					  break;
				 case '&':state=72;
					  break;
				 case '|':state=74;
					  break;
				 case '=':state=76;
					  break;
				 case '<':state=79;
					  break;
				 case '>':state=82;
					  break;
				 case '(':state=85;
					  break;
				 case ')':state=86;
					  break;
				 case '{':state=87;
					  break;
				 case '}':state=88;
					  break;
				 case ',':state=89;
					  break;
				 case ';':state=90;
					  break;
				 default :if((isalpha(ch))||(ch=='_')) //No keyword detected, try to detect identifier in next iteration
						{k_i=1;
						 pushback(infile);
						 break;
						}
					  error_handler(300);
					  flag=1;
					  break;
				}
			 break;
		 case  1:if(ch=='s')
				state=2;
			 else
				{k_i=1;
				 pushback(infile,2);
				 state=0;
				}
			 break;
		 case  2:if(ch=='m')
				state=3;
			 else
				{k_i=1;
				 pushback(infile,3);
				 state=0;
				}
			 break;
		 case  3:if((!isalnum(ch))&&(ch!='_'))
				{state=4;
				 pushback(infile);
				}
			 else
				{k_i=1;
				 pushback(infile,4);
				 state=0;
				}
			 break;
		 case  4:t.type=1;
			 flag=1;
			 break;
		 case  5:if(ch=='a')
				state=6;
			 else
				{k_i=1;
				 pushback(infile,2);
				 state=0;
				}
			 break;
		 case  6:if(ch=='l')
				state=7;
			 else
				{k_i=1;
				 pushback(infile,3);
				 state=0;
				}
			 break;
		 case  7:if(ch=='l')
				state=8;
			 else
				{k_i=1;
				 pushback(infile,4);
				 state=0;
				}
			 break;
		 case  8:if((!isalnum(ch))&&(ch!='_'))
				{pushback(infile);
				 state=9;
				}
			 else
				{k_i=1;
				 pushback(infile,5);
				 state=0;
				}
			 break;
		 case  9:t.type=2;
			 flag=1;
			 break;
		 case 10:if(ch=='l')
				state=11;
			 else
				{k_i=1;
				 pushback(infile,2);
				 state=0;
				}
			 break;
		 case 11:if(ch=='s')
				state=12;
			 else
				{k_i=1;
				 pushback(infile,3);
				 state=0;
				}
			 break;
		 case 12:if(ch=='e')
				state=13;
			 else
				{k_i=1;
				 pushback(infile,4);
				 state=0;
				}
			 break;
		 case 13:if((!isalnum(ch))&&(ch!='_'))
				{pushback(infile);
				 state=14;
				}
			 else
				{k_i=1;
				 pushback(infile,5);
				 state=0;
				}
			 break;
		 case 14:t.type=3;
			 flag=1;
			 break;
		 case 15:if(ch=='u')
				state=16;
			 else
				{k_i=1;
				 pushback(infile,2);
				 state=0;
				}
			 break;
		 case 16:if(ch=='n')
				state=17;
			 else
				{k_i=1;
				 pushback(infile,3);
				 state=0;
				}
			 break;
		 case 17:if(ch=='c')
				state=18;
			 else
				{k_i=1;
				 pushback(infile,4);
				 state=0;
				}
			 break;
		 case 18:if(ch=='t')
				state=19;
			 else
				{k_i=1;
				 pushback(infile,5);
				 state=0;
				}
			 break;
		 case 19:if(ch=='i')
				state=20;
			 else
				{k_i=1;
				 pushback(infile,6);
				 state=0;
				}
			 break;
		 case 20:if(ch=='o')
				state=21;
			 else
				{k_i=1;
				 pushback(infile,7);
				 state=0;
				}
			 break;
		 case 21:if(ch=='n')
				state=22;
			 else
				{k_i=1;
				 pushback(infile,8);
				 state=0;
				}
			 break;
		 case 22:if((!isalnum(ch))&&(ch!='_'))
				{pushback(infile);
				 state=23;
				}
			 else
				{k_i=1;
				 pushback(infile,9);
				 state=0;
				}
			 break;
		 case 23:t.type=4;
			 flag=1;
			 break;
		 case 24:if(ch=='f')
				state=25;
			 else
				if(ch=='n')
					state=27;
				else
					{k_i=1;
					 pushback(infile,2);
					 state=0;
					}
			 break;
		 case 25:if((!isalnum(ch))&&(ch!='_'))
				{pushback(infile);
				 state=26;
				}
			 else
				{k_i=1;
				 pushback(infile,3);
				 state=0;
				}
			 break;
		 case 26:t.type=5;
			 flag=1;
			 break;
		 case 27:if(ch=='t')
				state=29;
			 else
				if((!isalnum(ch))&&(ch!='_'))
					{pushback(infile);
					 state=28;
					}
				 else
					{k_i=1;
					 pushback(infile,3);
					 state=0;
					}
			 break;
		 case 28:t.type=6;
			 flag=1;
			 break;
		 case 29:if((!isalnum(ch))&&(ch!='_'))
				{pushback(infile);
				 state=30;
				}
			 else
				{k_i=1;
				 pushback(infile,4);
				 state=0;
				}
			 break;
		 case 30:t.type=7;
			 flag=1;
			 break;
		 case 31:if(ch=='a')
				state=32;
			 else
				{k_i=1;
				 pushback(infile,2);
				 state=0;
				}
			 break;
		 case 32:if(ch=='i')
				state=33;
			 else
				{k_i=1;
				 pushback(infile,3);
				 state=0;
				}
			 break;
		 case 33:if(ch=='n')
				state=34;
			 else
				{k_i=1;
				 pushback(infile,4);
				 state=0;
				}
			 break;
		 case 34:if((!isalnum(ch))&&(ch!='_'))
				{pushback(infile);
				 state=35;
				}
			 else
				{k_i=1;
				 pushback(infile,5);
				 state=0;
				}
			 break;
		 case 35:t.type=8;
			 flag=1;
			 break;
		 case 36:if(ch=='u')
				state=37;
			 else
				{k_i=1;
				 pushback(infile,2);
				 state=0;
				}
			 break;
		 case 37:if(ch=='t')
				state=38;
			 else
				{k_i=1;
				 pushback(infile,3);
				 state=0;
				}
			 break;
		 case 38:if((!isalnum(ch))&&(ch!='_'))
				{pushback(infile);
				 state=39;
				}
			 else
				{k_i=1;
				 pushback(infile,4);
				 state=0;
				}
			 break;
		 case 39:t.type=9;
			 flag=1;
			 break;
		 case 40:if(ch=='e')
				state=41;
			 else
				{k_i=1;
				 pushback(infile,2);
				 state=0;
				}
			 break;
		 case 41:if(ch=='t')
				state=42;
			 else
				{k_i=1;
				 pushback(infile,3);
				 state=0;
				}
			 break;
		 case 42:if(ch=='u')
				state=43;
			 else
				{k_i=1;
				 pushback(infile,4);
				 state=0;
				}
			 break;
		 case 43:if(ch=='r')
				state=44;
			 else
				{k_i=1;
				 pushback(infile,5);
				 state=0;
				}
			 break;
		 case 44:if(ch=='n')
				state=45;
			 else
				{k_i=1;
				 pushback(infile,6);
				 state=0;
				}
			 break;
		 case 45:if((!isalnum(ch))&&(ch!='_'))
				{pushback(infile);
				 state=46;
				}
			 else
				{k_i=1;
				 pushback(infile,7);
				 state=0;
				}
			 break;
		 case 46:t.type=10;
			 flag=1;
			 break;
		 case 47:if(ch=='h')
				state=48;
			 else
				{k_i=1;
				 pushback(infile,2);
				 state=0;
				}
			 break;
		 case 48:if(ch=='i')
				state=49;
			 else
				{k_i=1;
				 pushback(infile,3);
				 state=0;
				}
			 break;
		 case 49:if(ch=='l')
				state=50;
			 else
				{k_i=1;
				 pushback(infile,4);
				 state=0;
				}
			 break;
		 case 50:if(ch=='e')
				state=51;
			 else
				{k_i=1;
				 pushback(infile,5);
				 state=0;
				}
			 break;
		 case 51:if((!isalnum(ch))&&(ch!='_'))
				{pushback(infile);
				 state=52;
				}
			 else
				{k_i=1;
				 pushback(infile,6);
				 state=0;
				}
			 break;
		 case 52:t.type=11;
			 flag=1;
			 break;
		 case 53:if((isalnum(ch))||(ch=='_'))
				{identifier_name[identifier_name_length]=ch;
				 identifier_name_length++;
				}
			 else
				{pushback(infile);
				 state=54;
				}
			 break;
		 case 54:identifier_name[identifier_name_length]='\0';
			 if(identifier_name_length>8)
				{error_handler(301);
				 flag=1;
				 break;
				}
			 t.type=12;
			 t.val=st_insert_inquire(identifier_name,0);
			 k_i=0;
			 flag=1;
			 break;
		 case 55:if((isalnum(ch))||(ch=='_'))
				{identifier_name[identifier_name_length]=ch;
				 identifier_name_length++;
				}
			 else
				{pushback(infile);
				 state=56;
				}
			 break;
		 case 56:identifier_name[identifier_name_length]='\0';
			 if(identifier_name_length>8)
				{error_handler(301);
				 flag=1;
				 break;
				}
			 t.type=13;
			 t.val=st_insert_inquire(identifier_name,1);
			 k_i=0;
			 flag=1;
			 break;
		 case 57:if(isdigit(ch))
				numeric_literal=numeric_literal*10+ch-'0';
			 else
				{pushback(infile);
				 state=58;
				}
			 break;
		 case 58:if(numeric_literal>127)
				{error_handler(302);
				 flag=1;
				 break;
				}
			 t.type=14;
			 t.val=numeric_literal;
			 flag=1;
			 break;
		 case 59:if(ch=='+')
				state=60;
			 else
				{pushback(infile);
				 state=61;
				}
			 break;
		 case 60:t.type=15;
			 t.val=1;
			 flag=1;
			 break;
		 case 61:t.type=18;
			 t.val=1;
			 flag=1;
			 break;
		 case 62:if(ch=='-')
				state=63;
			 else
				{pushback(infile);
				 state=64;
				}
			 break;
		 case 63:t.type=15;
			 t.val=2;
			 flag=1;
			 break;
		 case 64:t.type=18;
			 t.val=2;
			 flag=1;
			 break;
		 case 65:t.type=16;
			 t.val=1;
			 flag=1;
			 break;
		 case 66:t.type=17;
			 t.val=1;
			 flag=1;
			 break;
		 case 67:t.type=17;
			 t.val=2;
			 flag=1;
			 break;
		 case 68:t.type=17;
			 t.val=3;
			 flag=1;
			 break;
		 case 69:if(ch=='=')
				state=70;
			 else
				{pushback(infile);
				 state=71;
				}
			 break;
		 case 70:t.type=21;
			 t.val=2;
			 flag=1;
			 break;
		 case 71:t.type=19;
			 t.val=1;
			 flag=1;
			 break;
		 case 72:if(ch=='&')
				state=73;
			 else
				error_handler(303);
			 break;
		 case 73:t.type=20;
			 t.val=1;
			 flag=1;
			 break;
		 case 74:if(ch=='|')
				state=75;
			 else
				error_handler(303);
			 break;
		 case 75:t.type=20;
			 t.val=2;
			 flag=1;
			 break;
		 case 76:if(ch=='=')
				state=77;
			 else
				{pushback(infile);
				 state=78;
				}
			 break;
		 case 77:t.type=21;
			 t.val=1;
			 flag=1;
			 break;
		 case 78:t.type=22;
			 t.val=1;
			 flag=1;
			 break;
		 case 79:if(ch=='=')
				state=80;
			 else
				{pushback(infile);
				 state=81;
				}
			 break;
		 case 80:t.type=21;
			 t.val=4;
			 flag=1;
			 break;
		 case 81:t.type=21;
			 t.val=3;
			 flag=1;
			 break;
		 case 82:if(ch=='=')
				state=83;
			 else
				{pushback(infile);
				 state=84;
				}
			 break;
		 case 83:t.type=21;
			 t.val=6;
			 flag=1;
			 break;
		 case 84:t.type=21;
			 t.val=5;
			 flag=1;
			 break;
		 case 85:t.type=23;
			 t.val=1;
			 flag=1;
			 break;
		 case 86:t.type=23;
			 t.val=2;
			 flag=1;
			 break;
		 case 87:t.type=23;
			 t.val=3;
			 flag=1;
			 break;
		 case 88:t.type=23;
			 t.val=4;
			 flag=1;
			 break;
		 case 89:t.type=24;
			 t.val=1;
			 flag=1;
			 break;
		 case 90:t.type=24;
			 t.val=2;
			 flag=1;
			 break;
		 case 91:t.type=25;
			 t.val=toascii(ch);
			 flag=1;
		}
	}

 if((t.type!=0)&&(t.type!=100)&&(header_mode==1))
	header_mode=0;

 // Return the Token Recognized
 return t;
}


// Prototypes of Functions Invoked by the Syntax Analyzer
// One Function for each Nonterminal in the Source Language Grammar
void program();
void flist();
void fdef();
void plist();
void slist();
void statement();
void dlist();
void dunit();
void alist();
void unary_expression();
void exponential_expression();
void exponential_expression_prime();
void multiplicative_expression();
void multiplicative_expression_prime();
void additive_expression();
void additive_expression_prime();
void and_expression();
void and_expression_prime();
void or_expression();
void or_expression_prime();
void expression();
void expression_prime();


// The Syntax Analyzer
void syntax_analyzer()
{program();
 cout<<"SUCCESSFUL COMPILATION!!!\n";
}

void program()
{token t;
 /* RULE NOS: 1-2
    RULES: program --> main { slist }
		     | flist main { slist }        */
 t=next(1);
 if(t.type==8)
	{intermediate_code_generator(0x0);
	 P_COUNT=0;
	 NO_ID=0;
	 t=next(1);
	 if((t.type==23)&&(t.val==3))
		{slist();
		 t=next(1);
		 if((t.type==23)&&(t.val==4))
			{intermediate_code_generator(0x2);
			 return;
			}
		}
	}
 else
	{save(t);
	 flist();
	 t=next(1);
	 if(t.type==8)
		{intermediate_code_generator(0x0);
		 P_COUNT=0;
		 NO_ID=NO_FUNC;
		 t=next(1);
		 if((t.type==23)&&(t.val==3))
			{slist();
			 t=next(1);
			 if((t.type==23)&&(t.val==4))
				{intermediate_code_generator(0x2);
				 return;
				}
			}
		}
	}
 error_handler(400);
}

void flist()
{token t;
 /* RULE NOS: 3-4
    RULES: flist --> fdef
		   | fdef flist        */
 fdef();
 t=next(1);
 if(t.type==4)
	{save(t);
	 flist();
	 return;
	}
 else
	{save(t);
	 return;
	}
}

void fdef()
{token t;
 /* RULE NOS: 5-8
    RULES: fdef --> function identifier ( ) { }
		  | function identifier ( plist ) { }
		  | function identifier ( ) { slist }
		  | function identifier ( plist ) { slist }        */
 t=next(1);
 if(t.type==4)
	{ST_MODE=0;
	 t=next(2);
	 ST_MODE=1;
	 if(t.type==13)
		{NO_FUNC++;
		 NO_ID=0;
		 ST1.list[t.val].address=NEXT_ADDR;
		 t=next(1);
		 if((t.type==23)&&(t.val==1))
			{t=next(1);
			 if((t.type==23)&&(t.val==2))
				{t=next(1);
				 if((t.type==23)&&(t.val==3))
					{t=next(1);
					 if((t.type==23)&&(t.val==4))
						{ST_SIZE=NO_FUNC;
						 return;
						}
					 else
						{save(t);
						 slist();
						 t=next(1);
						 if((t.type==23)&&(t.val==4))
							{ST_SIZE=NO_FUNC;
							 return;
							}
						}
					}
				}
			 else
				{save(t);
				 plist();
				 t=next(1);
				 if((t.type==23)&&(t.val==2))
					{t=next(1);
					 if((t.type==23)&&(t.val==3))
						{t=next(1);
						 if((t.type==23)&&(t.val==4))
							{ST_SIZE=NO_FUNC;
							 return;
							}
						 else
							{save(t);
							 slist();
							 t=next(1);
							 if((t.type==23)&&(t.val==4))
								{ST_SIZE=NO_FUNC;
								 return;
								}
							}
						}
					}
				}
			}
		}
	}
 error_handler(401);
 slist();
}

void plist()
{token t;
 /* RULE NOS: 9-10
    RULES: plist --> int identifier
		   | int identifier , plist        */
 t=next(1);
 if(t.type==7)
	{ST_MODE=0;
	 t=next(1);
	 ST_MODE=1;
	 if(t.type==12)
		{intermediate_code_generator(0xB); //Pop a parameter
		 intermediate_code_generator(0x4,t.val);
		 NO_ID++;
		 t=next(1);
		 if((t.type==24)&&(t.val==1))
			{plist();
			 return;
			}
		 else
			{save(t);
			 return;
			}
		}
	}
 error_handler(402);
 slist();
}

void slist()
{token t;
 /* RULE NOS: 11-12
    RULES: slist --> statement
		   | statement slist        */
 statement();
 t=next(1);
 if((t.type==23)&&(t.val==4))
	{save(t);
	 return;
	}
 else
	{save(t);
	 slist();
	 return;
	}
}

void statement()
{int m,m1; //Addresses of identifiers in symbol table
 char ch1,ch2; //asm instruction characters
 int i,temp;
 int a_addr1,b_addr1,a_addr2,b_addr2; //Address to be backpatched later
 token t,t1;
 t=next(1);
 /* RULE NO: 13
    RULE: statement --> ;         */
 if((t.type==24)&&(t.val==2))
	{intermediate_code_generator(0x1);
	 return;
	}
 /* RULE NO: 15
    RULE: statement --> int dlist ;         */
 if(t.type==7)
	{dlist();
	 t=next(1);
	 if((t.type==24)&&(t.val==2))
		return;
	}
 /* RULE NOS: 16,30-31
    RULE: statement --> identifier = expression ;
		      | identifier = call identifier ( ) ;
		      | identifier = call identifier ( alist ) ;        */
 if(t.type==12)
	{m=t.val;
	 t1=next(1);
	 if((t1.type==22)&&(t1.val==1))
		{t=next(1);
		 if(t.type==2)
			{t=next(2);
			 if(t.type==13)
				{t=next(1);
				 if((t.type==23)&&(t.val==1))
					{P_COUNT=0;
					 t=next(1);
					 if((t.type==23)&&(t.val==2))
						{t=next(1);
						 if((t.type==24)&&(t.val==2))
							{intermediate_code_generator(0x5,0x0);
							 intermediate_code_generator(0xA); //Push to create space for old SP
							 intermediate_code_generator(0x5,0x0);
							 intermediate_code_generator(0xA); //Push to create space for return value
							 intermediate_code_generator(0x5,NO_ID);
							 intermediate_code_generator(0xA); //Push increment in BP
							 a_addr1=ASM_FILE.tellp()+4;
							 b_addr1=BIN_FILE.tellp()+1;
							 intermediate_code_generator(0x5,0x0);
							 intermediate_code_generator(0xA); //Push to create space for return address
							 intermediate_code_generator(0x5,0x0);
							 intermediate_code_generator(0xA); //Push number of parameters
							 intermediate_code_generator(0x1D,FUNCTION_ADDR); //Actual call
							 backpatch(a_addr1,b_addr1);
							 intermediate_code_generator(0xB); //Pop return value
							 intermediate_code_generator(0x4,m);
							 intermediate_code_generator(0xB); //Pop to adjust SP
							 return;
							}
						}
					 else
						{save(t);
						 alist();
						 t=next(1);
						 if((t.type==23)&&(t.val==2))
							{t=next(1);
							 if((t.type==24)&&(t.val==2))
								{intermediate_code_generator(0x5,0x0);
								 intermediate_code_generator(0xA); //Push to create space for old SP
								 intermediate_code_generator(0x5,0x0);
								 intermediate_code_generator(0xA); //Push to create space for return value
								 intermediate_code_generator(0x5,NO_ID);
								 intermediate_code_generator(0xA); //Push increment in BP
								 a_addr1=ASM_FILE.tellp()+4;
								 b_addr1=BIN_FILE.tellp()+1;
								 intermediate_code_generator(0x5,0x0);
								 intermediate_code_generator(0xA); //Push to create space for return address
								 while(!ts_empty())
									{m1=ts_pop();
									 intermediate_code_generator(0x3,m1);
									 intermediate_code_generator(0xA); //Push a parameter
									}
								 intermediate_code_generator(0x5,P_COUNT);
								 intermediate_code_generator(0xA); //Push number of parameters
								 intermediate_code_generator(0x1D,FUNCTION_ADDR); //Actual call
								 backpatch(a_addr1,b_addr1);
								 intermediate_code_generator(0xB); //Pop return value
								 intermediate_code_generator(0x4,m);
								 intermediate_code_generator(0xB); //Pop to adjust SP
								 return;
								}
							}
						}
					}
				}
			}
		 else
			{save(t);
			 expression();
			 intermediate_code_generator(0xB);
			 intermediate_code_generator(0x4,m);
			 t=next(1);
			 if((t.type==24)&&(t.val==2))
				return;
			}
		}
	 else
		{save(t);
		 save(t1);
		}
	}
 /* RULE NO: 17
    RULE: statement --> in ( identifier ) ;        */
 if(t.type==6)
	{t=next(1);
	 if((t.type==23)&&(t.val==1))
		{t=next(1);
		 if(t.type==12)
			{m=t.val;
			 t=next(1);
			 if((t.type==23)&&(t.val==2))
				{t=next(1);
				 if((t.type==24)&&(t.val==2))
					{intermediate_code_generator(0x6);
					 intermediate_code_generator(0x4,m);
					 return;
					}
				 }
			}
		}
	}
 /* RULE NO: 18
    RULE: statement --> out ( identifier ) ;         */
 if(t.type==9)
	{t=next(1);
	 if((t.type==23)&&(t.val==1))
		{t=next(1);
		 if(t.type==12)
			{m=t.val;
			 t=next(1);
			 if((t.type==23)&&(t.val==2))
				{t=next(1);
				 if((t.type==24)&&(t.val==2))
					{intermediate_code_generator(0x3,m);
					 intermediate_code_generator(0x7);
					 return;
					}
				 }
			}
		}
	}
 /* RULE NO: 19
    RULE: statement --> asm ( asm_instruction ) ;        */
 if(t.type==1)
	{t=next(1);
	 if((t.type==23)&&(t.val==1))
		{i=1;
		 do{t=next(3);
		    if((t.val==' ')||(t.val=='\t'))
			continue;
		    if(t.val==41)
			break;
		    if((i==1)||(i==3)||(i==5))
			temp=t.val;
		    if(i==2)
			ch1=convert_to_asm(temp,t.val);
		    if(i==4)
			ch2=convert_to_asm(temp,t.val);
		    i++;
		   }while(1);
		 t=next(1);
		 if((t.type==24)&&(t.val==2))
			{intermediate_code_generator(ch1,ch2);
			 return;
			}
		}
	}
 /* RULE NOS: 20-25
    RULES: statement --> if ( expression ) statement
		       | if ( expression ) { slist }
		       | if ( expression ) statement else statement
		       | if ( expression ) statement else { slist }
		       | if ( expression ) { slist } else statement
		       | if ( expression ) { slist } else { slist }        */
 if(t.type==5)
	{t=next(1);
	 if((t.type==23)&&(t.val==1))
		{expression();
		 t=next(1);
		 if((t.type==23)&&(t.val==2))
			{t=next(1);
			 a_addr1=ASM_FILE.tellp()+8;
			 b_addr1=BIN_FILE.tellp()+2;
			 intermediate_code_generator(0xB);
			 intermediate_code_generator(0x1B,0x0);
			 if((t.type==23)&&(t.val==3))
				{slist();
				 t=next(1);
				 if((t.type==23)&&(t.val==4))
					{t=next(1);
					 if(t.type==3)
						{a_addr2=ASM_FILE.tellp()+4;
						 b_addr2=BIN_FILE.tellp()+1;
						 intermediate_code_generator(0x1A,0x0);
						 backpatch(a_addr1,b_addr1);
						 t=next(1);
						 if((t.type==23)&&(t.val==3))
							{slist();
							 t=next(1);
							 if((t.type==23)&&(t.val==4))
								{backpatch(a_addr2,b_addr2);
								 return;
								}
							}
						 else
							{save(t);
							 statement();
							 backpatch(a_addr2,b_addr2);
							 return;
							}
						}
					 else
						{save(t);
						 backpatch(a_addr1,b_addr1);
						 return;
						}
					}
				}
			 else
				{save(t);
				 statement();
				 t=next(1);
				 if(t.type==3)
					{a_addr2=ASM_FILE.tellp()+4;
					 b_addr2=BIN_FILE.tellp()+1;
					 intermediate_code_generator(0x1A,0x0);
					 backpatch(a_addr1,b_addr1);
					 t=next(1);
					 if((t.type==23)&&(t.val==3))
						{slist();
						 t=next(1);
						 if((t.type==23)&&(t.val==4))
							{backpatch(a_addr2,b_addr2);
							 return;
							}
						}
					 else
						{save(t);
						 statement();
						 backpatch(a_addr2,b_addr2);
						 return;
						}
					}
				 else
					{save(t);
					 backpatch(a_addr1,b_addr1);
					 return;
					}
				}
			}
		}
	}
 /* RULE NOS: 26-27
    RULES: statement --> while ( expression ) statement
		       | while ( expression ) { slist }        */
 if(t.type==11)
	{t=next(1);
	 if((t.type==23)&&(t.val==1))
		{m=NEXT_ADDR;
		 expression();
		 t=next(1);
		 if((t.type==23)&&(t.val==2))
			{t=next(1);
			 a_addr1=ASM_FILE.tellp()+8;
			 b_addr1=BIN_FILE.tellp()+2;
			 intermediate_code_generator(0xB);
			 intermediate_code_generator(0x1B,0x0);
			 if((t.type==23)&&(t.val==3))
				{slist();
				 t=next(1);
				 if((t.type==23)&&(t.val==4))
					{intermediate_code_generator(0x1A,m);
					 backpatch(a_addr1,b_addr1);
					 return;
					}
				}
			 else
				{save(t);
				 statement();
				 intermediate_code_generator(0x1A,m);
				 backpatch(a_addr1,b_addr1);
				 return;
				}
			}
		}
	}
 /* RULE NOS: 28-29
    RULES: statement --> call identifier ( ) ;
		       | call identifier ( alist ) ;        */
 if(t.type==2)
	{t=next(2);
	 if(t.type==13)
		{t=next(1);
		 if((t.type==23)&&(t.val==1))
			{P_COUNT=0;
			 t=next(1);
			 if((t.type==23)&&(t.val==2))
				{t=next(1);
				 if((t.type==24)&&(t.val==2))
					{intermediate_code_generator(0x5,0x0);
					 intermediate_code_generator(0xA); //Push to create space for old SP
					 intermediate_code_generator(0x5,0x0);
					 intermediate_code_generator(0xA); //Push to create space for return value
					 intermediate_code_generator(0x5,NO_ID);
					 intermediate_code_generator(0xA); //Push increment in BP
					 a_addr1=ASM_FILE.tellp()+4;
					 b_addr1=BIN_FILE.tellp()+1;
					 intermediate_code_generator(0x5,0x0);
					 intermediate_code_generator(0xA); //Push to create space for return address
					 intermediate_code_generator(0x5,0x0);
					 intermediate_code_generator(0xA); //Push number of parameters
					 intermediate_code_generator(0x1D,FUNCTION_ADDR); //Actual call
					 backpatch(a_addr1,b_addr1);
					 intermediate_code_generator(0xB); //Pop return value
					 intermediate_code_generator(0xB); //Pop to adjust SP
					 return;
					}
				}
			 else
				{save(t);
				 alist();
				 t=next(1);
				 if((t.type==23)&&(t.val==2))
					{t=next(1);
					 if((t.type==24)&&(t.val==2))
						{intermediate_code_generator(0x5,0x0);
						 intermediate_code_generator(0xA); //Push to create space for old SP
						 intermediate_code_generator(0x5,0x0);
						 intermediate_code_generator(0xA); //Push to create space for return value
						 intermediate_code_generator(0x5,NO_ID);
						 intermediate_code_generator(0xA); //Push increment in BP
						 a_addr1=ASM_FILE.tellp()+4;
						 b_addr1=BIN_FILE.tellp()+1;
						 intermediate_code_generator(0x5,0x0);
						 intermediate_code_generator(0xA); //Push to create space for return address
						 while(!ts_empty())
							{m1=ts_pop();
							 intermediate_code_generator(0x3,m1);
							 intermediate_code_generator(0xA); //Push a parameter
							}
						 intermediate_code_generator(0x5,P_COUNT);
						 intermediate_code_generator(0xA); //Push number of parameters
						 intermediate_code_generator(0x1D,FUNCTION_ADDR); //Actual call
						 backpatch(a_addr1,b_addr1);
						 intermediate_code_generator(0xB); //Pop return value
						 intermediate_code_generator(0xB); //Pop to adjust SP
						 return;
						}
					}
				}
			}
		}
	}
 /* RULE NOS: 32-33
    RULES: statement --> return ;
		       | return expression ;        */
  if(t.type==10)
	{t=next(1);
	 if((t.type==24)&&(t.val==2))
		{intermediate_code_generator(0x5,0x0);
		 intermediate_code_generator(0xA); //Push 0 as return value
		 intermediate_code_generator(0x5,NO_ID);
		 intermediate_code_generator(0xA); //Push decrement in BP
		 intermediate_code_generator(0x1E); //Actual return
		 return;
		}
	 else
		{save(t);
		 expression();
		 t=next(1);
		 if((t.type==24)&&(t.val==2))
			{intermediate_code_generator(0x5,NO_ID);
			 intermediate_code_generator(0xA); //Push decrement in BP
			 intermediate_code_generator(0x1E); //Actual return
			 return;
			}
		}
	}
 /* RULE NO: 14
    RULE: statement --> expression ;        */
 if((t.type==12)||(t.type==14)||(t.type==15)||(t.type==19)
   ||((t.type==23)&&(t.val==1)))
	{if(t.type!=12)
		save(t);
	 expression();
	 t=next(1);
	 if((t.type==24)&&(t.val==2))
		return;
	}
 error_handler(403);
 slist();
}

void dlist()
{token t;
 /* RULE NOS: 36-37
    RULES: dlist --> dunit
		   | dunit , dlist        */
 dunit();
 t=next(1);
 if((t.type==24)&&(t.val==1))
	{dlist();
	 return;
	}
 else
	{save(t);
	 return;
	}
}

void dunit()
{int m; //Address of identifier in symbol table
 token t;
 /* RULE NOS: 38-39
    RULES: dunit --> identifier
		   | identifier = expression        */
 ST_MODE=0;
 t=next(1);
 ST_MODE=1;
 if(t.type==12)
	{m=t.val;
	 NO_ID++;
	 t=next(1);
	 if((t.type==22)&&(t.val==1))
		{expression();
		 intermediate_code_generator(0xB);
		 intermediate_code_generator(0x4,m);
		 return;
		}
	 else
		{save(t);
		 return;
		}
	}
 error_handler(404);
 slist();
}

void alist()
{token t;
 /* RULE NOS: 34-35
    RULES: alist --> identifier
		   | identifier , alist        */
 t=next(1);
 if(t.type==12)
	{P_COUNT++;
	 ts_push(t.val);
	 t=next(1);
	 if((t.type==24)&&(t.val==1))
		{alist();
		 return;
		}
	 else
		{save(t);
		 return;
		}
	}
 error_handler(405);
 slist();
}

void unary_expression()
{int m; //Address of identifier in symbol table
 token t;
 t=next(1);
 /* RULE NOS: 40,44-45
    RULES: unary_expression --> identifier
			      | identifier ++
			      | identifier --        */
 if(t.type==12)
	{m=t.val;
	 t=next(1);
	 if((t.type==15)&&(t.val==1))
		{intermediate_code_generator(0x5,0x1);
		 intermediate_code_generator(0x4,0xFF);
		 intermediate_code_generator(0x3,m);
		 intermediate_code_generator(0xA);
		 intermediate_code_generator(0xC,0xFF);
		 intermediate_code_generator(0x4,m);
		 return;
		}
	 else
		if((t.type==15)&&(t.val==2))
			{intermediate_code_generator(0x5,0x1);
			 intermediate_code_generator(0x4,0xFF);
			 intermediate_code_generator(0x3,m);
			 intermediate_code_generator(0xA);
			 intermediate_code_generator(0xD,0xFF);
			 intermediate_code_generator(0x4,m);
			 return;
			}
		else
			{save(t);
			 intermediate_code_generator(0x3,m);
			 intermediate_code_generator(0xA);
			 return;
			}
	}
 /* RULE NOS: 41,47,50
    RULES: unary_expression --> + identifier
			      | + unsigned_integer_constant
			      | + ( expression )	*/
 if((t.type==18)&&(t.val==1))
	{t=next(1);
	 if(t.type==12)
		{intermediate_code_generator(0x3,t.val);
		 intermediate_code_generator(0xA);
		 return;
		}
	 else
		if(t.type==14)
			{intermediate_code_generator(0x5,t.val);
			 intermediate_code_generator(0xA);
			 return;
			}
		else
			if((t.type==23)&&(t.val==1))
				{expression();
				 t=next(1);
				 if((t.type==23)&&(t.val==2))
					return;
				}
	}
 /* RULE NOS: 42,48,51
    RULES: unary_expression --> - identifier
			      | - unsigned_integer_constant
			      | - ( expression )        */
 if((t.type==18)&&(t.val==2))
	{t=next(1);
	 if(t.type==12)
		{intermediate_code_generator(0x5,0x0);
		 intermediate_code_generator(0xD,t.val);
		 intermediate_code_generator(0xA);
		 return;
		}
	 else
		if(t.type==14)
			{intermediate_code_generator(0x5,t.val);
			 intermediate_code_generator(0x4,0xFF);
			 intermediate_code_generator(0x5,0x0);
			 intermediate_code_generator(0xD,0xFF);
			 intermediate_code_generator(0xA);
			 return;
			}
		else
			if((t.type==23)&&(t.val==1))
				{expression();
				 intermediate_code_generator(0xB);
				 intermediate_code_generator(0x4,0xFF);
				 intermediate_code_generator(0x5,0x0);
				 intermediate_code_generator(0xD,0xFF);
				 intermediate_code_generator(0xA);
				 t=next(1);
				 if((t.type==23)&&(t.val==2))
					return;
				}
	}
 /* RULE NOS: 43,52
    RULES: unary_expression --> ! identifier
			      | ! ( expression )        */
 if((t.type==19)&&(t.val==1))
	{t=next(1);
	 if(t.type==12)
		{intermediate_code_generator(0x3,t.val);
		 intermediate_code_generator(0x13);
		 intermediate_code_generator(0xA);
		 return;
		}
	 else
		if((t.type==23)&&(t.val==1))
			{expression();
			 intermediate_code_generator(0xB);
			 intermediate_code_generator(0x13);
			 intermediate_code_generator(0xA);
			 t=next(1);
			 if((t.type==23)&&(t.val==2))
				return;
			}
	}
 /* RULE NO: 46
    RULE: unary_expression --> unsigned_integer_constant        */
 if(t.type==14)
	{intermediate_code_generator(0x5,t.val);
	 intermediate_code_generator(0xA);
	 return;
	}
 /* RULE NO: 49
    RULES: unary_expression --> ( expression )        */
 if((t.type==23)&&(t.val==1))
	{expression();
	 t=next(1);
	 if((t.type==23)&&(t.val==2))
		return;
	}
 error_handler(406);
 slist();
}

void exponential_expression()
{token t;
 /* RULE NOS: 53-54
    RULES: exponential_expression --> unary_expression
				    | unary_expression exponential_expression'        */
 unary_expression();
 t=next(1);
 if(t.type==16)
	{save(t);
	 exponential_expression_prime();
	 return;
	}
 else
	{save(t);
	 return;
	}
}

void exponential_expression_prime()
{token t;
 /* RULE NOS: 55-56
    RULES: exponential_expression' --> ^ unary_expression
				     | ^ unary_expression exponential_expression'        */
 t=next(1);
 if((t.type==16)&&(t.val==1))
	{unary_expression();
	 intermediate_code_generator(0xB);
	 intermediate_code_generator(0x4,0xFF);
	 intermediate_code_generator(0xB);
	 intermediate_code_generator(0x10,0xFF);
	 intermediate_code_generator(0xA);
	 t=next(1);
	 if(t.type==16)
		{save(t);
		 exponential_expression_prime();
		 return;
		}
	 else
		{save(t);
		 return;
		}
	}
 error_handler(406);
 slist();
}

void multiplicative_expression()
{token t;
 /* RULE NOS: 57-58
    RULES: multiplicative_expression --> exponential_expression
				       | exponential_expression multiplicative_expression'        */
 exponential_expression();
 t=next(1);
 if(t.type==17)
	{save(t);
	 multiplicative_expression_prime();
	 return;
	}
 else
	{save(t);
	 return;
	}
}

void multiplicative_expression_prime()
{token t;
 /* RULE NOS: 59-64
    RULES: multiplicative_expression' --> * exponential_expression
					| / exponential_expression
					| % exponential_expression
					| * exponential_expression multiplicative_expression'
					| / exponential_expression multiplicative_expression'
					| % exponential_expression multiplicative_expression'        */
 t=next(1);
 if((t.type==17)&&((t.val==1)||(t.val==2)||(t.val==3)))
	{exponential_expression();
	 intermediate_code_generator(0xB);
	 intermediate_code_generator(0x4,0xFF);
	 intermediate_code_generator(0xB);
	 if(t.val==1)
		intermediate_code_generator(0xE,0xFF);
	 if(t.val==2)
		intermediate_code_generator(0xF,0xFF);
	 if(t.val==3)
		{intermediate_code_generator(0xF,0xFF);
		 intermediate_code_generator(0x8);
		}
	 intermediate_code_generator(0xA);
	 t=next(1);
	 if(t.type==17)
		{save(t);
		 multiplicative_expression_prime();
		 return;
		}
	 else
		{save(t);
		 return;
		}
	}
 error_handler(406);
 slist();
}

void additive_expression()
{token t;
 /* RULE NOS: 65-66
    RULES: additive_expression --> multiplicative_expression
				 | multiplicative_expression additive_expression'        */
 multiplicative_expression();
 t=next(1);
 if(t.type==18)
	{save(t);
	 additive_expression_prime();
	 return;
	}
 else
	{save(t);
	 return;
	}
}

void additive_expression_prime()
{token t;
 /* RULE NOS: 67-70
    RULES: additive_expression' --> + multiplicative_expression
				  | - multiplicative_expression
				  | + multiplicative_expression additive_expression'
				  | - multiplicative_expression additive_expression'        */
 t=next(1);
 if((t.type==18)&&((t.val==1)||(t.val==2)))
	{multiplicative_expression();
	 intermediate_code_generator(0xB);
	 intermediate_code_generator(0x4,0xFF);
	 intermediate_code_generator(0xB);
	 if(t.val==1)
		intermediate_code_generator(0xC,0xFF);
	 if(t.val==2)
		intermediate_code_generator(0xD,0xFF);
	 intermediate_code_generator(0xA);
	 t=next(1);
	 if(t.type==18)
		{save(t);
		 additive_expression_prime();
		 return;
		}
	 else
		{save(t);
		 return;
		}
	}
 error_handler(406);
 slist();
}

void and_expression()
{token t;
 /* RULE NOS: 71-72
    RULES: and_expression --> additive_expression
			    | additive_expression and_expression'        */
 additive_expression();
 t=next(1);
 if((t.type==20)&&(t.val==1))
	{save(t);
	 and_expression_prime();
	 return;
	}
 else
	{save(t);
	 return;
	}
}

void and_expression_prime()
{token t;
 /* RULE NOS: 73-74
    RULES: and_expression' --> && additive_expression
			     | && additive_expression and_expression'        */
 t=next(1);
 if((t.type==20)&&(t.val==1))
	{additive_expression();
	 intermediate_code_generator(0xB);
	 intermediate_code_generator(0x4,0xFF);
	 intermediate_code_generator(0xB);
	 intermediate_code_generator(0x11,0xFF);
	 intermediate_code_generator(0xA);
	 t=next(1);
	 if((t.type==20)&&(t.val==1))
		{save(t);
		 and_expression_prime();
		 return;
		}
	 else
		{save(t);
		 return;
		}
	}
 error_handler(406);
 slist();
}

void or_expression()
{token t;
 /* RULE NOS: 75-76
    RULES: or_expression --> and_expression
			    | and_expression or_expression'        */
 and_expression();
 t=next(1);
 if((t.type==20)&&(t.val==2))
	{save(t);
	 or_expression_prime();
	 return;
	}
 else
	{save(t);
	 return;
	}
}

void or_expression_prime()
{token t;
 /* RULE NOS: 77-78
    RULES: or_expression' --> || and_expression
			    | || and_expression or_expression'        */
 t=next(1);
 if((t.type==20)&&(t.val==2))
	{and_expression();
	 intermediate_code_generator(0xB);
	 intermediate_code_generator(0x4,0xFF);
	 intermediate_code_generator(0xB);
	 intermediate_code_generator(0x12,0xFF);
	 intermediate_code_generator(0xA);
	 t=next(1);
	 if((t.type==20)&&(t.val==2))
		{save(t);
		 or_expression_prime();
		 return;
		}
	 else
		{save(t);
		 return;
		}
	}
 error_handler(406);
 slist();
}

void expression()
{token t;
 /* RULE NOS: 79-80
    RULES: expression --> or_expression
			| or_expression expression'        */
 or_expression();
 t=next(1);
 if(t.type==21)
	{save(t);
	 expression_prime();
	 return;
	}
 else
	{save(t);
	 return;
	}
}

void expression_prime()
{token t;
 /* RULE NOS: 81-92
    RULES: expression' --> == or_expression
			 | != or_expression
			 | < or_expression
			 | <= or_expression
			 | > or_expression
			 | >= or_expression
			 | == or_expression expression'
			 | != or_expression expression'
			 | < or_expression expression'
			 | <= or_expression expression'
			 | > or_expression expression'
			 | >= or_expression expression'        */
 t=next(1);
 if((t.type==21)&&((t.val==1)||(t.val==2)||(t.val==3)||(t.val==4)||(t.val==5)||(t.val==6)))
	{or_expression();
	 intermediate_code_generator(0xB);
	 intermediate_code_generator(0x4,0xFF);
	 intermediate_code_generator(0xB);
	 if(t.val==1)
		intermediate_code_generator(0x14,0xFF);
	 if(t.val==2)
		intermediate_code_generator(0x15,0xFF);
	 if(t.val==3)
		intermediate_code_generator(0x16,0xFF);
	 if(t.val==4)
		intermediate_code_generator(0x17,0xFF);
	 if(t.val==5)
		intermediate_code_generator(0x18,0xFF);
	 if(t.val==6)
		intermediate_code_generator(0x19,0xFF);
	 intermediate_code_generator(0xA);
	 t=next(1);
	 if(t.type==21)
		{save(t);
		 expression_prime();
		 return;
		}
	 else
		{save(t);
		 return;
		}
	}
 error_handler(406);
 slist();
}


// Intermediate Code Generator Using Syntax Directed Translation
void intermediate_code_generator(char op,char operand)
{char str[3];
 // Operator
 switch(op)
	{case  0x0:ASM_FILE<<"ORG";
		   break;
	 case  0x1:ASM_FILE<<"NOP";
		   break;
	 case  0x2:ASM_FILE<<"HLT";
		   break;
	 case  0x3:ASM_FILE<<"LOD ";
		   break;
	 case  0x4:ASM_FILE<<"STR ";
		   break;
	 case  0x5:ASM_FILE<<"LDI ";
		   break;
	 case  0x6:ASM_FILE<<"IN";
		   break;
	 case  0x7:ASM_FILE<<"OUT";
		   break;
	 case  0x8:ASM_FILE<<"MOV A,B";
		   break;
	 case  0x9:ASM_FILE<<"MOV B,A";
		   break;
	 case  0xA:ASM_FILE<<"PSH";
		   break;
	 case  0xB:ASM_FILE<<"POP";
		   break;
	 case  0xC:ASM_FILE<<"ADD ";
		   break;
	 case  0xD:ASM_FILE<<"SUB ";
		   break;
	 case  0xE:ASM_FILE<<"MUL ";
		   break;
	 case  0xF:ASM_FILE<<"DIV ";
		   break;
	 case 0x10:ASM_FILE<<"EXP ";
		   break;
	 case 0x11:ASM_FILE<<"AND ";
		   break;
	 case 0x12:ASM_FILE<<"OR ";
		   break;
	 case 0x13:ASM_FILE<<"NOT";
		   break;
	 case 0x14:ASM_FILE<<"EQ ";
		   break;
	 case 0x15:ASM_FILE<<"NEQ ";
		   break;
	 case 0x16:ASM_FILE<<"LT ";
		   break;
	 case 0x17:ASM_FILE<<"LTE ";
		   break;
	 case 0x18:ASM_FILE<<"GT ";
		   break;
	 case 0x19:ASM_FILE<<"GTE ";
		   break;
	 case 0x1A:ASM_FILE<<"JMP ";
		   break;
	 case 0x1B:ASM_FILE<<"JZ ";
		   break;
	 case 0x1C:ASM_FILE<<"JNZ ";
		   break;
	 case 0x1D:ASM_FILE<<"CAL ";
		   break;
	 case 0x1E:ASM_FILE<<"RET";
		   break;
	}
 if(op==13)
	op=-1;
 if(op==26)
	op=-2;
 BIN_FILE.write((char*)&op,sizeof(char));
 if(op==-1)
	op=13;
 if(op==-2)
	op=26;
 NEXT_ADDR++;
 // Operand
 if((op==0x3)||(op==0x4)||(op==0x5)||(op==0xC)||(op==0xD)||(op==0xE)
   ||(op==0xF)||(op==0x10)||(op==0x11)||(op==0x12)||(op==0x14)
   ||(op==0x15)||(op==0x16)||(op==0x17)||(op==0x18)||(op==0x19)
   ||(op==0x1A)||(op==0x1B)||(op==0x1C)||(op==0x1D))
	{itoa(str,operand);
	 ASM_FILE<<str;
	 if(operand==13)
		operand=-1;
	 if(operand==26)
		operand=-2;
	 BIN_FILE.write((char*)&operand,sizeof(char));
	 NEXT_ADDR++;
	}
 ASM_FILE<<"\n";
}


// Backpatching
void backpatch(int a_addr,int b_addr)
{/*Writes the current value of NEXT_ADDR as the target address of the jump
  instructions at the address a_addr in the asm file and at the address
  b_addr in the binary file
 */
 char addr;
 char str[3];
 long temp;
 addr=char(NEXT_ADDR);
 itoa(str,addr);
 temp=ASM_FILE.tellp();
 ASM_FILE.seekp(a_addr);
 ASM_FILE<<str;
 ASM_FILE.seekp(temp);
 temp=BIN_FILE.tellp();
 BIN_FILE.seekp(b_addr);
 if(addr==13)
	addr=-1;
 if(addr==26)
	addr=-2;
 BIN_FILE.write((char*)&addr,sizeof(char));
 BIN_FILE.seekp(temp);
}


// Bookkeeping Module
int st_insert_inquire(char name[],int v_f)
{if(ST_MODE==1)	// Inquire
	{for(int i=0;i<ST_SIZE;i++)
		if(!strcmp(ST1.list[i].name,name))
			{if(v_f)
				FUNCTION_ADDR=ST1.list[i].address;
			 return i;
			}
	 error_handler(500);
	}
 else           // Insert
	{for(int i=0;i<ST_SIZE;i++)
		if(!strcmp(ST1.list[i].name,name))
			error_handler(501);
	 strcpy(ST1.list[ST_SIZE].name,name);
	 ST1.list[ST_SIZE].v_f=v_f;
	 return ST_SIZE++;
	}
 return -1;
}


// Error Handler Module
void error_handler(int error_code)
{static int first_error=1; //Is this the first error in the program? 1:yes, 0:no
 // Displaying Error Messages
 if(first_error)
	{cout<<"ERROR(S):\n";
	 first_error=0;
	}
 switch(error_code)
	{// Source and Header Files Related Errors
	 case 100:cout<<"ILLEGAL NUMBER OF COMMAND LINE ARGUMENTS.\n";
		  break;
	 case 101:cout<<"ILLEGAL SOURCE FILE NAME.\n";
		  break;
	 case 102:cout<<"ILLEGAL SOURCE FILE TYPE.\n";
		  break;
	 case 103:cout<<"FILE NOT FOUND.\n";
		  break;
	 // Preprocessing Errors
	 case 200:cout<<FILENAME<<" "<<LINE_NO<<": INCOMPLETE MULTIPLE LINE COMMENT.\n";
		  break;
	 case 201:cout<<FILENAME<<" "<<LINE_NO<<": ILLEGAL PREPROCESSOR DIRECTIVE.\n";
		  break;
	 case 202:cout<<FILENAME<<" "<<LINE_NO<<": HEADER FILE NAME TOO LONG.\n";
		  break;
	 case 203:cout<<FILENAME<<" "<<LINE_NO<<": ILLEGAL HEADER FILE TYPE.\n";
		  break;
	 case 204:cout<<FILENAME<<" "<<LINE_NO<<": HEADER FILE NOT FOUND.\n";
		  break;
	 // Lexical Errors
	 case 300:cout<<FILENAME<<" "<<LINE_NO<<": UNKNOWN TOKEN.\n";
		  break;
	 case 301:cout<<FILENAME<<" "<<LINE_NO<<": IDENTIFIER NAME TOO LONG.\n";
		  break;
	 case 302:cout<<FILENAME<<" "<<LINE_NO<<": INTEGER TOO BIG.\n";
		  break;
	 case 303:cout<<FILENAME<<" "<<LINE_NO<<": UNKNOWN OPERATOR.\n";
		  break;
	 case 304:exit(0);
	 // Parsing Errors
	 case 400:cout<<FILENAME<<" "<<LINE_NO<<": INCORRECT PROGRAM LAYOUT.\n";
		  break;
	 case 401:cout<<FILENAME<<" "<<LINE_NO<<": INCORRECT FUNCTION DEFINITION.\n";
		  break;
	 case 402:cout<<FILENAME<<" "<<LINE_NO<<": INCORRECT FUNCTION PARAMETERS.\n";
		  break;
	 case 403:cout<<FILENAME<<" "<<LINE_NO<<": INCORRECT STATEMENT.\n";
		  break;
	 case 404:cout<<FILENAME<<" "<<LINE_NO<<": INCORRECT VARIABLE DECLARATION.\n";
		  break;
	 case 405:cout<<FILENAME<<" "<<LINE_NO<<": FUNCTION CALL WITH INCORRECT ARGUMENTS.\n";
		  break;
	 case 406:cout<<FILENAME<<" "<<LINE_NO<<": INCORRECT EXPRESSION.\n";
		  break;
	 // Semantic Errors
	 case 500:cout<<FILENAME<<" "<<LINE_NO<<": UNDECLARED IDENTIFIER.\n";
		  break;
	 case 501:cout<<FILENAME<<" "<<LINE_NO<<": MULTIPLE DECLARATIONS OF IDENTIFIER.\n";
		  break;
	}
 // Handling the Errors
 token t;
 t.type=-1;
 save(t);
 save(t);
 if((error_code>=300)&&(error_code<400))
	{ERROR_CATEGORY=3;
	 lexical_analyzer(4);
	 return;
	}
 if((error_code>=400)&&(error_code<500))
	{ERROR_CATEGORY=4;
	 lexical_analyzer(4);
	 return;
	}
 exit(0);
}


// About the SIC Compiler
void about()
{cout<<"Simple Instructional Compiler\n";
 cout<<"Usage: compiler file.c\n";
 cout<<"       hm file.bin\n";
}


// Function to Push Characters Back in a File
void pushback(ifstream *infile,int pos)
{infile->seekg((infile->tellg())-pos);
}


// Fetch the Next Token for the Syntax Analyzer
token next(int mode)
{token t;
 if(SAVED1.type==-1)
	if(SAVED2.type==-1)
		{do{t=lexical_analyzer(mode);
		   }while((t.type==0)||(t.type==100));
		 return t;
		}
	else
		{t=SAVED2;
		 SAVED2.type=-1;
		 return t;
		}
 else
	{t=SAVED1;
	 SAVED1=SAVED2;
	 SAVED2.type=-1;
	 return t;
	}
}


// Save a Token for Future Use
void save(token t)
{SAVED1=SAVED2;
 SAVED2=t;
}


// Initialize Temporary Stack
void ts_init()
{TS.top=0;
}


// Check if Temporary Stack Empty
int ts_empty()
{if(TS.top)
	return 0;
 else
	return 1;
}


// Push an Element on Temporary Stack
void ts_push(int val)
{TS.A[TS.top++]=val;
}


// Pop an Element from Temporary Stack
int ts_pop()
{return TS.A[--TS.top];
}


// Convert Two Integers into 1 Byte of an asm Instruction
char convert_to_asm(char c1,char c2)
{char val;
 if(isdigit(c1))
	val=c1-'0';
 else
	if(islower(c1))
		val=c1-'a'+10;
	else
		if(isupper(c1))
			val=c1-'A'+10;
 if(isdigit(c2))
	val=val*16+c2-'0';
 else
	if(islower(c2))
		val=val*16+c2-'a'+10;
	else
		if(isupper(c2))
			val=val*16+c2-'A'+10;
 return val;
}


// Convert an Integer into a String of Hexadecimal Digits
void itoa(char str[],unsigned char val)
{char d;
 d=val/16;
 if(d<10)
	str[0]='0'+d;
 else
	str[0]='A'+d-10;
 d=val%16;
 if(d<10)
	str[1]='0'+d;
 else
	str[1]='A'+d-10;
 str[2]='\0';
}
