#include "gc9a01.h"
#include "gc9a01_if.h"


// Command codes:
#define COL_ADDR_SET        0x2A
#define ROW_ADDR_SET        0x2B
#define MEM_WR              0x2C
#define COLOR_MODE          0x3A
#define COLOR_MODE__12_BIT  0x03
#define COLOR_MODE__16_BIT  0x05
#define COLOR_MODE__18_BIT  0x06
#define MEM_WR_CONT         0x3C

/* Regulative Command Set */
#define Read_Display_Identification_Information_2 0x04
#define Read_Display_Status             0x09
#define Enter_Sleep_Mode	            0x10
#define Exit_Sleep_Mode		            0x11
#define Partial_Display_Mode_On	        0x12
#define Normal_Display_Mode_On	        0x13
#define Display_Inversion_Off	        0x20
#define Display_Inversion_On	        0x21
#define Display_Off			            0x28
#define Display_On			            0x29
#define Column_Address_Set	            0x2A
#define Page_Address_Set	            0x2B
#define Memory_Write		            0x2C
#define Partial_Area		            0x30
#define Vertical_Scrolling_Definition	0x33
#define Tearing_Effect_Line_Off	        0x34
#define Tearing_Effect_Line_On	        0x35
#define Memory_Access_Control	        0x36
#define Vertical_Scrolling_Start_Address	0x37
#define Idle_Mode_Off	                0x38
#define Idle_Mode_On	                0x39
#define Pixel_Format_Set	            0x3A
#define Write_Memory_Continue	        0x3C
#define Set_Tear_Scanline	            0x44
#define Get_Scanline	                0x45
#define Write_Display_Brightness	    0x51
#define Write_Ctrl_Display	            0x53
#define Read_ID1	                    0xDA
#define Read_ID2	                    0xDB
#define Read_ID3	                    0xDC

/* Extended Command Set  */
#define RGB_Interface_Signal_Control    0xB0
#define Blanking_Porch_Control          0xB5
#define Display_Function_Control        0xB6
#define TE_Control                      0xBA
#define Interface_Control               0xF6

/* Inter Command Set */
#define Power_Criterion_Control         0xC1
#define Vcore_Voltage_Control           0xA7
#define Vreg1a_Voltage_Control          0xC3
#define Vreg1b_Voltage_Control          0xC4
#define Vreg2a_Voltage_Control          0xC9
#define Frame_Rate                      0xE8
#define SPI_2data_Control               0xE9
#define Charge_Pump_Frequent_Control    0xEC
#define Inner_Register_Enable_1         0xFE
#define Inner_Register_Enable_2         0xEF
#define SET_GAM_MA1                     0xF0
#define SET_GAM_MA2                     0xF1
#define SET_GAM_MA3                     0xF2
#define SET_GAM_MA4                     0xF3


/*
The display data RAM stores display dots and consists of 1,382,400 bits (320x18x240 bits). There is no
restriction on access to the RAM even when the display data on the same address is loaded to DAC. There will
be no abnormal visible effect on the display when there is a simultaneous Panel Read and Interface Read or
Write to the same location of the Frame Memory.

Every pixel (18-bit) data in GRAM is located by a (Page, Column) address (Y, X). By specifying the arbitrary
window address SC, EC bits and SP, EP bits, it is possible to access the GRAM by setting RAMWR or
RAMRD commands from start positions of the window address.

GRAM address for display panel position as shown in the following table
(00,00)h (00,01)h …… …  (00, 13D)h (00, 13E)h (00,13F)h
(01,00)h (01,01)h …… …   (01, 13D)h (01, 13E)h (01, 13F)h
...
(EF,00)h (EF,01)h …… …   (EF, 13D)h (EF, 13E)h (EF, 13F)h

*/

static void gc9a01_Writ_Byte(uint8_t dat) {
    gc9a01_if_transmit(&dat, 1);
}

static void gc9a01_Writ_Halfword(uint16_t dat) {
    uint8_t buf[2];
    buf[0] = (uint8_t)(dat >> 8);
    buf[1] = (uint8_t)dat;
    gc9a01_if_transmit(buf, sizeof(buf));
}

static void LCD_WR_REG(uint8_t dat) {
    gc9a01_if_select_transmit_command_mode(); // 写命令
    gc9a01_Writ_Byte(dat);
    gc9a01_if_select_transmit_data_mode(); // 写数据
}

static void gc9a01_Set_Address(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    LCD_WR_REG(Column_Address_Set); // 列地址设置
    gc9a01_Writ_Halfword(x0);
    gc9a01_Writ_Halfword(x1);
    LCD_WR_REG(Page_Address_Set); // 行地址设置
    gc9a01_Writ_Halfword(y0);
    gc9a01_Writ_Halfword(y1);
    LCD_WR_REG(Memory_Write); // 储存器写
}

int gc9a01_init(void) {

    gc9a01_if_init();
    
    /* IC初始化 */

    LCD_WR_REG(0xEF);
    LCD_WR_REG(0xEB);
    gc9a01_Writ_Byte(0x14);

    LCD_WR_REG(0xFE);
    LCD_WR_REG(0xEF);

    LCD_WR_REG(0xEB);
    gc9a01_Writ_Byte(0x14);

    LCD_WR_REG(0x84);
    gc9a01_Writ_Byte(0x40);

    LCD_WR_REG(0x85);
    gc9a01_Writ_Byte(0xFF);

    LCD_WR_REG(0x86);
    gc9a01_Writ_Byte(0xFF);

    LCD_WR_REG(0x87);
    gc9a01_Writ_Byte(0xFF);

    LCD_WR_REG(0x88);
    gc9a01_Writ_Byte(0x0A);

    LCD_WR_REG(0x89);
    gc9a01_Writ_Byte(0x21);

    LCD_WR_REG(0x8A);
    gc9a01_Writ_Byte(0x00);

    LCD_WR_REG(0x8B);
    gc9a01_Writ_Byte(0x80);

    LCD_WR_REG(0x8C);
    gc9a01_Writ_Byte(0x01);

    LCD_WR_REG(0x8D);
    gc9a01_Writ_Byte(0x01);

    LCD_WR_REG(0x8E);
    gc9a01_Writ_Byte(0xFF);

    LCD_WR_REG(0x8F);
    gc9a01_Writ_Byte(0xFF);

    LCD_WR_REG(0xB6);
    gc9a01_Writ_Byte(0x00);
    gc9a01_Writ_Byte(0x20);

    LCD_WR_REG(0x36);
    if (USE_HORIZONTAL == 0)
        gc9a01_Writ_Byte(0x08);
    else if (USE_HORIZONTAL == 1)
        gc9a01_Writ_Byte(0xC8);
    else if (USE_HORIZONTAL == 2)
        gc9a01_Writ_Byte(0x68);
    else
        gc9a01_Writ_Byte(0xA8);

    LCD_WR_REG(Pixel_Format_Set);
    gc9a01_Writ_Byte(0x55); // DPI:=16 bits / pixel; DBI:=16 bits / pixel

    LCD_WR_REG(0x90);
    gc9a01_Writ_Byte(0x08);
    gc9a01_Writ_Byte(0x08);
    gc9a01_Writ_Byte(0x08);
    gc9a01_Writ_Byte(0x08);

    LCD_WR_REG(0xBD);
    gc9a01_Writ_Byte(0x06);

    LCD_WR_REG(0xBC);
    gc9a01_Writ_Byte(0x00);

    LCD_WR_REG(0xFF);
    gc9a01_Writ_Byte(0x60);
    gc9a01_Writ_Byte(0x01);
    gc9a01_Writ_Byte(0x04);

    LCD_WR_REG(Vreg1a_Voltage_Control);
    gc9a01_Writ_Byte(0x13);
    LCD_WR_REG(Vreg1b_Voltage_Control);
    gc9a01_Writ_Byte(0x13);
    LCD_WR_REG(Vreg2a_Voltage_Control);
    gc9a01_Writ_Byte(0x22);

    LCD_WR_REG(0xBE);
    gc9a01_Writ_Byte(0x11);

    LCD_WR_REG(0xE1);
    gc9a01_Writ_Byte(0x10);
    gc9a01_Writ_Byte(0x0E);

    LCD_WR_REG(0xDF);
    gc9a01_Writ_Byte(0x21);
    gc9a01_Writ_Byte(0x0c);
    gc9a01_Writ_Byte(0x02);

    LCD_WR_REG(SET_GAM_MA1);
    gc9a01_Writ_Byte(0x45);
    gc9a01_Writ_Byte(0x09);
    gc9a01_Writ_Byte(0x08);
    gc9a01_Writ_Byte(0x08);
    gc9a01_Writ_Byte(0x26);
    gc9a01_Writ_Byte(0x2A);

    LCD_WR_REG(SET_GAM_MA2);
    gc9a01_Writ_Byte(0x43);
    gc9a01_Writ_Byte(0x70);
    gc9a01_Writ_Byte(0x72);
    gc9a01_Writ_Byte(0x36);
    gc9a01_Writ_Byte(0x37);
    gc9a01_Writ_Byte(0x6F);

    LCD_WR_REG(SET_GAM_MA3);
    gc9a01_Writ_Byte(0x45);
    gc9a01_Writ_Byte(0x09);
    gc9a01_Writ_Byte(0x08);
    gc9a01_Writ_Byte(0x08);
    gc9a01_Writ_Byte(0x26);
    gc9a01_Writ_Byte(0x2A);

    LCD_WR_REG(SET_GAM_MA4);
    gc9a01_Writ_Byte(0x43);
    gc9a01_Writ_Byte(0x70);
    gc9a01_Writ_Byte(0x72);
    gc9a01_Writ_Byte(0x36);
    gc9a01_Writ_Byte(0x37);
    gc9a01_Writ_Byte(0x6F);

    LCD_WR_REG(0xED);
    gc9a01_Writ_Byte(0x1B);
    gc9a01_Writ_Byte(0x0B);

    LCD_WR_REG(0xAE);
    gc9a01_Writ_Byte(0x77);

    LCD_WR_REG(0xCD);
    gc9a01_Writ_Byte(0x63);

    LCD_WR_REG(0x70);
    gc9a01_Writ_Byte(0x07);
    gc9a01_Writ_Byte(0x07);
    gc9a01_Writ_Byte(0x04);
    gc9a01_Writ_Byte(0x0E);
    gc9a01_Writ_Byte(0x0F);
    gc9a01_Writ_Byte(0x09);
    gc9a01_Writ_Byte(0x07);
    gc9a01_Writ_Byte(0x08);
    gc9a01_Writ_Byte(0x03);

    LCD_WR_REG(0xE8);
    gc9a01_Writ_Byte(0x34);

    LCD_WR_REG(0x62);
    gc9a01_Writ_Byte(0x18);
    gc9a01_Writ_Byte(0x0D);
    gc9a01_Writ_Byte(0x71);
    gc9a01_Writ_Byte(0xED);
    gc9a01_Writ_Byte(0x70);
    gc9a01_Writ_Byte(0x70);
    gc9a01_Writ_Byte(0x18);
    gc9a01_Writ_Byte(0x0F);
    gc9a01_Writ_Byte(0x71);
    gc9a01_Writ_Byte(0xEF);
    gc9a01_Writ_Byte(0x70);
    gc9a01_Writ_Byte(0x70);

    LCD_WR_REG(0x63);
    gc9a01_Writ_Byte(0x18);
    gc9a01_Writ_Byte(0x11);
    gc9a01_Writ_Byte(0x71);
    gc9a01_Writ_Byte(0xF1);
    gc9a01_Writ_Byte(0x70);
    gc9a01_Writ_Byte(0x70);
    gc9a01_Writ_Byte(0x18);
    gc9a01_Writ_Byte(0x13);
    gc9a01_Writ_Byte(0x71);
    gc9a01_Writ_Byte(0xF3);
    gc9a01_Writ_Byte(0x70);
    gc9a01_Writ_Byte(0x70);

    LCD_WR_REG(0x64);
    gc9a01_Writ_Byte(0x28);
    gc9a01_Writ_Byte(0x29);
    gc9a01_Writ_Byte(0xF1);
    gc9a01_Writ_Byte(0x01);
    gc9a01_Writ_Byte(0xF1);
    gc9a01_Writ_Byte(0x00);
    gc9a01_Writ_Byte(0x07);

    LCD_WR_REG(0x66);
    gc9a01_Writ_Byte(0x3C);
    gc9a01_Writ_Byte(0x00);
    gc9a01_Writ_Byte(0xCD);
    gc9a01_Writ_Byte(0x67);
    gc9a01_Writ_Byte(0x45);
    gc9a01_Writ_Byte(0x45);
    gc9a01_Writ_Byte(0x10);
    gc9a01_Writ_Byte(0x00);
    gc9a01_Writ_Byte(0x00);
    gc9a01_Writ_Byte(0x00);

    LCD_WR_REG(0x67);
    gc9a01_Writ_Byte(0x00);
    gc9a01_Writ_Byte(0x3C);
    gc9a01_Writ_Byte(0x00);
    gc9a01_Writ_Byte(0x00);
    gc9a01_Writ_Byte(0x00);
    gc9a01_Writ_Byte(0x01);
    gc9a01_Writ_Byte(0x54);
    gc9a01_Writ_Byte(0x10);
    gc9a01_Writ_Byte(0x32);
    gc9a01_Writ_Byte(0x98);

    LCD_WR_REG(0x74);
    gc9a01_Writ_Byte(0x10);
    gc9a01_Writ_Byte(0x85);
    gc9a01_Writ_Byte(0x80);
    gc9a01_Writ_Byte(0x00);
    gc9a01_Writ_Byte(0x00);
    gc9a01_Writ_Byte(0x4E);
    gc9a01_Writ_Byte(0x00);

    LCD_WR_REG(0x98);
    gc9a01_Writ_Byte(0x3e);
    gc9a01_Writ_Byte(0x07);

    LCD_WR_REG(Tearing_Effect_Line_On);
    LCD_WR_REG(Display_Inversion_On);

    LCD_WR_REG(Exit_Sleep_Mode);
    gc9a01_if_delay(120);
    LCD_WR_REG(Display_On);
    gc9a01_if_delay(20);

    return (0);
}

void gc9a01_deinit(void) {}

void gc9a01_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    gc9a01_Set_Address(x,y,x,y);//设置光标位置 
	gc9a01_Writ_Halfword(color);
}

void gc9a01_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    uint16_t t;
    int      xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int      incx, incy, uRow, uCol;
    delta_x = x2 - x1; // 计算坐标增量
    delta_y = y2 - y1;
    uRow    = x1; // 画线起点坐标
    uCol    = y1;

    if (delta_x > 0) {
        incx = 1; // 设置单步方向
    } else if (delta_x == 0) {
        incx = 0; // 垂直线
    } else {
        incx    = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0) {
        incy = 1;
    } else if (delta_y == 0) {
        incy = 0; // 水平线
    } else {
        incy    = -1;
        delta_y = -delta_y;
    }

    //选取基本增量坐标轴 
    distance = (delta_x > delta_y) ? delta_x : delta_y;

    for (t = 0; t < distance + 1; t++) {
        gc9a01_draw_pixel(uRow, uCol, color); // 画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}

void gc9a01_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    gc9a01_draw_line(x1, y1, x2, y1, color);
    gc9a01_draw_line(x1, y1, x1, y2, color);
    gc9a01_draw_line(x1, y2, x2, y2, color);
    gc9a01_draw_line(x2, y1, x2, y2, color);
}

void gc9a01_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
    int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		gc9a01_draw_pixel(x-b,y-a,color);             //3           
		gc9a01_draw_pixel(x+b,y-a,color);             //0           
		gc9a01_draw_pixel(x-a,y+b,color);             //1                
		gc9a01_draw_pixel(x-a,y-b,color);             //2             
		gc9a01_draw_pixel(x+b,y+a,color);             //4               
		gc9a01_draw_pixel(x+a,y-b,color);             //5
		gc9a01_draw_pixel(x+a,y+b,color);             //6 
		gc9a01_draw_pixel(x-b,y+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//判断要画的点是否过远
		{
			b--;
		}
	}
}

void gc9a01_fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color) {
    uint8_t px_map_dma[240*2];
    uint16_t tempBufSize =  sizeof(px_map_dma);
    uint16_t tempBufLen = (sizeof(px_map_dma)/2);

    gc9a01_Set_Address(xsta,ysta,xend-1,yend-1);//设置显示范围
	// uint16_t i,j;
	// for(i=ysta;i<yend;i++)
	// {													   	 	
	// 	for(j=xsta;j<xend;j++)
	// 	{
	// 		gc9a01_Writ_Halfword(color);
	// 	}
	// }
	int px_map_size = (xsta-xend) * (ysta-yend);
	int px_map_align = px_map_size % tempBufLen;
	for (int i = 0; i < px_map_align; i++) {
	    gc9a01_Writ_Halfword(color);
	}
	int px_map_count = px_map_size / tempBufLen;
	if (px_map_count > 0) {
		uint8_t * p = px_map_dma;
		for (int i = 0; i < tempBufLen; i++) {
			*p++ = (uint8_t)(color>>8);
			*p++ = (uint8_t)color;
		}
	}
	for (int i = 0; i < px_map_count; i++) {
        gc9a01_if_transmit_dma((uint8_t *)px_map_dma, tempBufSize);
	}
}

void gc9a01_draw_picture(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, const uint8_t picture[]) {
    uint16_t i, j;
    uint32_t k = 0;
    gc9a01_Set_Address(x, y, x + width - 1, y + hight - 1);
    for (i = 0; i < width; i++) {
        for (j = 0; j < hight; j++) {
            gc9a01_Writ_Byte(picture[k * 2]);
            gc9a01_Writ_Byte(picture[k * 2 + 1]);
            k++;
        }
    }
}

void gc9a01_draw_buf(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint8_t *buf, uint16_t len) {
    gc9a01_Set_Address(x1,y1,x2,y2);
    gc9a01_if_transmit_dma(buf, len);
}

void gc9a01_enter_sleep(void) {
    LCD_WR_REG(Enter_Sleep_Mode);
    GC9A01_SPI_DeInit();
}

void gc9a01_exit_sleep(void) {
    GC9A01_MX_SPI_Init();
    LCD_WR_REG(Exit_Sleep_Mode);
}