#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define GPIO_SEL0 0X3F200000
#define GPIO_SET0 0X3F20001C
#define GPIO_CLR0 0X3F200028
#define LED_PIN 1 << 3 
// Pines 11-9, FSEL3 = 001 -> pin as output

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
    printf("Hello world, i am a rpi\n");

    int fd = open("/dev/mem", O_RDWR);
    if (fd == -1){
        printf("error %d al abrir el fichero\n", errno);
        return -1;
    }
    printf("Se ha abierto el archivo /dev/mem\n");
    uint32_t *gpio_sel = map_perif((void *)GPIO_SEL0, fd); 
    uint32_t *gpio_set = map_perif((void *)GPIO_SET0, fd); 
    uint32_t *gpio_clr = map_perif((void *)GPIO_CLR0, fd); 
    printf("Se tiene acceso a todos los registros en VMA\n");
    printf("\t gpio_sel esta en la dirreccion virtual: %x\n",(unsigned int)gpio_sel);

    printf("gpio sel content is %x\n",  *gpio_sel);
    *gpio_sel = *gpio_sel & ~(0x7 << 9);
    *gpio_sel = *gpio_sel | (0x1 << 9);

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
