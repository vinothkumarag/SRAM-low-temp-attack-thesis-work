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

public class SerialCommContext
{
	boolean typeIsInternal;	// true internal, false external
	int boardcount;
	
	String[] internalCommPorts;
	
	String externalPathToPlink;
	String[] externalPuttySessions;
	
	// Constructor for internal
	public SerialCommContext(int boardcount, String[] internalCommPorts)
	{
		this.typeIsInternal = true;
		this.boardcount = boardcount;
		this.internalCommPorts = internalCommPorts;
	}
	
	// Constructor for external
	public SerialCommContext(int boardcount, String externalPathToPlink,  String[] externalPuttySessions)
	{
		this.typeIsInternal = false;
		this.boardcount = boardcount;
		this.externalPathToPlink = externalPathToPlink;  
		this.externalPuttySessions = externalPuttySessions;
	}

	public boolean isTypeIsInternal()
	{
		return typeIsInternal;
	}

	public int getBoardcount()
	{
		return boardcount;
	}

	public String[] getInternalCommPorts()
	{
		return internalCommPorts;
	}

	public String getExternalPathToPlink()
	{
		return externalPathToPlink;
	}

	public String[] getExternalPuttySessions()
	{
		return externalPuttySessions;
	}
	
}
