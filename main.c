#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

// GPIO1 0x4804_C000
#define GPIO_SETDATAOUT1 0x4804C000 + 0x194
#define GPIO_CLEARDATAOUT1 0x4804C000 + 0x190
#define GPIO_DATAOUT1 0x4804C000 + 0x13C
#define LED_PIN 1 << 22 //usr1 

void *map_perif(void *addr, int fd){
    int page = getpagesize();
    off_t offset = ((off_t)addr/page)*page;
    int dif = (off_t)addr-offset;

    void *dir_map = mmap(NULL, 4, PROT_READ| PROT_WRITE, MAP_SHARED, fd, offset); 
    if ( (int)dir_map == -1){
        printf("mmap failed (errno %d)\n", errno);
        exit(1);
    }
    return dir_map + dif;
}

int main(){
    printf("Hello world, i am a pocket beagle\n");

    int fd = open("/dev/mem", O_RDWR);
    if (fd == -1){
        printf("error %d al abrir el fichero\n", errno);
        return -1;
    }
    printf("Se ha abierto el archivo /dev/mem\n");
    uint32_t *gpio_set = map_perif((void *)GPIO_SETDATAOUT1, fd); 
    uint32_t *gpio_clr = map_perif((void *)GPIO_CLEARDATAOUT1, fd); 
    uint32_t *gpio_data = map_perif((void *)GPIO_DATAOUT1, fd); 
    printf("Se tiene acceso a todos los registros en VMA\n");

    printf("gpio1 content is %x\n",  *gpio_data);

    while(1){
        *gpio_set = LED_PIN;
        printf("se acaba de encender el LED\n");
        sleep(1);
        *gpio_clr = LED_PIN;
        printf("se acaba de APAGAR el LED\n");
        sleep(1);
    }

    return 0;
}
