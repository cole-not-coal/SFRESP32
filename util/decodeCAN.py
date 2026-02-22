import pandas as pd
import os
import re
import tkinter as tk
from tkinter import filedialog

# Note from CP: I'm sorry. I cba with python and gpt spat this out.

# Initialize tkinter root
root = tk.Tk()
root.withdraw() # Hide the main window

# Paths
EXCEL_PATH = filedialog.askopenfilename(
    title="Select CAN Loading Excel File",
    initialdir=os.path.dirname(__file__),
    filetypes=[("Excel files", "*.xlsx *.xls")]
)

if not EXCEL_PATH:
    print("No file selected. Exiting.")
    exit()

OUTPUT_C_PATH = os.path.join(os.path.dirname(__file__), '../main/CAN/canDecodeAuto.c')
OUTPUT_H_PATH = os.path.join(os.path.dirname(__file__), '../main/CAN/canDecodeAuto.h')

def parse_id(id_val):
    if pd.isna(id_val):
        return None
    s = str(id_val).strip()
    if s.startswith('0x'):
        return int(s, 16)
    try:
        return int(s)
    except:
        return None

def generate_unpack_expr(start_bit, length, is_big_endian):
    # Generates C expression to extract bits
    # Assumptions:
    # - start_bit:
    #   - Little Endian (Intel): LSB of the signal.
    #   - Big Endian (Motorola/Vector): MSB of the signal. << VECTOR DEFINITION >>
    
    chunks = []
    
    remaining = length
    
    # Vector/Motorola Logic:
    # Start Bit is the "First Bit" transmitted, which is the MSB.
    # Bits are counted "backwards" within a byte, but bytes increment forward?
    # actually simplistic view:
    # Frame is array of 8 bytes.
    # 0 1 2 3 4 5 6 7 (Bit Index in Byte)
    # Most tools number bits 0..63 linearly?
    # Let's stick to (Byte, Bit) coordinates.
    
    # Vector Definition:
    # Start Bit is the bit index of the MSB.
    # Subsequent bits are found by decrementing the bit index.
    # BUT! When crossing a byte boundary (bit index < 0), we go to the previous byte? 
    # NO. Motorola "Backwards" means:
    # Signal: [MSB] ... [LSB]
    # Bit numbering usually 7..0 inside byte.
    
    # Let's map linear bit index (0..63) where 0 is Byte0.Bit0
    # Vector Start Bit (S) is the MSB.
    # Next bit is S - 1? No, Vector mapping is complex.
    
    # Standard Motorola processing usually works like this:
    # 1. Identify byte containing MSB.
    # 2. Extract bits from that byte downwards.
    # 3. If signal continues, it goes to the *next* byte (higher index), starting from bit 7.
    
    # Let convert everything to Linear Bit Index (0..63) where 0=Byte0.Bit0
    
    collected_chunks = []
    
    current_bit_ptr = start_bit # This is MSB in Vector/Motorola
    
    # If Big Endian, we scan from MSB (start_bit) down to LSB.
    # In Motorola forward sequential:
    # MSB is at `start_bit`. 
    # The "next" bit (less significant) is at `start_bit - 1`? 
    # Wait, if start_bit is 7 (Byte0.Bit7), where is next? 
    # Logic: Decrement bit index. If bit index wraps below 0 in a byte, go to next byte's bit 7?
    # Example: 16 bit signal starting at Byte0.Bit0 (Linear 0).
    # Motorola Start bit would be... 
    # Let's use the explicit "Sawtooth" pattern for Motorola.
    
    if not is_big_endian:
        # Intel / Little Endian
        # Start Bit is LSB.
        # Bits are contiguous in linear map.
        frame_bit = start_bit
        while remaining > 0:
            byte_idx = frame_bit // 8
            bit_in_byte = frame_bit % 8
            bits_avail = 8 - bit_in_byte
            to_take = min(remaining, bits_avail)
            
            mask = (1 << to_take) - 1
            chunk_expr = f"(stFrame.abData[{byte_idx}] >> {bit_in_byte})"
            chunk_expr = f"({chunk_expr} & 0x{mask:X})"
            
            collected_chunks.append({'expr': chunk_expr, 'width': to_take})
            
            frame_bit += to_take
            remaining -= to_take
            
    else:
        # Big Endian (Motorola / Vector)
        # Start Bit is MSB.
        # We trace bits from MSB (high value) down to LSB (low value).
        # "Down" in significance means "Forward" in time/byte index?
        # Actually in Vector DBC:
        # Startbit 7, Length 8 -> Bits 7,6,5,4,3,2,1,0 (All Byte 0)
        # Startbit 0, Length 2 -> Bits 0 (Byte0), then... where?
        # Usually signals don't straddle nicely like that in reverse.
        
        # Algorithm:
        # 1. Start at `current_bit_ptr` (Linear Index 0..63)
        # 2. This byte is `current_bit_ptr // 8`
        # 3. Bit index in byte is `current_bit_ptr % 8`
        # 4. We can take bits *downwards* to 0. (e.g. if bit 3, we can take 3,2,1,0 -> 4 bits)
        # 5. Length of chunk = min(remaining, bit_in_byte + 1)
        # 6. Shift = ? We are reading MSB first.
        
        # Loop for chunks
        while remaining > 0:
            byte_idx = current_bit_ptr // 8
            bit_in_byte = current_bit_ptr % 8
            
            # In Motorola, we consume bits "downwards" in the byte (7->0)
            # The number of bits available in this byte "below" current position is `bit_in_byte + 1`
            to_take = min(remaining, bit_in_byte + 1)
            
            # The chunk starts at `bit_in_byte - to_take + 1` and ends at `bit_in_byte`
            shift_in_byte = bit_in_byte - to_take + 1
            mask = (1 << to_take) - 1
            
            chunk_expr = f"(stFrame.abData[{byte_idx}] >> {shift_in_byte})"
            chunk_expr = f"({chunk_expr} & 0x{mask:X})"
            
            collected_chunks.append({'expr': chunk_expr, 'width': to_take})
            
            remaining -= to_take
             
            # Move to next byte:
            # For Motorola, if we haven't finished, we jump to the NEXT byte (byte_idx + 1)
            # and start at the TOP bit (7).
            # The linear index for "Next" logic is strangely discontinuous in this mapping view,
            # but logically we just move pointer.
            if remaining > 0:
                 current_bit_ptr = (byte_idx + 1) * 8 + 7
                 
                 # Safety check for out-of-bounds access
                 if (current_bit_ptr // 8) > 7:
                     # This implies the signal definition extends beyond the 8-byte CAN frame.
                     # We break here to avoid generating invalid C code (accessing abData[8]).
                     # Ideally, we should warn the user, but for code generation safely, we stop.
                     print(f"Warning: Signal '{sig['name']}' in Message 0x{pid:X} extends beyond 8 bytes (Start={sig['start_bit']}, Length={sig['length']}). Truncating to fit.")
                     break
                 
    # Reassemble
    full_expr_parts = []
    
    if not is_big_endian:
        # Little Endian: LSB chunks first
        current_shift = 0
        for chunk in collected_chunks:
             width_cast = "uint32_t" if length > 16 else ("uint16_t" if length > 8 else "uint8_t")
             part = chunk['expr']
             if length > 8: part = f"(({width_cast}){part})"
             if current_shift > 0: part = f"({part} << {current_shift})"
             full_expr_parts.append(part)
             current_shift += chunk['width']
    else:
        # Big Endian: MSB chunks first
        # collected_chunks[0] is the MSB part.
        # We need to shift it "Left" to the top of the variable.
        total_remaining = length
        for chunk in collected_chunks:
             width_cast = "uint32_t" if length > 16 else ("uint16_t" if length > 8 else "uint8_t")
             part = chunk['expr']
             
             # Shift this chunk to its position in the final integer
             # It is currently 'width' bits wide.
             # Its MSB should align with `total_remaining - 1`
             # We shift it left by `total_remaining - chunk['width']`
             
             shift = total_remaining - chunk['width']
             if length > 8: part = f"(({width_cast}){part})"
             if shift > 0: part = f"({part} << {shift})"
             
             full_expr_parts.append(part)
             total_remaining -= chunk['width']
            
    if len(full_expr_parts) == 1:
        return full_expr_parts[0]
    else:
        return " | ".join(full_expr_parts)

def main():
    try:
        print(f"Reading {EXCEL_PATH}...")
        
        # --- Read Main BUS (Map ID -> MessageName) ---
        df_bus_raw = pd.read_excel(EXCEL_PATH, sheet_name='Main BUS', header=None)
        
        # Find header for Main BUS (Row with "ID" and "Name")
        header_row_idx = None
        for i, row in df_bus_raw.iterrows():
            row_str = " ".join([str(x) for x in row if pd.notna(x)])
            if "ID" in row_str and "Name" in row_str:
                header_row_idx = i
                break
        
        if header_row_idx is None:
            raise Exception("Could not find header in Main BUS sheet")
            
        df_bus = pd.read_excel(EXCEL_PATH, sheet_name='Main BUS', header=header_row_idx)
        
        msg_map = {}
        seen_msg_names = set()
        
        for _, row in df_bus.iterrows():
            pid = parse_id(row['ID'])
            if pid is not None and pd.notna(row['Name']):
                msg_name_raw = str(row['Name']).strip()
                cleaned_name = re.sub(r'[^a-zA-Z0-9_]', '', msg_name_raw)
                
                if cleaned_name in seen_msg_names:
                    raise Exception(f"Duplicate message name found: '{cleaned_name}' (First seen, now at ID: 0x{pid:X})")
                seen_msg_names.add(cleaned_name)
                
                msg_map[pid] = {
                    'name': msg_name_raw,
                    'desc': str(row['Description']).strip() if pd.notna(row['Description']) else ""
                }
                
        # --- Read Main BUS Message (Signals) ---
        df_msgs_raw = pd.read_excel(EXCEL_PATH, sheet_name='Main BUS Message', header=None)
        
        # Find header
        header_row_idx = None
        for i, row in df_msgs_raw.iterrows():
             row_str = " ".join([str(x) for x in row if pd.notna(x)])
             if "ID" in row_str and "Name" in row_str and "start bit" in row_str:
                 header_row_idx = i
                 break
                 
        if header_row_idx is None:
             raise Exception("Could not find header in Main BUS Message sheet")
             
        df_msgs = pd.read_excel(EXCEL_PATH, sheet_name='Main BUS Message', header=header_row_idx)
        
        # Forward fill ID
        df_msgs['ID'] = df_msgs['ID'].ffill()
        
        messages = {}
        seen_signal_names = {} # Map name -> signal dict (for validation)
        
        print("Parsing signals...")
        for _, row in df_msgs.iterrows():
            pid = parse_id(row['ID'])
            if pid is None:
                continue
            
            sname = row['Name']
            if pd.isna(sname):
                continue
            
            sname = str(sname).strip()
            
            # Filter reserved/spare signals
            if any(x in sname.lower() for x in ['reserved', 'spare', 'padding', 'unused']):
                continue

            # Check for constant signal (Hex name like 0xF5)
            is_constant = False
            constant_val = 0
            if sname.lower().startswith('0x'):
                try:
                    constant_val = int(sname, 16)
                    is_constant = True
                    # Use a unique internal name for constants to avoid clashes if multiple messages use same constant? 
                    # Use original hex string as name, but clean it?
                    # Actually, we don't need a variable name for it. 
                    # But we need it to NOT clash with "0x..." string logic below.
                    # Let's keep sname as is for now, clean it later if needed.
                except:
                    pass

            if not is_constant:
                sname = re.sub(r'[^a-zA-Z0-9_]', '', sname)
            
                # Check if signal name collides with a message name
                if sname in seen_msg_names:
                    raise Exception(f"Signal name '{sname}' collides with a Message Name! (In Message ID: {row['ID']})")
                
            sdesc = str(row['Description']).strip() if pd.notna(row['Description']) else ""
            
            try:
                start_bit = int(row['start bit'])
                length = int(row['length (bits)'])
            except:
                continue # Skip bad rows
                
            gain = float(row['Gain']) if pd.notna(row['Gain']) else 1.0
            offset = float(row['Offset']) if pd.notna(row['Offset']) else 0.0
            is_signed_str = str(row['Signed']).strip().upper()
            is_signed = (is_signed_str == 'YES' or is_signed_str == 'TRUE')
            
            byte_order = str(row['Byte Order']).strip().upper() if pd.notna(row['Byte Order']) else ""
            is_big_endian = "MSB" in byte_order

            if is_big_endian:
                # Convert Intel-based Start Bit (LSB) to Motorola-based Start Bit (MSB)
                # mimicking the logic in CANDBGenerator.m: MSBPos = (7-mod(startBit,8)) + floor(startBit/8)*8;
                # This flips the bit index within the byte (0->7, 7->0) while keeping byte index same.
                # NOTE: This conversion assumes the input 'start_bit' is the LSB bit index (Intel/Linear).
                # If the input 'start_bit' is ALREADY the MSB (Motorola Standard), this conversion is wrong.
                # However, the user explicitly requested this conversion to fix "9th byte" issues.
                start_bit = (7 - (start_bit % 8)) + (start_bit // 8) * 8
            
            # --- Mux Handling ---
            mux_val = None
            is_mux_switch = False
            
            # Check if "Mux" column exists in dataframe
            if 'Mux' in row:
                mux_cell = row['Mux']
                if pd.notna(mux_cell):
                    mux_str = str(mux_cell).strip()
                    if mux_str.lower() == 'mux':
                        is_mux_switch = True
                    else:
                        # Try to parse as integer ID for the mux
                        try:
                            # Handle hex or decimal
                            if mux_str.startswith('0x'):
                                mux_val = int(mux_str, 16)
                            else:
                                mux_val = int(float(mux_str)) # float cast handles '1.0' from excel
                        except:
                            # If we can't parse it, treat as standard signal? or error?
                            # For now, treat as standard if not recognized
                            pass

            
            # --- Data Type Handling ---
            c_type = "float" # Default
            
            # Check if "Data Type" column exists and has value
            has_explicit_type = False
            if 'Data Type' in row and pd.notna(row['Data Type']):
                dtype_str = str(row['Data Type']).strip()
                # Map Excel types to C types
                if dtype_str.lower() in ['uint8', 'unsigned char']:
                    c_type = "uint8_t"
                    has_explicit_type = True
                elif dtype_str.lower() in ['int8', 'char', 'signed char']:
                    c_type = "int8_t"
                    has_explicit_type = True
                elif dtype_str.lower() in ['uint16', 'unsigned short']:
                    c_type = "uint16_t"
                    has_explicit_type = True
                elif dtype_str.lower() in ['int16', 'short', 'signed short']:
                    c_type = "int16_t"
                    has_explicit_type = True
                elif dtype_str.lower() in ['uint32', 'unsigned int', 'unsigned long']:
                    c_type = "uint32_t"
                    has_explicit_type = True
                elif dtype_str.lower() in ['int32', 'int', 'signed int', 'long', 'signed long']:
                    c_type = "int32_t"
                    has_explicit_type = True
                elif dtype_str.lower() in ['float']:
                    c_type = "float"
                    has_explicit_type = True
                elif dtype_str.lower() in ['bool', 'boolean']:
                    c_type = "bool"
                    has_explicit_type = True
                else:
                    # Fallback or use as is if it looks like a valid C type?
                    # For safety, stick to auto logic if not recognized, or warn?
                    print(f"Warning: Unknown Data Type '{dtype_str}' for signal '{sname}'. Using auto-detection.")

            if not has_explicit_type:
                if gain == 1.0 and offset == 0.0:
                    # If pure integer
                    if length <= 8:
                        c_type = "int8_t" if is_signed else "uint8_t"
                    elif length <= 16:
                        c_type = "int16_t" if is_signed else "uint16_t"
                    elif length <= 32:
                        c_type = "int32_t" if is_signed else "uint32_t"
            
            # Validate Shared Signals (Non-Constant)
            if not is_constant:
                 if sname in seen_signal_names:
                     existing_sig = seen_signal_names[sname]
                     # Check validation rules
                     # Rule: Same Length, Same Type
                     if length != existing_sig['length']:
                         raise Exception(f"Signal '{sname}' reused with different length! New: {length}, Old: {existing_sig['length']} (ID: 0x{pid:X})")
                     if c_type != existing_sig['type']:
                         raise Exception(f"Signal '{sname}' reused with different type! New: {c_type}, Old: {existing_sig['type']} (ID: 0x{pid:X})")
                     # Also maybe check is_signed?
                     if is_signed != existing_sig['signed']:
                         print(f"Warning: Signal '{sname}' reused with different signedness! New: {is_signed}, Old: {existing_sig['signed']} (ID: 0x{pid:X})")
                 else:
                     # Add to seen
                     seen_signal_names[sname] = {
                         'length': length,
                         'type': c_type,
                         'signed': is_signed
                     }

            signal = {
                'name': sname,
                'desc': sdesc,
                'start_bit': start_bit,
                'length': length,
                'gain': gain,
                'offset': offset,
                'signed': is_signed,
                'type': c_type,
                'big_endian': is_big_endian,
                'is_mux_switch': is_mux_switch,
                'mux_val': mux_val,
                'is_constant': is_constant,
                'constant_val': constant_val
            }
            
            if pid not in messages:
                messages[pid] = []
            messages[pid].append(signal)

        print(f"Found {len(messages)} messages with signals.")
        generate_c_code(messages, msg_map)

    except Exception as e:
        print(f"Error: {e}")
        import traceback
        traceback.print_exc()

def generate_c_code(messages, msg_map):
    h_content = "#ifndef CAN_DECODE_AUTO_H\n#define CAN_DECODE_AUTO_H\n\n#include <stdint.h>\n#include \"esp_err.h\"\n#include \"can.h\"\n#include \"esp_twai.h\"\n#include \"esp_twai_onchip.h\"\n\n"
    c_content = "/* This file is autogenerated from the script decodeCAN.py */\n#include \"canDecodeAuto.h\"\n\n"

    # 1. Generate Global Variables (Externs in H, Definitions in C)
    generated_globals = set()
    for pid in sorted(messages.keys()):
        for sig in messages[pid]:
            if sig['is_constant']:
                continue # Do not generate global for constants
            
            if sig['name'] in generated_globals:
                continue # Already generated
                
            generated_globals.add(sig['name'])
            h_content += f"extern {sig['type']} {sig['name']};\n"
            c_content += f"{sig['type']} {sig['name']} = 0;\n"
    
    h_content += "\n"
    c_content += "\n"

    # 2. Generate Defines
    for pid in sorted(messages.keys()):
        if pid not in msg_map:
            continue
        msg_info = msg_map[pid]
        msg_name = msg_info['name']
        msg_name_clean = re.sub(r'[^a-zA-Z0-9_]', '', msg_name)
        func_name = msg_name_clean
        if not func_name: func_name = f"Msg_{pid:X}"
        h_content += f"#define {func_name.upper()}_ID 0x{pid:X}\n"
    h_content += "\n"

    # 3. Generate Functions
    count = 0
    for pid in sorted(messages.keys()):
        if pid not in msg_map:
            continue
            
        msg_info = msg_map[pid]
        msg_name = msg_info['name']
        msg_desc = msg_info['desc']
        msg_name_clean = re.sub(r'[^a-zA-Z0-9_]', '', msg_name)
        base_name = msg_name_clean
        if not base_name: base_name = f"Msg_{pid:X}"
        
        # --- Decode Function ---
        func_rx_name = f"{base_name}Rx"
        args_str = "CAN_frame_t stFrame"
        h_content += f"esp_err_t {func_rx_name}({args_str});\n"
        
        c_content += f"esp_err_t {func_rx_name}({args_str})\n{{\n"
        c_content += "    /*\n"
        c_content += f"    *===========================================================================\n"
        c_content += f"    *   {func_rx_name}\n"
        c_content += f"    *   Message: {msg_name} (0x{pid:X})\n"
        if msg_desc:
            c_content += f"    *   Description: {msg_desc}\n"
        c_content += "    *   Takes:   stFrame: The CAN frame to decode\n"
        c_content += "    *   Returns: ESP_OK if successful, error code if not.\n"
        c_content += "    *   Autogenerated by decodeCAN.py\n"    
        c_content += "    */\n"
        
        c_content += "    if (stFrame.byDLC != 8) return ESP_ERR_INVALID_SIZE;\n"
        c_content += f"    if (stFrame.dwID != 0x{pid:X}) return ESP_ERR_INVALID_ARG;\n\n"
        
        # Sort out signals
        standard_sigs = [s for s in messages[pid] if not s['is_mux_switch'] and s['mux_val'] is None]
        mux_switch_sig = next((s for s in messages[pid] if s['is_mux_switch']), None)
        muxed_sigs = [s for s in messages[pid] if s['mux_val'] is not None]

        # Decode Standard Signals
        if standard_sigs:
            c_content += "    /* Standard Signals */\n"
            for sig in standard_sigs:
                c_content += generate_signal_decode(sig)

        # Decode Mux Switch (if exists)
        if mux_switch_sig:
            c_content += "\n    /* Mux Switch */\n"
            c_content += generate_signal_decode(mux_switch_sig)
             
            if muxed_sigs:
                unpack = generate_unpack_expr(mux_switch_sig['start_bit'], mux_switch_sig['length'], mux_switch_sig['big_endian'])
                c_content += "\n    /* Muxed Signals */\n"
                c_content += f"    switch({unpack})\n    {{\n"
                
                # Group by mux value
                mux_dict = {}
                for sig in muxed_sigs:
                    val = sig['mux_val']
                    if val not in mux_dict: mux_dict[val] = []
                    mux_dict[val].append(sig)
                    
                for val in sorted(mux_dict.keys()):
                    c_content += f"        case {val}:\n"
                    for sig in mux_dict[val]:
                        c_content += generate_signal_decode(sig, indent="            ")
                    c_content += "            break;\n"
                    
                c_content += "        default:\n            break;\n    }\n"
        
        c_content += "    return ESP_OK;\n}\n\n"

        # --- Transmit Function ---
        func_tx_name = f"{base_name}Tx"
        # We need stCANBus argument
        h_content += f"esp_err_t {func_tx_name}(twai_node_handle_t stCANBus);\n"
        
        c_content += f"esp_err_t {func_tx_name}(twai_node_handle_t stCANBus)\n{{\n"
        c_content += "    /*\n"
        c_content += f"    *===========================================================================\n"
        c_content += f"    *   {func_tx_name}\n"
        c_content += f"    *   Encodes and Transmits Message: {msg_name} (0x{pid:X})\n"
        c_content += "    *   Uses global signal variables.\n"
        c_content += "    *   Takes:   stCANBus: Handle to CAN bus to transmit on\n"
        c_content += "    *   Returns: ESP_OK if successful, error code if not.\n"
        c_content += "    *   Autogenerated by decodeCAN.py\n"  
        c_content += "    */\n"
        c_content += "    CAN_frame_t stFrame;\n"
        c_content += f"    stFrame.dwID = 0x{pid:X};\n"
        c_content += "    stFrame.byDLC = 8;\n" # Default to 8 for now, or calc max byte? 8 is safe for standard CAN
        c_content += "    memset(stFrame.abData, 0, 8);\n\n"
        
        # We need to sort logic for Muxed signals or just assume Standard? 
        # Standard packing logic:
        # Iterate all signals, pack them into stFrame.abData
        
        # Note: Muxing for TX is complex because multiple signals might map to same bits with different mux values.
        # For now, let's implement Standard signals packing. 
        # Making a generic TX for highly muxed messages implies we send multiple frames? 
        # Usually TX functions are specific to one Mux ID or just base.
        # Let's support Standard Signals + Mux Switch (if present) + Muxed Signals (This is tricky logic).
        
        # Simplified Strategy for Auto-Gen TX:
        # 1. Pack all Standard Signals.
        # 2. Pack Mux Switch signal.
        # 3. For Muxed signals... we can't send ALL of them in one frame if they overlap.
        #    If a message is muxed, the Global Variable approach for TX is ambiguous:
        #    Which Mux ID do we want to send right now?
        #    
        #    Option A: The Mux Switch Variable holds the ID we want to send. 
        #    We check `MuxSwitchVar` and pack only the signals corresponding to that value.
        
        standard_sigs = [s for s in messages[pid] if not s['is_mux_switch'] and s['mux_val'] is None]
        mux_switch_sig = next((s for s in messages[pid] if s['is_mux_switch']), None)
        muxed_sigs = [s for s in messages[pid] if s['mux_val'] is not None]
        
        # 1. Standard
        for sig in standard_sigs:
            c_content += generate_signal_encode(sig)
            
        # 2. Mux Switch
        if mux_switch_sig:
            c_content += "\n    /* Mux Switch */\n"
            c_content += generate_signal_encode(mux_switch_sig)
            
            if muxed_sigs:
                c_content += "\n    /* Muxed Signals */\n"
                # Switch on the Global Mux Variable to decide which signals to pack
                # But wait, signal type might be float. float switch? unexpected.
                # Cast global variable to int for switch
                c_content += f"    switch((int){mux_switch_sig['name']})\n    {{\n"
                
                mux_dict = {}
                for sig in muxed_sigs:
                    val = sig['mux_val']
                    if val not in mux_dict: mux_dict[val] = []
                    mux_dict[val].append(sig)
                    
                for val in sorted(mux_dict.keys()):
                    c_content += f"        case {val}:\n"
                    for sig in mux_dict[val]:
                        c_content += generate_signal_encode(sig, indent="            ")
                    c_content += "            break;\n"
                    
                c_content += "        default:\n            break;\n    }\n"
                
        c_content += "\n    return CAN_transmit(stCANBus, &stFrame);\n}\n\n"
        
        count += 1

    h_content += "\n#endif\n"
    
    with open(OUTPUT_C_PATH, 'w') as f:
        f.write(c_content)
    with open(OUTPUT_H_PATH, 'w') as f:
        f.write(h_content)
        
    print(f"Generated code for {count} messages in {OUTPUT_C_PATH}")

def generate_signal_decode(sig, indent="    "):
    if sig['is_constant']:
        return f"{indent}/* Constant {sig['name']} ignored on receive */\n"

    unpack = generate_unpack_expr(sig['start_bit'], sig['length'], sig['big_endian'])
    raw_expr = f"({unpack})"
    
    # Always apply gain/offset logic, but cast to target type at the end
    # We cast raw value to float/double first to ensure precision during calculation
    val_expr = f"(float){raw_expr}"
    
    if sig['gain'] != 1.0:
        val_expr += f" * {sig['gain']}f"
    if sig['offset'] != 0.0:
        val_expr += f" + {sig['offset']}f"
        
    # Cast back to target type
    val_expr = f"({sig['type']})({val_expr})"
        
    return f"{indent}{sig['name']} = {val_expr};\n"

def generate_signal_encode(sig, indent="    "):
    # Generate code to pack global variable 'sig['name']' into stFrame.abData
    # Inverse of generate_unpack_expr

    # Value preparation (Local variable for calculation readability)
    
    if sig['is_constant']:
        # Constant value provided in signal name (e.g. 0xF5)
        # We usage the raw value.
        width_mask = (1 << sig['length']) - 1
        val_expr = f"(0x{sig['constant_val']:X} & 0x{width_mask:X})"
    else:
        # All signals (float or int) undergo physical -> raw conversion
        # raw = (phys - offset) / gain
        val_term = f"((float){sig['name']})" 
        
        if sig['offset'] != 0.0:
            val_term = f"({val_term} - {sig['offset']}f)"
        if sig['gain'] != 1.0:
            val_term = f"({val_term} / {sig['gain']}f)"
        
        # Cast to integer type large enough (uint32_t covers most signals)
        # and mask to length
        width_mask = (1 << sig['length']) - 1
        val_expr = f"((uint32_t){val_term} & 0x{width_mask:X})"

    lines = ""
    
    current_bit_ptr = sig['start_bit']
    remaining = sig['length']
    
    collected_chunks = []
    
    # Chunking Logic (Must Match Unpack Logic exactly)
    if not sig['big_endian']:
        # Little Endian (Intel)
        frame_bit = sig['start_bit']
        while remaining > 0:
            byte_idx = frame_bit // 8
            bit_in_byte = frame_bit % 8
            bits_avail = 8 - bit_in_byte
            to_take = min(remaining, bits_avail)
            
            mask = (1 << to_take) - 1
            collected_chunks.append({'width': to_take, 'byte_idx': byte_idx, 'bit_in_byte': bit_in_byte, 'shift_in_byte': bit_in_byte})
            
            frame_bit += to_take
            remaining -= to_take
            
    else:
        # Big Endian (Motorola)
        # Start Bit is MSB.
        # We process chunks downwards in significance (MSB -> LSB)
        
        while remaining > 0:
            byte_idx = current_bit_ptr // 8
            bit_in_byte = current_bit_ptr % 8
            
            # Takes bits "downwards" from current bit
            # Example: start at bit 7, take 8 bits -> byte_idx=0 bit_in_byte=7 to_take=8
            # shift_in_byte = 7 - 8 + 1 = 0
            to_take = min(remaining, bit_in_byte + 1)
            shift_in_byte = bit_in_byte - to_take + 1
            
            collected_chunks.append({'width': to_take, 'byte_idx': byte_idx, 'shift_in_byte': shift_in_byte})
            
            remaining -= to_take
            
            if remaining > 0:
                 current_bit_ptr = (byte_idx + 1) * 8 + 7
                 
                 # Safety check
                 if (current_bit_ptr // 8) > 7:
                     print(f"Warning: Signal '{sig['name']}' extends beyond 8 bytes (Start={sig['start_bit']}, Length={sig['length']}). Truncating encode.")
                     break

    # Bit Packing Logic
    if not sig['big_endian']:
        # Little Endian (Intel)
        current_shift = 0
        for chunk in collected_chunks:
            chunk_mask = (1 << chunk['width']) - 1
            bits_extr = f"(({val_expr} >> {current_shift}) & 0x{chunk_mask:X})"
            lines += f"{indent}stFrame.abData[{chunk['byte_idx']}] |= (uint8_t)({bits_extr} << {chunk['shift_in_byte']});\n"
            current_shift += chunk['width']
            
    else:
        # Big Endian (Motorola)
        # We collected chunks from MSB downwards.
        # So chunk[0] corresponds to the Most Significant Bits of `val_expr`.
        # We extract from `val_expr` starting from the top.
        
        total_remaining = sig['length']
        
        for chunk in collected_chunks:
             # Extract topmost bits of remaining value
             # Value total width = `sig['length']`
             # Shift right to bring top `width` bits to LSB position?
             # wait, we shift right by (total_remaining - chunk_width)
             
             shift = total_remaining - chunk['width']
             chunk_mask = (1 << chunk['width']) - 1
             
             bits_extr = f"(({val_expr} >> {shift}) & 0x{chunk_mask:X})"
             
             # The chunk defines where in the BYTE it goes (shift_in_byte)
             lines += f"{indent}stFrame.abData[{chunk['byte_idx']}] |= (uint8_t)({bits_extr} << {chunk['shift_in_byte']});\n"
             
             total_remaining -= chunk['width']

    return lines

if __name__ == "__main__":
    main()
