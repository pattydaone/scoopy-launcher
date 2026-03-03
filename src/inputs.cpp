#include <iostream>
#include <ostream>
#include <string>

#include <unistd.h>

#include "ascii.hpp"
#include "inputs.hpp"

Inputs::Inputs(Event& event) 
	: event { event } {
	selected_line = 3;
}

void Inputs::get_input() {
}

void Inputs::backspace_pressed() {
}

void Inputs::check_input() {
}
