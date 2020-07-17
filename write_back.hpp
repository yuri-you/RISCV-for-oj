#ifndef WRITE_BACK_HPP
#define WRITE_BACK_HPP
#include"memory_access.hpp"
#include"register.hpp"
#include"instruction_fetch.hpp"
#include"instruction_decode.hpp"
#include"execute.hpp"
bool WB(MA get) {
		if (get.op == _OUT)return false;
		switch (get.op) {
		case _LW:case _LB:case _LH:case _LBU:case _LHU:
		case _LUI:case _AUIPC:
		case _ADDI:case _SLTI:case _SLTIU:case _XORI:case _ORI:case _ANDI:
		case _SLLI:case _SRLI:case _SRAI:
		case _ADD:case _SUB:case _SLL:case _SLT:case _SLTU:case _SRL:case _SRA:
		case _XOR:case _OR:case _AND:
		x[get.rd] = get.receive;  break;
		case _JAL:case _JALR:pc = get.rpc + 4; x[get.rd] = get.receive; --pc_of_jump; is_pc_forwarding = false; break;
		case _BEQ:case _BLT:case _BNE:case _BGE:case _BLTU:case _BGEU:pc = get.rpc+4; --pc_of_jump; is_pc_forwarding = false; break;
		}
		x[0] = 0;
		return true;
	}


#endif // 

