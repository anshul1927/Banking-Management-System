#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include<fcntl.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 1024

void admin(int sockfd)
{
	  char buffer[MAX],ipbuf[MAX];
    char id[MAX],trans[MAX],amount[MAX],pass[MAX],type[MAX];
    int n;
    char flag,ip;
    char confirm;

    printf("Do you want to continue using Banking services (y/n): \n");
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
        printf("\nSpecify action :\n1. Add new customer account\n2. Make a transaction to current user account(C-Credit/D-Debit)\n3. Update current user's password\n4. Delete user account\n");
        scanf("%[^\n]%*c",&ip);

        ipbuf[0] = ip;
        ipbuf[1] = '\0';
        int m = write(sockfd,ipbuf,strlen(ipbuf));///action to be performed -1-4
        if (m < 0)
            perror("ERROR writing to socket");

        switch (ip) {
          case '1': {//////////Add Account

  								 printf("\nManadatory Rs 100 as start balance for creating new account. \n");
									 bzero(type,MAX);
									 printf("Enter type - C for normal customer, J for joint customer\n");
									 scanf("%[^\n]%*c",type);
									 write(sockfd,type,sizeof(type));// send type
						       if(type[0]=='C')
									 {
									 bzero(id,MAX);
                   printf("\nEnter username of the customer\n");
                   scanf("%[^\n]%*c",id);
									 write(sockfd,id,strlen(id)); // send id

                   bzero(pass,MAX);
                   printf("\nSet Password of the new customer\n");
                   scanf("%[^\n]%*c",pass);
                   write(sockfd,pass,sizeof(pass));// send pass
							  	 }
									 else if(type[0]=='J')
									 {
										 bzero(id,MAX);
	                   printf("\nEnter account name of the joint customer\n");
	                   scanf("%[^\n]%*c",id);
										 write(sockfd,id,strlen(id)); // send id

										 bzero(id,MAX);
	                   printf("\nEnter username of primary customer\n");
	                   scanf("%[^\n]%*c",id);

										 bzero(pass,MAX);
	                   printf("\nSet Password of the primary customer\n");
	                   scanf("%[^\n]%*c",pass);
										 strcat(id," ");
										 strcat(id,pass);
	                   write(sockfd,id,sizeof(id));// send u(user1 pass1)

										 bzero(id,MAX);
	                   printf("\nEnter username of secondary customer\n");
	                   scanf("%[^\n]%*c",id);


	                   bzero(pass,MAX);
	                   printf("\nSet Password of the secondary customer\n");
	                   scanf("%[^\n]%*c",pass);
										 strcat(id," ");
										 strcat(id,pass);
	                   write(sockfd,id,sizeof(id));// send u(user2 pass2)

									 }

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

									 bzero(type,MAX);
                   printf("Enter type -C/J \n");
                   scanf("%[^\n]%*c",type);
									 write(sockfd,type,sizeof(type));//write amount

                   // true or false
                   bzero(buffer,MAX);
                   n = read(sockfd,buffer,sizeof(buffer));// read true/false
                   if (n < 0)
                       perror("ERROR reading from socket");

                   if(!strcmp(buffer,"false"))
                   {
                       printf("Transaction denied.\n\n");
                   }
                   else //if (!strcmp(buffer,"true"))
                   {
                    //   printf("Transaction successful.\n\n");

		                 /*  n = read(sockfd,buffer,sizeof(buffer));// read balance*/
											 printf("\n Transaction successful. \nThe updated balance is - %s \n",buffer);
											 bzero(buffer,MAX);

                   }

                   break;
                 }
          case '3':{///////////////////////Password update

										printf("\nEnter type - C for normal customer, J for joint customer\n");
										scanf("%[^\n]%*c",type);
										write(sockfd,type,sizeof(type));// send type
										if(type[0]=='C')
										{
											bzero(id,MAX);
											printf("\nEnter username of the customer\n");
											scanf("%[^\n]%*c",id);
											write(sockfd,id,strlen(id)); // send id

											bzero(trans,MAX);
											printf("\nEnter old password\n");
											scanf("%[^\n]%*c",trans);
											write(sockfd,trans,strlen(trans)); // send old password

											bzero(pass,MAX);
											printf("\nSet new Password of the customer\n");
											scanf("%[^\n]%*c",pass);
											write(sockfd,pass,sizeof(pass));// send new pass
										}

										else if(type[0]=='J')
 									 {
 										 bzero(id,MAX);
 	                   printf("Enter account name(not username) of the joint customer\n");
 	                   scanf("%[^\n]%*c",id);
 										 write(sockfd,id,strlen(id)); // send ACC id

 										 bzero(id,MAX);
 	                   printf("\nEnter username of customer whose password has to be changed\n");
 	                   scanf("%[^\n]%*c",id);
										 write(sockfd,id,strlen(id)); // send cust_id

										 bzero(trans,MAX);
										 printf("\nEnter old password\n");
										 scanf("%[^\n]%*c",trans);
										 write(sockfd,trans,strlen(trans)); // send old password

										 bzero(pass,MAX);
										 printf("\nSet new Password of the customer\n");
										 scanf("%[^\n]%*c",pass);
										 write(sockfd,pass,sizeof(pass));// send new pass
									 }

                   bzero(buffer,MAX);
                   n = read(sockfd,buffer,MAX-1);   // true or false
                   if (n < 0)
                       perror("ERROR reading from socket");

                   if(!strcmp(buffer,"false"))
                   {
                       printf("\nPassword could not be updated\n\n");
                   }
                   else
                   {
                       printf("\nPassword update successful.\n\n");
                   }

                   break;
                 }
          case '4':{////////////////Delete Account

								   printf("\nEnter Y to confirm delete\n");
                   char confirm[MAX];

                   scanf("%[^\n]%*c",confirm);
									 write(sockfd,confirm,sizeof(confirm));

                   if(confirm[0]=='Y')
                   {
										 bzero(type,MAX);
										 printf("\nEnter type - C for normal customer, J for joint customer\n");
 										scanf("%[^\n]%*c",type);
 										write(sockfd,type,sizeof(type));// send type
 										if(type[0]=='C')
 										{
 											bzero(id,MAX);
											printf("\nEnter username of the customer whose account has to be deleted\n");
											scanf("%[^\n]%*c",id);
 											write(sockfd,id,strlen(id)); // send id

 											bzero(pass,MAX);
 											printf("\nAsk Password of the customer for confirmation\n");
 											scanf("%[^\n]%*c",pass);
 											write(sockfd,pass,sizeof(pass));// send new pass
 										}

 										else if(type[0]=='J')
  									 {
  										 bzero(id,MAX);
  	                   printf("\nEnter account name of the joint customer\n");
  	                   scanf("%[^\n]%*c",id);
  										 write(sockfd,id,strlen(id)); // send id

  										 bzero(id,MAX);
  	                   printf("Enter username of primary customer\n");
  	                   scanf("%[^\n]%*c",id);// Ist user
											 write(sockfd,id,strlen(id)); // send id

 										 	bzero(trans,MAX);
 										 	printf("Enter username of secondary customer\n");
 										 	scanf("%[^\n]%*c",trans);
 										 	write(sockfd,trans,strlen(trans)); // secondary user

 										 	bzero(pass,MAX);
 										 	printf("Enter Password of primary customer to confirm\n");
 										 	scanf("%[^\n]%*c",pass);
 										 	write(sockfd,pass,sizeof(pass));// set new pass
 									 }
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


        printf("Do you want to continue using Banking services (y/n): \n");
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
  //   write(sockfd,cust_id,strlen(cust_id)); // send id
    int n;
    char flag,ip;
    char confirm;


    printf("Do you want to continue using Banking services (y/n): \n");
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
                   else
                   {
                    //   printf("Transaction successful.\n\n");
											 printf("\n Transaction successful. \nThe updated balance is - %s \n",buffer);
											 bzero(buffer,MAX);
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
                     n = read(sockfd,buffer,sizeof(buffer));
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


        printf("Do you want to continue using Banking services (y/n): \n");
        scanf("%c",&flag);
        getchar();

    // sending flag
    bzero(buffer,MAX);
    buffer[0] = flag;
    buffer[1] = '\0';
  //  n = write(sockfd,buffer,strlen(buffer));
  }
}

void jointCustomer(int sockfd , char *id)
{
	  char buffer[MAX],ipbuf[MAX];
    char trans[MAX],amount[MAX],pass[MAX],type[MAX];
    int n;
    char flag,ip;
    char confirm;

    printf("Do you want to continue using Banking services (y/n): \n");
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
        scanf("%[^\n]%*c",ipbuf);

      /*  ipbuf[0] = ip;
        ipbuf[1] = '\0';*/
				printf("%s\n",ipbuf );
        int m = write(sockfd,ipbuf,sizeof(ipbuf));///action to be performed -1-4
        if (m < 0)
            perror("ERROR writing to socket");

        switch (ipbuf[0]) {
          case '1':{//////////Check Balance

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
                   else
                   {
            //           printf("Transaction successful.\n\n");
											 printf("\n Transaction successful. \nThe updated balance is - %s \n",buffer);
											 bzero(buffer,MAX);
                   }

                   break;
                 }
          case '3':{///////////////////////Password update


										bzero(id,MAX);
										printf("Enter username of customer whose password has to be changed\n");
										scanf("%[^\n]%*c",id);
										write(sockfd,trans,strlen(trans)); // userbame write

										bzero(trans,MAX);
										printf("Enter old password\n");
										scanf("%[^\n]%*c",trans);
										write(sockfd,trans,strlen(trans)); // send old password

										bzero(pass,MAX);
										printf("Set Password of the new customer\n");
										scanf("%[^\n]%*c",pass);
										write(sockfd,pass,sizeof(pass));// send new pass

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
                     n = read(sockfd,buffer,sizeof(buffer));
							//			 printf("%s \n",buffer );
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


        printf("Do you want to continue using Banking services (y/n): \n");
        scanf("%c",&flag);
        getchar();

    // sending flag
    bzero(buffer,MAX);
    buffer[0] = flag;
    buffer[1] = '\0';
//    n = write(sockfd,buffer,strlen(buffer));
}
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
    //  printf("%s \n",buff);
      write(sock, buff, sizeof(buff));// write username
			strcpy(usr,buff);
      bzero(buff, sizeof(buff));


      read(sock, buff, sizeof(buff));///enter password
      printf("%s \n",buff);
      bzero(buff, sizeof(buff));
      n = 0;
  //    scanf("%[^\n]%*c",buff);

      //printf("enterrrrrrrrrrrrr\n" );
			char *password;
      password = getpass("");
			for(int i=0;i<strlen(password);i++)
			printf("*");
    //  printf("%s",password);
  //    printf("%s",password);
			strcpy(buff,password);
      write(sock, buff, strlen(buff));// write password
      bzero(buff, sizeof(buff));

			read(sock, buff, sizeof(buff));
			printf("\n");
			if(strcmp(buff,"1")==0)//admin
        {
					printf("\n ****************Welcome Admin********************* \n" );
					admin(sock);
				}

      else if(strcmp(buff,"2")==0)
        {
					printf("\n *****************Welcome %s************************ \n",usr);//normal user
				  customer(sock,usr);
				}

			else if(strcmp(buff,"3")==0)
	        {
						printf("\n *****************Welcome %s ********************\n",usr);//joint user
						jointCustomer(sock,usr);
					}

			else
         printf("Unauthorized User \n");

  return 0;
}
