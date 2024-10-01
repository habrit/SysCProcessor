#include "ALUtb.cpp"
#include "4bitaddertb.cpp"
#include "registerfileTB.cpp"

// Main function
int sc_main(int argc, char* argv[]) {
    Testbench tb("Testbench"); // Instantiate the testbench
    // tb.start_tests(); // This line is not needed; tests start in the constructor
    sc_start(); // Start the simulation
    return 0;
}