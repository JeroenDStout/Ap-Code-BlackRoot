import sys

def fnv1a_128(str) -> int:
    prime  = 0x0000000001000000000000000000013B
    offset = 0x6c62272e07bb014262b821756295c58d

    b = bytearray()
    b.extend(map(ord, str))
    
    ret = offset;
    length = len(b)
    
    for i in range(length):
        ret *= prime;
        ret ^= b[i];
        ret &= 0xffffffffffffffffffffffffffffffff
    
    return ret
    
fnv1a_128("Aglaea::Sluice Simple Command Stream v0.0.1")