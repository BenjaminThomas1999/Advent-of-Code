input = open("input").read().split("\n")

input = """..@@.@@@@.
@@@.@.@.@@
@@@@@.@.@@
@.@@@@..@.
@@.@@@@.@@
.@@@@@@@.@
.@.@.@.@@@
@.@@@.@@@@
.@@@@@@@@.
@.@.@@@.@."""

neighbours = [(-1,-1),
             (-1, 0),
             (-1, 1),
             (0, -1),
             (0, 1),
             (1, -1),
             (1, 0),
             (1, 1)]

def get_char(x, y):
    if x < 0 or x >= len(input[y]):
        return '.'

    if y < 0 or y >= len(input):
        return '.'

    return input[y][x]

def check_validity(x, y):
    roll_counter = 0
    for neighbour in neighbours:
        if get_char(x + neighbour[0], y + neighbour[1]) == "@":
            roll_counter += 1
    
    if roll_counter >= 4:
        return 0 
    else:
        return 1 

validity_counter = 0
for (row_number, row_content) in enumerate(input):
    for (column_number, column_content) in enumerate(row_content):
        print(get_char(column_number, row_number), end="")
        if column_content == "@":
            validity_counter += check_validity(row_number, column_number)
    print()

print(validity_counter)

print(check_validity(0, 1))
