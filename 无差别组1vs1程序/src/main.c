#include"UPLib\\UP_System.h"



int AD1 = 0;		//�ײ�ǰ��������
int AD2 = 0;		//�ײ��Ҳ������
int AD3 = 0;		//�ײ��󷽺�����
int AD4 = 0;		//�ײ���������

int AD5 = 0;		//ǰ�����ഫ����
int AD6 = 0;		//�Һ����ഫ����
int AD7 = 0;		//������ഫ����
int AD8 = 0;		//������ഫ����

int AD9 = 0;		//��ǰ����̨������
int AD10 = 0;		//��ǰ����̨������
int AD11 = 0;		//�Һ����̨������
int AD12 = 0;		//������̨������

int AD15 = 0;		//��Ǵ�����

int nStage = 0;	//�����̨�ϻ�����̨��
int nEdge = 0;	//��Ե
int nEnemy = 0;	//����
int nFence = 0;	//̨��״̬

int Qian = 0;		//ǰ���޵���
int You = 0;		//�����޵���
int Hou = 0;		//�����޵���
int Zuo = 0;		//�����޵���
int Summ = 0;	//ǰ�����Ҽ�����⵽		

int left = 0;
int right = 0;

int na = 0;     //��б��ʱ
int nb = 0;			//�����Ӽ�ʱ
int nc = 8;			//��ת��ʱ
int nd = 0;			//ǰ��ǳ��ʱ
int ne = 8;			//���ǳ��ʱ

int QX = 0;			//��б

void zhong()    //��̨Ĭ�϶���
{
	UP_CDS_SetAngle(5, 100, 800);
	UP_CDS_SetAngle(6, 940, 800);
	UP_CDS_SetAngle(7, 940, 800);
	UP_CDS_SetAngle(8, 80, 800);
}

void qding()  //��ǰצ
{
	UP_CDS_SetAngle(5, 900, 700);
	UP_CDS_SetAngle(6, 140, 700);
	UP_delay_ms(18);
	UP_CDS_SetAngle(5, 900, 700);
	UP_CDS_SetAngle(6, 140, 700);
}

void hding()  //�պ�צ
{
	UP_CDS_SetAngle(7, 10, 800);
	UP_CDS_SetAngle(8, 1010, 800);
	UP_delay_ms(10);
	UP_CDS_SetAngle(7, 1, 800);
	UP_CDS_SetAngle(8, 1010, 800);
}

void chanzi()  //��̨�����״̬
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


qianshangtai()  //ǰ��̨
{		move(0,0);	//ͣ��������ֹǰһ״̬��ת��ı���̨����
		UP_delay_ms(100);
		zhong();//��֧��̧��Ϊ֧��̨��׼��
		UP_delay_ms(400);
		move(500,500);//��׼��̨
		UP_delay_ms(700);
		qding();//ǰצ֧��
		UP_delay_ms(900);
		UP_CDS_SetAngle(5, 100, 512);
		UP_CDS_SetAngle(6, 940, 512);
		UP_delay_ms(500);
		hding();//֧����
		UP_delay_ms(800);
		zhong();
		//chanzi();//�պ�צ������״̬
		UP_delay_ms(1000);
		move(0,0);	//
		UP_delay_ms(500);
}

houshangtai()  //����̨
{
		move(0,0);	//ͣ��������ֹǰһ״̬��ת��ı���̨����
		UP_delay_ms(100);
		zhong();//��֧��̧��Ϊ֧��̨��׼��
		UP_delay_ms(400);
		move(-500,-500);//��׼��̨
		UP_delay_ms(600);
		hding();//ǰצ֧��
		UP_delay_ms(900);
		UP_CDS_SetAngle(7, 940, 512);
		UP_CDS_SetAngle(8, 80, 512);
		UP_delay_ms(500);
		qding();//֧����
		UP_delay_ms(800);
		zhong();
		//chanzi();//�պ�צ������״̬
		UP_delay_ms(1000);
		move(0,0);	//
		UP_delay_ms(500);
}


unsigned char Stage()//����Ƿ���̨��
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
			return 0;  //��̨��
		}
		else
		{
			return 1;   //��̨��
		}
	}
	else if (AD15<=2000)
	{
		return 3;   //������̨����ڵ����Ҳ�����̨
	}
	else 
	{
		return 4;   //������̨�Ҳ��ڵ����������̨
	}
}


#define FD  150  //
#define RD  150	 //
#define BD  150	 //
#define LD  180  //
unsigned char Fence()//��̨�¼�⳯��
{
	AD1 = UP_ADC_GetValue(1); //�ײ�ǰ��������
	AD2 = UP_ADC_GetValue(2); //�ײ��Ҳ������
	AD3 = UP_ADC_GetValue(3); //�ײ��󷽺�����
	AD4 = UP_ADC_GetValue(4); //�ײ���������
	AD5 = UP_ADC_GetValue(5); //ǰ�����ഫ����
	AD6 = UP_ADC_GetValue(6); //�Һ����ഫ����
	AD7 = UP_ADC_GetValue(7); //������ഫ����
	AD8 = UP_ADC_GetValue(8); //������ഫ����
	
/////////////////////////����̨��һ������⵽/////////////////////////
	if((AD3 < 1000)&&(AD2 > 1000)&&(AD4 > 1000)&&(AD5 > FD)&&(AD6 < RD)&&(AD7 < BD)&&(AD8 < LD))//ǰ���ͺ����
		{
			return 1;		//��̨�£��󷽶���̨
		}
	
	if((AD4 < 1000)&&(AD1 > 1000)&&(AD3 > 1000)&&(AD5 < FD)&&(AD6 > RD)&&(AD7 < BD)&&(AD8 < LD))//�Ҳ�������
		{
			return 2;		//��̨�£�������̨
		}		
	
	if((AD1 < 1000)&&(AD2 > 1000)&&(AD4 > 1000)&&(AD5 < FD)&&(AD6 < RD)&&(AD7 > BD)&&(AD8 < LD))//����ǰ����
		{
			return 3;		//��̨�£�ǰ������̨
		}		
	
	if((AD2 < 1000)&&(AD1 > 1000)&&(AD3 > 1000)&&(AD5 < FD)&&(AD6 < RD)&&(AD7 < BD)&&(AD8 > LD))//�����Һ���
		{
			return 4;		//��̨�£��Ҳ����̨
		}	
/////////////////////////��Χ�����������ڲ���⵽/////////////////////////
	
	if((AD2 > 1000)&&(AD3 > 1000)&&(AD5 > FD)&&(AD6 < RD)&&(AD7 < BD)&&(AD8 > LD))
		{
			return 5;		//��̨�£�ǰ���⵽Χ��
		}	
	if((AD3 > 1000)&&(AD4 > 1000)&&(AD5 > FD)&&(AD6 > RD)&&(AD7 < BD)&&(AD8 < LD))
		{
			return 6;		//��̨�£�ǰ�Ҽ�⵽Χ��
		}		

	if((AD1 > 1000)&&(AD4 > 1000)&&(AD5 < FD)&&(AD6 > RD)&&(AD7 > BD)&&(AD8 < LD))
		{
			return 7;		//��̨�£����Ҽ�⵽Χ��
		}	
	if((AD1 > 1000)&&(AD2 > 1000)&&(AD5 < FD)&&(AD6 < RD)&&(AD7 > BD)&&(AD8 > LD))
		{
			return 8;		//��̨�£������⵽Χ��
		}		

/////////////////////////̨���е��ˣ�������Բ���⵽/////////////////////////
	if((AD5 > FD)&&(AD6 < RD)&&(AD7 > BD)&&(AD8 < LD))
		{
			return 9;		//��̨�£�ǰ�������̨�ϵ���
		}	
	
	if((AD5 < FD)&&(AD6 > RD)&&(AD7 < BD)&&(AD8 > LD))
		{
			return 10;	//��̨�£������Ҳ���̨�ϵ���
		}	
		
/////////////////////////�������ϰ�����������⵽/////////////////////////
	if((AD5 > FD)&&(AD6 > RD)&&(AD7 < BD)&&(AD8 > LD))
		{
			return 11;	//��̨�£�ǰ���������Ҳ��⵽Χ��
		}	
	if((AD5 > FD)&&(AD6 > RD)&&(AD7 > BD)&&(AD8 < LD))
		{
			return 12;	//��̨�£�ǰ�����Ҳ�ͺ󷽼�⵽Χ��
		}	
	if((AD5 > FD)&&(AD6 < RD)&&(AD7 > BD)&&(AD8 > LD))
		{
			return 13;	//��̨�£�ǰ�������ͺ󷽼�⵽Χ��
		}	
	if((AD5 < FD)&&(AD6 > RD)&&(AD7 > BD)&&(AD8 > LD))
		{
			return 14;	//��̨�£��Ҳࡢ���ͺ󷽼�⵽Χ��
		}	
		
		
/////////////////////////б����̨�������������⵽/////////////////////////
	if((AD1 < 1000)&&(AD2 < 1000)&&(AD5 < FD)&&(AD6 < RD))
		{
			return 15;	//��̨�£�ǰ�����Ҳ����̨����������û��⵽
		}	
	if((AD1 < 1000)&&(AD4 < 1000)&&(AD5 < FD)&&(AD8 < LD))
		{
			return 16;	//��̨�£�ǰ����������̨����������û��⵽
		}
	if((AD2 < 1000)&&(AD3 < 1000)&&(AD6 < FD)&&(AD7 < RD))
		{
			return 17;	//��̨�£��󷽺��Ҳ����̨����������û��⵽
		}	
	if((AD3 < 1000)&&(AD4 < 1000)&&(AD7 < FD)&&(AD8 < LD))
		{
			return 18;	//��̨�£��󷽺�������̨����������û��⵽
		}
	
	//////////////
	else
		{
			return 101;//����
		}
		
}

unsigned char Edge()  //����Ե
{
	AD9 = UP_ADC_GetValue(9);   //��ǰ�����紫����
	AD10 = UP_ADC_GetValue(10); //��ǰ�����紫����
	AD11 = UP_ADC_GetValue(11); //�Һ�����紫����
	AD12 = UP_ADC_GetValue(12); //�������紫����
	AD5 = UP_ADC_GetValue(5); //��ȡǰ�����ֵ
	AD7 = UP_ADC_GetValue(7); //��ȡ�󷽲��ֵ
	if((AD9 < 1000)&&(AD10 < 1000)&&(AD11 < 1000)&&(AD12 < 1000))
	{
		return 0;		//û�м�⵽��Ե
	}
		else if((AD9 > 1000)&&(AD10 < 1000)&&(AD11 < 1000)&&(AD12 < 1000))
		{
			return 1;		//��ǰ��⵽��Ե
		}
			else if((AD9 < 1000)&&(AD10 > 1000)&&(AD11 < 1000)&&(AD12 < 1000))
		{
			return 2;		//��ǰ��⵽��Ե
		}
		else if((AD9 < 1000)&&(AD10 < 1000)&&(AD11 > 1000)&&(AD12 < 1000))
		{
			return 3;		//�Һ��⵽��Ե
		}
		else if((AD9 < 1000)&&(AD10 < 1000)&&(AD11 < 1000)&&(AD12 > 1000))
		{
			return 4;		//����⵽��Ե
		}
		else if((AD9 > 1000)&&(AD10 > 1000)&&(AD11 < 1000)&&(AD12 < 1000))
		{
			return 5;		//ǰ��������⵽��Ե
		}
			else if((AD9 < 1000)&&(AD10 < 1000)&&(AD11 > 1000)&&(AD12 > 1000))
		{
			return 6;		//��������⵽��Ե
		}
		else if((AD9 > 1000)&&(AD10 < 1000)&&(AD11 < 1000)&&(AD12 > 1000))
		{
			return 7;		//���������⵽��Ե
		}
		else if((AD9 < 1000)&&(AD10 > 1000)&&(AD11 > 1000)&&(AD12 < 1000))
		{
			return 8;		//�Ҳ�������⵽��Ե
		}	
		else if((AD5 > 480)&&(AD9 > 1000)&&(AD10 > 1000)&&(AD11 > 1000)&&(AD12 > 1000))
		{
			return 9;		//��ǳǮ������̨��
		}	
		else if((AD7 > 480)&&(AD9 > 1000)&&(AD10 > 1000)&&(AD11 > 1000)&&(AD12 > 1000))
		{
			return 10;		//��ǳǮ������̨��
		}
		else 
		{
			return 102;		//����
		}
}

unsigned char Enemy()   //������
{
	AD1 = UP_ADC_GetValue(1); // ǰ�����紫����
	AD2 = UP_ADC_GetValue(2); //�Һ����紫����
	AD3 = UP_ADC_GetValue(3); //������紫����
	AD4 = UP_ADC_GetValue(4); //������紫����
	AD5 = UP_ADC_GetValue(5); //ǰ�����ഫ����
	
	if ((AD1 > 100)&&(AD2 > 100)&&(AD3 > 100)&&(AD4 > 100))
	{
		return 0;  //�޵���
	}
	if ((AD1 < 100)&&(AD2 > 100)&&(AD3 > 100)&&(AD4 > 100))
	{
		if (AD5>1000)
		{
			return 11;  //ǰ��������
		}
		else
			{
				return 1;   //ǰ��������
			}
	}
	if ((AD1 > 100)&&(AD2 < 100)&&(AD3 > 100)&&(AD4 > 100))
	{
		return 2;   //�Ҳ��е��˻�����
	}
	if ((AD1 > 100)&&(AD2 > 100)&&(AD3 < 100)&&(AD4 > 100))
	{
		return 3;   //���е��˻�����
	}
	if ((AD1 > 100)&&(AD2 > 100)&&(AD3 > 100)&&(AD4 < 100))
	{
		return 4;   //����е��˻�����
	}
	else
	{
		return 103;//����
	}
}

//������
int main()
{
	/*��ʼ��ϵͳ*/
	UP_System_Init();
	//printf("Demo15 Woody_ImageRecog\n");	//��ʾDemo����
	//UP_LCD_ShowCharacterString(0,1,"    �����Ƽ�");					//��ʾ�����ַ���LOGO
	//UP_LCD_ShowCharacterString(0,2,"׿Խ֮�Ǵ����׼�");			//��ʾ�����ַ���LOGO
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
	
  while(1)
	{
		if((UP_ADC_GetValue(2) < 1000)||(UP_ADC_GetValue(4) < 1000))
		{
			break;
		}
		UP_delay_ms(10);
	}
	
	qianshangtai();
	
while(1)
{	
	nStage = Stage();	//�����̨
	
 	
 	UP_LCD_ShowInt(0, 0, nStage);
		switch(nStage)
		{
		  case 0:	//��̨��
// 				move(500,500);	
// 				UP_delay_ms(200);
				nFence = Fence();	//������
			 	UP_LCD_ClearScreen();
			
			

				UP_LCD_ShowInt(1, 1, nFence);
				switch(nFence)
				{
					case 1:	//��̨�º󷽶���̨
						houshangtai();
						break;
					case 2:	//������̨
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
					case 3:	//ǰ������̨
						qianshangtai();
						break;
					case 4:	//�Ҳ����̨
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
					case 5:	//ǰ���⵽Χ��
						move(-400,-400);	
				  	UP_delay_ms(400);
						break;
					case 6:	//ǰ�Ҽ�⵽Χ��
						move(-400,-400);	
 						UP_delay_ms(400);
 						break;
					case 7:	//���м�⵽Χ��
						move(400,400);	
 						UP_delay_ms(400);
						break;
					case 8:	//�����⵽Χ��
						move(400,400);	
 						UP_delay_ms(400);
						break;
					case 9:	//ǰ�������̨�ϵ���
						move(500,-500);	
						UP_delay_ms(300);
						move(400,400);	
						UP_delay_ms(400);
						break;
					case 10:	//�����Ҳ���̨�ϵ���
						move(400,400);	
						UP_delay_ms(400);
						break;
					case 11:	//ǰ���������Ҳ��⵽Χ��
						move(-400,-300);	
						UP_delay_ms(500);
						move(-400,400);	
						UP_delay_ms(300);
						break;
					case 12:	//ǰ�Һ��⵽Χ��
						move(300,600);	
						UP_delay_ms(400);
						break;
					case 13:	//ǰ����⵽Χ��
						move(600,300);	
						UP_delay_ms(400);
						break;
					case 14:	//������⵽Χ��
						move(-400,400);	
						UP_delay_ms(200);
						move(400,400);	
						UP_delay_ms(300);
						break;
					case 15:	//ǰ�Ҽ�⵽��̨
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
					case 16:	//ǰ���⵽��̨
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
						case 17:	//��̨�£��󷽺��Ҳ����̨����������û��⵽
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
					case 18:	//��̨�£��󷽺�������̨����������û��⵽
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
					case 101:	//����
						UP_delay_ms(10);
						break;

				}
				break;
				
			case 1:	//��̨��
				na=0;
				nb=0;
				nEdge = Edge();	//����Ե

				//UP_LCD_ClearScreen();
			
			
			
				UP_LCD_ShowInt(2, 2, nEdge);
				switch(nEdge)
				{
					case 0:	//û�м�⵽��Ե
					nEnemy = Enemy();	//������
					//UP_LCD_ClearScreen();
				
					UP_LCD_ShowInt(3, 3, nEnemy);
					switch(nEnemy)
						{
							case 0:	//�޵���
								move(400,400);	
								UP_delay_ms(10);
								break;
							case 1:	//ǰ��qizi
								move(800,800);	
								UP_delay_ms(20);
								break;
							case 2:	//�Ҳ��е���
								move(-400,-400);	
								UP_delay_ms(200);
								move(400,-400);	
								UP_delay_ms(300);
								break;
							case 3:	//���е���
								move(-400,400);	
								UP_delay_ms(800);
								break;
							case 4:	//����е���
								move(-400,-400);	
								UP_delay_ms(200);
								move(-400,400);	
								UP_delay_ms(300);
								break;
							case 11:	//ǰ����⵽����
								move(800,800);	
								UP_delay_ms(20);
								break;
							case 103:	//����
								move(400,400);	
								UP_delay_ms(10);
								break;
						}

						break;
					case 1:	//��ǰ��⵽��Ե
						move(-400,-400);	
						UP_delay_ms(400);
						move(400,-400);	
						UP_delay_ms(300);
						break;
					case 2:	//��ǰ��⵽��Ե
						move(-400,-400);	
						UP_delay_ms(400);
						move(400,-400);	
						UP_delay_ms(400);
						break;
					case 3:	//�Һ��⵽��Ե
						move(400,400);	
						UP_delay_ms(500);
						move(400,-400);	
						UP_delay_ms(500);
						break;
					case 4:	//����⵽��Ե
						move(400,400);	
						UP_delay_ms(500);
						move(400,-400);	
						UP_delay_ms(500);
						break;
					case 5:	//ǰ��������⵽��Ե

						move(-500,-500);	
						UP_delay_ms(700);
						move(500,-500);	
						UP_delay_ms(300);
					
						break;
// 					case 5:	//ǰ��������⵽��Ե
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
					case 6:	//��������⵽��Ե
						move(500,500);	
						UP_delay_ms(500);
						break;
					case 7:	//���������⵽��Ե
						move(500,-400);	
						UP_delay_ms(500);
						move(400,400);	
						UP_delay_ms(300);
						break;
					case 8:	//�Ҳ�������⵽��Ե
						move(-400,500);	
						UP_delay_ms(500);
						move(400,400);	
						UP_delay_ms(300);
						break;
					case 9:	//��ǳǰ�ڵ���
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
					case 10:	//��ǳǰ������
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
					case 102:	//����
						move(500,500);
						UP_delay_ms(10);
						break;

				}
						break;
				case 3://��ǳ�������̨�Ҳ��ڵ���
					na++;
					if (na==350)
					{
						move(-500,500);	
						UP_CDS_SetAngle(5, 1000, 800);//���֧��
						UP_CDS_SetAngle(7, 24, 800);
						//UP_CDS_SetAngle(6, 24, 800);//�Ҳ�֧��
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
				case 4://��ǳ�Ҳ�����̨����ڵ���
					na++;
					if(na==350)
					{	
						move(500,-500);	
						//UP_CDS_SetAngle(5, 1000, 800);//���֧��
						//UP_CDS_SetAngle(7, 24, 800);
						UP_CDS_SetAngle(6, 24, 800);//�Ҳ�֧��
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
