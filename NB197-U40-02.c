/************************************************************************************
*	CopyRight(c) 2006, HollySys														*
*	All rights reserved.															*
*																					*
*	目标文件的版本：NB197-U40-01.hex,由工程NB197-U40-01.Uv2编译生成,				*
*																					*
*	文件名称：NB197-U40-01.c        														*
*																					*
*	文件版本：A01       															*
*																					*
** 摘要(Module Description):														*
	该源程序所实现的功能说明：														*
	主程序实现：根据主从设置的不同,或接收GPS校时信息,或采用本地时钟芯片DS3231的时间 *
	信息,或者接收主NM197发过来的时间信息,生成本地时钟,输出15路时间数据,可用于对服务 *
	器校时,或对从NM197校时.															*
*																					*
*	作者：董伟杰																	*
*																					*
*	完成日期：2009-03-02															*
*	修改日期：2009-03-02															*
*************************************************************************************
*	文件名称：NB197-U40-02.c        												*
*																					*
*	文件版本：A02       															*
*																					*
** 摘要(Module Description):														*
	增加了时区设置功能，当采用外接GPS时,利用拨码开关第4位选择,"1"代表输入为格林威治 *
	时间,"0"代表输入为北京时间.                                                     *
*																					*
*	作者：李毅堂																	*
*																					*
*	完成日期：2012-07-17															*  
*	修改日期：2012-07-17															*																				 
*************************************************************************************
    定时器T2中断	中断来自PC16552串口芯片的输入，中断优先级为4
	定时器T1中断	10ms定时中断，优先级=0，在手动设置时间信息时，用于数码管闪烁显示。
	定时器T0		用于外部计数，计数初值重装载方式，初值255，相当于1秒中断。
	串口中断		用于接收内部GPS的数据，中断优先级为4
	外部中断1		用于处理按键信息
	P0.0～P0.7 		8位数据总线
	P1.0 			T2的外部计数输入，接入PC16552的串口1输出完成中断
	P1.1			T2的触发控制，接入PC16552的串口2输入中断
	P1.2 			模拟x5045的SO线
	P1.6 			模拟x5045的SCK线
	P1.7 			模拟x5045的SI线	
	P3.5 			模拟x5045的CS线
	P1.3			主从状态指示灯控制口线隔秒取反为"主+本地时钟"方式，否则为其他方式
	P1.4			与DS3231的I2C接口的SCL口线
	P1.5			与DS3231的I2C接口的SDA口线
	P2.0～P2.7		地址总线的高8位
	P3.2			来自于CPLD的分脉冲输入做为外部中断0
	P3.3			来自于CPLD的键盘输入中断
	P3.4 			T0的计数输入，来自于CPLD的秒脉冲
	P3.6			写信号线
	P3.7 			读信号线
	显示(FB199-A)	0XEFFFH
	UART(16552D)	0XF0FF~FFFFH
/*************************************************************************************/


/*  #define _DEBUG_  */
#ifdef 		_DEBUG_
#pragma  	large, debug, symbols, objectextend
#else
#pragma  	large
#endif 

#include 	"NB197-U40-01.h"


/************************************************************************
* 函数功能：将1字节二进制数拆成2字节BCD码								*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void bin_bcd() 
{
	g_time_buffer[2] = g_timecom_buffer[0] / 10;	 // 年
	g_time_buffer[3] = g_timecom_buffer[0] % 10;	
	
	g_time_buffer[4] = g_timecom_buffer[1] / 10;	 // 月
	g_time_buffer[5] = g_timecom_buffer[1] % 10;
	
	g_time_buffer[6] = g_timecom_buffer[2] / 10;	 // 日
	g_time_buffer[7] = g_timecom_buffer[2] % 10;
	
	g_time_buffer[8] = g_timecom_buffer[3] / 10;	 // 时
	g_time_buffer[9] = g_timecom_buffer[3] % 10;
	
	g_time_buffer[10] = g_timecom_buffer[4] / 10;	 // 分
	g_time_buffer[11] = g_timecom_buffer[4] % 10;
		
	g_time_buffer[12] = g_timecom_buffer[5] / 10;	 // 秒
	g_time_buffer[13] = g_timecom_buffer[5] % 10;
	return;
}

/************************************************************************
* 函数功能：将2字节BCD码合成1字节二进制数								*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
 void bcd_bin()      
{
	g_timecom_buffer[0] = g_time_buffer[2] * 10 + g_time_buffer[3];	  	// 年
	g_timecom_buffer[1] = g_time_buffer[4] * 10 + g_time_buffer[5];	  	// 月
	g_timecom_buffer[2] = g_time_buffer[6] * 10 + g_time_buffer[7];	  	// 日
	g_timecom_buffer[3] = g_time_buffer[8] * 10 + g_time_buffer[9];	 	// 时
	g_timecom_buffer[4] = g_time_buffer[10] * 10 + g_time_buffer[11];	// 分
	g_timecom_buffer[5] = g_time_buffer[12] * 10 + g_time_buffer[13];	// 秒
	return;
}

/************************************************************************
* 函数功能：将单片机中时间缓冲区中的数据传送到显示缓冲区				*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void time_dis() 
{
	display_buffer[0]  = g_time_buffer[0]; 			// 年
	display_buffer[1]  = g_time_buffer[1];
	display_buffer[2]  = g_time_buffer[2];
	display_buffer[3]  = g_time_buffer[3];	   		
	display_buffer[4]  = g_time_buffer[8];			// 月
	display_buffer[5]  = g_time_buffer[9];			
	display_buffer[6]  = g_time_buffer[12];	   		// 日
	display_buffer[7]  = g_time_buffer[13];
	display_buffer[8]  = g_time_buffer[4];	   		// 时
	display_buffer[9]  = g_time_buffer[5];
	display_buffer[10] = g_time_buffer[6];	  		// 分
	display_buffer[11] = g_time_buffer[7];
	display_buffer[12] = g_time_buffer[10];	  		// 秒
	display_buffer[13] = g_time_buffer[11]; 
	return;
}

/************************************************************************
* 函数功能：延时														*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void delay()      								// 延时
{
	uchar data i;
	for (i=0;i<100;i++)
	{
	}	
	return;			
}

/************************************************************************
* 函数功能：延时														*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void delay_L()      							// 较长时间的延时，中间要喂狗 
{
	uchar data i,j,k;
	for (i=0;i<15;i++)	
		for(j=0;j<255;j++)
			for(k=0;k<255;k++)
			{
				   	SSEL = 0;
				   	_nop_();
				   	_nop_();
				   	SSEL = 1;					// 喂看门狗,因为本次循环较长；
			}
	return;			
}

/************************************************************************
* 函数功能：显示函数，用于将要显示的数据传送到EPM570，					*
* 			由EPM570来模仿8279显示芯片									*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void display_0(void) using 0   					// 用在主函数中的显示程序
{
    uchar data i,j;
	DAT_8279=0xAA  ;      						// 与EPM570的传送协议，0xAA表示数据传送的开始；
	for (i=0;i<14;i++)
	{
		j = display_buffer[i];     				// 将要显示数据的对应数码段传送给EPM570； 
		DAT_8279 =  display_table[j];      
	}
	return;
}

void display_3(void) using 2   					// 用在T1中断中的显示函数
{												
    uchar data i,j;
	DAT_8279=0xAA  ;      						// 与EPM570的传送协议，0xAA表示数据传送的开始；
	for (i=0;i<14;i++)
	{
		j = display_buffer[i];     				// 将要显示数据的对应数码段传送给EPM570； 
		DAT_8279 = display_table[j];      
	}
	return;
}

void display_2(void) using 2   					// 外部中断1函数中的显示函数
{
    uchar data i,j;
	DAT_8279=0xAA  ;      						// 与EPM570的传送协议，0xAA表示数据传送的开始；
	for (i=0;i<14;i++)
	{
		j = display_buffer[i];     				// 将要显示数据的对应数码段传送给EPM570； 
		DAT_8279 = display_table[j];      
	}
	return;
}

/************************************************************************
* 函数功能：初始化PC16552D:  ch1 (output) ;  ch2 (input)				*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void Init_Pc16552D(void) 
{
	Uart1_Lcr = 0x80;   
	Uart1_Alf = 0;
	
	Uart2_Lcr = 0x80;   
	Uart2_Alf = 0;	
	
	Uart1_Lcr = 0x80;    							// Lcr.7=1 */
	Uart1_Divl = 0x0c;   							// 定义波特率 9600
	Uart1_Divm = 0x00;  
		
	Uart1_Fcr = 0x00;    							// 禁止缓冲  
	Uart1_Lcr = 0x00;    							// Lcr.7=0
	Uart1_Ier = 0x02;    							// enable thr empty int       
	Uart1_Lcr = 0x03;    							// 8位数据位；1位停止位	
											
	Uart2_Lcr = 0x80;    							// make Lcr.7=1
	Uart2_Divl = 0x0c;   							// defining the boud rate
	Uart2_Divm = 0x00;   							//  boud rate=9600 
	                 
	Uart2_Fcr = 0x00;    							// disable fifo     
	Uart2_Lcr = 0x00;    							// make Lcr.7=0
	Uart2_Ier = 0x01;    							// enable rhr full int     
	Uart2_Lcr = 0x03;    							// 8位数据位；1位停止位  
	return;  
}

/************************************************************************
* 函数功能：模拟SPI写													*	
* 参数说明：无															*
*			X5045 芯片程序段											*
* 返回值：	无															*
************************************************************************/
void WriteData(unsigned char mData)
{
 	unsigned char mTemp, i;
	mTemp = mData;
 	for(i=0; i<8; i++)
 	{
   		SCLK = 0;
   		MOSI = (mTemp >> (7 - i)) & 0x01;
   		SCLK = 1;
	}
	return;
}
/************************************************************************
* 函数功能：模拟SPI读													*	
* 参数说明：无															*
*			X5045 芯片程序段											*
* 返回值：	无															*
************************************************************************/
unsigned char ReadData(void)
{
 	unsigned char mTemp, mData, i;
 	mTemp = 0x00;
 	for(i=0; i<8; i++)
 	{
   		SCLK = 1;
   		_nop_();
    	SCLK = 0;
		_nop_();
   		mData = (unsigned char)(MISO);
   		mTemp = mTemp | (mData<<(7-i));
 	}
 	return mTemp;
}

/************************************************************************
* 函数功能：写X5045状态寄存器使能										*	
* 参数说明：无															*
*			X5045 芯片程序段											*
* 返回值：	无															*
************************************************************************/
void EN_Write(void)
{
	SCLK = 0;
 	SSEL = 0;
 	WriteData(WREN);
 	SCLK = 0;
 	_nop_();
 	SSEL = 1;
	return;
}    

/************************************************************************
* 函数功能：读X5045状态寄存器											*	
* 参数说明：无															*
*			X5045 芯片程序段											*
* 返回值：	无															*
************************************************************************/
unsigned char StatusRegRead(void)
{
 	unsigned char mTemp;
 	SCLK = 0;
 	SSEL = 0;
 	WriteData(RDSR);
 	mTemp = ReadData();
 	SCLK = 0;
 	_nop_();
 	SSEL = 1;
 	return mTemp;
}

/************************************************************************
* 函数功能：写X5045EEPROM寄存器											*	
* 参数说明：无															*
*			X5045 芯片程序段											*
* 返回值：	无															*
************************************************************************/
void WipPoll(void)
{
	unsigned char mTemp;
	unsigned char mData;
	for(mTemp=1; mTemp<MAX_POLL; mTemp++)
	{
 		mData = StatusRegRead();
 		if((mData&0x01)==0)
 		{
 			mTemp = MAX_POLL;
 		}
	}
	return;
}

/************************************************************************
* 函数功能：写X5045状态寄存器											*	
* 参数说明：无															*
*			X5045 芯片程序段											*
* 返回值：	无															*
************************************************************************/
void StatusRegWrite(unsigned char mData)
{
 	SCLK = 0;
 	SSEL = 0;
 	WriteData(WRSR);
 	WriteData(mData);
 	SCLK = 0;
 	_nop_();
 	SSEL = 1;
 	WipPoll();
	return;
}
/************************************************************************
* 函数功能：复位看门狗													*	
* 参数说明：无															*
*			X5045 芯片程序段											*
* 返回值：	无															*
************************************************************************/

void Reset_Watchdog(void)   
{
   	SSEL = 0;
   	_nop_();
   	_nop_();
   	SSEL = 1;
	return;
}

/************************************************************************
* 函数功能：初始化X5045													*	
* 参数说明：无															*
*			X5045 芯片程序段											*
* 返回值：	无															*
************************************************************************/

void Init_X5045(void)   
{
	EN_Write();									// Set Write Enable Latch(Enable Write Operations)
    StatusRegWrite(0x0F);						// 看门狗溢出时间1.4s，打开看门狗，整块都不保护，允许写入数据
	Reset_Watchdog();							// 喂看门狗
	return;
}

/************************************************************************
* 函数功能：I2C 传送开始												*	
* 参数说明：无															*
*			DS3231程序段												*
* 返回值：	无															*
************************************************************************/
void start(void)            					// I2C 传送开始
{
	sda = 1;  
	scl = 1;
	sda = 0; 
	return;
}

/************************************************************************
* 函数功能：I2C 传送终止												*	
* 参数说明：无															*
*			DS3231程序段												*
* 返回值：	无															*
************************************************************************/
void stop(void)            						// I2C 传送终止
{
    sda = 0;  
	sda = 0;
    scl = 1;  
	scl = 1;  
	sda = 1;
	return;
}

/************************************************************************
* 函数功能：通过I2C 将数据d写入DS3231									*	
* 参数说明：d为要写入DS3231的数据										*
*			DS3231程序段												*
* 返回值：	数据线状态													*
************************************************************************/
uchar i2cwrite(uchar d) 						// 通过I2C 将数据d写入DS3231；
{
	uchar i;

    scl = 0;
    for (i=0;i<8; i++)
    {
        if (d&0x80)
            sda = 1;  							// 从最大位开始传送；
        else
            sda = 0;
        scl = 0;
        scl = 1;
        d = d << 1;  							// 循环按位传送；
        scl = 0;
    }
    sda = 1;        
    scl = 0;
    scl = 1;

    scl = 0;
    return(sda);
}

/************************************************************************
* 函数功能：通过I2C 从DS3231将数据读出									*	
* 参数说明：d用来存放刚从DS3231读出的数据								*
*			DS3231程序段												*
* 返回值：	数据线状态													*
************************************************************************/
uchar i2cread(char b)							// 通过I2C 从DS3231将数据读出到 b；
{					   	
	uchar i, d;

    d = 0;
    sda = 1;             
    scl = 0;
    for (i=0; i<8; i++)  						// 从最高位读起；
    {
        scl = 1;
        d = d << 1;
        d = d | (unsigned char)sda;
        scl = 0;
    }
    sda = b;          							// 低位ack，高为nack；
    scl = 1;
    scl = 0;

    sda = 1;          							// 释放sda口线；
    return d;
}

/************************************************************************
* 函数功能：从时钟芯片中读取时间										*	
* 参数说明：无															*
*			DS3231程序段												*
* 返回值：	无															*
************************************************************************/
void  Read_Time(void)							// 从时钟芯片中读取时间 
{   
    start();									// I2C 传送开始；
    i2cwrite(ADDRTC);							// 写入从地址；
    i2cwrite(0);								// 从地址0开始连续读DS3231寄存器；
    start();									// 开始和结束均表示上次I2C传送的结束；
    i2cwrite(ADDRTC|1);
    sec = i2cread(ACK);							// 秒；
    min = i2cread(ACK);							// 分；
    hr = i2cread(ACK);							// 时；
    dy = i2cread(ACK);							// 星期；
    dt = i2cread(ACK);							// 日；
    mn = i2cread(ACK);							// 月；
    yr = i2cread(NACK);							// 年；
    stop();										// I2C 传送结束；
								  
	g_time_buffer[13] = sec&0x0f; 				// 因为从DS3231读出来的数据是BCD码，将高低位分别放入不同的寄存器，以方便显示；
	g_time_buffer[12] = (sec>>4)&0x0f; 			// 秒；
	g_time_buffer[11] = min&0x0f;					// 分；
	g_time_buffer[10] = (min>>4)&0x0f;	
	g_time_buffer[9] = hr&0x0f;					// 时；
	g_time_buffer[8] = (hr>>4)&0x0f;
	g_time_buffer[7] = dt&0x0f;					// 日；
	g_time_buffer[6] = (dt>>4)&0x0f;
	g_time_buffer[5] = mn&0x0f;					// 月；
	g_time_buffer[4] = (mn>>4)&0x0f;
	g_time_buffer[3] = yr&0x0f;					// 年；
	g_time_buffer[2] = (yr>>4)&0x0f;

	bcd_bin();									// bcd码时间转换成二进制时间，以方便计算用；
	
	return;      						
}

/************************************************************************
* 函数功能：向时钟芯片中写入时间										*	
* 参数说明：无															*
*			DS3231程序段												*
* 返回值：	无															*
************************************************************************/
void  Write_Time(void)							// 向时钟芯片中写入时间    
{    
    bin_bcd();  								// 刷新BCD码格式的时间
  
	sec = ((g_time_buffer[12]<<4)&0xf0)|(g_time_buffer[13]&0x0f); 	// 秒；
	min = ((g_time_buffer[10]<<4)&0xf0)|(g_time_buffer[11]&0x0f); 	// 分；
	hr =((g_time_buffer[8]<<4)&0xf0)|(g_time_buffer[9]&0x0f); 		// 时；
	dt = ((g_time_buffer[6]<<4)&0xf0)|(g_time_buffer[7]&0x0f); 		// 日；
	mn = ((g_time_buffer[4]<<4)&0xf0)|(g_time_buffer[5]&0x0f); 		// 月；
	yr =((g_time_buffer[2]<<4)&0xf0)|(g_time_buffer[3]&0x0f); 		// 年；
	dy = 1;										// 星期在这里没用，设置成1；

    start();									// I2C 传送开始；
    i2cwrite(ADDRTC);  							// 写入从地址；
    i2cwrite(0x00);     						// 从地址0开始连续写入DS3231寄存器；
    i2cwrite(sec);								// 秒；
    i2cwrite(min);								// 分；
    i2cwrite(hr);								// 时；
    i2cwrite(dy);								// 日；
    i2cwrite(dt);								// 星期；
    i2cwrite(mn);								// 月；
    i2cwrite(yr);								// 年；
    stop();	
	return;
}

/************************************************************************
* 函数功能：时钟芯片初始化												*	
* 参数说明：无															*
*			DS3231程序段												*
* 返回值：	无															*
************************************************************************/
void  Init_3231(void)     						// 时钟芯片初始化
{

    do      									// 因为上电或重启时,DS3231的I2C接口处在有效状态,我们必须让主机释放I2C口线.  
    {       		
        sda = 1;    
        scl = 1;
        if(sda) 								// 如果sda线为高,启动I2C传输  
        {
                sda = 0;						// DS3231 能够识别I2C传输的开始  
                sda = 1;    
        }
        scl = 0;
    } while(sda == 0);      					// 如果DS3231为低,从而出现错误状态,重新开始.
    start();
    i2cwrite(ADDRTC);							// DS3231的默认co0从机地址
    i2cwrite(0x0e); 							// 写DS3231的控制寄存器地址:0E
    i2cwrite(0x00); 							// INT/SQW 输出1HZ方波，作为单片机和EPM570的妙计数器用； 
	return;
}

/************************************************************************
* 函数功能：判断是否是闰年												*	
* 参数说明：无															*
*																		*
* 返回值：	如果是闰年返回1，否则返回0									*
************************************************************************/
uchar leap_year_using1_t(void)   	using 1		// T2中断函数中用到的闰年判断函数
{
 	uchar	data  	year;
	year=g_timecom_buffer[0];
	if((year%4)==0)   
	{
		return(1);
	}     
	return(0);
}
uchar leap_year_using1_s(void)   	using 1		// 串口中断中用到的闰年判断函数
{
 	uchar	data  	year;
	year=g_timecom_buffer[0];
	if((year%4)==0)   
	{
		return(1);
	}     
	return(0);
}
uchar leap_year_using2_s(void)  	using 2		// time_adj()函数中用到的闰年判断函数
{
 	uchar	data  	year;
	year=g_timecom_buffer[0];
	if((year%4)==0)   
	{
		return(1);
	}     
	return(0);
}

uchar leap_year_using2_i(void)  	using 2		// 外部中断中用到的闰年判断函数
{
 	uchar	data  	year;
	year=g_timecom_buffer[0];
	if((year%4)==0)   
	{
		return(1);
	}     
	return(0);
}

/************************************************************************
* 函数功能：时间调整													*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void time_adj(void)	using 2
{ // 调整时间开始  
	g_timecom_buffer[5]++; // 调整秒  
	if  (g_timecom_buffer[5]==60)
	{ // 秒已是60，则调整分 
		g_timecom_buffer[5] = 0; // 清零秒  
		g_timecom_buffer[4]++; // 调整分  
		if (g_timecom_buffer[4]==60)
		{ // 分已是60，则调整小时 
			g_timecom_buffer[4] = 0; // 清零分  
			g_timecom_buffer[3]++; // 调整小时  
			if (g_timecom_buffer[3]==24)
			{// 小时已是24，则调整日 
				g_timecom_buffer[3] = 0;  // 清零小时                    
				g_timecom_buffer[2]++;  // 调整日                                             
				if (g_timecom_buffer[2]>month_table[(g_timecom_buffer[1]-1)])
				{ // 日已大于本月最大天数 
					if ((g_timecom_buffer[1]!=2)||!(leap_year_using2_s())||(g_timecom_buffer[2]!=29))                               
					{ // 不是2月或不是闰年或本月不是29日，则调整到下个月1日，开始  
						g_timecom_buffer[2] = 1;
						g_timecom_buffer[1]++;
						if (g_timecom_buffer[1]==13)
						{ // 月已是13，调整年，开始 
							g_timecom_buffer[1] = 1;
							g_timecom_buffer[0]++;
						} // 月已是13，调整年，结束
					}   // 不是2月或不是闰年或本月不是29日，则调整到下个月1日，开始  
				} // 日已大于本月最大天数 
			} // 小时已是24，则调整日      
		} // 分已是60，则调整小时          
	} // 秒已是60，则调整分

	return;  
	                                                                          
} // 调整时间结束 

/****************************************************************
*函数功能：	 T2外部触发中断，用于接收外部时钟源信息				*
*中断优先级：highest，7											*
*说明：		该中断程序接收外部时钟源时间数据，并对其分析、处理	*
*      		该中断来自于UART芯片接受数据中断					*
****************************************************************/
void t2_recive() interrupt 5 using 1  				// 中断程序入口 
{
	uchar data 		i;
	uchar data 		gps_sec;  						// UART接收到的GPS对时"本次秒"数据;                                
	uchar data 		check_temp;						// 存放校验码
 	uchar data		data_t2;						// 存放从Uart接受到的数据

  	EA = 0;
  	EXF2 = 0;      									// EXF2 和 TF2 必须软件清零；
  	TF2 = 0;
	data_t2 = Uart2_Rhr;
  	if((HUB_STATE&0x01)==0x00)    					// 从时钟	    
  	{
		if(data_t2==0xaa)  							// 级联数据传送的格式为：AA 数据 BB；
		{      	      	  
			g_clr_count = 0;
			if(g_errFlag==0x8A&&g_gps_True==0x02)						// 说明本次已经连街上了,故障恢复
			{
				g_errFlag = 0x00;	
			}
			g_couter = 1;
		}
		else if((g_couter>0)&&(g_couter<8))
		{      	  
			g_adj[(g_couter-1)] = data_t2;
			g_couter++;
		}												   
	  	else if((g_couter==8)&&(data_t2==0xbb))	
		{      	        	    
			g_couter = 0; 
			check_temp = g_adj[0];
			for(i=1;i<7;i++)
			{ 
				check_temp = (check_temp^g_adj[i]);
			} 
			if(check_temp==0)
			{
			  	if (g_errFlag == 0x7A&&g_gps_True==0x02)
				{
					g_errFlag = 0x00;			   // 说明本次传输无故障,或者是故障恢复
				}			   		  
				gps_sec = g_adj[5];
				if (((gps_sec==0x00)&&(g_gps_sec_pre_s==59))||(gps_sec==g_gps_sec_pre_s+1))
				{
					g_gps_True++;					// 判断，如果时间秒信息是连续的，则使用其信息，否则不用
				}
				else
				{
					g_gps_True--;
					if(g_gps_True<0x02)
					{
						g_gps_True = 0x02;
						g_errFlag = 0x9A;	 		// 级联时主设备故障
					}
				}
				g_gps_sec_pre_s = gps_sec;	
				if (g_gps_True>0x02)
				{
					g_gps_True = 0x02;
					g_errFlag = 0x00;				// 外部GPS设备无故障
					for(i=0;i<6;i++)
					{
						g_timecom_buffer[i] = g_adj[i];
					}
				}
			}
			else
			{
				g_errFlag = 0x7A;
			    g_gps_True = 0x03;	       			// 防止出现传输错误后紧接着报设备故障
//				g_err_A = 0x01;	
			}
			g_trans_flag_t = 1; 					// 数据传送标识置1；
		}									   	
	  	else
		{
			g_couter = 0;  
		}
	}
  	
  	else  if((HUB_STATE&0x07)==0x07)      			// 集线器为主机，接收GPS信息包,秒脉冲来自GPS刷新显示   	
   	{     								  			// GPS数据包的格式是： GPZDA+时间数据； 所以先判断出GPZADA，然后将后边的时间数据提取出来；		      		
 		if(data_t2==36)					  			// head=$  $=36  
  	  	{      	      	  
 			g_clr_count = 0;
			if(g_errFlag==0x2A&&g_gps_True==0x02)	// 说明本次已经连街上了,故障恢复
			{
				g_errFlag = 0x00;	
			}
 	   		g_couter = 1;
  	  	}
  		else if((g_couter==1)&&(data_t2==71))  		//G=71
  	  	{      	  
  	   		g_couter++;
  	  	}	
  		else if((g_couter==2)&&(data_t2==80))  		//P=80	
  	  	{	      	        	    
  	    	g_couter++;
  	  	}  
  		else if((g_couter==3)&&(data_t2==90))  		//Z=90
  	  	{      	  
  	   		g_couter++;
  	  	}
		else if((g_couter==4)&&(data_t2==68))  		//D=68
		{      	  
			g_couter++;
		}
		else if((g_couter==5)&&(data_t2==65))  		//A=65
		{      	  
			g_couter++;
		}
		else if((g_couter==6)&&(data_t2==44))  		//,=44
		{      	  
			g_couter++;
		}     
		else if((g_couter>6)&&(g_couter<13))     	//hour,minute,second
		{      	  
			g_adj[(g_couter-7)] = (data_t2-0x30);
			g_check_xor ^= g_adj[(g_couter-7)];
			g_couter++;
		}
		else if(g_couter==13)  //&&(data_t2==0x46))	//.=
		{
			g_couter++;
		}      	        	    	   
		else if((g_couter>13)&&(g_couter<16))     	// sub_second
		{      	  
			g_couter++;
		}
		else if((g_couter==16)&&(data_t2==44))		//,=44
		{
			g_couter++;
		} 
		else if((g_couter>16)&&(g_couter<19))     	// day
		{      	  
			g_adj[(g_couter-11)] = (data_t2-0x30);
			g_check_xor ^= g_adj[(g_couter-11)];
			g_couter++;
		}
		else if((g_couter==19)&&(data_t2==44))		//,=44
		{
			g_couter++;
		} 
		else if((g_couter>19)&&(g_couter<22))     	// month
		{      	  
			g_adj[(g_couter-12)] = (data_t2-0x30);
			g_check_xor ^= g_adj[(g_couter-12)];
			g_couter++;
		}
		else if((g_couter==22)&&(data_t2==44))		//,=44
		{
			g_couter++;
		} 
		else if((g_couter>22)&&(g_couter<27))     	// year
		{      	  
			g_adj[(g_couter-13)] = (data_t2-0x30);
			g_check_xor ^= g_adj[(g_couter-13)];
			g_couter++;
		}
		else if(((g_couter==27)||(g_couter==28))&&(data_t2==44))	//,=44
		{
			g_couter++;
		}
		else if(((g_couter==28)||(g_couter==29))&&(data_t2==48))	//0 =48
		{
															//此处为空行.目的是兼容不同厂家的NMEA格式代码
		}
		else if((g_couter==29)&&(data_t2==42))				//*=42 0x2A 
		{
			g_couter++;
		} 
		else if(g_couter==30)								// 校验码	  
		{
			if ((data_t2)<58&&(data_t2>47))					// 校验码为数字
			{
				check_temp = (data_t2-0x30)<<4;
			}
			else if ((data_t2)<71&&((data_t2)>64)) 			// 校验码为大写字母:A B C D E F
			{
				check_temp = (data_t2-0x37)<<4;
			}
			else if ((data_t2)<103&&((data_t2)>96))			// 校验码为小写字母:a b c d e f
			{
				check_temp = (data_t2-0x57)<<4;
			}
           	else
			{
				check_temp = 0x00;
			}
			g_couter++;
		}
  		else if((g_couter==31))
  	  	{      	         	  	        		        	     
  	        g_couter=0; 
			     	                  
			if ((data_t2)<58&&(data_t2>47))
			{
				check_temp += (data_t2-0x30);
			}
			else if ((data_t2)<71&&((data_t2)>64))
			{
				check_temp += (data_t2-0x37);
			}
			else if ((data_t2)<103&&((data_t2)>96))			// 校验码为小写字母:a b c d e f
			{
				check_temp += (data_t2-0x57);
			}
           	else
			{
				check_temp = 0x00;
			}
           	
			if (check_temp==g_check_xor)
			{												// dongweijie 2008-10-20				
				
			  	if (g_gps_True==0x02&&g_gps_True==0x1A)
				{
					g_errFlag = 0x00;			   			// 说明本次传输无故障,或者是故障恢复
				}			   		  
											
				gps_sec = g_adj[4]*10+g_adj[5];
				if (((gps_sec==0x00)&&(g_gps_sec_pre==59))||(gps_sec==g_gps_sec_pre+1))
				{
					g_gps_True++;							// 判断，如果时间秒信息是连续的，则使用其信息，否则不用
				}
				else
				{
					g_gps_True--;
					if(g_gps_True<0x02)
					{
						g_gps_True = 0x02;
						g_errFlag = 0x3A;	 				// 级联时主设备故障
					}
				}
				g_gps_sec_pre = gps_sec;	
				if (g_gps_True>0x02)
				{
					g_gps_True = 0x02;
					g_errFlag = 0x00;						// 外部GPS设备无故障

					g_time_buffer[0]=g_adj[10];
		           	g_time_buffer[1]=g_adj[11];
		           	g_time_buffer[2]=g_adj[12];
		           	g_time_buffer[3]=g_adj[13];
		           	g_time_buffer[4]=g_adj[8];
		           	g_time_buffer[5]=g_adj[9];
		           	g_time_buffer[6]=g_adj[6];
		           	g_time_buffer[7]=g_adj[7];
		           	g_time_buffer[8]=g_adj[0];
		           	g_time_buffer[9]=g_adj[1];
		           	g_time_buffer[10]=g_adj[2];
		           	g_time_buffer[11]=g_adj[3];
		           	g_time_buffer[12]=g_adj[4];
		           	g_time_buffer[13]=g_adj[5];
		          
		           	bcd_bin();        						// bcd码时间转换成二进制时间 
		           	
		           	/* 2012-5-3,李毅堂修改,增加时区设置	*/
					if( (HUB_STATE&0x0F)==0x0F )			// 如果输入为格林威治时间
					{
			           	if(g_timecom_buffer[3]>15)
						{
							g_timecom_buffer[3]=(g_timecom_buffer[3]-16);
							/**********zuoxin modify 2003.01.29***************/
							g_timecom_buffer[2]++; //adjust day                                            
							if (g_timecom_buffer[2]>month_table[(g_timecom_buffer[1]-1)])
							{   // 日已大于本月最大天数 
								if ((g_timecom_buffer[1]!=2)||!(leap_year_using1_t())||(g_timecom_buffer[2]!=29))                               
								{ // 不是2月或不是闰年或本月不是29日，则调整到下个月1日，开始  
									g_timecom_buffer[2] = 1;
									g_timecom_buffer[1]++;
									if (g_timecom_buffer[1]==13)
									{ // 月已是13，调整年，开始  
										g_timecom_buffer[1] = 1;
										g_timecom_buffer[0]++;
									} // 月已是13，调整年，结束  
								} // 不是2月或不是闰年或本月不是29日，则调整到下个月1日，开始  
							} // 日已大于本月最大天数  
			               /**********zuoxin modify 2003.01.29***************/
			
			          	}
			       		else
			       	   	{
			       	     	g_timecom_buffer[3] = g_timecom_buffer[3]+8;
			           	}
					}
					else if ( (HUB_STATE&0x0F)==0x07 )		// 如果输入为北京时间
					{
						;	// 保持
					}
		         
				 /* if(g_gps_flag==0)		// wuchongxing   2005.07.06  
					{
						Write_Time();
						g_gps_flag=1;
						ADJUST_SEC=(g_timecom_buffer[5]+4);   	// 设置时间秒时，要加4 
					}  */

		   		}
			}
			else
			{
				g_errFlag = 0x1A;								// 外部GPS传输故障
				g_gps_True = 0x03;
			}
		    g_trans_flag_t = 1; 								// 数据传送标识置1；							
  		 	g_check_xor = 0x66; 								// 校验位恢复原值
		}
	   	else
	   	{   
	   		g_couter = 0; 
			g_check_xor = 0x66;
	   	}
	   
	}
	/*  2012-7-16,李毅堂修改,广利核要求本地模式不接受GPS校时信息,因此去掉此功能。*/
//  	else  if(((HUB_STATE&0x03)==0x01)&&(g_gps_flag==0))  		// 集线器为主机，本地时钟，但是又接了GPS的情况下的处理方式
//   	{      		      		
//		if(data_t2==36)								// head=$  $=36
//		{      	      	  
//			g_couter = 1;
//		}
//  		else if((g_couter==1)&&(data_t2==71))  		//G=71
//  	  	{      	  
//  	   		g_couter++;
//  	  	}	
//  		else if((g_couter==2)&&(data_t2==80))  		//P=80	
//  	  	{	      	        	    
//  	    	g_couter++;
//  	  	}  
//  		else if((g_couter==3)&&(data_t2==90))  		//Z=90
//  	  	{      	  
//  	   		g_couter++;
//  	  	}
//		else if((g_couter==4)&&(data_t2==68))  		//D=68
//		{      	  
//			g_couter++;
//		}
//		else if((g_couter==5)&&(data_t2==65))  		//A=65
//		{      	  
//			g_couter++;
//		}
//		else if((g_couter==6)&&(data_t2==44))  		//,=44
//		{      	  
//			g_couter++;
//		}     
//		else if((g_couter>6)&&(g_couter<13))     	//hour,minute,second
//		{      	  
//			g_adj[(g_couter-7)] = data_t2-0x30;
//			g_check_xor ^= g_adj[(g_couter-7)];
//			g_couter++;
//		}
//		else if(g_couter==13)  //&&(data_t2==0x46))	//.=
//		{
//			g_couter++;
//		}      	        	    	   
//		else if((g_couter>13)&&(g_couter<16))     	// sub_second
//		{      	  
//			g_couter++;
//		}
//		else if((g_couter==16)&&(data_t2==44))		//,=44
//		{
//			g_couter++;
//		} 
//		else if((g_couter>16)&&(g_couter<19))     	// day
//		{      	  
//			g_adj[(g_couter-11)] = data_t2-0x30;
//			g_check_xor ^= g_adj[(g_couter-11)];
//			g_couter++;
//		}
//		else if((g_couter==19)&&(data_t2==44))		//,=44
//		{
//			g_couter++;
//		} 
//		else if((g_couter>19)&&(g_couter<22))     	// month
//		{      	  
//			g_adj[(g_couter-12)] = data_t2-0x30;
//			g_check_xor ^= g_adj[(g_couter-12)];
//			g_couter++;
//		}
//		else if((g_couter==22)&&(data_t2==44))		//,=44
//		{
//			g_couter++;
//		} 
//		else if((g_couter>22)&&(g_couter<27))     	// year
//		{      	  
//			g_adj[(g_couter-13)] = data_t2-0x30;
//			g_check_xor ^= g_adj[(g_couter-13)];
//			g_couter++;
//		}
//		else if(((g_couter==27)||(g_couter==28))&&(data_t2==44))	//,=44
//		{
//			g_couter++;
//		}
//		else if(((g_couter==28)||(g_couter==29))&&(data_t2==48))	//0 =48
//		{
//																	//此处为空行.目的是兼容不同厂家的NMEA格式代码
//		}
//		else if((g_couter==29)&&(data_t2==42))		//*=42 0x2A 
//		{
//			g_couter++;
//		} 
//		else if(g_couter==30)						// 校验码
//		{
//			if ((data_t2)<58&&(data_t2>47))
//			{
//				check_temp = (data_t2-0x30)<<4;
//			}
//			else if ((data_t2)<71&&((data_t2)>64))
//			{
//				check_temp = (data_t2-0x37)<<4;
//			}
//			else if ((data_t2)<103&&((data_t2)>96))			// 校验码为小写字母:a b c d e f
//			{
//				check_temp = (data_t2-0x57)<<4;
//			}
//           	else
//			{
//				check_temp = 0x00;
//			}
//			g_couter++;
//		}
//  		else if((g_couter==31))
//  	  	{      	         	  	        		        	     
//  	        g_couter=0; 
//			     	                  
//			if ((data_t2)<58&&(data_t2>47))					// 校验码为数字
//			{
//				check_temp += (data_t2-0x30);
//			}
//			else if ((data_t2)<71&&((data_t2)>64))			// 校验码为大写字母:A B C D E F
//			{
//				check_temp += (data_t2-0x37);
//			}
//			else if ((data_t2)<103&&((data_t2)>96))			// 校验码为小写字母:a b c d e f
//			{
//				check_temp += (data_t2-0x57);
//			}
//           	else
//			{
//				check_temp = 0x00;
//			}
//
//			if (check_temp==g_check_xor)
//			{	
//				gps_sec = g_adj[4]*10+g_adj[5];
//				if (((gps_sec == 0x00)&&(g_gps_sec_pre == 59))||(gps_sec == g_gps_sec_pre + 1))
//				{
//					g_gps_True++;							// 判断，如果GPS时间秒信息是连续的，则使用其信息，否则不用
////					g_errFlag = 0x00;						// 此处不报告故障
//				}
//				else
//				{
//					g_gps_True = 0x02;
////					g_errFlag = 0x3A;						// 此处不报告故障
//
//				}
//				g_gps_sec_pre = gps_sec;	
//				if (g_gps_True>0x02)
//				{
//															// dongweijie 2008-10-20				     	                  
//					g_gps_True = 0x02;
//
//	           		g_time_buffer[0] = g_adj[10];
//	           		g_time_buffer[1] = g_adj[11];
//	           		g_time_buffer[2] = g_adj[12];
//	           		g_time_buffer[3] = g_adj[13];
//	           		g_time_buffer[4] = g_adj[8];
//	           		g_time_buffer[5] = g_adj[9];
//	           		g_time_buffer[6] = g_adj[6];
//	           		g_time_buffer[7] = g_adj[7];
//	           		g_time_buffer[8] = g_adj[0];
//	           		g_time_buffer[9] = g_adj[1];
//	           		g_time_buffer[10] = g_adj[2];
//	           		g_time_buffer[11] = g_adj[3];
//	           		g_time_buffer[12] = g_adj[4];
//	           		g_time_buffer[13] = g_adj[5];
//	          	
//	           		bcd_bin();      // bcd码时间转换成二进制时间 
//	           	
//	           		if(g_timecom_buffer[3]>15)
//					{
//						g_timecom_buffer[3]=(g_timecom_buffer[3]-16);
//						/**********zuoxin modify 2003.01.29***************/
//						g_timecom_buffer[2]++; // adjust day                                           
//						if (g_timecom_buffer[2]>month_table[(g_timecom_buffer[1]-1)])
//						{ // 日已大于本月最大天数 
//							if ((g_timecom_buffer[1]!=2)||!(leap_year_using1_t())||(g_timecom_buffer[2]!=29))                               
//							{ // 不是2月或不是闰年或本月不是29日，则调整到下个月1日，开始  
//								g_timecom_buffer[2] = 1;
//								g_timecom_buffer[1]++;
//								if (g_timecom_buffer[1]==13)
//								{ // 月已是13，调整年，开始 
//									g_timecom_buffer[1] = 1;
//									g_timecom_buffer[0]++;
//								} // 月已是13，调整年，结束  
//							} // 不是2月或不是闰年或本月不是29日，则调整到下个月1日，开始    
//						} // 日已大于本月最大天数 
//					/**********zuoxin modify end 2003.01.29***************/
//					}
//	           		else
//	           	   	{
//	           	     	g_timecom_buffer[3] = g_timecom_buffer[3]+8;
//	               	}
//		   			g_trans_flag_t = 1; 						// 数据传送标识置1；							
//	           	/*	wuchongxing   2005.07.06  */
//	            	Write_Time();
//	            	g_gps_flag = 1;
//				}
//  	    	}
//		    g_trans_flag_t = 1; 								// 数据传送标识置1；							
//			g_check_xor = 0x66; 							 	// 校验位恢复原值
//		}
//	   	else
//	   	{   
//	   		g_couter = 0; 
//			g_check_xor = 0x66;
//		}
//	}
	else
  	{
  	 	g_couter = 0;  
		g_check_xor = 0x66;
    }
  	
  	T2CON = 0x09;
  	EA = 1;
}

/****************************************************************
*函数功能：	 串口中断，用于接收外部时钟源信息					*
*中断优先级：highest，6											*
*说明：该串口中断程序接收GPS模块时间数据，并对其分析、处理		*
****************************************************************/
void int4_uart() interrupt 4 using 1 			// 串口中断程序入口    
{	
    uchar   data 		gps_sec_s;  			// 串口接收到的GPS对时"本次秒"数据;                                
	uchar   data 		check_temp_s;			// 存放校验码
	EA = 0;
	if(((HUB_STATE&0x07)==0x03)&&(RI==1))      	// 集线器为主机，接收GPS信息包,秒脉冲来自GPS刷新显示   	
   	{     								  		// GPS数据包的格式是： GPZDA+时间数据； 所以先判断出GPZADA，然后将后边的时间数据提取出来；		      		
		if(SBUF==36)  		 				    //$=36
		{      	      	  
			g_couter_s = 1;
		}
  		else if((g_couter_s==1)&&(SBUF==71))  	//G=71							  	
  	  	{      	  
  	   		g_couter_s++;
  	  	}	
  		else if((g_couter_s==2)&&(SBUF==80))  	//P=80	
  	  	{	      	        	    
  	    	g_couter_s++;
  	  	}  
  		else if((g_couter_s==3)&&(SBUF==90))  	//Z=90
  	  	{      	  
  	   		g_couter_s++;
  	  	}
		else if((g_couter_s==4)&&(SBUF==68))  	//D=68
		{      	  
			g_couter_s++;
		}
		else if((g_couter_s==5)&&(SBUF==65))  	//A=65
		{      	  
			g_couter_s++;
		}
		else if((g_couter_s==6)&&(SBUF==44))  	//,=44
		{      	  
			g_couter_s++;
		}     
		else if((g_couter_s>6)&&(g_couter_s<13))     	//hour,minute,second
		{      	  								
			g_adj_s[(g_couter_s-7)] = SBUF-0x30;
			g_check_xor_s ^= g_adj_s[(g_couter_s-7)];
			g_couter_s++;
		}
		else if(g_couter_s==13)  //&&(SBUF==0x46))		//.=
		{
			g_couter_s++;
		}      	        	    	   
		else if((g_couter_s>13)&&(g_couter_s<16))     	// sub_second
		{      	  
			g_couter_s++;
		}
		else if((g_couter_s==16)&&(SBUF==44))			//,=44
		{
			g_couter_s++;
		} 
		else if((g_couter_s>16)&&(g_couter_s<19))     	// day
		{      	  
			g_adj_s[(g_couter_s-11)] = SBUF-0x30;
			g_check_xor_s ^= g_adj_s[(g_couter_s-11)];
			g_couter_s++;
		}
		else if((g_couter_s==19)&&(SBUF==44))			//,=44
		{
			g_couter_s++;
		} 
		else if((g_couter_s>19)&&(g_couter_s<22))     	// month
		{      	  
			g_adj_s[(g_couter_s-12)] = SBUF-0x30;
			g_check_xor_s ^= g_adj_s[(g_couter_s-12)];
			g_couter_s++;
		}
		else if((g_couter_s==22)&&(SBUF==44))			//,=44
		{
			g_couter_s++;
		} 
		else if((g_couter_s>22)&&(g_couter_s<27))     	// year
		{      	  
			g_adj_s[(g_couter_s-13)] = SBUF-0x30;
			g_check_xor_s ^= g_adj_s[(g_couter_s-13)];
			g_couter_s++;
		}
		else if(((g_couter_s==27)||(g_couter_s==28))&&(SBUF==44))	//,=44
		{
			g_couter_s++;
		}
		else if(((g_couter_s==28)||(g_couter_s==29))&&(SBUF==48))	//0 =48
		{
														//此处为空行.目的是兼容不同厂家的NMEA格式代码
		}
		else if((g_couter_s==29)&&(SBUF==42))			//*=42 0x2A 
		{
			g_clr_count = 0;
			if(g_errFlag==0x5A&&g_gps_True_s==0x02)
			{
				g_errFlag = 0x00;	
			}

			g_couter_s++;
		} 
		else if(g_couter_s==30)							// 校验码
		{
			if ((SBUF)<58&&(SBUF>47))					// 校验码为数字
			{											
				check_temp_s = (SBUF-0x30)<<4;
			}
			else if ((SBUF)<71&&((SBUF)>64))			// 校验码为大写字母:A B C D E F
			{
				check_temp_s = (SBUF-0x37)<<4;			
			}
			else if ((SBUF)<103&&((SBUF)>96))			// 校验码为小写字母:a b c d e f
			{
				check_temp_s = (SBUF-0x57)<<4;
			}
			else
			{
				check_temp_s = 0x00;
			}
			g_couter_s++;
		}
		else if((g_couter_s==31))
  	  	{      	         	  	        		        	     
  	        g_couter_s=0; 
			     	                  
 			if ((SBUF)<58&&(SBUF>47))					// 校验码为数字
			{
				check_temp_s += (SBUF-0x30);
			}
			else if ((SBUF)<71&&((SBUF)>64))			// 校验码为大写字母:A B C D E F
			{
				check_temp_s += (SBUF-0x37);
			}
			else if ((SBUF)<103&&((SBUF)>96))			// 校验码为小写字母:a b c d e f
			{
				check_temp_s += (SBUF-0x57);
			}
           	else
			{
				check_temp_s = 0x00;
			}

			if (check_temp_s==g_check_xor_s)
			{											// dongweijie 2008-10-20							
			
			  	if (g_gps_True_s==0x02&&g_errFlag==0x4A)
				{
					g_errFlag = 0x00;			   		// 说明本次传输无故障,或者是故障恢复
				}			   		  

				gps_sec_s = g_adj_s[4]*10 + g_adj_s[5];
				if (((gps_sec_s == 0x00)&&(g_gps_sec_pre_s == 0x3B))||(gps_sec_s == g_gps_sec_pre_s + 1))
				{
					g_gps_True_s++;						// 判断，如果GPS时间秒信息是连续的，则使用其信息，否则不用
				}
				else
				{
					g_gps_True_s--;
					if(g_gps_True_s<0x02)
					{
						g_gps_True_s = 0x02;
						g_errFlag = 0x6A;	 			// 内部GPS设备故障
					}
				}
				g_gps_sec_pre_s = gps_sec_s;
					
				if (g_gps_True_s>0x02)
				{
					g_gps_True_s = 0x02;
					g_errFlag = 0x00;	 				// 内部GPS设备无故障

					g_time_buffer[0] = g_adj_s[10];
		           	g_time_buffer[1] = g_adj_s[11];
		           	g_time_buffer[2] = g_adj_s[12];
		           	g_time_buffer[3] = g_adj_s[13];
		           	g_time_buffer[4] = g_adj_s[8];
		           	g_time_buffer[5] = g_adj_s[9];
		           	g_time_buffer[6] = g_adj_s[6];
		           	g_time_buffer[7] = g_adj_s[7];
		           	g_time_buffer[8] = g_adj_s[0];
		           	g_time_buffer[9] = g_adj_s[1];
		           	g_time_buffer[10] = g_adj_s[2];
		           	g_time_buffer[11] = g_adj_s[3];
		           	g_time_buffer[12] = g_adj_s[4];
		           	g_time_buffer[13] = g_adj_s[5];
		          
		           	bcd_bin();        					// bcd码时间转换成二进制时间 
		           	
		           	if(g_timecom_buffer[3]>15)
					{
						g_timecom_buffer[3] = g_timecom_buffer[3]-16;
						/**********zuoxin modify 2003.01.29***************/
						g_timecom_buffer[2]++; //adjust day                                            
						if (g_timecom_buffer[2]>month_table[(g_timecom_buffer[1]-1)])
						{   // 日已大于本月最大天数 
							if ((g_timecom_buffer[1]!=2)||!(leap_year_using1_s())||(g_timecom_buffer[2]!=29))                               
							{ // 不是2月或不是闰年或本月不是29日，则调整到下个月1日，开始  
								g_timecom_buffer[2] = 1;
								g_timecom_buffer[1]++;
								if (g_timecom_buffer[1]==13)
								{ // 月已是13，调整年，开始  
									g_timecom_buffer[1] = 1;
									g_timecom_buffer[0]++;
								} // 月已是13，调整年，结束  
							} // 不是2月或不是闰年或本月不是29日，则调整到下个月1日，开始  
						} // 日已大于本月最大天数  
		               /**********zuoxin modify 2003.01.29***************/
		
		          	}
		       		else
		       	   	{
		       	     	g_timecom_buffer[3] = g_timecom_buffer[3]+8;
		           	}
		         /* wuchongxing   2005.07.06 
		           	if(g_gps_flag==0)
					{
						Write_Time();
						g_gps_flag=1;
						ADJUST_SEC=(g_timecom_buffer[5]+4);   // 设置时间秒时，要加4   
					} */
				}
			}
			else
			{
				g_errFlag = 0x4A;						// 内部GPS传输故障
				g_gps_True_s = 0x03;
			}
		    g_trans_flag_t = 1; 									// 数据传送标识置1；							
			g_check_xor_s = 0x66; 						// 校验位恢复原值 0x66
  	   }
	   else
	   {   
	   		g_couter_s = 0; 
			g_check_xor_s = 0x66; 						// 校验位恢复原值 0x66
	   }
	   
	}

	RI = 0; 
	EA = 1; 
}

/****************************************************************
*函数功能：	 外部中断0，用于处理分脉冲信息						*
*中断优先级：lowest，1											*
*说明：		每分钟都将CPLD内部的分钟计数器清零					*
****************************************************************/
void int0_mp() interrupt 0 using 2  					//中断程序入口 
{

}

/****************************************************************
*函数功能：	 T0的外部计数溢出中断，用于处理秒脉冲信息			*
*中断优先级：lowest，2											*
*说明：		每一秒单片机计数并调整时间信息，做为本地时钟		*
****************************************************************/
void timer0_sp() interrupt 1 using 2  					//T0中断程序入口 
{   
	if(g_adjust_flag==0&&((HUB_STATE&0x03)==0x01||(((HUB_STATE&0x03)==0x03||((HUB_STATE&0x01)==0x00))&&g_errFlag!=0x00)))    							// 主时钟
	//  非手动设置状态  &&(  1 （主+本地时钟）		||	2（	  （ 主+GPS/内部或外部	||  从状态）&& 有错误）	  )
	{   
		time_adj();
		g_trans_flag=1; 								// 数据传送标识置1；         
	}      
}

/****************************************************************
*函数功能：	 T1 中断，产生10ms的计时中断 						*
*中断优先级：lowest，4											*
*说明：		在有按键输入时产生数码管的闪烁效果					*
****************************************************************/
void time_10ms() interrupt 3 using 2  					// T1中断程序入口 
{
	uchar data temp1,temp2;

	if ((HUB_STATE&0x01)==0) 							// 防止在从状态下进入该状态
	{
		TR1 = 0;
		g_adjust_flag = 0;                   
		g_couter_adj = 0;   							// 指针返回指向头（年）            
		g_led_standby = 1;
		
	}

	EA = 0;
	g_number_dog = 0;
	TH1 = 0x15;         								//	TH1 = -(60000/256); 延迟约10ms
	TL1 = 0xA0;											//	TL1 = -(60000%256);
	if (g_couter_10ms==12)
	{
		g_couter_10ms = 0; 
		if(g_spark_flag==0)
		{
			g_spark_flag = ~g_spark_flag ;
			if(g_couter_adj==0)
			{
				g_time_buffer[0] = 0x0a;  				// 熄灭相应的时间单元
				g_time_buffer[1] = 0x0a;
			} 
			temp1 = g_time_buffer[(2*g_couter_adj+2)];
			temp2 = g_time_buffer[(2*g_couter_adj+3)];
			g_time_buffer[(2*g_couter_adj+2)] = 0x0a;  	// 熄灭相应的时间单元 
			g_time_buffer[(2*g_couter_adj+3)] = 0x0a;
			time_dis(); 								// 显示 
			display_3();
			if(g_couter_adj==0)
			{
				g_time_buffer[0] = 2;   
				g_time_buffer[1] = 0;
			} 
			g_time_buffer[(2*g_couter_adj+2)] = temp1;
			g_time_buffer[(2*g_couter_adj+3)] = temp2;              
		}
		else
		{
			g_spark_flag = ~g_spark_flag ; 
			time_dis(); 
			display_3();
		}
	}
	else
	g_couter_10ms = g_couter_10ms+1;
	EA = 1; 
}

/************************************************************************
* 函数功能：读按键信息													*	
* 参数说明：无															*
*																		*
* 返回值：	按键的键值													*
************************************************************************/
uchar dekey()  using 2
{
	uchar data k;
	k = DAT_8279;     									// 从CPLD中读取按键信息，模仿8279的功能
	return(k&0x0f); 
}
/************************************************************************
* 函数功能：按下SET键时的处理											*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void  key_set() using 2 
{
	if (g_adjust_flag==0)
	{
		g_adjust_flag = 1;
		g_couter_adj = 0;    							// 指针指向头（年）         
		TR1=1 ;
	}
	else  												//(g_adjust_flag=1)
	{            
		bin_bcd(); 	//bcd_bin ( );    					// bcd码时间转换成二进制时间
		ADJUST_SEC = g_timecom_buffer[5]+4;	            // 设置时间秒时，要加4
		Write_Time(); 
		g_adjust_flag = 0;                   
		g_couter_adj = 0;   							// 指针返回指向头（年）            
		TR1 = 0;
	} 
	return;           
}

/************************************************************************
* 函数功能：按下SEL键时的处理											*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void  key_sel() using 2 
{
	if (g_adjust_flag==1)
	{          
		g_couter_adj++;  
		if(g_couter_adj==6) 							// 已指向末尾（秒）             
		{
			g_couter_adj = 0; 							// 指针返回指向头（年）
		}                     
	}
	return;         
}

/************************************************************************
* 函数功能：按下UP键时的处理											*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void  key_up() using 2 
{
	if (g_adjust_flag==1)
	{
		g_timecom_buffer[g_couter_adj]++;
		if(g_couter_adj==0) 							//year 0~99 
		{
			if (g_timecom_buffer[0]>99) 
			g_timecom_buffer[0] = 0;                  
		} 
		else if (g_couter_adj==1)  						//month 1~12 
		{
			if (g_timecom_buffer[1]>12) 
			g_timecom_buffer[1] = 1;
		}            
		else if (g_couter_adj==2)  						//day 1~31 
		{
			if (g_timecom_buffer[2]>month_table[(g_timecom_buffer[1]-1)])
			{ 											//日已大于本月最大天数//
				if ((g_timecom_buffer[1]!=2)||!(leap_year_using2_i())||(g_timecom_buffer[2]!=29))                               
				{ 										//不是2月或不是闰年或不是29日，则调整到1日 // 
					g_timecom_buffer[2] = 1;
				}
			}             
		}             
		else if (g_couter_adj==3)  						// hour 0-23//
		{
			if (g_timecom_buffer[3]>23)
			g_timecom_buffer[3] = 0;               
		}
		else  											//min,sec 0-59 //
		{
			if (g_timecom_buffer[g_couter_adj]>59)
			g_timecom_buffer[g_couter_adj] = 0; 
		}                         
	
	} 													// (g_adjust_flag=0),nop // 
	return;          
}

/************************************************************************
* 函数功能：按下DOWN键时的处理											*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void  key_down() using 2 
{
	if (g_adjust_flag==1)        
	{          
		if(g_couter_adj==0)  							//year 0~99  //
		{
			if (g_timecom_buffer[0]==0) 
			{
				g_timecom_buffer[0] = 99;
			} 
			else 
			{
				g_timecom_buffer[g_couter_adj]--; 
			}              
		} 
		else if (g_couter_adj==1)  						//month 1~12//
		{
			if (g_timecom_buffer[1]==1) 
			{
				g_timecom_buffer[1] = 12;
			}
			else 
			{
				g_timecom_buffer[g_couter_adj]--;
			}
		}            
		else if (g_couter_adj==2)  						// day 1~31 //
		{
			if (g_timecom_buffer[2]==1)
			{
				if ((g_timecom_buffer[1]==2)&&(leap_year_using2_i()))                               
				{ 										//是2月而且是闰年，则调整到29日// 
					g_timecom_buffer[2] = 29;
				}               
				else	
				{
					g_timecom_buffer[2] = month_table[(g_timecom_buffer[1]-1)]; 
				}
			}
			else 
			{
				g_timecom_buffer[g_couter_adj]--; 
			} 
		}    
	          
		else if (g_couter_adj==3) 				 		// hour 0-23 //
		{
			if (g_timecom_buffer[3]==0)
			{
				g_timecom_buffer[3] = 23; 
			}
			else 
			{
				g_timecom_buffer[g_couter_adj]--;  
			}            
		}
		else   											// min,sec 0-59 //
		{
			if (g_timecom_buffer[g_couter_adj]==0)
			{
				g_timecom_buffer[g_couter_adj] = 59; 
			}
			else 
			{
				g_timecom_buffer[g_couter_adj]--;
			}
		}                         
	
	} 													// (g_adjust_flag=0),nop // 
	return;          
}

/****************************************************************
*函数功能：	 外部中断1，响应有按键信息时产生中断				*
*中断优先级：lowest，3											*
*说明：		消抖部分在CPLD中实现，此中断中读取按键信息			*
****************************************************************/
void int1_key() interrupt 2 using 2  					// int1中断程序入口 
{
	uchar data i;
	EA=0;
	if (dekey()!=0x0f)       							// 返回值如果不为零，意味着有按键按下
	{
		i = DAT_8279&0x0f; 
		switch(i)
		{
			case KEY_SET:   key_set();    break;
			case KEY_SEL:   key_sel();    break;
			case KEY_UP:    key_up();     break;
			case KEY_DOWN:  key_down();   break;
			default: break;
		} 
		bin_bcd();  									// 刷新BCD码格式的时间 
		time_dis();  									// 显示
		display_2();
	}    
	EA = 1;
}

/************************************************************************
* 函数功能：串口发送一个字节的数据										*	
* 参数说明：无															*
*																		*
* 返回值：																*
************************************************************************/
void Uart_SendChar(uchar data dat)
{
	SBUF = dat;											// 发送一个字节										
	while (TI == 0);
	TI = 0;	
	return; 
}

/************************************************************************
* 函数功能：GPS-M12初始化，将M12设置成NMEA格式输出,并且只输出GPZDA码	*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void Init_GPS_M12()
{
	Uart_SendChar(0x40);	// 40 40 43 69 01 2B 0D 0A 
	Uart_SendChar(0x40);	// @@Ci1C<CR><LF>
	Uart_SendChar(0x43);	// 将M12设置成NMEA格式输出,并且只输出GPZDA码
	Uart_SendChar(0x69);
	Uart_SendChar(0x01);
	Uart_SendChar(0x2B);
	Uart_SendChar(0x0D);
	Uart_SendChar(0x0A);

	Uart_SendChar(0x24);	// 24 50 4D 4F 54 47 2C 47 47 41 2C 30 0D 0A 
	Uart_SendChar(0x50);	// $PMOTG,GGA,0 禁止输出GPGGA格式编码
	Uart_SendChar(0x4D);	
	Uart_SendChar(0x4F);
	Uart_SendChar(0x54);
	Uart_SendChar(0x47);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x47);
	Uart_SendChar(0x47);
	Uart_SendChar(0x41);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x0D);
	Uart_SendChar(0x0A);

	Uart_SendChar(0x24);	// 24 50 4D 4F 54 47 2C 47 4C 4C 2C 30 0D 0A 
	Uart_SendChar(0x50);	// $PMOTG,GLL,0 禁止输出GPGLL格式编码
	Uart_SendChar(0x4D);	
	Uart_SendChar(0x4F);
	Uart_SendChar(0x54);
	Uart_SendChar(0x47);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x47);
	Uart_SendChar(0x4C);
	Uart_SendChar(0x4C);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x0D);
	Uart_SendChar(0x0A);

	Uart_SendChar(0x24);	// 24 50 4D 4F 54 47 2C 47 53 41 2C 30 0D 0A  
	Uart_SendChar(0x50);	// $PMOTG,GSA,0 禁止输出GPGSA格式编码
	Uart_SendChar(0x4D);	
	Uart_SendChar(0x4F);
	Uart_SendChar(0x54);
	Uart_SendChar(0x47);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x47);
	Uart_SendChar(0x53);
	Uart_SendChar(0x41);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x0D);
	Uart_SendChar(0x0A);

	Uart_SendChar(0x24);	// 24 50 4D 4F 54 47 2C 47 53 56 2C 30 0D 0A  
	Uart_SendChar(0x50);	// $PMOTG,GSA,0 禁止输出GPGSV格式编码
	Uart_SendChar(0x4D);	
	Uart_SendChar(0x4F);
	Uart_SendChar(0x54);
	Uart_SendChar(0x47);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x47);
	Uart_SendChar(0x53);
	Uart_SendChar(0x56);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x0D);
	Uart_SendChar(0x0A);

	Uart_SendChar(0x24);	// 24 50 4D 4F 54 47 2C 52 4D 43 2C 30 0D 0A   
	Uart_SendChar(0x50);	// $PMOTG,RMC,0 禁止输出GPRMC格式编码
	Uart_SendChar(0x4D);	
	Uart_SendChar(0x4F);
	Uart_SendChar(0x54);
	Uart_SendChar(0x47);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x52);
	Uart_SendChar(0x4D);
	Uart_SendChar(0x43);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x0D);
	Uart_SendChar(0x0A);

	Uart_SendChar(0x24);	// 24 50 4D 4F 54 47 2C 56 54 47 2C 30 0D 0A    
	Uart_SendChar(0x50);	// $PMOTG,VTG,0 禁止输出GPVTG格式编码
	Uart_SendChar(0x4D);	
	Uart_SendChar(0x4F);
	Uart_SendChar(0x54);
	Uart_SendChar(0x47);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x56);
	Uart_SendChar(0x54);
	Uart_SendChar(0x47);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x0D);
	Uart_SendChar(0x0A);

	Uart_SendChar(0x24);	// 24 50 4D 4F 54 47 2C 5A 44 41 2C 31 0D 0A    
	Uart_SendChar(0x50);	// $PMOTG,ZDA,1 允许输出GPZDA格式编码
	Uart_SendChar(0x4D);	
	Uart_SendChar(0x4F);
	Uart_SendChar(0x54);
	Uart_SendChar(0x47);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x5A);
	Uart_SendChar(0x44);
	Uart_SendChar(0x41);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x31);
	Uart_SendChar(0x0D);
	Uart_SendChar(0x0A);
	
	return;	
}

/************************************************************************
* 函数功能：将1000-U(Ublox)设置成NMEA格式输出,并且只输出GPZDA码			*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void Init_GPS_Ublox()
{
	Uart_SendChar(0x24);	// 24 50 55 42 58 2C 34 30 2C 5A 44 41 2C 30 2C 31 2C 30 2C 30 2A 34 35	0D 0A
	Uart_SendChar(0x50);	// $PUBX,40,ZDA,0,1,0,0*45
	Uart_SendChar(0x55);	// 将1000-U设置成NMEA格式输出,输出GPZDA码
	Uart_SendChar(0x42);
	Uart_SendChar(0x58);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x34);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x5A);  	// Z
	Uart_SendChar(0x44);  	// D
	Uart_SendChar(0x41);  	// A
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x31);	
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2A);
	Uart_SendChar(0x34); 	// CHECKSUM 45
	Uart_SendChar(0x35);	
	Uart_SendChar(0x0D);   
	Uart_SendChar(0x0A);   

	Uart_SendChar(0x24);	// 24 50 55 42 58 2C 34 30 2C 52 4D 43 2C 30 2C 31 2C 30 2C 30 2A 34 37	0D 0A
	Uart_SendChar(0x50);	// $PUBX,40,RMC,0,0,0,0*47
	Uart_SendChar(0x55);	
	Uart_SendChar(0x42);
	Uart_SendChar(0x58);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x34);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x52);  	// R
	Uart_SendChar(0x4D);  	// M
	Uart_SendChar(0x43);  	// C
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);	
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2A);
	Uart_SendChar(0x34); 	// CHECKSUM 47
	Uart_SendChar(0x37);	
	Uart_SendChar(0x0D);   
	Uart_SendChar(0x0A);   

	Uart_SendChar(0x24);	// 24 50 55 42 58 2C 34 30 2C 47 53 54 2C 30 2C 31 2C 30 2C 30 2A 35 42	0D 0A
	Uart_SendChar(0x50);	// $PUBX,40,GST,0,0,0,0*5B
	Uart_SendChar(0x55);	
	Uart_SendChar(0x42);
	Uart_SendChar(0x58);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x34);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x47);  	// G
	Uart_SendChar(0x53);  	// S
	Uart_SendChar(0x54);  	// T
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);	
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2A);
	Uart_SendChar(0x35); 	// CHECKSUM 5B
	Uart_SendChar(0x42);	
	Uart_SendChar(0x0D);   
	Uart_SendChar(0x0A);   

	Uart_SendChar(0x24);	// 24 50 55 42 58 2C 34 30 2C 47 53 56 2C 30 2C 31 2C 30 2C 30 2A 35 39	0D 0A
	Uart_SendChar(0x50);	// $PUBX,40,GSV,0,0,0,0*59
	Uart_SendChar(0x55);	
	Uart_SendChar(0x42);
	Uart_SendChar(0x58);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x34);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x47);  	// G
	Uart_SendChar(0x53);  	// S
	Uart_SendChar(0x56);  	// V
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);	
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2A);
	Uart_SendChar(0x35); 	// CHECKSUM 59
	Uart_SendChar(0x39);	
	Uart_SendChar(0x0D);   
	Uart_SendChar(0x0A);   

	Uart_SendChar(0x24);	// 24 50 55 42 58 2C 34 30 2C 47 4C 4C 2C 30 2C 31 2C 30 2C 30 2A 35 43	0D 0A
	Uart_SendChar(0x50);	// $PUBX,40,GLL,0,0,0,0*5C
	Uart_SendChar(0x55);	
	Uart_SendChar(0x42);
	Uart_SendChar(0x58);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x34);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x47);  	// G
	Uart_SendChar(0x4C);  	// L
	Uart_SendChar(0x4C);  	// L
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);	
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2A);
	Uart_SendChar(0x35); 	// CHECKSUM 5C
	Uart_SendChar(0x43);	
	Uart_SendChar(0x0D);   
	Uart_SendChar(0x0A);   

	Uart_SendChar(0x24);	// 24 50 55 42 58 2C 34 30 2C 47 4C 4C 2C 30 2C 31 2C 30 2C 30 2A 34 45	0D 0A
	Uart_SendChar(0x50);	// $PUBX,40,GSA,0,0,0,0*4E
	Uart_SendChar(0x55);	
	Uart_SendChar(0x42);
	Uart_SendChar(0x58);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x34);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x47);  	// G
	Uart_SendChar(0x53);  	// S
	Uart_SendChar(0x41);  	// A
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);	
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2A);
	Uart_SendChar(0x34); 	// CHECKSUM 4E
	Uart_SendChar(0x45);	
	Uart_SendChar(0x0D);   
	Uart_SendChar(0x0A);   

	Uart_SendChar(0x24);	// 24 50 55 42 58 2C 34 30 2C 56 54 47 2C 30 2C 31 2C 30 2C 30 2A 35 45	0D 0A
	Uart_SendChar(0x50);	// $PUBX,40,VTG,0,0,0,0*5E
	Uart_SendChar(0x55);	
	Uart_SendChar(0x42);
	Uart_SendChar(0x58);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x34);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x56);  	// V
	Uart_SendChar(0x54);  	// T
	Uart_SendChar(0x47);  	// G
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);	
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2A);
	Uart_SendChar(0x35); 	// CHECKSUM 5E
	Uart_SendChar(0x45);							  
	Uart_SendChar(0x0D);   
	Uart_SendChar(0x0A);   

	Uart_SendChar(0x24);	// 24 50 55 42 58 2C 34 30 2C 47 4C 4C 2C 30 2C 31 2C 30 2C 30 2A 35 44	0D 0A
	Uart_SendChar(0x50);	// $PUBX,40,GRS,0,0,0,0*5D
	Uart_SendChar(0x55);	
	Uart_SendChar(0x42);
	Uart_SendChar(0x58);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x34);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x47);  	// G
	Uart_SendChar(0x52);  	// R
	Uart_SendChar(0x53);  	// S
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);	
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2C);
	Uart_SendChar(0x30);
	Uart_SendChar(0x2A);
	Uart_SendChar(0x35); 	// CHECKSUM 5D
	Uart_SendChar(0x44);	
	Uart_SendChar(0x0D);   
	Uart_SendChar(0x0A);   									
                          	        
	Uart_SendChar(0xB5);	// B5 62 06 09 0C 00 00 00 00 00 FF FF FF FF 00 00 00 00 17 75
	Uart_SendChar(0x62);	// 二进制命令，保存上面所进行的设置
	Uart_SendChar(0x06);
	Uart_SendChar(0x09);
	Uart_SendChar(0x0C);
	Uart_SendChar(0x00);
	Uart_SendChar(0x00);
	Uart_SendChar(0x00);
	Uart_SendChar(0x00);
	Uart_SendChar(0x00);
	Uart_SendChar(0xFF);
	Uart_SendChar(0xFF);
	Uart_SendChar(0xFF);
	Uart_SendChar(0xFF);
	Uart_SendChar(0x00);
	Uart_SendChar(0x00);
	Uart_SendChar(0x00);
	Uart_SendChar(0x00);
	Uart_SendChar(0x17);
	Uart_SendChar(0x75);
	Uart_SendChar(0x0D);   
	Uart_SendChar(0x0A);   
	
	return;
}

/************************************************************************
* 函数功能：初始化CPU													*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void Init_CPU()
{
	CKCON0 = 0x01;							// X2 模式
	IT0 = 1;           						// 外部中断INT0，INT1均为边沿触发
	IT1 = 1;  
	TMOD = 0x16 ;      						// T1定时方式1，T0外部计数方式2       

	TH0 = 0xFF; 
	TL0 = 0xFF;        						// 计数初值重装载 
	TR0 = 1 ;         						// 启动T0计数 ，禁止T1计数 

	TH1 = 0x15;         					//	TH1 = -(60000/256); 延迟约10ms
	TL1 = 0xA0;								//	TL1 = -(60000%256);
								   	  	
	T2CON = 0x09;     						// 外部捕获方式，不须起动计数
	
	SCON = 0xD0; 							// 串口工作在方式1，8位数据位，1位停止位，无校验位
	
 	if ((HUB_STATE&0x08)==0)  				// =0 用波特率4800，因为M12中NMEA格式用的波特率为4800      
	{
		BRL = 0x64;							// 0X64 = 100 波特率4800,误差为0.16%;
		BDRCON = 0x1E; 						// 0001,1110串口接收与发送的波特率均由波特率发生器产生,SPD=1,SMOD1=0
	 	Init_GPS_M12();						// M12的NMEA格式用的是4800波特率
	}
	else									// =1 用波特率9600
	{
		BRL = 0xB2; 						// 0XB2 = 178 波特率9600,误差为0.16%;
		BDRCON = 0x1E; 						// 0001,1110串口接收与发送的波特率均由波特率发生器产生,SPD=1,SMOD1=0
		Init_GPS_Ublox();				  	// Ublox nmea格式用的是9600波特率 
	}  
	          
	IPH0 = 0x30;       						// 接收中断T2 TS为高级中断*/
	IPL0 = 0x30;
	IEN0 = 0x3e;     						// CPU关中断，允许中断  EA EC ET2 ES , ET1 EX1 ET0 EX0*/ 
	
	return;
}

/************************************************************************
* 函数功能：主函数														*	
* 参数说明：无															*
*																		*
* 返回值：	无															*
************************************************************************/
void main() using 0
{
	uchar data 		i;									// 循环用变量
	uchar data 		check_temp;							// 临时存放校验码
	uchar data		hub_state_pre = HUB_STATE&0x0F;			// 记录拨码开关的状态

	Init_CPU();
	Init_Pc16552D();
	Init_3231();
	Init_X5045();

	if ((HUB_STATE&0x03)==1)  							// =0，灯亮      
	{
		g_led_standby=0;
	}   
	else 
	{
		g_led_standby=1;   								// =1，灯灭    
	}

	display_0();     									// 上电时，所有的数码管全显示 8，方便检验数码管是否能够正常显示
	delay_L(); 

	g_time_buffer[0]=2; 								// 固定显示20
	g_time_buffer[1]=0; 
	Read_Time();     									// 上电读时间

	bcd_bin();      									// bcd码时间转换成二进制时间
	ADJUST_SEC=(g_timecom_buffer[5]+4);					// 设置时间秒时，要加4 

	time_dis();
	display_0(); 

	EA=1; 

	while(1)
	{    
		if(g_trans_flag==1||g_trans_flag_t==1) 
		{         
			do 											// 判断UART16552芯片是否可以发送数据；
			{
				g_adjust = Uart1_Lsr&0x60;
				g_adjust2 = Uart2_Lsr&0x60;
			}
			while((g_adjust!=0x60)||(g_adjust2!=0x60));  
			Uart1_Thr = 0xaa;				   			// AA 做为发送数据的标头；
			Uart2_Thr = 0xaa;
			delay();        
			
			for(i=0;i<6;i++)							// 循环发送年月日时分秒信息；
			{
				do 										// 判断UART16552芯片是否可以发送数据；
				{ 
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));       

				Uart1_Thr = g_timecom_buffer[i];         
				Uart2_Thr = g_timecom_buffer[i];         
				delay();        
			} 	

			check_temp = g_timecom_buffer[0];   
			for(i=1;i<6;i++)
			{ 
				check_temp = check_temp^g_timecom_buffer[i];	// 异或校验；
			}        
			do 													// 判断UART16552芯片是否可以发送数据；
			{
				g_adjust = Uart1_Lsr&0x60;
				g_adjust2 = Uart2_Lsr&0x60;
			}
			while((g_adjust!=0x60)||(g_adjust2!=0x60));

			Uart1_Thr = check_temp;								// 发送校验位；
			Uart2_Thr = check_temp; 
			delay(); 
			do 													// 判断UART16552芯片是否可以发送数据；
			{
				g_adjust = Uart1_Lsr&0x60;
				g_adjust2 = Uart2_Lsr&0x60;
			}
			while((g_adjust!=0x60)||(g_adjust2!=0x60));
			Uart1_Thr = 0xbb;									// BB做为发送数据的结束；
			Uart2_Thr = 0xbb;
			delay();
		// 处理时间信息        
			if (g_timecom_buffer[5]!=0)							// 将时间秒写入CPLD秒寄存器
			{
				ADJUST_SEC = g_timecom_buffer[5]+4;
			}
			if (g_timecom_buffer[5]==0)					
			{											 
				ADJUST_SEC = g_timecom_buffer[5]+64;			// 如果时间为第60秒,则写入64: 0100,0000,第七位产生分脉冲
				if (g_trans_flag_t==1)							// 如果为非"主+本地时钟"方式,则肯定为用GPS方式或级联方式,每分钟都对RTC写一次
				{
					Write_Time();
				}
			}
			bin_bcd();    										// 刷新BCD码格式的时间 
			time_dis();   										// 段码数据从时间寄存器到显示寄存器；    
			display_0();    									// 传送显示数据

			if(((g_errFlag>0x0A)&&(g_errFlag<0x4A)&&((HUB_STATE&0x07)==0x07))	// 主+外部GPS
		       ||((g_errFlag>0x3A)&&(g_errFlag<0x7A)&&((HUB_STATE&0x07)==0x03))	// 主+内部GPS
				 ||((g_errFlag>0x6A)&&((HUB_STATE&0x01)==0x00)))// 故障代码产生,判断其是否合理,如果合理,连续上报故障故障代码
			{		
				do 												// 判断UART16552芯片是否可以发送数据；
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0xaa;				   				// AA 做为发送数据的标头；
				Uart2_Thr = 0xaa;
				delay();
			
				do 												// 判断UART16552芯片是否可以发送数据；
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0x55;				   				// 0x55 0xCC 表示有故障；
				Uart2_Thr = 0x55;
				delay();
			
				do 												// 判断UART16552芯片是否可以发送数据；
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0xCC;				   				// 0x55 0xCC 表示有故障；
				Uart2_Thr = 0xCC;
				delay();
			
				do 												// 判断UART16552芯片是否可以发送数据；
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = g_errFlag;				   			// 发送故障标志g_errflag；
				Uart2_Thr = g_errFlag;
				delay();
	
				do 												// 判断UART16552芯片是否可以发送数据；
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0x99^g_errFlag;				   		// 发送异或校验位（AA和BB之间的数据的异或校验）；
				Uart2_Thr = 0x99^g_errFlag;
				delay();
	
				do 												// 判断UART16552芯片是否可以发送数据；
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0XBB;				   				// 发送结束标志BB；
				Uart2_Thr = 0XBB;
				delay();
	
			   	g_errReport = 0x03;  							// 故障已经上报
				g_errFlagReg = g_errFlag;						// 保存故障类型，在故障恢复上报信息中使用到
				g_led_standby = 1;   							// =1，灯灭    	          2009-03-30      dongweijie
			}
			else
			{
				g_errFlag = 0x00;
			}

/*			if(g_errFlag>0x3A)&&(g_errFlag<0x7A)
			{										   	
 				if ((HUB_STATE&0x08)==0)  						// =0 用波特率4800，因为M12中NMEA格式用的波特率为4800      
				{
					BRL = 0x64;									// 0X64 = 100 波特率4800,误差为0.16%;
					BDRCON = 0x1E; 								// 0001,1110串口接收与发送的波特率均由波特率发生器产生,SPD=1,SMOD1=0
	 				Init_GPS_M12();								// M12的NMEA格式用的是4800波特率
				}
				else											// =1 用波特率9600
				{
					BRL = 0xB2; 								// 0XB2 = 178 波特率9600,误差为0.16%;
					BDRCON = 0x1E; 								// 0001,1110串口接收与发送的波特率均由波特率发生器产生,SPD=1,SMOD1=0
					Init_GPS_Ublox();				  			// Ublox nmea格式用的是9600波特率 
				}  
			}  */

			if((g_errFlag==0x00)&&(g_errReport!=0)
				&&(((g_errFlagReg>0x0A)&&(g_errFlagReg<0x4A)&&((HUB_STATE&0x07)==0x07))  // 主+外部GPS
		       		||((g_errFlagReg>0x3A)&&(g_errFlagReg<0x7A)&&((HUB_STATE&0x07)==0x03))// 主+内部GPS
				 	||((g_errFlagReg>0x6A)&&((HUB_STATE&0x01)==0x00))))				// 故障已经解除，连续3次上报恢复信息
			{																		// 此处防止误报恢复信息,尤其在拨码开关改变的情况下
				do 												// 判断UART16552芯片是否可以发送数据；
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0xaa;				   				// AA 做为发送数据的标头；
				Uart2_Thr = 0xaa;
				delay();
			
				do 												// 判断UART16552芯片是否可以发送数据；
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0x55;				   				// 0x55 0xCC 表示有故障；
				Uart2_Thr = 0x55;
				delay();
			
				do 												// 判断UART16552芯片是否可以发送数据；
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0xCC;				   				// 0x55 0xCC 表示有故障；
				Uart2_Thr = 0xCC;
				delay();
			
				do 												// 判断UART16552芯片是否可以发送数据；
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = g_errFlagReg + 1;				   	// 发送故障恢复标志 g_errflag+1；
				Uart2_Thr = g_errFlagReg + 1;
				delay();
	
				do 												// 判断UART16552芯片是否可以发送数据；
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0x99^(g_errFlagReg + 1);			// 发送异或校验位（AA和BB之间的数据的疑惑校验）；
				Uart2_Thr = 0x99^(g_errFlagReg + 1);
				delay();
		
				do 												// 判断UART16552芯片是否可以发送数据；
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0XBB;				   				// 发送结束标志BB；
				Uart2_Thr = 0XBB;
				delay();
	
			   	g_errReport--;  								// 故障处理情况标志复位			
			}
		
			if((g_trans_flag_t==1)&&(g_errFlag==0))
			{
				g_led_standby=~g_led_standby;      				//  主集线器用本地时钟，闪灯 05.07.15   2009-03-30 dongweijie
			} 

			g_trans_flag = 0;
			g_trans_flag_t = 0;
			g_number_dog = 0;					
		} 
							
		Reset_Watchdog();									// 喂看门狗

		if (hub_state_pre != (HUB_STATE & 0x0F))			// 当发现拨码开关改变时
		{
			if ((HUB_STATE&0x03)==1)  							      
			{												// =0，灯亮
				g_led_standby = 0;
			}
			else
			{
				g_led_standby = 1;   						// =1，灯灭    
			} 
 			if ((HUB_STATE&0x08)==0)  						// =0 用波特率4800，因为M12中NMEA格式用的波特率为4800      
			{
				BRL = 0x64;									// 0X64 = 100 波特率4800,误差为0.16%;
				BDRCON = 0x1E; 								// 0001,1110串口接收与发送的波特率均由波特率发生器产生,SPD=1,SMOD1=0
	 			Init_GPS_M12();								// M12的NMEA格式用的是4800波特率
			}
			else											// =1 用波特率9600
			{
				BRL = 0xB2; 								// 0XB2 = 178 波特率9600,误差为0.16%;
				BDRCON = 0x1E; 								// 0001,1110串口接收与发送的波特率均由波特率发生器产生,SPD=1,SMOD1=0
				Init_GPS_Ublox();				  			// Ublox nmea格式用的是9600波特率 
			}  
			hub_state_pre = (HUB_STATE&0x0F); 
		} 

		g_clr_count++;

		if(g_clr_count==60000)									// 如果长时间未发现接受中断，则认为连接故障
		{
			if ((HUB_STATE&0x07)==0x03)
			{
				g_errFlag = 0x5A;								// 内部GPS连接故障
				g_gps_True_s = 0x03;							// 清除设备故障计数器
			}
			if ((HUB_STATE&0x07)==0x07)
			{
				g_errFlag = 0x2A;								// 外部GPS连接故障
				g_gps_True = 0x03;								// 清除设备故障计数器
			}
			if ((HUB_STATE&0x01)==0x00)
			{
				g_errFlag = 0x8A;								// 级联工作时连接故障
				g_gps_True = 0x03;								// 清除设备故障计数器
			}
			g_clr_count = 0;
			g_number_dog++;
		}

		if(g_number_dog>5)
		{
			while(1)
			{
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
			}
		}
	}/* */        
}