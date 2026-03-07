use std::fs;

struct CharIndex {
    character : char,
    index_in_bank : usize,
}

fn find_char_index(input : &str, search_char : char, from_index : usize, up_to_index : usize) -> Option<CharIndex>{
    for (i, c) in input.chars().enumerate() {
        if from_index <= i && i < up_to_index {
            if c == search_char {
                // println!("Found {c} at index {i}");
                return Some(CharIndex{character : c, index_in_bank : i});
            }
        }
    }
    None
} 

//the first CharIndex must leave at least 11 spaces.
//the second CharIndex must leave at least 10 spaces.
//
//the nth index must leave -length+n spaces.
//
//The number of spaces is defined by bank.len()-spaces
//
//the nth CharIndex must leave bank.len()-output_length+output.len()+1
//
//
//For example, if bank.len() = 20
//
//the first CharIndex cannot be later than index 9


fn highest_in_bank(bank : &str, output_length : usize) -> u64 {
    println!("{bank}");
    let search_array = ['9', '8', '7', '6', '5', '4', '3', '2', '1'];
    let mut output : Vec<CharIndex> = vec![];
    while output.len() < output_length {
        
        let start_index = match output.last() {
            Some(last_index) => last_index.index_in_bank + 1,
            None => 0,
        };

        let end_index = bank.chars().count() - output_length + output.len() + 1; 


        for i in search_array {
            println!("checking for {i}");
            match find_char_index(&bank, i, start_index, end_index){
                Some(char_index) => {
                    output.push(char_index);
                    break;
                    }
                ,
                None => (),
            }
        }
    }
    
    let mut output_string = "".to_string(); 
    for item in output {
        output_string.push(item.character);
    }
    
    match output_string.parse::<u64>(){
        Ok(int) => {
            println!("{}", int);
            int
        },
        Err(_x) => 0,
    }
}

fn main() {
    let input_file = match fs::read_to_string("input") {
        Ok(s) => s,
        Err(err) => {
            println!("Cannot read file, error: {}", err);
            return;
        }
    };
    
    let input_file = input_file.trim();

    let mut joltage_total : u64 = 0;

    for bank in input_file.lines(){
        joltage_total += highest_in_bank(&bank, 12);
    }
    println!("Joltage Total: {joltage_total}");
}
