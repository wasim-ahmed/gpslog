#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>

#include "serial.h"

using namespace std;

int uart0_filestream = -1;

void serial_init(void)
{
    uart0_filestream = open(PORTNAME, O_RDWR | O_NOCTTY | O_NDELAY);

    if (uart0_filestream == -1)
    {
        //TODO error handling...
    	cout<<"Error in opening Serial Device"<<endl;
    	cout<<__FUNCTION__<<endl;
    }
    else
    {
    	cout<<"Device:"<<PORTNAME<<"opened successfully"<<endl;
    }
}

void serial_config(void)
{
    int rv;
	struct termios options;
    if((rv = tcgetattr(uart0_filestream, &options)) == 0)
    {
    	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    	options.c_iflag = IGNPAR;
    	options.c_oflag = 0;
    	options.c_lflag = 0;

    	if((rv = tcflush(uart0_filestream, TCIFLUSH)) == 0)
    	{
    		if((rv = tcsetattr(uart0_filestream, TCSANOW, &options)) == 0)
    		{
    			cout<<"Serial Device Configuration Completed"<<endl;

    		}
    		else
    		{
            	cout<<"Error in setting terminal configuration parameter: "<<errno<<endl;
            	cout<<__FUNCTION__<<endl;

            	string error = strerror(errno);
            	if(error == "EBADF")
            	{
            		cout<<"The fildes argument is not a valid file descriptor"<<endl;
            	}
            	else if(error == "ENOTTY")
            	{
            		cout<<"The file associated with fildes is not a terminal."<<endl;
            	}
            	else if(error == "EINVAL")
            	{
            		cout<<"The optional_actions argument is not a supported value, "
            				"or an attempt was made to change an attribute represented in the termios "
            				"structure to an unsupported value"<<endl;
            	}
            	else if(error == "EINTR")
                {
                	cout<<"A signal interrupted tcsetattr()."<<endl;
                }
            	else
        		{
            		cout<<"Strange error"<<endl;
        		}

    		}
    	}
    	else
    	{
        	cout<<"Error in flushing ternminal data: "<<errno<<endl;
        	cout<<__FUNCTION__<<endl;

        	string error = strerror(errno);
        	if(error == "EBADF")
        	{
        		cout<<"The fildes argument is not a valid file descriptor"<<endl;
        	}
        	else if(error == "ENOTTY")
        	{
        		cout<<"The file associated with fildes is not a terminal."<<endl;
        	}
        	else if(error == "EINVAL")
        	{
        		cout<<"The queue_selector argument is not a supported value."<<endl;
        	}
        	else
    		{
        		cout<<"Strange error"<<endl;
    		}
    	}
    }
    else
    {
    	cout<<"Error in retrieving the terminal attributes: "<<errno<<endl;
    	cout<<__FUNCTION__<<endl;

    	string error = strerror(errno);
    	if(error == "EBADF")
    	{
    		cout<<"The fildes argument is not a valid file descriptor"<<endl;
    	}
    	else if(error == "ENOTTY")
    	{
    		cout<<"The file associated with fildes is not a terminal."<<endl;
    	}
    	else
		{
    		cout<<"Strange error"<<endl;
		}
    }

}

void serial_println(const char *line, int len)
{
    if (uart0_filestream != -1) {
        char *cpstr = (char *)malloc((len+1) * sizeof(char));
        strcpy(cpstr, line);
        cpstr[len-1] = '\r';
        cpstr[len] = '\n';

        int count = write(uart0_filestream, cpstr, len+1);
        if (count < 0) {
            //TODO: handle errors...
        }
        free(cpstr);
    }
}

// Read a line from UART.
// Return a 0 len string in case of problems with UART
void serial_readln(char *buffer, int len)
{
    char c;
    char *b = buffer;
    int rx_length = -1;
    while(1) {
        rx_length = read(uart0_filestream, (void*)(&c), 1);

        if (rx_length <= 0) {
            //wait for messages
        	if (rx_length <0)
        	{
        		cout<<"Error in Reading data: "<<errno<<endl;
        		cout<<"-------------";
        		cout<<__FUNCTION__;
        		cout<<"-------------";
        		cout<<endl;

				string error = strerror(errno);
				if(error == "EAGAIN")
				{
					cout<<"The file descriptor fd refers to a file other than a socket and "
							"has been marked nonblocking (O_NONBLOCK), and the read would block."<<endl;
				}
				else if(error == "EBADF")
				{
					cout<<"fd is not a valid file descriptor or is not open for reading."<<endl;
				}
				else if(error == "EFAULT")
				{
					cout<<"buf is outside your accessible address space."<<endl;
				}
				else if(error == "EINTR")
				{
					cout<<"The call was interrupted by a signal before any data was read; see signal(7)"<<endl;
				}
				else if(error == "EINVAL")
				{
					cout<<"fd is attached to an object which is unsuitable for reading; or the file was opened "
							"with the O_DIRECT flag, and either the address specified in buf, the value specified in count,"
							" or the current file offset is not suitably aligned."<<endl;
					cout<<"---------------------------------------------------------------------------"<<endl;
					cout<<"fd was created via a call to timerfd_create(2) and the wrong size buffer was given to read(); "
							"see timerfd_create(2) for further information."<<endl;
				}
				else if(error == "EIO")
				{
					cout<<"I/O error. This will happen for example when the process is in a background process group, "
							"tries to read from its controlling terminal, and either it is ignoring or blocking SIGTTIN or "
							"its process group is orphaned. It may also occur when there is a low-level I/O error while reading"
							" from a disk or tape."<<endl;
				}
				else if(error == "EISDIR")
				{
					cout<<"fd refers to a directory."<<endl;
				}
				else if(error == "EAGAIN")
				{
					cout<<"No Data available currently Try Again."<<endl;
				}
				else
				{
					cout<<"Strange error, number:"<<error<<endl;
				}
				sleep(1);
        	}
        	else
        	{
        		sleep(1);
        	}

        } else {
            if (c == '\n') {
                *b++ = '\0';
                break;
            }
            *b++ = c;
        }
    }
}

void serial_close(void)
{
    close(uart0_filestream);
}

