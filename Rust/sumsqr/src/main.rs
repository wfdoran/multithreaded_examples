use futures::executor::block_on;

async fn hello(task_id: i64, num_tasks: i64, n: i64) -> i64 {
    let mut rv: i64 = 0;
    let mut i = task_id;

    while i <= n {
        rv += i * i;
        i += num_tasks;
    }

    return rv;
}

fn main() {
    let n: i64 = 1000;
    let num_tasks: i64 = 5;
    let futures: Vec<_> = (0..num_tasks).map(|i| hello(i,num_tasks,n)).collect();

    let mut total: i64 = 0;
    for f in futures {
        total += block_on(f);
    }

    println!("{} {}", total, (n * (n - 1) * (2 * n - 1)) / 6);
}
