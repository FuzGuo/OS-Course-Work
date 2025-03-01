//【例7.1】  声明继承Thread类的奇数/偶数序列线程。

public class NumberThread extends Thread
{
  private int first;                           //序列初值

  public NumberThread(String name,int first)
  {
      super(name);
      this.first = first;
  }

  public NumberThread(String name)
  {
      this(name,0);
  }
  
  public void run()                            //线程体，覆盖Thread的run()
  {
      System.out.print("\n"+this.getName()+":  ");
      for (int i=first; i<50; i+=2){
          System.out.print(i+"  ");
		  try {
			sleep(10);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
      	}
      System.out.println(this.getName() +"结束！");
  }
  
  public static void main(String args[])
  {
      System.out.println("currentThread="+Thread.currentThread().getName());     //输出当前线程对象名
      System.out.println("main Priority="+Thread.currentThread().getPriority()); //输出当前线程对象的优先级

      NumberThread thread_odd = new NumberThread("奇数线程",1);     //创建线程对象
      NumberThread thread_even = new NumberThread("偶数线程",2);
      //thread_odd.setPriority(10);  //设置优先级为最高

      thread_odd.start();                      //启动线程对象
      thread_even.start();
      System.out.println("activeCount="+Thread.activeCount());//输出当前活动线程数
  }
}

/*
程序运行结果如下：
currentThread=main
activeCount=3
main Priority=5

奇数线程:  1  3  5  7  9  11  13  15  17  19  21  23  25  27
偶数线程:  2  4  6  8  10  12  14  16  18  20  22  24  26  28  30  32  34
36  38  40  42  44  46  48  偶数线程结束！
29  31  33  35  37  39  41  43  45  47  49  奇数线程结束！




再次运行
thread_odd.setPriority(10);      //最高优先级

奇数线程:  1  3  5  7  9  11  13  15  17  19  21  23  25  27  29  31  33  35  37
39  41  43  45  47  49  奇数线程结束！
activeCount=2

偶数线程:  2  4  6  8  10  12  14  16  18  20  22  24  26  28  30  32  34
36  38  40  42  44  46  48  偶数线程结束！




再次运行
thread_odd.setPriority(1);       //最低优先级

activeCount=3
偶数线程:  2  4  6  8  10  12  14  16  18  20  22  24  26  28  30  32  34
36  38  40  42  44  46  48  偶数线程结束！

奇数线程:  1  3  5  7  9  11  13  15  17  19  21  23  25  27  29  31  33  35  37
39  41  43  45  47  49  奇数线程结束！

*/