#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
dump304.py - M304 EEPROM Dump & Parser Tool (Python 3 port)
  Supports M304 Version 2.x and Version 3.x
"""

import sys
import argparse
import urllib.request
import urllib.error
import struct

APP_NAME = "dump304.py"
APP_VERSION = "3.06"


def fetch_lines(url: str):
    """Fetch text lines from target node URL."""
    try:
        req = urllib.request.Request(url, headers={'User-Agent': 'dump304-python'})
        with urllib.request.urlopen(req, timeout=5) as response:
            content = response.read().decode('ascii', errors='ignore')
            return content.splitlines(keepends=True)
    except urllib.error.URLError as e:
        print(f"Connection error to {url}: {e}", file=sys.stderr)
        return []
    except Exception as e:
        print(f"Error fetching {url}: {e}", file=sys.stderr)
        return []


def get_versions(host: str):
    """
    Fetch F/W and PARAM versions from 0x7F00 block.
    PARAM VERSION: 0x7FE0 (16 bytes)
    F/W VERSION:   0x7FF0 (16 bytes)
    """
    url = f"http://{host}/L007F00"
    lines = fetch_lines(url)
    res = {
        'fw': '',
        'param': 'NONE (Not set)'
    }

    for buffer in lines:
        if "0x7FE0:" in buffer:
            is_all_ff = True
            p_chars = []
            for x in range(16):
                idx = 3 * x + 8
                if idx + 2 <= len(buffer):
                    val = int(buffer[idx:idx + 3], 16) & 0xFF
                    if val != 0xFF:
                        is_all_ff = False
                    if val == 0x00 or val == 0xFF:
                        break
                    p_chars.append(chr(val))
            if not is_all_ff and p_chars:
                res['param'] = "".join(p_chars)
            else:
                res['param'] = "NONE (Not set)"

        if "0x7FF0:" in buffer:
            fw_chars = []
            for x in range(16):
                idx = 3 * x + 8
                if idx + 2 <= len(buffer):
                    val = int(buffer[idx:idx + 3], 16) & 0xFF
                    if val == 0x00 or val == 0xFF:
                        break
                    fw_chars.append(chr(val))
            res['fw'] = "".join(fw_chars)

    return res


def normal_dump(lines):
    """Output raw hex dump with ASCII representation on the right."""
    for buffer in lines:
        line_clean = buffer.rstrip("\r\n")
        if len(line_clean) > 6 and line_clean[6] == ':':
            ascii_chars = []
            for x in range(8, 54, 3):
                if x + 2 <= len(line_clean):
                    try:
                        y = int(line_clean[x:x + 3], 16)
                        if 0x20 <= y <= 0x7E:
                            ascii_chars.append(chr(y))
                        else:
                            ascii_chars.append('.')
                    except ValueError:
                        ascii_chars.append(' ')
            print(f"{line_clean}  {''.join(ascii_chars)}")
        else:
            print(line_clean)


def _ip_address(dt, ofs):
    ign = "(ignore)" if dt[0x0C] != 0 else ""
    return f"{dt[ofs]}.{dt[ofs+1]}.{dt[ofs+2]}.{dt[ofs+3]} {ign}".strip()


def _lc_text(dt, ofs, length):
    chars = []
    for x in range(length):
        if ofs + x >= len(dt):
            break
        val = dt[ofs + x]
        if val < 0x20 or val > 0x7E:
            break
        chars.append(chr(val))
    return "".join(chars)


def _lc_bit_sw(dt, ofs):
    d = dt[ofs] if ofs < len(dt) else 0
    return bin(d)[2:].zfill(8)


def _lc_float(dt, ofs):
    if ofs + 4 > len(dt):
        return "NaN"
    d = (dt[ofs + 3] << 24) | (dt[ofs + 2] << 16) | (dt[ofs + 1] << 8) | dt[ofs]
    if d == 0xFFFFFFFF:
        return "NaN"
    try:
        raw_bytes = struct.pack('<I', d)
        f = struct.unpack('<f', raw_bytes)[0]
        return f"{f:.6f}"
    except Exception:
        return "NaN"


def _lc_lv(val):
    lv_map = {
        1: "A-1S-0",
        2: "A-1S-1",
        3: "A-10S-0",
        4: "A-10S-1",
        5: "A-1M-0",
        6: "A-1M-1",
        7: "B0",
        8: "B1",
        9: "S-1S-0",
        10: "S-1M-0",
    }
    return lv_map.get(val, "??")


def _response_ope(ope):
    op = ope & 0x7F
    ope_map = {
        0x00: " ",
        0x7F: " ",
        0x01: "==",
        0x02: ">",
        0x03: "<",
        0x04: ">=",
        0x05: "<=",
        0x06: "&&",
        0x07: "||",
        0x08: "!=",
    }
    return ope_map.get(op, "??")


def _lc_rly(dt, rnum, verc):
    if verc == 2:
        dtl, dth = 0x2D, 0x2E
    else:
        dtl, dth = 0x0E, 0x0F

    if 1 <= rnum <= 4:
        x = dt[dtl]
        s = (4 - rnum) * 2
    else:
        x = dt[dth]
        s = (8 - rnum) * 2

    y = (x >> s) & 0x03
    rly_map = {0: "CONT", 1: "BOTH", 2: "BREAK", 3: "MAKE"}
    return rly_map.get(y, "CONT")


def prt_lc_info(dt, k):
    if k == "uecsid":
        return "".join(f"{dt[i]:02X}" for i in range(6))
    elif k == "mac":
        return ":".join(f"{dt[i]:02X}" for i in range(6, 12))
    elif k == "dhcpf":
        return "STATIC" if dt[0x0C] == 0 else "DHCP"
    elif k == "ipaddress":
        return _ip_address(dt, 0x10)
    elif k == "netmask":
        return _ip_address(dt, 0x14)
    elif k == "defgw":
        return _ip_address(dt, 0x18)
    elif k == "dns":
        return _ip_address(dt, 0x1C)
    elif k == "vender":
        return _lc_text(dt, 0x40, 16)
    elif k == "nodename":
        return _lc_text(dt, 0x50, 16)
    elif k == "dbgmsg":
        return _lc_bit_sw(dt, 0x60)
    return ""


def prt_lc_rtx(dt, k, verc):
    if k == "valid":
        return "E" if (dt[0] & 0xFF) == 1 else "N"
    elif k == "room":
        return dt[1] & 0xFF
    elif k == "region":
        return dt[2] & 0xFF
    elif k == "order":
        return ((dt[4] & 0xFF) << 8) | (dt[3] & 0xFF)
    elif k in ("lifecnt", "priority"):
        return dt[5] & 0xFF
    elif k == "lv":
        return _lc_lv(dt[6])
    elif k == "cast":
        return dt[7] & 0xFF
    elif k == "sr":
        return dt[8] & 0xFF
    elif k == "ccmtype":
        return _lc_text(dt, 9 if verc == 2 else 8, 20)
    elif k == "unit":
        return _lc_text(dt, 0x1D if verc == 2 else 0x1C, 10)
    elif k == "opeccmtype":
        return _lc_text(dt, 6, 20)
    elif k == "ope":
        return _response_ope(dt[0x1A])
    elif k == "fval":
        return _lc_float(dt, 0x1B)
    elif k == "sthr":
        return dt[0x27 if verc == 2 else 0x01] & 0xFF
    elif k == "stmn":
        return dt[0x28 if verc == 2 else 0x02] & 0xFF
    elif k == "edhr":
        return dt[0x29 if verc == 2 else 0x03] & 0xFF
    elif k == "edmn":
        return dt[0x2A] & 0xFF if verc == 2 else dt[0x04] & 0xFF
    elif k == "inmn":
        if verc == 2:
            return dt[0x2B] & 0xFF
        else:
            return ((dt[0x07] & 0xFF) << 8) | (dt[0x06] & 0xFF)
    elif k == "dumn":
        if verc == 2:
            return dt[0x2C] & 0xFF
        else:
            return ((dt[0x09] & 0xFF) << 8) | (dt[0x08] & 0xFF)
    elif k.startswith("rly"):
        rnum = int(k[3:])
        return _lc_rly(dt, rnum, verc)
    elif k == "cond":
        if (dt[0x30] & 0xFF) == 0xFF or (dt[0x00] & 0xFF) != 1:
            return ""
        z = f",ope{dt[0x30] & 0xFF:02d}"
        for i in range(4):
            c_val = dt[0x31 + i] & 0xFF
            if c_val == 6:
                z += "&&"
            elif c_val == 7:
                z += "||"
            else:
                return z
            y = dt[0x32 + i] & 0xFF
            if y == 0xFF:
                return z
            z += f"ope{y:02d}"
        return z
    elif k == "mnflag":
        val = dt[0x05] & 0xFF
        if val == 0:
            return "SEC"
        elif val == 0xFF:
            return "MIN"
        return "???"
    elif k.startswith("cmpccmid"):
        idx = int(k[8:])
        offsets = [0x10, 0x17, 0x1E, 0x25, 0x2C]
        return dt[offsets[idx]] & 0xFF
    elif k.startswith("cmpope"):
        idx = int(k[6:])
        offsets = [0x11, 0x18, 0x1F, 0x26, 0x2D]
        return _response_ope(dt[offsets[idx]])
    elif k.startswith("cmpval"):
        idx = int(k[6:])
        offsets = [0x12, 0x19, 0x20, 0x27, 0x2E]
        return _lc_float(dt, offsets[idx])
    elif k.startswith("cmbcmp"):
        idx = int(k[6:])
        offsets = [None, 0x16, 0x1D, 0x24, 0x2B]
        return _response_ope(dt[offsets[idx]])

    return ""


def display_info(lines, verc, vers):
    hexdata = [0xFF] * 128
    for y in range(min(8, len(lines))):
        buf = lines[y]
        for x in range(16):
            idx = 3 * x + 8
            if idx + 2 <= len(buf):
                hexdata[y * 16 + x] = int(buf[idx:idx + 3], 16) & 0xFF

    print(f"      UECS-ID: {prt_lc_info(hexdata, 'uecsid')}")
    print(f"  F/W VERSION: {vers['fw']}")
    print(f"PARAM VERSION: {vers['param']}")
    print(f"  MAC ADDRESS: {prt_lc_info(hexdata, 'mac')}")
    print(f"    IP ASSIGN: {prt_lc_info(hexdata, 'dhcpf')}")
    print(f"     FIXED IP: {prt_lc_info(hexdata, 'ipaddress')}")
    print(f"      NETMASK: {prt_lc_info(hexdata, 'netmask')}")
    print(f"      GATEWAY: {prt_lc_info(hexdata, 'defgw')}")
    print(f"          DNS: {prt_lc_info(hexdata, 'dns')}")
    print(f"       VENDER: {prt_lc_info(hexdata, 'vender')}")
    print(f"    NODE NAME: {prt_lc_info(hexdata, 'nodename')}")
    print(f"    DEBUG MSG: {prt_lc_info(hexdata, 'dbgmsg')}")


def display_rx(lines, verc, state):
    for y in range(4):
        hexdata = [0xFF] * 64
        for w in range(4):
            line_idx = y * 4 + w
            if line_idx < len(lines):
                buf = lines[line_idx]
                for x in range(16):
                    idx = 3 * x + 8
                    if idx + 2 <= len(buf):
                        hexdata[w * 16 + x] = int(buf[idx:idx + 3], 16) & 0xFF
        n = state['n']
        if verc == 2:
            row = (
                f"{n:02d}:{prt_lc_rtx(hexdata, 'valid', verc)},"
                f"{prt_lc_rtx(hexdata, 'room', verc):3d},{prt_lc_rtx(hexdata, 'region', verc):3d},"
                f"{prt_lc_rtx(hexdata, 'order', verc):3d},{prt_lc_rtx(hexdata, 'priority', verc):2d},"
                f"{prt_lc_rtx(hexdata, 'lv', verc)},{prt_lc_rtx(hexdata, 'cast', verc)},"
                f"{chr(prt_lc_rtx(hexdata, 'sr', verc))},{prt_lc_rtx(hexdata, 'ccmtype', verc)},"
                f"{prt_lc_rtx(hexdata, 'unit', verc)},{prt_lc_rtx(hexdata, 'sthr', verc)},"
                f"{prt_lc_rtx(hexdata, 'stmn', verc)},{prt_lc_rtx(hexdata, 'edhr', verc)},"
                f"{prt_lc_rtx(hexdata, 'edmn', verc)},{prt_lc_rtx(hexdata, 'inmn', verc)},"
                f"{prt_lc_rtx(hexdata, 'dumn', verc)},{prt_lc_rtx(hexdata, 'rly1', verc)},"
                f"{prt_lc_rtx(hexdata, 'rly2', verc)},{prt_lc_rtx(hexdata, 'rly3', verc)},"
                f"{prt_lc_rtx(hexdata, 'rly4', verc)},{prt_lc_rtx(hexdata, 'rly5', verc)},"
                f"{prt_lc_rtx(hexdata, 'rly6', verc)},{prt_lc_rtx(hexdata, 'rly7', verc)},"
                f"{prt_lc_rtx(hexdata, 'rly8', verc)}{prt_lc_rtx(hexdata, 'cond', verc)}"
            )
            print(row)
        else:
            row = (
                f"{n:02d}:{prt_lc_rtx(hexdata, 'valid', verc)},"
                f"{prt_lc_rtx(hexdata, 'sthr', verc):02d}:{prt_lc_rtx(hexdata, 'stmn', verc):02d},"
                f"{prt_lc_rtx(hexdata, 'edhr', verc):02d}:{prt_lc_rtx(hexdata, 'edmn', verc):02d},"
                f"{prt_lc_rtx(hexdata, 'mnflag', verc)},"
                f"{prt_lc_rtx(hexdata, 'inmn', verc)},{prt_lc_rtx(hexdata, 'dumn', verc)},"
                f"{prt_lc_rtx(hexdata, 'rly1', verc)},{prt_lc_rtx(hexdata, 'rly2', verc)},"
                f"{prt_lc_rtx(hexdata, 'rly3', verc)},{prt_lc_rtx(hexdata, 'rly4', verc)},"
                f"{prt_lc_rtx(hexdata, 'rly5', verc)},{prt_lc_rtx(hexdata, 'rly6', verc)},"
                f"{prt_lc_rtx(hexdata, 'rly7', verc)},{prt_lc_rtx(hexdata, 'rly8', verc)},"
                f"{prt_lc_rtx(hexdata, 'cmpccmid0', verc)},{prt_lc_rtx(hexdata, 'cmpope0', verc)},"
                f"{float(prt_lc_rtx(hexdata, 'cmpval0', verc)) if prt_lc_rtx(hexdata, 'cmpval0', verc) != 'NaN' else 'NaN'}, "
                f"{prt_lc_rtx(hexdata, 'cmbcmp1', verc)},{prt_lc_rtx(hexdata, 'cmpccmid1', verc)},"
                f"{prt_lc_rtx(hexdata, 'cmpope1', verc)}, {float(prt_lc_rtx(hexdata, 'cmpval1', verc)) if prt_lc_rtx(hexdata, 'cmpval1', verc) != 'NaN' else 'NaN'}, "
                f"{prt_lc_rtx(hexdata, 'cmbcmp2', verc)},{prt_lc_rtx(hexdata, 'cmpccmid2', verc)},"
                f"{prt_lc_rtx(hexdata, 'cmpope2', verc)}, {float(prt_lc_rtx(hexdata, 'cmpval2', verc)) if prt_lc_rtx(hexdata, 'cmpval2', verc) != 'NaN' else 'NaN'}, "
                f"{prt_lc_rtx(hexdata, 'cmbcmp3', verc)},{prt_lc_rtx(hexdata, 'cmpccmid3', verc)},"
                f"{prt_lc_rtx(hexdata, 'cmpope3', verc)}, {float(prt_lc_rtx(hexdata, 'cmpval3', verc)) if prt_lc_rtx(hexdata, 'cmpval3', verc) != 'NaN' else 'NaN'}, "
                f"{prt_lc_rtx(hexdata, 'cmbcmp4', verc)},{prt_lc_rtx(hexdata, 'cmpccmid4', verc)},"
                f"{prt_lc_rtx(hexdata, 'cmpope4', verc)}, {float(prt_lc_rtx(hexdata, 'cmpval4', verc)) if prt_lc_rtx(hexdata, 'cmpval4', verc) != 'NaN' else 'NaN'}"
            )
            print(row)

        state['n'] += 1
        if state['n'] > 29:
            return


def display_tx(lines, verc, state):
    wmax = 4 if verc == 2 else 3
    for y in range(4):
        hexdata = [0xFF] * (wmax * 16)
        for w in range(wmax):
            line_idx = y * wmax + w
            if line_idx < len(lines):
                buf = lines[line_idx]
                for x in range(16):
                    idx = 3 * x + 8
                    if idx + 2 <= len(buf):
                        hexdata[w * 16 + x] = int(buf[idx:idx + 3], 16) & 0xFF
        n = state['n']
        if verc == 2:
            row = (
                f"{n:02d}:{prt_lc_rtx(hexdata, 'valid', verc)},"
                f"{prt_lc_rtx(hexdata, 'room', verc):3d},{prt_lc_rtx(hexdata, 'region', verc):3d},"
                f"{prt_lc_rtx(hexdata, 'order', verc):3d},{prt_lc_rtx(hexdata, 'priority', verc):2d},"
                f"{prt_lc_rtx(hexdata, 'lv', verc)},{prt_lc_rtx(hexdata, 'cast', verc)},"
                f"{chr(prt_lc_rtx(hexdata, 'sr', verc))},{prt_lc_rtx(hexdata, 'ccmtype', verc)},"
                f"{prt_lc_rtx(hexdata, 'unit', verc)},{prt_lc_rtx(hexdata, 'sthr', verc)},"
                f"{prt_lc_rtx(hexdata, 'stmn', verc)},{prt_lc_rtx(hexdata, 'edhr', verc)},"
                f"{prt_lc_rtx(hexdata, 'edmn', verc)},{prt_lc_rtx(hexdata, 'inmn', verc)},"
                f"{prt_lc_rtx(hexdata, 'dumn', verc)},{prt_lc_rtx(hexdata, 'rly1', verc)},"
                f"{prt_lc_rtx(hexdata, 'rly2', verc)},{prt_lc_rtx(hexdata, 'rly3', verc)},"
                f"{prt_lc_rtx(hexdata, 'rly4', verc)},{prt_lc_rtx(hexdata, 'rly5', verc)},"
                f"{prt_lc_rtx(hexdata, 'rly6', verc)},{prt_lc_rtx(hexdata, 'rly7', verc)},"
                f"{prt_lc_rtx(hexdata, 'rly8', verc)}{prt_lc_rtx(hexdata, 'cond', verc)}"
            )
            print(row)
        else:
            row = (
                f"{n:02d}:{prt_lc_rtx(hexdata, 'valid', verc)},"
                f"{prt_lc_rtx(hexdata, 'room', verc):3d},{prt_lc_rtx(hexdata, 'region', verc):3d},"
                f"{prt_lc_rtx(hexdata, 'order', verc):3d},{prt_lc_rtx(hexdata, 'priority', verc):2d},"
                f"{prt_lc_rtx(hexdata, 'lv', verc)},{prt_lc_rtx(hexdata, 'cast', verc)},"
                f"{prt_lc_rtx(hexdata, 'ccmtype', verc)},{prt_lc_rtx(hexdata, 'unit', verc)}"
            )
            print(row)

        state['n'] += 1
        if state['n'] > 9:
            return


def display_ope(lines, verc, state):
    for y in range(min(8, len(lines) // 2)):
        hexdata = [0xFF] * 32
        for w in range(2):
            line_idx = y * 2 + w
            if line_idx < len(lines):
                buf = lines[line_idx]
                for x in range(16):
                    idx = 3 * x + 8
                    if idx + 2 <= len(buf):
                        hexdata[w * 16 + x] = int(buf[idx:idx + 3], 16) & 0xFF
        n = state['n']
        print(
            f"{n:02d}:{prt_lc_rtx(hexdata, 'valid', verc)},"
            f"{prt_lc_rtx(hexdata, 'room', verc):3d},{prt_lc_rtx(hexdata, 'region', verc):3d},"
            f"{prt_lc_rtx(hexdata, 'order', verc):3d},{prt_lc_rtx(hexdata, 'lifecnt', verc):2d},"
            f"{prt_lc_rtx(hexdata, 'opeccmtype', verc)}{prt_lc_rtx(hexdata, 'ope', verc)}"
            f"{prt_lc_rtx(hexdata, 'fval', verc)}"
        )
        state['n'] += 1
        if state['n'] > 9:
            return


def main():
    if len(sys.argv) == 1:
        print(f"Usage: {APP_NAME} [-V] [-2] [-u] -d <info|rx|tx|ope> <host>")
        print("  -V, --version  Show version")
        print("  -d <target>    Dump target (info, rx, tx, ope)")
        print("  -u             Human-readable formatted output")
        print("  -2             Version 2.x mode (default: 3.x)")
        sys.exit(1)

    parser = argparse.ArgumentParser(
        prog=APP_NAME,
        description="M304 EEPROM Dump & Parser Tool"
    )
    parser.add_argument(
        "-V", "--version",
        action="version",
        version=f"%(prog)s version {APP_VERSION}",
        help="Show program version and exit"
    )
    parser.add_argument(
        "-d", "--dump",
        choices=['rx', 'sched', 'tx', 'ope', 'info'],
        required=True,
        help="Dump target: rx/sched, tx, ope, or info"
    )
    parser.add_argument(
        "-u", "--user",
        action="store_true",
        help="Human readable formatted output"
    )
    parser.add_argument(
        "-2", "--ver2",
        action="store_true",
        help="Specify M304 firmware Version 2.x mode (default: 3.x)"
    )
    parser.add_argument("host", help="Target node IP or hostname")

    args = parser.parse_args()

    verc = 2 if args.ver2 else 3
    optd = args.dump

    addr_map = {
        'rx': (0x1000, 8),
        'sched': (0x1000, 8),
        'tx': (0x3000, 3),
        'ope': (0x5000, 2),
        'info': (0x0000, 1),
    }

    addr, lcnt = addr_map[optd]
    vers = get_versions(args.host)

    state = {'n': 0}

    for lc in range(lcnt):
        a = addr + (lc * 0x100)
        url = f"http://{args.host}/L00{a:X}00"
        lines = fetch_lines(url)
        if not lines:
            continue

        if args.user:
            if optd == 'info':
                display_info(lines, verc, vers)
            elif optd in ('rx', 'sched'):
                display_rx(lines, verc, state)
            elif optd == 'tx':
                display_tx(lines, verc, state)
            elif optd == 'ope':
                display_ope(lines, verc, state)
        else:
            normal_dump(lines)


if __name__ == '__main__':
    main()
