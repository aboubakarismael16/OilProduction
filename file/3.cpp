#include <stdio.h>
#include <stdlib.h>  
#include <math.h>
#define PI 3.1415926535
#define g 9.807
float H=1530;			//H:油层深度   
float Do=0.0889; 		//Do:油管内径   
float Dt=0.19; 			//Dt:套管直径
float O=0.0324;       	//O:地温梯度   
float t0=79.2; 		   //t0:井底温度  
float Ph=12.15;  		//Ph:地层压力  
float Pb=11.23;  		//Pb:饱和压力  
float K1=2.87;       	//K1:传热系数  
float Q=29.4;  			//Q:试井产液  
float Pwf=4.62; 			//Pwf:试井流压  
float fw=0.13; 			//fw:体积含水率  
float Zo=969.24; 		//Zo:原油密度
float Zw=1000;			//Zw:水的密度
float Co=2148.35;  		//Co:原油比热  
float Cw=4298.73; 		//Cw:地层水比热  
float Hs=207.24; 		//Hs:设计沉没度 
float Qs=27.7;   		//Qs:设计排量
float s=3; 				//s:抽油机冲程
float n=6; 				//n:抽油机冲次
float Ab=0.7;           //Ab:抽油机泵效
float a=9.8658,b=3.9188;				//excel回归得a,b的值 
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
    float Pwfo,Ps,Zp,Lp,Dp;                     // 设计排量下的井底流压，泵口吸入压力，抽汲液平均密度 ，泵下入深度，泵的直径 
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
    
	float u;								//原油动力粘度
	u=pow(10,a)/pow(T,b);					//粘度公式
	return(u);
}
float liuya()
{
	float qmax,Pwfo;
	qmax=Q/(1-0.2*Pwf/Ph-0.8*pow(Pwf/Ph,2));
	printf("最大排量:%f m3/d\n",qmax);
	Pwfo=(-0.2/Ph+pow((pow(0.2/Ph,2)-(4*0.8)/pow(Ph,2)*(Qs/qmax-1)),0.5))/2/0.8*pow(Ph,2);
	printf("设计排量下的井底流压：%f MPa\n",Pwfo);
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
    printf("泵吸入口压力:%f MPa\n",Ps);
	return(Ps);
}
float bengshen(float Pwfo,float Ps,float Zp)
{
      float Lp;
      Lp=H-(Pwfo-Ps)*pow(10,6)/g/Zp;
      printf("下泵深度:%f m\n",Lp);
	  return (Lp);
}
//确定泵径
float bengjing()
{
      float Dp;
      Dp=pow(Qs/(360*PI*s*n*Ab),0.5);
      printf("泵径:%f m\n",Dp);
	  return (Dp);
}
//悬点载荷计算及抽油杆强度校核
float chouyougan(float Lp,float Zp)
{
	int i,j;
	float W1,W10,H0,H1,dH,Have,Flv,L[5],t,u,dWr,dIru,dWrd; //井内液体的动力粘度
	float Pmax,Pmin,dPmax,dPmin,dFu,dIrd,dFd,dFrl;			//Pmax,Pmin：最大最小载荷，dIru，dIrd：上下冲程中的最大惯性载荷的增量 
																//dFu，dFd：上下冲程中的最大摩擦载荷的增量,dFrl:抽油杆住与液柱之间的摩擦力的增量 
																//Wr1：上冲程中抽油杆所受的的重力所产生的载荷，
																//Wl1：下冲程中液柱的重力与对抽油杆的浮力产生的载荷，
	float Zrod=7850,r=860,l=3200,uf=0.1;				//Zrod:抽油杆的密度，r:曲柄半径,l:连杆长度,uf:凡尔流动系数
	float fp=PI*0.044*0.044/4;
    float fo=PI*0.0272*0.0272/4;				//fp：抽油泵活塞横截面积，fo:凡尔孔截面积
	float dr[5]={0.016,0.019,0.022,0.025,0.029};//dt:油管内径,dr:1-5级油管直径	
	float fr[5]={PI*dr[0]*dr[0]/4,PI*dr[1]*dr[1]/4,PI*dr[2]*dr[2]/4,PI*dr[3]*dr[3]/4,PI*dr[4]*dr[4]/4};		//1-5级抽油杆的横截面积
	float Vmax=0.942,m;											//Vmax:抽油杆住最大下行速度
	float Sigmac,Sigmaa,Sigma1=90000000;			//Sigmac:抽油杆的折算应力,Sigmaa:循环应力的应力幅值,抽油杆材质循环疲劳极限应力
				//摩擦载荷之一
	W10=(fp-fr[2])*g*Lp*Zp;				//P297  公式12-23
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
			dFrl=2*PI*u/1000*dH*(m*m-1)/((m*m+1)*log(m)-(m*m-1))*Vmax;         //12-39  vmax求法在公式12-39下面
			dFu=0.015*dWr+dFrl/1.3;                                            //0.015可能是 4.1
			dFd=0.015*dWr+dFrl;
			dPmax=dWr+dIru+dFu; 										       //最大载荷增量
			dPmin=dWrd-dIrd-dFd;                                               //最小载荷增量  
			Pmax=Pmax+dPmax;                                                   
			Pmin=Pmin+dPmin; 
			Sigmaa=(Pmax-Pmin)/(2*fr[j]);                                       //12-102
			Sigmac=pow(Sigmaa*Pmax/fr[j],0.5);                                  //12-103
			if(Sigmac<=Sigma1)                                               //符合抽油杆强度条件
			{
				if(H1>=H)
				{
					break;
				}
				else
				{
					H0=H1;														//取起点H0=H1
				}
			}
			else                												
			{
				L[j]=H1-(H-Lp)-dH;                                              
				Pmax=Pmax-dPmax;
				Pmin=Pmin-dPmin;
				j=j+1;															//如果不满足，直径增大为下一个											
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
		if(fabs(W1-W10)<=0.1)					//W1与W10的绝对值大于等于0.1时,继续循环
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
		printf("第%d级抽油杆直径是：dr[%d]=%f",i+1,i+1,dr[i]);
		printf("第%d级抽油杆长度是：L[%d]=%f\n",i+1,i+1,L[i]);
	}
	printf("最大载荷Pmax:%fN\n",Pmax);
	printf("最小载荷Pmin:%fN\n",Pmin);
	
	  return 0;
}
float jiaohe()
{    
	float Pmaxa=100000,Pmax=47444.796875,Pmin=10813.097656,Mmax,M0max,M1max,M2max,Mmaxa=48000,Nmax,e=0.9;
     if(Pmax<100000)
		 printf("所选抽油机满足载荷要求\n");
	 else
		 printf("所选抽油机不满足载荷要求,需重新选择\n");
	 M0max=s*(Pmax-Pmin)/4;
	 M1max=300*s+0.236*s*(Pmax-Pmin);
     M2max=1800*s+0.202*s*(Pmax-Pmin);
	 Mmax=M0max;
	 if(M1max<M2max)M1max=M2max;
	 if(Mmax<M1max)Mmax=M1max;
	 printf("曲柄最大扭矩:  %fN.m\n",Mmax);
	 if(Mmax<Mmaxa)
	 printf("所选抽油机曲柄满足最大扭矩要求\n");
	 else
     printf("所选抽油机曲柄不满足最大扭矩要求,需重新选择\n");
	 Nmax=Mmax*n/(9549*e);
	 printf("需要的最大电机功率为Nmax:%fKW\n",Nmax);
	 // system("PAUSE");
	  return 0;
}

