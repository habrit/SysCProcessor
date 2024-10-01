#include "systemc.h"



SC_MODULE(registerFile) {
    sc_in<sc_uint<32>> BusW;
    sc_in<sc_uint<5>> ReadReg1, ReadReg2, WriteReg;
    sc_in<bool> RegWrite;
    sc_in<bool> clk;

    sc_out<sc_uint<32>> ReadData1, ReadData2;

    void do_registerFile() {
        sc_uint<32> registers[32] = {0}; // 32 registers
        //Set Zero register to 0
        registers[0] = 0;

        while (true) {
            wait(); // Wait for the clock edge

            // Read data from the registers
            ReadData1.write(registers[ReadReg1.read()]);
            ReadData2.write(registers[ReadReg2.read()]);

            // Write data to the registers and make sure not writing to register 0
            if ((RegWrite.read() == true) && (WriteReg.read() != 0)) {
                registers[WriteReg.read()] = BusW.read();
            }
        }
    }


}