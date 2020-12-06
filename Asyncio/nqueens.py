import asyncio

def count(n, board):
    m = len(board)
    if m == n:
        return 1

    good = [True] * n
    
    for i in range(m):
        good[board[i]] = False
        x = board[i] + (m - i)
        if x >= 0 and x < n:
            good[x] = False
        x = board[i] - (m - i)
        if x >= 0 and x < n:
            good[x] = False

    rv = 0
    for i in range(n):
        if good[i]:
            sub_board = board[:]
            sub_board.append(i)
            rv += count(n, sub_board)
    return rv

async def count2(n, board):
    m = len(board)
    if m == n:
        return 1

    good = [True] * n
    
    for i in range(m):
        good[board[i]] = False
        x = board[i] + (m - i)
        if x >= 0 and x < n:
            good[x] = False
        x = board[i] - (m - i)
        if x >= 0 and x < n:
            good[x] = False
    
    tasks = []
    for i in range(n):
        if good[i]:
            sub_board = board[:]
            sub_board.append(i)
            tasks.append(asyncio.create_task(count2(n, sub_board)))

    rv = 0
    for t in tasks:
        rv += await t
    return rv

async def task(n,i):
    return count(n, [i])

async def count3(n):
    tasks = (task(n,i) for i in range(n))
    x = await asyncio.gather(*tasks)
    return sum(x)
            
n = 8
print(count(n, []))
print(asyncio.run(count2(n,[])))
print(asyncio.run(count3(n)))



        
