; Copyright (c) 2021 KeaOS/3 developers and contributors
; SPDX-License-Identifier: GPL-3.0-or-later

global load_directory

load_directory:
    mov     cr3, rdi
    ret