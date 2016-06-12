/************************************************************************************
*	CopyRight(c) 2006, HollySys														*
*	All rights reserved.															*
*																					*
*	Ŀ���ļ��İ汾��NB197-U40-01.hex,�ɹ���NB197-U40-01.Uv2��������,				*
*																					*
*	�ļ����ƣ�NB197-U40-01.c        														*
*																					*
*	�ļ��汾��A01       															*
*																					*
** ժҪ(Module Description):														*
	��Դ������ʵ�ֵĹ���˵����														*
	������ʵ�֣������������õĲ�ͬ,�����GPSУʱ��Ϣ,����ñ���ʱ��оƬDS3231��ʱ�� *
	��Ϣ,���߽�����NM197��������ʱ����Ϣ,���ɱ���ʱ��,���15·ʱ������,�����ڶԷ��� *
	��Уʱ,��Դ�NM197Уʱ.															*
*																					*
*	���ߣ���ΰ��																	*
*																					*
*	������ڣ�2009-03-02															*
*	�޸����ڣ�2009-03-02															*
*************************************************************************************
*	�ļ����ƣ�NB197-U40-02.c        												*
*																					*
*	�ļ��汾��A02       															*
*																					*
** ժҪ(Module Description):														*
	������ʱ�����ù��ܣ����������GPSʱ,���ò��뿪�ص�4λѡ��,"1"��������Ϊ�������� *
	ʱ��,"0"��������Ϊ����ʱ��.                                                     *
*																					*
*	���ߣ�������																	*
*																					*
*	������ڣ�2012-07-17															*  
*	�޸����ڣ�2012-07-17															*																				 
*************************************************************************************
    ��ʱ��T2�ж�	�ж�����PC16552����оƬ�����룬�ж����ȼ�Ϊ4
	��ʱ��T1�ж�	10ms��ʱ�жϣ����ȼ�=0�����ֶ�����ʱ����Ϣʱ�������������˸��ʾ��
	��ʱ��T0		�����ⲿ������������ֵ��װ�ط�ʽ����ֵ255���൱��1���жϡ�
	�����ж�		���ڽ����ڲ�GPS�����ݣ��ж����ȼ�Ϊ4
	�ⲿ�ж�1		���ڴ�������Ϣ
	P0.0��P0.7 		8λ��������
	P1.0 			T2���ⲿ�������룬����PC16552�Ĵ���1�������ж�
	P1.1			T2�Ĵ������ƣ�����PC16552�Ĵ���2�����ж�
	P1.2 			ģ��x5045��SO��
	P1.6 			ģ��x5045��SCK��
	P1.7 			ģ��x5045��SI��	
	P3.5 			ģ��x5045��CS��
	P1.3			����״ָ̬ʾ�ƿ��ƿ��߸���ȡ��Ϊ"��+����ʱ��"��ʽ������Ϊ������ʽ
	P1.4			��DS3231��I2C�ӿڵ�SCL����
	P1.5			��DS3231��I2C�ӿڵ�SDA����
	P2.0��P2.7		��ַ���ߵĸ�8λ
	P3.2			������CPLD�ķ�����������Ϊ�ⲿ�ж�0
	P3.3			������CPLD�ļ��������ж�
	P3.4 			T0�ļ������룬������CPLD��������
	P3.6			д�ź���
	P3.7 			���ź���
	��ʾ(FB199-A)	0XEFFFH
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
* �������ܣ���1�ֽڶ����������2�ֽ�BCD��								*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
void bin_bcd() 
{
	g_time_buffer[2] = g_timecom_buffer[0] / 10;	 // ��
	g_time_buffer[3] = g_timecom_buffer[0] % 10;	
	
	g_time_buffer[4] = g_timecom_buffer[1] / 10;	 // ��
	g_time_buffer[5] = g_timecom_buffer[1] % 10;
	
	g_time_buffer[6] = g_timecom_buffer[2] / 10;	 // ��
	g_time_buffer[7] = g_timecom_buffer[2] % 10;
	
	g_time_buffer[8] = g_timecom_buffer[3] / 10;	 // ʱ
	g_time_buffer[9] = g_timecom_buffer[3] % 10;
	
	g_time_buffer[10] = g_timecom_buffer[4] / 10;	 // ��
	g_time_buffer[11] = g_timecom_buffer[4] % 10;
		
	g_time_buffer[12] = g_timecom_buffer[5] / 10;	 // ��
	g_time_buffer[13] = g_timecom_buffer[5] % 10;
	return;
}

/************************************************************************
* �������ܣ���2�ֽ�BCD��ϳ�1�ֽڶ�������								*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
 void bcd_bin()      
{
	g_timecom_buffer[0] = g_time_buffer[2] * 10 + g_time_buffer[3];	  	// ��
	g_timecom_buffer[1] = g_time_buffer[4] * 10 + g_time_buffer[5];	  	// ��
	g_timecom_buffer[2] = g_time_buffer[6] * 10 + g_time_buffer[7];	  	// ��
	g_timecom_buffer[3] = g_time_buffer[8] * 10 + g_time_buffer[9];	 	// ʱ
	g_timecom_buffer[4] = g_time_buffer[10] * 10 + g_time_buffer[11];	// ��
	g_timecom_buffer[5] = g_time_buffer[12] * 10 + g_time_buffer[13];	// ��
	return;
}

/************************************************************************
* �������ܣ�����Ƭ����ʱ�仺�����е����ݴ��͵���ʾ������				*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
void time_dis() 
{
	display_buffer[0]  = g_time_buffer[0]; 			// ��
	display_buffer[1]  = g_time_buffer[1];
	display_buffer[2]  = g_time_buffer[2];
	display_buffer[3]  = g_time_buffer[3];	   		
	display_buffer[4]  = g_time_buffer[8];			// ��
	display_buffer[5]  = g_time_buffer[9];			
	display_buffer[6]  = g_time_buffer[12];	   		// ��
	display_buffer[7]  = g_time_buffer[13];
	display_buffer[8]  = g_time_buffer[4];	   		// ʱ
	display_buffer[9]  = g_time_buffer[5];
	display_buffer[10] = g_time_buffer[6];	  		// ��
	display_buffer[11] = g_time_buffer[7];
	display_buffer[12] = g_time_buffer[10];	  		// ��
	display_buffer[13] = g_time_buffer[11]; 
	return;
}

/************************************************************************
* �������ܣ���ʱ														*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
void delay()      								// ��ʱ
{
	uchar data i;
	for (i=0;i<100;i++)
	{
	}	
	return;			
}

/************************************************************************
* �������ܣ���ʱ														*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
void delay_L()      							// �ϳ�ʱ�����ʱ���м�Ҫι�� 
{
	uchar data i,j,k;
	for (i=0;i<15;i++)	
		for(j=0;j<255;j++)
			for(k=0;k<255;k++)
			{
				   	SSEL = 0;
				   	_nop_();
				   	_nop_();
				   	SSEL = 1;					// ι���Ź�,��Ϊ����ѭ���ϳ���
			}
	return;			
}

/************************************************************************
* �������ܣ���ʾ���������ڽ�Ҫ��ʾ�����ݴ��͵�EPM570��					*
* 			��EPM570��ģ��8279��ʾоƬ									*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
void display_0(void) using 0   					// �����������е���ʾ����
{
    uchar data i,j;
	DAT_8279=0xAA  ;      						// ��EPM570�Ĵ���Э�飬0xAA��ʾ���ݴ��͵Ŀ�ʼ��
	for (i=0;i<14;i++)
	{
		j = display_buffer[i];     				// ��Ҫ��ʾ���ݵĶ�Ӧ����δ��͸�EPM570�� 
		DAT_8279 =  display_table[j];      
	}
	return;
}

void display_3(void) using 2   					// ����T1�ж��е���ʾ����
{												
    uchar data i,j;
	DAT_8279=0xAA  ;      						// ��EPM570�Ĵ���Э�飬0xAA��ʾ���ݴ��͵Ŀ�ʼ��
	for (i=0;i<14;i++)
	{
		j = display_buffer[i];     				// ��Ҫ��ʾ���ݵĶ�Ӧ����δ��͸�EPM570�� 
		DAT_8279 = display_table[j];      
	}
	return;
}

void display_2(void) using 2   					// �ⲿ�ж�1�����е���ʾ����
{
    uchar data i,j;
	DAT_8279=0xAA  ;      						// ��EPM570�Ĵ���Э�飬0xAA��ʾ���ݴ��͵Ŀ�ʼ��
	for (i=0;i<14;i++)
	{
		j = display_buffer[i];     				// ��Ҫ��ʾ���ݵĶ�Ӧ����δ��͸�EPM570�� 
		DAT_8279 = display_table[j];      
	}
	return;
}

/************************************************************************
* �������ܣ���ʼ��PC16552D:  ch1 (output) ;  ch2 (input)				*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
void Init_Pc16552D(void) 
{
	Uart1_Lcr = 0x80;   
	Uart1_Alf = 0;
	
	Uart2_Lcr = 0x80;   
	Uart2_Alf = 0;	
	
	Uart1_Lcr = 0x80;    							// Lcr.7=1 */
	Uart1_Divl = 0x0c;   							// ���岨���� 9600
	Uart1_Divm = 0x00;  
		
	Uart1_Fcr = 0x00;    							// ��ֹ����  
	Uart1_Lcr = 0x00;    							// Lcr.7=0
	Uart1_Ier = 0x02;    							// enable thr empty int       
	Uart1_Lcr = 0x03;    							// 8λ����λ��1λֹͣλ	
											
	Uart2_Lcr = 0x80;    							// make Lcr.7=1
	Uart2_Divl = 0x0c;   							// defining the boud rate
	Uart2_Divm = 0x00;   							//  boud rate=9600 
	                 
	Uart2_Fcr = 0x00;    							// disable fifo     
	Uart2_Lcr = 0x00;    							// make Lcr.7=0
	Uart2_Ier = 0x01;    							// enable rhr full int     
	Uart2_Lcr = 0x03;    							// 8λ����λ��1λֹͣλ  
	return;  
}

/************************************************************************
* �������ܣ�ģ��SPIд													*	
* ����˵������															*
*			X5045 оƬ�����											*
* ����ֵ��	��															*
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
* �������ܣ�ģ��SPI��													*	
* ����˵������															*
*			X5045 оƬ�����											*
* ����ֵ��	��															*
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
* �������ܣ�дX5045״̬�Ĵ���ʹ��										*	
* ����˵������															*
*			X5045 оƬ�����											*
* ����ֵ��	��															*
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
* �������ܣ���X5045״̬�Ĵ���											*	
* ����˵������															*
*			X5045 оƬ�����											*
* ����ֵ��	��															*
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
* �������ܣ�дX5045EEPROM�Ĵ���											*	
* ����˵������															*
*			X5045 оƬ�����											*
* ����ֵ��	��															*
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
* �������ܣ�дX5045״̬�Ĵ���											*	
* ����˵������															*
*			X5045 оƬ�����											*
* ����ֵ��	��															*
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
* �������ܣ���λ���Ź�													*	
* ����˵������															*
*			X5045 оƬ�����											*
* ����ֵ��	��															*
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
* �������ܣ���ʼ��X5045													*	
* ����˵������															*
*			X5045 оƬ�����											*
* ����ֵ��	��															*
************************************************************************/

void Init_X5045(void)   
{
	EN_Write();									// Set Write Enable Latch(Enable Write Operations)
    StatusRegWrite(0x0F);						// ���Ź����ʱ��1.4s���򿪿��Ź������鶼������������д������
	Reset_Watchdog();							// ι���Ź�
	return;
}

/************************************************************************
* �������ܣ�I2C ���Ϳ�ʼ												*	
* ����˵������															*
*			DS3231�����												*
* ����ֵ��	��															*
************************************************************************/
void start(void)            					// I2C ���Ϳ�ʼ
{
	sda = 1;  
	scl = 1;
	sda = 0; 
	return;
}

/************************************************************************
* �������ܣ�I2C ������ֹ												*	
* ����˵������															*
*			DS3231�����												*
* ����ֵ��	��															*
************************************************************************/
void stop(void)            						// I2C ������ֹ
{
    sda = 0;  
	sda = 0;
    scl = 1;  
	scl = 1;  
	sda = 1;
	return;
}

/************************************************************************
* �������ܣ�ͨ��I2C ������dд��DS3231									*	
* ����˵����dΪҪд��DS3231������										*
*			DS3231�����												*
* ����ֵ��	������״̬													*
************************************************************************/
uchar i2cwrite(uchar d) 						// ͨ��I2C ������dд��DS3231��
{
	uchar i;

    scl = 0;
    for (i=0;i<8; i++)
    {
        if (d&0x80)
            sda = 1;  							// �����λ��ʼ���ͣ�
        else
            sda = 0;
        scl = 0;
        scl = 1;
        d = d << 1;  							// ѭ����λ���ͣ�
        scl = 0;
    }
    sda = 1;        
    scl = 0;
    scl = 1;

    scl = 0;
    return(sda);
}

/************************************************************************
* �������ܣ�ͨ��I2C ��DS3231�����ݶ���									*	
* ����˵����d������Ÿմ�DS3231����������								*
*			DS3231�����												*
* ����ֵ��	������״̬													*
************************************************************************/
uchar i2cread(char b)							// ͨ��I2C ��DS3231�����ݶ����� b��
{					   	
	uchar i, d;

    d = 0;
    sda = 1;             
    scl = 0;
    for (i=0; i<8; i++)  						// �����λ����
    {
        scl = 1;
        d = d << 1;
        d = d | (unsigned char)sda;
        scl = 0;
    }
    sda = b;          							// ��λack����Ϊnack��
    scl = 1;
    scl = 0;

    sda = 1;          							// �ͷ�sda���ߣ�
    return d;
}

/************************************************************************
* �������ܣ���ʱ��оƬ�ж�ȡʱ��										*	
* ����˵������															*
*			DS3231�����												*
* ����ֵ��	��															*
************************************************************************/
void  Read_Time(void)							// ��ʱ��оƬ�ж�ȡʱ�� 
{   
    start();									// I2C ���Ϳ�ʼ��
    i2cwrite(ADDRTC);							// д��ӵ�ַ��
    i2cwrite(0);								// �ӵ�ַ0��ʼ������DS3231�Ĵ�����
    start();									// ��ʼ�ͽ�������ʾ�ϴ�I2C���͵Ľ�����
    i2cwrite(ADDRTC|1);
    sec = i2cread(ACK);							// �룻
    min = i2cread(ACK);							// �֣�
    hr = i2cread(ACK);							// ʱ��
    dy = i2cread(ACK);							// ���ڣ�
    dt = i2cread(ACK);							// �գ�
    mn = i2cread(ACK);							// �£�
    yr = i2cread(NACK);							// �ꣻ
    stop();										// I2C ���ͽ�����
								  
	g_time_buffer[13] = sec&0x0f; 				// ��Ϊ��DS3231��������������BCD�룬���ߵ�λ�ֱ���벻ͬ�ļĴ������Է�����ʾ��
	g_time_buffer[12] = (sec>>4)&0x0f; 			// �룻
	g_time_buffer[11] = min&0x0f;					// �֣�
	g_time_buffer[10] = (min>>4)&0x0f;	
	g_time_buffer[9] = hr&0x0f;					// ʱ��
	g_time_buffer[8] = (hr>>4)&0x0f;
	g_time_buffer[7] = dt&0x0f;					// �գ�
	g_time_buffer[6] = (dt>>4)&0x0f;
	g_time_buffer[5] = mn&0x0f;					// �£�
	g_time_buffer[4] = (mn>>4)&0x0f;
	g_time_buffer[3] = yr&0x0f;					// �ꣻ
	g_time_buffer[2] = (yr>>4)&0x0f;

	bcd_bin();									// bcd��ʱ��ת���ɶ�����ʱ�䣬�Է�������ã�
	
	return;      						
}

/************************************************************************
* �������ܣ���ʱ��оƬ��д��ʱ��										*	
* ����˵������															*
*			DS3231�����												*
* ����ֵ��	��															*
************************************************************************/
void  Write_Time(void)							// ��ʱ��оƬ��д��ʱ��    
{    
    bin_bcd();  								// ˢ��BCD���ʽ��ʱ��
  
	sec = ((g_time_buffer[12]<<4)&0xf0)|(g_time_buffer[13]&0x0f); 	// �룻
	min = ((g_time_buffer[10]<<4)&0xf0)|(g_time_buffer[11]&0x0f); 	// �֣�
	hr =((g_time_buffer[8]<<4)&0xf0)|(g_time_buffer[9]&0x0f); 		// ʱ��
	dt = ((g_time_buffer[6]<<4)&0xf0)|(g_time_buffer[7]&0x0f); 		// �գ�
	mn = ((g_time_buffer[4]<<4)&0xf0)|(g_time_buffer[5]&0x0f); 		// �£�
	yr =((g_time_buffer[2]<<4)&0xf0)|(g_time_buffer[3]&0x0f); 		// �ꣻ
	dy = 1;										// ����������û�ã����ó�1��

    start();									// I2C ���Ϳ�ʼ��
    i2cwrite(ADDRTC);  							// д��ӵ�ַ��
    i2cwrite(0x00);     						// �ӵ�ַ0��ʼ����д��DS3231�Ĵ�����
    i2cwrite(sec);								// �룻
    i2cwrite(min);								// �֣�
    i2cwrite(hr);								// ʱ��
    i2cwrite(dy);								// �գ�
    i2cwrite(dt);								// ���ڣ�
    i2cwrite(mn);								// �£�
    i2cwrite(yr);								// �ꣻ
    stop();	
	return;
}

/************************************************************************
* �������ܣ�ʱ��оƬ��ʼ��												*	
* ����˵������															*
*			DS3231�����												*
* ����ֵ��	��															*
************************************************************************/
void  Init_3231(void)     						// ʱ��оƬ��ʼ��
{

    do      									// ��Ϊ�ϵ������ʱ,DS3231��I2C�ӿڴ�����Ч״̬,���Ǳ����������ͷ�I2C����.  
    {       		
        sda = 1;    
        scl = 1;
        if(sda) 								// ���sda��Ϊ��,����I2C����  
        {
                sda = 0;						// DS3231 �ܹ�ʶ��I2C����Ŀ�ʼ  
                sda = 1;    
        }
        scl = 0;
    } while(sda == 0);      					// ���DS3231Ϊ��,�Ӷ����ִ���״̬,���¿�ʼ.
    start();
    i2cwrite(ADDRTC);							// DS3231��Ĭ��co0�ӻ���ַ
    i2cwrite(0x0e); 							// дDS3231�Ŀ��ƼĴ�����ַ:0E
    i2cwrite(0x00); 							// INT/SQW ���1HZ��������Ϊ��Ƭ����EPM570����������ã� 
	return;
}

/************************************************************************
* �������ܣ��ж��Ƿ�������												*	
* ����˵������															*
*																		*
* ����ֵ��	��������귵��1�����򷵻�0									*
************************************************************************/
uchar leap_year_using1_t(void)   	using 1		// T2�жϺ������õ��������жϺ���
{
 	uchar	data  	year;
	year=g_timecom_buffer[0];
	if((year%4)==0)   
	{
		return(1);
	}     
	return(0);
}
uchar leap_year_using1_s(void)   	using 1		// �����ж����õ��������жϺ���
{
 	uchar	data  	year;
	year=g_timecom_buffer[0];
	if((year%4)==0)   
	{
		return(1);
	}     
	return(0);
}
uchar leap_year_using2_s(void)  	using 2		// time_adj()�������õ��������жϺ���
{
 	uchar	data  	year;
	year=g_timecom_buffer[0];
	if((year%4)==0)   
	{
		return(1);
	}     
	return(0);
}

uchar leap_year_using2_i(void)  	using 2		// �ⲿ�ж����õ��������жϺ���
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
* �������ܣ�ʱ�����													*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
void time_adj(void)	using 2
{ // ����ʱ�俪ʼ  
	g_timecom_buffer[5]++; // ������  
	if  (g_timecom_buffer[5]==60)
	{ // ������60��������� 
		g_timecom_buffer[5] = 0; // ������  
		g_timecom_buffer[4]++; // ������  
		if (g_timecom_buffer[4]==60)
		{ // ������60�������Сʱ 
			g_timecom_buffer[4] = 0; // �����  
			g_timecom_buffer[3]++; // ����Сʱ  
			if (g_timecom_buffer[3]==24)
			{// Сʱ����24��������� 
				g_timecom_buffer[3] = 0;  // ����Сʱ                    
				g_timecom_buffer[2]++;  // ������                                             
				if (g_timecom_buffer[2]>month_table[(g_timecom_buffer[1]-1)])
				{ // ���Ѵ��ڱ���������� 
					if ((g_timecom_buffer[1]!=2)||!(leap_year_using2_s())||(g_timecom_buffer[2]!=29))                               
					{ // ����2�»���������²���29�գ���������¸���1�գ���ʼ  
						g_timecom_buffer[2] = 1;
						g_timecom_buffer[1]++;
						if (g_timecom_buffer[1]==13)
						{ // ������13�������꣬��ʼ 
							g_timecom_buffer[1] = 1;
							g_timecom_buffer[0]++;
						} // ������13�������꣬����
					}   // ����2�»���������²���29�գ���������¸���1�գ���ʼ  
				} // ���Ѵ��ڱ���������� 
			} // Сʱ����24���������      
		} // ������60�������Сʱ          
	} // ������60���������

	return;  
	                                                                          
} // ����ʱ����� 

/****************************************************************
*�������ܣ�	 T2�ⲿ�����жϣ����ڽ����ⲿʱ��Դ��Ϣ				*
*�ж����ȼ���highest��7											*
*˵����		���жϳ�������ⲿʱ��Դʱ�����ݣ����������������	*
*      		���ж�������UARTоƬ���������ж�					*
****************************************************************/
void t2_recive() interrupt 5 using 1  				// �жϳ������ 
{
	uchar data 		i;
	uchar data 		gps_sec;  						// UART���յ���GPS��ʱ"������"����;                                
	uchar data 		check_temp;						// ���У����
 	uchar data		data_t2;						// ��Ŵ�Uart���ܵ�������

  	EA = 0;
  	EXF2 = 0;      									// EXF2 �� TF2 ����������㣻
  	TF2 = 0;
	data_t2 = Uart2_Rhr;
  	if((HUB_STATE&0x01)==0x00)    					// ��ʱ��	    
  	{
		if(data_t2==0xaa)  							// �������ݴ��͵ĸ�ʽΪ��AA ���� BB��
		{      	      	  
			g_clr_count = 0;
			if(g_errFlag==0x8A&&g_gps_True==0x02)						// ˵�������Ѿ���������,���ϻָ�
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
					g_errFlag = 0x00;			   // ˵�����δ����޹���,�����ǹ��ϻָ�
				}			   		  
				gps_sec = g_adj[5];
				if (((gps_sec==0x00)&&(g_gps_sec_pre_s==59))||(gps_sec==g_gps_sec_pre_s+1))
				{
					g_gps_True++;					// �жϣ����ʱ������Ϣ�������ģ���ʹ������Ϣ��������
				}
				else
				{
					g_gps_True--;
					if(g_gps_True<0x02)
					{
						g_gps_True = 0x02;
						g_errFlag = 0x9A;	 		// ����ʱ���豸����
					}
				}
				g_gps_sec_pre_s = gps_sec;	
				if (g_gps_True>0x02)
				{
					g_gps_True = 0x02;
					g_errFlag = 0x00;				// �ⲿGPS�豸�޹���
					for(i=0;i<6;i++)
					{
						g_timecom_buffer[i] = g_adj[i];
					}
				}
			}
			else
			{
				g_errFlag = 0x7A;
			    g_gps_True = 0x03;	       			// ��ֹ���ִ�����������ű��豸����
//				g_err_A = 0x01;	
			}
			g_trans_flag_t = 1; 					// ���ݴ��ͱ�ʶ��1��
		}									   	
	  	else
		{
			g_couter = 0;  
		}
	}
  	
  	else  if((HUB_STATE&0x07)==0x07)      			// ������Ϊ����������GPS��Ϣ��,����������GPSˢ����ʾ   	
   	{     								  			// GPS���ݰ��ĸ�ʽ�ǣ� GPZDA+ʱ�����ݣ� �������жϳ�GPZADA��Ȼ�󽫺�ߵ�ʱ��������ȡ������		      		
 		if(data_t2==36)					  			// head=$  $=36  
  	  	{      	      	  
 			g_clr_count = 0;
			if(g_errFlag==0x2A&&g_gps_True==0x02)	// ˵�������Ѿ���������,���ϻָ�
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
															//�˴�Ϊ����.Ŀ���Ǽ��ݲ�ͬ���ҵ�NMEA��ʽ����
		}
		else if((g_couter==29)&&(data_t2==42))				//*=42 0x2A 
		{
			g_couter++;
		} 
		else if(g_couter==30)								// У����	  
		{
			if ((data_t2)<58&&(data_t2>47))					// У����Ϊ����
			{
				check_temp = (data_t2-0x30)<<4;
			}
			else if ((data_t2)<71&&((data_t2)>64)) 			// У����Ϊ��д��ĸ:A B C D E F
			{
				check_temp = (data_t2-0x37)<<4;
			}
			else if ((data_t2)<103&&((data_t2)>96))			// У����ΪСд��ĸ:a b c d e f
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
			else if ((data_t2)<103&&((data_t2)>96))			// У����ΪСд��ĸ:a b c d e f
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
					g_errFlag = 0x00;			   			// ˵�����δ����޹���,�����ǹ��ϻָ�
				}			   		  
											
				gps_sec = g_adj[4]*10+g_adj[5];
				if (((gps_sec==0x00)&&(g_gps_sec_pre==59))||(gps_sec==g_gps_sec_pre+1))
				{
					g_gps_True++;							// �жϣ����ʱ������Ϣ�������ģ���ʹ������Ϣ��������
				}
				else
				{
					g_gps_True--;
					if(g_gps_True<0x02)
					{
						g_gps_True = 0x02;
						g_errFlag = 0x3A;	 				// ����ʱ���豸����
					}
				}
				g_gps_sec_pre = gps_sec;	
				if (g_gps_True>0x02)
				{
					g_gps_True = 0x02;
					g_errFlag = 0x00;						// �ⲿGPS�豸�޹���

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
		          
		           	bcd_bin();        						// bcd��ʱ��ת���ɶ�����ʱ�� 
		           	
		           	/* 2012-5-3,�������޸�,����ʱ������	*/
					if( (HUB_STATE&0x0F)==0x0F )			// �������Ϊ��������ʱ��
					{
			           	if(g_timecom_buffer[3]>15)
						{
							g_timecom_buffer[3]=(g_timecom_buffer[3]-16);
							/**********zuoxin modify 2003.01.29***************/
							g_timecom_buffer[2]++; //adjust day                                            
							if (g_timecom_buffer[2]>month_table[(g_timecom_buffer[1]-1)])
							{   // ���Ѵ��ڱ���������� 
								if ((g_timecom_buffer[1]!=2)||!(leap_year_using1_t())||(g_timecom_buffer[2]!=29))                               
								{ // ����2�»���������²���29�գ���������¸���1�գ���ʼ  
									g_timecom_buffer[2] = 1;
									g_timecom_buffer[1]++;
									if (g_timecom_buffer[1]==13)
									{ // ������13�������꣬��ʼ  
										g_timecom_buffer[1] = 1;
										g_timecom_buffer[0]++;
									} // ������13�������꣬����  
								} // ����2�»���������²���29�գ���������¸���1�գ���ʼ  
							} // ���Ѵ��ڱ����������  
			               /**********zuoxin modify 2003.01.29***************/
			
			          	}
			       		else
			       	   	{
			       	     	g_timecom_buffer[3] = g_timecom_buffer[3]+8;
			           	}
					}
					else if ( (HUB_STATE&0x0F)==0x07 )		// �������Ϊ����ʱ��
					{
						;	// ����
					}
		         
				 /* if(g_gps_flag==0)		// wuchongxing   2005.07.06  
					{
						Write_Time();
						g_gps_flag=1;
						ADJUST_SEC=(g_timecom_buffer[5]+4);   	// ����ʱ����ʱ��Ҫ��4 
					}  */

		   		}
			}
			else
			{
				g_errFlag = 0x1A;								// �ⲿGPS�������
				g_gps_True = 0x03;
			}
		    g_trans_flag_t = 1; 								// ���ݴ��ͱ�ʶ��1��							
  		 	g_check_xor = 0x66; 								// У��λ�ָ�ԭֵ
		}
	   	else
	   	{   
	   		g_couter = 0; 
			g_check_xor = 0x66;
	   	}
	   
	}
	/*  2012-7-16,�������޸�,������Ҫ�󱾵�ģʽ������GPSУʱ��Ϣ,���ȥ���˹��ܡ�*/
//  	else  if(((HUB_STATE&0x03)==0x01)&&(g_gps_flag==0))  		// ������Ϊ����������ʱ�ӣ������ֽ���GPS������µĴ���ʽ
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
//																	//�˴�Ϊ����.Ŀ���Ǽ��ݲ�ͬ���ҵ�NMEA��ʽ����
//		}
//		else if((g_couter==29)&&(data_t2==42))		//*=42 0x2A 
//		{
//			g_couter++;
//		} 
//		else if(g_couter==30)						// У����
//		{
//			if ((data_t2)<58&&(data_t2>47))
//			{
//				check_temp = (data_t2-0x30)<<4;
//			}
//			else if ((data_t2)<71&&((data_t2)>64))
//			{
//				check_temp = (data_t2-0x37)<<4;
//			}
//			else if ((data_t2)<103&&((data_t2)>96))			// У����ΪСд��ĸ:a b c d e f
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
//			if ((data_t2)<58&&(data_t2>47))					// У����Ϊ����
//			{
//				check_temp += (data_t2-0x30);
//			}
//			else if ((data_t2)<71&&((data_t2)>64))			// У����Ϊ��д��ĸ:A B C D E F
//			{
//				check_temp += (data_t2-0x37);
//			}
//			else if ((data_t2)<103&&((data_t2)>96))			// У����ΪСд��ĸ:a b c d e f
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
//					g_gps_True++;							// �жϣ����GPSʱ������Ϣ�������ģ���ʹ������Ϣ��������
////					g_errFlag = 0x00;						// �˴����������
//				}
//				else
//				{
//					g_gps_True = 0x02;
////					g_errFlag = 0x3A;						// �˴����������
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
//	           		bcd_bin();      // bcd��ʱ��ת���ɶ�����ʱ�� 
//	           	
//	           		if(g_timecom_buffer[3]>15)
//					{
//						g_timecom_buffer[3]=(g_timecom_buffer[3]-16);
//						/**********zuoxin modify 2003.01.29***************/
//						g_timecom_buffer[2]++; // adjust day                                           
//						if (g_timecom_buffer[2]>month_table[(g_timecom_buffer[1]-1)])
//						{ // ���Ѵ��ڱ���������� 
//							if ((g_timecom_buffer[1]!=2)||!(leap_year_using1_t())||(g_timecom_buffer[2]!=29))                               
//							{ // ����2�»���������²���29�գ���������¸���1�գ���ʼ  
//								g_timecom_buffer[2] = 1;
//								g_timecom_buffer[1]++;
//								if (g_timecom_buffer[1]==13)
//								{ // ������13�������꣬��ʼ 
//									g_timecom_buffer[1] = 1;
//									g_timecom_buffer[0]++;
//								} // ������13�������꣬����  
//							} // ����2�»���������²���29�գ���������¸���1�գ���ʼ    
//						} // ���Ѵ��ڱ���������� 
//					/**********zuoxin modify end 2003.01.29***************/
//					}
//	           		else
//	           	   	{
//	           	     	g_timecom_buffer[3] = g_timecom_buffer[3]+8;
//	               	}
//		   			g_trans_flag_t = 1; 						// ���ݴ��ͱ�ʶ��1��							
//	           	/*	wuchongxing   2005.07.06  */
//	            	Write_Time();
//	            	g_gps_flag = 1;
//				}
//  	    	}
//		    g_trans_flag_t = 1; 								// ���ݴ��ͱ�ʶ��1��							
//			g_check_xor = 0x66; 							 	// У��λ�ָ�ԭֵ
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
*�������ܣ�	 �����жϣ����ڽ����ⲿʱ��Դ��Ϣ					*
*�ж����ȼ���highest��6											*
*˵�����ô����жϳ������GPSģ��ʱ�����ݣ����������������		*
****************************************************************/
void int4_uart() interrupt 4 using 1 			// �����жϳ������    
{	
    uchar   data 		gps_sec_s;  			// ���ڽ��յ���GPS��ʱ"������"����;                                
	uchar   data 		check_temp_s;			// ���У����
	EA = 0;
	if(((HUB_STATE&0x07)==0x03)&&(RI==1))      	// ������Ϊ����������GPS��Ϣ��,����������GPSˢ����ʾ   	
   	{     								  		// GPS���ݰ��ĸ�ʽ�ǣ� GPZDA+ʱ�����ݣ� �������жϳ�GPZADA��Ȼ�󽫺�ߵ�ʱ��������ȡ������		      		
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
														//�˴�Ϊ����.Ŀ���Ǽ��ݲ�ͬ���ҵ�NMEA��ʽ����
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
		else if(g_couter_s==30)							// У����
		{
			if ((SBUF)<58&&(SBUF>47))					// У����Ϊ����
			{											
				check_temp_s = (SBUF-0x30)<<4;
			}
			else if ((SBUF)<71&&((SBUF)>64))			// У����Ϊ��д��ĸ:A B C D E F
			{
				check_temp_s = (SBUF-0x37)<<4;			
			}
			else if ((SBUF)<103&&((SBUF)>96))			// У����ΪСд��ĸ:a b c d e f
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
			     	                  
 			if ((SBUF)<58&&(SBUF>47))					// У����Ϊ����
			{
				check_temp_s += (SBUF-0x30);
			}
			else if ((SBUF)<71&&((SBUF)>64))			// У����Ϊ��д��ĸ:A B C D E F
			{
				check_temp_s += (SBUF-0x37);
			}
			else if ((SBUF)<103&&((SBUF)>96))			// У����ΪСд��ĸ:a b c d e f
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
					g_errFlag = 0x00;			   		// ˵�����δ����޹���,�����ǹ��ϻָ�
				}			   		  

				gps_sec_s = g_adj_s[4]*10 + g_adj_s[5];
				if (((gps_sec_s == 0x00)&&(g_gps_sec_pre_s == 0x3B))||(gps_sec_s == g_gps_sec_pre_s + 1))
				{
					g_gps_True_s++;						// �жϣ����GPSʱ������Ϣ�������ģ���ʹ������Ϣ��������
				}
				else
				{
					g_gps_True_s--;
					if(g_gps_True_s<0x02)
					{
						g_gps_True_s = 0x02;
						g_errFlag = 0x6A;	 			// �ڲ�GPS�豸����
					}
				}
				g_gps_sec_pre_s = gps_sec_s;
					
				if (g_gps_True_s>0x02)
				{
					g_gps_True_s = 0x02;
					g_errFlag = 0x00;	 				// �ڲ�GPS�豸�޹���

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
		          
		           	bcd_bin();        					// bcd��ʱ��ת���ɶ�����ʱ�� 
		           	
		           	if(g_timecom_buffer[3]>15)
					{
						g_timecom_buffer[3] = g_timecom_buffer[3]-16;
						/**********zuoxin modify 2003.01.29***************/
						g_timecom_buffer[2]++; //adjust day                                            
						if (g_timecom_buffer[2]>month_table[(g_timecom_buffer[1]-1)])
						{   // ���Ѵ��ڱ���������� 
							if ((g_timecom_buffer[1]!=2)||!(leap_year_using1_s())||(g_timecom_buffer[2]!=29))                               
							{ // ����2�»���������²���29�գ���������¸���1�գ���ʼ  
								g_timecom_buffer[2] = 1;
								g_timecom_buffer[1]++;
								if (g_timecom_buffer[1]==13)
								{ // ������13�������꣬��ʼ  
									g_timecom_buffer[1] = 1;
									g_timecom_buffer[0]++;
								} // ������13�������꣬����  
							} // ����2�»���������²���29�գ���������¸���1�գ���ʼ  
						} // ���Ѵ��ڱ����������  
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
						ADJUST_SEC=(g_timecom_buffer[5]+4);   // ����ʱ����ʱ��Ҫ��4   
					} */
				}
			}
			else
			{
				g_errFlag = 0x4A;						// �ڲ�GPS�������
				g_gps_True_s = 0x03;
			}
		    g_trans_flag_t = 1; 									// ���ݴ��ͱ�ʶ��1��							
			g_check_xor_s = 0x66; 						// У��λ�ָ�ԭֵ 0x66
  	   }
	   else
	   {   
	   		g_couter_s = 0; 
			g_check_xor_s = 0x66; 						// У��λ�ָ�ԭֵ 0x66
	   }
	   
	}

	RI = 0; 
	EA = 1; 
}

/****************************************************************
*�������ܣ�	 �ⲿ�ж�0�����ڴ����������Ϣ						*
*�ж����ȼ���lowest��1											*
*˵����		ÿ���Ӷ���CPLD�ڲ��ķ��Ӽ���������					*
****************************************************************/
void int0_mp() interrupt 0 using 2  					//�жϳ������ 
{

}

/****************************************************************
*�������ܣ�	 T0���ⲿ��������жϣ����ڴ�����������Ϣ			*
*�ж����ȼ���lowest��2											*
*˵����		ÿһ�뵥Ƭ������������ʱ����Ϣ����Ϊ����ʱ��		*
****************************************************************/
void timer0_sp() interrupt 1 using 2  					//T0�жϳ������ 
{   
	if(g_adjust_flag==0&&((HUB_STATE&0x03)==0x01||(((HUB_STATE&0x03)==0x03||((HUB_STATE&0x01)==0x00))&&g_errFlag!=0x00)))    							// ��ʱ��
	//  ���ֶ�����״̬  &&(  1 ����+����ʱ�ӣ�		||	2��	  �� ��+GPS/�ڲ����ⲿ	||  ��״̬��&& �д���	  )
	{   
		time_adj();
		g_trans_flag=1; 								// ���ݴ��ͱ�ʶ��1��         
	}      
}

/****************************************************************
*�������ܣ�	 T1 �жϣ�����10ms�ļ�ʱ�ж� 						*
*�ж����ȼ���lowest��4											*
*˵����		���а�������ʱ��������ܵ���˸Ч��					*
****************************************************************/
void time_10ms() interrupt 3 using 2  					// T1�жϳ������ 
{
	uchar data temp1,temp2;

	if ((HUB_STATE&0x01)==0) 							// ��ֹ�ڴ�״̬�½����״̬
	{
		TR1 = 0;
		g_adjust_flag = 0;                   
		g_couter_adj = 0;   							// ָ�뷵��ָ��ͷ���꣩            
		g_led_standby = 1;
		
	}

	EA = 0;
	g_number_dog = 0;
	TH1 = 0x15;         								//	TH1 = -(60000/256); �ӳ�Լ10ms
	TL1 = 0xA0;											//	TL1 = -(60000%256);
	if (g_couter_10ms==12)
	{
		g_couter_10ms = 0; 
		if(g_spark_flag==0)
		{
			g_spark_flag = ~g_spark_flag ;
			if(g_couter_adj==0)
			{
				g_time_buffer[0] = 0x0a;  				// Ϩ����Ӧ��ʱ�䵥Ԫ
				g_time_buffer[1] = 0x0a;
			} 
			temp1 = g_time_buffer[(2*g_couter_adj+2)];
			temp2 = g_time_buffer[(2*g_couter_adj+3)];
			g_time_buffer[(2*g_couter_adj+2)] = 0x0a;  	// Ϩ����Ӧ��ʱ�䵥Ԫ 
			g_time_buffer[(2*g_couter_adj+3)] = 0x0a;
			time_dis(); 								// ��ʾ 
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
* �������ܣ���������Ϣ													*	
* ����˵������															*
*																		*
* ����ֵ��	�����ļ�ֵ													*
************************************************************************/
uchar dekey()  using 2
{
	uchar data k;
	k = DAT_8279;     									// ��CPLD�ж�ȡ������Ϣ��ģ��8279�Ĺ���
	return(k&0x0f); 
}
/************************************************************************
* �������ܣ�����SET��ʱ�Ĵ���											*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
void  key_set() using 2 
{
	if (g_adjust_flag==0)
	{
		g_adjust_flag = 1;
		g_couter_adj = 0;    							// ָ��ָ��ͷ���꣩         
		TR1=1 ;
	}
	else  												//(g_adjust_flag=1)
	{            
		bin_bcd(); 	//bcd_bin ( );    					// bcd��ʱ��ת���ɶ�����ʱ��
		ADJUST_SEC = g_timecom_buffer[5]+4;	            // ����ʱ����ʱ��Ҫ��4
		Write_Time(); 
		g_adjust_flag = 0;                   
		g_couter_adj = 0;   							// ָ�뷵��ָ��ͷ���꣩            
		TR1 = 0;
	} 
	return;           
}

/************************************************************************
* �������ܣ�����SEL��ʱ�Ĵ���											*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
void  key_sel() using 2 
{
	if (g_adjust_flag==1)
	{          
		g_couter_adj++;  
		if(g_couter_adj==6) 							// ��ָ��ĩβ���룩             
		{
			g_couter_adj = 0; 							// ָ�뷵��ָ��ͷ���꣩
		}                     
	}
	return;         
}

/************************************************************************
* �������ܣ�����UP��ʱ�Ĵ���											*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
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
			{ 											//���Ѵ��ڱ����������//
				if ((g_timecom_buffer[1]!=2)||!(leap_year_using2_i())||(g_timecom_buffer[2]!=29))                               
				{ 										//����2�»����������29�գ��������1�� // 
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
* �������ܣ�����DOWN��ʱ�Ĵ���											*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
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
				{ 										//��2�¶��������꣬�������29��// 
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
*�������ܣ�	 �ⲿ�ж�1����Ӧ�а�����Ϣʱ�����ж�				*
*�ж����ȼ���lowest��3											*
*˵����		����������CPLD��ʵ�֣����ж��ж�ȡ������Ϣ			*
****************************************************************/
void int1_key() interrupt 2 using 2  					// int1�жϳ������ 
{
	uchar data i;
	EA=0;
	if (dekey()!=0x0f)       							// ����ֵ�����Ϊ�㣬��ζ���а�������
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
		bin_bcd();  									// ˢ��BCD���ʽ��ʱ�� 
		time_dis();  									// ��ʾ
		display_2();
	}    
	EA = 1;
}

/************************************************************************
* �������ܣ����ڷ���һ���ֽڵ�����										*	
* ����˵������															*
*																		*
* ����ֵ��																*
************************************************************************/
void Uart_SendChar(uchar data dat)
{
	SBUF = dat;											// ����һ���ֽ�										
	while (TI == 0);
	TI = 0;	
	return; 
}

/************************************************************************
* �������ܣ�GPS-M12��ʼ������M12���ó�NMEA��ʽ���,����ֻ���GPZDA��	*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
void Init_GPS_M12()
{
	Uart_SendChar(0x40);	// 40 40 43 69 01 2B 0D 0A 
	Uart_SendChar(0x40);	// @@Ci1C<CR><LF>
	Uart_SendChar(0x43);	// ��M12���ó�NMEA��ʽ���,����ֻ���GPZDA��
	Uart_SendChar(0x69);
	Uart_SendChar(0x01);
	Uart_SendChar(0x2B);
	Uart_SendChar(0x0D);
	Uart_SendChar(0x0A);

	Uart_SendChar(0x24);	// 24 50 4D 4F 54 47 2C 47 47 41 2C 30 0D 0A 
	Uart_SendChar(0x50);	// $PMOTG,GGA,0 ��ֹ���GPGGA��ʽ����
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
	Uart_SendChar(0x50);	// $PMOTG,GLL,0 ��ֹ���GPGLL��ʽ����
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
	Uart_SendChar(0x50);	// $PMOTG,GSA,0 ��ֹ���GPGSA��ʽ����
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
	Uart_SendChar(0x50);	// $PMOTG,GSA,0 ��ֹ���GPGSV��ʽ����
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
	Uart_SendChar(0x50);	// $PMOTG,RMC,0 ��ֹ���GPRMC��ʽ����
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
	Uart_SendChar(0x50);	// $PMOTG,VTG,0 ��ֹ���GPVTG��ʽ����
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
	Uart_SendChar(0x50);	// $PMOTG,ZDA,1 �������GPZDA��ʽ����
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
* �������ܣ���1000-U(Ublox)���ó�NMEA��ʽ���,����ֻ���GPZDA��			*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
void Init_GPS_Ublox()
{
	Uart_SendChar(0x24);	// 24 50 55 42 58 2C 34 30 2C 5A 44 41 2C 30 2C 31 2C 30 2C 30 2A 34 35	0D 0A
	Uart_SendChar(0x50);	// $PUBX,40,ZDA,0,1,0,0*45
	Uart_SendChar(0x55);	// ��1000-U���ó�NMEA��ʽ���,���GPZDA��
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
	Uart_SendChar(0x62);	// ����������������������е�����
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
* �������ܣ���ʼ��CPU													*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
void Init_CPU()
{
	CKCON0 = 0x01;							// X2 ģʽ
	IT0 = 1;           						// �ⲿ�ж�INT0��INT1��Ϊ���ش���
	IT1 = 1;  
	TMOD = 0x16 ;      						// T1��ʱ��ʽ1��T0�ⲿ������ʽ2       

	TH0 = 0xFF; 
	TL0 = 0xFF;        						// ������ֵ��װ�� 
	TR0 = 1 ;         						// ����T0���� ����ֹT1���� 

	TH1 = 0x15;         					//	TH1 = -(60000/256); �ӳ�Լ10ms
	TL1 = 0xA0;								//	TL1 = -(60000%256);
								   	  	
	T2CON = 0x09;     						// �ⲿ����ʽ�������𶯼���
	
	SCON = 0xD0; 							// ���ڹ����ڷ�ʽ1��8λ����λ��1λֹͣλ����У��λ
	
 	if ((HUB_STATE&0x08)==0)  				// =0 �ò�����4800����ΪM12��NMEA��ʽ�õĲ�����Ϊ4800      
	{
		BRL = 0x64;							// 0X64 = 100 ������4800,���Ϊ0.16%;
		BDRCON = 0x1E; 						// 0001,1110���ڽ����뷢�͵Ĳ����ʾ��ɲ����ʷ���������,SPD=1,SMOD1=0
	 	Init_GPS_M12();						// M12��NMEA��ʽ�õ���4800������
	}
	else									// =1 �ò�����9600
	{
		BRL = 0xB2; 						// 0XB2 = 178 ������9600,���Ϊ0.16%;
		BDRCON = 0x1E; 						// 0001,1110���ڽ����뷢�͵Ĳ����ʾ��ɲ����ʷ���������,SPD=1,SMOD1=0
		Init_GPS_Ublox();				  	// Ublox nmea��ʽ�õ���9600������ 
	}  
	          
	IPH0 = 0x30;       						// �����ж�T2 TSΪ�߼��ж�*/
	IPL0 = 0x30;
	IEN0 = 0x3e;     						// CPU���жϣ������ж�  EA EC ET2 ES , ET1 EX1 ET0 EX0*/ 
	
	return;
}

/************************************************************************
* �������ܣ�������														*	
* ����˵������															*
*																		*
* ����ֵ��	��															*
************************************************************************/
void main() using 0
{
	uchar data 		i;									// ѭ���ñ���
	uchar data 		check_temp;							// ��ʱ���У����
	uchar data		hub_state_pre = HUB_STATE&0x0F;			// ��¼���뿪�ص�״̬

	Init_CPU();
	Init_Pc16552D();
	Init_3231();
	Init_X5045();

	if ((HUB_STATE&0x03)==1)  							// =0������      
	{
		g_led_standby=0;
	}   
	else 
	{
		g_led_standby=1;   								// =1������    
	}

	display_0();     									// �ϵ�ʱ�����е������ȫ��ʾ 8���������������Ƿ��ܹ�������ʾ
	delay_L(); 

	g_time_buffer[0]=2; 								// �̶���ʾ20
	g_time_buffer[1]=0; 
	Read_Time();     									// �ϵ��ʱ��

	bcd_bin();      									// bcd��ʱ��ת���ɶ�����ʱ��
	ADJUST_SEC=(g_timecom_buffer[5]+4);					// ����ʱ����ʱ��Ҫ��4 

	time_dis();
	display_0(); 

	EA=1; 

	while(1)
	{    
		if(g_trans_flag==1||g_trans_flag_t==1) 
		{         
			do 											// �ж�UART16552оƬ�Ƿ���Է������ݣ�
			{
				g_adjust = Uart1_Lsr&0x60;
				g_adjust2 = Uart2_Lsr&0x60;
			}
			while((g_adjust!=0x60)||(g_adjust2!=0x60));  
			Uart1_Thr = 0xaa;				   			// AA ��Ϊ�������ݵı�ͷ��
			Uart2_Thr = 0xaa;
			delay();        
			
			for(i=0;i<6;i++)							// ѭ������������ʱ������Ϣ��
			{
				do 										// �ж�UART16552оƬ�Ƿ���Է������ݣ�
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
				check_temp = check_temp^g_timecom_buffer[i];	// ���У�飻
			}        
			do 													// �ж�UART16552оƬ�Ƿ���Է������ݣ�
			{
				g_adjust = Uart1_Lsr&0x60;
				g_adjust2 = Uart2_Lsr&0x60;
			}
			while((g_adjust!=0x60)||(g_adjust2!=0x60));

			Uart1_Thr = check_temp;								// ����У��λ��
			Uart2_Thr = check_temp; 
			delay(); 
			do 													// �ж�UART16552оƬ�Ƿ���Է������ݣ�
			{
				g_adjust = Uart1_Lsr&0x60;
				g_adjust2 = Uart2_Lsr&0x60;
			}
			while((g_adjust!=0x60)||(g_adjust2!=0x60));
			Uart1_Thr = 0xbb;									// BB��Ϊ�������ݵĽ�����
			Uart2_Thr = 0xbb;
			delay();
		// ����ʱ����Ϣ        
			if (g_timecom_buffer[5]!=0)							// ��ʱ����д��CPLD��Ĵ���
			{
				ADJUST_SEC = g_timecom_buffer[5]+4;
			}
			if (g_timecom_buffer[5]==0)					
			{											 
				ADJUST_SEC = g_timecom_buffer[5]+64;			// ���ʱ��Ϊ��60��,��д��64: 0100,0000,����λ����������
				if (g_trans_flag_t==1)							// ���Ϊ��"��+����ʱ��"��ʽ,��϶�Ϊ��GPS��ʽ������ʽ,ÿ���Ӷ���RTCдһ��
				{
					Write_Time();
				}
			}
			bin_bcd();    										// ˢ��BCD���ʽ��ʱ�� 
			time_dis();   										// �������ݴ�ʱ��Ĵ�������ʾ�Ĵ�����    
			display_0();    									// ������ʾ����

			if(((g_errFlag>0x0A)&&(g_errFlag<0x4A)&&((HUB_STATE&0x07)==0x07))	// ��+�ⲿGPS
		       ||((g_errFlag>0x3A)&&(g_errFlag<0x7A)&&((HUB_STATE&0x07)==0x03))	// ��+�ڲ�GPS
				 ||((g_errFlag>0x6A)&&((HUB_STATE&0x01)==0x00)))// ���ϴ������,�ж����Ƿ����,�������,�����ϱ����Ϲ��ϴ���
			{		
				do 												// �ж�UART16552оƬ�Ƿ���Է������ݣ�
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0xaa;				   				// AA ��Ϊ�������ݵı�ͷ��
				Uart2_Thr = 0xaa;
				delay();
			
				do 												// �ж�UART16552оƬ�Ƿ���Է������ݣ�
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0x55;				   				// 0x55 0xCC ��ʾ�й��ϣ�
				Uart2_Thr = 0x55;
				delay();
			
				do 												// �ж�UART16552оƬ�Ƿ���Է������ݣ�
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0xCC;				   				// 0x55 0xCC ��ʾ�й��ϣ�
				Uart2_Thr = 0xCC;
				delay();
			
				do 												// �ж�UART16552оƬ�Ƿ���Է������ݣ�
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = g_errFlag;				   			// ���͹��ϱ�־g_errflag��
				Uart2_Thr = g_errFlag;
				delay();
	
				do 												// �ж�UART16552оƬ�Ƿ���Է������ݣ�
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0x99^g_errFlag;				   		// �������У��λ��AA��BB֮������ݵ����У�飩��
				Uart2_Thr = 0x99^g_errFlag;
				delay();
	
				do 												// �ж�UART16552оƬ�Ƿ���Է������ݣ�
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0XBB;				   				// ���ͽ�����־BB��
				Uart2_Thr = 0XBB;
				delay();
	
			   	g_errReport = 0x03;  							// �����Ѿ��ϱ�
				g_errFlagReg = g_errFlag;						// ����������ͣ��ڹ��ϻָ��ϱ���Ϣ��ʹ�õ�
				g_led_standby = 1;   							// =1������    	          2009-03-30      dongweijie
			}
			else
			{
				g_errFlag = 0x00;
			}

/*			if(g_errFlag>0x3A)&&(g_errFlag<0x7A)
			{										   	
 				if ((HUB_STATE&0x08)==0)  						// =0 �ò�����4800����ΪM12��NMEA��ʽ�õĲ�����Ϊ4800      
				{
					BRL = 0x64;									// 0X64 = 100 ������4800,���Ϊ0.16%;
					BDRCON = 0x1E; 								// 0001,1110���ڽ����뷢�͵Ĳ����ʾ��ɲ����ʷ���������,SPD=1,SMOD1=0
	 				Init_GPS_M12();								// M12��NMEA��ʽ�õ���4800������
				}
				else											// =1 �ò�����9600
				{
					BRL = 0xB2; 								// 0XB2 = 178 ������9600,���Ϊ0.16%;
					BDRCON = 0x1E; 								// 0001,1110���ڽ����뷢�͵Ĳ����ʾ��ɲ����ʷ���������,SPD=1,SMOD1=0
					Init_GPS_Ublox();				  			// Ublox nmea��ʽ�õ���9600������ 
				}  
			}  */

			if((g_errFlag==0x00)&&(g_errReport!=0)
				&&(((g_errFlagReg>0x0A)&&(g_errFlagReg<0x4A)&&((HUB_STATE&0x07)==0x07))  // ��+�ⲿGPS
		       		||((g_errFlagReg>0x3A)&&(g_errFlagReg<0x7A)&&((HUB_STATE&0x07)==0x03))// ��+�ڲ�GPS
				 	||((g_errFlagReg>0x6A)&&((HUB_STATE&0x01)==0x00))))				// �����Ѿ����������3���ϱ��ָ���Ϣ
			{																		// �˴���ֹ�󱨻ָ���Ϣ,�����ڲ��뿪�ظı�������
				do 												// �ж�UART16552оƬ�Ƿ���Է������ݣ�
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0xaa;				   				// AA ��Ϊ�������ݵı�ͷ��
				Uart2_Thr = 0xaa;
				delay();
			
				do 												// �ж�UART16552оƬ�Ƿ���Է������ݣ�
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0x55;				   				// 0x55 0xCC ��ʾ�й��ϣ�
				Uart2_Thr = 0x55;
				delay();
			
				do 												// �ж�UART16552оƬ�Ƿ���Է������ݣ�
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0xCC;				   				// 0x55 0xCC ��ʾ�й��ϣ�
				Uart2_Thr = 0xCC;
				delay();
			
				do 												// �ж�UART16552оƬ�Ƿ���Է������ݣ�
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = g_errFlagReg + 1;				   	// ���͹��ϻָ���־ g_errflag+1��
				Uart2_Thr = g_errFlagReg + 1;
				delay();
	
				do 												// �ж�UART16552оƬ�Ƿ���Է������ݣ�
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0x99^(g_errFlagReg + 1);			// �������У��λ��AA��BB֮������ݵ��ɻ�У�飩��
				Uart2_Thr = 0x99^(g_errFlagReg + 1);
				delay();
		
				do 												// �ж�UART16552оƬ�Ƿ���Է������ݣ�
				{
					g_adjust = Uart1_Lsr&0x60;
					g_adjust2 = Uart2_Lsr&0x60;
				}
				while((g_adjust!=0x60)||(g_adjust2!=0x60));  
				Uart1_Thr = 0XBB;				   				// ���ͽ�����־BB��
				Uart2_Thr = 0XBB;
				delay();
	
			   	g_errReport--;  								// ���ϴ��������־��λ			
			}
		
			if((g_trans_flag_t==1)&&(g_errFlag==0))
			{
				g_led_standby=~g_led_standby;      				//  ���������ñ���ʱ�ӣ����� 05.07.15   2009-03-30 dongweijie
			} 

			g_trans_flag = 0;
			g_trans_flag_t = 0;
			g_number_dog = 0;					
		} 
							
		Reset_Watchdog();									// ι���Ź�

		if (hub_state_pre != (HUB_STATE & 0x0F))			// �����ֲ��뿪�ظı�ʱ
		{
			if ((HUB_STATE&0x03)==1)  							      
			{												// =0������
				g_led_standby = 0;
			}
			else
			{
				g_led_standby = 1;   						// =1������    
			} 
 			if ((HUB_STATE&0x08)==0)  						// =0 �ò�����4800����ΪM12��NMEA��ʽ�õĲ�����Ϊ4800      
			{
				BRL = 0x64;									// 0X64 = 100 ������4800,���Ϊ0.16%;
				BDRCON = 0x1E; 								// 0001,1110���ڽ����뷢�͵Ĳ����ʾ��ɲ����ʷ���������,SPD=1,SMOD1=0
	 			Init_GPS_M12();								// M12��NMEA��ʽ�õ���4800������
			}
			else											// =1 �ò�����9600
			{
				BRL = 0xB2; 								// 0XB2 = 178 ������9600,���Ϊ0.16%;
				BDRCON = 0x1E; 								// 0001,1110���ڽ����뷢�͵Ĳ����ʾ��ɲ����ʷ���������,SPD=1,SMOD1=0
				Init_GPS_Ublox();				  			// Ublox nmea��ʽ�õ���9600������ 
			}  
			hub_state_pre = (HUB_STATE&0x0F); 
		} 

		g_clr_count++;

		if(g_clr_count==60000)									// �����ʱ��δ���ֽ����жϣ�����Ϊ���ӹ���
		{
			if ((HUB_STATE&0x07)==0x03)
			{
				g_errFlag = 0x5A;								// �ڲ�GPS���ӹ���
				g_gps_True_s = 0x03;							// ����豸���ϼ�����
			}
			if ((HUB_STATE&0x07)==0x07)
			{
				g_errFlag = 0x2A;								// �ⲿGPS���ӹ���
				g_gps_True = 0x03;								// ����豸���ϼ�����
			}
			if ((HUB_STATE&0x01)==0x00)
			{
				g_errFlag = 0x8A;								// ��������ʱ���ӹ���
				g_gps_True = 0x03;								// ����豸���ϼ�����
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