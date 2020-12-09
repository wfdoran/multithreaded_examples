import asyncio
import random
from heapq import merge

async def sort_part(a, i, j):
	a[i:j] = sorted(a[i:j])
	
async def merge_part(a, i, j, k):
	a[i:k] = merge(a[i:j], a[j:k])

async def asort(a, num_tasks):
	n = len(a)
	X = [(i * n) // num_tasks for i in range(num_tasks + 1)]
	
	sort_tasks = [asyncio.create_task(sort_part(a, X[i], X[i+1])) for i in range(num_tasks)]
	done, pending = await asyncio.wait(sort_tasks)
	
	assert(len(done) == num_tasks)
	assert(len(pending) == 0)
	
	while len(X) > 2:
		merge_tasks = [asyncio.create_task(merge_part(a, X[i], X[i+1], X[i+2])) for i in range(0, len(X)-2, 2)]
		await asyncio.wait(merge_tasks)
		
		X = X[::2]
		if X[-1] != n:
			X.append(n)

n = 100000
a = [random.random() for i in range(n)]
num_tasks = 7

asyncio.run(asort(a, num_tasks))


good = True
for i in range(n-1):
	if a[i] > a[i+1]:
		good = False
		
print("Good" if good else "Bad")



