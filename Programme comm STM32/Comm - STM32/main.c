
#include <windows.h>
#include <stdio.h>

int main()
{
    //long int text_to_send[32]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
    byte text_to_send[16];
    byte text_to_read[16];
    int j=0,i=0;
    char ptext[34];
    char command[] = "G\0",retour[2];

    FILE* ptextfile = fopen("pText.txt","r");



    // Declare variables and structures
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    // Open the highest available serial port number
    fprintf(stderr, "Opening serial port...");

    hSerial = CreateFile(
                "\\\\.\\COM14", GENERIC_READ|GENERIC_WRITE, 0, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
    if (hSerial == INVALID_HANDLE_VALUE)
    {
            fprintf(stderr, "Error\n");
            return 1;
    }
    else fprintf(stderr, "OK\n");


    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hSerial, &dcbSerialParams) == 0)
    {
        fprintf(stderr, "Error getting device state\n");
        CloseHandle(hSerial);
        return 1;
    }

   dcbSerialParams.BaudRate = CBR_57600;
   //dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if(SetCommState(hSerial, &dcbSerialParams) == 0)
    {
        fprintf(stderr, "Error setting device parameters\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Set COM port timeout settings
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if(SetCommTimeouts(hSerial, &timeouts) == 0)
    {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Send specified text (remaining command line arguments)
    for (j=0;j<1000;j++)
    {
        fgets(ptext,sizeof(ptext)+1,ptextfile);
        printf("text = \t%s\n\n",ptext);
        //printf("%s,%d\n\n",temp,sizeof(temp));

        convert(text_to_send,ptext);


        for (i=0;i<16;i++) printf("%d\t%d\n",i,text_to_send[i]);

        DWORD bytes_written, total_bytes_written = 0;

        fprintf(stderr, "Sending bytes...\n");
        printf("G = %c\n",command[0]);
        if(!WriteFile(hSerial, command, 1, &bytes_written, NULL))
        {
            fprintf(stderr, "Error\n");
            CloseHandle(hSerial);
            return 1;
        }
        fprintf(stderr, "%d bytes written\n", bytes_written);

        if(!WriteFile(hSerial, text_to_send, 16, &bytes_written, NULL))
        {
            fprintf(stderr, "Error\n");
            CloseHandle(hSerial);
            return 1;
        }
        fprintf(stderr, "%d bytes written\n", bytes_written);


        if (!ReadFile(hSerial,retour,1,&bytes_written,NULL)) printf("Error reading file !");
        else printf("retour = %c\n",retour[0]);

        if (!ReadFile(hSerial,text_to_read,16,&bytes_written,NULL)) printf("Error reading file !");
        else
        {
            for (i=0;i<16;i++) printf("crypte = %d\n",text_to_read[i]);
        }

        printf("\nj=%d\n",j);
        Sleep(100);

    }

    // Close serial port
    fprintf(stderr, "Closing serial port...");
    if (CloseHandle(hSerial) == 0)
    {
        fprintf(stderr, "Error\n");
        return 1;
    }
    fprintf(stderr, "OK\n");

//     exit normally
    return 0;
}



void convert(byte *dest, char *source )
{
    long int value;
    char temp[3];
    temp[2] = '\0';
    int i;

    for (i=0;i<16;i++)
    {
       memcpy(temp,&source[2*i],2);
       value = strtol(temp,NULL,16);
       dest[i] = (byte)value;
    }
    return;
}
