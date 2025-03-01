//【例7.4】 弹弹球，使用定时器实现图形动画。

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;

public class BallsCanvas extends Canvas implements ActionListener, FocusListener //画布组件响应定时、焦点事件
{
    private Ball balls[];                                  //多个球
    private Timer timer;
 
    private static class Ball                              //内部类
    {
        int x, y;                                          //坐标
        Color color;                                       //颜色
        boolean up,left;                                   //运动方向
        Ball(int x, int y, Color color)
        {
            this.x = x;
            this.y = x;
            this.color = color;
            up=left=false;
        }
    }
    
    public BallsCanvas(Color colors[], int delay)          //指定颜色、延时
    {
        this.balls = new Ball[colors.length];
        for (int i=0,x=40; i<colors.length; i++, x+=40)
             balls[i]=new Ball(x,x,colors[i]);
        this.addFocusListener(this);                       //注册焦点事件监听器
        timer = new Timer(delay,this);                     //创建定时器对象，delay指定延时
        timer.start();                                     //定时器启动
    }
    public void setDelay(int delay)                        //设置延时
    {
        timer.setDelay(delay);
    }
        
    public void paint(Graphics g)                          //在Canvas上作图
    {
        for (int i=0; i<balls.length; i++)
        {
            g.setColor(balls[i].color);                    //设置颜色
            balls[i].x = balls[i].left ? balls[i].x-10 : balls[i].x+10;
            if (balls[i].x<=0 || balls[i].x>=this.getWidth())    //到水平边更改方向
                balls[i].left = !balls[i].left;
            balls[i].y = balls[i].up ? balls[i].y-10 : balls[i].y+10;
            if (balls[i].y<=0 || balls[i].y>=this.getHeight())   //到垂直边更改方向
                balls[i].up = !balls[i].up;
            g.fillOval(balls[i].x,balls[i].y,20,20);             //画指定直径的圆
        }
    }
    
    public void actionPerformed(ActionEvent e)             //定时器定时执行事件
    {
        repaint();                                         //重画
    }
    
    public void focusGained(FocusEvent e)                  //获得焦点
    {
        timer.stop();                                      //定时器停止
    }
    public void focusLost(FocusEvent e)                    //失去焦点
    {
        timer.restart();                                   //定时器重启动
    }
}

class BallsJFrame extends JFrame implements ChangeListener
{
    private BallsCanvas ball;
    private JSpinner spinner;
    
    public BallsJFrame()
    {
        super("弹弹球");
        this.setBounds(300,200,400,300);
        this.setDefaultCloseOperation(EXIT_ON_CLOSE);
        Color colors[]={Color.red, Color.green, Color.blue, Color.magenta, Color.cyan};
        ball = new BallsCanvas(colors,100);
        this.getContentPane().add(ball);
        
        JPanel panel = new JPanel();
        this.getContentPane().add(panel,"South");
        panel.add(new JLabel("Delay"));
        spinner = new JSpinner(); 
        spinner.setValue(100);
        spinner.addChangeListener(this);
        panel.add(spinner);
        this.setVisible(true);
    }
    public void stateChanged(ChangeEvent e)                     //修改JSpinner值时
    {                                    //单击JSpinner的Up或Down按钮时，或在JSpinner中按Enter键
        ball.setDelay(Integer.parseInt(""+spinner.getValue())); //设置延时的时间间隔
    }
    public static void main(String arg[])
    {
        new BallsJFrame();
    }
}
