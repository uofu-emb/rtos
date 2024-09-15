import ctypes

DIV_POSITION = [
    'DIV_UDIVIDEND',
    "DIV_UDIVISOR",
    'DIV_SDIVIDEND',
    "DIV_SDIVISOR",
    "DIV_QUOTIENT",
    "DIV_REMAINDER",
    "DIV_CSR"
]

DIV_CSR_READY = 1 << 1
DIV_CSR_DIRTY = 1

if request.isInit:
    DIV_DIVIDEND = 0
    DIV_DIVISOR = 0
    DIV_QUOTIENT = 0
    DIV_REMAINDER = 0
    DIV_CSR = DIV_CSR_READY
elif request.isWrite:
    DIV_CSR |= DIV_CSR_DIRTY
    key = DIV_POSITION[request.offset // 4]
    if key == "DIV_UDIVIDEND":
        DIV_DIVIDEND = ctypes.c_uint32(request.value).value
    elif key == "DIV_UDIVISOR":
        DIV_DIVISOR = ctypes.c_uint32(request.value).value
    elif key == "DIV_SDIVIDEND":
        DIV_DIVIDEND = ctypes.c_int32(request.value).value
    elif key == "DIV_SDIVISOR":
        DIV_DIVISOR = ctypes.c_int32(request.value).value
    elif key == "DIV_QUOTIENT":
        DIV_QUOTIENT = request.value
        DIV_CSR |= DIV_CSR_READY
    elif key == "DIV_REMAINDER":
        DIV_REMAINDER = request.value
        DIV_CSR |= DIV_CSR_READY
    else:
        raise NotImplementedError("Something is horribly wrong, the key should be caught")

    if key == "DIV_UDIVIDEND" or key == "DIV_UDIVISOR" \
       or key == "DIV_SDIVIDEND" or key == "DIV_SDIVISOR":
        if DIV_DIVISOR != 0:
            DIV_QUOTIENT =  DIV_DIVIDEND // DIV_DIVISOR
            DIV_REMAINDER = DIV_DIVIDEND % DIV_DIVISOR
            DIV_CSR |= DIV_CSR_READY

elif request.isRead:
    key = DIV_POSITION[request.offset // 4]
    # Reading from QUOTIENT clears the CSR_DIRTY flag, so should read results in
    # the order REMAINDER, QUOTIENT if CSR_DIRTY is used
    if key == "DIV_UDIVIDEND" or key == "DIV_SDIVIDEND":
        request.value = DIV_DIVIDEND
    elif key == "DIV_UDIVISOR" or key == "DIV_SDIVISOR":
        request.value = DIV_DIVISOR
    elif key == "DIV_QUOTIENT":
        DIV_CSR &= ~(DIV_CSR_DIRTY)
        request.value = DIV_QUOTIENT
    elif key == "DIV_REMAINDER":
        request.value = DIV_REMAINDER
    elif key == "DIV_CSR":
        request.value = DIV_CSR
    else:
        raise NotImplementedError("Something is horribly wrong, the key should be caught")
