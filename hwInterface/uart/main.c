#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "hw_odor.h"
#include "hw_radar.h"
#include "hw_temp.h"


void odor();
void radar();
void temp();
void *readOdor();

int main()
{
    int ch , running = 1;
    printf("1.ODOR 2.RADAR 3.TEMP 4.EXIT\n");
    scanf("%d" , &ch);

AGAIN:
    while(running)
    {
        switch(ch)
        {
            case 1:
                odor();
            break;

            case 2:
                radar();
            break;

            case 3:
                temp();
            break;

            case 4:
                running = 0;
            break;

            default:
                printf("invalide choice\n");
                goto AGAIN;
            break;
        }
    }
    return 0;
}



void odor()
{
    
    int ret = ODOR_fnInit(2400 , "/dev/ttyCH9344USB6");

    if(ret != 0)
    {
        printf("failed to init odor \n");
    }
    pthread_t thread;
    pthread_create(&thread , NULL , readOdor , NULL);
    pthread_join(thread , NULL);
   


}

void *readOdor()
{
    st_Odor st_Odor_t;
    int i = 0;

    while(i < 5)
    {
        ODOR_fnReadData(&st_Odor_t);
        sleep(1);
        i++;
    }

    ODOR_fnClose();
    
}

void radar()
{
    int i = 0;
    int ret = HW_RADAR_fnInit("/dev/ttyCH9344USB5" , 115200);
    if(ret != 0)
    {
        printf("failed to init radar\n");
    }

    while(i < 5)
    {
        int ret = HW_RADAR_fnReadData();
        sleep(1);
        i++;
    }

    HW_RADAR_fnCloseUart();
}

void temp()
{
    HW_DS18B20_fnInitSensor("/dev/ds18b20");

    int i;
    for(i=0;i<10;i++)
    {
    
        float result=0;
        HW_DS18B20_fnReadTempVal(&result);
        sleep(1);
        printf("temp is %f\n" , result);
    }
  
    HW_DS18B20_fnCloseSensor();

}