use std::fs::File;
use std::io::prelude::*;
use std::path::Path;

fn wrap(input : i32, lower_bound : i32, upper_bound : i32) -> i32{
    let range = upper_bound - lower_bound + 1;
    if input < lower_bound {
        wrap(input+range, lower_bound, upper_bound)
    }
    else if input > upper_bound {
        wrap(input-range, lower_bound, upper_bound)
    }
    else {
        input
    }
}

fn main() {
    // Create a path to the desired file
    let path = Path::new("input");
    let display = path.display();
    // Open the path in read-only mode, returns `io::Result<File>`
    let mut file = match File::open(&path) {
        Err(why) => panic!("couldn't open {}: {}", display, why),
        Ok(file) => file,
    };

    // Read the file contents into a string, returns `io::Result<usize>`
    let mut s = String::new();
    match file.read_to_string(&mut s) {
        Err(why) => panic!("couldn't read {}: {}", display, why),
        Ok(_) => print!("{} contains:\n{}", display, s),
    };

    let instructions : Vec<&str> = s.lines().collect();
    
    let mut position = 50;
    let mut zero_count = 0;
    for instruction in instructions.iter(){
        let direction : i32 = match &instruction[..1]{
            "L" => -1,
            "R" => 1,
            _ => panic!("Unable to find direction"),
        };
        
        let distance : i32 = match instruction[1..].parse::<i32>() {
            Ok(num) => num,
            Err(e) => panic!("{}", e),
        };
        
        position = wrap(position + direction*distance, 0, 99);
        if position == 0 {zero_count += 1};
    }

    println!("{}", zero_count);

    // `file` goes out of scope, and the "hello.txt" file gets closed
}
