/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/30 13:40
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef RENDER_H
#define RENDER_H

#include <atomic>
#include <cassert>

class RenderObject {
public:
	virtual ~RenderObject() = default;

	void AddRef() { m_refCount.fetch_add(1, std::memory_order_relaxed); }

	void SubRef() {
		assert(m_refCount.load() >= 1);
		m_refCount.fetch_sub(1, std::memory_order_relaxed);
		if(m_refCount.load() == 0) {
			delete this;
		}
	}

private:
	std::atomic<uint32_t> m_refCount = 1;
};

#endif // RENDER_H
