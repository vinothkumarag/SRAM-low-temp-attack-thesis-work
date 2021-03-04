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

import java.lang.reflect.Field;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;

public class PidProcess
{
	Process process;
	int pid;
	
	public PidProcess(Process process, int pid)
	{
		this.process = process;
		this.pid = pid;
	}

	public Process getProcess()
	{
		return process;
	}

	public int getPid()
	{
		return pid;
	};
	
	static interface Kernel32 extends Library 
	{
	    public static Kernel32 INSTANCE = (Kernel32) Native.loadLibrary("kernel32", Kernel32.class);
	    public int GetProcessId(Long hProcess);
	}
	
	public static int getPid(Process p)
	{
		Field f;

		if (Platform.isWindows())
		{
			try
			{
				f = p.getClass().getDeclaredField("handle");
				f.setAccessible(true);
				int pid = Kernel32.INSTANCE.GetProcessId((Long) f.get(p));
				return pid;
			}
			catch (Exception ex)
			{
				Logger.getLogger(PidProcess.class.getName()).log(Level.SEVERE, null,ex);
			}
		}
		else if (Platform.isLinux())
		{
			try
			{
				f = p.getClass().getDeclaredField("pid");
				f.setAccessible(true);
				int pid = (Integer) f.get(p);
				return pid;
			}
			catch (Exception ex)
			{
				Logger.getLogger(PidProcess.class.getName()).log(Level.SEVERE, null,ex);
			}
		}
		else
		{
		}
		return 0;
	}
}
