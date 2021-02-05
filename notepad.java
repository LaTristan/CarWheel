import java.awt.event.*;

import javax.swing.JColorChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;

public class notepad extends JFrame{
    /**
     *
     */
    private static final long serialVersionUID = 1L;

    public static void main(String[] args) {
        JFrame frame=new JFrame();
        frame.setSize(600,400);
        frame.add(new JLabel("label"));
        frame.setVisible(true);
        frame.setDefaultCloseOperation(EXIT_ON_CLOSE);
    }
}