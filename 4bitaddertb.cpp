#include "systemc.h"
#include "4bitadder.cpp"

void test_four_bit_adder() {
    sc_signal<sc_uint<4>> A, B, S;
    sc_signal<bool> C_out;

    four_bit_adder four_bit_adder("four_bit_adder");
    four_bit_adder.A(A);
    four_bit_adder.B(B);
    four_bit_adder.S(S);
    four_bit_adder.C_out(C_out);

    // Test the four_bit_adder
    // Generate A 0-15 for all possible values as well as B for all possible values
    // Add a hit counter to see how many times the sum is correct
    int hit = 0;
    int total = 0;

    // Use VCD file to see the waveform
    sc_trace_file *wf = sc_create_vcd_trace_file("four_bit_adder");
    sc_trace(wf, A, "A");
    sc_trace(wf, B, "B");
    sc_trace(wf, S, "S");
    sc_trace(wf, C_out, "C_out");

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            A = i;
            B = j;
            sc_start(1, SC_NS);
            cout << "A = " << A.read() << ", B = " << B.read() << ", S = " << S.read() << ", C_out = " << C_out.read() << endl;

            // Check if the sum is correct
            if (S.read() != (i + j) % 16) {
                cout << "Error: " << i << " + " << j << " = " << S.read() << " != " << (i + j) % 16 << endl;
                total++;
            } else {
                hit++;
                total++;
            }
        }
    }
    cout << "Hit: " << hit << " out of " << total << endl;

    // Close the VCD file
    sc_close_vcd_trace_file(wf);
}