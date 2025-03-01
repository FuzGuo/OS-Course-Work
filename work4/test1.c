#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

// 定义进程结构信息
typedef struct pcbStruct {
  int pid;       // 进程id
  char name[10]; // 进程名称
  int status;    // 进程状态 1－运行  2－就绪  3－等待 0－完成
  int type;      // 进程类别 0－系统 进程 1－用户进程
  int res;       // 进程需要资源的时间点
  int totalTime; // 进程占用cpu的时间
  int runTime;   // 进程执行的时间
  int count;     // 进程运行时间计数器
  int prio;      // 进程优先级
  struct pcbStruct *next; // 链指针
} PCB;

// 定义资源结构
typedef struct resStruct {
  int pid;  // 占用进程id
  int free; // 是否可用，1：可用 0：不可用
} Resource;

Resource resource;                              // 资源
PCB *finish, *ready, *tail, *run, *wait, *head; // 队列指针

int N;                                   // 进程数
int timeSlice = 2;                       // 时间片
int hodeUpTime = 5;                      // 占用资源的时间
int changePrio = 0;                      // 改变优先数的时刻
void sort();                             // 声明排序函数
void changeRunPrio();                    // 改变run的优先级函数声明
void changeReadyPrio();                  // 改变ready的优先级函数声明
int randomPrio(double from, double to) { // 产生from到to之间的随机数
  return 1 + (int)((to)*rand() / (RAND_MAX + from));
}

// 将就绪队列中的第一个进程投入运行
runIn() {
  run = ready;         // 就绪队列头指针赋值给运行头指针
  run->status = 1;     // 进程状态变为运行态
  ready = ready->next; // 就绪队列头指针后移到下一进程
}

// 将等待队列中的第一个进程投入就绪
readyIn() {
  wait->status = 2;  // 进程状态变为就绪态
  tail->next = wait; // 将等待状态的第一个送入就绪态
  wait = wait->next; // 等待队列头指针后移到下一进程
}

// 标题输出函数
void print1() {
  printf(
      "------------------------------------------------------------------\n");
  printf("pid  name  status  type  prio res   totalTime count runTime\n");
}

// 进程PCB的输出
void print2(PCB *q) {
  printf("%d｜%-4s｜%-4d｜%-6d｜%-4d｜%-4d｜%-8d｜%-5d｜%-d\n", q->pid, q->name,
         q->status, q->type, q->prio, q->res, q->totalTime, q->count,
         q->runTime);
}

// 输出函数
void print() {
  PCB *p;
  if (ready != NULL)
    sort();        // 按优先数排序
  if (run != NULL) // 如果运行指针不为空
  {
    printf("Running..........\n");
    print2(run); // 输出当前正在运行的PCB
  }
  p = ready; // 输出就绪队列的PCB
  if (p != NULL)
    printf("Ready.........\n");
  while (p != NULL) {
    print2(p);
    p = p->next;
  }
  p = wait; // 输出等待队列的PCB
  if (p != NULL)
    printf("Waiting.........\n");
  while (p != NULL) {
    print2(p);
    p = p->next;
  }
  p = finish; // 输出完成队列的PCB
  if (p != NULL)
    printf("Finished.........\n");
  while (p != NULL) {
    print2(p);
    p = p->next;
  }
  print1();
}

insertReady(PCB *p2) // 轮转法插入就绪队列函数
{
  tail->next = p2;
  tail = p2;
  p2->next = NULL;
}

insertWait(PCB *p2) // 轮转法插入等待队列函数
{
  head->next = p2;
  head = p2;
  p2->next = NULL;
}

void creat() // 轮转法创建进程PCB
{
  PCB *p;
  int i, time;
  char na[10];
  ready = NULL;  // 就绪队列头指针
  finish = NULL; // 完成队列头指针
  run = NULL;    // 运行队列指针
  wait = NULL;   // 等待队列指针
  printf("Enter name and run time of each process :( eg. pid1[press ENTER]100) "
         "\n");
  for (i = 1; i <= N; i++) {
    p = malloc(sizeof(PCB));
    p->pid = 1000 + i;
    scanf("%s", na);
    scanf("%d", &time);
    strcpy(p->name, na);
    p->status = 2; // 进程状态初始为就绪状态

    if (i % 2 == 0) {
      p->type = 0;                    // 系统进程
      p->prio = randomPrio(1.0, 9.0); // 系统进程优先级
    } else {
      p->type = 1;                      // 用户进程
      p->prio = randomPrio(11.0, 19.0); // 用户进程优先级
    }
    p->res = time / 2;
    p->totalTime = time;
    p->count = 0;
    p->runTime = 0;
    if (ready != NULL)
      insertReady(p);
    else {
      p->next = ready;
      ready = p;
      tail = p;
    }
  }
  printf("********************调度开始********************\n");
  print1();
  print();     // 输出进程PCB信息
  run = ready; // 将就绪队列的第一个进程投入运行
  ready = ready->next;
  run->status = 1;
}

prioChangerun() // 优先数
{
  while (run != NULL) {
    if (run->res == run->runTime) { // 请求资源
      if (resource.free == 1) {
        resource.pid = run->pid;
        resource.free = 0;

      } else { // 进入等待队列
        run->count = 0;
        run->status = 3;
        PCB *p = run;
        if (wait != NULL)
          insertWait(p);
        else {
          p->next = wait;
          wait = p;
          head = p;
        }
        // 将就绪队列的第一个进程投入运行
        runIn();
      }
    }
    run->runTime = run->runTime + 1;
    run->count = run->count + 1;
    sleep(1);
    changePrio++;
    if (changePrio % 2 == 0) {
      changeRunPrio();   // 改变run的优先级
      changeReadyPrio(); // 改变ready的优先级
    }
    if ((run->runTime - run->res) >= hodeUpTime) // 释放资源
      resource.free = 1;

    // 运行完将其变为完成态，插入完成队列
    if (run->runTime >= run->totalTime) {
      if (run->pid == resource.pid) // 若当前进程运行完还占用资源，需要释放资源
        resource.free = 1;
      run->next = finish;
      finish = run;
      run->status = 0;
      run = NULL;
      if (ready != NULL) // 就绪队列不空，将第一个进程投入进行
        runIn();
    } else if (run->count == timeSlice) { // 需要切换下一个进程
      run->count = 0;                     // 计数器置0
      if (ready != NULL)                  // 如果就绪队列不空
      {
        // 将进程插入到就绪队列中等待轮转
        run->status = 2;
        insertReady(run); // 将就绪队列的第一个进程投入运行
        runIn();
      }
    }
    if (ready != NULL) { // 优先级低的先运行
      if (run->prio > ready->prio) {
        run->count = 0;    // 计数器置0
        if (ready != NULL) // 如果就绪队列不空
        {
          // 将进程插入到就绪队列中等待轮转
          run->status = 2;
          insertReady(run); // 将就绪队列的第一个进程投入运行
          runIn();
        }
      }
    }
    // 等待状态进入就绪态
    if (resource.free == 1) {
      if (wait != NULL) {
        wait->status = 2;
        insertReady(wait);
        wait = wait->next;
      }
    }
    print(); // 输出进程信息
  }
}

void changeRunPrio() { // 降低运行进程的优先级
  if (run->prio < 20)  // 优先数不能超过20
    run->prio += 1;
}

void changeReadyPrio() { // 增加就绪序列中的优先级
  PCB *p;
  p = ready;
  if (p != NULL) {
    do {
      if (p->type == 0) {
        if (p->prio > -20)
          p->prio -= 2;
      } else {
        if (p->prio > 0)
          p->prio -= 2;
      }

      p = p->next;
    } while (p != NULL);
  }
}

void sort() { // 按优先级排序仅仅需要将优先级最高的调到最前面即可

  PCB *p, *min;
  min = ready;
  p = ready;
  while (p->next != NULL) {
    if (min->prio > p->next->prio) {
      min = p->next;
      p->next = p->next->next;
      min->next = ready;
      ready = min;
    } else {
      if (p->next != NULL)
        p = p->next;
    }
  }
  p = ready;
  while (p->next != NULL) {
    p = p->next;
  }
  tail = p;
}
int main() {
  resource.free = 1; // 资源初始为空闲状态
  printf("Enter process number\n");
  scanf("%d", &N); // 输入进程数
  creat();
  prioChangerun();
}
