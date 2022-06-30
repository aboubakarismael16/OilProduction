package main

import (
	"fmt"
	"github.com/360EntSecGroup-Skylar/excelize"
	"log"
)

func main() {

	f, err := excelize.OpenFile("nw.xlsx")
	if err != nil {
		log.Fatal(err)
	}
	//defer f.Close()

	c1, err := f.GetCellValue("Sheet1", "A1")

	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(c1)

	c2, err := f.GetCellValue("Sheet1", "A4")

	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(c2)
	//fmt.Println(f)
}
