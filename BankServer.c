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
#define MAX 1024

int deleteAccount(char *cust_id,char *pass,char t)
{
  int i=0;
  int l = 1;
  int count=0;
  int flag=0;
  char *token;
  char line[1024];

   int fp = open("login_file.txt",O_RDWR);
    if(fp <0)
        perror("Error");
    while (read(fp,&line[i],1)==1){
            count++;
            if(line[i]=='\n' || line[i]==0x0 ){
                line[i] = 0;
                token = strtok(line," ");
                if(strcmp(token,cust_id)==0)
                  {
                      count= count-(strlen(pass)+strlen(cust_id)+3);/// 8 is for 5 word password
                      lseek(fp,count,SEEK_SET); //positions at first char at beginnging of file.
                      write(fp,"#",strlen("#"));
                      flag=1;
                      break;
                    }
                i=0;
                l++;
                continue;
            }
            i++;
        }


  close(fp);
  if(flag==1)
  {
    strcat(cust_id,".txt");
    if (remove(cust_id) == 0)
      {
        printf("Deleted successfully");
        return 1;
      }
    else
      printf("Unable to delete the file");


  }
  return 0;
}

int deleteAccountJ(char *cust_id,char *cust_id1,char *cust_id2,char *pass,char t)
{
  int i=0;
  int l = 1;
  int count=0;
  int flag=0;
  char *token;
  char line[1024];
  int check=0;

  int fp = open("login_fileJ.txt",O_RDWR);
   if(fp <0)
       perror("Error");
   while (read(fp,&line[i],1)==1){
           count++;
           if(line[i]=='\n' || line[i]==0x0 ){
               line[i] = 0;
               token = strtok(line," ");
               if(strcmp(token,cust_id)==0)
                 {
                     check++;
                     count= count-(strlen(pass)+strlen(cust_id)+strlen(cust_id1)+3);/// 8 is for 5 word password
                     lseek(fp,count,SEEK_SET); //positions at first char at beginnging of file.
                     write(fp,"#",strlen("#"));
                     if(check==2)
                     break;
                   }


               i=0;
               l++;
               continue;
           }
           i++;
       }


  close(fp);
  if(flag==1)
  {
    strcat(cust_id,".txt");
    if (remove(cust_id) == 0)
      {
        printf("Deleted successfully");
        return 1;
      }
    else
      printf("Unable to delete the file");


  }
  return 0;
}

int createNewUser(char *cust_id, char *buf, char t)
{
  strcat(cust_id,".txt");
  strcat(buf,"\n");
  int fd, fd1;
//  strcat(buf,"100\n");
   fd=open(cust_id,O_CREAT|O_EXCL|O_RDWR,0666);
  if(t=='C')
   fd1=open("login_file.txt",O_RDWR|O_APPEND);
else
   fd1=open("login_fileJ.txt",O_RDWR|O_APPEND);
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

int checkValidUser(char *cust_id, char t)
{
  // checking for validity of user_id
  char line[1024];
  int check = 0, fp;
  printf("%s\n",cust_id );
  if(t=='C')
    fp = open("login_file.txt",O_RDONLY,0777);
  else
    fp = open("login_fileJ.txt",O_RDONLY,0777);
  if(fp <0)
      perror("Error in opening login_file.");

    printf("Inside function\n" );


  int i=0;
  int l = 1;
  while (read(fp,&line[i],1)==1){

      if(line[i]=='\n' || line[i]==0x0 ){
          line[i] = 0;
          char *token = strtok(line," ");

          if(strcmp(token,cust_id)==0){
              token=strtok(NULL," ");
              check=1;
            /*  close(fp);
              return check;*/
              break;
            }
          i=0;
          l++;
          continue;
      }
      i++;
  }
close(fp);
printf("%d",check);
return check;
}

int is_valid(char *amount)// checking validity of amount
{
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
////  strcat(cust_id,".txt");
struct flock fl;
fl.l_type   = F_RDLCK;
fl.l_whence = SEEK_SET;
fl.l_start  = 0;
fl.l_len    = 0;
fl.l_pid    = getpid();
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

  fl.l_type   = F_UNLCK;
  fcntl(fp, F_SETLK, &fl);
  close(fp);

  return amount;

}

void mini_statement(int sock,char *cust_id)
{
////  strcat(cust_id,".txt");
  int fp =open(cust_id,O_RDONLY);
  char readbuf[MAX];
  int total=read(fp,readbuf,1024);
  int pr=write(sock,readbuf,strlen(readbuf));
  printf("%s",readbuf);
  close(fp);
}

int verify_credentials(char *cust_id,char *pass)
{
  char line[1024];
  int check = 0, fp,fp1;
  int i=0;
  int l = 1;
  printf("%s %s\n",cust_id,pass );
  fp = open("login_file.txt",O_RDONLY,0777);
  while (read(fp,&line[i],1)==1){

      if(line[i]=='\n' || line[i]==0x0 ){
          line[i] = 0;
          char *token = strtok(line," ");
          if(strcmp(token,cust_id)==0){
              token=strtok(NULL," ");
              if(strcmp(token,pass)==0)
                {
                  check=2;
                  close(fp);
                  return check;
              }
            }
          i=0;
          l++;
          continue;
         }
      i++;
     }
close(fp);
if(check==0)
{
  i=0;
  l=1;
  fp1 = open("login_fileJ.txt",O_RDONLY,0777);
  while (read(fp1,&line[i],1)==1){

      if(line[i]=='\n' || line[i]==0x0 ){
          line[i] = 0;
          char *token = strtok(line," ");
          if(strcmp(token,cust_id)==0){
              token=strtok(NULL," ");
              token=strtok(NULL," ");
              if(!strcmp(token,pass))
                {check=3;
                 close(fp1);
                 return check;
              }
            }
          i=0;
          l++;
          continue;
      }
      i++;
  }
}




close(fp1);
return check;
}

int credit_amount(char *id, char *amount,char *trans)
{

  double curr_amount=available_balance(id);
  char filename[50];
   strcpy(filename,id);
  // strcat(filename,".txt");
   printf("%s\n",filename );

   struct flock fl;
   fl.l_type   = F_WRLCK;
   fl.l_whence = SEEK_SET;
   fl.l_start  = 0;
   fl.l_len    = 0;
   fl.l_pid    = getpid();
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

fl.l_type   = F_UNLCK;
fcntl(fp, F_SETLK, &fl);
close(fp);

return 1;
}

int debit_amount(char *id, char *amount, char *trans)
{
  double curr_amount=available_balance(id);
  char filename[50];
   strcpy(filename,id);
  // strcat(filename,".txt");

struct flock fl;
fl.l_type   = F_WRLCK;
fl.l_whence = SEEK_SET;
fl.l_start  = 0;
fl.l_len    = 0;
fl.l_pid    = getpid();

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

fl.l_type   = F_UNLCK;
fcntl(fp, F_SETLK, &fl);

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
    //      printf("%d\n",count );
              if(strcmp(token,cust_id)==0)
                {
                    count= count-(strlen(pass)+3);/// 8 is for 5 word password
                    lseek(fp,count,SEEK_SET); //positions at first char at beginnging of file.
                    write(fp,newpass,strlen(newpass));
                    break;
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

int changePasswordJ(char* cust_id,char *cust_id1,char *pass, char *newpass)
{
  int fp = open("login_fileJ.txt",O_RDWR);
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

      //    printf("%d\n",count );
              if(strcmp(token,cust_id)==0)
                 {
                  token =strtok(NULL," ");
                  if(strcmp(token,cust_id1));
                  {
              //      count= count-(strlen(pass)+strlen(cust_id1)+4);/// 8 is for 5 word password
                      count= count-(strlen(pass)+3);
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
    char buffer[MAX],cust_id1[MAX];
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

          switch (ip[0]) {
            case '1': {//////////Add Account

                      bzero(type, MAX);
                      bzero(id, MAX);
                      bzero(pass, MAX);

                      read(sockfd,type,sizeof(type));//read type
                      printf("%c\n",type[0]);

                      if(type[0]=='C')
                      {
                      read(sockfd,id,sizeof(id));//read id
                      printf("%s\n",id );
                      read(sockfd,pass,sizeof(pass));//read pass
                      printf("%s\n",pass );

                      bzero(buffer,MAX);
                      strcat(buffer,id);
                      strcat(buffer," ");
              	      strcat(buffer,pass);
                      strcat(buffer," ");
                      strcat(buffer,type);
                     }

                     if(type[0]=='J')
                     {

                     read(sockfd,id,sizeof(id));//read id
                     printf("%s\n",id );
                     read(sockfd,pass,sizeof(pass));//(read user1 pass1)
                     printf("%s\n",pass );

                     bzero(buffer,MAX);
                     strcat(buffer,id);
                     strcat(buffer," ");
                     strcat(buffer,pass);
                     strcat(buffer," ");
                     strcat(buffer,type);
                     strcat(buffer,"\n");

                     bzero(pass,MAX);
                     read(sockfd,pass,sizeof(pass));//(read user2 pass2)
                     printf("%s\n",pass );
                     strcat(buffer,id);
                     strcat(buffer," ");
                     strcat(buffer,pass);
                     strcat(buffer," ");
                     strcat(buffer,type);
                     strcat(buffer,"\n");
                    }
                      bzero(cust_id1,MAX);
                      strcpy(cust_id1,id);
                      strcat(cust_id1,".txt");
                      if(checkValidUser(cust_id1,type[0])==0 && createNewUser(id,buffer,type[0])==1)// sending command
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
                     read(sockfd,type,sizeof(type));//read amount
                     printf("%s\n",type );
                     bzero(cust_id1,MAX);
                     strcat(cust_id1,id);
                     strcat(id,".txt");

                     int check=checkValidUser(cust_id1,type[0]);

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
                             	  //   bzero(buffer,MAX);
                            //       strcpy(buffer,"true");// sending true
                              //     n = write(sockfd,buffer,strlen(buffer));
/////yash update
                                   bzero(buffer,MAX);
                                   double bal =available_balance(id);
                                   sprintf(buffer,"%.2f",bal);
                                   printf("%s\n",buffer );
                                   n = write(sockfd,buffer,strlen(buffer));
///
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
                                  /* bzero(buffer,MAX);
                 	                strcpy(buffer,"true");
                 	                n = write(sockfd,buffer,strlen(buffer));*/// sending true

                                  bzero(buffer,MAX);
                                  double bal =available_balance(id);
                                  sprintf(buffer,"%.2f",bal);
                                  printf("%s\n",buffer );
                                  n = write(sockfd,buffer,strlen(buffer));

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

                      bzero(type, MAX);
                      bzero(id, MAX);
                      bzero(pass, MAX);
                      bzero(cust_id1,MAX);
                      bzero(trans,MAX);//treating transas old password
                      int x;
                      read(sockfd,type,sizeof(type));//read type
                      printf("%c\n",type[0]);

                      if(type[0]=='C')
                      {
                        read(sockfd,id,sizeof(id));//read id
                        printf("%s\n",id );
                        read(sockfd,trans,sizeof(trans));//read old pass
                        printf("%s\n",trans );
                        read(sockfd,pass,sizeof(pass));//read pass
                        printf("%s\n",pass );


                        x =changePassword(id,trans,pass);
                      }

                      if(type[0]=='J')
                      {

                      read(sockfd,id,sizeof(id));//read id
                      printf("%s\n",id );
                      read(sockfd,cust_id1,sizeof(cust_id1));//cust_id1
                      printf("%s\n",cust_id1 );
                      read(sockfd,trans,sizeof(trans));//read old pass
                      printf("%s\n",trans );
                      read(sockfd,pass,sizeof(pass));//read pass
                      printf("%s\n",pass );

                      x =changePasswordJ(id,cust_id1,trans,pass);

                     }

                        bzero(buffer,MAX);
                        if(x==0)
                        strcpy(buffer,"false");
                        else
                        strcpy(buffer,"true");
                        n = write(sockfd,buffer,strlen(buffer));
                        if (n < 0)
                          perror("ERROR writing to socket");

                        break;
                   }
            case '4':{////////////////Delete Account
              bzero(type,MAX);
              read(sockfd,type,sizeof(type));
              if (type[0]=='Y')
              {
                      bzero(type, MAX);
                      bzero(id, MAX);
                      bzero(pass, MAX);
                      bzero(cust_id1,MAX);
                      bzero(trans,MAX);//treating trans as cust_id2 password
                      int x;
                      read(sockfd,type,sizeof(type));//read type
                      printf("%c\n",type[0]);

                      if(type[0]=='C')
                      {
                        read(sockfd,id,sizeof(id));//read id
                        printf("%s\n",id );
                        read(sockfd,pass,sizeof(pass));//read pass
                        printf("%s\n",pass );

                        bzero(cust_id1,MAX);
                        strcpy(cust_id1,id);
                        strcat(cust_id1,".txt");
                        printf("%s %s\n",cust_id1,id );
                        double bal = available_balance(cust_id1);

                        if(bal>0)
                        x=0;
                       else
                        {
                          x=deleteAccount(id,pass,type[0]);
                        }
                      }

                      if(type[0]=='J')
                      {

                        read(sockfd,id,sizeof(id));//read id
                        printf("%s\n",id );
                        read(sockfd,cust_id1,sizeof(cust_id1));//cust_id1
                        printf("%s\n",cust_id1 );
                        read(sockfd,trans,sizeof(trans));//read cust_id2
                        printf("%s\n",trans );
                        read(sockfd,pass,sizeof(pass));//read pass
                        printf("%s\n",pass );

                        bzero(amount,MAX);
                        strcpy(amount,id);
                        strcat(amount,".txt");///treaating amount as temp id to check the balance

                        double bal = available_balance(amount);
                        bzero(amount,MAX);
                        if(bal>0)
                        x=0;
                        else
                        {
                          x=deleteAccountJ(id,cust_id1,trans,pass,type[0]);
                        }

                      }

                      bzero(buffer,MAX);
                      if(x==0)
                      strcpy(buffer,"false");
                      else
                      strcpy(buffer,"true");
                      n = write(sockfd,buffer,strlen(buffer));
                      if (n < 0)
                      perror("ERROR writing to socket");
                    }

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

void customer(int sockfd,char *cust_id)
{
  int n,m;
  char buffer[MAX];
  char id[MAX], trans[MAX], amount[MAX], ip[MAX], pass[MAX],type[MAX];
  bzero(id,MAX);
  strcpy(id,cust_id);
//  read(sockfd,id,sizeof(id));//read id of customer

  //////* Reading flag
  bzero(buffer,MAX);
  n = read(sockfd,buffer,sizeof(buffer));
//  printf("%s %c\n",buffer,buffer[0] );
  if (n < 0)
      perror("ERROR reading from socket");

  strcat(cust_id,".txt");
  while(buffer[0]=='y')
  {


      printf("%s %c\n",buffer,buffer[0] );
      bzero(ip,MAX);
      n = read(sockfd,ip,sizeof(ip));///read choice 1 - 4
      if (m < 0)
        perror("ERROR reading from socket");

     printf(" ip selected by user = %s .\n",ip );

        switch (ip[0]) {
          case '1':{//////////Check Balance

                    if(checkValidUser(id,'C')==1 )// sending command
                    {
                      double amt = available_balance(cust_id);
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


                   read(sockfd,trans,sizeof(trans));//read trans
                   printf("%s\n",trans );
                   read(sockfd,amount,sizeof(amount));//read amount
                   printf("%s\n",amount );

                   int check=checkValidUser(id,'C');
                   printf("cust ID - %s , check %d\n",cust_id,check );
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
                            int x =credit_amount(cust_id,amount,trans);
                            if(x==1)
                               {
                                 printf("Credit request from customer  successfully executed. \n");
                              /*   bzero(buffer,MAX);
                                 strcpy(buffer,"true");// sending true
                                 n = write(sockfd,buffer,strlen(buffer));*/

                                 bzero(buffer,MAX);
                                 double bal =available_balance(cust_id);
                                 sprintf(buffer,"%.2f",bal);
                                 printf("%s\n",buffer );
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
                            int f = debit_amount(cust_id, amount, trans);
                               if(f==1)
                               {
                                 printf("Debit request for customer  successfully executed. \n");
                          /*       bzero(buffer,MAX);
                                strcpy(buffer,"true");
                                n = write(sockfd,buffer,strlen(buffer));// sending true*/

                                bzero(buffer,MAX);
                                double bal =available_balance(cust_id);
                                sprintf(buffer,"%.2f",bal);
                                printf("%s\n",buffer );
                                n = write(sockfd,buffer,strlen(buffer));

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
                  strcpy(buffer,"false");
                  else
                  strcpy(buffer,"true");
                   n = write(sockfd,buffer,strlen(buffer));
                   if (n < 0)
                       perror("ERROR writing to socket");


                   break;
                 }
          case '4':{////////////////Mini Statement

                   mini_statement(sockfd,cust_id);

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
  char buffer[MAX],cust_id1[MAX];
  char id[MAX],trans[MAX], amount[MAX], ip[MAX], pass[MAX],type[MAX];
  bzero(id,MAX);
  strcpy(id,cust_id);
  //////* Reading flag
  bzero(buffer,MAX);
  n = read(sockfd,buffer,MAX-1);
  if (n < 0)
      perror("ERROR reading from socket");

  strcat(cust_id,".txt");

  while(buffer[0]=='y')
  {

    printf("%s.....\n",buffer );
      bzero(ip,MAX);
      n = read(sockfd,ip,sizeof(ip));///read choice 1 - 4
      if (m < 0)
        perror("ERROR reading from socket");

      printf("ip =%s \n",ip );

        switch (ip[0]) {
          case '1':{//////////Check Balance


                    if(checkValidUser(id,'J')==1 )// sending command
                    {
                      printf("valid user\n" );
                      double amt = available_balance(cust_id);
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

                   read(sockfd,trans,sizeof(trans));//read trans
                   printf("%s\n",trans );
                   read(sockfd,amount,sizeof(amount));//read amount

                   printf("%s\n",amount );
                   int check=checkValidUser(id,'J');
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
                            int x =credit_amount(cust_id,amount,trans);
                            if(x==1)
                               {
                                 printf("Credit request from customer  successfully executed. \n");
                      /*           bzero(buffer,MAX);
                                 strcpy(buffer,"true");// sending true
                                 n = write(sockfd,buffer,strlen(buffer));*/

                                 bzero(buffer,MAX);
                                 double bal =available_balance(cust_id);
                                 printf("%s\n",id );
                                 sprintf(buffer,"%.2f",bal);
                                 printf("%s\n",buffer );
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
                            int f = debit_amount(cust_id, amount, trans);
                               if(f==1)
                               {
                                 printf("Debit request for customer  successfully executed. \n");
                              /*   bzero(buffer,MAX);
                                strcpy(buffer,"true");
                                n = write(sockfd,buffer,strlen(buffer));// sending true*/

                                bzero(buffer,MAX);
                                double bal =available_balance(cust_id);
                                sprintf(buffer,"%.2f",bal);
                                printf("%s\n",buffer );
                                n = write(sockfd,buffer,strlen(buffer));

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

                    int x;
                    bzero(cust_id1,MAX);
                    bzero(trans,MAX);
                    bzero(pass,MAX);

                    read(sockfd,cust_id1,sizeof(cust_id1));//cust_id1
                    printf("%s\n",cust_id1 );
                    read(sockfd,trans,sizeof(trans));//read old pass
                    printf("%s\n",trans );
                    read(sockfd,pass,sizeof(pass));//read pass
                    printf("%s\n",pass );

                    printf("uuiuiui\n");
                    x =changePasswordJ(id,cust_id1,trans,pass);
                    printf("yuyyuyuyu\n");
                    bzero(buffer,MAX);
                    if(x==0)
                    strcpy(buffer,"false");
                    else
                    strcpy(buffer,"true");
                    n = write(sockfd,buffer,strlen(buffer));
                    if (n < 0)
                      perror("ERROR writing to socket");

                    break;
                 }
          case '4':{////////////////Mini Statement

                   mini_statement(sockfd,cust_id);

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
    char *welcomemessage = "\nWelcome to the Banking System\n\n";

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
    while(1)
    {
    int new_socket;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&server, (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
  //  bzero(welcomemessage, MAX);
  if(!fork())
  {
    char buff[MAX],usr[MAX],pass[MAX];
    int n;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("\n" );
     write(new_socket , welcomemessage , strlen(welcomemessage) );////welcome to banking system
     bzero(buff, MAX);
     strcpy(buff,"Enter your account name\n");
     write(new_socket,buff,sizeof(buff));///enter username
     bzero(buff, MAX);
     read(new_socket, buff, sizeof(buff));// read username
     strcpy(usr,buff);

     bzero(buff, MAX);
     strcpy(buff,"Enter password\n");
     write(new_socket,buff,sizeof(buff));//enter password
     bzero(buff, MAX);
     read(new_socket, buff, sizeof(buff));// read password
     strcpy(pass,buff);


     if(strcmp(usr,"admin")==0  && strcmp(pass,"1212")==0)// if admin
     {
       write(new_socket,"1",sizeof("1"));
       admin(new_socket);
     }
     else
     {
       int v= verify_credentials(usr,pass);
       printf("%d \n",v);
       if(v==2)
       {
         write(new_socket,"2",sizeof("2"));
         customer(new_socket,usr);
       }
       else if(v==3)
       {
         write(new_socket,"3",sizeof("3"));
         jointCustomer(new_socket,usr);
       }
       else
       {
         write(new_socket,"Unauthorized User",sizeof("Unauthorized User"));
       }

     }
   }
}
    return 0;

}
