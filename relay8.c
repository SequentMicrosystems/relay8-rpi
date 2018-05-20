/*
 * relay8.c:
 *	Command-line interface to the Raspberry
 *	Pi's 8-Relay board.
 *	Copyright (c) 2016-2018 Sequent Microsystem
 *	<http://www.sequentmicrosystem.com>
 ***********************************************************************
 *	Author: Alexandru Burcea
 ***********************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include "relay.h"


#define VERSION_BASE	(int)1
#define VERSION_MAJOR	(int)0
#define VERSION_MINOR	(int)2

int gHwAdd = RELAY8_HW_I2C_BASE_ADD;

int relayChSet(int dev, u8 channel, OutStateEnumType state);

char *usage = "Usage:	 relay8 -h <command>\n"
		"         relay8 -v\n"
		"         relay8 -warranty\n"
		"         relay8 -list\n"
	    "         relay8 <id> write <channel> <on/off>\n"
		"         relay8 <id> write <value>\n"
        "         relay8 <id> read <channel>\n" 
        "         relay8 <id> read\n"	
		"         relay8 <id> test\n"
		"Where: <id> = Board level id = 0..7\n"
		"Type relay8 -h <command> for more help";// No trailing newline needed here.

		
char *warranty ="	       Copyright (c) 2016-2018 Sequent Microsystems\n"
				"                                                             \n"
				"		This program is free software; you can redistribute it and/or modify\n"
				"		it under the terms of the GNU Leser General Public License as published\n"
				"		by the Free Software Foundation, either version 3 of the License, or\n"
				"		(at your option) any later version.\n"
				"                                    \n"
				"		This program is distributed in the hope that it will be useful,\n"
				"		but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
				"		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
				"		GNU Lesser General Public License for more details.\n"
				"			\n"
				"		You should have received a copy of the GNU Lesser General Public License\n"
				"		along with this program. If not, see <http://www.gnu.org/licenses/>.";
				

int relayChSet(int dev, u8 channel, OutStateEnumType state)
{
	int resp;
	u8 val;
	
	if((channel < CHANNEL_NR_MIN) || (channel > RELAY_CH_NR_MAX))
	{
		printf("Invalid relay nr!\n");
		return ERROR;
	}
	val = readReg8(dev, RELAY8_INPORT_REG_ADD);
	switch(state)
	{
	case OFF:
		val &= ~(1 << (channel - 1));
		resp = writeReg8(dev,RELAY8_OUTPORT_REG_ADD, val);
		break;
	case ON:
		val |= 1 << (channel - 1);
		resp = writeReg8(dev,RELAY8_OUTPORT_REG_ADD, val);
		break;
		
	default:
		printf("Invalid relay state!\n");
		return ERROR;
		break;
	}
	if(0 < resp)
	{
		return OK;
	}
	return FAIL;
}

/*
* doRelayWrite:
*	Write coresponding relay channel
**************************************************************************************
*/
static void doRelayWrite(int argc, char *argv[])
{
	int pin, val, dev, valR, valRmask, retry;

	if ((argc != 5) && (argc != 4))
	{
		printf( "Usage: relay8 <id> write <relay number> <on/off> \n") ;
		printf( "Usage: relay8 <id> write <relay reg value> \n") ;
		exit (1) ;
	}

	dev = doBoardInit (gHwAdd);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if(argc == 5)
	{
		pin = atoi (argv [3]) ;
		if((pin < 1) || (pin > 8))
		{
			printf( "Relay number value out of range\n");
			exit(1);
		}

		/**/ if ((strcasecmp (argv [4], "up") == 0) || (strcasecmp (argv [4], "on") == 0))
			val = ON ;
		else if ((strcasecmp (argv [4], "down") == 0) || (strcasecmp (argv [4], "off") == 0))
			val = OFF ;
		else
		{
			val = atoi (argv [4]) ;
		}
		relayChSet(dev, pin, val);
		valR = readReg8(dev, RELAY8_INPORT_REG_ADD);
		
		valRmask = 0x01 & (valR >> (pin - 1));
		retry = RETRY_TIMES;

		while((retry > 0) && (valRmask != val))
		{
			relayChSet(dev, pin, val);
			valR = readReg8(dev, RELAY8_INPORT_REG_ADD);
			valRmask = 0x01 & (valR >> (pin - 1));
			retry--;
		}
		#ifdef DEBUG_I
		if(retry < RETRY_TIMES)
		{
			printf("retry %d times\n", 3-retry);
		}
		#endif
		if(retry == 0)
		{
			printf( "Fail to write relay\n");
			exit(1);
		}
	}
	else
	{
		val = atoi (argv [3]) ;
		if(val < 0 || val > 255)
		{
			printf( "Invalid relay value\n");
			exit(1);
		}
		retry = RETRY_TIMES;
		valR = -1;
		while((retry > 0) && (valR != val))
		{			
			writeReg8(dev, RELAY8_OUTPORT_REG_ADD, val);
			valR = readReg8(dev, RELAY8_INPORT_REG_ADD);
		}
		if(retry == 0)
		{
			printf( "Fail to write relay\n");
			exit(1);
		}
	}
}


/*
* doRelayRead:
*	Read relay state
******************************************************************************************
*/
static void doRelayRead(int argc, char *argv[])
{
	int pin, val, dev;

	dev = doBoardInit (gHwAdd);
	if(dev <= 0)
	{		 
		exit(1);
	}

	if (argc == 4)
	{
		pin = atoi (argv [3]) ;
		if((pin < 1) || (pin > 8))
		{
			printf( "Relay number value out of range\n");
			exit(1);
		}
		val = readReg8(dev, RELAY8_INPORT_REG_ADD);
		if(val < 0)
		{
			printf("Fail to read\n");
			exit(1);
		}

		val = val & (1 << (pin-1));
		if(val != 0)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
	}
	else if(argc == 3)
	{
		val = readReg8(dev, RELAY8_INPORT_REG_ADD);
		if(val < 0)
		{
			printf("Fail to read\n");
			exit(1);
		}
		printf("%d\n", val);
	}
	else
	{
		printf( "Usage: %s read relay value\n", argv [0]) ;
		exit (1) ;
	}
}

void doHelp(int argc, char *argv[])
{
	if(argc == 3)
	{
		/**/ if (strcasecmp (argv [2], "write"    ) == 0)	
		{ 
			printf("\twrite:       Set relays On/Off\n");
			printf("\tUsage:       relay8 <id> write <channel> <on/off>\n");
			printf("\tUsage:       relay8 <id> write <value>\n");
			printf("\tExample:     relay8 0 write 2 On; Set Relay #2 on Board #0 On\n"); 
		}
		else if (strcasecmp (argv [2], "test"     ) == 0)	
		{ 
			printf("\ttest:        Turn ON and OFF the relays until press a key\n");
			printf("\tUsage:       relay8 <id> test\n");
			printf("\tExample:     relay8 0 test\n"); 
		}
		else if (strcasecmp (argv [2], "read"     ) == 0)	
		{ 
			printf("\tread:        Read relays status\n");
			printf("\tUsage:       relay8 <id> read <channel>\n");
			printf("\tUsage:       relay8 <id> read\n");
			printf("\tExample:     relay8 0 read 2; Read Status of Relay #2 on Board #0\n"); 
		}
		else if (strcasecmp (argv [2], "-list"     ) == 0)	
		{ 
			printf("\t-list:       List all relay8 boards connected,\n\treturn nr of boards and stack level for every board\n");
			printf("\tUsage:       relay8 -list\n");
			printf("\tExample:     relay8 -list display: 1,0 \n"); 
		}
		else
		{
			printf("Invalid command!\n");
			printf("%s: %s\n", argv [0], usage);
		}
	}
	else
	{
		printf("%s: %s\n", argv [0], usage);
	}
}


static void doVersion(void)
{
	printf("relay8 v%d.%d.%d Copyright (c) 2016 - 2018 Sequent Microsystems\n", VERSION_BASE, VERSION_MAJOR, VERSION_MINOR);
	printf("\nThis is free software with ABSOLUTELY NO WARRANTY.\n");
	printf("For details type: relay8 -warranty\n");

}

static void doList(void)
{
	int ids[8];
	int i;
	int cnt = 0;
	
	for (i = 0; i< 8; i++)
	{
		if(boardCheck(RELAY8_HW_I2C_BASE_ADD + i) == OK)
		{
			ids[cnt]= i;
			cnt++;
		}
	}
	printf("%d", cnt);
	while(cnt > 0)
	{
		cnt--;
		printf(",%d", ids[cnt]);
	}
	printf("\n");
}

/* 
* Self test for production
*/
static void doTest(int argc, char* argv[])
{
	int dev, i, retry;
	u8 relVal, valR;
	int relayResult = 0;
	FILE* file = NULL;
	const u8 relayOrder[8] = {1, 3, 2, 4, 5, 7, 8, 6};

	dev = doBoardInit (gHwAdd);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if(argc == 4)
	{
		file = fopen( argv[3], "w");
		if(!file)
		{
			printf( "Fail to open result file\n");
			//return -1;
		}
	}	
//relay test****************************	
	if (strcasecmp( argv[2], "test") == 0)
	{
		relVal = 0;
		printf("Are all relays and LEDs turning on and off in sequence?\nPress y for Yes or any key for No....");
		startThread();
		while(relayResult == 0)
		{			
			for (i = 0; i < 8; i++)
			{
				relayResult = checkThreadResult();
				if(relayResult != 0)
				{
					break;
				}
				valR = 0;
				relVal = (u8)1<< (relayOrder[i] -1);
			
				retry = RETRY_TIMES;
				while((retry > 0) && ((valR & relVal) == 0))
				{
					relayChSet(dev, relayOrder[i], 1);	
					valR = readReg8(dev, RELAY8_INPORT_REG_ADD);
				}
				if(retry == 0)
				{
					printf( "Fail to write relay\n");
					if(file)
						fclose(file);
					exit(1);
				}		
				busyWait(150);
			}
		
		
			for (i = 0; i < 8; i++)
			{	
				relayResult = checkThreadResult();
				if(relayResult != 0)
				{
					break;
				}
				
				valR = 0xff;
				relVal = (u8)1<< (relayOrder[i] -1);
				retry = RETRY_TIMES;
				while((retry > 0) && ((valR & relVal) != 0))
				{
					relayChSet(dev, relayOrder[i], 0);	
					valR = readReg8(dev, RELAY8_INPORT_REG_ADD);
				}
				if(retry == 0)
				{
					printf( "Fail to write relay!\n");
					if(file)
						fclose(file);
					exit(1);
				}
				
				busyWait(150);
			}
		}
		
	}
	if(relayResult == YES)
	{
		if(file)
		{
			fprintf(file, "Relay Test ............................ PASS\n");
		}
		else
		{
			printf("Relay Test ............................ PASS\n");
		}
	}
	else
	{
		if(file)
		{
			fprintf(file, "Relay Test ............................ FAIL!\n");
		}
		else
		{
			printf("Relay Test ............................ FAIL!\n");
	
		}
	}
	if(file)
	{
		fclose(file);
	}
	writeReg8(dev,RELAY8_OUTPORT_REG_ADD, 0);
}


int main(int argc, char *argv [])
{
  int id;
  
  if (argc == 1)
  {
    printf( "%s\n", usage) ;
    return 1 ;
  }

  
// Help

  if (strcasecmp (argv [1], "-h") == 0)
  {
    doHelp(argc, argv);
    return 0;
  }
  
  // Warranty

  if (strcasecmp (argv [1], "-warranty") == 0)
  {
    printf("%s\n", warranty);
    return 0;
  }
  // Version

  if (strcasecmp (argv [1], "-v") == 0)
  {
    doVersion();
    return 0;
  }

  if (strcasecmp (argv [1], "-list") == 0)
  {
    doList();
    return 0;
  }
  
  if(argc < 3)
  {
	printf( "%s\n", usage) ;
    return 1 ;
  }
  id = atoi(argv[1]);
  if((id < 0) || (id > 7))
  {
    printf( "Invalid board id [0..7]\n");
	return 1;
  }
  gHwAdd = RELAY8_HW_I2C_BASE_ADD + id;
  
   /**/if (strcasecmp (argv [2], "write"    ) == 0)	{ doRelayWrite     (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "read"     ) == 0)	{ doRelayRead      (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "test"      ) == 0)	{ doTest           (argc, argv) ;	return 0 ; }
  else { printf( "%s\n", usage) ; return 1;}
  return 0;
}
