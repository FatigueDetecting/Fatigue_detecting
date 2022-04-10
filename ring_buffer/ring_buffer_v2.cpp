#ifndef _CRINGBUFFER_HPP_
#define _CRINGBUFFER_HPP_
//2646699 owns all rights in this document
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

	//清空环形缓冲区
	void Clear()
	{
		std::lock_guard<std::mutex> lock(mtx);
		m_nUnreadSize = 0;
		m_nWritePos = 0;
		m_nReadPos = 0;
	}

	//设置环形缓冲区
	//nSize：缓冲区大小
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
	//写入数据到环形缓冲区
	//返回值：写入的数据大小
	unsigned int WriteData(const T* pT, unsigned int nSize = 1)
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (nSize > m_nSize - m_nUnreadSize) //缓冲区大小不足时，仅写入最前面的数据
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
				// 如果区域长度-写入下标=剩余长度，而数字长度大于写入区域。
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
	//从环形缓冲区读取数据
	//nSize：缓冲区大小
	//返回值：读取的数据大小
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
	T* m_pBuffer;					//buffer
	unsigned int m_nSize;			//buffer_size

	unsigned int m_nUnreadSize;		//未读取的数据大小

	unsigned int m_nWritePos;		//write_label
	unsigned int m_nReadPos;		//read_label

	std::mutex mtx;//mutex
};
#endif // !_CRINGBUFFER_HPP_