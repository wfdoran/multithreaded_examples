package main

import "fmt"

type node struct {
	n     int
	m     int
	board []int
}

func worker2(request chan (chan (node)), work chan (node), done chan (int)) {
	count := 0
	var stack []node

	for iter := 0; ; iter++ {
		if len(stack) == 0 {
			req := make(chan (node))
			request <- req
			x := <-req

			if x.n < 0 {
				done <- count
				return
			}

			stack = append(stack, x)
			iter = 1
		}

		if iter%100 == 0 && len(stack) > 1 {
			work <- stack[0]
			stack = stack[1:]
		}

		x := stack[len(stack)-1]
		stack = stack[:len(stack)-1]

		if x.n == x.m {
			count++
			continue
		}

		bad := make([]bool, x.n)
		for i, pos := range x.board {
			bad[pos] = true
			delta := x.m - i
			pos_left := pos - delta
			if pos_left >= 0 && pos_left < x.n {
				bad[pos_left] = true
			}
			pos_right := pos + delta
			if pos_right >= 0 && pos_right < x.n {
				bad[pos_right] = true
			}
		}

		for pos, v := range bad {
			if !v {
				new_board := make([]int, len(x.board)+1)
				copy(new_board, x.board)
				new_board[x.m] = pos
				child := node{n: x.n, m: x.m + 1, board: new_board}
				stack = append(stack, child)
			}
		}
	}
}

func worker(request chan (chan (node)), work chan (node), done chan (int)) {
	count := 0

	for {
		req := make(chan (node))
		request <- req
		x := <-req

		if x.n == x.m {
			count++
			continue
		}

		if x.n < 0 {
			done <- count
			return
		}

		bad := make([]bool, x.n)
		for i, pos := range x.board {
			bad[pos] = true
			delta := x.m - i
			pos_left := pos - delta
			if pos_left >= 0 && pos_left < x.n {
				bad[pos_left] = true
			}
			pos_right := pos + delta
			if pos_right >= 0 && pos_right < x.n {
				bad[pos_right] = true
			}
		}

		for pos, v := range bad {
			if !v {
				new_board := make([]int, len(x.board)+1)
				copy(new_board, x.board)
				new_board[x.m] = pos
				child := node{n: x.n, m: x.m + 1, board: new_board}
				work <- child
			}
		}
	}
}

func master(n int, num_workers int, request chan (chan (node)), work chan (node)) {
	var stack []node

	root := node{n: n, m: 0, board: []int{}}
	stack = append(stack, root)

	var waiting []chan (node)

	for {
		if len(stack) == 0 && len(waiting) == num_workers {
			for _, ch := range waiting {
				no_work := node{n: -1}
				ch <- no_work
			}
			return
		}

		select {
		case req := <-request:
			if len(stack) > 0 {
				req <- stack[len(stack)-1]
				stack = stack[:len(stack)-1]
			} else {
				waiting = append(waiting, req)
			}

		case x := <-work:
			if len(waiting) > 0 {
				waiting[0] <- x
				waiting = waiting[1:]
			} else {
				stack = append(stack, x)
			}
		}
	}
}

func main() {
	n := 10
	num_workers := 4

	request := make(chan (chan (node)))
	work := make(chan (node))
	count := make(chan (int))

	go master(n, num_workers, request, work)

	for i := 0; i < num_workers; i++ {
		// go worker(request, work, count)
		go worker2(request, work, count)
	}

	total := 0
	for i := 0; i < num_workers; i++ {
		x := <-count
		fmt.Println(x)
		total += x
	}
	fmt.Println(total)
}
