#include "systemc.h"






SC_MODULE(four_bit_adder) {
    sc_in<sc_uint<4> > A, B;
    sc_out<sc_uint<4> > S;
    sc_out<bool> C_out;

  void do_four_bit_adder() {
    S.write(A.read() + B.read());
    C_out.write((A.read() + B.read()) > 15);
  }

    SC_CTOR(four_bit_adder) {
        SC_METHOD(do_four_bit_adder);
        sensitive << A << B;
    }
};