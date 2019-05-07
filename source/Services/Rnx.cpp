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

#include "Rnx.hpp"

static Service g_rnxSrv;
static u64 g_rnxRefCnt;

bool Rnx::IsUsingReiNX() {
    #ifdef __SWITCH__
    if (serviceIsActive(&g_rnxSrv)) return true;
    #endif
    return false;
}

Result Rnx::Initialize() {
    Result rc = 0;
    #ifdef __SWITCH__
    atomicIncrement64(&g_rnxRefCnt);
    if (serviceIsActive(&g_rnxSrv)) return 0;
    rc = smGetService(&g_rnxSrv, "rnx");
    #endif
    return rc;
}

void Rnx::Exit() {
    #ifdef __SWITCH__
    if (atomicDecrement64(&g_rnxRefCnt) == 0) {
        serviceClose(&g_rnxSrv);
    }
    #endif
}

Result Rnx::SetHbTidForDelta(u64 tid) {
    Result rc = 0;
    #ifdef __SWITCH__
    IpcCommand c;
    ipcInitialize(&c);
    struct Raw
    {
        u64 Magic;
        u64 CmdId;
        u64 Tid;
    } *raw;
    
    raw = (struct Raw*)ipcPrepareHeader(&c, sizeof(*raw));
    raw->Magic = SFCI_MAGIC;
    raw->CmdId = 1;
    raw->Tid = tid;
    
    rc = serviceIpcDispatch(&g_rnxSrv);
    if(R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);
        struct Parsed
        {
            u64 Magic;
            u64 Result;
        } *resp = (struct Parsed*)r.Raw;
        rc = resp->Result;
    }
    #endif
    return rc;
}