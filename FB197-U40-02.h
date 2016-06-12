/********************************************************************************
*	CopyRight(c) 2006, HollySys													*
*	All rights reserved.														*
*																				*
*	�ļ����ƣ�NB197-U40-01.h															*
*																				*
*	�ļ��汾��01																*
*																				*
*	ժҪ�������ʼ�� admit.c �ļ����õ�����ȫ�ֱ���								*
*																				*
*	���ߣ���ΰ��																*
*																				*
*	������ڣ�2009-03-02														* 
*																				*
*	�޸��ߣ�																	*
*																				*
*	�޸����ڣ�																	* 
********************************************************************************/

#include <absacc.h>
#include <stdio.h>
#include <intrins.h>
#include <reg51xd2.h>

#define 	uchar 					unsigned 	char
#define  	uint 					unsigned 	int

/* ��ַ���� */
#define 	HUB_STATE  	XBYTE 		[0xbfff]	// ��ʱ������״̬����ַ��              
#define 	READ_SEC 	XBYTE 		[0xcfff]	// ��EPM570��Ĵ�����ַ��
#define 	ADJUST_SEC 	XBYTE 		[0xdfff]	// дEPM570��Ĵ�����ַ��ADJUST_SEC
#define 	DAT_8279 	XBYTE 		[0xefff]  	// ��д8279���ݵ�ַ����������ʾ����ģ�µ�8279оƬ����

/*���϶���*/
#define		GPS_ERR_CC				0xCC		// GPS�������
#define		GPS_ERR_DD				0XDD		// GPS�������
#define		GPS_ERR_LL				0XEE		// GPS���ӹ���

/* DS3231 */     								// dongweijie 2008-09-03
sbit    	scl = P1^4;     					// I2C ���߶��壻
sbit    	sda = P1^5;
#define 	ADDRTC  	0xd0    				// DS3231 Ĭ�ϴӵ�ַ��
#define 	ACK     	0						// I2CЭ����Ӧ��
#define 	NACK    	1						// I2CЭ���Ӧ��
uchar data  sec, min, hr, dy, dt, mn, yr; 		// ʱ���������ձ������壻

/* X5045 */      								// dongweijie 2009-02-03
sbit SSEL = P3^5;  								// ģ��x5045��CS��
sbit MISO = P1^2;								// ģ��x5045��SO��
sbit MOSI = P1^7;								// ģ��x5045��SI��
sbit SCLK = P1^6;								// ģ��x5045��SCK��

#define 	WREN 		06						// ʹ��д����
#define 	WRDI 		04						// ��ֹд����
#define 	WRSR 		01						// д״̬�Ĵ���
#define 	RDSR 		05						// ��״̬�Ĵ���
#define 	MAX_POLL  	99						// ���д����ʱ��,ȷ��5045������д����̵�ʱ��
              
/* PC16552D �Ĵ�������*/
#define 	Uart1_Rhr 	XBYTE		[0xf8ff] 	/*����ʱ��Lcr.7=0 */
#define 	Uart1_Thr 	XBYTE 		[0xf8ff] 	/*����ʱ��Lcr.7=0 */
#define 	Uart1_Ier 	XBYTE 		[0xf9ff] 	/*����ʱ��Lcr.7=0 */
#define 	Uart1_Isr 	XBYTE 		[0xfaff] 	/*����ʱ��Lcr.7=0 */
#define 	Uart1_Fcr 	XBYTE 		[0xfaff] 	/*����ʱ��Lcr.7=0 */
#define 	Uart1_Lcr 	XBYTE 		[0xfbff] 	/*����ʱ��Lcr.7=0��1������ */
#define 	Uart1_Lsr 	XBYTE 		[0xfdff] 	/*����ʱ��Lcr.7=0��1������ */
#define 	Uart1_Scr 	XBYTE 		[0xffff] 
#define 	Uart1_Divl  XBYTE 		[0xf8ff] 	/*����ʱ��Lcr.7=1 */
#define 	Uart1_Divm  XBYTE 		[0xf9ff] 	/*����ʱ��Lcr.7=1 */
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

/*�й���ʾ�õ���ȫ�ֱ���*/
uchar code 	display_table[12] 	= 	{0x0C,0x9F,0x4A,0x0B,0x99,0x29,
                             		0x28,0x8F,0x08,0x09,0xFF,0x00 };   	// {0 1 2 3 4 5 6 7 8 9 ȫ��ʾ,��}	  dongweijie 2008-09-04
uchar code 	month_table[12] 	= 	{31,28,31,30,31,30,31,31,30,31,
                             		30,31};                             
uchar data 	display_buffer[14]	=	{11,11,11,11,11,11,11,	 			// ��ʾ�Ĵ�����ֵ��
                                	11,11,11,11,11,11,11};                                                                                           
uchar 	 data 		g_time_buffer[14];  			// year ,month,day,hour,minute,second  BCD��ʽ
uchar  	 data 		g_timecom_buffer[6]; 			// year,month,day,hour,minute,second   �����Ƹ�ʽ 
uchar   idata 		g_adj[14];  					// UART���յ���GPS��ʱ����;                                
uchar   idata 		g_gps_sec_pre = 0;  			// UART���յ���GPS��ʱ"�ϴ���"����;                                
uchar   idata 		g_gps_True = 3;  				// UART���յ���GPS��ʱ"�ϴ���"����;                                
uchar 	 data   	g_check_xor = 0x66;				// UART�����������У��λ  GPZDA���ȥʱ����Ϣ������ֵ��0x66
uchar 	 data 		g_couter = 0;					// ��UART�����ݼ�������
uchar    data 		g_adj_s[14];  					// ���ڽ��յ���GPS��ʱ����;                                
uchar   idata 		g_gps_sec_pre_s = 0;  			// ���ڽ��յ���GPS��ʱ"�ϴ���"����;                                
uchar   idata 		g_gps_True_s = 3;  				// ���ڽ��յ���GPS��ʱ"�ϴ���"����;                                
uchar 	 data		g_check_xor_s = 0x66;			// ���ڽ����������У��λ  GPZDA���ȥʱ����Ϣ������ֵ��0x66
uchar 	 data 		g_couter_s = 0;					// �Ӵ��ڶ����ݼ�������

sbit 				g_led_standby	= P1^3;  			// ��ʱ������ǰ���LED��ʾ���ƿ��ߣ�            
                            
bit 	bdata 		g_adjust_flag = 0;				// �ֶ����ñ�ʶ��
bit 	bdata 		g_spark_flag = 0;				// �ֶ�����ʱ�������˸��ʶ��
bit 	bdata 		g_trans_flag = 0;				// �������ݳɹ���ʾ��
bit 	bdata 		g_trans_flag_t = 0;				// ����ʱ���ݴ���ɹ���ʾ��
bit 	bdata 		g_gps_flag=0;					// �Ƿ�ʹ��GPS��ʶ��

uchar 	idata 		g_couter_10ms = 0;				// �ֶ�����ʱ��˸�����ã�
uchar	idata 		g_adjust = 0;					// UARTоƬ״̬��д�ã� 
uchar 	idata 		g_adjust2 = 0;						// UARTоƬ״̬��д�ã� 
uchar 	idata 		g_couter_adj = 0;				// �ֶ����õ������λ����������
uchar 	idata 		g_errFlag = 0;		  			// GPS �źŹ������ͱ�־ 0x1(4/7)A ��ʾ������� 0x2(5/8)A��ʾ���ӹ��� 0x3(6)A���豸����
uchar	idata		g_errFlagReg = 0;				// ����������ʱ�Ĵ���
uchar 	idata		g_errReport = 0;				// ���ϴ������
uchar   idata		g_err_A = 0;					// A ��������
uchar 	idata     	g_number_dog = 0;				// һЩ�����õı�����
uint 	idata 		g_clr_count = 0;				// һЩ�����õı�����

