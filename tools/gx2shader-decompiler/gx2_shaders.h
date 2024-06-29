#pragma once

#include "gx2r_buffer.h"
#include "types.h"

#include <libdecaf/src/cafe/libraries/gx2/gx2_format.h>
#include <libgpu/latte/latte_registers.h>

typedef struct _GX2UniformVar
{
    const char* name(const void* basePtr) const
    {
        return (const char*)((uintptr_t)basePtr + nameOffset);
    }

    uint32_t nameOffset; // const char*
    cafe::gx2::GX2ShaderVarType type;
    uint32_t count;
    uint32_t offset;
    int32_t block;
} GX2UniformVar;
CHECK_OFFSET(GX2UniformVar, 0x00, nameOffset);
CHECK_OFFSET(GX2UniformVar, 0x04, type);
CHECK_OFFSET(GX2UniformVar, 0x08, count);
CHECK_OFFSET(GX2UniformVar, 0x0C, offset);
CHECK_OFFSET(GX2UniformVar, 0x10, block);
CHECK_SIZE(GX2UniformVar, 0x14);

typedef struct _GX2UniformBlock
{
    const char* name(const void* basePtr) const
    {
        return (const char*)((uintptr_t)basePtr + nameOffset);
    }

    uint32_t nameOffset; // const char*
    uint32_t offset;
    uint32_t size;
} GX2UniformBlock;
CHECK_OFFSET(GX2UniformBlock, 0x00, nameOffset);
CHECK_OFFSET(GX2UniformBlock, 0x04, offset);
CHECK_OFFSET(GX2UniformBlock, 0x08, size);
CHECK_SIZE(GX2UniformBlock, 0x0C);

typedef struct _GX2AttribVar
{
    const char* name(const void* basePtr) const
    {
        return (const char*)((uintptr_t)basePtr + nameOffset);
    }

    uint32_t nameOffset; // const char*
    cafe::gx2::GX2ShaderVarType type;
    uint32_t count;
    uint32_t location;
} GX2AttribVar;
CHECK_OFFSET(GX2AttribVar, 0x00, nameOffset);
CHECK_OFFSET(GX2AttribVar, 0x04, type);
CHECK_OFFSET(GX2AttribVar, 0x08, count);
CHECK_OFFSET(GX2AttribVar, 0x0C, location);
CHECK_SIZE(GX2AttribVar, 0x10);

typedef struct _GX2SamplerVar
{
    const char* name(const void* basePtr) const
    {
        return (const char*)((uintptr_t)basePtr + nameOffset);
    }

    uint32_t nameOffset; // const char*
    cafe::gx2::GX2SamplerVarType type;
    uint32_t location;
} GX2SamplerVar;
CHECK_OFFSET(GX2SamplerVar, 0x00, nameOffset);
CHECK_OFFSET(GX2SamplerVar, 0x04, type);
CHECK_OFFSET(GX2SamplerVar, 0x08, location);
CHECK_SIZE(GX2SamplerVar, 0x0C);

typedef struct _GX2LoopVar
{
    uint32_t offset;
    uint32_t value;
} GX2LoopVar;
CHECK_OFFSET(GX2LoopVar, 0x00, offset);
CHECK_OFFSET(GX2LoopVar, 0x04, value);
CHECK_SIZE(GX2LoopVar, 0x08);

typedef struct _GX2VertexShader
{
    uint8_t* data(const void* basePtr) const
    {
        return (uint8_t*)((uintptr_t)basePtr + dataOffset);
    }

    GX2UniformBlock* uniformBlocks(const void* basePtr) const
    {
        return (GX2UniformBlock*)((uintptr_t)basePtr + uniformBlocksOffset);
    }

    GX2UniformVar* uniformVars(const void* basePtr) const
    {
        return (GX2UniformVar*)((uintptr_t)basePtr + uniformVarsOffset);
    }

    GX2LoopVar* loopVars(const void* basePtr) const
    {
        return (GX2LoopVar*)((uintptr_t)basePtr + loopVarsOffset);
    }

    GX2SamplerVar* samplerVars(const void* basePtr) const
    {
        return (GX2SamplerVar*)((uintptr_t)basePtr + samplerVarsOffset);
    }

    GX2AttribVar* attribVars(const void* basePtr) const
    {
        return (GX2AttribVar*)((uintptr_t)basePtr + attribVarsOffset);
    }

    struct
    {
        latte::SQ_PGM_RESOURCES_VS sq_pgm_resources_vs;
        latte::VGT_PRIMITIVEID_EN vgt_primitiveid_en;
        latte::SPI_VS_OUT_CONFIG spi_vs_out_config;
        uint32_t num_spi_vs_out_id;
        latte::SPI_VS_OUT_ID_N spi_vs_out_id[10];
        latte::PA_CL_VS_OUT_CNTL pa_cl_vs_out_cntl;
        latte::SQ_VTX_SEMANTIC_CLEAR sq_vtx_semantic_clear;
        uint32_t num_sq_vtx_semantic;
        latte::SQ_VTX_SEMANTIC_N sq_vtx_semantic[32];
        latte::VGT_STRMOUT_BUFFER_EN vgt_strmout_buffer_en;
        latte::VGT_VERTEX_REUSE_BLOCK_CNTL vgt_vertex_reuse_block_cntl;
        latte::VGT_HOS_REUSE_DEPTH vgt_hos_reuse_depth;
    } regs;

    uint32_t size;
    uint32_t dataOffset; // uint8_t*
    cafe::gx2::GX2ShaderMode mode;

    uint32_t uniformBlockCount;
    uint32_t uniformBlocksOffset; // GX2UniformBlock*

    uint32_t uniformVarCount;
    uint32_t uniformVarsOffset; // GX2UniformVar*

    /* uint32_t initialValueCount */ uint32_t unused0;
    /* GX2UniformInitialValue* initialValues */ uint32_t unused1;

    uint32_t loopVarCount;
    uint32_t loopVarsOffset; // GX2LoopVar*

    uint32_t samplerVarCount;
    uint32_t samplerVarsOffset; // GX2SamplerVar*

    uint32_t attribVarCount;
    uint32_t attribVarsOffset; // GX2AttribVar*

    uint32_t ringItemsize;

    BOOL hasStreamOut;
    uint32_t streamOutStride[4];

    GX2RBuffer gx2rData;
} GX2VertexShader;
CHECK_OFFSET(GX2VertexShader, 0x00, regs.sq_pgm_resources_vs);
CHECK_OFFSET(GX2VertexShader, 0x04, regs.vgt_primitiveid_en);
CHECK_OFFSET(GX2VertexShader, 0x08, regs.spi_vs_out_config);
CHECK_OFFSET(GX2VertexShader, 0x0C, regs.num_spi_vs_out_id);
CHECK_OFFSET(GX2VertexShader, 0x10, regs.spi_vs_out_id);
CHECK_OFFSET(GX2VertexShader, 0x38, regs.pa_cl_vs_out_cntl);
CHECK_OFFSET(GX2VertexShader, 0x3C, regs.sq_vtx_semantic_clear);
CHECK_OFFSET(GX2VertexShader, 0x40, regs.num_sq_vtx_semantic);
CHECK_OFFSET(GX2VertexShader, 0x44, regs.sq_vtx_semantic);
CHECK_OFFSET(GX2VertexShader, 0xC4, regs.vgt_strmout_buffer_en);
CHECK_OFFSET(GX2VertexShader, 0xC8, regs.vgt_vertex_reuse_block_cntl);
CHECK_OFFSET(GX2VertexShader, 0xCC, regs.vgt_hos_reuse_depth);
CHECK_OFFSET(GX2VertexShader, 0xD0, size);
CHECK_OFFSET(GX2VertexShader, 0xD4, dataOffset);
CHECK_OFFSET(GX2VertexShader, 0xD8, mode);
CHECK_OFFSET(GX2VertexShader, 0xDC, uniformBlockCount);
CHECK_OFFSET(GX2VertexShader, 0xE0, uniformBlocksOffset);
CHECK_OFFSET(GX2VertexShader, 0xE4, uniformVarCount);
CHECK_OFFSET(GX2VertexShader, 0xE8, uniformVarsOffset);
CHECK_OFFSET(GX2VertexShader, 0xEC, /* initialValueCount */ unused0);
CHECK_OFFSET(GX2VertexShader, 0xF0, /* initialValues */ unused1);
CHECK_OFFSET(GX2VertexShader, 0xF4, loopVarCount);
CHECK_OFFSET(GX2VertexShader, 0xF8, loopVarsOffset);
CHECK_OFFSET(GX2VertexShader, 0xFC, samplerVarCount);
CHECK_OFFSET(GX2VertexShader, 0x100, samplerVarsOffset);
CHECK_OFFSET(GX2VertexShader, 0x104, attribVarCount);
CHECK_OFFSET(GX2VertexShader, 0x108, attribVarsOffset);
CHECK_OFFSET(GX2VertexShader, 0x10C, ringItemsize);
CHECK_OFFSET(GX2VertexShader, 0x110, hasStreamOut);
CHECK_OFFSET(GX2VertexShader, 0x114, streamOutStride);
CHECK_OFFSET(GX2VertexShader, 0x124, gx2rData);
CHECK_SIZE(GX2VertexShader, 0x134);

typedef struct _GX2PixelShader
{
    uint8_t* data(const void* basePtr) const
    {
        return (uint8_t*)((uintptr_t)basePtr + dataOffset);
    }

    GX2UniformBlock* uniformBlocks(const void* basePtr) const
    {
        return (GX2UniformBlock*)((uintptr_t)basePtr + uniformBlocksOffset);
    }

    GX2UniformVar* uniformVars(const void* basePtr) const
    {
        return (GX2UniformVar*)((uintptr_t)basePtr + uniformVarsOffset);
    }

    GX2LoopVar* loopVars(const void* basePtr) const
    {
        return (GX2LoopVar*)((uintptr_t)basePtr + loopVarsOffset);
    }

    GX2SamplerVar* samplerVars(const void* basePtr) const
    {
        return (GX2SamplerVar*)((uintptr_t)basePtr + samplerVarsOffset);
    }

    struct
    {
        latte::SQ_PGM_RESOURCES_PS sq_pgm_resources_ps;
        latte::SQ_PGM_EXPORTS_PS sq_pgm_exports_ps;
        latte::SPI_PS_IN_CONTROL_0 spi_ps_in_control_0;
        latte::SPI_PS_IN_CONTROL_1 spi_ps_in_control_1;
        uint32_t num_spi_ps_input_cntl;
        latte::SPI_PS_INPUT_CNTL_N spi_ps_input_cntls[32];
        latte::CB_SHADER_MASK cb_shader_mask;
        latte::CB_SHADER_CONTROL cb_shader_control;
        latte::DB_SHADER_CONTROL db_shader_control;
        latte::SPI_INPUT_Z spi_input_z;
    } regs;

    uint32_t size;
    uint32_t dataOffset; // uint8_t*
    cafe::gx2::GX2ShaderMode mode;

    uint32_t uniformBlockCount;
    uint32_t uniformBlocksOffset; // GX2UniformBlock*

    uint32_t uniformVarCount;
    uint32_t uniformVarsOffset; // GX2UniformVar*

    /* uint32_t initialValueCount */ uint32_t unused0;
    /* GX2UniformInitialValue* initialValues */ uint32_t unused1;

    uint32_t loopVarCount;
    uint32_t loopVarsOffset; // GX2LoopVar*

    uint32_t samplerVarCount;
    uint32_t samplerVarsOffset; // GX2SamplerVar*

    GX2RBuffer gx2rData;
} GX2PixelShader;
CHECK_OFFSET(GX2PixelShader, 0x00, regs.sq_pgm_resources_ps);
CHECK_OFFSET(GX2PixelShader, 0x04, regs.sq_pgm_exports_ps);
CHECK_OFFSET(GX2PixelShader, 0x08, regs.spi_ps_in_control_0);
CHECK_OFFSET(GX2PixelShader, 0x0C, regs.spi_ps_in_control_1);
CHECK_OFFSET(GX2PixelShader, 0x10, regs.num_spi_ps_input_cntl);
CHECK_OFFSET(GX2PixelShader, 0x14, regs.spi_ps_input_cntls);
CHECK_OFFSET(GX2PixelShader, 0x94, regs.cb_shader_mask);
CHECK_OFFSET(GX2PixelShader, 0x98, regs.cb_shader_control);
CHECK_OFFSET(GX2PixelShader, 0x9C, regs.db_shader_control);
CHECK_OFFSET(GX2PixelShader, 0xA0, regs.spi_input_z);
CHECK_OFFSET(GX2PixelShader, 0xA4, size);
CHECK_OFFSET(GX2PixelShader, 0xA8, dataOffset);
CHECK_OFFSET(GX2PixelShader, 0xAC, mode);
CHECK_OFFSET(GX2PixelShader, 0xB0, uniformBlockCount);
CHECK_OFFSET(GX2PixelShader, 0xB4, uniformBlocksOffset);
CHECK_OFFSET(GX2PixelShader, 0xB8, uniformVarCount);
CHECK_OFFSET(GX2PixelShader, 0xBC, uniformVarsOffset);
CHECK_OFFSET(GX2PixelShader, 0xC0, /* initialValueCount */ unused0);
CHECK_OFFSET(GX2PixelShader, 0xC4, /* initialValues */ unused1);
CHECK_OFFSET(GX2PixelShader, 0xC8, loopVarCount);
CHECK_OFFSET(GX2PixelShader, 0xCC, loopVarsOffset);
CHECK_OFFSET(GX2PixelShader, 0xD0, samplerVarCount);
CHECK_OFFSET(GX2PixelShader, 0xD4, samplerVarsOffset);
CHECK_OFFSET(GX2PixelShader, 0xD8, gx2rData);
CHECK_SIZE(GX2PixelShader, 0xe8);

typedef struct _GX2GeometryShader
{
    uint8_t* data(const void* basePtr) const
    {
        return (uint8_t*)((uintptr_t)basePtr + dataOffset);
    }

    uint8_t* vertexShaderData(const void* basePtr) const
    {
        return (uint8_t*)((uintptr_t)basePtr + vertexShaderDataOffset);
    }

    GX2UniformBlock* uniformBlocks(const void* basePtr) const
    {
        return (GX2UniformBlock*)((uintptr_t)basePtr + uniformBlocksOffset);
    }

    GX2UniformVar* uniformVars(const void* basePtr) const
    {
        return (GX2UniformVar*)((uintptr_t)basePtr + uniformVarsOffset);
    }

    GX2LoopVar* loopVars(const void* basePtr) const
    {
        return (GX2LoopVar*)((uintptr_t)basePtr + loopVarsOffset);
    }

    GX2SamplerVar* samplerVars(const void* basePtr) const
    {
        return (GX2SamplerVar*)((uintptr_t)basePtr + samplerVarsOffset);
    }

    struct
    {
        latte::SQ_PGM_RESOURCES_GS sq_pgm_resources_gs;
        latte::VGT_GS_OUT_PRIM_TYPE vgt_gs_out_prim_type;
        latte::VGT_GS_MODE vgt_gs_mode;
        latte::PA_CL_VS_OUT_CNTL pa_cl_vs_out_cntl;
        latte::SQ_PGM_RESOURCES_VS sq_pgm_resources_vs;
        latte::SQ_GS_VERT_ITEMSIZE sq_gs_vert_itemsize;
        latte::SPI_VS_OUT_CONFIG spi_vs_out_config;
        uint32_t num_spi_vs_out_id;
        latte::SPI_VS_OUT_ID_N spi_vs_out_id[10];
        latte::VGT_STRMOUT_BUFFER_EN vgt_strmout_buffer_en;
    } regs;

    uint32_t size;
    uint32_t dataOffset; // uint8_t*
    uint32_t vertexShaderSize;
    uint32_t vertexShaderDataOffset; // uint8_t*
    cafe::gx2::GX2ShaderMode mode;

    uint32_t uniformBlockCount;
    uint32_t uniformBlocksOffset; // GX2UniformBlock*

    uint32_t uniformVarCount;
    uint32_t uniformVarsOffset; // GX2UniformVar*

    /* uint32_t initialValueCount */ uint32_t unused0;
    /* GX2UniformInitialValue* initialValues */ uint32_t unused1;

    uint32_t loopVarCount;
    uint32_t loopVarsOffset; // GX2LoopVar*

    uint32_t samplerVarCount;
    uint32_t samplerVarsOffset; // GX2SamplerVar*

    uint32_t ringItemSize;
    BOOL hasStreamOut;
    uint32_t streamOutStride[4];

    GX2RBuffer gx2rData;
    GX2RBuffer gx2rVertexShaderData;
} GX2GeometryShader;
CHECK_OFFSET(GX2GeometryShader, 0x00, regs.sq_pgm_resources_gs);
CHECK_OFFSET(GX2GeometryShader, 0x04, regs.vgt_gs_out_prim_type);
CHECK_OFFSET(GX2GeometryShader, 0x08, regs.vgt_gs_mode);
CHECK_OFFSET(GX2GeometryShader, 0x0C, regs.pa_cl_vs_out_cntl);
CHECK_OFFSET(GX2GeometryShader, 0x10, regs.sq_pgm_resources_vs);
CHECK_OFFSET(GX2GeometryShader, 0x14, regs.sq_gs_vert_itemsize);
CHECK_OFFSET(GX2GeometryShader, 0x18, regs.spi_vs_out_config);
CHECK_OFFSET(GX2GeometryShader, 0x1C, regs.num_spi_vs_out_id);
CHECK_OFFSET(GX2GeometryShader, 0x20, regs.spi_vs_out_id);
CHECK_OFFSET(GX2GeometryShader, 0x48, regs.vgt_strmout_buffer_en);
CHECK_OFFSET(GX2GeometryShader, 0x4C, size);
CHECK_OFFSET(GX2GeometryShader, 0x50, dataOffset);
CHECK_OFFSET(GX2GeometryShader, 0x54, vertexShaderSize);
CHECK_OFFSET(GX2GeometryShader, 0x58, vertexShaderDataOffset);
CHECK_OFFSET(GX2GeometryShader, 0x5C, mode);
CHECK_OFFSET(GX2GeometryShader, 0x60, uniformBlockCount);
CHECK_OFFSET(GX2GeometryShader, 0x64, uniformBlocksOffset);
CHECK_OFFSET(GX2GeometryShader, 0x68, uniformVarCount);
CHECK_OFFSET(GX2GeometryShader, 0x6C, uniformVarsOffset);
CHECK_OFFSET(GX2GeometryShader, 0x70, /* initialValueCount */ unused0);
CHECK_OFFSET(GX2GeometryShader, 0x74, /* initialValues */ unused1);
CHECK_OFFSET(GX2GeometryShader, 0x78, loopVarCount);
CHECK_OFFSET(GX2GeometryShader, 0x7C, loopVarsOffset);
CHECK_OFFSET(GX2GeometryShader, 0x80, samplerVarCount);
CHECK_OFFSET(GX2GeometryShader, 0x84, samplerVarsOffset);
CHECK_OFFSET(GX2GeometryShader, 0x88, ringItemSize);
CHECK_OFFSET(GX2GeometryShader, 0x8C, hasStreamOut);
CHECK_OFFSET(GX2GeometryShader, 0x90, streamOutStride);
CHECK_OFFSET(GX2GeometryShader, 0xA0, gx2rData);
CHECK_OFFSET(GX2GeometryShader, 0xB0, gx2rVertexShaderData);
CHECK_SIZE(GX2GeometryShader, 0xC0);

typedef struct _GX2AttribStream
{
    uint32_t location;
    uint32_t buffer;
    uint32_t offset;
    cafe::gx2::GX2AttribFormat format;
    cafe::gx2::GX2AttribIndexType type;
    uint32_t aluDivisor;
    uint32_t mask;
    cafe::gx2::GX2EndianSwapMode endianSwap;
    int32_t nameIdx;
} GX2AttribStream;
CHECK_OFFSET(GX2AttribStream, 0x0, location);
CHECK_OFFSET(GX2AttribStream, 0x4, buffer);
CHECK_OFFSET(GX2AttribStream, 0x8, offset);
CHECK_OFFSET(GX2AttribStream, 0xC, format);
CHECK_OFFSET(GX2AttribStream, 0x10, type);
CHECK_OFFSET(GX2AttribStream, 0x14, aluDivisor);
CHECK_OFFSET(GX2AttribStream, 0x18, mask);
CHECK_OFFSET(GX2AttribStream, 0x1C, endianSwap);
CHECK_OFFSET(GX2AttribStream, 0x20, nameIdx);
CHECK_SIZE(GX2AttribStream, 0x24);

typedef struct _GX2FetchShader
{
    cafe::gx2::GX2FetchShaderType type;

    struct
    {
        latte::SQ_PGM_RESOURCES_FS sq_pgm_resources_fs;
    } regs;

    uint32_t size;
    uint8_t* data; // Struct not serialized, so, we can just use a pointer here
    uint32_t attribCount;
    uint32_t numDivisors;
    uint32_t divisors[2];
} GX2FetchShader;
CHECK_OFFSET(GX2FetchShader, 0x0, type);
CHECK_OFFSET(GX2FetchShader, 0x4, regs.sq_pgm_resources_fs);
CHECK_OFFSET(GX2FetchShader, 0x8, size);
static const size_t ptr_size = sizeof(uint8_t*);
static const size_t offset_data = ((0xC - 1) | (ptr_size - 1)) + 1;
CHECK_OFFSET(GX2FetchShader, offset_data, data);
CHECK_OFFSET(GX2FetchShader, (offset_data + ptr_size), attribCount);
CHECK_OFFSET(GX2FetchShader, (offset_data + ptr_size + 0x4), numDivisors);
CHECK_OFFSET(GX2FetchShader, (offset_data + ptr_size + 0x8), divisors);
CHECK_SIZE(GX2FetchShader, (offset_data + ptr_size + 0x10));

uint32_t GX2CalcFetchShaderSize(uint32_t attribCount);
void GX2InitFetchShader(GX2FetchShader* fetchShader, uint8_t* buffer, uint32_t attribCount, const GX2AttribStream* attribs);

forceinline void GX2InitAttribStream(GX2AttribStream* attrib, uint32_t location, uint32_t buffer, uint32_t offset, cafe::gx2::GX2AttribFormat format, int32_t nameIdx=-1)
{
    assert(attrib != NULL);
    assert(buffer < 16 && "Cannot have more than 16 vertex attribute buffers!");

    switch (format & 0x1F)
    {
    case cafe::gx2::GX2AttribFormatType::TYPE_8:
    case cafe::gx2::GX2AttribFormatType::TYPE_16:
    case cafe::gx2::GX2AttribFormatType::TYPE_16_FLOAT:
    case cafe::gx2::GX2AttribFormatType::TYPE_32:
    case cafe::gx2::GX2AttribFormatType::TYPE_32_FLOAT:
        attrib->mask = 0x00040405; // X001
        break;
    case cafe::gx2::GX2AttribFormatType::TYPE_4_4:
    case cafe::gx2::GX2AttribFormatType::TYPE_8_8:
    case cafe::gx2::GX2AttribFormatType::TYPE_16_16:
    case cafe::gx2::GX2AttribFormatType::TYPE_16_16_FLOAT:
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32:
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_FLOAT:
        attrib->mask = 0x00010405; // XY01
        break;
    case cafe::gx2::GX2AttribFormatType::TYPE_10_11_11_FLOAT:
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_32:
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_32_FLOAT:
        attrib->mask = 0x00010205; // XYZ1
        break;
    case cafe::gx2::GX2AttribFormatType::TYPE_8_8_8_8:
    case cafe::gx2::GX2AttribFormatType::TYPE_10_10_10_2:
    case cafe::gx2::GX2AttribFormatType::TYPE_16_16_16_16:
    case cafe::gx2::GX2AttribFormatType::TYPE_16_16_16_16_FLOAT:
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_32_32:
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_32_32_FLOAT:
        attrib->mask = 0x00010203; // XYZW
        break;
    default:
        assert(!"Invalid GX2AttribFormat");
        attrib->mask = 0x04040405;
    }

    attrib->location = location;
    attrib->buffer = buffer;
    attrib->offset = offset;
    attrib->format = format;
    attrib->type = cafe::gx2::GX2AttribIndexType::PerVertex;
    attrib->aluDivisor = 0;
    attrib->endianSwap = cafe::gx2::GX2EndianSwapMode::Default;
    attrib->nameIdx = nameIdx;
}

forceinline cafe::gx2::GX2AttribFormat GX2ShaderVarTypeToAttribFormat(cafe::gx2::GX2ShaderVarType type)
{
    switch (type)
    {
    case cafe::gx2::GX2ShaderVarType::Bool:
    case cafe::gx2::GX2ShaderVarType::Uint:
        return cafe::gx2::GX2AttribFormat::UINT_32;
    case cafe::gx2::GX2ShaderVarType::Int:
        return cafe::gx2::GX2AttribFormat::SINT_32;
    case cafe::gx2::GX2ShaderVarType::Float:
        return cafe::gx2::GX2AttribFormat::FLOAT_32;
    case cafe::gx2::GX2ShaderVarType::Float2:
    case cafe::gx2::GX2ShaderVarType::Float2x2:
    case cafe::gx2::GX2ShaderVarType::Float3x2:
    case cafe::gx2::GX2ShaderVarType::Float4x2:
        return cafe::gx2::GX2AttribFormat::FLOAT_32_32;
    case cafe::gx2::GX2ShaderVarType::Float3:
    case cafe::gx2::GX2ShaderVarType::Float2x3:
    case cafe::gx2::GX2ShaderVarType::Float3x3:
    case cafe::gx2::GX2ShaderVarType::Float4x3:
        return cafe::gx2::GX2AttribFormat::FLOAT_32_32_32;
    case cafe::gx2::GX2ShaderVarType::Float4:
    case cafe::gx2::GX2ShaderVarType::Float2x4:
    case cafe::gx2::GX2ShaderVarType::Float3x4:
    case cafe::gx2::GX2ShaderVarType::Float4x4:
        return cafe::gx2::GX2AttribFormat::FLOAT_32_32_32_32;
    case cafe::gx2::GX2ShaderVarType::Bool2:
    case cafe::gx2::GX2ShaderVarType::Uint2:
        return cafe::gx2::GX2AttribFormat::UINT_32_32;
    case cafe::gx2::GX2ShaderVarType::Bool3:
    case cafe::gx2::GX2ShaderVarType::Uint3:
        return cafe::gx2::GX2AttribFormat::UINT_32_32_32;
    case cafe::gx2::GX2ShaderVarType::Bool4:
    case cafe::gx2::GX2ShaderVarType::Uint4:
        return cafe::gx2::GX2AttribFormat::UINT_32_32_32_32;
    case cafe::gx2::GX2ShaderVarType::Int2:
        return cafe::gx2::GX2AttribFormat::SINT_32_32;
    case cafe::gx2::GX2ShaderVarType::Int3:
        return cafe::gx2::GX2AttribFormat::SINT_32_32_32;
    case cafe::gx2::GX2ShaderVarType::Int4:
        return cafe::gx2::GX2AttribFormat::SINT_32_32_32_32;

    case cafe::gx2::GX2ShaderVarType::Void:
    case cafe::gx2::GX2ShaderVarType::Double:
    case cafe::gx2::GX2ShaderVarType::Double2:
    case cafe::gx2::GX2ShaderVarType::Double3:
    case cafe::gx2::GX2ShaderVarType::Double4:
    case cafe::gx2::GX2ShaderVarType::Double2x2:
    case cafe::gx2::GX2ShaderVarType::Double2x3:
    case cafe::gx2::GX2ShaderVarType::Double2x4:
    case cafe::gx2::GX2ShaderVarType::Double3x2:
    case cafe::gx2::GX2ShaderVarType::Double3x3:
    case cafe::gx2::GX2ShaderVarType::Double3x4:
    case cafe::gx2::GX2ShaderVarType::Double4x2:
    case cafe::gx2::GX2ShaderVarType::Double4x3:
    case cafe::gx2::GX2ShaderVarType::Double4x4:
    default:
        assert(!"Unsupported/Invalid GX2ShaderVarType");
        return cafe::gx2::GX2AttribFormat::UNORM_8;
    }
}

forceinline uint32_t GX2ShaderVarTypeToStreamCount(cafe::gx2::GX2ShaderVarType type)
{
    switch (type)
    {
    case cafe::gx2::GX2ShaderVarType::Bool:
    case cafe::gx2::GX2ShaderVarType::Int:
    case cafe::gx2::GX2ShaderVarType::Uint:
    case cafe::gx2::GX2ShaderVarType::Float:
    case cafe::gx2::GX2ShaderVarType::Float2:
    case cafe::gx2::GX2ShaderVarType::Float3:
    case cafe::gx2::GX2ShaderVarType::Float4:
    case cafe::gx2::GX2ShaderVarType::Bool2:
    case cafe::gx2::GX2ShaderVarType::Bool3:
    case cafe::gx2::GX2ShaderVarType::Bool4:
    case cafe::gx2::GX2ShaderVarType::Int2:
    case cafe::gx2::GX2ShaderVarType::Uint2:
    case cafe::gx2::GX2ShaderVarType::Int3:
    case cafe::gx2::GX2ShaderVarType::Uint3:
    case cafe::gx2::GX2ShaderVarType::Int4:
    case cafe::gx2::GX2ShaderVarType::Uint4:
        return 1;
    case cafe::gx2::GX2ShaderVarType::Float2x2:
    case cafe::gx2::GX2ShaderVarType::Float2x3:
    case cafe::gx2::GX2ShaderVarType::Float2x4:
        return 2;
    case cafe::gx2::GX2ShaderVarType::Float3x2:
    case cafe::gx2::GX2ShaderVarType::Float3x3:
    case cafe::gx2::GX2ShaderVarType::Float3x4:
        return 3;
    case cafe::gx2::GX2ShaderVarType::Float4x2:
    case cafe::gx2::GX2ShaderVarType::Float4x3:
    case cafe::gx2::GX2ShaderVarType::Float4x4:
        return 4;

    case cafe::gx2::GX2ShaderVarType::Void:
    case cafe::gx2::GX2ShaderVarType::Double:
    case cafe::gx2::GX2ShaderVarType::Double2:
    case cafe::gx2::GX2ShaderVarType::Double3:
    case cafe::gx2::GX2ShaderVarType::Double4:
    case cafe::gx2::GX2ShaderVarType::Double2x2:
    case cafe::gx2::GX2ShaderVarType::Double2x3:
    case cafe::gx2::GX2ShaderVarType::Double2x4:
    case cafe::gx2::GX2ShaderVarType::Double3x2:
    case cafe::gx2::GX2ShaderVarType::Double3x3:
    case cafe::gx2::GX2ShaderVarType::Double3x4:
    case cafe::gx2::GX2ShaderVarType::Double4x2:
    case cafe::gx2::GX2ShaderVarType::Double4x3:
    case cafe::gx2::GX2ShaderVarType::Double4x4:
    default:
        assert(!"Unsupported/Invalid GX2ShaderVarType");
        return 0;
    }
}

forceinline uint32_t GX2AttribFormatByteSize(cafe::gx2::GX2AttribFormat format)
{
    switch (format & 0x1F)
    {
    case cafe::gx2::GX2AttribFormatType::TYPE_8:
    case cafe::gx2::GX2AttribFormatType::TYPE_4_4:
        return 1;
    case cafe::gx2::GX2AttribFormatType::TYPE_16:
    case cafe::gx2::GX2AttribFormatType::TYPE_16_FLOAT:
    case cafe::gx2::GX2AttribFormatType::TYPE_8_8:
        return 2;
    case cafe::gx2::GX2AttribFormatType::TYPE_32:
    case cafe::gx2::GX2AttribFormatType::TYPE_32_FLOAT:
    case cafe::gx2::GX2AttribFormatType::TYPE_16_16:
    case cafe::gx2::GX2AttribFormatType::TYPE_16_16_FLOAT:
    case cafe::gx2::GX2AttribFormatType::TYPE_10_11_11_FLOAT:
    case cafe::gx2::GX2AttribFormatType::TYPE_8_8_8_8:
    case cafe::gx2::GX2AttribFormatType::TYPE_10_10_10_2:
        return 4;
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32:
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_FLOAT:
    case cafe::gx2::GX2AttribFormatType::TYPE_16_16_16_16:
    case cafe::gx2::GX2AttribFormatType::TYPE_16_16_16_16_FLOAT:
        return 8;
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_32:
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_32_FLOAT:
        return 12;
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_32_32:
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_32_32_FLOAT:
        return 16;
    default:
        assert(!"Invalid GX2AttribFormat");
        return 0;
    }
}

forceinline const char* GX2ShaderVarTypeStr(cafe::gx2::GX2ShaderVarType type)
{
    switch (type)
    {
    case cafe::gx2::GX2ShaderVarType::Bool:
        return "cafe::gx2::GX2ShaderVarType::Bool";
    case cafe::gx2::GX2ShaderVarType::Int:
        return "cafe::gx2::GX2ShaderVarType::Int";
    case cafe::gx2::GX2ShaderVarType::Uint:
        return "cafe::gx2::GX2ShaderVarType::Uint";
    case cafe::gx2::GX2ShaderVarType::Float:
        return "cafe::gx2::GX2ShaderVarType::Float";
    case cafe::gx2::GX2ShaderVarType::Float2:
        return "cafe::gx2::GX2ShaderVarType::Float2";
    case cafe::gx2::GX2ShaderVarType::Float3:
        return "cafe::gx2::GX2ShaderVarType::Float3";
    case cafe::gx2::GX2ShaderVarType::Float4:
        return "cafe::gx2::GX2ShaderVarType::Float4";
    case cafe::gx2::GX2ShaderVarType::Bool2:
        return "cafe::gx2::GX2ShaderVarType::Bool2";
    case cafe::gx2::GX2ShaderVarType::Bool3:
        return "cafe::gx2::GX2ShaderVarType::Bool3";
    case cafe::gx2::GX2ShaderVarType::Bool4:
        return "cafe::gx2::GX2ShaderVarType::Bool4";
    case cafe::gx2::GX2ShaderVarType::Int2:
        return "cafe::gx2::GX2ShaderVarType::Int2";
    case cafe::gx2::GX2ShaderVarType::Uint2:
        return "cafe::gx2::GX2ShaderVarType::Uint2";
    case cafe::gx2::GX2ShaderVarType::Int3:
        return "cafe::gx2::GX2ShaderVarType::Int3";
    case cafe::gx2::GX2ShaderVarType::Uint3:
        return "cafe::gx2::GX2ShaderVarType::Uint3";
    case cafe::gx2::GX2ShaderVarType::Int4:
        return "cafe::gx2::GX2ShaderVarType::Int4";
    case cafe::gx2::GX2ShaderVarType::Uint4:
        return "cafe::gx2::GX2ShaderVarType::Uint4";
    case cafe::gx2::GX2ShaderVarType::Float2x2:
        return "cafe::gx2::GX2ShaderVarType::Float2x2";
    case cafe::gx2::GX2ShaderVarType::Float2x3:
        return "cafe::gx2::GX2ShaderVarType::Float2x3";
    case cafe::gx2::GX2ShaderVarType::Float2x4:
        return "cafe::gx2::GX2ShaderVarType::Float2x4";
    case cafe::gx2::GX2ShaderVarType::Float3x2:
        return "cafe::gx2::GX2ShaderVarType::Float3x2";
    case cafe::gx2::GX2ShaderVarType::Float3x3:
        return "cafe::gx2::GX2ShaderVarType::Float3x3";
    case cafe::gx2::GX2ShaderVarType::Float3x4:
        return "cafe::gx2::GX2ShaderVarType::Float3x4";
    case cafe::gx2::GX2ShaderVarType::Float4x2:
        return "cafe::gx2::GX2ShaderVarType::Float4x2";
    case cafe::gx2::GX2ShaderVarType::Float4x3:
        return "cafe::gx2::GX2ShaderVarType::Float4x3";
    case cafe::gx2::GX2ShaderVarType::Float4x4:
        return "cafe::gx2::GX2ShaderVarType::Float4x4";

    case cafe::gx2::GX2ShaderVarType::Void:
    case cafe::gx2::GX2ShaderVarType::Double:
    case cafe::gx2::GX2ShaderVarType::Double2:
    case cafe::gx2::GX2ShaderVarType::Double3:
    case cafe::gx2::GX2ShaderVarType::Double4:
    case cafe::gx2::GX2ShaderVarType::Double2x2:
    case cafe::gx2::GX2ShaderVarType::Double2x3:
    case cafe::gx2::GX2ShaderVarType::Double2x4:
    case cafe::gx2::GX2ShaderVarType::Double3x2:
    case cafe::gx2::GX2ShaderVarType::Double3x3:
    case cafe::gx2::GX2ShaderVarType::Double3x4:
    case cafe::gx2::GX2ShaderVarType::Double4x2:
    case cafe::gx2::GX2ShaderVarType::Double4x3:
    case cafe::gx2::GX2ShaderVarType::Double4x4:
    default:
        assert(!"Unsupported/Invalid GX2ShaderVarType");
        return "";
    }
}

forceinline const char* GX2ShaderVarTypeToAttribFormatStr(cafe::gx2::GX2ShaderVarType type)
{
    switch (type)
    {
    case cafe::gx2::GX2ShaderVarType::Bool:
    case cafe::gx2::GX2ShaderVarType::Uint:
        return "cafe::gx2::GX2AttribFormat::UINT_32";
    case cafe::gx2::GX2ShaderVarType::Int:
        return "cafe::gx2::GX2AttribFormat::SINT_32";
    case cafe::gx2::GX2ShaderVarType::Float:
        return "cafe::gx2::GX2AttribFormat::FLOAT_32";
    case cafe::gx2::GX2ShaderVarType::Float2:
    case cafe::gx2::GX2ShaderVarType::Float2x2:
    case cafe::gx2::GX2ShaderVarType::Float3x2:
    case cafe::gx2::GX2ShaderVarType::Float4x2:
        return "cafe::gx2::GX2AttribFormat::FLOAT_32_32";
    case cafe::gx2::GX2ShaderVarType::Float3:
    case cafe::gx2::GX2ShaderVarType::Float2x3:
    case cafe::gx2::GX2ShaderVarType::Float3x3:
    case cafe::gx2::GX2ShaderVarType::Float4x3:
        return "cafe::gx2::GX2AttribFormat::FLOAT_32_32_32";
    case cafe::gx2::GX2ShaderVarType::Float4:
    case cafe::gx2::GX2ShaderVarType::Float2x4:
    case cafe::gx2::GX2ShaderVarType::Float3x4:
    case cafe::gx2::GX2ShaderVarType::Float4x4:
        return "cafe::gx2::GX2AttribFormat::FLOAT_32_32_32_32";
    case cafe::gx2::GX2ShaderVarType::Bool2:
    case cafe::gx2::GX2ShaderVarType::Uint2:
        return "cafe::gx2::GX2AttribFormat::UINT_32_32";
    case cafe::gx2::GX2ShaderVarType::Bool3:
    case cafe::gx2::GX2ShaderVarType::Uint3:
        return "cafe::gx2::GX2AttribFormat::UINT_32_32_32";
    case cafe::gx2::GX2ShaderVarType::Bool4:
    case cafe::gx2::GX2ShaderVarType::Uint4:
        return "cafe::gx2::GX2AttribFormat::UINT_32_32_32_32";
    case cafe::gx2::GX2ShaderVarType::Int2:
        return "cafe::gx2::GX2AttribFormat::SINT_32_32";
    case cafe::gx2::GX2ShaderVarType::Int3:
        return "cafe::gx2::GX2AttribFormat::SINT_32_32_32";
    case cafe::gx2::GX2ShaderVarType::Int4:
        return "cafe::gx2::GX2AttribFormat::SINT_32_32_32_32";

    case cafe::gx2::GX2ShaderVarType::Void:
    case cafe::gx2::GX2ShaderVarType::Double:
    case cafe::gx2::GX2ShaderVarType::Double2:
    case cafe::gx2::GX2ShaderVarType::Double3:
    case cafe::gx2::GX2ShaderVarType::Double4:
    case cafe::gx2::GX2ShaderVarType::Double2x2:
    case cafe::gx2::GX2ShaderVarType::Double2x3:
    case cafe::gx2::GX2ShaderVarType::Double2x4:
    case cafe::gx2::GX2ShaderVarType::Double3x2:
    case cafe::gx2::GX2ShaderVarType::Double3x3:
    case cafe::gx2::GX2ShaderVarType::Double3x4:
    case cafe::gx2::GX2ShaderVarType::Double4x2:
    case cafe::gx2::GX2ShaderVarType::Double4x3:
    case cafe::gx2::GX2ShaderVarType::Double4x4:
    default:
        assert(!"Unsupported/Invalid GX2ShaderVarType");
        return "";
    }
}
