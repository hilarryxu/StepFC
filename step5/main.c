﻿#include "sfc_famicom.h"
#define SFC_NO_SUBRENDER
#define SFC_NO_SL
#include "../common/d2d_interface.h"
#include "sfc_cpu.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>

void sfc_log_exec(void* arg, sfc_famicom_t* famicom);

sfc_famicom_t* g_famicom = NULL;
extern uint32_t sfc_stdpalette[];
uint32_t palette_data[16];

/// <summary>
/// 获取坐标像素
/// </summary>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
/// <param name="nt">The nt.</param>
/// <param name="bg">The bg.</param>
/// <returns></returns>
uint32_t get_pixel(unsigned x, unsigned y, const uint8_t* nt, const uint8_t* bg) {
    // 获取所在名称表
    const unsigned id = (x >> 3) + (y >> 3) * 32;
    const uint32_t name = nt[id];
    // 查找对应图样表
    const uint8_t* nowp0 = bg + name * 16;
    const uint8_t* nowp1 = nowp0 + 8;
    // Y坐标为平面内偏移
    const uint8_t p0 = nowp0[y & 0x7];
    const uint8_t p1 = nowp1[y & 0x7];
    // X坐标为字节内偏移
    const uint8_t shift = (~x) & 0x7;
    const uint8_t mask = 1 << shift;
    // 计算低二位
    const uint8_t low = ((p0 & mask) >> shift) | ((p1 & mask) >> shift << 1);
    // 计算所在属性表
    const unsigned aid = (x >> 5) + (y >> 5) * 8;
    const uint8_t attr = nt[aid + (32 * 30)];
    // 获取属性表内位偏移
    const uint8_t aoffset = ((x & 0x10) >> 3) | ((y & 0x10) >> 2);
    // 计算高两位
    const uint8_t high = (attr & (3 << aoffset)) >> aoffset << 2;
    // 合并作为颜色
    const uint8_t index = high | low;

    return palette_data[index];
}

/// <summary>
/// 主渲染
/// </summary>
/// <param name="rgba">The RGBA.</param>
extern void main_render(void* rgba) {
    uint32_t* data = rgba;

    for (int i = 0; i != 5000; ++i) {
        sfc_cpu_execute_one(g_famicom);
    }

    sfc_do_vblank(g_famicom);


    // 生成调色板颜色
    {
        for (int i = 0; i != 16; ++i) {
            palette_data[i] = sfc_stdpalette[g_famicom->ppu.spindexes[i]];
        }
        palette_data[4 * 1] = palette_data[0];
        palette_data[4 * 2] = palette_data[0];
        palette_data[4 * 3] = palette_data[0];
    }
    // 背景
    const uint8_t* now = g_famicom->ppu.banks[8];
    const uint8_t* bgp = g_famicom->ppu.banks[
        g_famicom->ppu.ctrl & SFC_PPU2000_BgTabl ? 4 : 0];
    for (unsigned i = 0; i != 256 * 240; ++i) {
        data[i] = get_pixel(i & 0xff, i >> 8, now, bgp);
    }
}

/// <summary>
/// 应用程序入口
/// </summary>
/// <returns></returns>
int main() {
    sfc_interface_t interfaces = { NULL };
    interfaces.before_execute = sfc_log_exec;

    sfc_famicom_t famicom;
    g_famicom = &famicom;
    if (sfc_famicom_init(&famicom, NULL, &interfaces)) return 1;

    printf(
        "ROM: PRG-RPM: %d x 16kb   CHR-ROM %d x 8kb   Mapper: %03d\n",
        (int)famicom.rom_info.count_prgrom16kb,
        (int)famicom.rom_info.count_chrrom_8kb,
        (int)famicom.rom_info.mapper_number
    );

    main_cpp();

    sfc_famicom_uninit(&famicom);
    return 0;
}


/// <summary>
/// Users the input.
/// </summary>
/// <param name="index">The index.</param>
/// <param name="data">The data.</param>
void user_input(int index, unsigned char data) {
    assert(index >= 0 && index < 16);
    g_famicom->button_states[index] = data;
}



void sfc_log_exec(void* arg, sfc_famicom_t* famicom) {
    const uint16_t pc = famicom->registers.program_counter;
    static int line = 0;  line++;
    return;
    //if (line < 230297) return;

    char buf[SFC_DISASSEMBLY_BUF_LEN2];
    sfc_fc_disassembly(pc, famicom, buf);
    printf(
        "%4d - %s   A:%02X X:%02X Y:%02X P:%02X SP:%02X\n",
        line, buf,
        (int)famicom->registers.accumulator,
        (int)famicom->registers.x_index,
        (int)famicom->registers.y_index,
        (int)famicom->registers.status,
        (int)famicom->registers.stack_pointer
    );
}
