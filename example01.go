// Golang program to illustrate the usage of
// fmt.Scanf() function

// Including the main package
package main

// Importing fmt
import (
	"fmt"
)

// Calling main
func main() {

	// Declaring some variables
	var name string
	var alphabet_count int
	var float_value float32
	var bool_value bool

	// Calling Scanf() function for
	// scanning and reading the input
	// texts given in standard input
	fmt.Println("name: ")
	fmt.Scanln( &name)
	fmt.Println("alpha : ")
	fmt.Scanln(&alphabet_count)
	fmt.Println("float: ")
	fmt.Scanln(&float_value)
	fmt.Println("bool: ")
	fmt.Scanln(&bool_value)

	// Printing the given texts
	fmt.Printf("%s %d %g %t", name,
		alphabet_count, float_value, bool_value)

}
