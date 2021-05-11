#include <stdio.h> 
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
//Zepu Wang

#define MAX_LINE 80 /* The maximum length command */
#define TRUE 1
#define FALSE 0
void process (char **args,char *cmd,int i);
int split_str (char **args, char *cmd);
int check_char(char **args, int ar, char *c);
int cd(char *path);
int char_index (char *str,int length, char c);
void check_split (char *args[]);
char *hist_cmd[MAX_LINE];

int main(void) {
    char *args[MAX_LINE/2 + 1]; /* command line arguments */ 
    int should_run = 1; /* flag to determine when to exit program */
    char cmd[MAX_LINE];
    int hist_i=0;


    while (should_run){ 
        int i=0;
        printf("mysh:~$ "); 
        fflush(stdout);
        fgets(cmd,MAX_LINE,stdin);
        cmd[strlen (cmd)-1]='\0';
        if (cmd[0]!='\0' ){
        if (strcmp (cmd,"exit")==0){
            //exit condition
            should_run=0;
            
            
        }
        
        else if (strcmp(cmd,"!!")==0){ //check the history command
            if (args[0]==NULL) { 
                printf("No commands in history");}
            else {
                i=hist_i;
            }

        }
        else{       //if the outtercommands are normal,start to split the input
            i=split_str(args,cmd);
            //check_split(args[i]);
            hist_i=i;
            
            
            
        
        }

        if (should_run)
        //if the condition right, start to execute the order.
            process(args,cmd,i);
        
        }




        
        /** After reading user input, the steps are:  
         * (1) fork a child process using fork()  
         * (2) the child process will invoke execvp()  
         * (3) parent will invoke wait() unless command included & 
         */
    }

return 0;
}
//command that split the input
int split_str (char **args, char *cmd){
   int i=0;
   char *string=strdup(cmd);
   char *split = strtok(string, " ");
   while (split !=NULL){
       args[i]=split;
       i++;
       split = strtok(NULL, " ");
   }
   args[i]=NULL;
   return i;
}

void process (char **args,char *cmd,int i){// function that start to execute the input after splitting the string
    if (strcmp(args[0],"cd")==0){ // before creating the child process, we test the cd function

       int i= cd(args[1]);
        if (i <0){           //if the input dir not exists;
        perror(args[1]);
        }

    }

else{  //if not the cd function, we begin to create child process regarding different commands.

pid_t pid =fork();
    if(pid<0 ){
        perror("FAILD");

       } 
    else if (pid ==0){
        if (check_char(args,i,"&")==TRUE){//when checking the & command
            for (int j =0; j<=i-1;j++){
                
                if (strcmp(args[j],"&")==0){
                    //printf("1");
                    //fflush(stdin);
                    args[j]=NULL;
                    execvp(args[0],args);
                    args[i-1]=="&";

                }
                else{
                    //if (strcmp(args[j-1],"top")==0){do the top & command; }
                }
                
            }

        }
        else if(check_char(args, i,">")==TRUE){//checking input command
            
            for (int j =0; j<=i-1;j++){

            if(strcmp(args[j],">")==0)
            {
                 //printf("2");
                //fflush(stdin);
            FILE *out =fopen(args[j+1], "w");
            args[j]=NULL;
            dup2(fileno(out),STDOUT_FILENO);
            execvp(args[0],args);
          
            args[j]=">";
            fclose(out);
            }
        }
        }

        else if (check_char(args, i ,"<")==TRUE){ //checking output command
            for (int j =0; j<=i-1;j++){
                if(strcmp(args[j],"<")==0){
                    //printf("3");
                    //fflush(stdin);
                    FILE *out =fopen(args[j+1],"r");
                    args[j]=NULL;
                    dup2 (fileno(out),STDIN_FILENO);
                    execvp(args[0],args);
                    args[j]="<";
                    fclose(out);
                }
            }

        }
        else if (check_char(args, i ,"|")==TRUE){ //checking pipe command
            for (int j =0; j<=i-1;j++){
                int pfd [2];
            pipe (pfd);
                if (pipe(pfd)==-1){
                    perror("ERROR");
                }
            char *command1[1+j];
            char *command2[i-j];

            for (int a =0;a < j; a++){ //first content
                command1[a]=args[a];

            }
            command1[j]=NULL;

             for (int m=0;m<i-1-j;m++){// second content in pipe, also make on space larger because of |
                 command2[m]=args[m+1+j];
             }
            command2[i-j-1]=NULL;

               int p_pipe = fork();


            if (p_pipe>0){
            wait(NULL);
            
            close(pfd[1]);

            dup2(pfd[0],STDIN_FILENO);

            execvp(command2[0],command2);
            
            close(pfd[0]);
            }
            else if(p_pipe == 0){
                close(pfd[0]);

                dup2(pfd[1],STDOUT_FILENO);

                execvp(command1[0],command1);
                close(pfd[1]);
            }
            else if (p_pipe<0){
                perror("p_pipe is smaller than 1");
            }

            close(pfd[0]);
            close(pfd[1]);

                
            }

        }

       
        
        
        
        else execvp(args[0],args); //if commands without special characters, do the execvp normally
        
            
    }
    else {     //if commands do not contains &, we call wait(NULL);
        if(strcmp(args[i-1],"&")!=0) {
            while (wait(NULL) != pid);      
        }


    }
}
}
int check_char(char **args, int ar, char *c){  //function to check if the input has specific char
    for(int i = 0; i < ar; i++) {
        if(strcmp(args[i],c)==0) {
            return TRUE;
        }
    }
    return FALSE;


    
}

int cd(char *path) {          //the cd function
     return chdir(path); 
     }
//test 1

//functions used while debuging
int char_index (char *str,int length, char c){
    for (int i =0; i<length;i++ ){
        if(*(str +i)==c)
        return i;
    }

}

void check_split (char *args[]){ //checking if the split strings are correct.
    int i=0;
    
    while (args[i] != NULL){
        printf ("%s", args[i]);
        if (strcmp )
        i++;
        printf("\n");
    }
}





    


