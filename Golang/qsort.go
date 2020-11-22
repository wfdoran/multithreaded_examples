package main

import "fmt"
import "math/rand"
import "time"

func apply_pivot(pivot_value int, a []int) int {
	n := len(a)
	i := 0
	j := n - 1
	
	
	for {
		for i < n && a[i] <= pivot_value {
			i++
		}
		for j >= 0 && a[j] > pivot_value {
			j--
		}
		
		if i > j {
			return i
		}
		
		a[i], a[j] = a[j], a[i]
	}	
}

func pivot(a []int) int {
	n := len(a)
	if n <= 1 {
		return 0
	}
	
	x := apply_pivot(a[0], a[1:])
	a[0], a[x] = a[x], a[0]
	return x + 1
}

func select_pivot(a []int, m int, k int) {
	n := len(a)
	idx := make([]int, m)
	for i := 0; i < m; i++ {
		idx[i] = (i + 1) * n / (m + 1)
	}
	
	for i := 0; i < m; i++ {
		for j := i + 1; j < m; j++ {
			if a[idx[i]] > a[idx[j]] {
				a[idx[i]], a[idx[j]] = a[idx[j]], a[idx[i]]
			}
		}
	}
	a[0], a[idx[k]] = a[idx[k]], a[0]
}

func pivot_threaded(a []int, num_threads int) int {
	switch num_threads {
	case 2:
		select_pivot(a, 5, 2)
	case 3:
		select_pivot(a, 5, 1)
	default:
		select_pivot(a, 3, 1)
	}

	return pivot(a)
}

func bubble_sort(a []int) {
	n := len(a)
	
	for i := 0; i < n; i++ {
		for j := i + 1; j < n; j++ {
			if a[i] > a[j] {
				a[i], a[j] = a[j], a[i]
			}
		}
	}
}

func qsort(a []int) {
	n := len(a)
	
	if n <= 6 {
		bubble_sort(a)
		return
	}
	
	select_pivot(a, 3, 1)
	x := pivot(a)
	qsort(a[:x-1])
	qsort(a[x:])
}

func split_threads(num_threads, x1, x2 int) (int, int) {
	t1 := num_threads * x1 / (x1 + x2)
	if t1 == 0 {
		t1 = 1
	}
	if t1 == num_threads {
		t1 = num_threads - 1
	}
	t2 := num_threads - t1
	return t1, t2
}

func qsort_threaded(a []int, num_threads int) {
    n := len(a)
	fmt.Println("AAA ", n, num_threads)
	
	if n <= 6 {
		bubble_sort(a)
		return
	}
	
	if num_threads == 1 {
		qsort(a)
		return
	}
	
	x := pivot_threaded(a, num_threads)
	t1, t2 := split_threads(num_threads, x-1, n-x)
	
	done := make(chan struct{})
	
	go func(){
		qsort_threaded(a[:x-1], t1)
		done <- struct{}{}
	}()
	go func() {
		qsort_threaded(a[x:], t2)
		done <- struct{}{}
	}()
	
	<-done
	<-done
}

func is_sorted(a []int) bool {
	n := len(a)
	
	for i := 0; i < n-1; i++ {
		if a[i] > a[i+1] {
			return false
		}
	}
	return true
}

func main() {
	rand.Seed(time.Now().UnixNano())
	n := 1000000
	max := 10000000
	num_threads := 20
	
	a := make([]int, n)
	for i, _ := range a {
		a[i] = rand.Intn(max)
	}
	
	pivot(a)
		
	start := time.Now()	
	qsort_threaded(a, num_threads)
	duration := time.Since(start)
	
	if is_sorted(a) {
		fmt.Println("good", duration)
    } else {
		fmt.Println("bad")
	}
}


