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

public class RelaisControlContext
{
	boolean typeIsInternal;	// true internal, false external
	int boardcount;
	
	String internalBoardPort;
	String externalPathToExecutable;
	String externalPuttySession;
	
	// Constructor for internal
	public RelaisControlContext(int boardcount, String internalBoardPort)
	{
		this.typeIsInternal = true;
		this.boardcount = boardcount;
		this.internalBoardPort = internalBoardPort;
	}
	
	// Constructor for External
	public RelaisControlContext(int boardcount, String externalPathToExecutable, String puttySession)//, byte[] externalBoardAddresses
	{
		this.typeIsInternal = false;
		this.boardcount = boardcount;
		this.externalPathToExecutable = externalPathToExecutable;
		this.externalPuttySession = puttySession;
	}

	public boolean isTypeIsInternal()
	{
		return typeIsInternal;
	}

	public int getBoardcount()
	{
		return boardcount;
	}

	public String getInternalBoardPort()
	{
		return internalBoardPort;
	}

	public String getExternalPathToExecutable()
	{
		return externalPathToExecutable;
	}

	public String getExternalPuttySession()
	{
		return externalPuttySession;
	}
	
}
