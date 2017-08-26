#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>

main()
{
    int   i, j;
    int   words = 500;
    int   devmem;
    off_t PageOffset, PageAddress;

    unsigned int *hw_addr = (unsigned int *) 0x20000000;
    unsigned int *mem_pointer;
    unsigned int  read_data;

    devmem = open("/dev/mem", O_RDWR | O_SYNC);
    //PageOffset = (off_t) hw_addr % getpagesize();
    //PageAddress = (off_t) (hw_addr - PageOffset);
    printf("size of unsigned long:%d",sizeof(unsigned long));

    mem_pointer = (unsigned int *) mmap(0, 1024000, PROT_READ|PROT_WRITE, MAP_FILE | MAP_SHARED, devmem, hw_addr);

    if (mem_pointer == MAP_FAILED) {
                       printf("Failed to creating mapping.\n");
                      // printf("ERRNO: %s\n", strerror(errno));
                       return -1;
               }

   printf("Writing to hardware model\n");
    for (i = 0, j = 0; i < words; i++, j+=4)
    {
        *(mem_pointer+i) = 0xabcd0000+j;
        read_data = *(mem_pointer+i);
    }
    printf("Read from hardware address %x, is %x\n",mem_pointer,read_data);

}
