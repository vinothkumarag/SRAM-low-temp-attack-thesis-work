/**
 * Copyright (c) 2014 CASED - Center for Advanced Security Research Darmstadt
 * Security Hardware Group  - http://shw.cased.de
 * 
 * PUF Measurement Controller
 *
 * @author  Tolga Arul, 2014
 * @contact tolga.arul@cased.de
 */
package puf.context;

import gnu.io.SerialPort;
import puf.MeasurementControl;
import puf.Serial;
import puf.utils.PidProcess;
import puf.utils.SerialCommProcessMessageConsumer;
import puf.utils.SerialCommProcessStandardOutputStreamReader;

public class Measurement implements Runnable
{
	private Thread thread;
	private volatile boolean isRunning = false;
	private volatile boolean isSuspended = false;
	
	private int index;
	private int numberOfMeasurements;
	private int timeBetweenBoards;
	private int timeBetweenMeasurements;
	private int osPortDetectionOffset;
	private String stopString;
	private int timeout;
	private RelaisControlContext relaisControlContext;
	private SerialCommContext serialCommContext;
	private MeasurementControl mc;
	
	private PidProcess externalSerialCommProcess;
	SerialCommProcessStandardOutputStreamReader outputStreamReader;
	
	private Serial internalSerial;
	SerialCommProcessMessageConsumer messageConsumer;
	
	int successCounter = 0;
	int unsuccessCounter = 0;
	
	public Measurement(int index, int numberOfMeasurements, int timeBetweenBoards, int osPortDetectionOffset,
			int timeBetweenMeasurements, String stopString, int timeout,
			RelaisControlContext relaisControlContext,
			SerialCommContext serialCommContext, MeasurementControl mc)
	{
		this.index = index;
		this.numberOfMeasurements = numberOfMeasurements;
		this.timeBetweenBoards = timeBetweenBoards;
		this.osPortDetectionOffset = osPortDetectionOffset;
		this.timeBetweenMeasurements = timeBetweenMeasurements;
		this.stopString = stopString;
		this.timeout = timeout;
		this.relaisControlContext = relaisControlContext;
		this.serialCommContext = serialCommContext;
		this.mc = mc;
		
		isRunning = true;
		thread = new Thread(this);
		thread.start();
	}

	
	public void killSerialCommProcess(boolean measurementSuccessful)
	{
		// 5. Kill serial comm process
		if(serialCommContext.isTypeIsInternal())
		{
			internalSerial.dispose();
			internalSerial = null;
			messageConsumer = null;
		}
		else
		{
			mc.killProcess(externalSerialCommProcess);
			outputStreamReader = null;
			externalSerialCommProcess = null;
		}
		
		mc.relaisControl(
				false, 
				relaisControlContext.isTypeIsInternal(), 
				1, 
				index+1, 
				relaisControlContext.isTypeIsInternal() ? relaisControlContext.getInternalBoardPort():relaisControlContext.getExternalPathToExecutable(), 
				relaisControlContext.isTypeIsInternal() ? "" : relaisControlContext.getExternalPuttySession());
		
		if(measurementSuccessful)
		{
			successCounter++;
		}
		else
		{
			unsuccessCounter++;
		}

		mc.propagateProgress(index, successCounter, unsuccessCounter);

		// 7. Resume thread
		if(isSuspended())
		{
			resumeThread();
		}
	}

	
	@Override
	public void run()
	{
		
		// 1. Initial wait timeBetweenBoards
//		try
//		{
//			Thread.sleep(index * timeBetweenBoards);// * 1000);
//		}
//		catch (InterruptedException e)
//		{
//			e.printStackTrace();
//		}
		
		for(int i=0;i<numberOfMeasurements;i++)
		{
			if(isRunning)
			{
				// 2. Switch on port
				mc.relaisControl(
					true, 
					relaisControlContext.isTypeIsInternal(), 
					1, 
					index+1, 
					relaisControlContext.isTypeIsInternal() ? relaisControlContext.getInternalBoardPort():relaisControlContext.getExternalPathToExecutable(),
					relaisControlContext.isTypeIsInternal() ? "" : relaisControlContext.getExternalPuttySession());
				
//				// ------> new ON CYLCE
//				try
//				{
//					Thread.sleep(timeout);
//				}
//				catch (InterruptedException e)
//				{
//					e.printStackTrace();
//				}
//				
//				mc.relaisControl(
//						false, 
//						relaisControlContext.isTypeIsInternal(), 
//						1, 
//						index+1, 
//						relaisControlContext.isTypeIsInternal() ? relaisControlContext.getInternalBoardPort():relaisControlContext.getExternalPathToExecutable(), 
//						relaisControlContext.isTypeIsInternal() ? "" : relaisControlContext.getExternalPuttySession());
//				
//				successCounter++;
//
//				mc.propagateProgress(index, successCounter, unsuccessCounter);
				// ------> new 
				
				//2.A wait for COM port to become ready
				try
				{
					Thread.sleep(osPortDetectionOffset);	//1000, 675
				}
				catch (InterruptedException e1)
				{
					e1.printStackTrace();
				}
			
				// 3. Start serial comm process
				if(serialCommContext.isTypeIsInternal())
				{
					internalSerial = new Serial(serialCommContext.getInternalCommPorts()[index], 115200, SerialPort.PARITY_NONE, SerialPort.DATABITS_8, SerialPort.STOPBITS_1);
					messageConsumer = new SerialCommProcessMessageConsumer(internalSerial, stopString, timeout, serialCommContext.getInternalCommPorts()[index], this);
				}
				else
				{
					externalSerialCommProcess = mc.createProcess("cmd /c " + serialCommContext.getExternalPathToPlink() + " -load \"" + serialCommContext.getExternalPuttySessions()[index] + "\"");
					
					outputStreamReader = new SerialCommProcessStandardOutputStreamReader(
							externalSerialCommProcess, 
							SerialCommProcessStandardOutputStreamReader.STREAMTYPE_STD_OUT, 
							stopString, 
							timeout,
							serialCommContext.getExternalPuttySessions()[index],
							this);
				}
				
				// 4. Wait for signal from serial comm/reader process
				System.out.println(serialCommContext.getExternalPuttySessions()[index] + " suspend");
				suspendThread();
				
				// 8. Wait timeBetweenMeasurements
				try
				{
					//System.out.println(serialCommContext.getExternalPuttySessions()[index] + " wait between measurements");
					Thread.sleep(timeBetweenMeasurements); // * 1000
				}
				catch (InterruptedException e)
				{
					e.printStackTrace();
				}
				
			}
		}
		
	}


	public void stop()
	{
		isRunning = false;
		
		// 5. Kill serial comm process
		if (serialCommContext.isTypeIsInternal())
		{
			if(internalSerial != null)
			{
				internalSerial.dispose();
				internalSerial = null;
				messageConsumer = null;
			}
		}
		else
		{
			if(externalSerialCommProcess != null)
			{
				mc.killProcess(externalSerialCommProcess);
				outputStreamReader = null;
				externalSerialCommProcess = null;
			}
		}

		// 6. switch off port
		mc.relaisControl(
				false,
				relaisControlContext.isTypeIsInternal(),
				1,
				index + 1,
				relaisControlContext.isTypeIsInternal() ? relaisControlContext.getInternalBoardPort() : relaisControlContext.getExternalPathToExecutable(),
				relaisControlContext.isTypeIsInternal() ? "" : relaisControlContext.getExternalPuttySession());

		// 7. Resume thread
		if (isSuspended())
		{
			resumeThread();
		}
	}
	
	public void suspendThread() 
	{
		synchronized(this.thread)
		{
			isSuspended = true;
			try
			{
				this.thread.wait();
			}
			catch (InterruptedException e)
			{
				e.printStackTrace();
			}
		}
	}

	public void resumeThread() 
	{
		synchronized(this.thread)
		{
			isSuspended = false;
			this.thread.notify();
		}
	}

	public boolean isSuspended() 
	{
		return isSuspended;
	} 

	boolean isRunning()
	{
		return isRunning;
	}

	public String getSessionDescriptor()
	{
		return serialCommContext.isTypeIsInternal() ? serialCommContext.getInternalCommPorts()[index] : serialCommContext.getExternalPuttySessions()[index];
	}

}
