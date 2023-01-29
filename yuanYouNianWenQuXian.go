package main

import (
	"fmt"
	"github.com/360EntSecGroup-Skylar/excelize"
)

func main() {
	categories := map[string]string{
		"A1": "温度℃",  "B1": "粘度mPa•s"}
	values := map[string]float64{
		"A2": 40.00, "A3": 45.00,"A4": 50.00,"A5": 55.00,"A6": 60.00,"A7": 65.00,"A8": 70.00,"A9": 75.00,"A10": 80.00,"A11":85.00,
		"B2": 5699, "B3": 3608,"B4": 2398,"B5": 1656,"B6": 1182,"B7": 866,"B8": 650,"B9": 497,"B10": 387,"B11": 306 }
	f := excelize.NewFile()
	for k, v := range categories {
		f.SetCellValue("Sheet1", k, v)
	}
	for k, v := range values {
		f.SetCellValue("Sheet1", k, v)
	}
	if err := f.AddChart("Sheet1", "E1", `{
        "type": "line",
        "series": [
        {
            "name": "Sheet1!$A$1",
			"categories": "Sheet1!$A$2:$A$11"
        },
        {
            "name": "Sheet1!$B$1",
			"categories": "Sheet1!$B$1",
            "values": "Sheet1!$B$2:$B$11"
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
            "position": "left",
            "show_legend_key": false
        },
        "title":
        {
            "name": "原油粘温曲线"
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
		"x_axis":
        {
			"maximum": 90,
            "minimum": 30
        },
        "y_axis":
        {
            "maximum": 6000,
            "minimum": 0
        }
    }`); err != nil {
		fmt.Println(err)
	}
	// Save workbook
	if err := f.SaveAs("原油粘温曲线.xlsx"); err != nil {
		fmt.Println(err)
	}
}
