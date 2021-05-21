use futures::executor::block_on;

async fn hello(task_id: i32, num_tasks: i32) {
    println!("Hello {} of {}", task_id, num_tasks);
}

fn main() {
    let num_tasks = 5;
    let mut futures = vec![];

    for i in 0..num_tasks {
        let f = hello(i, num_tasks);
        futures.push(f);
    }

    for f in futures {
        block_on(f);
    }
}
