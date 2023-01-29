#include <stdio.h>
#include <stdlib.h>  
#include <math.h>
#define PI 3.1415926535
#define g 9.807
float H=1530;			//H:�Ͳ����   
float Do=0.0889; 		//Do:�͹��ھ�   
float Dt=0.19; 			//Dt:�׹�ֱ��
float O=0.0324;       	//O:�����ݶ�   
float t0=79.2; 		   //t0:�����¶�  
float Ph=12.15;  		//Ph:�ز�ѹ��  
float Pb=11.23;  		//Pb:����ѹ��  
float K1=2.87;       	//K1:����ϵ��  
float Q=29.4;  			//Q:�Ծ���Һ  
float Pwf=4.62; 			//Pwf:�Ծ���ѹ  
float fw=0.13; 			//fw:�����ˮ��  
float Zo=969.24; 		//Zo:ԭ���ܶ�
float Zw=1000;			//Zw:ˮ���ܶ�
float Co=2148.35;  		//Co:ԭ�ͱ���  
float Cw=4298.73; 		//Cw:�ز�ˮ����  
float Hs=207.24; 		//Hs:��Ƴ�û�� 
float Qs=27.7;   		//Qs:�������
float s=3; 				//s:���ͻ����
float n=6; 				//n:���ͻ����
float Ab=0.7;           //Ab:���ͻ���Ч
float a=9.8658,b=3.9188;				//excel�ع��a,b��ֵ 
float wendu(float L);
float niandu(float T);
float liuya();
float pingjunmidu();
float bengya(float Zp);
float bengshen(float Pwfo,float Ps,float Zp);
float bengjing();
float chouyougan(float Lp,float Zp);
float jiaohe();
main(void)
{
    float Pwfo,Ps,Zp,Lp,Dp;                     // ��������µľ�����ѹ���ÿ�����ѹ�����鼳Һƽ���ܶ� ����������ȣ��õ�ֱ�� 
	Pwfo=liuya();
	Zp=pingjunmidu();
    Ps=bengya(Zp);
    Lp=bengshen(Pwfo,Ps,Zp);
    Dp=bengjing();
    chouyougan(Lp,Zp);
	jiaohe();
}
float wendu(float L)
{	  
	float T,q1=0,W,fz;
	fz=1*Zw*fw/(1*Zw*fw+(1-fw)*Zo);
	W=(fz*Cw+(1-fz)*Co)*((Q*fz*Zw+(1-fz)*Q*Zo)/(24*60*60));
	T=(W*O+q1)/K1*(1-exp(-K1/W*L))+t0-O*L;
	return (T);
}
float niandu(float T)
{
    
	float u;								//ԭ�Ͷ���ճ��
	u=pow(10,a)/pow(T,b);					//ճ�ȹ�ʽ
	return(u);
}
float liuya()
{
	float qmax,Pwfo;
	qmax=Q/(1-0.2*Pwf/Ph-0.8*pow(Pwf/Ph,2));
	printf("�������:%f m3/d\n",qmax);
	Pwfo=(-0.2/Ph+pow((pow(0.2/Ph,2)-(4*0.8)/pow(Ph,2)*(Qs/qmax-1)),0.5))/2/0.8*pow(Ph,2);
	printf("��������µľ�����ѹ��%f MPa\n",Pwfo);
	return(Pwfo);
}
float pingjunmidu()
{
      float Zp;
      Zp=Zo*(1-fw)+Zw*fw;
      return(Zp);
}
float bengya(float Zp)
{
    float Ps;
    Ps=Zp*g*Hs/pow(10,6);
    printf("�������ѹ��:%f MPa\n",Ps);
	return(Ps);
}
float bengshen(float Pwfo,float Ps,float Zp)
{
      float Lp;
      Lp=H-(Pwfo-Ps)*pow(10,6)/g/Zp;
      printf("�±����:%f m\n",Lp);
	  return (Lp);
}
//ȷ���þ�
float bengjing()
{
      float Dp;
      Dp=pow(Qs/(360*PI*s*n*Ab),0.5);
      printf("�þ�:%f m\n",Dp);
	  return (Dp);
}
//�����غɼ��㼰���͸�ǿ��У��
float chouyougan(float Lp,float Zp)
{
	int i,j;
	float W1,W10,H0,H1,dH,Have,Flv,L[5],t,u,dWr,dIru,dWrd; //����Һ��Ķ���ճ��
	float Pmax,Pmin,dPmax,dPmin,dFu,dIrd,dFd,dFrl;			//Pmax,Pmin�������С�غɣ�dIru��dIrd�����³���е��������غɵ����� 
																//dFu��dFd�����³���е����Ħ���غɵ�����,dFrl:���͸�ס��Һ��֮���Ħ���������� 
																//Wr1���ϳ���г��͸����ܵĵ��������������غɣ�
																//Wl1���³����Һ����������Գ��͸˵ĸ����������غɣ�
	float Zrod=7850,r=860,l=3200,uf=0.1;				//Zrod:���͸˵��ܶȣ�r:�����뾶,l:���˳���,uf:��������ϵ��
	float fp=PI*0.044*0.044/4;
    float fo=PI*0.0272*0.0272/4;				//fp�����ͱû�����������fo:�����׽����
	float dr[5]={0.016,0.019,0.022,0.025,0.029};//dt:�͹��ھ�,dr:1-5���͹�ֱ��	
	float fr[5]={PI*dr[0]*dr[0]/4,PI*dr[1]*dr[1]/4,PI*dr[2]*dr[2]/4,PI*dr[3]*dr[3]/4,PI*dr[4]*dr[4]/4};		//1-5�����͸˵ĺ�����
	float Vmax=0.942,m;											//Vmax:���͸�ס��������ٶ�
	float Sigmac,Sigmaa,Sigma1=90000000;			//Sigmac:���͸˵�����Ӧ��,Sigmaa:ѭ��Ӧ����Ӧ����ֵ,���͸˲���ѭ��ƣ�ͼ���Ӧ��
				//Ħ���غ�֮һ
	W10=(fp-fr[2])*g*Lp*Zp;				//P297  ��ʽ12-23
	while(1)
	{
		W1=0;
		H1=0;
		H0=H-Lp;	
		Pmax=W10+1717;			                               //1717  from P300 4.2
		Flv=(Zp*fp*fp*fp*s*s*n*n)/(729*uf*uf*fo*fo);          //P301  12-43
		Pmin=-1717-Flv;
		dH=0.01;
		j=0;
		L[j]=0;
		while(1)
		{

			if((H0+dH)>H)
			{
				dH=H-H0;
			}
			H1=H0+dH;
			Have=H0+dH/2;
			t=wendu(Have);
			u=niandu(t);
			dWr=Zrod*g*fr[j]*dH;                           //P297  12-21
			dIru=dWr*s*n*n*(1+r/l)/1790;                        //12-48
			dIrd=dWr*s*n*n*(1-r/l)/1790;                        //12-49   
			dWrd=dWr-Zp*g*fr[j]*dH;                        ////12-22
			m=Do/dr[j];
			dFrl=2*PI*u/1000*dH*(m*m-1)/((m*m+1)*log(m)-(m*m-1))*Vmax;         //12-39  vmax���ڹ�ʽ12-39����
			dFu=0.015*dWr+dFrl/1.3;                                            //0.015������ 4.1
			dFd=0.015*dWr+dFrl;
			dPmax=dWr+dIru+dFu; 										       //����غ�����
			dPmin=dWrd-dIrd-dFd;                                               //��С�غ�����  
			Pmax=Pmax+dPmax;                                                   
			Pmin=Pmin+dPmin; 
			Sigmaa=(Pmax-Pmin)/(2*fr[j]);                                       //12-102
			Sigmac=pow(Sigmaa*Pmax/fr[j],0.5);                                  //12-103
			if(Sigmac<=Sigma1)                                               //���ϳ��͸�ǿ������
			{
				if(H1>=H)
				{
					break;
				}
				else
				{
					H0=H1;														//ȡ���H0=H1
				}
			}
			else                												
			{
				L[j]=H1-(H-Lp)-dH;                                              
				Pmax=Pmax-dPmax;
				Pmin=Pmin-dPmin;
				j=j+1;															//��������㣬ֱ������Ϊ��һ��											
				L[j]=0;
			}
		}
		L[j]=Lp;
		for(i=j;i>0;i--)
		{
			L[i]=L[i]-L[i-1];
		}
		
		W1=0;
		for(i=0;i<=j;i++)
		{
			W1=W1+(fp-fr[i])*Zp*g*L[i];
		}
		if(fabs(W1-W10)<=0.1)					//W1��W10�ľ���ֵ���ڵ���0.1ʱ,����ѭ��
		{
			break;
		}
		else
		{
			W10=(W1+W10)/2;
		}
	}
	for(i=0;i<=j;i++)
	{
		printf("��%d�����͸�ֱ���ǣ�dr[%d]=%f",i+1,i+1,dr[i]);
		printf("��%d�����͸˳����ǣ�L[%d]=%f\n",i+1,i+1,L[i]);
	}
	printf("����غ�Pmax:%fN\n",Pmax);
	printf("��С�غ�Pmin:%fN\n",Pmin);
	
	  return 0;
}
float jiaohe()
{    
	float Pmaxa=100000,Pmax=47444.796875,Pmin=10813.097656,Mmax,M0max,M1max,M2max,Mmaxa=48000,Nmax,e=0.9;
     if(Pmax<100000)
		 printf("��ѡ���ͻ������غ�Ҫ��\n");
	 else
		 printf("��ѡ���ͻ��������غ�Ҫ��,������ѡ��\n");
	 M0max=s*(Pmax-Pmin)/4;
	 M1max=300*s+0.236*s*(Pmax-Pmin);
     M2max=1800*s+0.202*s*(Pmax-Pmin);
	 Mmax=M0max;
	 if(M1max<M2max)M1max=M2max;
	 if(Mmax<M1max)Mmax=M1max;
	 printf("�������Ť��:  %fN.m\n",Mmax);
	 if(Mmax<Mmaxa)
	 printf("��ѡ���ͻ������������Ť��Ҫ��\n");
	 else
     printf("��ѡ���ͻ��������������Ť��Ҫ��,������ѡ��\n");
	 Nmax=Mmax*n/(9549*e);
	 printf("��Ҫ�����������ΪNmax:%fKW\n",Nmax);
	 // system("PAUSE");
	  return 0;
}

