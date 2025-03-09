#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::ifstream input("input.txt");
std::ofstream output("output.txt");

// Structure to represent coordinates
struct Coordinate {
    int row;
    int col;

    Coordinate(int r, int c) : row(r), col(c) {}
};

// Function to get the width and height of the lawn from the path
std::pair<int, int> getWidthAndHeight(const std::string& line) {
    int max_height = 0, min_height = 0;
    int max_width = 0, min_width = 0;
    int crt_height = 0, crt_width = 0;

    for (char direction : line) {
        if (direction == 'W') crt_height++;
        if (direction == 'S') crt_height--;
        if (direction == 'D') crt_width++;
        if (direction == 'A') crt_width--;

        max_height = std::max(max_height, crt_height);
        min_height = std::min(min_height, crt_height);
        max_width = std::max(max_width, crt_width);
        min_width = std::min(min_width, crt_width);
    }

    return {max_width - min_width + 1, max_height - min_height + 1};
}

// Function to validate the path
bool validatePath(int width, int height, const std::vector<std::string>& lawn, const std::string& path, Coordinate initial_coordinate) {
    auto copyCoordinate = [](Coordinate coordinate) -> Coordinate { return {coordinate.row, coordinate.col}; };

    if (path.size() != width * height - 2) return false;

    std::vector<Coordinate> coordinate_list;
    coordinate_list.push_back(initial_coordinate);

    for (char direction : path) {
        Coordinate crt_coordinate = copyCoordinate(coordinate_list.back());
        if (direction == 'W') crt_coordinate.row--;
        if (direction == 'S') crt_coordinate.row++;
        if (direction == 'A') crt_coordinate.col--;
        if (direction == 'D') crt_coordinate.col++;

        int x = crt_coordinate.row;
        int y = crt_coordinate.col;

        if (std::find(coordinate_list.begin(), coordinate_list.end(), crt_coordinate) != coordinate_list.end()) return false;
        if (x < 0 || y < 0 || x >= height || y >= width) return false;
        if (lawn[x][y] == 'X') return false;

        coordinate_list.push_back(crt_coordinate);
    }

    return true;
}

// Function to validate the path with unknown start
bool validatePathUnknownStart(int width, int height, const std::vector<std::string>& lawn, const std::string& path) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (validatePath(width, height, lawn, path, {i, j})) return true;
        }
    }
    return false;
}

// Function to get the coordinate of the tree
Coordinate getTreeCoordinate(int width, int height, const std::vector<std::string>& lawn) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (lawn[i][j] == 'X') return {i, j};
        }
    }
    return {0, 0}; // Default return if not found
}

// Function to rotate clockwise and get the tree
Coordinate rotateClockwiseAndGetTree(int width, int height, const Coordinate& tree_coordinate) {
    return {tree_coordinate.col, height - tree_coordinate.row - 1};
}

// Function to rotate clockwise path
std::string rotateClockwisePath(const std::string& path) {
    std::string rotated_path = path;
    for (char& direction : rotated_path) {
        if (direction == 'W') direction = 'D';
        else if (direction == 'D') direction = 'S';
        else if (direction == 'S') direction = 'A';
        else if (direction == 'A') direction = 'W';
    }
    return rotated_path;
}

// Function to generate path for plan C
std::string planC(int width, int height, const std::vector<std::string>& lawn) {
    return "IMPOSSIBLE";
}

// Function to generate path for plan B
std::string planB(int width, int height, const std::vector<std::string>& lawn) {
    Coordinate tree_coordinate = getTreeCoordinate(width, height, lawn);

    int number_of_right_turns = 0;

    while (tree_coordinate.row % 2 == 1 || tree_coordinate.col % 2 == 1 || (width - tree_coordinate.col) % 2 == 0) {
        tree_coordinate = rotateClockwiseAndGetTree(width, height, tree_coordinate);
        std::swap(width, height);
        number_of_right_turns++;

        if (number_of_right_turns == 4) return planC(width, height, lawn);
    }

    int tree_x = tree_coordinate.row;
    int tree_y = tree_coordinate.col;
    int x = tree_x;
    int y = tree_y + 1;

    std::vector<Coordinate> coordinate_list;
    coordinate_list.push_back({x, y});

    while (y + 1 < width) {
        y++;
        coordinate_list.push_back({x, y});
    }

    if (tree_x % 2 == 0) {
        while (y > tree_y) {
            while (x > 0) {
                x--;
                coordinate_list.push_back({x, y});
            }
            y--;
            coordinate_list.push_back({x, y});
            while (x + 1 < tree_x) {
                x++;
                coordinate_list.push_back({x, y});
            }
            y--;
            coordinate_list.push_back({x, y});
        }

        while (x > 0) {
            x--;
            coordinate_list.push_back({x, y});
        }

        int x_increment = -1;
        while (x - 1 < tree_x) {
            if (x_increment == -1) {
                while (y > 0) {
                    y--;
                    coordinate_list.push_back({x, y});
                }
                x++;
                coordinate_list.push_back({x, y});
            } else {
                while (y + 1 < tree_y) {
                    y++;
                    coordinate_list.push_back({x, y});
                }
                x++;
                coordinate_list.push_back({x, y});
            }
            x_increment *= -1;
        }

        while (x < height) {
            while (y + 1 < width) {
                y++;
                coordinate_list.push_back({x, y});
            }
            x++;
            coordinate_list.push_back({x, y});
            while (y > 0) {
                y--;
                coordinate_list.push_back({x, y});
            }
            x++;
            coordinate_list.push_back({x, y});
        }
    }

    int target_length = width * height - 1;
    while (coordinate_list.size() > target_length) coordinate_list.pop_back();

    std::string path;

    for (int i = 1; i < coordinate_list.size(); ++i) {
        if (coordinate_list[i].row < coordinate_list[i - 1].row) path += 'W';
        if (coordinate_list[i].col > coordinate_list[i - 1].col) path += 'D';
        if (coordinate_list[i].row > coordinate_list[i - 1].row) path += 'S';
        if (coordinate_list[i].col < coordinate_list[i - 1].col) path += 'A';
    }

    while (number_of_right_turns % 4 != 0) {
        number_of_right_turns++;
        path = rotateClockwisePath(path);
    }

    return path;
}

// Function to generate path for plan A
std::string generatePath(int width, int height, const std::vector<std::string>& lawn) {
    Coordinate tree_coordinate = getTreeCoordinate(width, height, lawn);

    int number_of_right_turns = 0;

    while (tree_coordinate.row % 2 == 0 || tree_coordinate.col % 2 == 0) {
        tree_coordinate = rotateClockwiseAndGetTree(width, height, tree_coordinate);
        std::swap(width, height);
        number_of_right_turns++;

        if (number_of_right_turns == 4) return planB(width, height, lawn);
    }

    int tree_x = tree_coordinate.row;
    int tree_y = tree_coordinate.col;
    int x = tree_x + 1;
    int y = tree_y;

    std::vector<Coordinate> coordinate_list;
    coordinate_list.push_back({x, y});

    while (x + 1 < height) {
        x++;
        coordinate_list.push_back({x, y});
    }

    if (tree_y % 2 == 1) {
        y--;
        coordinate_list.push_back({x, y});
        while (x > 1) {
            x--;
            coordinate_list.push_back({x, y});
        }
        while (y - 1 >= 0) {
            y--;
            coordinate_list.push_back({x, y});
            while (x + 1 < height) {
                x++;
                coordinate_list.push_back({x, y});
            }
            y--;
            coordinate_list.push_back({x, y});
            while (x > 1) {
                x--;
                coordinate_list.push_back({x, y});
            }
        }
        x--;
        coordinate_list.push_back({x, y});
        while (y < tree_y) {
            y++;
            coordinate_list.push_back({x, y});
        }

        if (tree_x % 2 == 1) {
            while (y + 1 < width) {
                y++;
                coordinate_list.push_back({x, y});
            }
            x++;
            coordinate_list.push_back({x, y});
            while (x < tree_x) {
                while (y > tree_y) {
                    y--;
                    coordinate_list.push_back({x, y});
                }
                x++;
                coordinate_list.push_back({x, y});
                while (y + 1 < width) {
                    y++;
                    coordinate_list.push_back({x, y});
                }
                x++;
                coordinate_list.push_back({x, y});
            }
            int y_increment = -1;
            while (x < height) {
                if (y_increment == -1) {
                    while (y + y_increment > tree_y) {
                        y += y_increment;
                        coordinate_list.push_back({x, y});
                    }
                    x++;
                    coordinate_list.push_back({x, y});
                } else {
                    while (y + 1 < width) {
                        y++;
                        coordinate_list.push_back({x, y});
                    }
                    x++;
                    coordinate_list.push_back({x, y});
                }
                y_increment *= -1;
            }
        }
    }

    int target_length = width * height - 1;
    while (coordinate_list.size() > target_length) coordinate_list.pop_back();

    std::string path;

    for (int i = 1; i < coordinate_list.size(); ++i) {
        if (coordinate_list[i].row < coordinate_list[i - 1].row) path += 'W';
        if (coordinate_list[i].col > coordinate_list[i - 1].col) path += 'D';
        if (coordinate_list[i].row > coordinate_list[i - 1].row) path += 'S';
        if (coordinate_list[i].col < coordinate_list[i - 1].col) path += 'A';
    }

    while (number_of_right_turns % 4 != 0) {
        number_of_right_turns++;
        path = rotateClockwisePath(path);
    }

    return path;
}

void run() {
    int no_lines;
    input >> no_lines;

    for (int i = 0; i < no_lines; ++i) {
        int w, h;
        input >> w >> h;

        std::vector<std::string> lawn(h);
        for (int j = 0; j < h; ++j) {
            input >> lawn[j];
        }

        std::string path = generatePath(w, h, lawn);
        output << path << std::endl;
    }
}

int main() {
    run();
    input.close();
    output.close();
    return 0;
}
