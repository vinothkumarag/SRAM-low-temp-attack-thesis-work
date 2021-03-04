/**
 * Copyright (c) 2014 CASED - Center for Advanced Security Research Darmstadt
 * Security Hardware Group  - http://shw.cased.de
 * 
 * PUF Measurement Controller
 *
 * @author  Tolga Arul, 2014
 * @contact tolga.arul@cased.de
 */
package puf.gui;

import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;

import puf.MeasurementControl;

public class ProgressGUI extends JDialog
{
	private static final long serialVersionUID = -6098178782494141190L;

    private JPanel jPanel;
    private MeasurementControl mc;
	
    private int boardCount;
    private JLabel[] labels;
    private JLabel[] successCount;
    private JLabel[] unsuccessCount;
    
	public ProgressGUI(MeasurementControl mc)
	{
		this.mc = mc;
		initialize();
	}
	
	private void initialize()
	{
		try
		{
			UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
		}
		catch (ClassNotFoundException e)
		{
			e.printStackTrace();
		}
		catch (InstantiationException e)
		{
			e.printStackTrace();
		}
		catch (IllegalAccessException e)
		{
			e.printStackTrace();
		}
		catch (UnsupportedLookAndFeelException e)
		{
			e.printStackTrace();
		}
		
		this.setPreferredSize(new java.awt.Dimension(448, 409));
		{
			jPanel = getJPanel();
			getContentPane().add(jPanel, BorderLayout.NORTH);
			jPanel.setSize(480, 150);
		}
		this.setSize(448, 409);
		
		addWindowListener(new WindowAdapter()
		{
	        public void windowClosing(WindowEvent we)
	        {
	        	mc.stopMeasurement();
	        }
		});
	}
	
	private JPanel getJPanel() {
		if(jPanel == null) {
			
			boardCount = mc.getNumberOfBoards();
			
		    labels = new JLabel[boardCount];
		    successCount = new JLabel[boardCount];
		    unsuccessCount = new JLabel[boardCount];
			
		    GridLayout thisLayout = new GridLayout(boardCount+1, 3);
		    thisLayout.setColumns(3);
		    thisLayout.setHgap(10);
		    thisLayout.setVgap(10);
		    
			jPanel = new JPanel();
			jPanel.setLayout(thisLayout);
						
			JLabel testLabel = new JLabel();
			Font labelFont = testLabel.getFont();
			Font myLabelFont = new Font(labelFont.getName(), Font.BOLD, 16);
			Border empty = new EmptyBorder(10, 10, 0, 0);
			
			JLabel header = new JLabel();
			header.setText("Board");
			header.setSize(30, 15);
			header.setBorder(empty);
			header.setFont(myLabelFont);
			
			JLabel successes = new JLabel();
			successes.setText("Successes");
			successes.setSize(30, 15);
			successes.setBorder(empty);
			successes.setFont(myLabelFont);
			
			JLabel fails = new JLabel();
			fails.setText("Fails");
			fails.setSize(30, 15);
			fails.setBorder(empty);
			fails.setFont(myLabelFont);
			
			jPanel.add(header);
			jPanel.add(successes);
			jPanel.add(fails);
			
			for(int i=0;i<boardCount;i++)
			{
				labels[i] = new JLabel();
				labels[i].setText(mc.getMeasurements()[i].getSessionDescriptor());
				labels[i].setSize(30, 15);
				labels[i].setBorder(empty);
				labels[i].setFont(myLabelFont);
				
				successCount[i] = new JLabel();
				successCount[i].setText("0");
				successCount[i].setSize(30, 15);
				successCount[i].setBorder(empty);
				successCount[i].setFont(myLabelFont);
				
				unsuccessCount[i] = new JLabel();
				unsuccessCount[i].setText("0");
				unsuccessCount[i].setSize(30, 15);
				unsuccessCount[i].setBorder(empty);
				unsuccessCount[i].setFont(myLabelFont);
				
				jPanel.add(labels[i]);
				jPanel.add(successCount[i]);
				jPanel.add(unsuccessCount[i]);
						
			}
			
			jPanel.setBounds(0, 0, 480, 150);
		}
		return jPanel;
	}
	
	public void updateLabels(int measurementIndex, int successes, int unsuccesses)
	{
		successCount[measurementIndex].setText(successes+"");
		unsuccessCount[measurementIndex].setText(unsuccesses+"");
	}

}
