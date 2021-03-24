#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

char wbuf[64],buf[64],prevSeconds;
uint8_t slave_address = 0x00;
uint32_t len = 0;
uint8_t data;
char r1[100] = "RECEIVER> Record 1:";
char r2[100] = "RECEIVER> Record 2:";
char r3[100] = "RECEIVER> Record 3:";
char out1[100],out2[100],out3[100];

void getDate(){
    slave_address = 0x68;
    bcm2835_i2c_setSlaveAddress(slave_address);
    data = bcm2835_i2c_read(buf, 64);
    }

void showDate(){
    getDate();
    strcpy(out3,out2);
    strcpy(out2,out1);
    char day[4];
    switch(buf[3]){
        case 0x01:
            strcpy(day,"Mon");
        break;
        case 0x02:
            strcpy(day,"Tue");
        break;
        case 0x03:
            strcpy(day,"Wed");
        break;
        case 0x04:
            strcpy(day,"Thu");
        break;
        case 0x05:
            strcpy(day,"Fri");
        break;
        case 0x06:
            strcpy(day,"Sat");
        break;
        case 0x07:
            strcpy(day,"Sun");
        break;
        default:
            strcpy(day,"Err");
        break;
    }
    snprintf(out1,100, "%x%x/%x%x/%x%x %c%c%c %x%x:%x%x:%x%x ",buf[4]>>4,buf[4]&0x0f,buf[5]>>4,buf[5]&0x0f,buf[6]>>4,buf[6]&0x0f,day[0],day[1],day[2],(buf[2]&0x01>>4),buf[2]&0x0f,buf[1]>>4,buf[1]&0x0f,(buf[0]>>4)&0x07,buf[0]&0x0f);
    buf[2]>>4 >=2 ? strcat(out1,"PM"):strcat(out1,"AM");
    }
void setDate(){
    slave_address = 0x68;
    bcm2835_i2c_setSlaveAddress(slave_address);
    //Escribir
    wbuf[0]=0x01;
    wbuf[1]=0x01;
    wbuf[2]=0x77;
    wbuf[3]=0x01;
    wbuf[4]=0x01;
    wbuf[5]=0x01;
    wbuf[6]=0x01;
    data = bcm2835_i2c_write(wbuf, 64);
    }
void printDate(){
    getDate();
    printf("%x%x/",buf[4]>>4,buf[4]&0x0f);
    printf("%x%x/",buf[5]>>4,buf[5]&0x0f);
    printf("%x%x ",buf[6]>>4,buf[6]&0x0f);
    switch(buf[3]){
        case 0x01:
            printf("Mon ");
        break;
        case 0x02:
            printf("Tue ");
        break;
        case 0x03:
            printf("Wed ");
        break;
        case 0x04:
            printf("Thu ");
        break;
        case 0x05:
            printf("Fri ");
        break;
        case 0x06:
            printf("Sat ");
        break;
        case 0x07:
            printf("Sun ");
        break;
        default:
            printf("Error ");
        break;
    }
    printf("%x%x:",(buf[2]>>4)&0x01,buf[2]&0x0f);
    printf("%x%x:",buf[1]>>4,buf[1]&0x0f);
    printf("%x%x ",(buf[0]>>4)&0x07,buf[0]&0x0f);
    buf[2]>>4 >=2 ? printf("PM\n"):printf("AM\n");
    
        
    
    }
    
int getTemp(){
    bcm2835_i2c_setSlaveAddress(0x4d);
    buf[0]=0x00;
    data = bcm2835_i2c_read(buf, 1);
    printf("%d grads\n",buf[0]);
    return (int)buf[0];
    }
int main(int argc, char **argv) {
    if(!bcm2835_init())
        return 1;
    bcm2835_i2c_begin();
    bcm2835_i2c_set_baudrate(10000);
    int temp,counter = 0;
    
    while(1){
    setDate();
    temp = getTemp();
    if(temp >30 || counter == 3){
    showDate();
    FILE *file = fopen("archivo.txt","w");
    fprintf(file,"%s%s\n",r1,out1);
    fprintf(file,"%s%s\n",r2,out2);
    fprintf(file,"%s%s\n",r3,out3);
    printf("%s%s\n",r1,out1);
    printf("%s%s\n",r2,out2);
    printf("%s%s\n",r3,out3);
    fclose(file);
    }
    sleep(3);
    if(counter ==3)
        counter = 0;
    else
        counter = counter +1;
}
    bcm2835_i2c_end();
    bcm2835_close();
    return 0;
}

