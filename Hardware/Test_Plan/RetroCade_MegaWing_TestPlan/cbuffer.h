#ifndef __CBUFFER_H__
#define __CBUFFER_H__



template<typename T, unsigned int bits> struct CircularBuffer
{
	CircularBuffer():head(0),tail(0) {}

	static const unsigned int mask = (1<<bits)-1;

	inline bool hasData() { return head!=tail; }
	inline bool isFull() { return  (((tail+1) & mask) == head); }

	bool push(const T &value) {
		unsigned int nextTail = increment(tail);
		if(nextTail != head)
		{
			buffer[tail] = value;
			tail = nextTail;
			return true;
		}
		return false;
	}

	inline T pop()
	{
		// No checks
		T ret = buffer[head];
		head = increment(head);
		return ret;
	}

	void clear()
	{
		cli();
		head=tail=0;
		sei();
	}


	inline unsigned increment(unsigned int i)
	{
		return (i+1)&mask;
	}

	volatile unsigned int head, tail;

	T buffer[1<<bits];
};

#endif
