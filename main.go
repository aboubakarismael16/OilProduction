package main

import (
	"fmt"
	"math"
)

const (
	H   = 1520    //H油层深度
	Do  = 0.0889  //Do油管内径
	Dt  = 0.19    //Dt套管直径
	O   = 0.0313  //O地温梯度
	t0  = 78.8    //t0井底温度
	Pr  = 12      //Pr地层压力
	Pb  = 10.58   //Pb饱和压力
	K1  = 2.73    //K1传热系数
	Q   = 29.6    //Q试井产液
	Pwf = 4.58    //Pwf试井流压
	fw  = 0.12    //fw体积含水率
	Zo  = 908.74  //Zo原油密度
	Zw  = 1000    //Zw水的密度
	Co  = 2299.21 //Co原油比热
	Cw  = 4452.56 //Cw地层水比热
	Hs  = 222.49  //Hs设计沉没度
	Qs  = 27.8    //Qs设计排量
	//93. Q16-6-105B
	s   = 4       //s抽油机冲程
	n   = 4       //n抽油机冲次
	Ab  = 0.7     //Ab抽油机泵效
	a   = 9.9719  //excel回归得a的值
	b   = 3.8801  //excel回归得b的值
	g   = 9.81
)

func wenDu(L float64) float64 {
	var (
		T  float64
		q1 float64
		W  float64
		fz float64
	)
	q1 = 0 //内热源 q1 = 0
	fz = 1 * Zw * fw / (1*Zw*fw + (1-fw)*Zo)
	W = (fz*Cw + (1-fz)*Co) * ((Q*fz*Zw + (1-fz)*Q*Zo) / (24 * 60 * 60))
	T = (W*O+q1)/K1*(1-math.Exp(-K1/W*L)) + (t0 - O*L)
	//fmt.Println("T =", T)
	return T
}

func nianDu(T float64) float64 {
	var u float64
	u = math.Pow(10, a) / math.Pow(T, b)
	//fmt.Println("u = ", u)
	return u
}

func liuYa() float64 {
	var (
		qmax float64
		Pwfo float64
	)
	qmax = Q / (1 - 0.2*Pwf/Pr - 0.8*math.Pow(Pwf/Pr, 2))
	fmt.Printf("最大排量:%f m3/d\n", qmax)
	Pwfo = (-0.2/Pr + math.Pow(math.Pow(0.2/Pr, 2)-(4*0.8)/math.Pow(Pr, 2)*(Qs/qmax-1), 0.5)) / 2 / 0.8 * math.Pow(Pr, 2)
	fmt.Printf("设计排量下的井底流压：%f MPa\n", Pwfo)
	return Pwfo
}

func pingJunMiDu() float64 {
	var Zp float64
	Zp = Zo*(1-fw) + Zw*fw
	fmt.Println("Zp = ",Zp)
	return Zp
}

func bengYa(Zp float64) float64 {
	var Ps float64
	Ps = Zp * g * Hs / math.Pow(10, 6)
	fmt.Printf("泵吸入口压力:%f MPa\n", Ps)
	return Ps
}

func bengShen(Pwfo float64, Ps float64, Zp float64) float64 {
	var Lp float64
	Lp = H - (Pwfo-Ps)*math.Pow(10, 6)/g/Zp
	fmt.Printf("下泵深度:%f m\n", Lp)
	return Lp
}

func bengJing() float64 {
	var Dp float64
	// 1440/4 = 360
	Dp = math.Pow(Qs/(360*math.Pi*s*n*Ab), 0.5)
	fmt.Printf("泵经:%f m\n", Dp)
	return Dp
}

//悬点载荷计算及抽油杆强度校核
func chouYouGan(Lp float64, Zp float64) float64 {
	var i, j int
	//井内液体的动力粘度
	var W1, W10, H0, H1, dH, Have, Flv, t, u, dWr, dIru, dWrd float64
	L := make([]float64, 5)
	//Pmax,Pmin 最大最小载荷
	//dIru，dIrd上下冲程中的最大惯性载荷的增量
	//dFu，dFd上下冲程中的最大摩擦载荷的增量,
	//dFrl抽油杆住与液柱之间的摩擦力的增量
	//Wr1上冲程中抽油杆所受的的重力所产生的载荷，
	//Wl1下冲程中液柱的重力与对抽油杆的浮力产生的载荷，
	var Pmax, Pmin, dPmax, dPmin, dFu, dIrd, dFd, dFrl float64
	Zrod := float64(7850)               //Zrod抽油杆的密度
	r := float64(860)                   //r曲柄半径
	l := float64(3200)                  //l连杆长度
	uf := 0.1                           //uf凡尔流动系数
	fp := math.Pi * 0.044 * 0.044 / 4   //fp抽油泵活塞横截面积
	fo := math.Pi * 0.0272 * 0.0272 / 4 //fo凡尔孔截面积
	//dt油管内径,dr 1-5级油管直径}
	dr := []float64{0.016, 0.019, 0.022, 0.025, 0.029}
	//1-5级抽油杆的横截面积
	fr := []float64{math.Pi * dr[0] * dr[0] / 4, math.Pi * dr[1] * dr[1] / 4, math.Pi * dr[2] * dr[2] / 4, math.Pi * dr[3] * dr[3] / 4, math.Pi * dr[4] * dr[4] / 4}
	//Vmax抽油杆住最大下行速度 (Pi*S*n)/60
	Vmax := 0.942
	var m float64
	//Sigmac抽油杆的折算应力,Sigmaa 循环应力的应力幅值,抽油杆材质循环疲劳极限应力
	var Sigmac, Sigmaa float64
	Sigma1 := float64(90000000)

	//摩擦载荷之一
	W10 = (fp - fr[2]) * g * Lp * Zp //P297  公式12-23

	for {
		W1 = 0
		H1 = 0
		H0 = H - Lp
		Pmax = W10 + 1717                                                     //1717  from P300 4.2
		Flv = (Zp * fp * fp * fp * s * s * n * n) / (729 * uf * uf * fo * fo) //P301  12-43
		Pmin = -1717 - Flv
		dH = 0.01
		j = 0
		L[j] = 0
		for {
			if (H0 + dH) > H {
				dH = H - H0
			}
			H1 = H0 + dH
			Have = H0 + dH/2
			t = wenDu(Have)
			u = nianDu(t)
			//P297  12-21
			dWr = float64(Zrod) * g * fr[j] * dH
			//12-48
			dIru = dWr * s * n * n * (1 + r/l) / 1790
			//12-49
			dIrd = dWr * s * n * n * (1 - r/l) / 1790
			//12-22
			dWrd = dWr - Zp*g*fr[j]*dH
			m = Do / dr[j]
			//12-39  vmax求法在公式12-39下面
			dFrl = 2 * math.Pi * u / 1000 * dH * (m*m - 1) / ((m*m+1)*math.Log(m) - (m*m - 1)) * Vmax
			//0.015可能是 4.1
			dFu = 0.015*dWr + dFrl/1.3
			dFd = 0.015*dWr + dFrl
			dPmax = dWr + dIru + dFu  //最大载荷增量
			dPmin = dWrd - dIrd - dFd //最小载荷增量
			Pmax = Pmax + dPmax
			Pmin = Pmin + dPmin
			Sigmaa = (Pmax - Pmin) / (2 * fr[j])      //12-102
			Sigmac = math.Pow(Sigmaa*Pmax/fr[j], 0.5) //12-103
			if Sigmac <= Sigma1 {                     //符合抽油杆强度条件
				if H1 >= H {
					break
				} else {
					H0 = H1 //取起点H0=H1
				}
			} else {
				L[j] = H1 - (H - Lp) - dH
				Pmax = Pmax - dPmax
				Pmin = Pmin - dPmin
				j++        //如果不满足，直径增大为下一个
				L[j] = 0
			}
		}
		L[j] = Lp
		for i = j; i > 0; i-- {
			L[i] = L[i] - L[i-1]
		}

		W1 = 0
		for i = 0; i <= j; i++ {
			W1 = W1 + (fp-fr[i])*Zp*g*L[i]
		}
		if math.Abs(W1-W10) <= 0.1 { //W1与W10的绝对值大于等于0.1时,继续循环
			break
		} else {
			W10 = (W1 + W10) / 2
		}
	}
	for i = 0; i <= j; i++ {
		fmt.Printf("第%d级抽油杆直径是：dr[%d]=%.3f m ,", i+1, i+1, dr[i])
		fmt.Printf("第%d级抽油杆长度是：L[%d]=%.3f m \n ", i+1, i+1, L[i])
	}

	fmt.Printf("最大载荷Pmax:%fN\n", Pmax)
	fmt.Printf("最小载荷Pmin:%fN\n", Pmin)

	return 0
}

func jiaoHe() float64 {
	Pmaxa := 160000.0
	Pmax := 68402.60
	Pmin := 2116.55
	var Mmax, M0max, M1max, M2max float64
	Mmaxa := 105000.0
	var Nmax float64
	e := 0.9

	if Pmax < Pmaxa {
		fmt.Printf("所选抽油机满足载荷要求\n")
	} else {
		fmt.Printf("所选抽油机不满足载荷要求,需重新选择\n")
	}
	M0max = s * (Pmax - Pmin) / 4
	M1max = 300*s + 0.236*s*(Pmax-Pmin)
	M2max = 1800*s + 0.202*s*(Pmax-Pmin)
	Mmax = M0max

	if M1max < M2max {
		M1max = M2max
	}
	if Mmax < M1max {
		Mmax = M1max
	}
	fmt.Printf("曲柄最大扭矩:  %fN.m\n", Mmax)

	if Mmax < Mmaxa {
		fmt.Printf("所选抽油机曲柄满足最大扭矩要求\n")
	} else {
		fmt.Printf("所选抽油机曲柄不满足最大扭矩要求,需重新选择\n")
	}

	Nmax = Mmax * n / (9549 * e)
	fmt.Printf("需要的最大电机功率为Nmax:%fKW\n", Nmax)

	return 0
}

func main() {
	//wenDu(1520)
	//nianDu(40)
	//liuYa()
	//pingJunMiDu()
	//bengYa(919.6912)
	bengShen(5.26,2.00,919.69)
	//bengJing()
	chouYouGan(1368.00,919.69)
	jiaoHe()
}
