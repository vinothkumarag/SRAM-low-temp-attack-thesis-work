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

import java.io.File;
import java.io.IOException;

import javax.swing.SwingUtilities;

import org.jdesktop.application.Application;

import puf.context.Measurement;
import puf.context.RelaisControlContext;
import puf.context.SerialCommContext;
import puf.gui.ProgressGUI;
import puf.utils.PidProcess;
import puf.utils.SerialCommRelaisHandler;

public class MeasurementControl implements IMessageConsumer
{
	private byte switchState = 0x00;
	private Measurement[] measurements;
	
	ProgressGUI pg;
	Serial serial = null;
	volatile String lastMessage = "0";
	int numberOfBoards;
	PidProcess switchProcess = null;
	SerialCommRelaisHandler outputStreamReader = null;
	
	public MeasurementControl()
	{}
	
	public void startMeasurement(
			int numberOfMeasurements, String stopString,
			int numberOfBoards, int osPortDetectionOffset, int timeBetweenMeasurements,
			int timeBetweenBoards, int timeout,
			RelaisControlContext relaisControlContext,
			SerialCommContext serialCommContext
			)
	{
		
		this.numberOfBoards = numberOfBoards;
		
		// turn off all boards
		relaisControl(false, 
						relaisControlContext.isTypeIsInternal(), 
						numberOfBoards, 
						1,
						relaisControlContext.isTypeIsInternal() ? relaisControlContext.getInternalBoardPort():relaisControlContext.getExternalPathToExecutable(),
						relaisControlContext.isTypeIsInternal() ? "" : relaisControlContext.getExternalPuttySession()				
				);
		
		// create measurement objects
		measurements = new Measurement[numberOfBoards];
		for(int i=0; i<numberOfBoards; i++)
		{
			measurements[i] = new Measurement(i, numberOfMeasurements, timeBetweenBoards, osPortDetectionOffset, timeBetweenMeasurements, stopString, timeout, relaisControlContext, serialCommContext, this);
		}
	}
	
	public void stopMeasurement()
	{
		for(int i=0; i<numberOfBoards; i++)
		{
			measurements[i].stop();
		}
		
		numberOfBoards=0;
	}
	
	public void relaisControl(boolean toggleOn, boolean isRelaisControlHandlingisInternal, int boardCount, int port, String argument, String argument2)
	{
		if(!isRelaisControlHandlingisInternal)
		{
			if(switchProcess == null)
			{
				//System.out.println("cmd /c " + argument + " -load \"" + argument2 + "\"");
				switchProcess = createProcess("cmd /c " + argument + " -load \"" + argument2 + "\"");
				outputStreamReader = new SerialCommRelaisHandler(switchProcess, this);
			}
			
			if(toggleOn)
			{
				for(int i=0;i<boardCount;i++)
				{
					outputStreamReader.send("s"+(i+port)+"1"+"\r");
				}
			}
			else
			{
				for(int i=0;i<boardCount;i++)
				{
					outputStreamReader.send("s"+(i+port)+"0"+"\r");
				}
			}
		}

	}
	
	public void message(final String s)
	{
		SwingUtilities.invokeLater(new Runnable()
		{
			public void run()
			{
				lastMessage = s;
				System.out.println(s);
			}
		});
	}
	
	public void setProgressGui(ProgressGUI pg)
	{
		this.pg = pg;
	}
	
	public void propagateProgress(int measurementIndex, int successes, int unsuccesses)
	{
		pg.updateLabels(measurementIndex, successes, unsuccesses);
	}
	
	public byte getSwitchState()
	{
		return switchState;
	}

	public synchronized void setSwitchState(byte switchState)
	{
		this.switchState = switchState;
	}
	
	public String getSwitchOffCommand(byte switchOnByte, String externalRelaisControlPath)
	{
		String result = externalRelaisControlPath + " " + String.format("%02x",(getSwitchState() ^ switchOnByte)).toUpperCase();
		setSwitchState((byte) (getSwitchState() ^ switchOnByte));
		return result;
	}
	
	
	public String getSwitchOnCommand(byte switchOnByte, String externalRelaisControlPath)
	{
		String result = externalRelaisControlPath + " " + String.format("%02x",(getSwitchState() ^ switchOnByte)).toUpperCase();
		setSwitchState((byte) (getSwitchState() ^ switchOnByte));
		return result;
	}
	
	public static String getDefaultExternalRelaisControlPath()
	{
		return getCurrentPath() + "\\src\\" + Application.getInstance().getContext().getResourceMap().getResourcesDir().replace("/", "\\") + "bins\\plink.exe";
	}
	
	public static String getDefaultExternalCommControlPath()
	{
		return getCurrentPath() + "\\src\\" + Application.getInstance().getContext().getResourceMap().getResourcesDir().replace("/", "\\") + "bins\\plink.exe";
	}
	
	public static String getCurrentPath()
	{
    	String result="";
		try
		{
			result = new File(".").getCanonicalPath();
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	public PidProcess createProcess(String command)
	{
		Process p;
		int pid;
		
		try
		{
			p = Runtime.getRuntime().exec(command);
			pid = PidProcess.getPid(p);
			return new PidProcess(p, pid);
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}

		return null;
	}
	
	public void killProcess(PidProcess p)
	{
		try
		{
			Runtime.getRuntime().exec("cmd /c taskkill /PID "+ p.getPid() + " /T /F");
		}
		catch (IOException e)
		{
			System.err.println("Failed to kill process " + p.getPid());
			e.printStackTrace();
		}
		catch (NullPointerException e)
		{
			System.err.println("Process p is null in MeasurementControl.killProcess(PidProcess p)");
			e.printStackTrace();
		}
	}

	public int getNumberOfBoards()
	{
		return numberOfBoards;
	}

	public Measurement[] getMeasurements()
	{
		return measurements;
	}

}
