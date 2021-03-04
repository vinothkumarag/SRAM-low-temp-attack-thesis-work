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

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.Charset;
import java.text.SimpleDateFormat;
import java.util.Date;

import puf.Serial;
import puf.context.Measurement;

public class SerialCommProcessMessageConsumer implements Runnable
{
	final Serial serial;
    final String stopString;
    final int measurementTimeout;
    final String serialCommSessionDescriptor;
    final Measurement measurement;
    
    private Thread thread;
    
    public SerialCommProcessMessageConsumer(
    		Serial serial,
    		String stopString, 
    		int measurementTimeout,
    		String serialCommSessionDescriptor,
    		Measurement measurement)
    {
    	this.serial = serial;
        this.stopString = stopString;
        this.measurementTimeout = measurementTimeout;
        this.serialCommSessionDescriptor = serialCommSessionDescriptor;
        this.measurement = measurement;
        
        thread = new Thread(this);
        thread.start();
    }

    public void run()
    {

       	try
       	{	
            // 2. Read from serial port
       		ByteBuffer bb = ByteBuffer.allocate(40000);  //40000
       		bb.flip();
       		bb.clear();
       		boolean success = false;
       		
            while((serial.readBytes(bb.array())) != -1) 
            {
            	String s = new String( bb.array(), Charset.forName("UTF-8") );
            	
            	if(s.indexOf(stopString) != -1)
            	{
            		System.out.println("_______________stopString found________________");
            		success = true;
            		break;
            	}
            }
            bb.flip();
            
            // 3. Write out data
            writeBuffered(bb);
            
            // 4. Kill serial comm process
            measurement.killSerialCommProcess(success);

        }
        catch (final IOException ioe)
        {
            System.err.println(ioe.getMessage());
            throw new RuntimeException(ioe);
        }
    }

	private void writeBuffered(ByteBuffer content) throws IOException 
	{
		Date now = new Date();
	    File file = new File(serialCommSessionDescriptor + "_" + new SimpleDateFormat("yyyy_MM_dd_HH_mm_ss_SSS").format(now));
		FileOutputStream f = new FileOutputStream(file);
		FileChannel writeChannel = f.getChannel();
		System.out.println(file.getAbsolutePath());
		writeChannel.write(content);
		writeChannel.close();
		f.close();
	}

}