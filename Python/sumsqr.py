import multiprocessing as mp

DONE = None

def generate_em(n, num_workers, out_q):
    for i in range(1, n+1):
        out_q.put(i)

    for _ in range(num_workers):
        out_q.put(DONE)

def square_em(in_q, out_q):
    while True:
        a = in_q.get()
        if a is None:
            out_q.put(DONE)
            break
        out_q.put(a * a)

def sum_em(num_workers, in_q, out_q):
    total = 0
    num_done = 0

    while num_done < num_workers:
        a = in_q.get()
        if a is None:
            num_done += 1
        else:
            total += a
    out_q.put(total)


if __name__ == '__main__':
    q1 = mp.Queue()
    q2 = mp.Queue()
    q3 = mp.Queue()
    n = 10000
    num_workers = 4
    
    p1 = mp.Process(target=generate_em, args = (n, num_workers, q1))
    p1.start()

    p2 = [None] * num_workers
    for i in range(num_workers):
        p2[i] = mp.Process(target=square_em, args = (q1, q2))
        p2[i].start()

    p3 = mp.Process(target=sum_em, args = (num_workers, q2, q3))
    p3.start()
    p3.join()

    print(q3.get())
    print(n * (n + 1) * (2 * n + 1) // 6)
