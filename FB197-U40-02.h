/********************************************************************************
*	CopyRight(c) 2006, HollySys													*
*	All rights reserved.														*
*																				*
*	文件名称：NB197-U40-01.h															*
*																				*
*	文件版本：01																*
*																				*
*	摘要：定义初始化 admit.c 文件中用到所有全局变量								*
*																				*
*	作者：董伟杰																*
*																				*
*	完成日期：2009-03-02														* 
*																				*
*	修改者：																	*
*																				*
*	修改日期：																	* 
********************************************************************************/

#include <absacc.h>
#include <stdio.h>
#include <intrins.h>
#include <reg51xd2.h>

#define 	uchar 					unsigned 	char
#define  	uint 					unsigned 	int

/* 地址分配 */
#define 	HUB_STATE  	XBYTE 		[0xbfff]	// 对时集线器状态读地址；              
#define 	READ_SEC 	XBYTE 		[0xcfff]	// 读EPM570秒寄存器地址；
#define 	ADJUST_SEC 	XBYTE 		[0xdfff]	// 写EPM570秒寄存器地址；ADJUST_SEC
#define 	DAT_8279 	XBYTE 		[0xefff]  	// 读写8279数据地址（本程序显示部分模仿的8279芯片）；

/*故障定义*/
#define		GPS_ERR_CC				0xCC		// GPS传输故障
#define		GPS_ERR_DD				0XDD		// GPS丢秒故障
#define		GPS_ERR_LL				0XEE		// GPS连接故障

/* DS3231 */     								// dongweijie 2008-09-03
sbit    	scl = P1^4;     					// I2C 口线定义；
sbit    	sda = P1^5;
#define 	ADDRTC  	0xd0    				// DS3231 默认从地址；
#define 	ACK     	0						// I2C协议是应答；
#define 	NACK    	1						// I2C协议否应答；
uchar data  sec, min, hr, dy, dt, mn, yr; 		// 时分秒年月日变量定义；

/* X5045 */      								// dongweijie 2009-02-03
sbit SSEL = P3^5;  								// 模拟x5045的CS线
sbit MISO = P1^2;								// 模拟x5045的SO线
sbit MOSI = P1^7;								// 模拟x5045的SI线
sbit SCLK = P1^6;								// 模拟x5045的SCK线

#define 	WREN 		06						// 使能写操作
#define 	WRDI 		04						// 禁止写操作
#define 	WRSR 		01						// 写状态寄存器
#define 	RDSR 		05						// 读状态寄存器
#define 	MAX_POLL  	99						// 最大写过程时间,确定5045的最大的写入过程的时间
              
/* PC16552D 寄存器定义*/
#define 	Uart1_Rhr 	XBYTE		[0xf8ff] 	/*操作时，Lcr.7=0 */
#define 	Uart1_Thr 	XBYTE 		[0xf8ff] 	/*操作时，Lcr.7=0 */
#define 	Uart1_Ier 	XBYTE 		[0xf9ff] 	/*操作时，Lcr.7=0 */
#define 	Uart1_Isr 	XBYTE 		[0xfaff] 	/*操作时，Lcr.7=0 */
#define 	Uart1_Fcr 	XBYTE 		[0xfaff] 	/*操作时，Lcr.7=0 */
#define 	Uart1_Lcr 	XBYTE 		[0xfbff] 	/*操作时，Lcr.7=0，1都可以 */
#define 	Uart1_Lsr 	XBYTE 		[0xfdff] 	/*操作时，Lcr.7=0，1都可以 */
#define 	Uart1_Scr 	XBYTE 		[0xffff] 
#define 	Uart1_Divl  XBYTE 		[0xf8ff] 	/*操作时，Lcr.7=1 */
#define 	Uart1_Divm  XBYTE 		[0xf9ff] 	/*操作时，Lcr.7=1 */
#define 	Uart1_Alf  	XBYTE 		[0xfaff]
              				   	
#define 	Uart2_Rhr 	XBYTE 		[0xf0ff]
#define 	Uart2_Thr 	XBYTE 		[0xf0ff] 
#define 	Uart2_Ier	XBYTE 		[0xf1ff]
#define 	Uart2_Isr 	XBYTE 		[0xf2ff] 
#define 	Uart2_Fcr 	XBYTE 		[0xf2ff]
#define 	Uart2_Lcr 	XBYTE 		[0xf3ff]
#define 	Uart2_Lsr 	XBYTE 		[0xf5ff] 
#define 	Uart2_Scr 	XBYTE 		[0xf7ff] 
#define 	Uart2_Divl  XBYTE 		[0xf0ff]
#define 	Uart2_Divm  XBYTE 		[0xf1ff] 
#define 	Uart2_Alf  	XBYTE 		[0xf2ff] 

/*Key_Code Define */     						// dongweijie 2008-09-03
#define		 KEY_SET  	 	0x0E	 			// 1110
#define 	 KEY_SEL   		0x0D	 			// 1101
#define 	 KEY_UP    		0x0B	 			// 1011
#define		 KEY_DOWN  		0x07   				// 0111

/*有关显示用到的全局变量*/
uchar code 	display_table[12] 	= 	{0x0C,0x9F,0x4A,0x0B,0x99,0x29,
                             		0x28,0x8F,0x08,0x09,0xFF,0x00 };   	// {0 1 2 3 4 5 6 7 8 9 全显示,空}	  dongweijie 2008-09-04
uchar code 	month_table[12] 	= 	{31,28,31,30,31,30,31,31,30,31,
                             		30,31};                             
uchar data 	display_buffer[14]	=	{11,11,11,11,11,11,11,	 			// 显示寄存器初值；
                                	11,11,11,11,11,11,11};                                                                                           
uchar 	 data 		g_time_buffer[14];  			// year ,month,day,hour,minute,second  BCD格式
uchar  	 data 		g_timecom_buffer[6]; 			// year,month,day,hour,minute,second   二进制格式 
uchar   idata 		g_adj[14];  					// UART接收到的GPS对时数据;                                
uchar   idata 		g_gps_sec_pre = 0;  			// UART接收到的GPS对时"上次秒"数据;                                
uchar   idata 		g_gps_True = 3;  				// UART接收到的GPS对时"上次秒"数据;                                
uchar 	 data   	g_check_xor = 0x66;				// UART接收数据异或校验位  GPZDA码除去时间信息外的异或值是0x66
uchar 	 data 		g_couter = 0;					// 从UART读数据计数器；
uchar    data 		g_adj_s[14];  					// 串口接收到的GPS对时数据;                                
uchar   idata 		g_gps_sec_pre_s = 0;  			// 串口接收到的GPS对时"上次秒"数据;                                
uchar   idata 		g_gps_True_s = 3;  				// 串口接收到的GPS对时"上次秒"数据;                                
uchar 	 data		g_check_xor_s = 0x66;			// 串口接收数据异或校验位  GPZDA码除去时间信息外的异或值是0x66
uchar 	 data 		g_couter_s = 0;					// 从串口读数据计数器；

sbit 				g_led_standby	= P1^3;  			// 对时集线器前面板LED显示控制口线；            
                            
bit 	bdata 		g_adjust_flag = 0;				// 手动设置标识；
bit 	bdata 		g_spark_flag = 0;				// 手动设置时数码管闪烁标识；
bit 	bdata 		g_trans_flag = 0;				// 接受数据成功表示；
bit 	bdata 		g_trans_flag_t = 0;				// 级联时数据传输成功表示；
bit 	bdata 		g_gps_flag=0;					// 是否使用GPS标识；

uchar 	idata 		g_couter_10ms = 0;				// 手动设置时闪烁计数用；
uchar	idata 		g_adjust = 0;					// UART芯片状态读写用； 
uchar 	idata 		g_adjust2 = 0;						// UART芯片状态读写用； 
uchar 	idata 		g_couter_adj = 0;				// 手动设置的数码管位数计数器；
uchar 	idata 		g_errFlag = 0;		  			// GPS 信号故障类型标志 0x1(4/7)A 表示传输故障 0x2(5/8)A表示连接故障 0x3(6)A是设备故障
uchar	idata		g_errFlagReg = 0;				// 故障类型临时寄存器
uchar 	idata		g_errReport = 0;				// 故障处理情况
uchar   idata		g_err_A = 0;					// A 类错误计数
uchar 	idata     	g_number_dog = 0;				// 一些计数用的变量；
uint 	idata 		g_clr_count = 0;				// 一些计数用的变量；

