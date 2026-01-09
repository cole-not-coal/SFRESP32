import pandas as pd
import os
import re

# Paths
EXCEL_PATH = os.path.join(os.path.dirname(__file__), 'CAN Loading.xlsx')
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
    # - start_bit is always the LSB of the signal regarding bit allocation in the frame (Intel definition)
    # - If big_endian: reconstruct value assuming lower byte indices are MSB.
    # - If little_endian: reconstruct value assuming lower byte indices are LSB.
    
    # First, identify the chunks of bits involved
    chunks = []
    
    current_bit = 0
    remaining = length
    frame_bit = start_bit
    
    # Collect all chunks: (valid_c_expr, bit_width)
    collected_chunks = []
    
    while remaining > 0:
        byte_idx = frame_bit // 8
        bit_in_byte = frame_bit % 8
        bits_avail = 8 - bit_in_byte
        to_take = min(remaining, bits_avail)
        
        mask = (1 << to_take) - 1
        
        chunk_expr = f"(stFrame.abData[{byte_idx}]"
        if bit_in_byte > 0:
            chunk_expr += f" >> {bit_in_byte}"
        chunk_expr += ")"
        
        if to_take < 8:
            chunk_expr = f"({chunk_expr} & 0x{mask:X})"
            
        collected_chunks.append({'expr': chunk_expr, 'width': to_take, 'byte_idx': byte_idx})
        
        frame_bit += to_take
        remaining -= to_take
        
    # Now assemble them
    full_expr_parts = []
    current_shift = 0
    
    # Process chunks based on Endianness
    if not is_big_endian:
        # Little Endian: Chunks in order 0..N are LSB..MSB
        for chunk in collected_chunks:
            width_cast = "uint32_t" if length > 16 else ("uint16_t" if length > 8 else "uint8_t")
            part = chunk['expr']
            
            # Cast
            if length > 8:
                 part = f"(({width_cast}){part})"
            
            # Shift
            if current_shift > 0:
                part = f"({part} << {current_shift})"
                
            full_expr_parts.append(part)
            current_shift += chunk['width']
    else:
        # Big Endian: Chunk 0 is MSB?
        # Assuming StartBit defines the START of the signal in memory layout.
        # If MSB First, the lowest addressed byte (Byte 6 in standard ex) is MSB.
        # So Chunk 0 (lowest byte index) is MSB.
        
        total_bits = length
        bits_processed = 0
        
        for chunk in collected_chunks:
            width_cast = "uint32_t" if length > 16 else ("uint16_t" if length > 8 else "uint8_t")
            part = chunk['expr']
            
            if length > 8:
                 part = f"(({width_cast}){part})"
            
            shift = total_bits - bits_processed - chunk['width']
            
            if shift > 0:
                 part = f"({part} << {shift})"
            
            full_expr_parts.append(part)
            bits_processed += chunk['width']
            
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
        for _, row in df_bus.iterrows():
            pid = parse_id(row['ID'])
            if pid is not None and pd.notna(row['Name']):
                msg_map[pid] = {
                    'name': str(row['Name']).strip(),
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
        df_msgs['ID'] = df_msgs['ID'].fillna(method='ffill')
        
        messages = {}
        
        print("Parsing signals...")
        for _, row in df_msgs.iterrows():
            pid = parse_id(row['ID'])
            if pid is None:
                continue
            
            sname = row['Name']
            if pd.isna(sname):
                continue
            
            sname = str(sname).strip()
<<<<<<< HEAD
            
            # Filter reserved/spare signals
            if any(x in sname.lower() for x in ['reserved', 'spare', 'padding', 'unused']):
                continue

=======
>>>>>>> 9105219 (CAN message decode script)
            # Clean variable names (remove illegal chars)
            sname = re.sub(r'[^a-zA-Z0-9_]', '', sname)
            
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
            
            # Determine type
            c_type = "float" 
            if gain == 1.0 and offset == 0.0:
                # If pure integer
                if length <= 8:
                    c_type = "int8_t" if is_signed else "uint8_t"
                elif length <= 16:
                    c_type = "int16_t" if is_signed else "uint16_t"
                elif length <= 32:
                    c_type = "int32_t" if is_signed else "uint32_t"
            
            signal = {
                'name': sname,
                'desc': sdesc,
                'start_bit': start_bit,
                'length': length,
                'gain': gain,
                'offset': offset,
                'signed': is_signed,
                'type': c_type,
                'big_endian': is_big_endian
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
    h_content = "#ifndef CAN_DECODE_AUTO_H\n#define CAN_DECODE_AUTO_H\n\n#include <stdint.h>\n#include \"esp_err.h\"\n#include \"can.h\"\n\n"
    c_content = "/* This file is autogenerated from the script decodeCAN.py */\n#include \"canDecodeAuto.h\"\n\n"

    # Generate Defines
    for pid in sorted(messages.keys()):
        if pid not in msg_map:
            continue
            
        msg_info = msg_map[pid]
        msg_name = msg_info['name']
        
        # Sanitize for C function name
        msg_name_clean = re.sub(r'[^a-zA-Z0-9_]', '', msg_name)
        func_name = msg_name_clean
        if not func_name: func_name = f"Msg_{pid:X}"
        
        h_content += f"#define {func_name.upper()}_ID 0x{pid:X}\n"

    h_content += "\n"

    # Generate Defines
    for pid in sorted(messages.keys()):
        if pid not in msg_map:
            continue
            
        msg_info = msg_map[pid]
        msg_name = msg_info['name']
        
        # Sanitize for C function name
        msg_name_clean = re.sub(r'[^a-zA-Z0-9_]', '', msg_name)
        func_name = msg_name_clean
        if not func_name: func_name = f"Msg_{pid:X}"
        
        h_content += f"#define {func_name.upper()}_ID 0x{pid:X}\n"

    h_content += "\n"

<<<<<<< HEAD
    # Generate Defines
    for pid in sorted(messages.keys()):
        if pid not in msg_map:
            continue
            
        msg_info = msg_map[pid]
        msg_name = msg_info['name']
        
        # Sanitize for C function name
        msg_name_clean = re.sub(r'[^a-zA-Z0-9_]', '', msg_name)
        func_name = msg_name_clean
        if not func_name: func_name = f"Msg_{pid:X}"
        
        h_content += f"#define {func_name.upper()}_ID 0x{pid:X}\n"

    h_content += "\n"

=======
>>>>>>> 9105219 (CAN message decode script)
    count = 0
    for pid in sorted(messages.keys()):
        if pid not in msg_map:
            # Maybe warning?
            continue
            
        msg_info = msg_map[pid]
        msg_name = msg_info['name']
        msg_desc = msg_info['desc']
        
        # Sanitize for C function name
        msg_name_clean = re.sub(r'[^a-zA-Z0-9_]', '', msg_name)
        
        # User requested exact name from Excel (sanitized), NO Status prefix manipulation
        func_name = msg_name_clean
            
        if not func_name: func_name = f"Msg_{pid:X}"
        
        args_str = "CAN_frame_t stFrame"
        for sig in messages[pid]:
            args_str += f", {sig['type']}* {sig['name']}"
            
        h_content += f"esp_err_t {func_name}({args_str});\n"
        
        c_content += f"esp_err_t {func_name}({args_str})\n{{\n"
        c_content += "    /*\n"
        c_content += f"    *===========================================================================\n"
        c_content += f"    *   {func_name}\n"
        c_content += f"    *   Message: {msg_name} (0x{pid:X})\n"
        if msg_desc:
            c_content += f"    *   Description: {msg_desc}\n"
        c_content += "    *   Takes:   stFrame: The CAN frame to decode\n"
        
        # Add variable descriptions
        for sig in messages[pid]:
            arg_desc = f"{sig['name']}: {sig['desc']}"
            c_content += f"    *            {arg_desc}\n"
            
        c_content += "    * \n"
        c_content += "    *   Returns: ESP_OK if successful, error code if not.\n"
        c_content += "    * \n"
        c_content += f"    *   Autogenerated by decodeCAN.py\n"    
        c_content += "    */\n"
        
        c_content += "    if (stFrame.byDLC != 8)\n    {\n        return ESP_ERR_INVALID_SIZE;\n    }\n"
        c_content += f"    if (stFrame.dwID != 0x{pid:X}) \n    {{\n        return ESP_ERR_INVALID_ARG;\n    }}\n"
        
        for sig in messages[pid]:
             unpack = generate_unpack_expr(sig['start_bit'], sig['length'], sig['big_endian'])
             
             raw_expr = f"({unpack})"
             
             if sig['type'] == 'float':
                 val_expr = f"(float){raw_expr}"
                 if sig['gain'] != 1.0:
                     val_expr += f" * {sig['gain']}f"
                 if sig['offset'] != 0.0:
                     val_expr += f" + {sig['offset']}f"
             else:
                 val_expr = f"({sig['type']}){raw_expr}"
                 
             c_content += f"    *{sig['name']} = {val_expr};\n"
             
        c_content += "    return ESP_OK;\n}\n\n"
        count += 1

    h_content += "\n#endif\n"
    
    with open(OUTPUT_C_PATH, 'w') as f:
        f.write(c_content)
    with open(OUTPUT_H_PATH, 'w') as f:
        f.write(h_content)
        
    print(f"Generated code for {count} messages in {OUTPUT_C_PATH}")

if __name__ == "__main__":
    main()
