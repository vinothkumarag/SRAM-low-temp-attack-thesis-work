/**
 * Copyright (c) 2014 CASED - Center for Advanced Security Research Darmstadt
 * Security Hardware Group  - http://shw.cased.de
 * 
 * PUF Measurement Controller
 *
 * @author  Tolga Arul, 2014
 * @contact tolga.arul@cased.de
 */
package puf;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.EventObject;

import javax.swing.ActionMap;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.ComboBoxModel;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.JToggleButton;
import javax.swing.JToolBar;

import org.jdesktop.application.Action;
import org.jdesktop.application.Application;
import org.jdesktop.application.SingleFrameApplication;

import puf.context.RelaisControlContext;
import puf.context.SerialCommContext;
import puf.gui.ProgressGUI;

public class GUI extends SingleFrameApplication {
    private JTextField jTextField3;
    private JTextField jTextField2;
    private JTextField jTextField1;
    private JTextField jTextField0;
    private JLabel jLabel3;
    private JLabel jLabel2;
    private JLabel jLabel1;
    private JLabel jLabel0;
    private JPanel Measurement;
    private JPanel Boards;
    private JLabel jLabel12;
    private JButton jButton4;
    private JButton jButton3;
    private JLabel jLabel11;
    private JPanel jPanel3;
    private JRadioButton jRadioButton4;
    private JRadioButton jRadioButton3;
    private ButtonGroup buttonGroup2;
    private JLabel jLabel10;
    private JRadioButton jRadioButton2;
    private JRadioButton jRadioButton1;
    private ButtonGroup buttonGroup1;
    private JLabel jLabel9;
    private JTextField jTextField4;
    private JLabel jLabel8;
    private JComboBox<String> jComboBox1;
    private JLabel jLabel7;
    private JButton jButton2;
    private JPanel topPanel;
    private JSeparator jSeparator;
    private JButton saveButton;
    private JButton openButton;
    private JTextField jTextField6;
    private JTextField jTextField10;
    private JLabel jLabel4;
    private JButton jButton1;
    private JPanel jPanel8;
    private JPanel jPanel7;
    private JTextField jTextField7;
    private JLabel jLabel15;

    private JPanel jPanel6;
    private JPanel jPanel5;
    private JLabel jLabel14;
    private JTextField jTextField5;
    private JLabel jLabel13;
    private JPanel jPanel4;
    private JButton newButton;
    private JToolBar toolBar;
    private JPanel toolBarPanel;
    private JPanel contentPanel;
    private ComboBoxModel<String> jComboBox1Model;
    private JButton jButton5;
    private JPanel jPanel2;
    private JPanel jPanel1;
    private boolean relaisControlHandlingisInternal = true;
    private boolean serialCommHandlingisInternal = true;

    private int boardCount = 1;

    private ProgressGUI progressGUI;
    private String[] allportNames = new String[0];
    
    private JLabel[] internalCommControlBoardLabels;
    private JToggleButton jToggleButton1;
    private JToggleButton jToggleButton2;
    private ArrayList<JComboBox<String>> internalCommControlBoardPorts;
    private ArrayList<DefaultComboBoxModel<String>> comboboxModels;

    private JLabel[] externalCommControlBoardLabels;
    private JTextField[] externalCommControlBoardTextFields;
        
    MeasurementControl mc;
    
    private void generateInternalCommControlBoardPanel()
    {
    	internalCommControlBoardLabels = new JLabel[boardCount];
    	internalCommControlBoardPorts = new ArrayList<JComboBox<String>>();
    	comboboxModels = new ArrayList<DefaultComboBoxModel<String>>();
    	
    	if(jPanel8 != null)
    	{
    		jPanel5.remove(jPanel8);
    	}
    	
    	jPanel5.setSize(330, 55 + (boardCount * 26));
    	GridLayout jPanel8Layout = new GridLayout(boardCount, 2);
		jPanel8Layout.setColumns(2);
		jPanel8Layout.setRows(boardCount);
		jPanel8Layout.setHgap(5);
		jPanel8Layout.setVgap(5);

		jPanel8 = new JPanel();
		jPanel8.setSize(320, 15 + (boardCount * 21));
		jPanel8.setLayout(jPanel8Layout);

		
    	for(int i=0; i<boardCount; i++)
    	{
    		JLabel tempLabel = new JLabel();
    		tempLabel.setText("Board " + (i+1) + " comm port");
    		internalCommControlBoardLabels[i] = tempLabel;
    		
    		JComboBox<String> tempCombobox = new JComboBox<String>();
    		DefaultComboBoxModel<String> tempModel = new DefaultComboBoxModel<String>(allportNames);
    		tempCombobox.setModel(tempModel);
    		tempCombobox.setPreferredSize(new java.awt.Dimension(152, 22));

    		internalCommControlBoardPorts.add(tempCombobox);
    		comboboxModels.add(tempModel);

    		jPanel8.add(tempLabel);
    		jPanel8.add(tempCombobox);
    	}
    	
   		jPanel5.add(jPanel8, new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
    }
    
    
    private void generateExternalCommControlBoardPanel()
    {
        externalCommControlBoardLabels = new JLabel[boardCount];
        externalCommControlBoardTextFields = new JTextField[boardCount];
        
        if(jPanel7 != null)
        {
        	jPanel6.remove(jPanel7);
        }
        
        jPanel6.setSize(330, 45 + (boardCount * 26));
        
		GridLayout jPanel7Layout = new GridLayout(boardCount, 2);
		jPanel7Layout.setColumns(2);
		jPanel7Layout.setRows(boardCount);
		jPanel7Layout.setHgap(5);
		jPanel7Layout.setVgap(5);
		
		jPanel7 = new JPanel();
		jPanel7.setLayout(jPanel7Layout);
		jPanel7.setSize(320, 15 + (boardCount * 21));

    	for(int i=0; i<boardCount; i++)
    	{
    		JLabel tempLabel = new JLabel();
    		tempLabel.setText("Board " + (i+1) + " putty session");
    		externalCommControlBoardLabels[i] = tempLabel;
    		
    		JTextField tempTextfield = new JTextField();
    		
    		String name = "ExternalCommControl_"+i+".text";
    		String value = (String) Application.getInstance().getContext().getResourceMap(this.getClass()).getObject(name, String.class);
    		tempTextfield.setText(value);
    		externalCommControlBoardTextFields[i] = tempTextfield;
    		jPanel7.add(tempLabel);
    		jPanel7.add(tempTextfield);
    	}

    	jPanel6.add(jPanel7, new GridBagConstraints(0, 1, 3, 2, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
    }
    
    private void adjustWindowSize()
    {
		contentPanel.setSize(contentPanel.getWidth(), jButton1.getY() + 35);
		contentPanel.setPreferredSize(new Dimension(contentPanel.getWidth(), jButton1.getY() + 35));
		topPanel.setSize(topPanel.getWidth(), contentPanel.getHeight() + toolBarPanel.getHeight());
		getMainFrame().setSize(347, topPanel.getHeight() + 55);//getMainFrame().getWidth()
    }
    
    
    @Action
    public void open() {
    }

    @Action
    public void save() {
    }

    @Action
    public void newFile() {
    }

    private ActionMap getAppActionMap() {
        return Application.getInstance().getContext().getActionMap(this);
    }

    @Override
    protected void startup() {
    	mc = new MeasurementControl();
    	
    	this.addExitListener(new ExitListener()
		{
			
			@Override
			public void willExit(EventObject event)
			{
				if(mc != null)
				{
					if(mc.outputStreamReader != null)
					{
						mc.outputStreamReader.close();
					}
				}
				
			}
			
			@Override
			public boolean canExit(EventObject event)
			{
				return true;
			}
		});
    	
    	
    	GridLayout mainFrameLayout = new GridLayout(1, 1);
    	mainFrameLayout.setColumns(1);
    	mainFrameLayout.setHgap(5);
    	mainFrameLayout.setVgap(5);
    	getMainFrame().getContentPane().setLayout(mainFrameLayout);
    	getMainFrame().setResizable(false);
    	getMainFrame().setLocation(new java.awt.Point(0, 0));
        {
            topPanel = new JPanel();
            getMainFrame().getContentPane().add(topPanel);
            BorderLayout panelLayout = new BorderLayout();
            topPanel.setLayout(panelLayout);
            {
                toolBarPanel = new JPanel();
                topPanel.add(toolBarPanel, BorderLayout.NORTH);
                BorderLayout jPanel1Layout = new BorderLayout();
                toolBarPanel.setLayout(jPanel1Layout);
                {
                    toolBar = new JToolBar();
                    toolBarPanel.add(toolBar, BorderLayout.CENTER);
                    {
                        newButton = new JButton();
                        toolBar.add(newButton);
                        newButton.setAction(getAppActionMap().get("newFile"));
                        newButton.setName("newButton");
                        newButton.setFocusable(false);
                    }
                    {
                        openButton = new JButton();
                        toolBar.add(openButton);
                        openButton.setAction(getAppActionMap().get("open"));
                        openButton.setName("openButton");
                        openButton.setFocusable(false);
                    }
                    {
                        saveButton = new JButton();
                        toolBar.add(saveButton);
                        saveButton.setAction(getAppActionMap().get("save"));
                        saveButton.setName("saveButton");
                        saveButton.setFocusable(false);
                    }
                }
                {
                    jSeparator = new JSeparator();
                    toolBarPanel.add(jSeparator, BorderLayout.SOUTH);
                }
            }
            {
            	topPanel.add(getContentPanel(), BorderLayout.NORTH);

            }
        }

        Boards.setVisible(true);
		jPanel4.setVisible(false);
        
		jPanel5.setVisible(true);
		jPanel6.setVisible(false);
		
        adjustWindowSize();
        show(topPanel);
        
        
        getJRadioButton2().setSelected(true);
   		for(ActionListener a: getJRadioButton2().getActionListeners()) 
		{
			a.actionPerformed(new ActionEvent("jRadioButton4", ActionEvent.ACTION_PERFORMED, "external", System.currentTimeMillis(), MouseEvent.MOUSE_PRESSED));
		}
        getJRadioButton4().setSelected(true);
   		for(ActionListener a: getJRadioButton4().getActionListeners()) 
		{
			a.actionPerformed(new ActionEvent("jRadioButton4", ActionEvent.ACTION_PERFORMED, "external", System.currentTimeMillis(), MouseEvent.MOUSE_PRESSED));
			
		}
   		
    }


    private ButtonGroup getButtonGroup1() {
    	if(buttonGroup1 == null) {
    		buttonGroup1 = new ButtonGroup();
    	}
    	return buttonGroup1;
    }
    
    private JLabel getJLabel10() {
    	if(jLabel10 == null) {
    		jLabel10 = new JLabel();
    		jLabel10.setName("jLabel10");
    		jLabel10.setMinimumSize(new java.awt.Dimension(30, 15));
    	}
    	return jLabel10;
    }

    private ButtonGroup getButtonGroup2() {
    	if(buttonGroup2 == null) {
    		buttonGroup2 = new ButtonGroup();
    	}
    	return buttonGroup2;
    }
    
    
    
    private JRadioButton getJRadioButton1() {
    	if(jRadioButton1 == null) {
    		jRadioButton1 = new JRadioButton();
    		jRadioButton1.setName("jRadioButton3");
    		jRadioButton1.setBounds(386, 53, 156, 21);
    		jRadioButton1.addActionListener(new ActionListener() {
    			public void actionPerformed(ActionEvent evt) {
    				if(jRadioButton1.isSelected())
    				{
    					setRelaisControlHandlingisInternal(true);
    					Boards.setVisible(true);
    					jPanel4.setVisible(false);
    					
    					jPanel5.setLocation(new Point(5,  getBoards().getY() + getBoards().getHeight() + 12));
    					jPanel6.setLocation(new Point(5,  getBoards().getY() + getBoards().getHeight() + 12));
    					
    					
    					if(isSerialCommHandlingisInternal())
    		    		{
    		    			jButton1.setBounds(10, getJPanel5().getY() + getJPanel5().getHeight() + 12, 320, 23);//677
    		    		}
    		    		else
    		    		{
    		    			jButton1.setBounds(10, getJPanel6().getY() + getJPanel6().getHeight() + 12, 320, 23);//677
    		    		}
    					
    					adjustWindowSize();
    				}
    			}
    		});
    		getButtonGroup1().add(jRadioButton1);
    	}
    	return jRadioButton1;
    }
    
    private JRadioButton getJRadioButton2() {
    	if(jRadioButton2 == null) {
    		jRadioButton2 = new JRadioButton();
    		jRadioButton2.setName("jRadioButton4");
    		jRadioButton2.setBounds(488, 79, 75, 21);    		

    		jRadioButton2.addActionListener(new ActionListener() {
    			public void actionPerformed(ActionEvent evt) {
    				if(jRadioButton2.isSelected())
    				{
    					setRelaisControlHandlingisInternal(false);
    					Boards.setVisible(false);
    					jPanel4.setVisible(true);
    					
    					//generateExternalRelaisControlBoardPanel();
    					
    					jPanel5.setLocation(new Point(5,  getJPanel4().getY() + getJPanel4().getHeight() + 12));
    					jPanel6.setLocation(new Point(5,  getJPanel4().getY() + getJPanel4().getHeight() + 12));
    					
    		    		if(isSerialCommHandlingisInternal())
    		    		{
    		    			jButton1.setLocation(new Point(10, getJPanel5().getY() + getJPanel5().getHeight() + 12));
    		    		}
    		    		else
    		    		{
    		    			jButton1.setLocation(new Point(10, getJPanel6().getY() + getJPanel6().getHeight() + 12));
    		    		}
    					
    					adjustWindowSize();
    				}
    			}
    		});
    		getButtonGroup1().add(jRadioButton2);
    	}
    	return jRadioButton2;
    }
    
    
    private JRadioButton getJRadioButton3() {
    	if(jRadioButton3 == null) {
    		jRadioButton3 = new JRadioButton();
    		jRadioButton3.setName("jRadioButton3");
    		jRadioButton3.setBounds(412, 34, 156, 21);
    		jRadioButton3.setPreferredSize(new java.awt.Dimension(63, 21));
    		jRadioButton3.addActionListener(new ActionListener() {
    			public void actionPerformed(ActionEvent evt) {
    				if(jRadioButton3.isSelected())
    				{
    					setSerialCommHandlingisInternal(true);
    					jPanel5.setVisible(true);
    					jPanel6.setVisible(false);

    					generateInternalCommControlBoardPanel();
    					  					
    					jButton1.setLocation(new Point(10, getJPanel5().getY() + getJPanel5().getHeight() + 12));
    					
    					adjustWindowSize();
    				}
    			}
    		});
    		getButtonGroup2().add(jRadioButton3);
    	}
    	return jRadioButton3;
    }
    
    private JRadioButton getJRadioButton4() {
    	if(jRadioButton4 == null) {
    		jRadioButton4 = new JRadioButton();
    		jRadioButton4.setName("jRadioButton4");
    		jRadioButton4.setBounds(488, 79, 75, 21);
    		jRadioButton4.setPreferredSize(new java.awt.Dimension(75, 21));
    		jRadioButton4.addActionListener(new ActionListener() {
    			public void actionPerformed(ActionEvent evt) {
    				if(jRadioButton4.isSelected())
    				{
    					setSerialCommHandlingisInternal(false);
    					jPanel5.setVisible(false);
    					jPanel6.setVisible(true);
			
    					generateExternalCommControlBoardPanel();
    					
    					jButton1.setLocation(new Point(10, getJPanel6().getY() + getJPanel6().getHeight() + 12));
    					
    					adjustWindowSize();
    				}
    			}
    		});
    		
    		getButtonGroup2().add(jRadioButton4);
    	}

    	return jRadioButton4;
    }
    
    private JPanel getJPanel3() {
    	if(jPanel3 == null) {
    		jPanel3 = new JPanel();
    		GridLayout jPanel3Layout = new GridLayout(1, 3);
    		jPanel3Layout.setColumns(3);
    		jPanel3Layout.setHgap(5);
    		jPanel3Layout.setVgap(5);
    		jPanel3.setLayout(jPanel3Layout);
    		jPanel3.setBounds(119, 331, 290, 24);
    		jPanel3.add(getJButton4());
    		jPanel3.add(getJLabel11());
    		jPanel3.add(getJButton3());
    	}
    	return jPanel3;
    }
    
    private JLabel getJLabel11() {
    	if(jLabel11 == null) {
    		jLabel11 = new JLabel();
    		jLabel11.setText(getBoardCount()+"");
    	}
    	return jLabel11;
    }
    
    private JButton getJButton3() {
    	if(jButton3 == null) {
    		jButton3 = new JButton();
    		jButton3.setName("jButton3");
    		jButton3.addActionListener(new ActionListener() {
    			public void actionPerformed(ActionEvent evt) {
    				setBoardCount(getBoardCount()+1);
					
		    		if(isRelaisControlHandlingisInternal())
		    		{
		        		jPanel5.setLocation(5, getBoards().getY() + getBoards().getHeight() + 12);//480
		        		jPanel6.setLocation(5, getBoards().getY() + getBoards().getHeight() + 12);//480
		    		}
		    		else
		    		{
		    			//generateExternalRelaisControlBoardPanel();
		        		jPanel5.setLocation(5, getJPanel4().getY() + getJPanel4().getHeight() + 12);//480
		        		jPanel6.setLocation(5, getJPanel4().getY() + getJPanel4().getHeight() + 12);//480
		    		}
    				
					if(serialCommHandlingisInternal)
					{
						generateInternalCommControlBoardPanel();
		    			jButton1.setLocation(10, getJPanel5().getY() + getJPanel5().getHeight() + 12);
					}
					else
					{
						generateExternalCommControlBoardPanel();
		    			jButton1.setLocation(10,getJPanel6().getY() + getJPanel6().getHeight() + 12);
					}
					
					adjustWindowSize();
    			}
    		});
    	}
    	return jButton3;
    }
    
    private JButton getJButton4() {
    	if(jButton4 == null) {
    		jButton4 = new JButton();
    		jButton4.setName("jButton4");
    		jButton4.addActionListener(new ActionListener() {
    			public void actionPerformed(ActionEvent evt) {
    				if(getBoardCount()-1 > 0)
    				{
    					setBoardCount(getBoardCount()-1);
    					
    		    		if(isRelaisControlHandlingisInternal())
    		    		{
    		        		jPanel5.setLocation(5, getBoards().getY() + getBoards().getHeight() + 12);//480
    		        		jPanel6.setLocation(5, getBoards().getY() + getBoards().getHeight() + 12);//480
    		    		}
    		    		else
    		    		{
    		    			//generateExternalRelaisControlBoardPanel();
    		        		jPanel5.setLocation(5, getJPanel4().getY() + getJPanel4().getHeight() + 12);//480
    		        		jPanel6.setLocation(5, getJPanel4().getY() + getJPanel4().getHeight() + 12);//480
    		    		}
    					
    					if(serialCommHandlingisInternal)
    					{
    						generateInternalCommControlBoardPanel();
    						jButton1.setLocation(10, getJPanel5().getY() + getJPanel5().getHeight() + 12);
    					}
    					else
    					{
    						generateExternalCommControlBoardPanel();
    						jButton1.setLocation(10, getJPanel6().getY() + getJPanel6().getHeight() + 12);
    					}

    					adjustWindowSize();
    				}
    			}
    		});
    	}
    	return jButton4;
    }
    
    private JLabel getJLabel12() {
    	if(jLabel12 == null) {
    		jLabel12 = new JLabel();
    		jLabel12.setName("jLabel12");
    	}
    	return jLabel12;
    }
    

    
    private JLabel getJLabel13() {
    	if(jLabel13 == null) {
    		jLabel13 = new JLabel();
    		GridLayout jLabel13Layout = new GridLayout(1, 1);
    		jLabel13Layout.setColumns(1);
    		jLabel13Layout.setHgap(5);
    		jLabel13Layout.setVgap(5);
    		jLabel13.setLayout(jLabel13Layout);
    		jLabel13.setName("jLabel13");
    		jLabel13.setPreferredSize(new java.awt.Dimension(146, 14));
    		jLabel13.setSize(160, 14);
    	}
    	return jLabel13;
    }
    
    private JTextField getJTextField5() {
    	if(jTextField5 == null) {
    		jTextField5 = new JTextField();
    		jTextField5.setName("jTextField5");
    		jTextField5.setText(MeasurementControl.getDefaultExternalRelaisControlPath());
    		jTextField5.setPreferredSize(new java.awt.Dimension(177, 20));
    	}
    	return jTextField5;
    }
    
    private JLabel getJLabel14() {
    	if(jLabel14 == null) {
    		jLabel14 = new JLabel();
    		GridLayout jLabel14Layout = new GridLayout(1, 1);
    		jLabel14Layout.setColumns(1);
    		jLabel14Layout.setHgap(5);
    		jLabel14Layout.setVgap(5);
    		jLabel14.setLayout(jLabel14Layout);
    		jLabel14.setName("jLabel14");
    		jLabel14.setPreferredSize(new java.awt.Dimension(159, 14));
    		jLabel14.setSize(160, 14);
    	}
    	return jLabel14;
    }
    
    private JTextField getJTextField6() {
    	if(jTextField6 == null) {
    		jTextField6 = new JTextField();
    		jTextField6.setName("jTextField6");
    		//jTextField6.setText("Arduino");
    		jTextField6.setPreferredSize(new java.awt.Dimension(186, 20));
    	}
    	return jTextField6;
    }
    
//    private JPanel getBoardPanel() 
//    {
//    	generateExternalRelaisControlBoardPanel();
//    	return BoardPanel;
//    }
    
	private JLabel getJLabel7()
	{
		if(jLabel7 == null)
		{
			jLabel7 = new JLabel();
			jLabel7.setBounds(404, 330, 92, 14);
			jLabel7.setName("jLabel7");
		}
		return jLabel7;
	}
	
	
	private JComboBox<String> getJComboBox1()
	{
		if(jComboBox1 == null)
		{
			jComboBox1Model = new DefaultComboBoxModel<String>(new String[] { });
			jComboBox1 = new JComboBox<String>();
			jComboBox1.setModel(jComboBox1Model);
			jComboBox1.setBounds(506, 324, 70, 20);
		}
		return jComboBox1;
	}
	
	private JButton getJButton2()
	{
		if(jButton2 == null)
		{
			jButton2 = new JButton();
			jButton2.setBounds(369, 186, 145, 23);
			jButton2.setName("jButton2");
			jButton2.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					jButton2ActionPerformed(evt);
				}
			});
		}
		return jButton2;
	}
    
    private JTextField getJTextField0() {
    	if(jTextField0 == null) {
    		jTextField0 = new JTextField();
    		jTextField0.setName("jTextField0");
    	}
    	return jTextField0;
    }
	
    private JTextField getJTextField1() {
    	if(jTextField1 == null) {
    		jTextField1 = new JTextField();
    		jTextField1.setName("jTextField1");
    		//jTextField1.setText("1");
    	}
    	return jTextField1;
    }
	
    private JTextField getJTextField2() {
    	if(jTextField2 == null) {
    		jTextField2 = new JTextField();
    		jTextField2.setName("jTextField2");
    		//jTextField2.setText("~~~~~~~~~~~~~~~");
    	}
    	return jTextField2;
    }
    
    private JTextField getJTextField3() {
    	if(jTextField3 == null) {
    		jTextField3 = new JTextField();
    		jTextField3.setName("jTextField3");
    		//jTextField3.setText("1");
    	}
    	return jTextField3;
    }
    
    private JTextField getJTextField4() {
    	if(jTextField4 == null) {
    		jTextField4 = new JTextField();
    		jTextField4.setBounds(164, 68, 152, 20);
    		jTextField4.setName("jTextField4");
    		//jTextField4.setText("1");
    	}
    	return jTextField4;
    }
    
	private JLabel getJLabel0()
	{
		if(jLabel0 == null)
		{
			jLabel0 = new JLabel();
			jLabel0.setName("jLabel0");
		}
		return jLabel0;
	}
    
	private JLabel getJLabel1()
	{
		if(jLabel1 == null)
		{
			jLabel1 = new JLabel();
			jLabel1.setName("jLabel1");
		}
		return jLabel1;
	}
	
	private JLabel getJLabel2()
	{
		if(jLabel2 == null)
		{
			jLabel2 = new JLabel();
			jLabel2.setName("jLabel2");
		}
		return jLabel2;
	}

	private JLabel getJLabel3()
	{
		if(jLabel3 == null)
		{
			jLabel3 = new JLabel();
			jLabel3.setName("jLabel3");
		}
		return jLabel3;
	}
	
	private JLabel getJLabel8()
	{
		if(jLabel8 == null)
		{
			jLabel8 = new JLabel();
			jLabel8.setBounds(10, 70, 103, 14);
			jLabel8.setName("jLabel8");
		}
		return jLabel8;
	}
	
	private JLabel getJLabel9()
	{
		if(jLabel9 == null)
		{
			jLabel9 = new JLabel();
			jLabel9.setName("jLabel9");
			jLabel9.setMinimumSize(new java.awt.Dimension(20, 15));
		}
		return jLabel9;
	}
	
	private JPanel getContentPanel()
	{
		if(contentPanel == null)
		{
        	contentPanel = new JPanel();
        	contentPanel.setLayout(null);
        	contentPanel.setPreferredSize(new java.awt.Dimension(330, 500));
        	contentPanel.setSize(330, 500);
        	contentPanel.setName("contentPanel");
       		contentPanel.add(getMeasurement());
       		contentPanel.add(getBoards());
       		contentPanel.add(getJPanel4());
       		contentPanel.add(getJPanel5());
       		contentPanel.add(getJPanel6());
       		contentPanel.add(getJButton1());
		}
		return contentPanel;
	}
	
	// Measurement
    private JPanel getMeasurement()
    {
    	if(Measurement == null)
    	{
    		GridLayout MeasurementLayout = new GridLayout(9, 2);
    		MeasurementLayout.setColumns(2);
    		MeasurementLayout.setHgap(5);
    		MeasurementLayout.setVgap(5);
    		MeasurementLayout.setRows(9);
    		Measurement = new JPanel();
    		Measurement.setLayout(MeasurementLayout);
    		Measurement.setBorder(BorderFactory.createTitledBorder("Measurement"));
    		Measurement.setBounds(5, 4, 330, 248);
			Measurement.add(getJLabel1());
			Measurement.add(getJTextField1());
			Measurement.add(getJLabel2());
			Measurement.add(getJTextField2());
			Measurement.add(getJLabel12());
			Measurement.add(getJPanel3());
			
			Measurement.add(getJLabel0());
			Measurement.add(getJTextField0());
			
			Measurement.add(getJLabel3());
			Measurement.add(getJTextField3());
			Measurement.add(getJLabel8());
			Measurement.add(getJTextField4());
			Measurement.add(getJLabel4());
			Measurement.add(getJTextField10());
			Measurement.add(getJLabel9());
			Measurement.add(getJPanel1());
			Measurement.add(getJLabel10());
			Measurement.add(getJPanel2());
    	}
    	
    	return Measurement;
    }
    
    // Internal relais control
    private JPanel getBoards()
    {
    	if(Boards == null) {
    		GridBagLayout BoardsLayout = new GridBagLayout();
    		BoardsLayout.rowWeights = new double[] {0.0, 0.0, 0.0};
    		BoardsLayout.rowHeights = new int[] {26, 26, 26};
    		BoardsLayout.columnWeights = new double[] {0.1, 0.1};
    		BoardsLayout.columnWidths = new int[] {7, 7};
    		Boards = new JPanel();
    		Boards.setLayout(BoardsLayout);
    		Boards.setBounds(5, getMeasurement().getHeight() + 12 , 330, 96);//240
    		Boards.setBorder(BorderFactory.createTitledBorder("Internal relais control"));
			Boards.add(getJButton2(), new GridBagConstraints(0, 0, 2, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
			Boards.add(getJLabel7(), new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
			Boards.add(getJComboBox1(), new GridBagConstraints(1, 1, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
			Boards.add(getJToggleButton1(), new GridBagConstraints(0, 2, 2, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
    	}
    	
    	return Boards;
    }
    
    // External relais control
    private JPanel getJPanel4() {
    	if(jPanel4 == null) {
    		jPanel4 = new JPanel();
    		GridBagLayout jPanel4Layout = new GridBagLayout();
    		jPanel4Layout.rowWeights = new double[] {0.0, 0.0, 0.0};
    		jPanel4Layout.rowHeights = new int[] {26, 26, 26};
    		jPanel4Layout.columnWeights = new double[] {0.0, 0.1};
    		jPanel4Layout.columnWidths = new int[] {161, 7};
    		jPanel4.setLayout(jPanel4Layout);
    		jPanel4.setBounds(5, getMeasurement().getHeight() + 12, 330, 96); //96
    		jPanel4.setBorder(BorderFactory.createTitledBorder("External relais control"));
    		jPanel4.add(getJLabel13(), new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0, GridBagConstraints.LINE_START, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
    		jPanel4.add(getJTextField5(), new GridBagConstraints(1, 0, 1, 1, 0.0, 0.0, GridBagConstraints.LINE_START, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
    		jPanel4.add(getJLabel14(), new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0, GridBagConstraints.LINE_START, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
    		jPanel4.add(getJTextField6(), new GridBagConstraints(1, 1, 1, 1, 0.0, 0.0, GridBagConstraints.LINE_START, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
    		//jPanel4.add(getBoardPanel(), new GridBagConstraints(0, 2, 2, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
    		jPanel4.add(getJToggleButton2(), new GridBagConstraints(0, 2, 2, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
    	
    	}
    	return jPanel4;
    }
    
    // Internal comm control
    private JPanel getJPanel5() {
    	if(jPanel5 == null) {
    		jPanel5 = new JPanel();
    		GridBagLayout jPanel5Layout = new GridBagLayout();
    		jPanel5Layout.rowWeights = new double[] {0.0, 0.1};
    		jPanel5Layout.rowHeights = new int[] {23, 7};
    		jPanel5Layout.columnWeights = new double[] {0.1};
    		jPanel5Layout.columnWidths = new int[] {7};
    		jPanel5.setLayout(jPanel5Layout);
    		if(isRelaisControlHandlingisInternal())
    		{
        		jPanel5.setBounds(5, getBoards().getY() + getBoards().getHeight() + 12, 330, 80);//480
    		}
    		else
    		{
        		jPanel5.setBounds(5, getJPanel4().getY() + getJPanel4().getHeight() + 12, 330, 80);//480
    		}
    		jPanel5.setBorder(BorderFactory.createTitledBorder("Internal comm control"));
    		jPanel5.setName("jPanel5");
    		jPanel5.add(getJPanel8(), new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
    		jPanel5.add(getJButton5(), new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
    	}
    	return jPanel5;
    }
    
    
   // External comm control
    private JPanel getJPanel6() {
    	if(jPanel6 == null) {
    		jPanel6 = new JPanel();
    		GridBagLayout jPanel6Layout = new GridBagLayout();
    		jPanel6Layout.rowWeights = new double[] {0.0, 0.1};
    		jPanel6Layout.rowHeights = new int[] {25, 7};
    		jPanel6Layout.columnWeights = new double[] {0.0, 0.1};
    		jPanel6Layout.columnWidths = new int[] {95, 7};
    		jPanel6.setLayout(jPanel6Layout);
    		
    		if(isRelaisControlHandlingisInternal())
    		{
    			jPanel6.setBounds(5, getBoards().getY() + getBoards().getHeight() + 12, 330, 80);//480
    		}
    		else
    		{
    			jPanel6.setBounds(5, getJPanel4().getY() + getJPanel4().getHeight() + 12, 330, 80);//480
    		}
    		
    		jPanel6.setBorder(BorderFactory.createTitledBorder("External comm control"));
    		jPanel6.add(getJLabel15(), new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
    		jPanel6.add(getJTextField7(), new GridBagConstraints(1, 0, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
    		jPanel6.add(getJPanel7(), new GridBagConstraints(0, 1, 3, 2, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
    	}
    	return jPanel6;
    }
    
    private JLabel getJLabel15() {
    	if(jLabel15 == null) {
    		jLabel15 = new JLabel();
    		jLabel15.setName("jLabel15");
    		jLabel15.setBounds(368, 471, 96, 20);
    	}
    	return jLabel15;
    }
    
    private JTextField getJTextField7() {
    	if(jTextField7 == null) {
    		jTextField7 = new JTextField();
    		jTextField7.setName("jTextField7");
    		jTextField7.setText(MeasurementControl.getDefaultExternalCommControlPath());
    		jTextField7.setBounds(535, 480, 59, 20);
    	}
    	return jTextField7;
    }
    
    private JPanel getJPanel7()
    {
    	generateExternalCommControlBoardPanel();
    	return jPanel7;
    }
    
    private JPanel getJPanel8()
    {
    	generateInternalCommControlBoardPanel();
    	return jPanel8;
    }
    

    private ProgressGUI getProgressGUI(MeasurementControl mc)	
    {
    	progressGUI = new ProgressGUI(mc);
    	progressGUI.setModal(true);
    	progressGUI.setLocation(getMainFrame().getX()+200, this.getMainFrame().getY() +200);
    	progressGUI.setResizable(false);
    	progressGUI.setTitle("Progress");
    	return progressGUI;
    }
    
    
    private JButton getJButton1() {
    	if(jButton1 == null) {
    		jButton1 = new JButton();
    		
    		if(isSerialCommHandlingisInternal())
    		{
    			jButton1.setBounds(10, getJPanel5().getY() + getJPanel5().getHeight() + 12, 320, 23);//677
    		}
    		else
    		{
    			jButton1.setBounds(10, getJPanel6().getY() + getJPanel6().getHeight() + 12, 320, 23);//677
    		}
    		
    		
    		jButton1.setName("jButton1");
    		jButton1.addActionListener(new ActionListener() {
    			public void actionPerformed(ActionEvent evt) 
    			{
    				boolean allOK = true;
    				RelaisControlContext rcc = null;
    				SerialCommContext scc = null;
    				
   					String stopString ="";
   					int osPortDetectionOffset = 0;
    				int numberOfMeasurements = 0;
					int timeBetweenMeasurements = 0;
					int timeBetweenBoards = 0;
					int timeout = 0;
    				
    				if(isRelaisControlHandlingisInternal())
    	    		{
    					if(getJComboBox1().getSelectedItem() != null)
    					{
    						String internalBoardPort = getJComboBox1().getSelectedItem().toString();
	    					rcc = new RelaisControlContext(boardCount, internalBoardPort);
    					}
    					else
    					{
    						allOK = false;
    						showError("Please scan for available ports first!","Empty argument");
    					}
    	    		}
    				else
    				{

    					boolean fieldsOK = true;
    					
    					if(fieldsOK && !getJTextField5().getText().isEmpty() && !getJTextField6().getText().isEmpty())
    					{
    						rcc = new RelaisControlContext(boardCount, getJTextField5().getText(), getJTextField6().getText());
    					}
    					else
    					{
    						allOK = false;
    						showError("Please check inputs for external relais control!","Empty argument");
    					}
    				}
    				
    				if(isSerialCommHandlingisInternal())
    	    		{
    					String[] internalCommPorts = new String[boardCount];
    					
    					if(internalCommControlBoardPorts.get(0).getSelectedItem() != null)
    					{
    						for(int i=0; i<boardCount;i++)
    						{
    							internalCommPorts[i] = new String(internalCommControlBoardPorts.get(i).getSelectedItem().toString());
    						}
	    					scc = new SerialCommContext(boardCount, internalCommPorts);
    					}
    					else
    					{
    						allOK = false;
    						showError("Please scan for available ports first!","Empty argument");
    					}
    	    		}
    				else
    				{
    					boolean fieldsOK = true;
    					String[] externalPuttySessions = new String[boardCount];
    					
   						for(int i=0; i<boardCount;i++)
   						{
   							if(externalCommControlBoardTextFields[i].getText().isEmpty())
   							{
   								fieldsOK = false;
   								break;
   							}
   							else
   							{
   								externalPuttySessions[i] = new String(externalCommControlBoardTextFields[i].getText());
   							}
   						}
   						
   						if(fieldsOK && !getJTextField7().getText().isEmpty())
   						{
   	    					scc = new SerialCommContext(boardCount, getJTextField7().getText(), externalPuttySessions);
   						}
    					else
    					{
    						allOK = false;
    						showError("Please enter putty sessions!","Empty argument");
    					}
    				}


    				if(allOK)
    				{
    					if(!getJTextField2().getText().isEmpty())
    					{	
    						stopString = getJTextField2().getText();
    					}
    					else
    					{
    						allOK = false;
	    					showError("Stop string can not be empty","Empty argument");
    					}
						
	    				try
	    				{
	    					numberOfMeasurements = Integer.parseInt(getJTextField1().getText());
	    					osPortDetectionOffset = Integer.parseInt(getJTextField0().getText());
	    					timeBetweenMeasurements = Integer.parseInt(getJTextField3().getText());
	    					timeBetweenBoards = Integer.parseInt(getJTextField4().getText());
	    					timeout = Integer.parseInt(getJTextField10().getText());
	    				}
	    				catch(NumberFormatException n)
	    				{
	    					allOK = false;
	    					showError("Please check measurement settings!","Wrong argument type");
	    				}
    				}
    				
    				if(allOK)
    				{
    					mc.startMeasurement(numberOfMeasurements, stopString, boardCount, osPortDetectionOffset, timeBetweenMeasurements, timeBetweenBoards, timeout, rcc, scc);
    					final ProgressGUI pg = getProgressGUI(mc);
    					mc.setProgressGui(pg);
    					pg.pack();
    					pg.setVisible(true);

    				}
    				
    			}
    		});
    		
    	}
    	return jButton1;
    }
    
    private JLabel getJLabel4() {
    	if(jLabel4 == null) {
    		jLabel4 = new JLabel();
    		jLabel4.setName("jLabel4");
    	}
    	return jLabel4;
    }

    
    
    private JTextField getJTextField10() {
    	if(jTextField10 == null) {
    		jTextField10 = new JTextField();
    		jTextField10.setName("jTextField10");
    	}
    	return jTextField10;
    }

	public int getBoardCount()
	{
		return boardCount;
	}

	public void setBoardCount(int boardCount)
	{
		this.boardCount = boardCount;
		jLabel11.setText(getBoardCount()+"");
	}

	public void showError(String message, String errorType)
	{
		JOptionPane.showMessageDialog(getContentPanel(),
				message,
				errorType,
			    JOptionPane.ERROR_MESSAGE);
	}
	
	public boolean isRelaisControlHandlingisInternal()
	{
		return relaisControlHandlingisInternal;
	}

	public void setRelaisControlHandlingisInternal(boolean relaisControlHandlingisInternal)
	{
		this.relaisControlHandlingisInternal = relaisControlHandlingisInternal;
	}

	public boolean isSerialCommHandlingisInternal()
	{
		return serialCommHandlingisInternal;
	}

	public void setSerialCommHandlingisInternal(boolean serialCommHandlingisInternal)
	{
		this.serialCommHandlingisInternal = serialCommHandlingisInternal;
	}
	
	// Measurement - Relais control handling
	private JPanel getJPanel1() {
		if(jPanel1 == null) {
			jPanel1 = new JPanel();
			GridLayout jPanel1Layout1 = new GridLayout(1, 2);
			jPanel1Layout1.setColumns(2);
			jPanel1Layout1.setHgap(5);
			jPanel1Layout1.setVgap(5);
			jPanel1.setLayout(jPanel1Layout1);
			jPanel1.setBounds(382, 216, 172, 91);
			jPanel1.add(getJRadioButton1());
			jPanel1.add(getJRadioButton2());
		}
		return jPanel1;
	}
	
	// Mesaurement - Serial comm handling
	private JPanel getJPanel2() {
		if(jPanel2 == null) {
			jPanel2 = new JPanel();
			GridLayout jPanel2Layout = new GridLayout(1, 2);
			jPanel2Layout.setColumns(2);
			jPanel2Layout.setHgap(5);
			jPanel2Layout.setVgap(5);
			jPanel2.setLayout(jPanel2Layout);
			jPanel2.setBounds(372, 232, 187, 30);
			jPanel2.add(getJRadioButton3());
			jPanel2.add(getJRadioButton4());
		}
		return jPanel2;
	}
	
	private void jButton2ActionPerformed(ActionEvent evt)
	{
		jComboBox1Model = new DefaultComboBoxModel<String>(getAllPortnames());
		jComboBox1.setModel(jComboBox1Model);
		
		generateInternalCommControlBoardPanel();
		for(int i=0; i<internalCommControlBoardPorts.size(); i++)
		{
			DefaultComboBoxModel<String> tempModel = new DefaultComboBoxModel<String>(allportNames);
			comboboxModels.add(tempModel);
			internalCommControlBoardPorts.get(i).setModel(tempModel);
		}
		
	}
	
	private String[] getAllPortnames()
	{
		allportNames = Serial.getPorts();
		return allportNames;
	}
	
	private JButton getJButton5() {
		if(jButton5 == null) {
			jButton5 = new JButton();
			jButton5.setName("jButton5");
			jButton5.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					jButton2ActionPerformed(evt);
				}
		});
		}
		return jButton5;
	}
    
    private JToggleButton getJToggleButton1() {
    	if(jToggleButton1 == null) {
    		jToggleButton1 = new JToggleButton();
    		jToggleButton1.setBounds(70, 403, 145, 23);
    		jToggleButton1.setName("jToggleButton1");
    		jToggleButton1.setText("Activate all ports");
    		jToggleButton1.addItemListener(new ItemListener() 
    		{
    			public void itemStateChanged(ItemEvent evt) 
    			{
    				boolean allOK = true;
    				String pathToExecutable = "";
					String commandPuttySession ="";

					if(!getJTextField5().getText().isEmpty() && !getJTextField6().getText().isEmpty())
					{
						pathToExecutable = getJTextField5().getText();
						commandPuttySession = getJTextField6().getText();
					}
					else
					{
						allOK = false;
						showError("Please check inputs for external relais control!","Empty argument");
					}

    				if(allOK)
    				{
    					if(evt.getStateChange()==ItemEvent.DESELECTED)
    					{
    						jToggleButton1.setText("Activate all ports");
    						mc.relaisControl(false, isRelaisControlHandlingisInternal(), boardCount, 1, pathToExecutable, commandPuttySession); 
    					}
    					else if(evt.getStateChange()==ItemEvent.SELECTED)
    					{
    						jToggleButton1.setText("Deactivate all ports");
    						mc.relaisControl(true, isRelaisControlHandlingisInternal(), boardCount, 1, pathToExecutable, commandPuttySession);
    					}
    				}
    				
    			}
    		});


    	}
    	return jToggleButton1;
    }
    
    
    private JToggleButton getJToggleButton2() {
    	if(jToggleButton2 == null) {
    		jToggleButton2 = new JToggleButton();
    		jToggleButton2.setBounds(70, 403, 145, 23);
    		jToggleButton2.setName("jToggleButton2");
    		jToggleButton2.setText("Activate all ports");
    		jToggleButton2.addItemListener(new ItemListener() {
    			public void itemStateChanged(ItemEvent evt)
    			{
    				boolean allOK = true;
    				String pathToExecutable = "";
					String commandPuttySession ="";
    				
					if(!getJTextField5().getText().isEmpty() && !getJTextField6().getText().isEmpty())
					{
						pathToExecutable = getJTextField5().getText();
						commandPuttySession = getJTextField6().getText();
					}
					else
					{
						allOK = false;
						showError("Please check inputs for external relais control!","Empty argument");
					}
    				
    				if(allOK)
    				{
    					if(evt.getStateChange()==ItemEvent.DESELECTED)
    					{
    						jToggleButton2.setText("Activate all ports");
    						mc.relaisControl(false, isRelaisControlHandlingisInternal(), boardCount, 1, pathToExecutable, commandPuttySession);
    					}
    					
    					else if(evt.getStateChange()==ItemEvent.SELECTED)
    					{
    						jToggleButton2.setText("Deactivate all ports");
    						mc.relaisControl(true, isRelaisControlHandlingisInternal(), boardCount, 1, pathToExecutable, commandPuttySession);
    					}
    				}
    			}
    		});
    	}
    	return jToggleButton2;
    }
 
    public static void main(String[] args)
    {
    	launch(GUI.class, args);
    }
}
