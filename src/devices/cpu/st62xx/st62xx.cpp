// license:BSD-3-Clause
// copyright-holders:Ryan Holtz
/**********************************************************************

    STmicro ST6-series microcontroller emulation skeleton

	To Do:
		- Cycle counts
		- STOP, WAIT opcodes
		- Peripherals

**********************************************************************/

#include "emu.h"
#include "st62xx.h"
#include "st62xx_dasm.h"

DEFINE_DEVICE_TYPE(ST6228,   st6228_device,   "st6228",   "STmicro ST6228")

void st6228_device::st6228_program_map(address_map &map)
{
	map(0x000, 0x7ff).bankr("program_rombank");
	map(0x800, 0xfff).rom().region(tag(), 0x800);
}

void st6228_device::st6228_data_map(address_map &map)
{
	map(0x00, 0x3f).bankrw("rambank");
	map(0x40, 0x7f).bankr("data_rombank");
	map(0x80, 0xff).rw(FUNC(st6228_device::regs_r), FUNC(st6228_device::regs_w));
}

st6228_device::st6228_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: cpu_device(mconfig, ST6228, tag, owner, clock)
	, m_pc(0)
	, m_mode(MODE_NMI)
	, m_prev_mode(MODE_NORMAL)
	, m_program_config("program", ENDIANNESS_LITTLE, 8, 12, 0, address_map_constructor(FUNC(st6228_device::st6228_program_map), this))
	, m_data_config("data", ENDIANNESS_LITTLE, 8, 8, 0, address_map_constructor(FUNC(st6228_device::st6228_data_map), this))
	, m_program(nullptr)
	, m_data(nullptr)
	, m_rambank(*this, "^rambank")
	, m_program_rombank(*this, "^program_rombank")
	, m_data_rombank(*this, "^data_rombank")
	, m_rom(*this, this->tag())
{
}

void st6228_device::unimplemented_opcode(uint8_t op)
{
	fatalerror("ST62xx: unknown opcode (%02x) at %04x\n", op, m_pc);
}

void st6228_device::device_start()
{
	m_pc = 0;

	m_program = &space(AS_PROGRAM);
	m_data = &space(AS_DATA);

	// register our state for the debugger
	state_add(STATE_GENPC,     "GENPC",    m_pc).noshow();
	state_add(STATE_GENPCBASE, "CURPC",    m_pc).noshow();
	state_add(STATE_GENFLAGS,  "GENFLAGS", m_flags[0]).callimport().callexport().formatstr("%6s").noshow();
	state_add(STATE_FLAGS,     "FLAGS",    m_flags[0]).mask(0x3f);
	state_add(STATE_PC,        "PC",       m_pc).mask(0xfff);
	state_add(STATE_SP,        "SP",       m_stack_index).mask(0x7);
	state_add(STATE_STACK,     "STACK",    m_stack[0]).callimport().callexport().formatstr("%04X");
	state_add(STATE_A,         "A",        m_regs[REG_A]);
	state_add(STATE_X,         "X",        m_regs[REG_X]);
	state_add(STATE_Y,         "Y",        m_regs[REG_Y]);
	state_add(STATE_V,         "V",        m_regs[REG_V]);
	state_add(STATE_W,         "W",        m_regs[REG_W]);

	save_item(NAME(m_regs));
	save_item(NAME(m_ram));
	save_item(NAME(m_pc));
	save_item(NAME(m_mode));
	save_item(NAME(m_prev_mode));
	save_item(NAME(m_flags));
	save_item(NAME(m_stack));
	save_item(NAME(m_stack_index));
	save_item(NAME(m_icount));

	// set our instruction counter
	set_icountptr(m_icount);

	m_rambank->configure_entries(0, 2, m_ram, 0x40);
	m_program_rombank->configure_entries(0, 2, m_rom, 0x800);
	m_data_rombank->configure_entries(0, 128, m_rom, 0x40);
}

void st6228_device::device_reset()
{
	std::fill(std::begin(m_regs), std::end(m_regs), 0);
	std::fill(std::begin(m_ram), std::end(m_ram), 0);
	std::fill(std::begin(m_stack), std::end(m_stack), 0);
	std::fill(std::begin(m_flags), std::end(m_flags), 0);

	m_pc = m_program->read_word(VEC_RESET);
	m_stack_index = 0;
	m_mode = MODE_NMI;
	m_prev_mode = MODE_NORMAL;

	m_rambank->set_entry(0);
	m_program_rombank->set_entry(0);
	m_data_rombank->set_entry(0);
}

device_memory_interface::space_config_vector st6228_device::memory_space_config() const
{
	return space_config_vector {
		std::make_pair(AS_PROGRAM, &m_program_config),
		std::make_pair(AS_DATA,    &m_data_config)
	};
}

void st6228_device::state_string_export(const device_state_entry &entry, std::string &str) const
{
	switch (entry.index())
	{
		case STATE_GENFLAGS:
			str = string_format("%c%c%c%c%c%c",
				(m_flags[2] & FLAG_C) ? 'C' : '.',
				(m_flags[2] & FLAG_Z) ? 'Z' : '.',
				(m_flags[1] & FLAG_C) ? 'C' : '.',
				(m_flags[1] & FLAG_Z) ? 'Z' : '.',
				(m_flags[0] & FLAG_C) ? 'C' : '.',
				(m_flags[0] & FLAG_Z) ? 'Z' : '.');
			break;
		case STATE_STACK:
			str = string_format("%04X", m_stack[m_stack_index]);
			break;
	}
}

std::unique_ptr<util::disasm_interface> st6228_device::create_disassembler()
{
	return std::make_unique<st62xx_disassembler>();
}

WRITE8_MEMBER(st6228_device::regs_w)
{
	offset += 0x80;

	switch (offset)
	{
		case REG_X:
		case REG_Y:
		case REG_V:
		case REG_W:
		case REG_A:
			m_regs[offset] = data;
			break;
		default:
			logerror("%s: Unknown register write: %02x = %02x\n", machine().describe_context(), offset, data);
			break;
	}
}

READ8_MEMBER(st6228_device::regs_r)
{
	uint8_t ret = 0;
	offset += 0x80;

	switch (offset)
	{
		case REG_X:
		case REG_Y:
		case REG_V:
		case REG_W:
		case REG_A:
			ret = m_regs[offset];
			break;

		default:
			logerror("%s: Unknown register read: %02x\n", machine().describe_context(), offset);
			break;
	}
	return ret;
}

uint32_t st6228_device::execute_min_cycles() const
{
	return 2;
}

uint32_t st6228_device::execute_max_cycles() const
{
	return 5;
}

uint32_t st6228_device::execute_input_lines() const
{
	return 0;
}

void st6228_device::execute_set_input(int inputnum, int state)
{
	logerror("%s: Unimplemented: execute_set_input line %d = %d\n", machine().describe_context(), inputnum, state);
}

void st6228_device::tick_timers(int cycles)
{
}

void st6228_device::execute_run()
{
	while (m_icount > 0)
	{
		debugger_instruction_hook(m_pc);

		uint8_t op = m_program->read_byte(m_pc);
		m_pc++;

		int cycles = 4;

		switch (op)
		{
			case 0x00: case 0x10: case 0x20: case 0x30: case 0x40: case 0x50: case 0x60: case 0x70:
			case 0x80:            case 0xa0: case 0xb0: case 0xc0: case 0xd0: case 0xe0: case 0xf0:
			case 0x08: case 0x18: case 0x28: case 0x38: case 0x48: case 0x58: case 0x68: case 0x78:
			case 0x88:            case 0xa8: case 0xb8: case 0xc8: case 0xd8: case 0xe8: case 0xf8:	// JRNZ e
			{
				const int8_t e = ((int8_t)op) >> 3;
				if (!(m_flags[m_mode] & FLAG_Z))
					m_pc += e;
				break;
			}
			case 0x01: case 0x11: case 0x21: case 0x31: case 0x41: case 0x51: case 0x61: case 0x71:
			case 0x81: case 0x91: case 0xa1: case 0xb1: case 0xc1: case 0xd1: case 0xe1: case 0xf1: // CALL abc
			{
				const uint8_t ab = m_program->read_byte(m_pc);
				m_pc++;
				const uint16_t abc = ((op & 0xf0) >> 4) | (ab << 4);
				if (m_stack_index < 6) // FIXME: magic numbers
				{
					m_stack[m_stack_index] = m_pc;
					m_stack_index++;
				}
				else
				{
					// Per documentation: "If more calls [than the maximum] are nested, the latest stacked PC
					//                     values will be lost. In this case, returns will return to the PC
					//                     values stacked first."
				}
				m_pc = abc;
				break;
			}
			case 0x09: case 0x19: case 0x29: case 0x39: case 0x49: case 0x59: case 0x69: case 0x79:
			case 0x89: case 0x99: case 0xa9: case 0xb9: case 0xc9: case 0xd9: case 0xe9: case 0xf9:	// JP abc
			{
				const uint8_t ab = m_program->read_byte(m_pc);
				m_pc++;
				const uint16_t abc = ((op & 0xf0) >> 4) | (ab << 4);
				m_pc = abc;
				break;
			}
			case 0x02: case 0x12: case 0x22: case 0x32: case 0x42: case 0x52: case 0x62: case 0x72:
			case 0x82: case 0x92: case 0xa2: case 0xb2: case 0xc2: case 0xd2: case 0xe2: case 0xf2:
			case 0x0a: case 0x1a: case 0x2a: case 0x3a: case 0x4a: case 0x5a: case 0x6a: case 0x7a:
			case 0x8a: case 0x9a: case 0xaa: case 0xba: case 0xca: case 0xda: case 0xea: case 0xfa:	// JRNC abc
			{
				const int8_t e = ((int8_t)op) >> 3;
				if (!(m_flags[m_mode] & FLAG_C))
					m_pc += e;
				break;
			}
			case 0x03: case 0x23: case 0x43: case 0x63: case 0x83: case 0xa3: case 0xc3: case 0xe3:	// JRR b,rr,ee
			{
				const uint8_t b = (op >> 5) & 7;
				const uint8_t rr = m_program->read_byte(m_pc+1);
				const int8_t ee = (int8_t)m_program->read_byte(m_pc+2);
				const uint8_t value = m_data->read_byte(rr);
				if (BIT(value, b))
					m_pc += 2;
				else
					m_pc += ee;
				break;
			}
			case 0x13: case 0x33: case 0x53: case 0x73: case 0x93: case 0xb3: case 0xd3: case 0xf3:	// JRS b,rr,ee
			{
				const uint8_t b = (op >> 5) & 7;
				const uint8_t rr = m_program->read_byte(m_pc+1);
				const int8_t ee = (int8_t)m_program->read_byte(m_pc+2);
				const uint8_t value = m_data->read_byte(rr);
				if (BIT(value, b))
					m_pc += ee;
				else
					m_pc += 2;
				break;
			}
			case 0x0b: case 0x2b: case 0x4b: case 0x6b: case 0x8b: case 0xab: case 0xcb: case 0xeb:	// RES b,rr
			{
				const uint8_t b = (op >> 5) & 7;
				const uint8_t rr = m_program->read_byte(m_pc);
				const uint8_t nn = m_data->read_byte(rr);
				m_data->write_byte(rr, nn & ~(1 << b));
				m_pc++;
				break;
			}
			case 0x1b: case 0x3b: case 0x5b: case 0x7b: case 0x9b: case 0xbb: case 0xdb: case 0xfb:	// SET b,rr
			{
				const uint8_t b = (op >> 5) & 7;
				const uint8_t rr = m_program->read_byte(m_pc);
				const uint8_t nn = m_data->read_byte(rr);
				m_data->write_byte(rr, nn | (1 << b));
				m_pc++;
				break;
			}
			case 0x04: case 0x14: case 0x24: case 0x34: case 0x44: case 0x54: case 0x64: case 0x74:
			case 0x84: case 0x94: case 0xa4: case 0xb4: case 0xc4: case 0xd4: case 0xe4: case 0xf4:
			case 0x0c: case 0x1c: case 0x2c: case 0x3c: case 0x4c: case 0x5c: case 0x6c: case 0x7c:
			case 0x8c: case 0x9c: case 0xac: case 0xbc: case 0xcc: case 0xdc: case 0xec: case 0xfc:	// JRZ e
			{
				const int8_t e = ((int8_t)op) >> 3;
				if (m_flags[m_mode] & FLAG_Z)
					m_pc += e;
				break;
			}
			case 0x06: case 0x16: case 0x26: case 0x36: case 0x46: case 0x56: case 0x66: case 0x76:
			case 0x86: case 0x96: case 0xa6: case 0xb6: case 0xc6: case 0xd6: case 0xe6: case 0xf6:
			case 0x0e: case 0x1e: case 0x2e: case 0x3e: case 0x4e: case 0x5e: case 0x6e: case 0x7e:
			case 0x8e: case 0x9e: case 0xae: case 0xbe: case 0xce: case 0xde: case 0xee: case 0xfe:	// JRC e
			{
				const int8_t e = ((int8_t)op) >> 3;
				if (m_flags[m_mode] & FLAG_C)
					m_pc += e;
				break;
			}
			case 0x15: // INC X
				m_regs[REG_X]++;

				if (m_regs[REG_X])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x35: // LD A,X
				m_regs[REG_A] = m_regs[REG_X];

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x55: // INC Y
				m_regs[REG_Y]++;

				if (m_regs[REG_Y])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x75: // LD A,Y
				m_regs[REG_A] = m_regs[REG_Y];

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x95: // INC V
				m_regs[REG_V]++;

				if (m_regs[REG_V])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0xb5: // LD A,V
				m_regs[REG_A] = m_regs[REG_V];

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0xd5: // INC W
				m_regs[REG_W]++;

				if (m_regs[REG_W])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0xf5: // LD A,W
				m_regs[REG_A] = m_regs[REG_W];

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x0d:	// LDI rr,nn
			{
				const uint8_t rr = m_program->read_byte(m_pc);
				const uint8_t nn = m_program->read_byte(m_pc);
				m_data->write_byte(rr, nn);

				if (nn)
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				m_pc += 2;
				break;
			}
			case 0x1d: // DEC X
				m_regs[REG_X]--;

				if (m_regs[REG_X])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x2d: // COM A
				if (BIT(m_regs[REG_A], 7))
					m_flags[m_mode] |= FLAG_C;
				else
					m_flags[m_mode] &= FLAG_C;

				m_regs[REG_A] = ~m_regs[REG_A];

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x3d: // LD X,A
				m_regs[REG_X] = m_regs[REG_A];

				if (m_regs[REG_X])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x4d:
				if (m_stack_index > 0)
				{
					m_stack_index--;
					m_pc = m_stack[m_stack_index];
					m_mode = m_prev_mode;
					m_prev_mode = MODE_NORMAL;
				}
				else
				{
					fatalerror("Attempted to RETI with nothing on the stack");
				}
				break;
			case 0x5d: // DEC Y
				m_regs[REG_Y]--;

				if (m_regs[REG_Y])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x6d:
				//util::stream_format(stream, "STOP");
				break;
			case 0x7d: // LD Y,A
				m_regs[REG_Y] = m_regs[REG_A];

				if (m_regs[REG_Y])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x9d: // DEC V
				m_regs[REG_V]--;

				if (m_regs[REG_V])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0xad: // RLC
			{
				const uint8_t old_c = (m_flags[m_mode] & FLAG_C) ? 1 : 0;
				if (BIT(m_regs[REG_A], 7))
					m_flags[m_mode] |= FLAG_C;
				else
					m_flags[m_mode] &= ~FLAG_C;
				m_regs[REG_A] = (m_regs[REG_A] << 1) | old_c;

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			}
			case 0xbd: // LD V,A
				m_regs[REG_V] = m_regs[REG_A];

				if (m_regs[REG_V])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0xcd:
				if (m_stack_index > 0)
				{
					m_stack_index--;
					m_pc = m_stack[m_stack_index];
				}
				else
				{
					fatalerror("Attempted to RET with nothing on the stack");
				}
				break;
			case 0xdd: // DEC W
				m_regs[REG_W]--;

				if (m_regs[REG_W])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0xed:
				//util::stream_format(stream, "WAIT");
				break;
			case 0xfd: // LD W,A
				m_regs[REG_W] = m_regs[REG_A];

				if (m_regs[REG_W])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x07: // LD A,(X)
				m_regs[REG_A] = m_data->read_byte(m_regs[REG_X]);

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x17:	// LDI A,rr
			{
				m_regs[REG_A] = m_program->read_byte(m_pc);

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				m_pc++;
				break;
			}
			case 0x27: // CP A,(X)
			{
				const uint8_t nn = m_data->read_byte(m_regs[REG_X]);

				if (m_regs[REG_A] < nn)
				{
					m_flags[m_mode] |= FLAG_C;
					m_flags[m_mode] &= ~FLAG_Z;
				}
				else
				{
					m_flags[m_mode] &= ~FLAG_C;
					if (m_regs[REG_A] == nn)
						m_flags[m_mode] |= FLAG_Z;
					else
						m_flags[m_mode] &= ~FLAG_Z;
				}
				break;
			}
			case 0x37: // CPI A,nn
			{
				const uint8_t nn = m_program->read_byte(m_pc);

				if (m_regs[REG_A] < nn)
				{
					m_flags[m_mode] |= FLAG_C;
					m_flags[m_mode] &= ~FLAG_Z;
				}
				else
				{
					m_flags[m_mode] &= ~FLAG_C;
					if (m_regs[REG_A] == nn)
						m_flags[m_mode] |= FLAG_Z;
					else
						m_flags[m_mode] &= ~FLAG_Z;
				}
				m_pc++;
				break;
			}
			case 0x47: // ADD A,(X)
			{
				const uint8_t nn = m_data->read_byte(m_regs[REG_X]);
				const uint16_t sum = m_regs[REG_A] + nn;

				if (sum > 0xff)
				{
					m_flags[m_mode] |= FLAG_C;
					m_flags[m_mode] &= ~FLAG_Z;
				}
				else
				{
					m_flags[m_mode] &= ~FLAG_C;
					if (sum == 0)
						m_flags[m_mode] |= FLAG_Z;
					else
						m_flags[m_mode] &= ~FLAG_Z;
				}
				m_regs[REG_A] = (uint8_t)sum;
				break;
			}
			case 0x57: // ADDI A,nn
			{
				const uint8_t nn = m_program->read_byte(m_pc);
				const uint16_t sum = m_regs[REG_A] + nn;

				if (sum > 0xff)
				{
					m_flags[m_mode] |= FLAG_C;
					m_flags[m_mode] &= ~FLAG_Z;
				}
				else
				{
					m_flags[m_mode] &= ~FLAG_C;
					if (sum == 0)
						m_flags[m_mode] |= FLAG_Z;
					else
						m_flags[m_mode] &= ~FLAG_Z;
				}
				m_regs[REG_A] = (uint8_t)sum;
				m_pc++;
				break;
			}
			case 0x67: // INC (X)
			{
				const uint8_t rr = m_data->read_byte(m_regs[REG_X]) + 1;
				m_data->write_byte(m_regs[REG_X], rr);

				if (rr)
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			}
			case 0x87: // LD (X),A
				m_data->write_byte(m_regs[REG_X], m_regs[REG_A]);

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0xa7: // AND A,(X)
				m_regs[REG_A] &= m_data->read_byte(m_regs[REG_X]);

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0xb7: // ANDI A,nn
			{
				m_regs[REG_A] &= m_program->read_byte(m_pc);

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				m_pc++;
				break;
			}
			case 0xc7: // SUB A,(X)
			{
				const uint8_t nn = m_data->read_byte(m_regs[REG_X]);

				if (m_regs[REG_A] < nn)
				{
					m_flags[m_mode] |= FLAG_C;
					m_flags[m_mode] &= ~FLAG_Z;
				}
				else
				{
					m_flags[m_mode] &= ~FLAG_C;
					if (m_regs[REG_A] == nn)
						m_flags[m_mode] |= FLAG_Z;
					else
						m_flags[m_mode] &= ~FLAG_Z;
				}
				m_regs[REG_A] -= nn;
				break;
			}
			case 0xd7: // SUBI A,nn
			{
				const uint8_t nn = m_program->read_byte(m_pc);

				if (m_regs[REG_A] < nn)
				{
					m_flags[m_mode] |= FLAG_C;
					m_flags[m_mode] &= ~FLAG_Z;
				}
				else
				{
					m_flags[m_mode] &= ~FLAG_C;
					if (m_regs[REG_A] == nn)
						m_flags[m_mode] |= FLAG_Z;
					else
						m_flags[m_mode] &= ~FLAG_Z;
				}
				m_regs[REG_A] -= nn;
				m_pc++;
				break;
			}
			case 0xe7: // DEC (X)
			{
				const uint8_t rr = m_data->read_byte(m_regs[REG_X]) - 1;
				m_data->write_byte(m_regs[REG_X], rr);

				if (rr)
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			}
			case 0x0f: // LD A,(Y)
				m_regs[REG_A] = m_data->read_byte(m_regs[REG_Y]);

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x1f: // LD A,rr
			{
				m_regs[REG_A] = m_data->read_byte(m_program->read_byte(m_pc));

				if (m_regs[REG_V])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				m_pc++;
				break;
			}
			case 0x2f: // CP A,(Y)
			{
				const uint8_t nn = m_data->read_byte(m_regs[REG_Y]);

				if (m_regs[REG_A] < nn)
				{
					m_flags[m_mode] |= FLAG_C;
					m_flags[m_mode] &= ~FLAG_Z;
				}
				else
				{
					m_flags[m_mode] &= ~FLAG_C;
					if (m_regs[REG_A] == nn)
						m_flags[m_mode] |= FLAG_Z;
					else
						m_flags[m_mode] &= ~FLAG_Z;
				}
				break;
			}
			case 0x3f: // CP A,rr
			{
				const uint8_t nn = m_data->read_byte(m_program->read_byte(m_pc));

				if (m_regs[REG_A] < nn)
				{
					m_flags[m_mode] |= FLAG_C;
					m_flags[m_mode] &= ~FLAG_Z;
				}
				else
				{
					m_flags[m_mode] &= ~FLAG_C;
					if (m_regs[REG_A] == nn)
						m_flags[m_mode] |= FLAG_Z;
					else
						m_flags[m_mode] &= ~FLAG_Z;
				}
				m_pc++;
				break;
			}
			case 0x4f: // ADD A,(Y)
			{
				const uint8_t nn = m_data->read_byte(m_regs[REG_Y]);
				const uint16_t sum = m_regs[REG_A] + nn;

				if (sum > 0xff)
				{
					m_flags[m_mode] |= FLAG_C;
					m_flags[m_mode] &= ~FLAG_Z;
				}
				else
				{
					m_flags[m_mode] &= ~FLAG_C;
					if (sum == 0)
						m_flags[m_mode] |= FLAG_Z;
					else
						m_flags[m_mode] &= ~FLAG_Z;
				}
				m_regs[REG_A] = (uint8_t)sum;
				break;
			}
			case 0x5f: // ADD A,rr
			{
				const uint8_t nn = m_data->read_byte(m_program->read_byte(m_pc));
				const uint16_t sum = m_regs[REG_A] + nn;

				if (sum > 0xff)
				{
					m_flags[m_mode] |= FLAG_C;
					m_flags[m_mode] &= ~FLAG_Z;
				}
				else
				{
					m_flags[m_mode] &= ~FLAG_C;
					if (sum == 0)
						m_flags[m_mode] |= FLAG_Z;
					else
						m_flags[m_mode] &= ~FLAG_Z;
				}
				m_regs[REG_A] = (uint8_t)sum;
				m_pc++;
				break;
			}
			case 0x6f: // INC (Y)
			{
				const uint8_t rr = m_data->read_byte(m_regs[REG_Y]) + 1;
				m_data->write_byte(m_regs[REG_Y], rr);

				if (rr)
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			}
			case 0x7f: // INC A
			{
				m_regs[REG_A]++;
				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				m_pc++;
				break;
			}
			case 0x8f: // LD (Y),A
				m_data->write_byte(m_regs[REG_Y], m_regs[REG_A]);

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			case 0x9f: // LD rr,A
			{
				m_regs[REG_A] = m_data->read_byte(m_program->read_byte(m_pc));

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				m_pc++;
				break;
			}
			case 0xaf: // AND A,(Y)
				m_regs[REG_A] &= m_data->read_byte(m_regs[REG_Y]);

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;

				break;
			case 0xbf: // AND A,rr
			{
				m_regs[REG_A] &= m_data->read_byte(m_program->read_byte(m_pc));

				if (m_regs[REG_A])
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				m_pc++;
				break;
			}
			case 0xcf: // SUB A,(Y)
			{
				const uint8_t nn = m_data->read_byte(m_regs[REG_Y]);

				if (m_regs[REG_A] < nn)
				{
					m_flags[m_mode] |= FLAG_C;
					m_flags[m_mode] &= ~FLAG_Z;
				}
				else
				{
					m_flags[m_mode] &= ~FLAG_C;
					if (m_regs[REG_A] == nn)
						m_flags[m_mode] |= FLAG_Z;
					else
						m_flags[m_mode] &= ~FLAG_Z;
				}
				m_regs[REG_A] -= nn;
				break;
			}
			case 0xdf: // SUB A,rr
			{
				const uint8_t nn = m_data->read_byte(m_program->read_byte(m_pc));

				if (m_regs[REG_A] < nn)
				{
					m_flags[m_mode] |= FLAG_C;
					m_flags[m_mode] &= ~FLAG_Z;
				}
				else
				{
					m_flags[m_mode] &= ~FLAG_C;
					if (m_regs[REG_A] == nn)
						m_flags[m_mode] |= FLAG_Z;
					else
						m_flags[m_mode] &= ~FLAG_Z;
				}
				m_regs[REG_A] -= nn;
				m_pc++;
				break;
			}
			case 0xef: // DEC (Y)
			{
				const uint8_t rr = m_data->read_byte(m_regs[REG_Y]) - 1;
				m_data->write_byte(m_regs[REG_Y], rr);

				if (rr)
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				break;
			}
			case 0xff: // DEC rr
			{
				const uint8_t rr = m_program->read_byte(m_pc);
				const uint8_t value = m_data->read_byte(rr) - 1;
				m_data->write_byte(rr, value);

				if (value)
					m_flags[m_mode] &= ~FLAG_Z;
				else
					m_flags[m_mode] |= FLAG_Z;
				m_pc++;
				break;
			}
			default:
				logerror("%s: Unsupported opcode: %02x\n", op);
		}

		tick_timers(cycles);

		m_icount -= cycles;
	}
}
