#include "task.h"
#include "ucos_ii.h"
#include "stdio.h"
#include "uart.h"
#include "delay.h"
#include "monochome_oled.h"
#include "funtion.h"
u8 error4,error5,error6,error7;
u8 check_flag;
u8 finger_mesg;
u8 read_passwd_buf[9]={0};
u8 root_passwd_buf[9]={0};
extern u8 key_buf[9];
u8 check_buf[8]={0};
extern u32 at;
extern u16 flagat;
void CheckPwTask(void *pd)
{
	while(1)
	{
	check_flag=OSFlagPend (OSFLAGGET, 
                      1<<2|1<<1|1<<0,
											OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
                      0,
                     &error4);
	if(check_flag==1) //�����ж�
	{
		if(flagat==0)
		{
//			At24c02ReadBytes(16,9,read_passwd_buf);
			memset((char *)read_passwd_buf,0,strlen((char *)read_passwd_buf));
			memset((char *)root_passwd_buf,0,strlen((char *)root_passwd_buf));
			At24c02ReadBytes(at+16,8,read_passwd_buf);
			printf("��������%s\r\n",(char *)read_passwd_buf);
		
			At24c02ReadBytes(at,8,root_passwd_buf);
			printf("����Ա����%s\r\n",(char *)root_passwd_buf);
		}
		if(flagat!=0)
		{
			//			At24c02ReadBytes(16,9,read_passwd_buf);
			memset((char *)read_passwd_buf,0,strlen((char *)read_passwd_buf));
			memset((char *)root_passwd_buf,0,strlen((char *)root_passwd_buf));
			At24c02ReadBytes(at+8,8,read_passwd_buf);
			printf("��������%s\r\n",(char *)read_passwd_buf);
		
			At24c02ReadBytes(at,8,root_passwd_buf);
			printf("����Ա����%s\r\n",(char *)root_passwd_buf);
		}
		
////		printf("�ı�%s",(char *)change_buf(read_passwd_buf));

////		printf("�ı�%s",(char *)change_buf(key_buf));
		for(int t=0;t<8;t++)
		{
			check_buf[t]= key_buf[t];
		}
			printf("���������Ϊ%s\r\n",(char *)check_buf);
		
		if(strcmp((char *)check_buf,(char *)read_passwd_buf)==0)   //�뿪������ıȽ�
		{
			OSFlagPost (OSFLAGCHEK, 
                      1<<0,    
											OS_FLAG_SET,
                     &error5);
		}
		if(strcmp((char *)check_buf,(char *)root_passwd_buf)==0)    //�����Ա����ıȽ�
		{
								OSFlagPost (OSFLAGCHEK, 
                      1<<3,    
											OS_FLAG_SET,
                     &error5); //3λ����Ա
		}
		//��֤�ɹ�
		
			

	//��֤ʧ��		
			OSFlagPost (OSFLAGFAIL, 
                      1<<0,    
											OS_FLAG_SET,
                     &error5); 
	}
	if(check_flag==2)  //ָ���ж�
	{
		if(finger_mesg)
		{
			printf("׼������\r\n");
			//��֤�ɹ�
			OSFlagPost (OSFLAGCHEK, 
                      1<<1,    
											OS_FLAG_SET,
                     &error6);  
		}
		//��֤ʧ��
		OSFlagPost (OSFLAGFAIL, 
                      1<<1,    
											OS_FLAG_SET,
                     &error6);  
	}
	if(check_flag==4) //ˢ���ж�
	{
		//��֤�ɹ�
		OSFlagPost (OSFLAGCHEK, 
                      1<<2,    
											OS_FLAG_SET,
                     &error7); 
		//��֤ʧ��
		OSFlagPost (OSFLAGFAIL, 
                      1<<2,    
											OS_FLAG_SET,
                     &error7); 
	}
}
}