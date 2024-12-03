#pragma once
#include "net_common.h"

namespace nw 
{
	namespace net 
	{
		template<typename T>
		class tsqueue 
		{
		public:
			tsqueue() = default;
			tsqueue(const tsqueue<T>&) = delete;
			virtual ~tsqueue() { clear(); }

		public:
			// Returns and maintains item at the front of the Queue
			const T& front()
			{
				std::scoped_lock lock(muxQueue);
				return deQueue.front();
			}
			// Returns and maintains item at the back of the Queue
			const T& back()
			{
				std::scoped_lock lock(muxQueue);
				return deQueue.back();
			}
			// Adds item to front
			void push_front(const T& item)
			{
				std::scoped_lock lock(muxQueue);
				return deQueue.emplace_front(std::move(item));
			}
			// Adds item to back
			void push_back(const T& item) 
			{
				std::scoped_lock lock(muxQueue);
				return deQueue.emplace_back(std::move(item));
			}
			// Returns true if queue has no items
			bool empty() 
			{
				std::scoped_lock lock(muxQueue);
				return deQueue.empty();
			}
			// Returns number of items in queue
			size_t count() 
			{
				std::scoped_lock lock(muxQueue);
				return deQueue.size();
			}
			// Clears queue
			void clear() 
			{
				std::scoped_lock lock(muxQueue);
				return deQueue.clear();
			}
			// Removes and returns item at front of queue
			const T& pop_front() 
			{
				std::scoped_lock lock(muxQueue);
				auto item = std::move(deqQueue.front());
				deqQueue.pop_front();
				return item;
			}
			// Removes and returns item at back of queue
			const T& pop_back() 
			{
				std::scoped_lock lock(muxQueue);
				auto item = std::move(deqQueue.back());
				deqQueue.pop_back();
				return item;
			}


		protected:
			std::mutex muxQueue;
			std::deque deqQueue;
		};
	}
}