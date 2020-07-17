#ifndef MEMORY_ACCESS_HPP
#define MEMORY_ACCESS_HPP
#include"execute.hpp"
#include"register.hpp"
#include"instruction_decode.hpp"
#include"instruction_fetch.hpp"
struct MA {
	unsigned receive;
	command op;
	unsigned rpc, rd;
	MA() {}
	MA(const EX& other) {
		if (other.op == _OUT) {
			printf("%u", other.result);
			op = other.op;
			return;
		}
		rd = other.rd;
		op = other.op;
		rpc = other.rpc;
		switch (op) {
		case _LB:receive = sext(((unsigned)(*((unsigned char*)(other.document->memory) + other.result))), 8); break;
		case _LH:receive = sext((unsigned)(*((unsigned short*)((unsigned char*)(other.document->memory) + other.result))), 16); break;
		case _LW:receive = *((unsigned*)((unsigned char*)(other.document->memory) + other.result)); break;
		case _LBU:receive = (unsigned)*((unsigned char*)(other.document->memory) + other.result); break;
		case _LHU:receive = (unsigned)(*(unsigned short*)((unsigned char*)(other.document->memory) + other.result)); break;
		case _SB: {
			unsigned char* tmp = (unsigned char*)(&(other.rst2));
			*((unsigned char*)(other.document->memory) + other.result) = *(tmp + 3);
			break;
		}
		case _SH: {
			unsigned short* tmp = (unsigned short*)(&(other.rst2));
			*((unsigned short*)((unsigned char*)(other.document->memory) + other.result)) = *(tmp + 1);
			break;
		}
		case _SW:*((unsigned*)((unsigned char*)(other.document->memory) + other.result)) = other.rst2; break;
		case _BEQ:case _BLT:case _BNE:case _BGE:case _BLTU:case _BGEU:
		case _JAL:case _JALR:receive = other.result; is_pc_forwarding = true; pc_forwarding = rpc; break;
		default:receive = other.result; 
		}
	}
};
#endif // !MEMORY_ACCESS