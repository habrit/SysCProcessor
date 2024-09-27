#include "systemc.h"

#define AND 0b0000
#define OR 0b0001
#define ADD 0b0010
#define SLL 0b0011
#define SRL 0b0100
#define SUB 0b0110
#define SLT 0b0111
#define ADDU 0b1000
#define SUBU 0b1001
#define XOR 0b1010
#define SLTU 0b1011
#define NOR 0b1100
#define SRA 0b1101
#define LUI 0b1110

SC_MODULE(ALU) {
    sc_in<sc_uint<32>> BusA, BusB;
    sc_out<sc_uint<32>> BusW;
    sc_out<bool> Zero;
    sc_in<sc_uint<4>> ALUOp;

    void do_ALU() {
        sc_uint<32> result; // Store the result temporarily

        // Perform ALU operation based on ALUOp
        switch (ALUOp.read()) {
            case AND:
                result = BusA.read() & BusB.read();
                break;
            case OR:
                result = BusA.read() | BusB.read();
                break;
            case ADD:
                result = BusA.read() + BusB.read();
                break;
            case SLL:
                result = BusA.read() << BusB.read();
                break;
            case SRL:
                result = BusA.read() >> BusB.read();
                break;
            case SUB:
                result = BusA.read() - BusB.read();
                break;
            case SLT:
                result = BusA.read() < BusB.read() ? 1 : 0; // Return 1 for true, 0 for false
                break;
            case ADDU:
                result = BusA.read() + BusB.read();
                break;
            case SUBU:
                result = BusA.read() - BusB.read();
                break;
            case XOR:
                result = BusA.read() ^ BusB.read();
                break;
            case SLTU:
                result = BusA.read() < BusB.read() ? 1 : 0; // Return 1 for true, 0 for false
                break;
            case NOR:
                result = ~(BusA.read() | BusB.read());
                break;
            case SRA:
                result = BusA.read() >> BusB.read();
                break;
            case LUI:
                result = BusB.read() << 16;
                break;
            default:
                result = 0; // Default case for undefined ALUOp
                break;
        }

        // Write the result to BusW
        BusW.write(result);

        // Update the Zero flag based on the result
        Zero.write(result == 0); // Sets Zero to true if result is 0
    }

    SC_CTOR(ALU) {
        SC_METHOD(do_ALU);
        sensitive << BusA << BusB << ALUOp; // Make sure to trigger on these signals
    }
};