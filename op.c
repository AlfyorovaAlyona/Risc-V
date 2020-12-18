#include <stdint.h>
#include "op.h"

extern inline void  add_op(int32_t op1, int32_t op2, int32_t* rd, int32_t* pc);

extern inline void slt_op(int32_t op1, int32_t op2, int32_t* rd, int32_t* pc);

extern inline void sltu_op(int32_t op1, int32_t op2, int32_t* rd, int32_t* pc);

extern inline void xor_op(int32_t op1, int32_t op2, int32_t* rd, int32_t* pc);

extern inline void sll_op(int32_t op1, int32_t op2, int32_t* rd, int32_t* pc);

extern inline void srl_op(int32_t op1, int32_t op2, int32_t* rd, int32_t* pc);

extern inline void sra_op(int32_t op1, int32_t op2, int32_t* rd, int32_t* pc);

extern inline void or_op(int32_t op1, int32_t op2, int32_t* rd, int32_t* pc);

extern inline void and_op(int32_t op1, int32_t op2, int32_t* rd, int32_t* pc);

extern inline void lui_op(int32_t imm, int32_t* rd, int32_t* pc);

extern inline void auipc_op(int32_t imm, int32_t* rd, int32_t* pc);

//******************** UNCONDITIONAL JUMPS ********************

extern inline void jal_op(int32_t offset, int32_t *rd, int32_t *pc);

extern inline void jalr_op(int32_t offset, int32_t base, int32_t* rd, int32_t* pc);

//*************************** BRANCH ***************************

extern inline void beq_op(int32_t offset, int32_t op1, int32_t op2, int32_t* pc);

extern inline void bne_op(int32_t offset, int32_t op1, int32_t op2, int32_t* pc);

extern inline void blt_op(int32_t offset, int32_t op1, int32_t op2, int32_t* pc);

extern inline void bltu_op(int32_t offset, int32_t op1, int32_t op2, int32_t* pc);

extern inline void bge_op(int32_t offset, int32_t op1, int32_t op2, int32_t* pc);

extern inline void bgeu_op(int32_t offset, int32_t op1, int32_t op2, int32_t* pc);

//************************ LOAD AND STORE ***********************

extern inline void   lw_op(int32_t offset, int32_t base, char* mem, int32_t* rd, int32_t* pc);

extern inline void   lh_op(int32_t offset, int32_t base, char* mem, int32_t* rd, int32_t* pc);

extern inline void   lb_op(int32_t offset, int32_t base, char* mem, int32_t* rd, int32_t* pc);

extern inline void  lhu_op(int32_t offset, int32_t base, char* mem, int32_t* rd, int32_t* pc);

extern inline void  lbu_op(int32_t offset, int32_t base, char* mem, int32_t* rd, int32_t* pc);

extern inline void   sw_op(int32_t offset, int32_t base, char* mem, int32_t val, int32_t* pc);

extern inline void   sh_op(int32_t offset, int32_t base, char* mem, int32_t val, int32_t* pc);

extern inline void   sb_op(int32_t offset, int32_t base, char* mem, int32_t val, int32_t* pc);
