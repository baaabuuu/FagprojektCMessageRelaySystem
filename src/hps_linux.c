#define soc_cv_av
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <math.h>


#include "../inc/hps_soc_system.h"
#include "../inc/hps_linux.h"
//#include "hwlib.h"
//#include "socal/alt_gpio.h"
//#include "socal/hps.h"
//#include "../inc/socal.h"
//#include "alt_generalpurpose_io.h"


// memory-mapped peripherals
//void   *hps_gpio     = NULL;
//size_t hps_gpio_span = ALT_GPIO1_UB_ADDR - ALT_GPIO1_LB_ADDR + 1;
//size_t hps_gpio_ofst = ALT_GPIO1_OFST;

void   *h2f_lw_axi_master     = NULL;
size_t h2f_lw_axi_master_span = ALT_FPGA_BRIDGE_LWH2F_UB_ADDR - ALT_FPGA_BRIDGE_LWH2F_LB_ADDR + 1;
size_t h2f_lw_axi_master_ofst = ALT_FPGA_BRIDGE_LWH2F_OFST;

void 	*h2f_axi_master  	= NULL;
size_t h2f_axi_master_span = ALT_FPGA_BRIDGE_H2F128_UB_ADDR- ALT_FPGA_BRIDGE_H2F128_LB_ADDR + 1;
size_t h2f_axi_master_ofst = ALT_FPGA_BRIDGE_H2F128_OFST;


int fd_dev_mem = 0;


void *fpga_leds = NULL;
void *fpga_switches = NULL;
void *fpga_command = NULL;
volatile unsigned int * *hps_to_fpga_fifo = NULL;
//void *hps_to_fpga_fifo = NULL;
volatile uint32_t *hps_to_fpga_fifo_status = NULL;
void *fpga_to_hps_fifo = NULL;
volatile uint32_t *fpga_to_hps_fifo_status = NULL;

volatile unsigned int * FIFO_transmit_ptr = NULL ;
volatile unsigned int * FIFO_receive_ptr = NULL ;
volatile unsigned int * FIFO_transmit_status_ptr = NULL ;
volatile unsigned int * FIFO_receive_status_ptr = NULL ;
volatile void * FIFO_ctrl_ptr = NULL;
volatile unsigned int * FIFO_ctrl_status_ptr = NULL;

#define FIFO_WRITE		     (*(FIFO_transmit_ptr))
#define FIFO_READ		     (*(FIFO_receive_ptr))
#define FIFO_CTRL			 (*(uint16_t *) (FIFO_ctrl_ptr))
#define WRITE_FIFO_FILL_LEVEL	  (*FIFO_transmit_status_ptr) 
#define READ_FIFO_FILL_LEVEL	  (*FIFO_receive_status_ptr) 
#define CTRL_FIFO_FILL_LEVEL 	  (*FIFO_ctrl_status_ptr)
#define WRITE_FIFO_FULL		  ((*(FIFO_transmit_status_ptr+1))& 1 ) 
#define WRITE_FIFO_EMPTY	  ((*(FIFO_transmit_status_ptr+1))& 2 ) 
#define READ_FIFO_FULL		  ((*(FIFO_receive_status_ptr+1)) & 1 )
#define READ_FIFO_EMPTY	      ((*(FIFO_receive_status_ptr+1)) & 2 )
#define CTRL_FIFO_FULL		  ((*(FIFO_ctrl_status_ptr+1)) & 1 )
#define CTRL_FIFO_EMPTY	      ((*(FIFO_ctrl_status_ptr+1)) & 2 )




/******************************************************************************
 * We need to access the system's physical memory so we can map it to user    *
 * space. We will use the /dev/mem file to do this. /dev/mem is a character   *
 * device file that is an image of the main memory of the computer. Byte      *
 * addresses in /dev/mem are interpreted as physical memory addresses.        *
 * Remember that you need to execute this program as ROOT in order to have    *
 * access to /dev/mem.														  *
 *****************************************************************************/
void open_physical_memory_device() {


    fd_dev_mem = open("/dev/mem", O_RDWR | O_SYNC);
    if(fd_dev_mem  == -1) {
        printf("ERROR: could not open \"/dev/mem\".\n");
        printf("    errno = %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/********************************
 * Terminates it, freeing up mem*
 *******************************/
void close_physical_memory_device() {
    close(fd_dev_mem);
}

/********************************
 * Memory maps the peripherals  *
 *******************************/
void mmap_fpga_peripherals() {
    h2f_lw_axi_master = mmap(NULL, h2f_lw_axi_master_span, PROT_READ | PROT_WRITE, MAP_SHARED, fd_dev_mem, h2f_lw_axi_master_ofst);
    if (h2f_lw_axi_master == MAP_FAILED) {
        printf("[hps_linux][mmap_fpga_peripherals] failed.\n");
        printf("    errno = %s\n", strerror(errno));
        close(fd_dev_mem);
        exit(EXIT_FAILURE);
    }
	h2f_axi_master = mmap(NULL, h2f_axi_master_span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd_dev_mem, h2f_axi_master_ofst);
    if (h2f_axi_master == MAP_FAILED) {
        printf("[hps_linux][mmap_fpga_peripherals] failed.\n");
        printf("[hps_linux][mmap_fpga_peripherals] errno = %s\n", strerror(errno));
        close(fd_dev_mem);
        exit(EXIT_FAILURE);
    }
	FIFO_transmit_ptr = (unsigned int *) (h2f_axi_master + FIFO_HPS_TO_FPGA_IN_BASE);
	FIFO_receive_ptr  = (unsigned int *) (h2f_axi_master + FIFO_FPGA_TO_HPS_OUT_BASE);
	FIFO_ctrl_ptr  = (unsigned int *) (h2f_axi_master + CTRL_FIFO_TO_HPS_OUT_BASE);
	FIFO_transmit_status_ptr = (unsigned int *)(h2f_lw_axi_master +  FIFO_HPS_TO_FPGA_IN_CSR_BASE);
	FIFO_receive_status_ptr = (unsigned int *)(h2f_lw_axi_master +  FIFO_FPGA_TO_HPS_OUT_CSR_BASE);
	FIFO_ctrl_status_ptr = (unsigned int *)(h2f_lw_axi_master +  CTRL_FIFO_TO_HPS_OUT_CSR_BASE);
	
	
	
    fpga_leds = h2f_lw_axi_master + HPS_FPGA_LEDS_BASE;
	fpga_switches = h2f_lw_axi_master + HPS_FPGA_SWITCHES_BASE;
	fpga_command = h2f_lw_axi_master + CTRL_FROM_HPS_BASE;
//	hps_to_fpga_fifo = h2f_axi_master + FIFO_HPS_TO_FPGA_IN_BASE;
//	hps_to_fpga_fifo_status = h2f_lw_axi_master + FIFO_HPS_TO_FPGA_IN_CSR_BASE;
//	fpga_to_hps_fifo = h2f_axi_master + FIFO_FPGA_TO_HPS_OUT_BASE;
//	fpga_to_hps_fifo_status = h2f_lw_axi_master + FIFO_FPGA_TO_HPS_OUT_CSR_BASE;
}

/********************************
 * Unmemory maps the peripherals*
 *******************************/
void munmap_fpga_peripherals() {
    if (munmap(h2f_lw_axi_master, h2f_lw_axi_master_span) != 0) {
        printf("[hps_linux][munmap_fpga_peripherals] h2f_lw_axi_master munmap() failed\n");
        printf("    errno = %s\n", strerror(errno));
        close(fd_dev_mem);
        exit(EXIT_FAILURE);
    }
	if (munmap(h2f_axi_master, h2f_axi_master_span) != 0) {
        printf("[hps_linux][munmap_fpga_peripherals] h2f_lw_axi_master munmap() failed\n");
        printf("    errno = %s\n", strerror(errno));
        close(fd_dev_mem);
        exit(EXIT_FAILURE);
    }
    h2f_lw_axi_master = NULL;
	h2f_axi_master    = NULL;
    fpga_leds         = NULL;
	fpga_switches	  = NULL;
		//hps_to_fpga_fifo = NULL;
	//hps_to_fpga_fifo_status = NULL;
	//fpga_to_hps_fifo = NULL;
	//fpga_to_hps_fifo_status = NULL;
	FIFO_transmit_ptr = NULL;
	FIFO_receive_ptr  = NULL;
	FIFO_ctrl_ptr  = NULL;
	FIFO_transmit_status_ptr = NULL;
	FIFO_receive_status_ptr = NULL;
	FIFO_ctrl_status_ptr = NULL;
}

/*************************************************
 * Allows for the FPGA to interface with the C   *
 * Needs to be called at startup with ROOT access*
 ************************************************/
void setupFPGA()
{
	printf("[hps_linux][setupFPGA] DE1-SoC setting up\n");
	open_physical_memory_device();
    mmap_fpga_peripherals();
}

/***************************************
 * Checks if the video FIFO is not full*
 **************************************/
int canWriteVideo()
{
	return !WRITE_FIFO_FULL;
}
/**************************************
 * Writes an integer to the video FIFO*
 *************************************/
void writeVideo(int value)
{
	FIFO_WRITE = value;
}

/****************************************
 * Checks if the video FIFO is not empty*
 ***************************************/
int canReadVideo()
{
	return !READ_FIFO_EMPTY;
}

/*******************************************
 * Returns a value from video FIFO         *
 * Henrik claims to be 16 bit 			   *
 ******************************************/
uint16_t readVideoFifo()
{
	return  (uint16_t) FIFO_READ;
}

/****************************************
 * Checks if the video FIFO is not empty*
 ***************************************/
int canReadControl()
{
	return !CTRL_FIFO_EMPTY;
}

/*******************************************
 * Returns a value from control FIFO       *
 ******************************************/
uint16_t readControl()
{
	return FIFO_CTRL;
}

/*******************************************
 * Updates modulation                      *
 ******************************************/
void writeMod(int value)
{
	*(uint16_t *) fpga_command = value;
}

int readSWN(int switchValue)
{
	switch (switchValue)
	{
		case 0:
			return (*(uint16_t*) fpga_switches & 1) != 0;
		case 1:
			return (*(uint16_t*) fpga_switches & 2) != 0;
		case 2:
			return (*(uint16_t*) fpga_switches & 4) != 0;
		case 3:
			return (*(uint16_t*) fpga_switches & 8) != 0;
		case 4:
			return (*(uint16_t*) fpga_switches & 16) != 0;
		case 5:
			return (*(uint16_t*) fpga_switches & 32) != 0;
		case 6:
			return (*(uint16_t*) fpga_switches & 64) != 0;
		case 7:
			return (*(uint16_t*) fpga_switches & 128) != 0;
		case 8:
			return (*(uint16_t*) fpga_switches & 256) != 0;
		case 9:
			return (*(uint16_t*) fpga_switches & 512) != 0;
		default:
			return 0;
	}
	return 0;
}

int readLEDRN(int ledValue)
{
	switch (ledValue)
	{
		case 0:
			return (*(uint16_t*) fpga_leds & 1) != 0;
		case 1:
			return (*(uint16_t*) fpga_leds & 2) != 0;
		case 2:
			return (*(uint16_t*) fpga_leds & 4) != 0;
		case 3:
			return (*(uint16_t*) fpga_leds & 8) != 0;
		case 4:
			return (*(uint16_t*) fpga_leds & 16) != 0;
		case 5:
			return (*(uint16_t*) fpga_leds & 32) != 0;
		case 6:
			return (*(uint16_t*) fpga_leds & 64) != 0;
		case 7:
			return (*(uint16_t*) fpga_leds & 128) != 0;
		case 8:
			return (*(uint16_t*) fpga_leds & 256) != 0;
		case 9:
			return (*(uint16_t*) fpga_leds & 512) != 0;
		default:
			return 0;
	}
	return 0;
}

void setLEDRN(int ledValue)
{
	*(uint16_t *) fpga_leds = ledValue;
}


int temp_main() {
    int scanok;
	unsigned int N;
	printf("DE1-SoC linux demo\n");
	setupFPGA();
    
	char c;
	int exit = 0;
	while(!exit) 
	{
		printf("\n\rChoose option (W Write Data to FIFO, R Read Data from FIFO, S Status of FIFO, L Write to LED, H Help, Q Quit) ");
		scanok = scanf(" %c", &c);
		switch (c)
		{
			case 'W': 
				printf("\n\r Write data to FIFO \n\r Enter value between 0 and 65535: "); 
				scanok = scanf("%d",&N);
				printf("\n\r Writing %d to FIFO\n",N);
				if (!WRITE_FIFO_FULL) {FIFO_WRITE=N;};
				break;
			case 'R': 
				printf("%s\n","Read data from Receive FIFO"); 
				if (!READ_FIFO_EMPTY) {printf("Returned value %d: ",FIFO_READ);};
				printf("%s\n","Read data from Control FIFO"); 
				if (!CTRL_FIFO_EMPTY) {printf("Returned value %d: ",(int16_t) FIFO_CTRL);};
				break;
				
			case 'L':
				printf("Enter value between 0 and 1023 for LED\n"); scanok = scanf("%d",&N);
				*(uint16_t *) fpga_leds = N;
				printf("%d\n", *(uint16_t *) fpga_leds);
				break;
			case 'Q': 
				printf("%s\n","Quit - Goodbye"); 
				exit = 1;
				break;
			case 'F':
				printf("Filling 250 values to fifo incrementing from 1\n");
				for (int k=0;k<250;k++) {if (!WRITE_FIFO_FULL) {FIFO_WRITE=k;};};
				break;
			case 'E': 
				printf("Emptying FIFO\n");
				while(!READ_FIFO_EMPTY) {printf("Read value: %d\n",FIFO_READ);};
				break;
			case 'S': 
				printf("%s\n\r","Status update"); 
				printf("Transmit FIFO Empty value %d \n", WRITE_FIFO_EMPTY);
				printf("Transmit FIFO Full value %d \n", WRITE_FIFO_FULL);
				printf("Transmit FIFO fill level %d \n", WRITE_FIFO_FILL_LEVEL);
				printf("Receive FIFO Empty value %d \n", READ_FIFO_EMPTY);
				printf("Receive FIFO Full value %d \n", READ_FIFO_FULL);
				printf("Receive FIFO fill level %d \n", READ_FIFO_FILL_LEVEL);		
				printf("Ctrl FIFO Empty value %d \n", CTRL_FIFO_EMPTY);
				printf("Ctrl FIFO Full value %d \n", CTRL_FIFO_FULL);
				printf("Ctrl FIFO fill level %d \n", CTRL_FIFO_FILL_LEVEL);		
				break;
			case 'H': printf("Helpdesk\nUse KEYS depending on chosen hardware to move data from WRITE to READ FIFO\nKEY_0 reset FIFOs\nKEY_3 act as clock for FIFOs\nSW(0) read enable Write FIFO\nSW(1) Write Enable Read FIFO\nData from Write FIFO connected to READ FIFO "); break;
		};
	};
    
    munmap_fpga_peripherals();
    close_physical_memory_device();

    return 0;
}
