#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "qq_ipc.h"
#include "mylink.h"

#define SERVER_PORT "SERV_FIFO"

mylink head = NULL;

struct QQ_DATA_INFO{
	int protocal;
	char srcname[20];
	char destname[20];
	char data[100];
};
void sys_err(char *str)
{
	perror(str);
	exit(1);
}

int login_qq(struct QQ_DATA_INFO *buf,mylink *head)
{
	int fd;

	fd = open(buf->scrname,O_WRONLY);

	mylink node = make_node(buf->scrname,fd);
	mylink_insert(head,node);

	return 0;
}

void transfer_qq(struct QQ_DATA_INFO *buf,mylink *head)
{
	mylink p = mylink_search(head,buf->destname);
	if(p == NULL){
		struct QQ_DATA_INFO lineout = {3};
		strcpy(lineout.destname,buf->destname);
		mylink q = mylink_search(head,buf->srcname);

		write(q->fifo_fd,&lineout,sizeof(lineout));
	}else
		write(p->fifo_fd,buf,sizeof(*buf));
}


int logout_qq(atruct QQ_DATA_INFO *buf,mylink *head)
{
	mylink p = mylink_search(head,buf->srcname);

	close(p->fifo_fd);
	mylink_delete(head,p);
	free_node(p);
}

void err_qq(struct QQ_DATA_INFO *buf)
{
	fprintf(stderr,"bad client %s connect \n",buf->srcname);
}


int main()
{
	int server_fd;
	struct QQ_DATA_INFO dbuf;

	if(access(SERVER_PORT,F_OK) == 0){
		mkfifo(SERVER,0644)'
	}
	if((server_fd = open(SERVR_PORT,O_RDONLY)) < 0)
		sys_err("open");

	mylink_init(&head);

	while(1){
		read(server_fd,&dbuf,sizeof(dbuf));

		switch(ddbuf.protocal){
			case 1:login_qq(&dbuf,&head);
			       break;
			case 2:transfer_qq(&dbuf,&head);
			       break;
			case 4:logout_qq(&dbuf,&head);
			       break;
			default:err_qq(&dbuf);
		}
	}

	close(server_fd);
}


