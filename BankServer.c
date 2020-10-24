#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include<fcntl.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<string.h>
#include<time.h>
#define MAX 1000

struct admin
{
  char username[MAX];
  char password [MAX];
//  char type='A';
};
struct customer
{
  char accountid[MAX];
  char username [MAX];
  char name [MAX];
  char password [MAX];
  double balance;
  char t;

};
struct jointcustomer
{
  char accountid[MAX];
  char username [MAX];
  char name [MAX];
  char password [MAX];
  double balance;
  char t;

};



////////////////////////////////////////////////////////Admin Functions///////////////////////////////////////////////////////////////////////////////////////////
int createNewUser(char *cust_id, char *buf)
{
  strcat(cust_id,".txt");
  strcat(buf,"\n");
//  strcat(buf,"100\n");
  int fd=open(cust_id,O_CREAT|O_EXCL|O_RDWR,0666);
  int fd1=open("login_file.txt",O_RDWR|O_APPEND);

  if(fd<0)
    {
      perror("User exist\n");
      return 0;
    }

    char timebuff[21];
    time_t now = time(NULL);
    strftime(timebuff, 21, "%Y-%m-%d-%H:%M:%S ", localtime(&now));
    printf("time buff%s\n",timebuff);
    char update[MAX];
    bzero(update,MAX);
    strcat(update,timebuff);
    strcat(update,"100\n");
    write(fd,update,strlen(update));
    close(fd);
    bzero(update,MAX);
 //cust file Updated
 //login file update->
    write(fd1,buf,strlen(buf));
    close(fd1);
    bzero(buf,MAX);
    return 1;
}

int checkValidUser(char *cust_id)
{
  // checking for validity of user_id
  char line[1024];
  int check = 0;
  int fp = open("login_file.txt",O_RDONLY,0777);
  if(fp <0)
      perror("Error in opening login_file.");

    printf("Inside function\n" );


  int i=0;
  int l = 1;
  while (read(fp,&line[i],1)==1){

      if(line[i]=='\n' || line[i]==0x0 ){
          line[i] = 0;
          char *token = strtok(line," ");
      //    printf("%s\n",token);
      //    printf("1\n");
          if(strcmp(token,cust_id)==0){
              token=strtok(NULL," ");
              check=1;
          //    printf("%s\n",token);
          //    printf("2\n");

            }
          i=0;
          l++;
          continue;
      }
      i++;
  }
close(fp);
return check;
}

int is_valid(char *amount)
{
	// checking validity of amount
	int i;
	int count=0;
	for(i=0;amount[i];i++)
	{
		if(amount[i]=='.')
		{
			count++;
			if(count>1)
				return 0;
		}
		else if(amount[i]<='9'&&amount[i]>='0')
			continue;
		else
			return 0;
	}
	return 1;
}

double available_balance(char *cust_id)
{
  char filename[100];
  strcat(cust_id,".txt");
  int fp =open(cust_id,O_RDONLY);
  if(fp <0)
      {
        perror("Error in opening user file for balance.");

      }
      int i=0;
      int l = 1;

  char line[1024];
  char *token;
  while (read(fp,&line[i],1)==1){

      if(line[i]=='\n' || line[i]==0x0 ){
          line[i] = 0;
          token = strtok(line," ");
          token = strtok(NULL," ");

          i=0;
          l++;
          continue;
      }
      i++;

  }
  double amount=atoi(token);

  close(fp);
  return amount;
}

void mini_statement(int sock,char *cust_id)
{
  strcat(cust_id,".txt");
  int fp =open(cust_id,O_RDONLY);
  char readbuf[MAX];
  int total=read(fp,readbuf,200);
  int pr=write(sock,readbuf,strlen(readbuf));
  close(fp);
}

int verify_credentials(int sockfd, char *user_type,int *cust_id)
{
    int n;
    char buffer[MAX];
    char *user, *pass;

    // reading username and password
    bzero(buffer,MAX);
    n = read(sockfd,buffer,MAX-1);
    if (n < 0)
        perror("ERROR reading from socket");

    // breaking it
    user = strtok(buffer, "$$$");
    pass = strtok(NULL, "$$$");

    user[strlen(user)-1] = '\0';
    pass[strlen(pass)-1] = '\0';

    if(strlen(user)==0 || strlen(pass)==0)
        return 0;

    // checking for validity
   FILE *fp = fopen("login_file.txt","r");
    if(fp == NULL)
        perror("Error in opening login_file.");

    char *cred = NULL;
    size_t len = 0;

    while(getline(&cred,&len,fp)!=-1)
    {
        char *username = strtok(cred," ");
        char *password = strtok(NULL," ");
        char *usertype = strtok(NULL, " ");
        if(!strcmp(username,user)&&!strcmp(password,pass))
        {
            *user_type = usertype[0];
            *cust_id = atoi(username);
            free(cred);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    free(cred);
    return 0;
}

int credit_amount(char *id, char *amount,char *trans)
{
  double curr_amount=available_balance(id);
  char filename[50];
   strcpy(filename,id);
  // strcat(filename,".txt");
   printf("%s\n",filename );
 int fp = open(filename,O_RDWR|O_APPEND);
 if(fp <0)
     {
       perror("Error in opening user file for balance.");
       return 0;
     }

 double new_amount=atoi(amount);
 double update_amount = curr_amount+new_amount;

 char timebuff[21];
 time_t now = time(NULL);
 strftime(timebuff, 21, "%Y-%m-%d-%H:%M:%S ", localtime(&now));

 char buf[100];

  sprintf(buf,"%f",update_amount);
  printf("%s amount modified\n",buf );
  char transaction[50];
  bzero(transaction,50);
   strcpy(transaction,timebuff);
   strcat(transaction,buf);
   strcat(transaction,"\n");
   printf("tran %s\n",transaction);

write(fp,transaction,strlen(transaction));
close(fp);
return 1;
}


int debit_amount(char *id, char *amount, char *trans)
{
  double curr_amount=available_balance(id);
  char filename[50];
   strcpy(filename,id);
  // strcat(filename,".txt");

 int fp = open(filename,O_RDWR|O_APPEND);
 if(fp <0)
     {
       perror("Error in opening user file for balance.");
       return 0;
     }

 double new_amount=atoi(amount);
 double update_amount = curr_amount-new_amount;
 if(update_amount<0)
 return 0;

 char timebuff[21];
 time_t now = time(NULL);
 strftime(timebuff, 21, "%Y-%m-%d-%H:%M:%S ", localtime(&now));

 char buf[100];

  sprintf(buf,"%f",update_amount);
  printf("%s amount modified\n",buf );
  char transaction[50];
  bzero(transaction,50);
   strcpy(transaction,timebuff);
   strcat(transaction,buf);
   strcat(transaction,"\n");
   printf("tran %s\n",transaction);

write(fp,transaction,strlen(transaction));
close(fp);
return 1;
}

int changePassword(char* cust_id,char *pass, char *newpass)
{
  int fp = open("login_file.txt",O_RDWR);
  if(fp <0)
      perror("Error in opening user file for balance.");
      char line[1024];
      int check = 0;
      int i=0;
      int l = 1;
      int count=0;
      char *token;

      while (read(fp,&line[i],1)==1){
          count++;
          if(line[i]=='\n' || line[i]==0x0 ){
              line[i] = 0;
              token = strtok(line," ");
          //    printf("%s\n",token);
          //    printf("1\n");
          printf("%d\n",count );
              if(strcmp(token,cust_id)==0){
                {
                    count= count-(strlen(pass)+3);/// 8 is for 5 word password
                    lseek(fp,count,SEEK_SET); //positions at first char at beginnging of file.
                    write(fp,newpass,strlen(newpass));
                    break;
                  }

                }
              i=0;
              l++;
              continue;
          }
          i++;
      }

  close(fp);
  return 1;
}

void admin(int sockfd)
{
    int n,m;
    char buffer[MAX];
    char id[MAX], trans[MAX], amount[MAX], ip[MAX], pass[MAX],type[MAX];

    //////* Reading flag
    bzero(buffer,MAX);
    n = read(sockfd,buffer,MAX-1);
    if (n < 0)
        perror("ERROR reading from socket");

    while(buffer[0]=='y')
    {

        printf("%s\n",buffer );
        bzero(ip,MAX);
        n = read(sockfd,ip,sizeof(ip));///read choice 1 - 4
        if (m < 0)
          perror("ERROR reading from socket");

        printf("%s  ip = \n",ip );

          switch (ip[0]) {
            case '1': {//////////Add Account

                      bzero(id, MAX);
                      bzero(pass, MAX);
                      bzero(type, MAX);

                      read(sockfd,id,sizeof(id));//read id
                      printf("%s\n",id );
                      read(sockfd,pass,sizeof(pass));//read pass
                      printf("%s\n",pass );
                      read(sockfd,type,sizeof(type));//read type

                      bzero(buffer,MAX);
                      strcat(buffer,id);
                      strcat(buffer," ");
              	      strcat(buffer,pass);
                      strcat(buffer," ");
                      strcat(buffer,type);

                      if(checkValidUser(id)==0 && createNewUser(id,buffer)==1)// sending command
                      {
                        n = write(sockfd,"successfully created new user\n",strlen("Successfully created new user\n"));
                        if (n < 0)
                           perror("ERROR writing to socket");
                      }
                     else
                     {
                       n = write(sockfd,"New user cannot be created\n",strlen("New user cannot be created\n"));
                       if (n < 0)
                          perror("ERROR writing to socket");
                     }

                     break;
                   }

            case '2':{/////////////Transaction


                     bzero(amount, MAX);
                     bzero(id, MAX);
                     bzero(trans, MAX);

                     read(sockfd,id,sizeof(id));//read id
                     printf("%s\n",id );
                     read(sockfd,trans,sizeof(trans));//read trans
                     printf("%s\n",trans );
                     read(sockfd,amount,sizeof(amount));//read amount

                     printf("%s\n",amount );
                     int check=checkValidUser(id);

                     if(check==0 || (strcmp(trans,"credit") && strcmp(trans,"debit")) || !is_valid(amount))
                         {
                             printf("Request from client with ip  declined. \n");
                             bzero(buffer,MAX);
                             strcpy(buffer,"false");
                             n = write(sockfd,buffer,strlen(buffer));
                             if (n < 0)
                                 perror("ERROR writing to socket");
                         }
                         else
                         {
                         	if(!strcmp(trans,"credit"))
                             {
                             	int x =credit_amount(id,amount,trans);
                              if(x==1)
                                 {
                                   printf("Credit request from customer  successfully executed. \n");
                             	     bzero(buffer,MAX);
                                   strcpy(buffer,"true");// sending true
                                   n = write(sockfd,buffer,strlen(buffer));
                                   if (n < 0)
                                     perror("ERROR writing to socket");
                                   }
                                   else
                                   {
                                     bzero(buffer,MAX);
                                     strcpy(buffer,"false");
                                     n = write(sockfd,buffer,strlen(buffer));// send false
                                     if (n < 0)
                                         perror("ERROR writing to socket");
                                   }
                             }
                             else if(!strcmp(trans,"debit"))
                             {
                             	int f = debit_amount(id, amount, trans);
                                 if(f==1)
                                 {
                                 	 printf("Debit request for customer  successfully executed. \n");
                                   bzero(buffer,MAX);
                 	                strcpy(buffer,"true");
                 	                n = write(sockfd,buffer,strlen(buffer));// sending true
                 	                if (n < 0)
                 	                    perror("ERROR writing to socket");
                                 }
                                 else
                                 {
                                 	printf("Debit request from client declined. \n");
                                  bzero(buffer,MAX);
                 	                strcpy(buffer,"false");
                 	                n = write(sockfd,buffer,strlen(buffer));//sending false
                 	                if (n < 0)
                 	                    perror("ERROR writing to socket");
                                 }
                             }
                           }
                     break;
                   }
            case '3':{///////////////////////Password update

                        bzero(pass, MAX);
                        read(sockfd,pass,sizeof(pass));//read pass
                        bzero(trans,MAX);
                        read(sockfd,trans,sizeof(trans));//read pass

                        int x =changePassword(id,pass,trans);
                        bzero(buffer,MAX);
                        if(x==0)
                        buffer="false";
                        else
                        buffer="true";
                        n = write(sockfd,buffer,strlen(buffer));
                        if (n < 0)
                          perror("ERROR writing to socket");


                        break;
                   }

            case '4':{////////////////Delete Account
                     bzero(id,MAX);
                     printf("Enter username of the customer whose account has to be deleted\n");
                     scanf("%[^\n]%*c",id);


                       bzero(buffer,MAX);
                       strcat(buffer,id);


                       // sending command
                       n = write(sockfd,buffer,strlen(buffer));
                       if (n < 0)
                           perror("ERROR writing to socket");

                       // true or false
                       bzero(buffer,MAX);
                       n = read(sockfd,buffer,MAX-1);
                       if (n < 0)
                           perror("ERROR reading from socket");

                       if(!strcmp(buffer,"false"))
                       {
                           printf("Account could not be deleted the amount is not 0 \n");
                       }
                       else
                       {
                           printf("Deletion successful.\n\n");
                       }
                    /*   else
                       {
                         printf("Deletion Cancelled  \n");
                       }


                  //   }
*/

                     break;
                   }
            default:{
                     printf("You entered wrong Input ----try again\n");
                     break;
                    }
                  }

        //// Reading flag
        bzero(buffer,MAX);
        n = read(sockfd,buffer,MAX-1);
        if (n < 0)
            perror("ERROR reading from socket");


}
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////Customer Functions //////////////////////////////////////////////////////////////


void customer(int sockfd,char *cust_id)
{
  int n,m;
  char buffer[MAX];
  char id[MAX], trans[MAX], amount[MAX], ip[MAX], pass[MAX],type[MAX];
  read(sockfd,id,sizeof(id));//read id of customer

  //////* Reading flag
  bzero(buffer,MAX);
  n = read(sockfd,buffer,MAX-1);
//  printf("%s %c\n",buffer,buffer[0] );
  if (n < 0)
      perror("ERROR reading from socket");

  while(buffer[0]=='y')
  {

      printf("%s %c\n",buffer,buffer[0] );
      bzero(ip,MAX);
      n = read(sockfd,ip,sizeof(ip));///read choice 1 - 4
      if (m < 0)
        perror("ERROR reading from socket");

     printf(" ip = %s hghghg\n",ip );

        switch (ip[0]) {
          case '1':{//////////Check Balance

                    if(checkValidUser(id)==1 )// sending command
                    {
                      double amt = available_balance(id);
                      bzero(buffer,MAX);
                      sprintf(buffer,"%f",amt);
                      n = write(sockfd,buffer,strlen(buffer));
                      if (n < 0)
                         perror("ERROR writing to socket");
                    }
                   else
                   {
                     n = write(sockfd,"Try Again\n",strlen("Try Again\n"));
                     if (n < 0)
                        perror("ERROR writing to socket");
                   }

                   break;
                 }
          case '2':{/////////////Transaction


                   bzero(amount, MAX);

                   bzero(trans, MAX);

                /*   read(sockfd,id,sizeof(id));//read id
                   printf("%s\n",id );*/
                   read(sockfd,trans,sizeof(trans));//read trans
                   printf("%s\n",trans );
                   read(sockfd,amount,sizeof(amount));//read amount

                   printf("%s\n",amount );
                   int check=checkValidUser(id);

                   if(check==0 || (strcmp(trans,"credit") && strcmp(trans,"debit")) || !is_valid(amount))
                       {
                           printf("Request from client with ip  declined. \n");
                           bzero(buffer,MAX);
                           strcpy(buffer,"false");
                           n = write(sockfd,buffer,strlen(buffer));
                           if (n < 0)
                               perror("ERROR writing to socket");
                       }
                       else
                       {
                        if(!strcmp(trans,"credit"))
                           {
                            int x =credit_amount(id,amount,trans);
                            if(x==1)
                               {
                                 printf("Credit request from customer  successfully executed. \n");
                                 bzero(buffer,MAX);
                                 strcpy(buffer,"true");// sending true
                                 n = write(sockfd,buffer,strlen(buffer));
                                 if (n < 0)
                                   perror("ERROR writing to socket");
                                 }
                                 else
                                 {
                                   bzero(buffer,MAX);
                                   strcpy(buffer,"false");
                                   n = write(sockfd,buffer,strlen(buffer));// send false
                                   if (n < 0)
                                       perror("ERROR writing to socket");
                                 }
                           }
                           else if(!strcmp(trans,"debit"))
                           {
                            int f = debit_amount(id, amount, trans);
                               if(f==1)
                               {
                                 printf("Debit request for customer  successfully executed. \n");
                                 bzero(buffer,MAX);
                                strcpy(buffer,"true");
                                n = write(sockfd,buffer,strlen(buffer));// sending true
                                if (n < 0)
                                    perror("ERROR writing to socket");
                               }
                               else
                               {
                                printf("Debit request from client declined. \n");
                                bzero(buffer,MAX);
                                strcpy(buffer,"false");
                                n = write(sockfd,buffer,strlen(buffer));//sending false
                                if (n < 0)
                                    perror("ERROR writing to socket");
                               }
                           }
                         }
                   break;
                 }
          case '3':{///////////////////////Password update

                    bzero(pass, MAX);
                    read(sockfd,pass,sizeof(pass));//read pass
                    bzero(trans,MAX);
                    read(sockfd,trans,sizeof(trans));//read pass

                    int x =changePassword(id,pass,trans);
                  bzero(buffer,MAX);
                  if(x==0)
                  buffer="false";
                  else
                  buffer="true";
                   n = write(sockfd,buffer,strlen(buffer));
                   if (n < 0)
                       perror("ERROR writing to socket");


                   break;
                 }
          case '4':{////////////////Mini Statement

                   mini_statement(sockfd,id);

                   break;
                 }
          default:{
                   printf("You entered wrong Input ----try again\n");
                   break;
                  }
                }

      //// Reading flag
      bzero(buffer,MAX);
      n = read(sockfd,buffer,MAX-1);
      if (n < 0)
          perror("ERROR reading from socket");

}
}

void jointCustomer(int sockfd,char *cust_id)
{
  int n,m;
  char buffer[MAX];
  char id[MAX], trans[MAX], amount[MAX], ip[MAX], pass[MAX],type[MAX];

  //////* Reading flag
  bzero(buffer,MAX);
  n = read(sockfd,buffer,MAX-1);
  if (n < 0)
      perror("ERROR reading from socket");

  while(buffer[0]=='y')
  {

      printf("%s\n",buffer );
      bzero(ip,MAX);
      n = read(sockfd,ip,sizeof(ip));///read choice 1 - 4
      if (m < 0)
        perror("ERROR reading from socket");

      printf("%s  ip = \n",ip );

        switch (ip[0]) {
          case '1':{//////////Check Balance

                    bzero(id, MAX);
                    bzero(pass, MAX);
                    bzero(type, MAX);

                    read(sockfd,id,sizeof(id));//read id
                    printf("%s\n",id );


                    if(checkValidUser(id)==1 )// sending command
                    {
                      double amt = available_balance(id);
                      bzero(buffer,MAX);
                      sprintf(buffer,"%f",amt);
                      n = write(sockfd,buffer,strlen(buffer));
                      if (n < 0)
                         perror("ERROR writing to socket");
                    }
                   else
                   {
                     n = write(sockfd,"Try Again\n",strlen("Try Again\n"));
                     if (n < 0)
                        perror("ERROR writing to socket");
                   }

                   break;
                 }
          case '2':{/////////////Transaction


                   bzero(amount, MAX);
                   bzero(id, MAX);
                   bzero(trans, MAX);

                   read(sockfd,id,sizeof(id));//read id
                   printf("%s\n",id );
                   read(sockfd,trans,sizeof(trans));//read trans
                   printf("%s\n",trans );
                   read(sockfd,amount,sizeof(amount));//read amount

                   printf("%s\n",amount );
                   int check=checkValidUser(id);

                   if(check==0 || (strcmp(trans,"credit") && strcmp(trans,"debit")) || !is_valid(amount))
                       {
                           printf("Request from client with ip  declined. \n");
                           bzero(buffer,MAX);
                           strcpy(buffer,"false");
                           n = write(sockfd,buffer,strlen(buffer));
                           if (n < 0)
                               perror("ERROR writing to socket");
                       }
                       else
                       {
                        if(!strcmp(trans,"credit"))
                           {
                            int x =credit_amount(id,amount,trans);
                            if(x==1)
                               {
                                 printf("Credit request from customer  successfully executed. \n");
                                 bzero(buffer,MAX);
                                 strcpy(buffer,"true");// sending true
                                 n = write(sockfd,buffer,strlen(buffer));
                                 if (n < 0)
                                   perror("ERROR writing to socket");
                                 }
                                 else
                                 {
                                   bzero(buffer,MAX);
                                   strcpy(buffer,"false");
                                   n = write(sockfd,buffer,strlen(buffer));// send false
                                   if (n < 0)
                                       perror("ERROR writing to socket");
                                 }
                           }
                           else if(!strcmp(trans,"debit"))
                           {
                            int f = debit_amount(id, amount, trans);
                               if(f==1)
                               {
                                 printf("Debit request for customer  successfully executed. \n");
                                 bzero(buffer,MAX);
                                strcpy(buffer,"true");
                                n = write(sockfd,buffer,strlen(buffer));// sending true
                                if (n < 0)
                                    perror("ERROR writing to socket");
                               }
                               else
                               {
                                printf("Debit request from client declined. \n");
                                bzero(buffer,MAX);
                                strcpy(buffer,"false");
                                n = write(sockfd,buffer,strlen(buffer));//sending false
                                if (n < 0)
                                    perror("ERROR writing to socket");
                               }
                           }
                         }
                   break;
                 }
          case '3':{///////////////////////Password update

                    bzero(pass, MAX);
                    read(sockfd,pass,sizeof(pass));//read pass
                    bzero(trans,MAX);
                    read(sockfd,trans,sizeof(trans));//read pass

                    int x =changePassword(id,pass,trans);
                    bzero(buffer,MAX);
                    if(x==0)
                    buffer="false";
                    else
                    buffer="true";
                    n = write(sockfd,buffer,strlen(buffer));
                    if (n < 0)
                    perror("ERROR writing to socket");


                    break;
                 }
          case '4':{////////////////Mini Statement
                   bzero(id,MAX);
                   read(sockfd,id,sizeof(id));//read id
                   printf("%s\n",id );
                   mini_statement(sockfd,id);

                   break;
                 }
          default:{
                   printf("You entered wrong Input ----try again\n");
                   break;
                  }
                }

      //// Reading flag
      bzero(buffer,MAX);
      n = read(sockfd,buffer,MAX-1);
      if (n < 0)
          perror("ERROR reading from socket");

}
}
////////////////////////////////////////////////////////////////Cust function ends/////////////////////////////////////////////////////////////////////////////

int main()
{
    int server_fd;
    struct sockaddr_in server;
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Bank server cannot be connected");
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    int addrlen = sizeof(server);
    char buffer[1024] = {0};
    char *welcomemessage = "Welcome to the Banking System\n";

    // attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,  &opt, sizeof(opt))<0)
    {
        perror("Can't be connected");
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 4567 );

    // Binding socket to the port
    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server))<0)
    {
        perror("Can't be connected - bind failed");
        exit(EXIT_FAILURE);
    }
    // wait for client
    if (listen(server_fd, 10) < 0)
    {
        perror("Wait for your turn / connect again after some time");
        exit(EXIT_FAILURE);
    }
    int new_socket;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&server, (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
  //  bzero(welcomemessage, MAX);
    char buff[MAX],usr[MAX],pass[MAX];
    int n;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  for (;;) {
     printf("\n" );
     write(new_socket , welcomemessage , strlen(welcomemessage) );////welcome to banking system
     bzero(buff, MAX);
     strcpy(buff,"Enter username");
     write(new_socket,buff,sizeof(buff));///enter username
     bzero(buff, MAX);
     read(new_socket, buff, sizeof(buff));// read username
     strcpy(usr,buff);

     bzero(buff, MAX);
     strcpy(buff,"Enter password");
     write(new_socket,buff,sizeof(buff));//enter password
     bzero(buff, MAX);
     read(new_socket, buff, sizeof(buff));// read password
     strcpy(pass,buff);


     if(strcmp(usr,"admin")==0)// if admin
     {
       admin(new_socket);
     }
     else if(strcmp(usr,"yash")==0)
     {
       customer(new_socket,usr);

     }
     else
     {
       jointCustomer(new_socket,usr);
     }

    return 0;
}
