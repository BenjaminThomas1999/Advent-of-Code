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
    
   /* let input = ".......S.......
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
...............";*/

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

fn print_board(board : &Vec<Vec<Value>>){
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

fn count_beam_ends(board : &Vec<Vec<Value>>) -> u32 {
    let mut end_count :u32 = 0;

    for value in board[board.len()-1].iter() {
        if let Value::BeamEnd = value {
            end_count += 1;
        }
    }
    end_count
}

fn main() {
    let mut board = parseInput();
    let mut split_count : u32 = 0;
    print_board(&board);

    for working_line in 1..board.len() {//start on the second line and look at the line above for
                                        //what to do, then repeat for every line underneath
        for cursor in 0..board[working_line].len() {
            match board[working_line-1][cursor] {//go through the values above to decide what to
                                                 //do.
                Value::Start => board[working_line][cursor] = Value::BeamEnd,
                Value::BeamEnd => {
                    match board[working_line][cursor] {
                        Value::Splitter => { //if pos above is beam end and current pos is a splitter
                            board[working_line][cursor-1] = Value::BeamEnd;
                            board[working_line][cursor+1] = Value::BeamEnd;
                            split_count += 1;
                        }
                        Value::Empty | Value::BeamEnd => board[working_line][cursor] = Value::BeamEnd,

                        Value::Start | Value::Beam => panic!("Something horrible happened!"),
                    }
                    board[working_line-1][cursor] = Value::Beam;
                }
                _ => (),
            }
        }
    }
    print_board(&board);

    println!("{}", split_count);

}
