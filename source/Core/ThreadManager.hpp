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
#include <switch.h>

class ThreadManager {
	public:
		ThreadManager() {
			m_shouldRun = true;
			m_theadExited = false;
			memset(&t, 0, sizeof(t));
		}

		ThreadManager(std::function<bool()> callback) {
			m_shouldRun = true;
			m_theadExited = false;
			memset(&t, 0, sizeof(t));
			this->callback() = callback;
		}

		virtual ~ThreadManager() {
			if (shouldRun())
				m_shouldRun = false;

			join();
		}

		void join(){
			if (!threadFinished())
				threadWaitForExit(&t);
			threadClose(&t);
		}

		bool start(u32 threadOffset = 1, u8 priority = 0x3B) {
			memset(&t, 0, sizeof(t));
			if (threadCreate(&t, (void(*)(void*))&__threadEntry, this, 0x20000, priority, (svcGetCurrentProcessorNumber() + threadOffset) % 4))
				return false;

			if (threadStart(&t))
				return false;
			
			return true;
		}

		virtual bool init() { return true; }

		virtual bool step() {
			if (callback()) 
				return callback()();
			return false;	
		}

		virtual void exit() { /*STUB*/ }

		static void __threadEntry(ThreadManager* p) {

			if (!p || !p->init()) {
				if (p)
					p->shouldRun() = false;

				return;
			}

			while (p->shouldRun() && p->step()) {}

			p->exit();
			p->threadFinished() = true;
		}

		bool& shouldRun() { return m_shouldRun;	}
		bool& threadFinished() { return m_theadExited; }
		std::function<bool()>& callback() { return m_callback; }

	protected:
		std::function<bool()> m_callback;
		bool m_shouldRun;
		bool m_theadExited;
		Thread t;
};