package main

import (
	"fmt"
	"math"
)

//地温的计算公式
func (this myValue) t() float32 {
	return this.t0 - (this.m * this.L)
}

//地温的计算公式的参数
type myValue struct {
	//井底原油温度
	t0 float32
	//地层温度梯度
	m float32
	//井深度
	L float32
}

//井筒能量方程的参数
type myValue2 struct {
	//总传热系统
	k1 float32
	t myValue
	//内热源
	q1 float32
	//
	W float32
}

type diWen interface {
	t() float32
}

//井筒能量方程
func(this myValue2) omega() float32 {
	p1 := (this.W * this.t.m + this.q1) / this.k1
	n := -(this.k1 * this.t.L / this.W)
	p2 := 1 - math.Exp(float64(n))
	p3 := this.t.t0 - (this.t.m * this.t.L)
	return (p1 * float32(p2)) + p3
}

//水当量的参数
type myValue3 struct {
	//地层油的质量流量
	Mo float32
	//地层油的比热
	Co float32
	//地层水的质量流量
	Mw float32
	//地层水的比热
	Cw float32

}

//水当量
func (this myValue3) waterValue() float32 {
	return (this.Mo * this.Co) + (this.Mw * this.Cw)
}

func main() {
	//var t diWen
	var (
		t0 float32
		m float32
		L float32
		W float32
		k1 float32
		q1 float32
	)
	//fmt.Scanln(&t0)
	//fmt.Scanln(&m)
	//fmt.Scanln(&L)
	//t = myValue{t0,m,L}
	//fmt.Println("the value of grad temp: ", t.t())

	fmt.Print("W = ")
	fmt.Scanln(&W)
	fmt.Print("m = ")
	fmt.Scanln(&m)
	fmt.Print("q1 = ")
	fmt.Scanln(&q1)
	fmt.Print("k1 = ")
	fmt.Scanln(&k1)
	fmt.Print("t0 = ")
	fmt.Scanln(&t0)
	fmt.Print("L = ")
	fmt.Scanln(&L)

	O := myValue2{
		k1,
		myValue{
			t0,
			m,
			L,
		},
		q1,
		W,
	}
	fmt.Println("Omega: ", O.omega())

	var (
		Mo float32
		Co float32
		Mw float32
		Cw float32
	)
	fmt.Print("Mo = ")
	fmt.Scanln(&Mo)
	fmt.Print("Co = ")
	fmt.Scanln(&Co)
	fmt.Print("Mw = ")
	fmt.Scanln(&Mw)
	fmt.Print("Cw = ")
	fmt.Scanln(&Cw)

	WQ := myValue3{Mo,Co,Mw,Cw}
	fmt.Println("W = ", WQ.waterValue())
}
