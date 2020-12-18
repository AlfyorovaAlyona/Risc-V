#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "instruction_decoder.h"
#include "op.h"

#define ones_or_zeroes(bit, len) (((bit) << (len)) - (bit))
#define slice(val, lsb, len) (((val) >> (lsb)) & ((1 << (len)) - 1))
#define i_imm(inst) (slice(inst, 20, 11) \
                    | (ones_or_zeroes(slice(inst, 31, 1), 21) << 11))
#define s_imm(inst) (slice(inst, 7, 5) | (slice(inst, 25, 6) << 5) | (ones_or_zeroes(slice(inst, 31, 1), 21) << 11))
#define j_imm(inst) ((slice(inst, 21, 10) << 1) | (slice(inst, 20, 1) << 11) | (slice(inst, 12, 8) << 12) \
                    | (ones_or_zeroes(slice(inst, 31, 1), 12) << 20))
#define b_imm(inst) ((slice(inst, 8, 4) << 1) | (slice(inst, 25, 6) << 5) \
                    | (slice(inst, 7, 1) << 11) | (ones_or_zeroes(slice(inst, 31, 1), 20) << 12))
#define u_imm(inst) ((slice(inst, 12, 8) << 12) | (slice(inst, 20, 11) << 20) | (slice(inst, 31, 1) << 31))

extern int32_t x[32];
extern uint32_t inst_cnt;

void decode_B_type(int32_t inst, int32_t *x, char *mem, inst_t *i)
{
    i->funct3 = slice(inst, 12, 3);
    i->op1    = (x + slice(inst, 15, 5));
    i->op2    = (x + slice(inst, 20, 5));
    i->imm_offs   = b_imm(inst);
}

void decode_R_type(int32_t inst, int32_t *x, char *mem, inst_t *i)
{
    i->funct3 = slice(inst, 12, 3);
    i->funct7 = slice(inst, 25, 7);

    i->rd  = (x + slice(inst,  7, 5));
    i->op1 = (x + slice(inst, 15, 5));
    i->op2 = (x + slice(inst, 20, 5));
}

void decode_I_type(int32_t inst, int32_t *x, char *mem, inst_t *i)
{
    i->funct3 = slice(inst, 12, 3);
    i->funct7 = slice(inst, 25, 7);
    i->imm_offs = i_imm(inst);
    i->rd     = x + slice(inst, 7, 5);
    i->op1    = x + slice(inst, 15, 5);
}

void decode_S_type(int32_t inst, int32_t *x, char *mem, inst_t *i)
{
    i->funct3 = slice(inst, 12, 3);
    i->imm_offs   = s_imm(inst);
    i->op1    = x + slice(inst, 15, 5);
    i->op2    = x + slice(inst, 20, 5);
}

void decode_J_type(int32_t inst, int32_t *x, char *mem, inst_t *i)
{
    i->rd = x + slice(inst, 7, 5);
    i->imm_offs = j_imm(inst);
}

void decode_U_type(int32_t inst, int32_t *x, char *mem, inst_t *i)
{
    i->rd     = x + slice(inst, 7, 5);
    i->imm_offs    = u_imm(inst);
}


int exec_arithm_op(int32_t funct3, int32_t funct7, int32_t op1, int32_t op2, int32_t *rd, int32_t *pc)
{
    switch (funct3) {
        case ADD:
            add_op(op1, op2, rd, pc);
            return EXEC_OK;
        case SLT:
            slt_op(op1, op2, rd, pc);
            return EXEC_OK;
        case SLTU:
            sltu_op(op1, op2, rd, pc);
            return EXEC_OK;
        case XOR:
            xor_op(op1, op2, rd, pc);
            return EXEC_OK;
        case SRL: //same as case SRA
            switch (funct7)
            {
                case 0x00:
                    srl_op(op1, op2 & 0x1F, rd, pc);
                    return EXEC_OK;
                case 0x20:
                    sra_op(op1, op2 & 0x1F, rd, pc);
                    return EXEC_OK;
                default:
                    return EXEC_EXIT;
            }
        case SLL:
            if(funct7 == 0x00) {
                sll_op(op1, op2 & 0x1F, rd, pc);
                return EXEC_OK;
            } else
                return EXEC_EXIT;
        case AND:
            and_op(op1, op2, rd, pc);
            return EXEC_OK;
        case OR:
            or_op(op1, op2, rd, pc);
            return EXEC_OK;
        default:
            return EXEC_EXIT;
    }
}

int exec_branch_op(int32_t funct3, int32_t offset, int32_t op1, int32_t op2, int32_t *pc)
{
    switch (funct3)
    {
        case BEQ:
            beq_op(offset, op1, op2, pc);
            return EXEC_OK;
        case BNE:
            bne_op(offset, op1, op2, pc);
            return EXEC_OK;
        case BLT:
            blt_op(offset, op1, op2, pc);
            return EXEC_OK;
        case BLTU:
            bltu_op(offset, op1, op2, pc);
            return EXEC_OK;
        case BGE:
            bge_op(offset, op1, op2, pc);
            return EXEC_OK;
        case BGEU:
            bgeu_op(offset, op1, op2, pc);
            return EXEC_OK;
        default:
            return EXEC_EXIT;
    }
}

int exec_load_op(int32_t funct3, int32_t offset, int32_t base, char *mem, int32_t *rd, int32_t *pc)
{
    switch (funct3)
    {
        case LB:
            lb_op(offset, base, mem, rd, pc);
            return EXEC_OK;
        case LH:
            lh_op(offset, base, mem, rd, pc);
            return EXEC_OK;
        case LW:
            lw_op(offset, base, mem, rd, pc);
            return EXEC_OK;
        case LBU:
            lbu_op(offset, base, mem, rd, pc);
            return EXEC_OK;
        case LHU:
            lhu_op(offset, base, mem, rd, pc);
            return EXEC_OK;
        default:
            return EXEC_EXIT;
    }
}

int exec_store_op(int32_t funct3, int32_t offset, int32_t base, char *mem, int32_t val, int32_t *pc)
{
    switch (funct3)
    {
        case SB:
            sb_op(offset, base, mem, val, pc);
            return EXEC_OK;
        case SH:
            sh_op(offset, base, mem, val, pc);
            return EXEC_OK;
        case SW:
            sw_op(offset, base, mem, val, pc);
            return EXEC_OK;
        default:
            return EXEC_EXIT;
    }
}


int get_linear_block_size(char* mem, int32_t* pc)
{
    int32_t pc0 = *pc;
    int n = 0;
    do
    {
        int32_t inst = *(int32_t*)(mem + pc0 + 4*n);
        int32_t opcode = slice(inst, 0, 7);
        n++;
        if ((opcode == BRANCH) || (opcode == JALR) || (opcode == JAL))
            break;
    } while(1);
    return n;
}

int load_decode_linear_block(char *mem, int pc0, linear_block *lb, int n_inst, int32_t *x)
{
    lb->inst = (inst_t*) calloc(n_inst, sizeof(inst_t));
    lb->n_inst = n_inst;
    lb->pc0 = pc0;

    for(int i = 0; i < n_inst; i++)
    {
        int32_t inst = *(int32_t*)(mem + pc0 + 4*i);
        lb->inst[i].opcode = slice(inst, 0, 7);
        switch (lb->inst[i].opcode)
        {
            case OP:
                decode_R_type(inst, x, mem, &(lb->inst[i]));
                break;
            case OP_IMM:
            case LOAD:
            case JALR:
                decode_I_type(inst, x, mem, &(lb->inst[i]));
                break;
            case LUI:
            case AUIPC:
                decode_U_type(inst, x, mem, &(lb->inst[i]));
                break;
            case BRANCH:
                decode_B_type(inst, x, mem, &(lb->inst[i]));
                break;
            case JAL:
                decode_J_type(inst, x, mem, &(lb->inst[i]));
                break;
            case STORE:
                decode_S_type(inst, x, mem, &(lb->inst[i]));
                break;
            default:
                return EXEC_EXIT;

        }
    }
}

#define ld_reg(addr) (*(int32_t*)(addr))
#define ld_reg_sub(addr, funct3, funct7) (((funct3) == SUB) && ((funct7) == 0x20) ? -ld_reg(addr) : ld_reg(addr))

int exec_linear_block(char* mem, int32_t* pc, linear_block* lb, FILE* f_log)
{
    for(int i = 0; i < lb->n_inst; i++)
    {
        int32_t inst = ld_reg(mem + *pc);
        int32_t rd = slice(inst, 7, 5);
        fprintf(f_log, "%8x\t%8x\t%2d\t", *pc, inst,
                ((lb->inst[i].opcode != STORE) && (lb->inst[i].opcode != BRANCH)) ? rd : 0);
        switch(lb->inst[i].opcode)
        {
            case OP:
                exec_arithm_op(lb->inst[i].funct3, lb->inst[i].funct7, ld_reg(lb->inst[i].op1), ld_reg_sub(lb->inst[i].op2, lb->inst[i].funct3, lb->inst[i].funct7), lb->inst[i].rd, pc);
                break;
            case OP_IMM:
                exec_arithm_op(lb->inst[i].funct3, lb->inst[i].funct7, ld_reg(lb->inst[i].op1), lb->inst[i].imm_offs, lb->inst[i].rd, pc);
                break;
            case BRANCH:
                exec_branch_op(lb->inst[i].funct3, lb->inst[i].imm_offs, ld_reg(lb->inst[i].op1), ld_reg(lb->inst[i].op2), pc);
                break;
            case LOAD:
                exec_load_op(lb->inst[i].funct3, lb->inst[i].imm_offs, ld_reg(lb->inst[i].op1), mem, lb->inst[i].rd, pc);
                break;
            case STORE:
                exec_store_op(lb->inst[i].funct3, lb->inst[i].imm_offs, ld_reg(lb->inst[i].op1), mem, ld_reg(lb->inst[i].op2), pc);
                break;
            case LUI:
                lui_op(lb->inst[i].imm_offs, lb->inst[i].rd, pc);
                break;
            case AUIPC:
                auipc_op(lb->inst[i].imm_offs, lb->inst[i].rd, pc);
                break;
            case JALR:
                jalr_op(lb->inst[i].imm_offs, ld_reg(lb->inst[i].op1), lb->inst[i].rd, pc);
                break;
            case JAL:
                jal_op(lb->inst[i].imm_offs, lb->inst[i].rd, pc);
                break;
            default:
                return EXEC_EXIT;
        }
        x[0] = 0;
        inst_cnt++;
    }
    return EXEC_OK;
}

int find_linear_block(int32_t pc0, linear_block* lb_cache, int n_block)
{
    for (int i = 0; i < n_block; i++)
        if(lb_cache[i].pc0 == pc0)
            return i;
    return -1;
}

int clear_linear_block(linear_block* lb)
{
    if (!lb->inst)
        free(lb->inst);
    lb->pc0 = 0;
    lb->n_inst = 0;
}