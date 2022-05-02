#ifndef _CRINGBUFFER_HPP_
#define _CRINGBUFFER_HPP_

#include <mutex>
#include <assert.h>

template <class T>
class CRingBuffer
{
public:
	CRingBuffer(unsigned int nSize)
	{
		m_pBuffer = new T[nSize];
		if (m_pBuffer != NULL)
		{
			memset(m_pBuffer, 0x00, nSize * sizeof(T));
			m_nSize = nSize;
			m_nUnreadSize = 0;
			m_nWritePos = 0;
			m_nReadPos = 0;
		}
	}

	~CRingBuffer()
	{
		// automatic lock sth
		std::lock_guard<std::mutex> lock(mtx);
		if (m_pBuffer != NULL)
		{
			delete[] m_pBuffer;
			m_pBuffer = NULL;
			m_nSize = 0;
			m_nUnreadSize = 0;
			m_nWritePos = 0;
			m_nReadPos = 0;
		}
	}

	//Emptying the ring buffer
	void Clear()
	{
		std::lock_guard<std::mutex> lock(mtx);
		m_nUnreadSize = 0;
		m_nWritePos = 0;
		m_nReadPos = 0;
	}

	//Setting up the ring buffer
	//nSize：Buffer size
	void ReSetBufferSize(unsigned int nSize)
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (m_pBuffer != NULL)
		{
			delete[] m_pBuffer;
			m_pBuffer = NULL;
			m_nSize = 0;
			m_nUnreadSize = 0;
			m_nWritePos = 0;
			m_nReadPos = 0;
		}

		m_pBuffer = new T[nSize];
		if (m_pBuffer != NULL)
		{
			memset(m_pBuffer, 0x00, nSize * sizeof(T));
			m_nSize = nSize;
			m_nUnreadSize = 0;
			m_nWritePos = 0;
			m_nReadPos = 0;
		}
	}
	//Write data to ring buffer
	//Return value: size of data written
	unsigned int WriteData(const T* pT, unsigned int nSize = 1)
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (nSize > m_nSize - m_nUnreadSize) //If the buffer is undersized, only the top data is written
			nSize = m_nSize - m_nUnreadSize;

		if (nSize > 0)
		{
			if (m_nWritePos + nSize <= m_nSize)
			{
				memcpy(&m_pBuffer[m_nWritePos], pT, nSize * sizeof(T));
			}
			else
			{
				memcpy(&m_pBuffer[m_nWritePos], pT, (m_nSize - m_nWritePos) * sizeof(T));
				// If the area length - write subscript = remaining length and the number length is greater than the write area.
				if (nSize - (m_nSize - m_nWritePos) > 0)
					memcpy(m_pBuffer, &pT[m_nSize - m_nWritePos], (nSize - (m_nSize - m_nWritePos)) * sizeof(T));
			}

			m_nUnreadSize += nSize;
			m_nWritePos += nSize;
			if (m_nWritePos >= m_nSize)
				m_nWritePos %= m_nSize;
		}

		return nSize;
	}
	//Reading data from the ring buffer
	//nSize：Buffer size
	//Return value: size of data read
	unsigned int ReadData(T* pT, unsigned int nSize = 1)
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (nSize > m_nUnreadSize || m_nUnreadSize == 0)
			nSize = m_nUnreadSize;

		if (nSize > 0)
		{
			if (m_nReadPos + nSize <= m_nSize)
			{
				memcpy(pT, &m_pBuffer[m_nReadPos], nSize * sizeof(T));
			}
			else
			{
				memcpy(pT, &m_pBuffer[m_nReadPos], (m_nSize - m_nReadPos) * sizeof(T));
				if (nSize - (m_nSize - m_nReadPos) > 0)
					memcpy(&pT[m_nSize - m_nReadPos], m_pBuffer, (nSize - (m_nSize - m_nReadPos)) * sizeof(T));
			}

			m_nUnreadSize -= nSize;
			m_nReadPos += nSize;
			if (m_nReadPos >= m_nSize)
				m_nReadPos %= m_nSize;
		}

		return nSize;
	}

	unsigned int GetLength()
	{
		std::lock_guard<std::mutex> lock(mtx);
		return m_nUnreadSize;
	}

private:
	T* m_pBuffer;					//buffer zone 
	unsigned int m_nSize;			//buffer size

	unsigned int m_nUnreadSize;		//Size of unread data

	unsigned int m_nWritePos;		//Write to subscript
	unsigned int m_nReadPos;		//Read subscript

	std::mutex mtx;//Thread locks
};
#endif // !_CRINGBUFFER_HPP_
