#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<strings.h>
#include<string.h>
int main(int argc, char **argv)
{
int listenfd,connfd,len;
struct sockaddr_in servaddr,caddr;
char rbuf[300];
int n;
char expected_pkt[300];
int ackno;
if(listenfd= socket(AF_INET,SOCK_STREAM,0)<0)
printf("socket creation error");
else
fprintf(stderr,"this is server socket");
bzero((struct sockaddr*)&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_port=htons(atoi(argv[1]));
servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
printf("bind error");
else
fprintf(stderr,"binding successful");
listen(listenfd,15);
//iterative server
for(;;)
{
len=sizeof(caddr);
connfd=accept(listenfd,(struct sockaddr*)&caddr,&len);
fprintf(stderr,"connection from %s, port no %d", inet_ntoa(caddr.sin_addr), 
                                                                                     ntohs(caddr.sin_port));
read(connfd,rbuf,300);
if(strcmp(rbuf,"RTS")==0)
write(connfd,"10",300);
for(;;)
{
sleep(2);
n=read(connfd,rbuf,300);
sleep(1);
fprintf(stderr,"received packet : %s \n",rbuf);
sleep(1);
if((strcmp(rbuf,"END"))!=0)
{ 
ackno=atoi(rbuf);
ackno=ackno+1;
}
sprintf(expected_pkt,"%d",ackno);
if(strcmp(rbuf,"END")==0)
{
fprintf(stderr,"Now writing ack to connfd \n ");
write(connfd,expected_pkt,300);
}
if(ackno==11)
{
fprintf(stderr,"NOw writing ack to connfd\n");
write(connfd,expected_pkt,300);
}
//sleep(5);
}
close(connfd);
}
}
