if request.isInit:
    SPINLOCKS = [1 for _ in range(0,32)]
elif request.isRead:
    if SPINLOCKS[request.offset // 4]:
        SPINLOCKS[request.offset // 4] = 0
        request.value = 1
    else:
        request.value = 0
elif request.isWrite:
    SPINLOCKS[request.offset // 4] = 1
    request.value = 1 << (request.offset // 4)
