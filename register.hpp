#ifndef REGISTER_HPP
#define REGISTER_HPP
#include<cstring>
	unsigned sext(unsigned x, int n) {
		return (x & (1U << (n - 1))) ? (((0xffffffff >> n) << n)| x) : x;
	}
	enum command {
		_LUI, _AUIPC, _JAL, _JALR, _BEQ, _BNE, _BLT, _BGE, _BLTU, _BGEU,
		_LB, _LH, _LW, _LBU, _LHU, _SB, _SH, _SW, _ADDI, _SLTI, _SLTIU,
		_XORI, _ORI, _ANDI, _SLLI, _SRLI, _SRAI, _ADD, _SUB, _SLL, _SLT,
		_SLTU,_XOR, _SRL, _SRA, _OR, _AND,_OUT,_NOT_JUMP
	};
	enum types {
		_U, _I, _B, _S, _J, _R
	};
	unsigned x[34] = { 0 };
	const unsigned& zero = x[0];
	unsigned& ra = x[1], & sp = x[2], & gp = x[3], & tp = x[4], & t0 = x[5], & t1 = x[6], & t2 = x[7], & fp = x[8], & s0 = x[8], & s1 = x[9], & a0 = x[10], & a1 = x[11], & a2 = x[12], & a3 = x[13], & a4 = x[14], & a5 = x[15], & a6 = x[16];
	unsigned& a7 = x[17], & s2 = x[18], & s3 = x[19], & s4 = x[20], & s5 = x[21], & s6 = x[22], & s7 = x[23], & s8 = x[24], & s9 = x[25], & s10 = x[26], & s11 = x[27], & t3 = x[28], & t4 = x[29], & t5 = x[30], & t6 = x[31], & pc = x[32];
	int wait_for_store[35] = { 0 };
	int pc_of_jump =  0 ;
	bool finish = false,is_pc_forwarding=false;
	int MAtime = 0;
	unsigned pc_forwarding = 0;
	unsigned alltimes = 0, righttimes = 0;
	bool popstore = false;
	bool idm[34] = { false };
	bool mam[34] = { false };
	bool first;
	void clearidm() {
		memset(idm, 0, 34 * sizeof(bool));
	}
	void clearmam() {
		memset(mam, 0, 34 * sizeof(bool));
	}
	unsigned b000 = 0, b001 = 0, b010 = 0, b011 = 0, b100 = 0, b101 = 0, b110 = 0, b111 = 0, lastrecord[3] = { 0,1,0 };
	void predict_result() {
		std::cout << std::endl << "alltimes:" << alltimes << " righttimes:" << righttimes << std::endl;
		std::cout << "percent:" << std::setiosflags(std::ios::fixed) << std::setprecision(2) << (double)(righttimes) / (double)(alltimes) << std::endl;
	}
	bool predict() {
		switch ((lastrecord[0] << 2) + (lastrecord[1]<<1)+lastrecord[2]) {
		case 0:return (b000 > 1);
		case 1:return (b001 > 1);
		case 2:return (b010 > 1);
		case 3:return(b011 > 1);
		case 4:return (b100 > 1);
		case 5:return (b101 > 1);
		case 6:return (b110 > 1);
		case 7:return(b111 > 1);
		}
		return true;
	}
	void add_counter() {
		switch ((lastrecord[0] << 2) + (lastrecord[1] << 1) + lastrecord[2]) {
		case 0: {
			if (b000<3) {
				++b000;
			}
			break;
		}
		case 1: {
			if (b001 < 3) {
				++b001;
			}
			break;
		}
		case 2: {
			if (b010 < 3) {
				++b010;
			}
			break;
		}
		case 3: {
			if (b011 < 3) {
				++b011;
			}
			break;
		}
		case 4: {
			if (b100 < 3) {
				++b100;
			}
			break;
		}
		case 5: {
			if (b101 < 3) {
				++b101;
			}
			break;
		}
		case 6: {
			if (b110 < 3) {
				++b110;
			}
			break;
		}
		case 7: {
			if (b111 < 3) {
				++b111;
			}
			break;
		}
		}
		lastrecord[0] = lastrecord[1];
		lastrecord[1] = lastrecord[2];
		lastrecord[2] = 1;
	}
	void minus_counter() {
		switch ((lastrecord[0] << 2) + (lastrecord[1] << 1) + lastrecord[2]) {
		case 0: {
			if (b000 > 0) {
				--b000;
			}
			break;
		}
		case 1: {
			if (b001 > 0) {
				--b001;
			}
			break;
		}
		case 2: {
			if (b010 > 0) {
				--b010;
			}
			break;
		}
		case 3: {
			if (b011 > 0) {
				--b011;
			}
			break;
		}
		case 4: {
			if (b100 > 0) {
				--b100;
			}
			break;
		}
		case 5: {
			if (b101 > 0) {
				--b101;
			}
			break;
		}
		case 6: {
			if (b110 > 0) {
				--b110;
			}
			break;
		}
		case 7: {
			if (b111 > 0) {
				--b111;
			}
			break;
		}
		}
		lastrecord[0] = lastrecord[1];
		lastrecord[1] = lastrecord[2];
		lastrecord[2] = 0;
	}
	void yout(command op) {
		switch (op) {
		case _LUI:std::cout << "LUI"; break;
		case _AUIPC:std::cout << "AUIPC"; break;
		case _JAL:std::cout << "JAL"; break;
		case _JALR:std::cout << "JALR"; break;
		case _BEQ:std::cout << "BEQ"; break;
		case _BNE:std::cout << "BNE"; break;
		case _BLT:std::cout << "BLT"; break;
		case _BGE:std::cout << "BGE"; break;
		case _BLTU:std::cout << "BLTU"; break;
		case _BGEU:std::cout << "BGEU"; break;
		case _LB:std::cout << "LB"; break;
		case _LH:std::cout << "LH"; break;
		case _LW:std::cout << "LW"; break;
		case _LBU:std::cout << "LBU"; break;
		case _LHU:std::cout << "LHU"; break;
		case _SB:std::cout << "SB"; break;
		case _SH:std::cout << "SH"; break;
		case _ADDI:std::cout << "ADDI"; break;
		case _SW:std::cout << "SW"; break;
		case _SLTI:std::cout << "SLTI"; break;
		case _SLTIU:std::cout << "SLTIU"; break;
		case _XORI:std::cout << "XORI"; break;
		case _ORI:std::cout << "ORI"; break;
		case _ANDI:std::cout << "ANDI"; break;
		case _SLLI:std::cout << "SLLI"; break;
		case _SRLI:std::cout << "SRLI"; break;
		case _SRAI:std::cout << "SRAI"; break;
		case _ADD:std::cout << "ADD"; break;
		case _SUB:std::cout << "SUB"; break;
		case _SLL:std::cout << "SLL"; break;
		case _SLT:std::cout << "SLT"; break;
		case _SLTU:std::cout << "SLTU"; break;
		case _XOR:std::cout << "XOR"; break;
		case _SRL:std::cout << "SRL"; break;
		case _SRA:std::cout << "SRA"; break;
		case _OR:std::cout << "OR"; break;
		case _AND:std::cout << "AND"; break;
		case _OUT:std::cout << "OUT"; break;
		case _NOT_JUMP:std::cout << "NOT_JUMP"; break;
		}
		std::cout << std::endl;
	}


#endif 

