
//【例7.3】  设计滚动字演示线程状态及改变方法。

import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

public class WelcomeJFrame extends JFrame
{
  public WelcomeJFrame(String[] texts)         //texts指定移动字符串，数组长度决定窗口中的面板数
  {
      super("滚动字");        
      this.setBounds(300,240,400,300);
      this.setDefaultCloseOperation(EXIT_ON_CLOSE);
      
      if (texts==null || texts.length==0)
          this.getContentPane().add(new RollbyJPanel("Welcome!")); //至少有一行字符串
      else
      {
          this.getContentPane().setLayout(new GridLayout(texts.length,1));//网格布局，1行若干列
          for (int i=0; i<texts.length; i++)
              this.getContentPane().add(new RollbyJPanel(texts[i])); 
      }
      this.setVisible(true);
  }    
  public WelcomeJFrame()
  {
      this(null);
  }
  
  private class RollbyJPanel extends JPanel implements ActionListener, Runnable
  {                                            //自定义面板类，私有内部类，实现单击事件监听器接口和线程接口
      JTextField text_word,text_sleep;                   //滚动字文本行，线程睡眠时间文本行
      JButton button_start,button_interrupt;             //启动按钮，中断按钮
      JTextField text_state;                             //线程状态文本行
      Thread thread_rollby;                              //线程对象
      int sleeptime;                                     //线程睡眠时间

      RollbyJPanel(String text)                          //内部面板类构造方法
      {
          this.setLayout(new GridLayout(2,1));
          char space[]=new char[100];
          java.util.Arrays.fill(space, ' ');             //将字符数组space填充满空格
          text_word = new JTextField(text+new String(space));  //text后加空格字符串
          this.add(text_word);

          JPanel panel_sub = new JPanel(new FlowLayout(FlowLayout.LEFT)); 
          this.add(panel_sub);
          panel_sub.add(new JLabel("sleep"));
          this.sleeptime = (int)(Math.random()*100);     //产生随机数作为间隔时间
          text_sleep = new JTextField(""+sleeptime,5);
          panel_sub.add(text_sleep);
          text_sleep.addActionListener(this);
          
          button_start = new JButton("启动");
          panel_sub.add(button_start);
          button_start.addActionListener(this);
          button_interrupt = new JButton("中断");
          panel_sub.add(button_interrupt);
          button_interrupt.addActionListener(this);

          thread_rollby = new Thread(this);              //创建线程对象，目标对象是当前对象
          button_interrupt.setEnabled(false);            //设置中断按钮为无效状态
          panel_sub.add(new JLabel("state"));
          text_state = new JTextField(""+thread_rollby.getState(),10);
          text_state.setEditable(false);
          panel_sub.add(text_state);
      }

      public void run()                                  //线程体，必须是公有方法    
      {
          while (true)   //线程活动且没中断时//thread_rollby.isAlive() && !thread_rollby.isInterrupted())
              try
              {
                  String str = text_word.getText();
                  str = str.substring(1)+ str.substring(0,1);
                  text_word.setText(str);
                  Thread.sleep(sleeptime);        //线程睡眠，抛出中断异常
              }
              catch(InterruptedException e)
              {
                  break;                                 //退出循环
              }
      }                                                  //run()方法结束，线程对象终止

      public void actionPerformed(ActionEvent e)         //单击事件处理，必须是公有方法
      {
          if (e.getSource()==button_start)               //单击启动按钮时
          {
              this.sleeptime = Integer.parseInt(text_sleep.getText());
        	  thread_rollby = new Thread(this);          //重新创建一个线程对象
              
              thread_rollby.start();
              text_state.setText(""+thread_rollby.getState()); //显示线程状态
              button_start.setEnabled(false);
              button_interrupt.setEnabled(true);
          }
          
          if (e.getSource()==button_interrupt)           //单击中断按钮时
          {
              thread_rollby.interrupt();                 //设置当前线程对象中断标记
              text_state.setText(""+thread_rollby.getState()); 
              button_start.setEnabled(true);
              button_interrupt.setEnabled(false);
          }

          
      }
  }//内部类结束

  public static void main(String arg[])
  {
      String[] texts={"Welcome","Hello","Rollby"};
      new WelcomeJFrame(texts);
  }
}

/*
      new WelcomeJFrame();
相当于
      String[] texts={"Welcome"};
      new WelcomeJFrame(texts);

*/



