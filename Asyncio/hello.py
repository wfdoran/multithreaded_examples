import asyncio

async def say_hello(i, n):
    print("Hello from %d of %d" % (i,n))

async def main(n):
    tasks = [asyncio.create_task(say_hello(i, n)) for i in range(n)]
    for t in tasks:
        await t

asyncio.run(main(5))
