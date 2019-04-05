#include "UPLib\\UP_System.h"
#include "UP_Bluetooth.h"
#include "UP_Globle.h"
#include "UP_UART.h" 




int AD1 = 0;		//底部前方红外光电
int AD2 = 0;		//底部右侧红外光电
int AD3 = 0;		//底部后方红外光电
int AD4 = 0;		//底部左侧红外光电

int AD5 = 0;		//前红外测距传感器
int AD6 = 0;		//右红外测距传感器
int AD7 = 0;		//后红外测距传感器
int AD8 = 0;		//左红外测距传感器

int AD9 = 0;		//左前防掉台红外光电
int AD10 = 0;		//右前防掉台红外光电
int AD11 = 0;		//右后防掉台红外光电
int AD12 = 0;		//左后防掉台红外光电

int AD15 = 0;		//倾角传感器

int nStage = 0;	//检测在台上还是在台下
int nEdge = 0;	//边缘
int nEnemy = 0;	//敌人
int nFence = 0;	//台下状态

int Qian = 0;		//前有无敌人
int You = 0;		//有有无敌人
int Hou = 0;		//后有无敌人
int Zuo = 0;		//左有无敌人
int Summ = 0;	//前后左右几个检测到		

int left = 0;
int right = 0;

int na = 0;     //倾斜计时
int nb = 0;			//推箱子计时
int nc = 8;			//旋转计时
int nd = 0;			//前搁浅计时
int ne = 8;			//后搁浅计时

int QX = 0;			//倾斜

void zhong()    //上台默认动作
{
	UP_CDS_SetAngle(5, 384, 800);
	UP_CDS_SetAngle(6, 640, 800);
	UP_CDS_SetAngle(7, 640, 800);
	UP_CDS_SetAngle(8, 384, 800);
}

void qding()  //收前爪
{
	UP_CDS_SetAngle(5, 1000, 800);
	UP_CDS_SetAngle(6, 24, 800);
	UP_delay_ms(10);
	UP_CDS_SetAngle(5, 1000, 800);
	UP_CDS_SetAngle(6, 24, 800);
}

void hding()  //收后爪
{
	UP_CDS_SetAngle(7, 24, 800);
	UP_CDS_SetAngle(8, 1000, 800);
	UP_delay_ms(10);
	UP_CDS_SetAngle(7, 24, 800);
	UP_CDS_SetAngle(8, 1000, 800);
}

void chanzi()  //上台后铲子状态
{
	UP_CDS_SetAngle(5, 780, 800);
	UP_CDS_SetAngle(6, 244, 800);
	UP_CDS_SetAngle(7, 244, 800);
	UP_CDS_SetAngle(8, 780, 800);
}

void move(int forward,int turn)
{
	left = forward;
	right = turn;
	if(left > 1023)
	{
		left = 1023;
	}
	if(left < -1023)
	{
		left = -1023;
	}
	if(right > 1023)
	{
		right = 1023;
	}
	if(right < -1023)
	{
		right = -1023;
	}
	UP_CDS_SetSpeed(1, left);
	UP_CDS_SetSpeed(2, -right);
	UP_CDS_SetSpeed(3, left);
	UP_CDS_SetSpeed(4, -right);
}


qianshangtai()  //前上台
{		move(0,0);	//停下来，防止前一状态是转弯改变上台方向
		UP_delay_ms(100);
		zhong();//四支架抬起为支擂台做准备
		UP_delay_ms(400);
		move(500,500);//对准擂台
		UP_delay_ms(700);
		qding();//前爪支地
		UP_delay_ms(800);
		UP_CDS_SetAngle(5, 384, 512);
		UP_CDS_SetAngle(6, 640, 512);
		UP_delay_ms(500);
		hding();//支后腿
		UP_delay_ms(800);
		zhong();
		//chanzi();//收后爪到铲子状态
		UP_delay_ms(1000);
		move(0,0);	//
		UP_delay_ms(500);
}

houshangtai()  //后上台
{
		move(0,0);	//停下来，防止前一状态是转弯改变上台方向
		UP_delay_ms(100);
		zhong();//四支架抬起为支擂台做准备
		UP_delay_ms(400);
		move(-500,-500);//对准擂台
		UP_delay_ms(600);
		hding();//前爪支地
		UP_delay_ms(900);
		UP_CDS_SetAngle(7, 640, 512);
		UP_CDS_SetAngle(8, 384, 512);
		UP_delay_ms(500);
		qding();//支后腿
		UP_delay_ms(800);
		zhong();
		//chanzi();//收后爪到铲子状态
		UP_delay_ms(1000);
		move(0,0);	//
		UP_delay_ms(500);
}


unsigned char Stage()//检测是否在台上
{
	AD15 = UP_ADC_GetValue(15); 
	Qian = (!UP_ADC_GetIO(1))|UP_ADC_GetIO(5);
	You  = (!UP_ADC_GetIO(2))|UP_ADC_GetIO(6);
	Hou  = (!UP_ADC_GetIO(3))|UP_ADC_GetIO(7);
	Zuo  = (!UP_ADC_GetIO(4))|UP_ADC_GetIO(8);
	Summ = Qian + Hou + Zuo + You;
	if((AD15 > 2000)&&(AD15 < 2400))
	{
		if (Summ>=2)
		{
			return 0;  //在台下
		}
		else
		{
			return 1;   //在台上
		}
	}
	else if (AD15<=2000)
	{
		return 3;   //卡在擂台左侧在地面右侧在擂台
	}
	else 
	{
		return 4;   //卡在擂台右侧在地面左侧在擂台
	}
}


#define FD  150  //
#define RD  150	 //
#define BD  150	 //
#define LD  180  //
unsigned char Fence()//在台下检测朝向
{
	AD1 = UP_ADC_GetValue(1); //底部前方红外光电
	AD2 = UP_ADC_GetValue(2); //底部右侧红外光电
	AD3 = UP_ADC_GetValue(3); //底部后方红外光电
	AD4 = UP_ADC_GetValue(4); //底部左侧红外光电
	AD5 = UP_ADC_GetValue(5); //前红外测距传感器
	AD6 = UP_ADC_GetValue(6); //右红外测距传感器
	AD7 = UP_ADC_GetValue(7); //后红外测距传感器
	AD8 = UP_ADC_GetValue(8); //左红外测距传感器
	
/////////////////////////对擂台，一个测距检测到/////////////////////////
	if((AD3 < 1000)&&(AD2 > 1000)&&(AD4 > 1000)&&(AD5 > FD)&&(AD6 < RD)&&(AD7 < BD)&&(AD8 < LD))//前测距和后红外
		{
			return 1;		//在台下，后方对擂台
		}
	
	if((AD4 < 1000)&&(AD1 > 1000)&&(AD3 > 1000)&&(AD5 < FD)&&(AD6 > RD)&&(AD7 < BD)&&(AD8 < LD))//右侧距左红外
		{
			return 2;		//在台下，左侧对擂台
		}		
	
	if((AD1 < 1000)&&(AD2 > 1000)&&(AD4 > 1000)&&(AD5 < FD)&&(AD6 < RD)&&(AD7 > BD)&&(AD8 < LD))//后测距前红外
		{
			return 3;		//在台下，前方对擂台
		}		
	
	if((AD2 < 1000)&&(AD1 > 1000)&&(AD3 > 1000)&&(AD5 < FD)&&(AD6 < RD)&&(AD7 < BD)&&(AD8 > LD))//左测距右红外
		{
			return 4;		//在台下，右侧对擂台
		}	
/////////////////////////对围栏，两个相邻测距检测到/////////////////////////
	
	if((AD2 > 1000)&&(AD3 > 1000)&&(AD5 > FD)&&(AD6 < RD)&&(AD7 < BD)&&(AD8 > LD))
		{
			return 5;		//在台下，前左检测到围栏
		}	
	if((AD3 > 1000)&&(AD4 > 1000)&&(AD5 > FD)&&(AD6 > RD)&&(AD7 < BD)&&(AD8 < LD))
		{
			return 6;		//在台下，前右检测到围栏
		}		

	if((AD1 > 1000)&&(AD4 > 1000)&&(AD5 < FD)&&(AD6 > RD)&&(AD7 > BD)&&(AD8 < LD))
		{
			return 7;		//在台下，后右检测到围栏
		}	
	if((AD1 > 1000)&&(AD2 > 1000)&&(AD5 < FD)&&(AD6 < RD)&&(AD7 > BD)&&(AD8 > LD))
		{
			return 8;		//在台下，后左检测到围栏
		}		

/////////////////////////台上有敌人，两个相对测距检测到/////////////////////////
	if((AD5 > FD)&&(AD6 < RD)&&(AD7 > BD)&&(AD8 < LD))
		{
			return 9;		//在台下，前方或后方有台上敌人
		}	
	
	if((AD5 < FD)&&(AD6 > RD)&&(AD7 < BD)&&(AD8 > LD))
		{
			return 10;	//在台下，左侧或右侧由台上敌人
		}	
		
/////////////////////////三侧有障碍，三个测距检测到/////////////////////////
	if((AD5 > FD)&&(AD6 > RD)&&(AD7 < BD)&&(AD8 > LD))
		{
			return 11;	//在台下，前方、左侧和右侧检测到围栏
		}	
	if((AD5 > FD)&&(AD6 > RD)&&(AD7 > BD)&&(AD8 < LD))
		{
			return 12;	//在台下，前方、右侧和后方检测到围栏
		}	
	if((AD5 > FD)&&(AD6 < RD)&&(AD7 > BD)&&(AD8 > LD))
		{
			return 13;	//在台下，前方、左侧和后方检测到围栏
		}	
	if((AD5 < FD)&&(AD6 > RD)&&(AD7 > BD)&&(AD8 > LD))
		{
			return 14;	//在台下，右侧、左侧和后方检测到围栏
		}	
		
		
/////////////////////////斜对擂台，两个红外光电检测到/////////////////////////
	if((AD1 < 1000)&&(AD2 < 1000)&&(AD5 < FD)&&(AD6 < RD))
		{
			return 15;	//在台下，前方和右侧对擂台其他传感器没检测到
		}	
	if((AD1 < 1000)&&(AD4 < 1000)&&(AD5 < FD)&&(AD8 < LD))
		{
			return 16;	//在台下，前方和左侧对擂台其他传感器没检测到
		}
	if((AD2 < 1000)&&(AD3 < 1000)&&(AD6 < FD)&&(AD7 < RD))
		{
			return 17;	//在台下，后方和右侧对擂台其他传感器没检测到
		}	
	if((AD3 < 1000)&&(AD4 < 1000)&&(AD7 < FD)&&(AD8 < LD))
		{
			return 18;	//在台下，后方和左侧对擂台其他传感器没检测到
		}
	
	//////////////
	else
		{
			return 101;//错误
		}
		
}

unsigned char Edge()  //检测边缘
{
	AD9 = UP_ADC_GetValue(9);   //左前红外光电传感器
	AD10 = UP_ADC_GetValue(10); //右前红外光电传感器
	AD11 = UP_ADC_GetValue(11); //右后红外光电传感器
	AD12 = UP_ADC_GetValue(12); //左后红外光电传感器
	AD5 = UP_ADC_GetValue(5); //获取前方测距值
	AD7 = UP_ADC_GetValue(7); //获取后方测距值
	if((AD9 < 1000)&&(AD10 < 1000)&&(AD11 < 1000)&&(AD12 < 1000))
	{
		return 0;		//没有检测到边缘
	}
		else if((AD9 > 1000)&&(AD10 < 1000)&&(AD11 < 1000)&&(AD12 < 1000))
		{
			return 1;		//左前检测到边缘
		}
			else if((AD9 < 1000)&&(AD10 > 1000)&&(AD11 < 1000)&&(AD12 < 1000))
		{
			return 2;		//右前检测到边缘
		}
		else if((AD9 < 1000)&&(AD10 < 1000)&&(AD11 > 1000)&&(AD12 < 1000))
		{
			return 3;		//右后检测到边缘
		}
		else if((AD9 < 1000)&&(AD10 < 1000)&&(AD11 < 1000)&&(AD12 > 1000))
		{
			return 4;		//左后检测到边缘
		}
		else if((AD9 > 1000)&&(AD10 > 1000)&&(AD11 < 1000)&&(AD12 < 1000))
		{
			return 5;		//前方两个检测到边缘
		}
			else if((AD9 < 1000)&&(AD10 < 1000)&&(AD11 > 1000)&&(AD12 > 1000))
		{
			return 6;		//后方两个检测到边缘
		}
		else if((AD9 > 1000)&&(AD10 < 1000)&&(AD11 < 1000)&&(AD12 > 1000))
		{
			return 7;		//左侧两个检测到边缘
		}
		else if((AD9 < 1000)&&(AD10 > 1000)&&(AD11 > 1000)&&(AD12 < 1000))
		{
			return 8;		//右侧两个检测到边缘
		}	
		else if((AD5 > 480)&&(AD9 > 1000)&&(AD10 > 1000)&&(AD11 > 1000)&&(AD12 > 1000))
		{
			return 9;		//搁浅钱放在擂台下
		}	
		else if((AD7 > 480)&&(AD9 > 1000)&&(AD10 > 1000)&&(AD11 > 1000)&&(AD12 > 1000))
		{
			return 10;		//搁浅钱放在擂台上
		}
		else 
		{
			return 102;		//错误
		}
}

unsigned char Enemy()   //检测敌人
{
	AD1 = UP_ADC_GetValue(1); // 前红外光电传感器
	AD2 = UP_ADC_GetValue(2); //右红外光电传感器
	AD3 = UP_ADC_GetValue(3); //后红外光电传感器
	AD4 = UP_ADC_GetValue(4); //左红外光电传感器
	AD5 = UP_ADC_GetValue(5); //前红外测距传感器
	
	if ((AD1 > 100)&&(AD2 > 100)&&(AD3 > 100)&&(AD4 > 100))
	{
		return 0;  //无敌人
	}
	if ((AD1 < 100)&&(AD2 > 100)&&(AD3 > 100)&&(AD4 > 100))
	{
		if (AD5>1000)
		{
			return 11;  //前方是箱子
		}
		else
			{
				return 1;   //前方有棋子
			}
	}
	if ((AD1 > 100)&&(AD2 < 100)&&(AD3 > 100)&&(AD4 > 100))
	{
		return 2;   //右侧有敌人或棋子
	}
	if ((AD1 > 100)&&(AD2 > 100)&&(AD3 < 100)&&(AD4 > 100))
	{
		return 3;   //后方有敌人或棋子
	}
	if ((AD1 > 100)&&(AD2 > 100)&&(AD3 > 100)&&(AD4 < 100))
	{
		return 4;   //左侧有敌人或棋子
	}
	else
	{
		return 103;//错误
	}
}

//蓝牙发送数据声明区
	char ms1[]={"系统已启动"};
	char ms2[]={"前上台"};
	char ms3[]={"检测擂台"};
	char ms4[]={"在台下"};

//主函数
int main()
{
	/*初始化系统*/
	UP_System_Init();
	//printf("Demo15 Woody_ImageRecog\n");	//显示Demo名称
	//UP_LCD_ShowCharacterString(0,1,"    博创科技");					//显示汉字字符串LOGO
	//UP_LCD_ShowCharacterString(0,2,"卓越之星创意套件");			//显示汉字字符串LOGO
	//UP_delay_ms(2000); 
	//UP_LCD_ClearScreen();
  UP_delay_ms(100); 
	UP_CDS_SetMode(1, CDS_MOTOMODE);
	UP_CDS_SetMode(2, CDS_MOTOMODE);
	
	UP_CDS_SetMode(5, CDS_SEVMODE);
	UP_CDS_SetMode(6, CDS_SEVMODE);
	UP_CDS_SetMode(7, CDS_SEVMODE);
	UP_CDS_SetMode(8, CDS_SEVMODE);

	zhong();
	UP_delay_ms(1000);
	
	
	UP_EnableBluetoothIT(9600);

	UP_Bluetooth_Puts (ms1);


	
  while(1)
	{
		if((UP_ADC_GetValue(2) < 1000)||(UP_ADC_GetValue(4) < 1000))
		{
			break;
		}
		UP_delay_ms(10);
	}
	
	qianshangtai();
	UP_Bluetooth_Puts (ms2);
	
while(1)
{	
	nStage = Stage();	//检测擂台
	
	UP_Bluetooth_Puts (ms3);
 	
 	UP_LCD_ShowInt(0, 0, nStage);
		switch(nStage)
		{
		  case 0:	//在台下
// 				move(500,500);	
// 				UP_delay_ms(200);
			UP_Bluetooth_Puts (ms4);
			
				nFence = Fence();	//检测边沿
			 	UP_LCD_ClearScreen();
			
			

				UP_LCD_ShowInt(1, 1, nFence);
				switch(nFence)
				{
					case 1:	//在台下后方对擂台
						houshangtai();
						break;
					case 2:	//左侧对擂台
						move(0,0);	
						UP_delay_ms(200);
						while(1)
						{	
							AD1 = UP_ADC_GetValue(1); 
							AD6 = UP_ADC_GetValue(6);
							AD4 = UP_ADC_GetValue(4);
							if((AD1<1000)&&(AD6<160)&&(AD4>1000))
								{
									UP_delay_ms(200);
									move(400,400);	
									UP_delay_ms(300);
									break;
								}
								else
								{
									move(-400,400);	
									UP_delay_ms(2);
								}
						}
						break;
					case 3:	//前方对擂台
						qianshangtai();
						break;
					case 4:	//右侧对擂台
						move(0,0);	
						UP_delay_ms(200);
						while(1)
						{	
							AD1 = UP_ADC_GetValue(1); 
							AD6 = UP_ADC_GetValue(6);
							AD4 = UP_ADC_GetValue(4);
							if((AD1<1000)&&(AD6<160)&&(AD4>1000))
								{
									UP_delay_ms(200);
									move(400,400);	
									UP_delay_ms(300);
									break;
								}
								else
								{
									move(400,-400);	
									UP_delay_ms(2);
								}
						}
						break;
					case 5:	//前左检测到围栏
						move(-400,-400);	
				  	UP_delay_ms(400);
						break;
					case 6:	//前右检测到围栏
						move(-400,-400);	
 						UP_delay_ms(400);
 						break;
					case 7:	//后有检测到围栏
						move(400,400);	
 						UP_delay_ms(400);
						break;
					case 8:	//后左检测到围栏
						move(400,400);	
 						UP_delay_ms(400);
						break;
					case 9:	//前方或后方有台上敌人
						move(500,-500);	
						UP_delay_ms(300);
						move(400,400);	
						UP_delay_ms(400);
						break;
					case 10:	//左侧或右侧有台上敌人
						move(400,400);	
						UP_delay_ms(400);
						break;
					case 11:	//前方、左侧和右侧检测到围栏
						move(-400,-300);	
						UP_delay_ms(500);
						move(-400,400);	
						UP_delay_ms(300);
						break;
					case 12:	//前右后检测到围栏
						move(300,600);	
						UP_delay_ms(400);
						break;
					case 13:	//前左后检测到围栏
						move(600,300);	
						UP_delay_ms(400);
						break;
					case 14:	//右左后检测到围栏
						move(-400,400);	
						UP_delay_ms(200);
						move(400,400);	
						UP_delay_ms(300);
						break;
					case 15:	//前右检测到擂台
						move(0,0);	
						UP_delay_ms(200);
						while(1)
						{	
							AD1 = UP_ADC_GetValue(1); 
							AD6 = UP_ADC_GetValue(6);
							AD4 = UP_ADC_GetValue(4);
							if((AD1<1000)&&(AD6<160)&&(AD4>1000))
								{
									UP_delay_ms(200);
									move(400,400);	
									UP_delay_ms(300);
									break;
								}
								else
								{
									move(360,-400);	
									UP_delay_ms(2);
								}
						}
						break;
					case 16:	//前左检测到擂台
						move(0,0);	
						UP_delay_ms(200);
						while(1)
						{	
							AD1 = UP_ADC_GetValue(1); 
							AD6 = UP_ADC_GetValue(6);
							AD4 = UP_ADC_GetValue(4);
							if((AD1<1000)&&(AD6<160)&&(AD4>1000))
								{
									UP_delay_ms(200);
									move(400,400);	
									UP_delay_ms(300);
									break;
								}
								else
								{
									move(-400,400);	
									UP_delay_ms(2);
								}
						}
						break;
						case 17:	//在台下，后方和右侧对擂台其他传感器没检测到
						move(0,0);	
						UP_delay_ms(200);
						while(1)
						{	
							AD1 = UP_ADC_GetValue(1); 
							AD6 = UP_ADC_GetValue(6);
							AD4 = UP_ADC_GetValue(4);
							if((AD1<1000)&&(AD6<160)&&(AD4>1000))
								{
									UP_delay_ms(200);
									move(400,400);	
									UP_delay_ms(300);
									break;
								}
								else
								{
									move(400,-500);	
									UP_delay_ms(2);
								}
						}
						break;
					case 18:	//在台下，后方和左侧对擂台其他传感器没检测到
						move(0,0);	
						UP_delay_ms(200);
						while(1)
						{	
							AD1 = UP_ADC_GetValue(1); 
							AD6 = UP_ADC_GetValue(6);
							AD4 = UP_ADC_GetValue(4);
							if((AD1<1000)&&(AD6<160)&&(AD4>1000))
								{
									UP_delay_ms(200);
									move(400,400);	
									UP_delay_ms(300);
									break;
								}
								else
								{
									move(-420,350);	
									UP_delay_ms(2);
								}
						}
						break;
					case 101:	//错误
						UP_delay_ms(10);
						break;

				}
				break;
				
			case 1:	//在台上
				na=0;
				nb=0;
				nEdge = Edge();	//检测边缘

				//UP_LCD_ClearScreen();
			
			
			
				UP_LCD_ShowInt(2, 2, nEdge);
				switch(nEdge)
				{
					case 0:	//没有检测到边缘
					nEnemy = Enemy();	//检测敌人
					//UP_LCD_ClearScreen();
				
					UP_LCD_ShowInt(3, 3, nEnemy);
					switch(nEnemy)
						{
							case 0:	//无敌人
								move(400,400);	
								UP_delay_ms(10);
								break;
							case 1:	//前有qizi
								move(800,800);	
								UP_delay_ms(20);
								break;
							case 2:	//右侧有敌人
								move(-400,-400);	
								UP_delay_ms(200);
								move(400,-400);	
								UP_delay_ms(300);
								break;
							case 3:	//后方有敌人
								move(-400,400);	
								UP_delay_ms(800);
								break;
							case 4:	//左侧有敌人
								move(-400,-400);	
								UP_delay_ms(200);
								move(-400,400);	
								UP_delay_ms(300);
								break;
							case 11:	//前方检测到箱子
								move(800,800);	
								UP_delay_ms(20);
								break;
							case 103:	//错误
								move(400,400);	
								UP_delay_ms(10);
								break;
						}

						break;
					case 1:	//左前检测到边缘
						move(-400,-400);	
						UP_delay_ms(400);
						move(400,-400);	
						UP_delay_ms(300);
						break;
					case 2:	//右前检测到边缘
						move(-400,-400);	
						UP_delay_ms(400);
						move(400,-400);	
						UP_delay_ms(400);
						break;
					case 3:	//右后检测到边缘
						move(400,400);	
						UP_delay_ms(500);
						move(400,-400);	
						UP_delay_ms(500);
						break;
					case 4:	//左后检测到边缘
						move(400,400);	
						UP_delay_ms(500);
						move(400,-400);	
						UP_delay_ms(500);
						break;
					case 5:	//前方两个检测到边缘

						move(-500,-500);	
						UP_delay_ms(700);
						move(500,-500);	
						UP_delay_ms(300);
					
						break;
// 					case 5:	//前方两个检测到边缘
// 						nc++;
// 						move(-500,-500);	
// 						UP_delay_ms(700);
// 						move((40*nc),-(40*nc));	
// 						UP_delay_ms(500);
// 						if (nc==15)
// 						{
// 							nc=8;
// 						}
// 						break;
					case 6:	//后方两个检测到边缘
						move(500,500);	
						UP_delay_ms(500);
						break;
					case 7:	//左侧两个检测到边缘
						move(500,-400);	
						UP_delay_ms(500);
						move(400,400);	
						UP_delay_ms(300);
						break;
					case 8:	//右侧两个检测到边缘
						move(-400,500);	
						UP_delay_ms(500);
						move(400,400);	
						UP_delay_ms(300);
						break;
					case 9:	//搁浅前在底下
						nd++;
						if (nd>20)
						{
							nd=0;
							move(0,0);	
							UP_delay_ms(10);
							move(-500,-500);	
							UP_delay_ms(200);
							qding();
							move(-500,-500);	
							UP_delay_ms(800);
							zhong();
							move(-500,-500);	
							UP_delay_ms(500);
							move(500,-500);	
							UP_delay_ms(300);
							move(0,0);	
							UP_delay_ms(100);
						}
						else
						{
							UP_delay_ms(20);
						}
						
						
						break;
					case 10:	//搁浅前在上面
						ne++;
						if (ne>20)
						{
							ne=0;
							move(0,0);	
							UP_delay_ms(10);
							move(500,500);	
							UP_delay_ms(200);
							hding();
							move(500,500);	
							UP_delay_ms(800);
							zhong();
							move(500,500);	
							UP_delay_ms(400);
							move(0,0);	
							UP_delay_ms(100);
						}
						else
						{
							UP_delay_ms(20);
						}
						break;
					case 102:	//错误
						move(500,500);
						UP_delay_ms(10);
						break;

				}
						break;
				case 3://搁浅左侧在擂台右侧在地面
					na++;
					if (na==350)
					{
						move(-500,500);	
						UP_CDS_SetAngle(5, 1000, 800);//左侧支地
						UP_CDS_SetAngle(7, 24, 800);
						//UP_CDS_SetAngle(6, 24, 800);//右侧支地
						//UP_CDS_SetAngle(8, 1000, 800);
						UP_delay_ms(800);
						zhong();
						UP_delay_ms(600);
						na=0;
					}
					else 
					{
						UP_delay_ms(1);
					}
					break;
				case 4://搁浅右侧在擂台左侧在地面
					na++;
					if(na==350)
					{	
						move(500,-500);	
						//UP_CDS_SetAngle(5, 1000, 800);//左侧支地
						//UP_CDS_SetAngle(7, 24, 800);
						UP_CDS_SetAngle(6, 24, 800);//右侧支地
						UP_CDS_SetAngle(8, 1000, 800);
						UP_delay_ms(800);
						zhong();
						UP_delay_ms(600);
						na=0;
					}
					else 
					{
						UP_delay_ms(1);
					}
					break;
		}
}
}
