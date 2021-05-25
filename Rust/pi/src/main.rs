use std::sync::mpsc::{Sender, Receiver};
use std::sync::mpsc;
use std::thread;
use rand::Rng;

fn compute_pi(tx: std::sync::mpsc::Sender<i64>, num_trials: i64)  {
    let mut rng = rand::thread_rng();
    let mut count: i64 = 0;

    for _ in 0..num_trials {
        let x = rng.gen::<f64>();
        let y = rng.gen::<f64>();

        if x * x + y * y < 1.0 {
            count += 1;
        }
    }
    let pi = 4.0 * (count as f64) / (num_trials as f64);
    println!("thread: count = {} {:.5}", count, pi);
    tx.send(count).unwrap();
}

fn main() {
    let (tx, rx) : (Sender<i64>, Receiver<i64>) = mpsc::channel();
    let num_trials: i64 = 1000000;
    let num_tasks: i64 = 5;

    for _ in 0..num_tasks {
        let thread_tx = tx.clone();
        thread::spawn(move || compute_pi(thread_tx, num_trials));
    }

    let mut total: i64 = 0;
    for _ in 0..num_tasks {
        total += rx.recv().unwrap();
    }

    println!();
    let denom = (num_trials * num_tasks) as f64;
    let pi = (4.0 * (total as f64)) / denom;
    println!("estimate = {:.10}", pi);
    println!("actual   = {:.10}", std::f64::consts::PI);
    println!("stddev   = {:.10}", (1.0 / denom).sqrt());
}
