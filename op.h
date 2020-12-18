#ifndef RISC_V_OP_H
#define RISC_V_OP_H

#include <stdint.h>
#include <stdio.h>

extern FILE *f_log;

#define log(name) fprintf(f_log, "%s\t%8x\n", #name, *rd)
#define log_n(name) fprintf(f_log, "%s\n", #name)

//******************** ARITHMETIC OPERATIONS ********************

inline void add_op(int32_t op1, int32_t op2, int32_t *rd, int32_t *pc) {
    *rd = op1 + op2;
    *pc += 4; //+4 bits since pc is pointer to int32_t
    log(add_op);
}

inline void slt_op(int32_t op1, int32_t op2, int32_t *rd, int32_t *pc) {
    *rd = (op1 < op2) ? 1 : 0;
    *pc += 4;
    log(slt_op);
}

inline void sltu_op(int32_t op1, int32_t op2, int32_t *rd, int32_t *pc) {
    *rd = ((uint32_t) op1 < (uint32_t) op2) ? 1 : 0;
    *pc += 4;
    log(sltu_op);
}

inline void xor_op(int32_t op1, int32_t op2, int32_t *rd, int32_t *pc) {
    *rd = op1 ^ op2;
    *pc += 4;
    log(xor_op);
}

inline void sll_op(int32_t op1, int32_t op2, int32_t *rd, int32_t *pc) {
    *rd = op1 << (op2 & 0x1F);
    *pc += 4;
    log(sll_op);
}

inline void srl_op(int32_t op1, int32_t op2, int32_t *rd, int32_t *pc) {
    *rd = (uint32_t) op1 >> (op2 & 0x1F);
    *pc += 4;
    log(srl_op);
}

inline void sra_op(int32_t op1, int32_t op2, int32_t *rd, int32_t *pc) {
    *rd = op1 >> (op2 & 0x1F);
    *pc += 4;
    log(sra_op);
}

inline void or_op(int32_t op1, int32_t op2, int32_t *rd, int32_t *pc) {
    *rd = op1 | op2;
    *pc += 4;
    log(or_op);
}

inline void and_op(int32_t op1, int32_t op2, int32_t *rd, int32_t *pc) {
    *rd = op1 & op2;
    *pc += 4;
    log(and_op);
}

inline void lui_op(int32_t imm, int32_t *rd, int32_t *pc) {
    *rd = imm;
    *pc += 4;
    log(lui_op);
}

inline void auipc_op(int32_t imm, int32_t *rd, int32_t *pc) {
    *rd = imm + *pc;
    *pc += 4;
    log(auipc_op);
}
//******************** UNCONDITIONAL JUMPS ********************

inline void jal_op(int32_t offset, int32_t *rd, int32_t *pc) {
    *rd = *pc + 4;
    *pc += offset;
    log(jal_op);
}

inline void jalr_op(int32_t offset, int32_t base, int32_t *rd, int32_t *pc) {
    *rd = *pc + 4;
    *pc = (offset + base) & (0xFFFFFFFE);
    log(jalr_op);
}

//*************************** BRANCH ***************************

inline void beq_op(int32_t offset, int32_t op1, int32_t op2, int32_t *pc) {
    *pc += (op1 == op2) ? offset : 4;
    log_n(beq_op);
}

inline void bne_op(int32_t offset, int32_t op1, int32_t op2, int32_t *pc) {
    *pc += (op1 != op2) ? offset : 4;
    log_n(bne_op);
}

inline void blt_op(int32_t offset, int32_t op1, int32_t op2, int32_t *pc) {
    *pc += (op1 < op2) ? offset : 4;
    log_n(blt_op);
}

inline void bltu_op(int32_t offset, int32_t op1, int32_t op2, int32_t *pc) {
    *pc += ((uint32_t) op1 < (uint32_t) op2) ? offset : 4;
    log_n(bltu_op);
}

inline void bge_op(int32_t offset, int32_t op1, int32_t op2, int32_t *pc) {
    *pc += (op1 >= op2) ? offset : 4;
    log_n(bge_op);
}

inline void bgeu_op(int32_t offset, int32_t op1, int32_t op2, int32_t *pc) {
    *pc += ((uint32_t) op1 >= (uint32_t) op2) ? offset : 4;
    log_n(bgeu_op);
}

//************************ LOAD AND STORE ***********************

inline void lw_op(int32_t offset, int32_t base, char *mem, int32_t *rd, int32_t *pc) {
    *rd = *((int32_t * )(mem + (uint32_t) base + offset));
    *pc += 4;
    log(lw_op);
}

inline void lh_op(int32_t offset, int32_t base, char *mem, int32_t *rd, int32_t *pc) {
    *rd = *((int32_t * )(mem + (uint32_t) base + offset)) & 0x0000FFFF;
    if ((*rd >> 15) & 0x00000001)
        *rd = *rd | 0xFFFF0000;
    *pc += 4;
    log(lh_op);
}

inline void lb_op(int32_t offset, int32_t base, char *mem, int32_t *rd, int32_t *pc) {
    *rd = *((int32_t * )(mem + (uint32_t) base + offset)) & 0x000000FF;
    if ((*rd >> 7) & 0x00000001)
        *rd = *rd | 0xFFFFFF00;
    *pc += 4;
    log(lb_op);
}

inline void lhu_op(int32_t offset, int32_t base, char *mem, int32_t *rd, int32_t *pc) {
    *rd = *((int32_t * )(mem + (uint32_t) base + offset)) & 0x0000FFFF;
    *pc += 4;
    log(lhu_op);
}

inline void lbu_op(int32_t offset, int32_t base, char *mem, int32_t *rd, int32_t *pc) {
    *rd = *((int32_t * )(mem + (uint32_t) base + offset)) & 0x000000FF;
    *pc += 4;
    log(lbu_op);
}

inline void sw_op(int32_t offset, int32_t base, char *mem, int32_t val, int32_t *pc) {
    *((int32_t * )(mem + (uint32_t) base + offset)) = val;
    *pc += 4;
    log_n(sw_op);
}

inline void sh_op(int32_t offset, int32_t base, char *mem, int32_t val, int32_t *pc) {
    *((int16_t * )(mem + (uint32_t) base + offset)) = val & 0x0000FFFF;
    *pc += 4;
    log_n(sh_op);
}

inline void sb_op(int32_t offset, int32_t base, char *mem, int32_t val, int32_t *pc) {
    *((int8_t * )(mem + (uint32_t) base + offset)) = val & 0x000000FF;
    *pc += 4;
    log_n(sb_op);
}

#endif //RISC_V_OP_H
