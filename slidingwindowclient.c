
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<strings.h>
#include<string.h>
#include<unistd.h>
int seqno=0;
int main(int argc, char **argv)
{
int sockfd;
int generated_pkts;
int extra_pkts=0;
int n,len;
char str[30];
struct sockaddr_in servaddr,cliaddr;
char rbuf[300],buf[300],ack[300];
int i; 
char packetno[10];
int ack_pkts=0;
int sent_pkts=0;
int window_size;
if((sockfd= socket(AF_INET,SOCK_STREAM,0))<0)
printf("socket creation error");
else
printf("socket created successfully");
bzero((struct sockaddr*)&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_port=htons(atoi(argv[2]));
inet_aton(argv[1],&servaddr.sin_addr);
if(connect(sockfd,(struct sockaddr*) &servaddr,sizeof(servaddr))<0)
printf("connection error ");
else
printf("connection successful");
len=sizeof(cliaddr);
// request to send
strcpy(buf,"RTS");
write(sockfd,buf,300);
read(sockfd,rbuf,300);
window_size=atoi(rbuf);
fprintf(stdout,"the window size is %d",window_size);
for(;;)
{
fflush(stdout);
printf("\n enter the no of packets to generate ");
fscanf(stdin,"%d",&generated_pkts);
fflush(stdout);
fprintf(stdout," packets generated is %d",generated_pkts);
if(generated_pkts <= window_size)
{
printf("generated packets is less than window size\n");
for(i=1;i<=generated_pkts;i++)
{
fprintf(stdout,"packet %d going to be sent \n",i);
sprintf(packetno,"%d",i);
write(sockfd,packetno,100);
sent_pkts++;sleep(6);
}
strcpy(str,"END");
write(sockfd,str,100);
read(sockfd,ack,300);
ack_pkts=atoi(ack)-1;
printf("%d packets have been acknowledged \n",ack_pkts);
}
else
{
fprintf(stdout,"generated packets is more than window size\n");
ack_pkts=0;
do
{
for(i=1;i<=window_size;i++)
{
fprintf(stdout,"packet %d going to be sent\n",i+ack_pkts);
sprintf(packetno,"%d",i);
write(sockfd,packetno,100);
sent_pkts++; sleep(6);
}
extra_pkts=generated_pkts-sent_pkts;
read(sockfd,ack,300);
ack_pkts+=atoi(ack)-1;
printf("%d packets have been acknowledged",ack_pkts);
} 
while(extra_pkts>window_size);
for(i=1;i<=extra_pkts;i++)
{
fprintf(stdout,"packet %d going to be sent.... \n",i+ack_pkts);
sprintf(packetno,"%d",i);
write(sockfd,packetno,100);
sent_pkts++;sleep(6);
}
strcpy(str,"END");
write(sockfd,str,100);
read(sockfd,ack,300);
ack_pkts+=atoi(ack)-1;
printf("%d packets have been acknowledged",ack_pkts);
}// end of else
} // end of infinite for loop
close(sockfd);
}
