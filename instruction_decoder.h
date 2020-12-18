#ifndef RISC_V_INSTRUCTION_DECODER_H
#define RISC_V_INSTRUCTION_DECODER_H

enum opcode_e {
    OP     = 0x33,
    OP_IMM = 0x13,
    JAL    = 0x6F,
    JALR   = 0x67,
    BRANCH = 0x63,
    LOAD   = 0x03,
    STORE  = 0x23,
    LUI    = 0x37,
    AUIPC  = 0x17
};

enum funct3_e {
    ADD  = 0x0,
    SUB  = 0x0,
    SLL  = 0x1,
    SLT  = 0x2,
    SLTU = 0x3,
    XOR  = 0x4,
    SRL  = 0x5,
    SRA  = 0x5,
    OR   = 0x6,
    AND  = 0x7,

    BEQ  = 0x0,
    BNE  = 0x1,
    BLT  = 0x4,
    BGE  = 0x5,
    BLTU = 0x6,
    BGEU = 0x7,

    LB   = 0x0,
    LH   = 0x1,
    LW   = 0x2,
    LBU  = 0x4,
    LHU  = 0x5,

    SB   = 0x0,
    SH   = 0x1,
    SW   = 0x2
};

enum exec_command_status
{
    EXEC_OK,
    EXEC_EXIT
};

typedef struct {
    int32_t  opcode;
    int32_t* rd;
    int32_t  funct3;
    int32_t  funct7;
    int32_t* op1;
    int32_t* op2;
    int32_t  imm_offs;
} inst_t;

typedef struct {
    int n_inst;
    int32_t pc0;
    inst_t* inst;
} linear_block;

int get_linear_block_size(char* mem, int32_t* pc);
int clear_linear_block(linear_block* lb);
int load_decode_linear_block(char *mem, int pc0, linear_block *lb, int n_inst, int32_t *x);
int exec_linear_block(char* mem, int32_t* pc, linear_block* lb, FILE* f_log);
int find_linear_block(int32_t pc0, linear_block* lb_cache, int n_block);

int exec_arithm_op(int32_t funct3, int32_t funct7, int32_t op1, int32_t op2, int32_t *rd, int32_t *pc);
int exec_branch_op(int32_t funct3, int32_t offset, int32_t op1, int32_t op2, int32_t *pc);
int exec_load_op(int32_t funct3, int32_t offset, int32_t base, char *mem, int32_t *rd, int32_t *pc);
int exec_store_op(int32_t funct3, int32_t offset, int32_t base, char *mem, int32_t val, int32_t *pc);

void decode_B_type(int32_t inst, int32_t *x, char *mem, inst_t *i);
void decode_S_type(int32_t inst, int32_t *x, char *mem, inst_t *i);
void decode_J_type(int32_t inst, int32_t *x, char *mem, inst_t *i);
void decode_R_type(int32_t inst, int32_t *x, char *mem, inst_t *i);
void decode_I_type(int32_t inst, int32_t *x, char *mem, inst_t *i);
void decode_U_type(int32_t inst, int32_t *x, char *mem, inst_t *i);

#endif //RISC_V_INSTRUCTION_DECODER_H