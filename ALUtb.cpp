#include "systemc.h"
#include "ALU.cpp"

SC_MODULE(Testbench) {
    // Declare signals
    sc_signal<sc_uint<32>> BusA, BusB;    // Input signals
    sc_signal<sc_uint<32>> BusW;           // Output signal
    sc_signal<bool> Zero;                   // Zero flag signal
    sc_signal<sc_uint<4>> ALUOp;           // Operation code

    // Instance of the ALU
    ALU* alu;

    // Constructor
    SC_CTOR(Testbench) {
        alu = new ALU("ALU_Instance");  // Create an instance of the ALU

        // Connect signals to the ALU
        alu->BusA(BusA);
        alu->BusB(BusB);
        alu->BusW(BusW);
        alu->Zero(Zero);
        alu->ALUOp(ALUOp);

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

        int testPassed = 0;
        int testFailed = 0;

        // Test 1: AND
        BusA.write(0xF0F0F0F0);
        BusB.write(0x0F0F0F0F);
        ALUOp.write(AND);
        wait(1, SC_NS); // Wait for the computation
        custom_assert(BusW.read() == (0xF0F0F0F0 & 0x0F0F0F0F), "AND test");
        custom_assert(Zero.read() == (BusW.read() == 0), "AND Zero flag test");
        

        // Test 2: OR
        BusA.write(0xF0F0F0F0);
        BusB.write(0x0F0F0F0F);
        ALUOp.write(OR);
        wait(1, SC_NS);
        custom_assert(BusW.read() == (0xF0F0F0F0 | 0x0F0F0F0F), "OR test");
        custom_assert(Zero.read() == (BusW.read() == 0), "OR Zero flag test");

        // Test 3: ADD
        BusA.write(5);
        BusB.write(7);
        ALUOp.write(ADD);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 12, "ADD test");
        custom_assert(Zero.read() == (BusW.read() == 0), "ADD Zero flag test");

        // Test 4: SUB
        BusA.write(10);
        BusB.write(5);
        ALUOp.write(SUB);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 5, "SUB test");
        custom_assert(Zero.read() == (BusW.read() == 0), "SUB Zero flag test");

        // Test 5: SLT (Set Less Than)
        BusA.write(5);
        BusB.write(10);
        ALUOp.write(SLT);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 1, "SLT test");
        custom_assert(Zero.read() == (BusW.read() == 0), "SLT Zero flag test");

        // Test 6: ADDU (unsigned addition)
        BusA.write(0xFFFFFFFF);
        BusB.write(1);
        ALUOp.write(ADDU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0, "ADDU test");
        custom_assert(Zero.read() == (BusW.read() == 0), "ADDU Zero flag test");


        // Test 7: SUBU (unsigned subtraction)
        BusA.write(0);
        BusB.write(0);
        ALUOp.write(SUBU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0, "SUBU test");
        custom_assert(Zero.read() == (BusW.read() == 0), "SUBU Zero flag test");

        //Stealing from verilog code
        // Test 8: 	SRL
        BusA.write(0xFFFF1234);
        BusB.write(6);
        ALUOp.write(SRL);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0x003FFFC48, "SRL test");
        custom_assert(Zero.read() == (BusW.read() == 0), "SRL Zero flag test");

        //Test 9: ADDU
        BusA.write(0x00000000);
        BusB.write(0x00000000);
        ALUOp.write(ADDU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0, "ADDU 0,0 test");
        custom_assert(Zero.read() == (BusW.read() == 0), "ADDU 0,0 Zero flag test");

        //Test 10: ADDU
        BusA.write(0x00000000);
        BusB.write(0xFFFFFFFF);
        ALUOp.write(ADDU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0xFFFFFFFF, "ADDU 0,-1 test");
        custom_assert(Zero.read() == (BusW.read() == 0), "ADDU 0,-1 Zero flag test");

        //Test 11: ADDU
        BusA.write(0xFFFFFFFF);
        BusB.write(0x00000000);
        ALUOp.write(ADDU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0xFFFFFFFF, "ADDU -1,0 test");
        custom_assert(Zero.read() == (BusW.read() == 0), "ADDU -1,0 Zero flag test");

        //Test 12: ADDU
        BusA.write(0x000000FF);
        BusB.write(0x00000001);
        ALUOp.write(ADDU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0x00000100, "ADDU FF,1 test");
        custom_assert(Zero.read() == (BusW.read() == 0), "ADDU FF,1 Zero flag test");

        //Test 13: ADDU
        //		{BusA, BusB, ALUCtrl} = {32'h00000000, 32'h00000000, 4'd8}; #40; passTest({Zero, BusW}, 33'h100000000, "SUBU 0,0",);
        BusA.write(0x00000000);
        BusB.write(0x00000000);
        ALUOp.write(SUBU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0, "SUBU 0,0 test");
        custom_assert(Zero.read() == 1, "SUBU 0,0 Zero flag test");

        //Test 14: SUBU
        BusA.write(0x00000001);
        BusB.write(0xFFFFFFFF);
        ALUOp.write(SUBU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 2, "SUBU 1,-1 test");
        custom_assert(Zero.read() == 0, "SUBU 1,-1 Zero flag test");

        //Test 15: SUBU
        BusA.write(0x00000001);
        BusB.write(0x00000001);
        ALUOp.write(SUBU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0, "SUBU 1,1 test");
        custom_assert(Zero.read() == 1, "SUBU 1,1 Zero flag test");

        //Test 16: XOR
        BusA.write(0xF0F0F0F0);
        BusB.write(0x0000FFFF);
        ALUOp.write(XOR);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0x0F0F00F0F, "XOR 0xF0F0F0F0,0x0000FFFF test");
        custom_assert(Zero.read() == 0, "XOR 0xF0F0F0F0,0x0000FFFF Zero flag test");

        //Test 17: XOR
        BusA.write(0x12345678);
        BusB.write(0x87654321);
        ALUOp.write(XOR);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0x095511559, "XOR 0x12345678,0x87654321 test");
        custom_assert(Zero.read() == 0, "XOR 0x12345678,0x87654321 Zero flag test");

        //Test 18: SLTU
        BusA.write(0x00000000);
        BusB.write(0x00000000);
        ALUOp.write(SLTU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0, "SLTU 0,0 test");
        custom_assert(Zero.read() == 1, "SLTU 0,0 Zero flag test");

        //Test 19: SLTU
        BusA.write(0x00000000);
        BusB.write(0x00000001);
        ALUOp.write(SLTU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 1, "SLTU 0,1 test");
        custom_assert(Zero.read() == 0, "SLTU 0,1 Zero flag test");

        //Test 20: SLTU
        BusA.write(0x00000000);
        BusB.write(0xFFFFFFFF);
        ALUOp.write(SLTU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 1, "SLTU 0,-1 test");
        custom_assert(Zero.read() == 0, "SLTU 0,-1 Zero flag test");

        //Test 21: SLTU
        BusA.write(0x00000001);
        BusB.write(0x00000000);
        ALUOp.write(SLTU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0, "SLTU 1,0 test");
        custom_assert(Zero.read() == 1, "SLTU 1,0 Zero flag test");

        //Test 22: SLTU
        BusA.write(0xFFFFFFFF);
        BusB.write(0x00000000);
        ALUOp.write(SLTU);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0, "SLTU -1,0 test");
        custom_assert(Zero.read() == 1, "SLTU -1,0 Zero flag test");

        //Test 23: NOR
        BusA.write(0xF0F0F0F0);
        BusB.write(0x0000FFFF);
        ALUOp.write(NOR);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0x00F0F0000, "NOR 0xF0F0F0F0,0x0000FFFF test");
        custom_assert(Zero.read() == 0, "NOR 0xF0F0F0F0,0x0000FFFF Zero flag test");

        //Test 24: NOR
        BusA.write(0x12345678);
        BusB.write(0x87654321);
        ALUOp.write(NOR);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0x0688aa886, "NOR 0x12345678,0x87654321 test");
        custom_assert(Zero.read() == 0, "NOR 0x12345678,0x87654321 Zero flag test");

        //Test 25: SRA
        BusA.write(0x00000001);
        BusB.write(3);
        ALUOp.write(SRA);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0, "SRA 0x00000001,3 test");
        custom_assert(Zero.read() == 1, "SRA 0x00000001,3 Zero flag test");

        //Test 26: SRA
        BusA.write(0x00001234);
        BusB.write(6);
        ALUOp.write(SRA);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0x000000048, "SRA 0x00001234,6 test");
        custom_assert(Zero.read() == 0, "SRA 0x00001234,6 Zero flag test");

        //Test 27: SRA
        BusA.write(0xFFFF1234);
        BusB.write(6);
        ALUOp.write(SRA);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0x3FFFC48, "SRA 0xFFFF1234,6 test");
        custom_assert(Zero.read() == 0, "SRA 0xFFFF1234,6 Zero flag test");

        //Test 28: LUI
        BusA.write(0);
        BusB.write(0x12345678);
        ALUOp.write(LUI);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0x56780000, "LUI 0x12345678 test");
        custom_assert(Zero.read() == 0, "LUI 0x12345678 Zero flag test");

        //Test 29: LUI
        BusA.write(0);
        BusB.write(0x00001234);
        ALUOp.write(LUI);
        wait(1, SC_NS);
        custom_assert(BusW.read() == 0x12340000, "LUI 0x00001234 test");
        custom_assert(Zero.read() == 0, "LUI 0x00001234 Zero flag test");

        // Finish simulation

        sc_stop();
    }

    ~Testbench() {
        delete alu; // Clean up allocated memory
    }
};

// Main function
int sc_main(int argc, char* argv[]) {
    Testbench tb("Testbench"); // Instantiate the testbench
    sc_start(); // Start the simulation
    return 0;
}