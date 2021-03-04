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

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;

import puf.MeasurementControl;

public class SerialCommRelaisHandler
{
    private BufferedWriter brout = null;
    private MeasurementControl mc;
    private boolean isFirst = true;
    private PidProcess serialCommProcess;
    
    public SerialCommRelaisHandler(PidProcess serialCommProcess,  MeasurementControl mc)
    {
    	this.serialCommProcess = serialCommProcess;
    	this.mc = mc;
    	brout = new BufferedWriter(new OutputStreamWriter(serialCommProcess.getProcess().getOutputStream()));
    }

    public void send(final String s)
    {
    	
        try
		{
			if(brout != null)
			{
				if(isFirst)
				{
					Thread.sleep(1000);
					isFirst = false;
				}
				
				new Thread()
				{
					public void run()
					{
						try
						{
							brout.write(s);
							brout.flush();
							//System.out.println("writing " + s);
							Thread.sleep(500);
						}
						catch (IOException e)
						{
							e.printStackTrace();
						}
						catch (InterruptedException e)
						{
							e.printStackTrace();
						}
					}
				}.start();

			}
			else
			{
				Thread.sleep(1000);
				send(s);
			}
		}
		catch (InterruptedException e)
		{
			e.printStackTrace();
		}
    }
    
    public void close()
    {
        try
		{
			brout.close();
			mc.killProcess(serialCommProcess);
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
    }
    
}

