import asyncio

async def sumsqr_task(my_id, num_tasks, n):
    total = 0
    for i in range(my_id, n+1, num_tasks):
        total += i * i
    return total

async def sumsqr(num_tasks, n):
    tasks = [asyncio.create_task(sumsqr_task(i, num_tasks, n)) for i in range(num_tasks)]
    total = 0
    for t in tasks:
        total += await t
    return total

n = 10000
num_tasks = 6
print(asyncio.run(sumsqr(num_tasks, n)))
print(n * (n+1) * (2*n+1) // 6)
