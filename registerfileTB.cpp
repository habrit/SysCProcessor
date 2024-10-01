#include "systemc.h"
#include "registerfile.cpp"


SC_MODULE(registerTestBench){
    // Declare signals
    sc_in<sc_uint<32>> BusW;
    sc_in<sc_uint<5>> ReadReg1, ReadReg2, WriteReg;
    sc_in<bool> RegWrite;
    sc_in<bool> clk;

    sc_out<sc_uint<32>> ReadData1, ReadData2;

    // Instance of the registerFile
    registerFile* regFile;

    // Constructor
    SC_CTOR(registerTestBench) {
        regFile = new registerFile("registerFile_Instance");  // Create an instance of the registerFile

        // Connect signals to the registerFile
        regFile->BusW(BusW);
        regFile->ReadReg1(ReadReg1);
        regFile->ReadReg2(ReadReg2);
        regFile->WriteReg(WriteReg);
        regFile->RegWrite(RegWrite);
        regFile->clk(clk);
        regFile->ReadData1(ReadData1);
        regFile->ReadData2(ReadData2);

        // Start the test process
        SC_THREAD(run_tests);
    }

    void custom_assert(bool condition, const std::string& message) {
        if (!condition) {
            cout << "Assertion FAILED: " << message << endl;
            cout << "Condition was: " << condition << endl;
            
        } else {
            cout << "Assertion Passed: " << message << endl;
        }
    }

    void run_tests() {
        wait(10, SC_NS);  // Wait for a few nanoseconds to allow setup

        // Test Case 1: Write to register 1, read back from register 1
        WriteReg.write(1);    // Writing to register 1
        BusW.write(0x12345678);  // Value to write
        RegWrite.write(true); // Enable write
        wait(clk.posedge_event());  // Wait for clock edge
        RegWrite.write(false); // Disable write after clock edge

        ReadReg1.write(1);     // Read from register 1
        wait(clk.posedge_event());
        custom_assert(ReadData1.read() == 0x12345678, "Test Case 1: Write to register 1 and read back");

        // Test Case 2: Write to register 2, read back from register 2
        WriteReg.write(2);    // Writing to register 2
        BusW.write(0x87654321);  // Value to write
        RegWrite.write(true); // Enable write
        wait(clk.posedge_event());
        RegWrite.write(false); // Disable write

        ReadReg1.write(2);     // Read from register 2
        wait(clk.posedge_event());
        custom_assert(ReadData1.read() == 0x87654321, "Test Case 2: Write to register 2 and read back");

        // Test Case 3: Write to register 0 (shouldn't be allowed, as register 0 is constant 0)
        WriteReg.write(0);    // Attempt to write to register 0
        BusW.write(0xFFFFFFFF);  // Value to write (shouldn't be written)
        RegWrite.write(true); // Enable write
        wait(clk.posedge_event());
        RegWrite.write(false); // Disable write

        ReadReg1.write(0);     // Read from register 0 (should still be 0)
        wait(clk.posedge_event());
        custom_assert(ReadData1.read() == 0x0, "Test Case 3: Write to register 0 and read back (should still be 0)");

        // Test Case 4: Read from two registers simultaneously
        WriteReg.write(3);    // Writing to register 3
        BusW.write(0xDEADBEEF);  // Value to write
        RegWrite.write(true); // Enable write
        wait(clk.posedge_event());
        RegWrite.write(false); // Disable write

        ReadReg1.write(1);  // Read from register 1
        ReadReg2.write(3);  // Read from register 3
        wait(clk.posedge_event());
        custom_assert(ReadData1.read() == 0x12345678 && ReadData2.read() == 0xDEADBEEF,
            "Test Case 4: Simultaneous read from register 1 and 3");

        // Finish the simulation
        sc_stop();
    }

}