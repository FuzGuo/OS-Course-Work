#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include<time.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<errno.h>

#define RN 3
#define WN 2

int readcount=0;
pthread_t Writers[WN];
pthread_t Readers[RN];
int mn[10];
int semid;
int rc=0;
int wc=0;

union semun{int val;struct semid_ds* buf;ushort* array;};
static struct sembuf px={0,-1,0};
static struct sembuf vx={0,1,0};
static struct sembuf pz={1,-1,0};
static struct sembuf vz={1,1,0};
static struct sembuf pwsem={2,-1,0};
static struct sembuf vwsem={2,1,0};

void Show(int s,int t_id,int j)
{
  int i;
  printf("Location:%d	Content:%d\n",s,t_id);
  if(j==1)
  {
    printf("now the sequence is:");
    for(i=0;i<10;++i)
    {
      printf("%3d",mn[i]);
    }
    printf("\n");
  }
  printf("\n");
}

void *reader(void *a1)
{
  int s;
  s=rand()%10;
  printf(">>Reader pthread %d creat\n",a1);
  semop(semid,&pz,1);
  semop(semid,&px,1);

  if(readcount==0)
  {
    semop(semid,&pwsem,1);
  }
  ++readcount;

  semop(semid,&vx,1);
  semop(semid,&vz,1);

  printf("Reader %d begin to read,and %d writers %d readers are waiting...\n",a1,WN-wc,RN-rc-1);

  sleep(2);
  printf("Reader %d Read Succeed!\n",a1);

  Show(s+1,mn[s],0);

  semop(semid,&px,1);
  --readcount;
  if(readcount==0)
    semop(semid,&vwsem,1);
  semop(semid,&vx,1);
}

void *writer(void *a2)
{
  int s;
  int newdata;
  s=rand()%10;
  printf("Writer pthread %d creat\n",a2);

  semop(semid,&pwsem,1);
  printf("Writer %d begin to write,and %d writers %d readers are waiting...\n",a2,WN-wc-1,RN-rc);
  newdata=rand()%10+1;
  mn[s]=newdata;
  sleep(2);
  printf("Writer %d write Succed:	",a2);
  Show(s+1,mn[s],1);
  semop(semid,&vwsem,1);
}

int main(int argc,char *argv[])
{
  int i;
  int pid;
  int ret;
  union semun sem;
  sem.val=1;
  key_t key=ftok("mem",0);

  semid=semget(key,4,IPC_CREAT|0666);
  ret=semctl(semid,0,SETVAL,sem);
  if(ret==-1)
  {
    perror("semctl 0:");
  }

  ret=semctl(semid,1,SETVAL,sem);
  if(ret==-1)
  {
    perror("semctl 0:");
  }

  ret=semctl(semid,2,SETVAL,sem);
  if(ret==-1)
  {
    perror("semctl 0:");
  }

  ret=semctl(semid,3,SETVAL,sem);
  if(ret==-1)
  {
    perror("semctl 0:");
  }

  int j;
  for(j=0;j<10;++j)
  {
    mn[j]=j+1;
  }

  printf("the original sequence is:\n");

  for(j=0;j<10;++j)
  {
    printf("%d	",mn[j]);
  }

  printf("\n");

  char c;
  while(1)
  {
    printf("r/w:");
    scanf("%s",&c);
    if(c=='w')
    {
      if(wc==WN&&rc!=RN)
      printf("writers are finished.\n");
      else
      {
        pthread_create(&Writers[wc],NULL,(void*)writer,(void*)(wc+1));
        ++wc;
        sleep(3);	
      }
    }

    else if(c=='r')
    {
      if(rc==RN&&wc!=WN)
      printf("readers are finished.\n");

      else
      {
        for(rc=0;rc<RN;++rc)
        {
          pthread_create(&Readers[rc],NULL,(void*)reader,(void*)(rc+1));
          sleep(3);
        }
      }
    }

    else
      printf("you input a wrong choice,try again:\n");

    if(wc==WN&&rc==RN)
    {
      printf("All writers and readers are finished.\n");
      exit(0);
    }

  }
    semctl(semid,IPC_RMID,0);

  return 0;	
}

