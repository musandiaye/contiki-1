#include "rn2483.h"

//MAC Commands

/**
*
* This command will automatically reset the software LoRaWAN stack and initialize it
* with the parameters for the selected band.
*
* This command will set default values for most of the LoRaWAN parameters. Everything set prior to this command will lose its set value.
*
* <band>: decimal number representing the frequency band, either 868 or 433
*
**/
unsigned int mac_reset(unsigned int band)
{
	if((band == 433) | (band == 868))
	{
		char* command = "mac reset ";
		
		//int to char*
		char* band_str; //TODO: warning, band_str may be uninitialised in this function.
		sprintf(band_str, "%u", band);

		//copy all into one array
		char *result = malloc(strlen(command)+strlen(band_str)+3);
    	strcpy(result, command);
    	strcat(result, band_str);
    	strcat(result, "\r\n");

    	rs232_print(RS232_PORT_1, result);

    	//TODO: set func name mac_reset_response as saved variable to be run when the process is brought back with the serial_line_event_message_1 event
    	//For passing func names as parameters, see LMIC for inspiration
    	//Then repeat this for all  
    	//poll 

    	return 1;
	}
	else
	{
		rs232_print(RS232_PORT_0, "ERROR: mac_reset: band chosen is not 433 or 868.\r\n");
		return 0;
	}
}

/**
*
* Response generated by the rn2483 after mac_reset command
*
* Response: "ok" if band is valid
*			"invalid_param" if band is not valid
*
**/
unsigned int mac_reset_response(char* data)
{
	if(strcmp(data,"ok"))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}




unsigned int mac_tx(char* type, unsigned int portno, char* data)
{
	if(strcmp(type,"cnf") | strcmp(type,"uncnf"))
	{
		if(portno >= 1 && portno <= 223)
		{
			//check if data is in hex form.
			if(hexFormArrayCheck(data))
			{
				//int to char*
				char* portno_str; //TODO: warning, portno_str may be uninitialised in this function.
				sprintf(portno_str, "%u", portno);

				char* command = "mac tx ";

				//copy all into one array
				char *result = malloc(strlen(command)+strlen(type)+strlen(portno_str)+strlen(data)+5);
    			strcpy(result, command);
    			strcat(result, type);
    			strcat(result, " ");
    			strcat(result, portno_str);
    			strcat(result, " ");
    			strcat(result, data);
    			strcat(result, "\r\n");

    			rs232_print(RS232_PORT_1, result);

    			return 1;
			}
			else
			{
				rs232_print(RS232_PORT_0, "ERROR: mac_tx: data not in hex format.\r\n");
				return 0;
			}
		}
		else
		{
			rs232_print(RS232_PORT_0, "ERROR: mac_tx: portno not between 1 and 223.\r\n");
			return 0;
		}
	}
	else
	{
		rs232_print(RS232_PORT_0, "ERROR: mac_tx: type chosen not cnf or uncnf\n");
		return 0;
	}
}

//TODO: the response - there's two parts
unsigned int mac_tx_reponse_initial(char* data)
{
	//TODO: Magic numbers! Define a number for each possible response (there should only be about 10 in total)
	//response can be one of "ok", "invalid_param", "not_joined", "no_free_ch", "silent", "busy",  invalid_data_len,, frame_counter_err_rejoin_needed or "mac_paused"
	if(strcmp(data,"invalid_param"))
	{
		return 0;
	}
	else if(strcmp(data,"ok"))
	{
		return 1;
	}
	else if(strcmp(data,"not_joined"))
	{
		return 2;
	}
	else if(strcmp(data,"no_free_ch"))
	{
		return 3;
	}
	else if(strcmp(data,"silent"))
	{
		return 4;
	}
	else if(strcmp(data,"busy"))
	{
		return 5;
	}
	else if(strcmp(data,"invalid_data_len"))
	{
		return 7;
	}
	else if(strcmp(data,"frame_counter_err_rejoin_needed"))
	{
		return 8;
	}
	else if(strcmp(data,"mac_paused"))
	{
		return 9;
	}

	//undefined response
	return 0;
}

unsigned int mac_tx_reponse_final(char* data)
{
	if(strcmp(data,"mac_tx_ok"))
	{
		return 1;
	}
	else if(strcmp(data,"mac_err"))
	{
		return 0;
	}
	else if(strcmp(data,"invalid_data_len"))
	{
		return 2;
	}
	else
	{
		//TODO: have received a packet in the format "mac_rx <portno> <data>"
		return 0;
	}
}

unsigned int mac_join(char* mode)
{
	if((strcmp(mode,"otaa")) | (strcmp(mode,"abp")))
	{
		char* command = "mac join ";

		//copy all into one array
		char *result = malloc(strlen(command)+strlen(mode)+3);
    	strcpy(result, command);
    	strcat(result, mode);
    	strcat(result, "\r\n");

    	rs232_print(RS232_PORT_1, result);

    	return 1;
	}
	else
	{
		rs232_print(RS232_PORT_0, "ERROR: mac_join: mode chosen not otaa or abp\n");
		return 0;
	}
}

unsigned int mac_join_response_initial(char* data)
{
	//TODO: Magic numbers!
	//response can be one of "ok", "invalid param", "keys_not_init", "no_free_ch", "silent", "busy", or "mac_paused"
	if(strcmp(data,"invalid_param"))
	{
		return 0;
	}
	else if(strcmp(data,"ok"))
	{
		return 1;
	}
	else if(strcmp(data,"keys_not_init"))
	{
		return 2;
	}
	else if(strcmp(data,"no_free_ch"))
	{
		return 3;
	}
	else if(strcmp(data,"silent"))
	{
		return 4;
	}
	else if(strcmp(data,"busy"))
	{
		return 5;
	}
	else if(strcmp(data,"mac_paused"))
	{
		return 6;
	}

	//undefined response
	return 0;
}

unsigned int mac_join_response_final(char* data)
{
	//response can be "accepted" or "denied"
	if(strcmp(data,"accepted"))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

unsigned int mac_save()
{
	rs232_print(RS232_PORT_1, "mac save\r\n");

	return 1;
}

unsigned int mac_save_response(char* data)
{
	if(strcmp(data,"ok"))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



unsigned int mac_forceENABLE()
{
	rs232_print(RS232_PORT_1, "mac forceENABLE\r\n");

	return 1;
}

unsigned int mac_forceENABLE_response(char* data)
{
	if(strcmp(data,"ok"))
	{
		return 1;
	}
	else
	{
		return 0;
	}	
}


unsigned int mac_pause()
{
	rs232_print(RS232_PORT_1, "mac pause\r\n");

	return 1;
}

unsigned long mac_pause_response(char* data)
{
	return (unsigned long)strtoul(data, NULL, 0);
}

unsigned int mac_resume()
{
	rs232_print(RS232_PORT_1, "mac resume\r\n");

	return 1;
}

unsigned int mac_resume_response(char* data)
{
	if(strcmp(data,"ok"))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}