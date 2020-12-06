import asyncio

def get_good(n, board):
    m = len(board)
    good = [True] * n
    
    for i in range(m):
        good[board[i]] = False
        x = board[i] + (m - i)
        if x >= 0 and x < n:
            good[x] = False
        x = board[i] - (m - i)
        if x >= 0 and x < n:
            good[x] = False

    return good

def count(n, board):
    if n == len(board):
        return 1
    
    good = get_good(n, board)

    rv = 0
    for i in range(n):
        if good[i]:
            sub_board = board[:]
            sub_board.append(i)
            rv += count(n, sub_board)
    return rv

async def count2(n, board):
    if n == len(board):
        return 1
    
    good = get_good(n, board)
    
    tasks = []
    for i in range(n):
        if good[i]:
            sub_board = board[:]
            sub_board.append(i)
            tasks.append(count2(n, sub_board))

    x = await asyncio.gather(*tasks)
    return sum(x)

async def task(n,i):
    return count(n, [i])

async def count3(n):
    tasks = (task(n,i) for i in range(n))
    x = await asyncio.gather(*tasks)
    return sum(x)
            
n = 10
print(count(n, []))
print(asyncio.run(count2(n,[])))
print(asyncio.run(count3(n)))



        
