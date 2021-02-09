package main

import (
    "fmt"
    math_rand "math/rand"
    "encoding/binary"
    crypto_rand "crypto/rand"
    "math"
)

func monte_carlo(n int64, ch chan int64) {
    var b [8]byte
    _, err := crypto_rand.Read(b[:])
    if err != nil {
        panic("crypto_rand.Read failed")
    }
    s := math_rand.NewSource(int64(binary.LittleEndian.Uint64(b[:])))
    r := math_rand.New(s)

    count := int64(0)
    for i := int64(0); i < n; i++ {
    	a := r.Float64()
	b := r.Float64()
	if a * a + b * b <= 1.0 {
	   count++
	}
    }
    ch <- count
}

func main() {
    ch := make(chan int64)
    num_procs := 10
    n := int64(1000000)

    for i := 0; i < num_procs; i++ {
        go monte_carlo(n, ch)
    }

    total := int64(0)
    for i := 0; i < num_procs; i++ {
    	total += <-ch
    }
    samples := float64(n) * float64(num_procs)
    pi := 4.0 * float64(total) / samples
    fmt.Printf("%20.12f\n", pi)
    fmt.Printf("%20.12f\n", math.Pi)
    fmt.Printf("%20.12f\n", 1.0 / math.Sqrt(samples))
}

     