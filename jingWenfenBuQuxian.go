package main

import (
	"fmt"
	"github.com/360EntSecGroup-Skylar/excelize"
)

func main() {
	categories := map[string]string{
		"A1": "地温", "B1": "井温"}
	values := map[string]float64{
		"A2": 33.62, "A3": 43.01, "A4": 52.40, "A5": 61.79, "A6": 64.92, "A7": 71.18, "A8": 74.31, "A9": 77.44, "A10": 81.20,
		"B2": 42.88, "B3": 52.17, "B4": 61.29, "B5": 69.95, "B6": 72.61, "B7": 77.32, "B8": 79.18, "B9": 80.53, "B10": 81.2,
		"C2": 0, "C3": 300, "C4": 600, "C5": 900, "C6": 1000, "C7": 1200, "C8": 1300, "C9": 1400, "C10": 1520,
		"D2": 0, "D3": 300, "D4": 600, "D5": 900, "D6": 1000, "D7": 1200, "D8": 1300, "D9": 1400, "D10": 1520}
	f1 := excelize.NewFile()
	for k, v := range categories {
		f1.SetCellValue("Sheet1", k, v)
	}
	for k, v := range values {
		f1.SetCellValue("Sheet1", k, v)
	}
	if err := f1.AddChart("Sheet1", "E1", `{
        "type": "line",
        "series": [
        {
            "name": "Sheet1!$A$1",
			"categories": "Sheet1!$A$2:$A$10"
        },
        {
            "name": "Sheet1!$C$1",
			"values": "Sheet1!$C$2:$C$10",
			"categories": "Sheet1!$C$1"
        }],
		"series": [
        {
            "name": "Sheet1!$B$1",
			"categories": "Sheet1!$B$2:$B$10"
        },
        {
            "name": "Sheet1!$D$1",
			"values": "Sheet1!$D$2:$D$10",
			"categories": "Sheet1!$B$1"
        }],
        "format":
        {
            "x_scale": 1.0,
            "y_scale": 1.0,
            "x_offset": 15,
            "y_offset": 10,
            "print_obj": true,
            "lock_aspect_ratio": false,
            "locked": false
        },
        "legend":
        {
            "position": "top",
            "show_legend_key": false
        },
        "title":
        {
            "name": "井温分布曲线"
        },
        "plotarea":
        {
            "show_bubble_size": true,
            "show_cat_name": false,
            "show_leader_lines": false,
            "show_percent": false,
            "show_series_name": true,
            "show_val": false
        },
        "show_blanks_as": "zero",
     	"y_axis":
        {
			"reverse_order": false
        }
    }`); err != nil {
		fmt.Println(err)
	}
	// Save workbook
	if err := f1.SaveAs("井温分布曲线.xlsx"); err != nil {
		fmt.Println(err)
	}
}
