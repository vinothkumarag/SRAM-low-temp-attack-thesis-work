/**
 * Copyright (c) 2014 CASED - Center for Advanced Security Research Darmstadt
 * Security Hardware Group  - http://shw.cased.de
 * 
 * PUF Measurement Controller
 *
 * @author  Tolga Arul, 2014
 * @contact tolga.arul@cased.de
 */
package puf.utils;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import puf.context.Measurement;

public class SerialCommProcessStandardOutputStreamReader implements Runnable
{
	public static final boolean STREAMTYPE_STD_OUT = true;
	public static final boolean STREAMTYPE_ERR_OUT = false;

    final PidProcess serialCommProcess;
    final boolean standardOutputStreamType;
    final String stopString;
    final int measurementTimeout;
    final String serialCommSessionDescriptor;
    final Measurement measurement;
    
    private Thread thread;
    private boolean isRunning;
    private BufferedInputStream input;
    
	private Timer timer;
	private TimerTask timerTask;
    
	private boolean success = false;

		
    public SerialCommProcessStandardOutputStreamReader(
    		PidProcess serialCommProcess,
    		boolean standardOutputStreamType, 
    		String stopString, 
    		int measurementTimeout,
    		final String serialCommSessionDescriptor,
    		final Measurement measurement)
    {
        this.serialCommProcess = serialCommProcess;
        this.standardOutputStreamType = standardOutputStreamType;
        this.stopString = stopString;
        this.measurementTimeout = measurementTimeout;
        this.serialCommSessionDescriptor = serialCommSessionDescriptor;
        this.measurement = measurement;
        
		timer = new Timer();
		timerTask = new TimerTask()
		{
			@Override
			public void run()
			{
				if(isRunning)
				{
					System.out.println(serialCommSessionDescriptor + "_______________________________________________________Running force Stop");
					measurement.killSerialCommProcess(false);
					interrupt();
				}
			}
		};
        
        
        isRunning = true;
        thread = new Thread(this);
        thread.start();
    }

    public void run()
    {
		// start timeout timer
		timer.schedule(timerTask, measurementTimeout);// * 1000
    	
       	try
       	{	
            // 1. open stream
       		if(standardOutputStreamType == STREAMTYPE_STD_OUT)
       		{
       			input = new BufferedInputStream(serialCommProcess.getProcess().getInputStream());
       		}
       		else
       		{
       			input = new BufferedInputStream(serialCommProcess.getProcess().getErrorStream());
       		}
       		
       		Thread.sleep (1000);	//650

            // 2. Read from process
       		byte[] complete = new byte[4000000];	//40000
       		int completePos = 0;
       		byte[] content = new byte[1024];
       		byte[] stopStringBytes = stopString.getBytes();
       		boolean allCharsFound = true;

       		int read = 0;
			int i = 0;
       		outerWhile:
       		while(((read = input.read(content)) != -1) && !thread.isInterrupted())
       		{
				System.arraycopy(content, 0, complete, completePos, read);

				if(completePos >= stopString.length())
				{
					i = completePos-stopString.length();
				}
				else
				{
					i = completePos;
				}
				
				for(;i<completePos+read; i++)
				{
					if(complete[i] == stopStringBytes[0])
					{
						allCharsFound = true;
						
						innerFor:
						for(int j=0; j<stopString.length(); j++)
				 		{
							if(complete[i+j] != stopStringBytes[j])
							{
								allCharsFound = false;
								break innerFor;
							}
				   		}
							
						if(allCharsFound)
						{
							System.out.println(serialCommSessionDescriptor + "_______________stopString found________________");
							success = true;
							break outerWhile;
						}
					}
				}
					
				completePos+=read;
			}
            
			input.close();
       		input = null;
            System.out.println(serialCommSessionDescriptor + " input closed");
            
            if(success)
            {
            	// clean out stop string from data
            	for(int j=completePos-1;j>(completePos-1)-(stopString.length()+9);j--) //j>stopString.length()+2
            	{
            		if(complete[j] == -115) //13
            		{
            			completePos = j;
            			break;
            		}
            	}
            	
            	// for STM
            	int start = 0;
            	int completePosLabel = completePos;
            	if(completePos == 8193)
            	{
            		start=1;
            		completePos--;
            		completePosLabel = completePos;
            	}
            	
	            // 3. Write out data
				writeBuffered(ByteBuffer.wrap(complete,start,completePos), completePosLabel);
            }

        }
        catch (IOException e)
        {
        	e.printStackTrace();
        }
		catch (InterruptedException e)
		{
			e.printStackTrace();
		}
		
		if(!thread.isInterrupted())
		{
			// cancel timeout timer
			timerTask.cancel();
	       	isRunning = false;
	       	
	        // 4. Kill serial comm process
	        measurement.killSerialCommProcess(success);
		}
		
    }

	public void interrupt()
	{
		this.thread.interrupt();
	}
	
	private void writeBuffered(ByteBuffer content, int size) throws IOException 
	{
		Date now = new Date();
	    File file = new File("measurements\\" + serialCommSessionDescriptor + "\\" + serialCommSessionDescriptor + "_" + new SimpleDateFormat("yyyy_MM_dd_HH_mm_ss_SSS").format(now) + "_" + size);
	    file.getParentFile().mkdirs();
		FileOutputStream f = new FileOutputStream(file);
		FileChannel writeChannel = f.getChannel();
		System.out.println(serialCommSessionDescriptor + " writing result to " + file.getAbsolutePath());
		writeChannel.write(content);
		writeChannel.close();
		f.close();
	}
   
}
