#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>
#include <string>

#include "TextTable.h"

// min & max temperatures
double min_temperature = 0.1;
double max_temperature = 10'000;

// declared here to avoid passing as function parameters
double a = -4;
double b = 3;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dis(a, b);
TextTable tu, tm;
int experiment_number = 1;

double get_fucn_val(double x) {
    return ((1 - x) * (1 - x) + exp(x));
}

double get_multi_fucn_val(double x) {
    return get_fucn_val(x) * sin(5 * x);
}

bool move_with_possibility(double& delta, double& current_temperature) {
    double m = rand() % 100;
    if (m < 100 * exp(-delta / current_temperature)) {
        return true;
    } else {
        return false;
    }
}

void sa_search_uni(double& min, double current_temperature) {
    if (current_temperature > min_temperature) {
        tu.add(std::to_string(experiment_number++));
        tu.add(std::to_string(current_temperature));
        tu.add(std::to_string(min));
        double min_val = get_fucn_val(min);
        tu.add(std::to_string(min_val));
        tu.endOfRow();
        double cur = dis(gen);
        double cur_val = get_fucn_val(cur);
        double delta = cur_val - min_val;
        if (delta <= 0) {
            min = cur;
        } else {
            if (move_with_possibility(delta, current_temperature)) {
                min = cur;
            }
        }
        auto temp = current_temperature * 0.95;
        sa_search_uni(min, temp);
    }
    return;
}

void sa_search_multi(double& min, double current_temperature) {
    if (current_temperature > min_temperature) {
        tm.add(std::to_string(experiment_number++));
        tm.add(std::to_string(current_temperature));
        tm.add(std::to_string(min));
        double min_val = get_multi_fucn_val(min);
        tm.add(std::to_string(min_val));
        tm.endOfRow();
        double cur = dis(gen);
        double cur_val = get_fucn_val(cur);
        double delta = cur_val - min_val;
        if (delta <= 0) {
            min = cur;
        } else {
            if (move_with_possibility(delta, current_temperature)) {
                min = cur;
            }
        }
        auto temp = current_temperature * 0.95;
        sa_search_multi(min, temp);
    }
    return;
}

int main() {
    tu.add("N"); tm.add("N");
    tu.add("T"); tm.add("T");
    tu.add("x"); tm.add("x");
    tu.add("f(x)"); tm.add("f(x)");
    tu.endOfRow(); tm.endOfRow();
    double min = dis(gen);
    // copy is made to change global value of min_copy this is used in result message
    double min_copy = min;
    sa_search_uni(min_copy, max_temperature);
    std::cout << "unimodal function:\n" << tu <<
    "Result: Xmin = " << min_copy << " Fmin = " << get_fucn_val(min_copy) << "\n\n";
    experiment_number = 1;
    min_copy = min;
    sa_search_multi(min_copy, max_temperature);
    std::cout << "multimodal function:\n" << tm <<
    "Result: Xmin = " << min_copy << " Fmin = " << get_fucn_val(min_copy) << "\n\n";
    return 0;
}
