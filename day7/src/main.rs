use std::fs::{self};

#[derive(Debug, Clone, Copy)]
enum Value {
    Empty,
    Splitter,
    Beam,
    BeamEnd,
    Start,
}

fn parseInput () -> Vec<Vec<Value>> {

    let input = fs::read_to_string("input")
        .unwrap_or("!".to_string());
    
    let input = ".......S.......
...............
.......^.......
...............
......^.^......
...............
.....^.^.^.....
...............
....^.^...^....
...............
...^.^...^.^...
...............
..^...^.....^..
...............
.^.^.^.^.^...^.
...............";

    let mut output = vec![];
    for line in input.lines() {
        let mut line_cache = vec![];
        for char in line.chars() {
            match char {
                '.' => line_cache.push(Value::Empty),
                '^' => line_cache.push(Value::Splitter),
                'S' => line_cache.push(Value::Start),
                _ => panic!("Error while reading the file!"),

            }
        }
        output.push(line_cache);
    }
    output
}

fn printBoard(board : &Vec<Vec<Value>>){
    for line in board {
        let mut line_string = "".to_string();
        for value in line {
            line_string.push(match value {
                Value::Empty => '.',
                Value::Beam => '|',
                Value::Start => 'S',
                Value::BeamEnd => '!',
                Value::Splitter => '^',
            });
        }
        println!("{line_string}");
    }
}

fn main() {
    let mut gameLoop = true;
    let board = parseInput();
    while gameLoop {
        printBoard(&board);
        gameLoop = false;
    }
}
