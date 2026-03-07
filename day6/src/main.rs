use std::fs::{self};

#[derive(Clone, Copy, Debug)]
enum Operator {
    Multiply,
    Add,
}

#[derive(Debug)]
struct Expression {
    terms: Vec<u64>,
    operator: Operator,
}

fn read_file(file_path: &str) -> Option<Vec<Expression>> {//I need to change this to a result type
    match fs::read_to_string(file_path) {
        Ok(raw_input) => Some(make_expressions(&raw_input.lines().collect())),
        Err(_e) => None,
    }
}

fn make_expressions(lines : &Vec<&str>) -> Vec<Expression> {//I should change this to a result type
                                                            //instead of panicking 
    let operator_line = lines[lines.len()-1];
    
    let mut output : Vec<Expression> = vec![];
    let mut number_cache : Vec<u64> = vec![];

    //work from right to left in the file in columns.
    let mut column_cursor : i32 = (operator_line.len() -1) as i32;
    while column_cursor >= 0 {
        let mut num : String = "".to_string();
        for line_number in 0..lines.len()-1 {
            num.push(lines[line_number].chars().nth(column_cursor as usize).unwrap());
        }
        let num = num.trim().parse::<u64>().unwrap(); 
        number_cache.push(num);

        match operator_line.chars().nth(column_cursor as usize).unwrap() {
            '*' => {
                output.push(Expression {
                    terms : number_cache.clone(),
                    operator : Operator::Multiply
                });
                column_cursor -= 2;
                number_cache.clear();
            },
            '+' => {
                output.push(Expression {
                    terms : number_cache.clone(),
                    operator : Operator::Add
                });
                column_cursor -= 2;
                number_cache.clear();
            },
            ' ' => {
                column_cursor -= 1;
            },
            _ => panic!("Unknown character found on operator line at column: {column_cursor}"),
        }
    }
    output
}

fn evaluate_expression(expression: Expression) -> u64 {
    match expression.operator {
        Operator::Multiply => {
            let mut result = 1;
            for term in expression.terms {
                result *= term;
            }
            result
        }
        Operator::Add => {
            let mut result = 0;
            for term in expression.terms {
                result += term;
            }
            result
        }
    }
}

fn main() {
    let expressions: Vec<Expression> = match read_file("input") {//this should be a result type
        Some(file_input) => file_input,
        None => panic!("Could not read and process file!"),
    };
    
    let mut sum_of_expressions = 0;
    for expression in expressions {
        sum_of_expressions += evaluate_expression(expression);
    }

    println!("{sum_of_expressions}");
}
