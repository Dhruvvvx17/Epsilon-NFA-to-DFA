/*NFA TO DFA COnversion code:
1) We are increasing the size of the DFA as we get new states while processing the NFA table
2) We are processing in alphabetical order. Eg: 01, 0123, 23 ,13 etc. 31,213 etc not allowed
3) We are taking in phi for ""*/

#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#define STATES  256
#define SYMBOLS 20
/*Global variables. Have to make them local */

int N_symbols; 
int NFA_states;
int DFA_states;	//Number of DFA states

/*Print state-transition table.*/
/*Here we are first printing the borders. Then first the table with character symbols(A,B etc) . Then we will print what each character stands for (A="012" etc) */

void print_dfa_table(int tab[][SYMBOLS],char dfa_res[][SYMBOLS],int nstates,int nsymbols)
{
    	int i, j;
	printf("STATE TRANSITION TABLE\n");
    	printf("     | ");
    	for (i = 0; i < nsymbols; i++) 
		printf("  %c  ", '0'+i);
 	printf("\n-----+--");
    	for (i = 0; i < nsymbols; i++) 
		printf("-----");
	printf("\n");
 	//Will print table with character states using DFAttab table
    	for (i = 0; i < nstates; i++) 
	{
        	printf("  %c  | ", 'A'+i);
        	for (j = 0; j < nsymbols; j++)
            		printf("  %c  ", 'A'+ tab[i][j]);	//0 is A, B= 'A' + 1 ,etc..
        	printf("\n");
    	}
	//Printing what each of the states are using dfa_res table
   	printf("\nWhere the states are: \n");
   
    	for(int k=0;k<nstates;k++)
    	{
		if(strlen(dfa_res[k])==0)
		{
			strcpy(dfa_res[k],"dead state");
		}
    		printf("%c : %s \n",'A'+k,dfa_res[k]);
	}
}
 
/*String 't' is merged into 's' in an alphabetical order.*/
void string_merge(char *s, char *t)
{
    	char temp[STATES], *r=temp, *p=s;		//*t is NFAtab, *s is temp from get_next_state()
 	while (*p && *t) 				//For first chracter, *p will be null, so while loop not executed. FOr next character onwards while loop will be executed to copy in correct 								//alphabetical order
	{
        	if (*p == *t)
		{
            		*r++ = *p++; 
			 t++;
		}
         	else if (*p < *t)
		{
            		*r++ = *p++;
		}
         	else
            		*r++ = *t++;
    	}
    	*r = '\0';
 
    	if(*p) 
		strcat(r, p);
    	else if(*t) 
		strcat(r, t);
 
    	strcpy(s, temp);
   
}
/*Initialize NFA table.*/
void init_NFA_table(char *nfa[STATES][SYMBOLS])
{
	int nf;
	int symb;
	printf("Enter number of states symbols: ");
	//scanf("%d",&nf);
	//scanf("%d",&symb);
	scanf("%d %d",&nf,&symb);
	printf("\n");
	getchar();
	NFA_states = nf;
    	DFA_states = 0;
    	N_symbols = symb;
	printf("Enter transition table : \n");
	for(int i=0;i<nf;i++)
	{
		for(int j=0;j<=symb;j++)	//For E, take string excluding current symbol. then call string_merge to merge current character with string 
		{
			char *st=(char*)malloc(sizeof(char)*SYMBOLS);	// Neccesary to get different string locations each time so pointer in NFAtab[i][j] will not point to same string
			scanf("%[^\n]",st);
			//scanf("%s",st);
			getchar();		//To take care of \n character
			if(strcmp(st,"null")==0)	//If it is phi
				st="";
			nfa[i][j]=st;	//HAve to use equality operator. strcat will not work as NFAtab[i][j] is a pointer to a string, not string itself. 
		}
	}

    /*	NFAtab[0][0] = "01";
    	NFAtab[0][1] = "03";
    	NFAtab[1][0] = "";
    	NFAtab[1][1] = "2";
    	NFAtab[2][0] = "";
    	NFAtab[2][1] = "";
    	NFAtab[3][0] = "4";
    	NFAtab[3][1] = "";
    	NFAtab[4][0] = "";
    	NFAtab[4][1] = "";	*/
 
    
}
 

 
/*Get next-state string for current-state string.*/
void get_next_state(char *nextstates, char *cur_states,char *nfa[STATES][SYMBOLS], int n_nfa, int symbol)
{
    	int i;			//cur_states is statename from nfa_to_dfa()
    	char temp[STATES];
 	temp[0] = '\0';
    	for (i = 0; i < strlen(cur_states); i++)		//For each character in current state, see where all they go to.Eg: First 0 transitions, then 2 transitions etc if string was "02"
        	string_merge(temp, nfa[cur_states[i]-'0'][symbol]);	//Merge result of each character into same temp
    	strcpy(nextstates, temp);
}
 
 
int state_index(char *state, char statename[][STATES], int *pn,char dfa_res[][SYMBOLS])
{
    	int i;		//pn is current number of states dfa
 	/*if (!*state) 
		return  ;*/ /* no next state */
 	for (i = 0; i < *pn; i++)
        	if (!strcmp(state, statename[i]))		//New State already present at index i of statename, so will not increase number of dfa states. Just return the index
			return i;
 	
    	strcpy(statename[i], state);
	
	//Put new state in statename, copy it to dfa_res, increase number of dfa states by 1 and return
	strcpy(dfa_res[i],state) ;
	
    	return (*pn)++;
}
 
/*
    Convert NFA table to DFA table.
    Return value: number of DFA states.
*/
int nfa_to_dfa(char *nfa[STATES][SYMBOLS], int n_nfa, int n_sym, int dfa[][SYMBOLS], char dfa_res[][SYMBOLS])
{
    	char statename[STATES][STATES];
    	int i = 0;  /* current index of DFA */
    	int n = 1;  /* number of DFA states */
 	char nextstate[STATES];
	char nextstate_E[STATES];
    	int j;
	char *start_state;
	start_state=NFAtab[0][n_sym];	//E-closure of 0
 	strcpy(statename[0], start_state);  /* start state */ //Have to change this for E-Closure
	strcpy(dfa_res[0],statename[0]);
	
 	for (i = 0; i < n; i++) 	/* for each DFA state , starts at 1, will increase if new states get added from below code*/
	{    
        	for (j = 0; j < n_sym; j++) /* for each input symbol */
		{    				
            		get_next_state(nextstate, statename[i], nfa, n_nfa, j);
			//E-

			get_next_state(nextstate_E,nextstate, nfa, n_nfa ,n_sym);// 
           		dfa[i][j]= state_index(nextstate_E, statename, &n,dfa_res);
			
   		}
		
    	}
    	return n;
}
 
int main()
{
	char *NFAtab[STATES][SYMBOLS];	//Stores the NFA Transition Table. Each element is a pointer to a string. Eg: NFAtab[0][1] is a pointer pointing to a string.
	char DFAres[STATES][SYMBOLS];	//Stores the states in string form Eg: A is "012", B is "23" etc
	int DFAtab[STATES][SYMBOLS];	//Stores the index of the DFA states. Eg: A on input 0 goes to B, A on input 1 goes to C etc, This stores integers: 0 corresponds to A. 1 corresponds to B etc
 	init_NFA_table(NFAtab);
    	DFA_states = nfa_to_dfa(NFAtab, NFA_states, N_symbols, DFAtab,DFAres);
    	print_dfa_table(DFAtab,DFAres, DFA_states, N_symbols);
}


/* To do E-Closure well have to get string from nextstate , find its E-Closure using functions similiar to string_merge and get_next_state, then put it in statename */

/* INput format (FOr the question we did in our NB, 5 states q0 to q4 and 2 symbols :
 5 2
01
03
phi
2
phi
phi
4
phi
phi
phi
*/

/* AFter typing each line, press enter key, not space */


/* For E-NFA to DFA , as we get each string in nextstate, before putting in statename we'll have to find E-CLosure of it then put that in statename
   To do E-Closure we will have to do something similiar to string_merge and get_next_state */

