use std::fs;

fn pair_equal(part_1 : &str, part_2 : &str) -> bool{
    println!("Comparing {part_1} to {part_2}");
    part_1 == part_2
}

fn valid_product_code(input : &str) -> bool {
    println!("Checking: {}", input);
    let mut dividor = 2;
    let len : usize = input.len();

    while dividor <= len {//compare until you're comparing all values individually
        if len%dividor == 0 {//only compare input chunks if the input can be perfectly broken into
                            //divided chunks
            let step_size = len/dividor;
            let mut it_all_matches = true; //assume everything matches until proved otherwise
            for i in 1..=dividor-1 {
                let parting_index = i*step_size;
                let index_start = parting_index-step_size;
                let index_end = parting_index+step_size;

                it_all_matches = it_all_matches && pair_equal(&input[index_start..parting_index], &input[parting_index..index_end]);//if everything has matched so far and the current comparision also matches

                if !it_all_matches {//minor optimisation. If two chunks are found to not match, we don't need to
                    break;
                }
            }//returns an array with the indexes between the required 
            if it_all_matches {
                println!("----------- Found a pattern -------------");
                return false;//if a matching pattern is found, the product code is not valid
            }//minor optimisation
        }
        dividor += 1;
    }
    true //product code is valid
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
    
    let mut invalid_id_counter : u64 = 0;
    for product_range_str in input_file.split(",") {
        let product_range: Vec<&str> = product_range_str.split('-').collect();
        let start : u64 = product_range[0].parse().expect("error");
        let end : u64 = product_range[1].parse().expect("error");
        
        for product_code in start..=end {
            if !valid_product_code(&product_code.to_string()) {
                invalid_id_counter += product_code;
            } 
        }
    }

    println!("{}", invalid_id_counter);
}
