/*
* ΔLaunch
* Copyright (C) 2018  Reisyukaku
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <functional>

class Memory
{
    public:
        static void RunInManagedHeap(size_t newHeap, std::function<void()> f) {
            void *addr;
            Result rc = 0;
            rc = svcSetHeapSize(&addr, newHeap);
            if (R_FAILED(rc)) fatalSimple(MAKERESULT(Module_Libnx, LibnxError_HeapAllocFailed));
            f();
            rc = svcSetHeapSize(&addr, GetMaxHeap());
            if (R_FAILED(rc)) fatalSimple(MAKERESULT(Module_Libnx, LibnxError_HeapAllocFailed));
        }
    private:
        static size_t GetMaxHeap() {
            size_t size = 0;
            size_t mem_available = 0, mem_used = 0;
            svcGetInfo(&mem_available, 6, CUR_PROCESS_HANDLE, 0);
            svcGetInfo(&mem_used, 7, CUR_PROCESS_HANDLE, 0);
            if (mem_available > mem_used+0x200000)
                size = (mem_available - mem_used - 0x200000) & ~0x1FFFFF;
            if (size==0)
                size = 0x2000000*16;
            return size;
        }
};