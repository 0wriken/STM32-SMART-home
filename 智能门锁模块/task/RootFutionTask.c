#include "task.h"
#include "ucos_ii.h"
#include "stdio.h"
#include "uart.h"
#include "delay.h"
#include "monochome_oled.h"
#include "MG200.h"
#include "funtion.h"
//管理员模式下的各种功能的实现
u8 funtion_error;
u16 funtion_flag;
u8 finger;
u8 finger_del;
u8 finger_delall;
u8 passwd_buf[9];
u8 root_buf[9];
extern u32 i;
extern u32 at;
u32 i1=0;
u32 i2[2];
u32 i3;
u16 flagi2,flagat,flagat1;
static u32 u=5;
void RootFutionTask(void *pd)
{
	while(1)
	{
	
			OSFlagPend (OSFLAGCHEK, 
                      1<<3,
											OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
                      0,
					                      &funtion_error);
//	funtion_flag=(u16)OSFlagPend (OSFLAGRootFution, 
//                      1<<9|1<<8|1<<7|1<<6|1<<5|1<<4|1<<3|1<<2|1<<1|1<<0,
//											OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
//											                     0,
//					                      &funtion_error);
//		
//		printf("funtion_flag****%d",funtion_flag);	
		
	loop1 :
		oled_display_clear();
		i1=0;	
		flagi2=0;
		memset((char *)i2,0,strlen((char *)i2));
		oled_display_arbitrary_string(0,0,(u8 *)"已进入管理员模式");
		oled_display_arbitrary_string(4,0,(u8 *)"请输入指令");
		oled_display_arbitrary_string(6,0,(u8 *)"#确认");
		oled_display_arbitrary_string(6,86,(u8 *)"*退出");  
		while(1) //当无按键按下时候
		{
			if(i>5)
				i=0;
			i3=root_get_key();
		if(i3!=88&&i3!=-48)  //判断有按键按下时
		{
			printf("i3:%d\r\n",i3);

			if(i3==-13||i3==-6)
			{
				i2[1]=i3;
				flagi2=1;
					printf("i2[1]:%d\r\n",i2[1]);
			}
			if(flagi2==0)
				i2[0]=i3;      //第一位为数字指令
				printf("i2[0]:%d\r\n",i2[0]);
			}
		
		if(i2[1]==-6||i2[0]==-6)   //按键为*时退出
			goto loop1; 
			if(i2[1]==-13)//当有确认键
			{	
				printf("指令为%d:\r\n",i2[0]);
				break;
			}
		}
	switch(i2[0])
	{
		case 1://修改按键密码
			printf("已进入修改开门密码模式\r\n");
					if(	get_key_buf(passwd_buf)==1)  //获取8个数据的值
			{				
				printf("修改后的密码为：%s\r\n",(char*)passwd_buf);
//				voice_data(0X02);
				if(flagat==0)  //第一次进入开门密码修改
				{
					
					at=at+(u*8);
					At24c02WriteBytes(at+8,8,passwd_buf);//地址40位存放开门密码传入数组中
					flagat=1;
					break;
				}
					if(flagat!=0)   
				{
					
					at=+(flagat*8);
					At24c02WriteBytes(at+8,8,passwd_buf);//地址0位存放管理员密码传入数组中
					flagat++;
					break;
				}
			}
		case 2://修改管理员密码
					if(	get_key_buf(root_buf)==1)  //获取8个数据的值
			{
					voice_data(0x01);
				if(flagat==0)  //第一次修改管理员密码
				{
					at=at+(u*8);
					At24c02WriteBytes(at,8,root_buf);//地址40位存放开门密码传入数组中
					flagat=1;
					break;
				}
				if(flagat!=0)   
				{
					at=+(flagat*8);
					At24c02WriteBytes(at,8,root_buf);//地址0位存放管理员密码传入数组中
					flagat++;
					break;
				}
			}
		case 3:while(1);break;
		case 4:while(1);break;
		case 5: while(1);break;
		case 6:while(1);break;
		case 7:while(1);break;
		case 8:finger=Enroll();   //添加指纹		 
			switch((u16 )finger)
		{
					case 0x00:oled_display_arbitrary_string(4 ,0, (u8*)"注册成功\r\n");break;
					case 0x83:oled_display_arbitrary_string(4 ,0, (u8*)"ID 错误(ID < 0 或者 ID > 最大用户数)或者通信错误\r\n");break;
					case 0x91:oled_display_arbitrary_string(4 ,0, (u8*)"用户区域已满\r\n");break;
					case 0x93:oled_display_arbitrary_string(4 ,0, (u8*)"已经注册的 ID\r\n");break;
					case 0x94:oled_display_arbitrary_string(4 ,0, (u8*)"指纹提取次数 < 3\r\n");break;		
		}
		case 9:  finger_del=EraseOne(Match1N());//删除指纹
		switch(finger_del)
			{ 
				case 0x00:oled_display_clear(); oled_display_arbitrary_string(4 ,0, (u8*)"删除成功\r\n");break;
				case 0x83:oled_display_clear();oled_display_arbitrary_string(4 ,0, (u8*)"删除失败\r\n");break;
				case 0x90:oled_display_clear();oled_display_arbitrary_string(4 ,0, (u8*)"未注册的用户\r\n");break;
				case 0xFF:oled_display_clear();oled_display_arbitrary_string(4 ,0, (u8*)"未注册的用户\r\n");break;
			}
			break;
		case 0:finger_delall=EraseAll(); //删除所有指纹
			switch(finger_del)
		{
					case 0x00:oled_display_arbitrary_string(4 ,0, (u8*)"删除成功\r\n");break;
					case 0x90:oled_display_arbitrary_string(4 ,0, (u8*)"注册数量为 0\r\n");break;
					default:oled_display_arbitrary_string(4 ,0, (u8*)"删除失败\r\n");break;
		}
		break;
		case 10:while(1);break;  
		case 20://恢复出厂设置
		format();
		break;   
	}
}
}




