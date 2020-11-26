package main

import "fmt"

func main() {
    n := 10000
    num_workers := 4
    results := make(chan int64)

    for i := 0; i < num_workers; i++ {
    	go func(id int) {
	   my_sum := int64(0)

           for idx := id; idx <= n; idx += num_workers {
	       my_sum += int64(idx * idx)
	   }
	   results <- my_sum
        }(i)
    }
    
    total := int64(0)
    for i := 0; i < num_workers; i++ {
        total += <- results
    }
    fmt.Println(total);
    n64 := int64(n)
    fmt.Println(n64 * (n64 + 1) * (2 * n64 + 1) / 6)
}