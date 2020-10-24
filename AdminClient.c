#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include<fcntl.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 1000



void admin(int sockfd)
{
	  char buffer[MAX],ipbuf[MAX];
    char id[MAX],trans[MAX],amount[MAX],pass[MAX],type[MAX];
    int n;
    char flag,ip;
    char confirm;

    printf("Do u want to continue (y/n): ");
    scanf("%c",&flag);
    getchar();

    while(flag=='y')
    {
        bzero(buffer,MAX);
        buffer[0] = flag;
        buffer[1] = '\0';
        n = write(sockfd,buffer,strlen(buffer));//yes/no
        if (n < 0)
            perror("ERROR writing to socket");

        bzero(ipbuf,MAX);
        printf("Specify action :\n1. Add new customer account\n2. Make a transaction to current user account(C-Credit/D-Debit)\n3. Update current user's password\n4. Delete user account\n");
        scanf("%[^\n]%*c",&ip);

        ipbuf[0] = ip;
        ipbuf[1] = '\0';
        int m = write(sockfd,ipbuf,strlen(ipbuf));///action to be performed -1-4
        if (m < 0)
            perror("ERROR writing to socket");

        switch (ip) {
          case '1': {//////////Add Account
                   bzero(id,MAX);
                   printf("Enter username of the customer\n");
                   scanf("%[^\n]%*c",id);
									 write(sockfd,id,strlen(id)); // send id

                   bzero(pass,MAX);
                   printf("Set Password of the new customer\n");
                   scanf("%[^\n]%*c",pass);
                   write(sockfd,pass,sizeof(pass));// send pass

                   bzero(type,MAX);
                   printf("Enter type - C for normal customer, J for joint customer\n");
                   scanf("%[^\n]%*c",type);
                   write(sockfd,type,sizeof(type));// send type

                   printf("Manadatory Rs 100 as start balance.\n");

                   // true or false
                   bzero(buffer,MAX);
                   n = read(sockfd,buffer,sizeof(buffer));//read new user update status
                   if (n < 0)
                       perror("ERROR reading from socket");

                   printf("%s\n",buffer);

                   break;
                 }

          case '2':{/////////////Transaction
                   bzero(id,MAX);
                   printf("Enter username of the  customer\n");
                   scanf("%[^\n]%*c",id);
									 write(sockfd,id,sizeof(id));//write id

                   bzero(trans,MAX);
                   printf("Enter credit for credit / debit for debit\n");
                   scanf("%[^\n]%*c",trans);
									 write(sockfd,trans,sizeof(trans));// write trans

                   bzero(amount,MAX);
                   printf("Enter amount - \n");
                   scanf("%[^\n]%*c",amount);
									 write(sockfd,amount,sizeof(amount));//write amount

                   // true or false
                   bzero(buffer,MAX);
                   n = read(sockfd,buffer,sizeof(buffer));// read true/false
                   if (n < 0)
                       perror("ERROR reading from socket");

                   if(!strcmp(buffer,"false"))
                   {
                       printf("Transaction denied.\n\n");
                   }
                   else (!strcmp(buffer,"true"));
                   {
                       printf("Transaction successful.\n\n");
                   }

                   break;
                 }
          case '3':{///////////////////////Password update

                   bzero(id,MAX);
                   printf("Enter username of the customer whose password has to be updated\n");
                   scanf("%[^\n]%*c",id);
                   write(sockfd,id,sizeof(id));//write id

                   bzero(pass,MAX);
                   printf("Set new Password of the new customer\n");
                   scanf("%[^\n]%*c",pass);
                   write(sockfd,pass,sizeof(pass));//write pass
                   printf("The new password is -\n");

//////////////////////////////////////////////////////////////////////////////////

                   // true or false
                   bzero(buffer,MAX);
                   n = read(sockfd,buffer,MAX-1);
                   if (n < 0)
                       perror("ERROR reading from socket");

                   if(!strcmp(buffer,"false"))
                   {
                       printf("pass not updated\n\n");
                   }
                   else
                   {
                       printf("Password update successful.\n\n");
                   }

                   break;
                 }

          case '4':{////////////////Delete Account
                   bzero(id,MAX);
                   printf("Enter username of the customer whose account has to be deleted\n");
                   scanf("%[^\n]%*c",id);


                   printf("Enter Y to confirm delete\n");
                   char confirm;
                   scanf("%c",&confirm);

                   if(confirm=='Y')
                   {
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
                   }
                     else
                     {
                       printf("Deletion Cancelled  \n");
                     }





                   break;
                 }
          default:{
                   printf("You entered wrong Input ----try again\n");
                   break;
                  }
        }


        printf("Do u want to continue (y/n): ");
        scanf("%c",&flag);
        getchar();
    }
    // sending flag
    bzero(buffer,MAX);
    buffer[0] = flag;
    buffer[1] = '\0';
    n = write(sockfd,buffer,strlen(buffer));
}

void customer(int sockfd , char *cust_id)
{
	  char buffer[MAX],ipbuf[MAX];
    char id[MAX],trans[MAX],amount[MAX],pass[MAX],type[MAX];
     write(sockfd,cust_id,strlen(cust_id)); // send id
    int n;
    char flag,ip;
    char confirm;


    printf("Do u want to continue (y/n): ");
    scanf("%c",&flag);
    getchar();

    while(flag=='y')
    {
        bzero(buffer,MAX);
        buffer[0] = flag;
        buffer[1] = '\0';
        n = write(sockfd,buffer,strlen(buffer));//yes/no
        if (n < 0)
            perror("ERROR writing to socket");

        bzero(ipbuf,MAX);
        printf("Specify action :\n1. Check Balance\n2. Make a transaction in user account(C-Credit/D-Debit)\n3. Update current user's password\n4. Print Mini Statement\n");
        scanf("%[^\n]%*c",&ip);

        ipbuf[0] = ip;
        ipbuf[1] = '\0';
        int m = write(sockfd,ipbuf,strlen(ipbuf));///action to be performed -1-4
        printf("%s\n",ipbuf );
        if (m < 0)
            perror("ERROR writing to socket");

        switch (ip) {
          case '1':{//////////Check Balance
                   printf("Balance is  \n");
                   // true or false
                   bzero(buffer,MAX);
                   n = read(sockfd,buffer,sizeof(buffer));//read new user update status
                   if (n < 0)
                       perror("ERROR reading from socket");

                   printf("%s\n",buffer);

                   break;
                 }
          case '2':{/////////////Transaction

                   bzero(trans,MAX);
                   printf("Enter credit for credit / debit for debit\n");
                   scanf("%[^\n]%*c",trans);
									 write(sockfd,trans,sizeof(trans));// write trans

                   bzero(amount,MAX);
                   printf("Enter amount - \n");
                   scanf("%[^\n]%*c",amount);
									 write(sockfd,amount,sizeof(amount));//write amount

                   // true or false
                   bzero(buffer,MAX);
                   n = read(sockfd,buffer,sizeof(buffer));// read true/false
                   if (n < 0)
                       perror("ERROR reading from socket");

                   if(!strcmp(buffer,"false"))
                   {
                       printf("Transaction denied.\n\n");
                   }
                   else (!strcmp(buffer,"true"));
                   {
                       printf("Transaction successful.\n\n");
                   }

                   break;
                 }
          case '3':{///////////////////////Password update

                   bzero(pass,MAX);
                   printf("Confirm your old password\n");
                   scanf("%[^\n]%*c",pass);
                   write(sockfd,pass,sizeof(pass));//write pass
                   bzero(pass,MAX);
                   printf("Set a new password\n");
                   scanf("%[^\n]%*c",pass);
                   write(sockfd,pass,sizeof(pass));//write pass

                   // true or false
                   bzero(buffer,MAX);
                   n = read(sockfd,buffer,MAX-1);
                   if (n < 0)
                       perror("ERROR reading from socket");

                   if(!strcmp(buffer,"false"))
                   {
                       printf("pass not updated\n\n");
                   }
                   else
                   {
                       printf("Password update successful.\n\n");
                   }

                   break;
                 }
          case '4':{////////////////Mini Statement
                     // true or false
                     bzero(buffer,MAX);
                     n = read(sockfd,buffer,MAX-1);
                     if (n < 0)
                         perror("ERROR reading from socket");

                     printf("%s \n",buffer );
                   break;
                 }
          default:{
                   printf("You entered wrong Input ----try again\n");
                   break;
                  }
        }


        printf("Do u want to continue (y/n): ");
        scanf("%c",&flag);
        getchar();

    // sending flag
    bzero(buffer,MAX);
    buffer[0] = flag;
    buffer[1] = '\0';
    n = write(sockfd,buffer,strlen(buffer));
  }
}

void jointCustomer(int sockfd , char *id)
{
	  char buffer[MAX],ipbuf[MAX];
    char trans[MAX],amount[MAX],pass[MAX],type[MAX];
    int n;
    char flag,ip;
    char confirm;

    printf("Do u want to continue (y/n): ");
    scanf("%c",&flag);
    getchar();

    while(flag=='y')
    {
        bzero(buffer,MAX);
        buffer[0] = flag;
        buffer[1] = '\0';
        n = write(sockfd,buffer,strlen(buffer));//yes/no
        if (n < 0)
            perror("ERROR writing to socket");

        bzero(ipbuf,MAX);
        printf("Specify action :\n1. Check Balance\n2. Make a transaction in user account(C-Credit/D-Debit)\n3. Update current user's password\n4. Print Mini Statement\n");
        scanf("%[^\n]%*c",&ip);

        ipbuf[0] = ip;
        ipbuf[1] = '\0';
        int m = write(sockfd,ipbuf,strlen(ipbuf));///action to be performed -1-4
        if (m < 0)
            perror("ERROR writing to socket");

        switch (ip) {
          case '1':{//////////Check Balance
                   bzero(id,MAX);
                   write(sockfd,id,strlen(id)); // send id


                   // true or false
                   bzero(buffer,MAX);
                   n = read(sockfd,buffer,sizeof(buffer));//read new user update status
                   if (n < 0)
                       perror("ERROR reading from socket");

                   printf("%s\n",buffer);

                   break;
                 }
          case '2':{/////////////Transaction

									 write(sockfd,id,sizeof(id));//write id

                   bzero(trans,MAX);
                   printf("Enter credit for credit / debit for debit\n");
                   scanf("%[^\n]%*c",trans);
									 write(sockfd,trans,sizeof(trans));// write trans

                   bzero(amount,MAX);
                   printf("Enter amount - \n");
                   scanf("%[^\n]%*c",amount);
									 write(sockfd,amount,sizeof(amount));//write amount

                   // true or false
                   bzero(buffer,MAX);
                   n = read(sockfd,buffer,sizeof(buffer));// read true/false
                   if (n < 0)
                       perror("ERROR reading from socket");

                   if(!strcmp(buffer,"false"))
                   {
                       printf("Transaction denied.\n\n");
                   }
                   else (!strcmp(buffer,"true"));
                   {
                       printf("Transaction successful.\n\n");
                   }

                   break;
                 }
          case '3':{///////////////////////Password update

                   write(sockfd,id,sizeof(id));//write id

                   bzero(pass,MAX);
                   printf("Set new Password of the new customer\n");
                   scanf("%[^\n]%*c",pass);
                   write(sockfd,pass,sizeof(pass));//write pass
                   printf("The new password is -\n");

//////////////////////////////////////////////////////////////////////////////////

                   // true or false
                   bzero(buffer,MAX);
                   n = read(sockfd,buffer,MAX-1);
                   if (n < 0)
                       perror("ERROR reading from socket");

                   if(!strcmp(buffer,"false"))
                   {
                       printf("pass not updated\n\n");
                   }
                   else
                   {
                       printf("Password update successful.\n\n");
                   }

                   break;
                 }
          case '4':{////////////////Mini Statement
                   bzero(id,MAX);
                   write(sockfd,id,strlen(id)); // send id

                     // true or false
                     bzero(buffer,MAX);
                     n = read(sockfd,buffer,MAX-1);
                     if (n < 0)
                         perror("ERROR reading from socket");

                     printf("%s \n",buffer );
                   break;
                 }
          default:{
                   printf("You entered wrong Input ----try again\n");
                   break;
                  }
        }


        printf("Do u want to continue (y/n): ");
        scanf("%c",&flag);
        getchar();
    }
    // sending flag
    bzero(buffer,MAX);
    buffer[0] = flag;
    buffer[1] = '\0';
    n = write(sockfd,buffer,strlen(buffer));
}


int main()
{

      int sock = 0, valread;
      struct sockaddr_in adminclient;
      char buffer[1024] = {0};
      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      {
          printf("\n Socket creation error \n");
          return -1;
      }

      adminclient.sin_family = AF_INET;
      adminclient.sin_port = htons(4567);

      // Convert IPv4 and IPv6 addresses from text to binary form
      if(inet_pton(AF_INET, "127.0.0.1", &adminclient.sin_addr)<=0)
      {
          printf("\nInvalid address/ Address not supported \n");
          return -1;
      }

      if (connect(sock, (struct sockaddr *)&adminclient, sizeof(adminclient)) < 0)
      {
          printf("\nConnection Failed \n");
          return -1;
      }


      char buff[MAX],usr[MAX],pass[MAX];
      int n;


  //  for (;;) {
      bzero(buff, sizeof(buff));
      read(sock, buff, sizeof(buff));///welcome message
      printf("%s \n",buff);
      bzero(buff, sizeof(buff));
      //  bzero(buff, sizeof(buff));
      read(sock, buff, sizeof(buff));///enter username
      printf("%s \n",buff);
      bzero(buff, MAX);
      n = 0;
      scanf("%[^\n]%*c",buff);
    //  while ((buff[n++] = getchar()) != '\n')   ;
      printf("%s \n",buff);
      write(sock, buff, sizeof(buff));// write username
			strcpy(usr,buff);
      bzero(buff, sizeof(buff));


      read(sock, buff, sizeof(buff));///enter password
      printf("%s \n",buff);
      bzero(buff, sizeof(buff));
      n = 0;
      while ((buff[n++] = getchar()) != '\n');

      write(sock, buff, sizeof(buff));// write password
      bzero(buff, sizeof(buff));

      if(strcmp(usr,"admin")==0) // if admin
      {
        admin(sock);
      }

      else if(strcmp(usr,"yash")==0)
      {
        customer(sock,usr);

      }
      else
      {
        jointCustomer(sock,usr);
      }



  return 0;
}
