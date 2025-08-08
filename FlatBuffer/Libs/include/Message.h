#pragma once

class CMessage
{
public:
	enum { header_length = 4 };
	enum { max_body_length = 512 };

public:
	CMessage() {}
	CMessage(const uint8_t* body, size_t body_size)
		: m_Body_length(body_size)
	{
		assert(body_size <= max_body_length);
		std::copy(body, body + body_size, this->body());
		encode_header();
	}

public:
	const uint8_t* data() const
	{
		return m_pData;
	}

	uint8_t* data()
	{
		return m_pData;
	}

	std::size_t length() const
	{
		return header_length + m_Body_length;
	}

	const uint8_t* body() const
	{
		return m_pData + header_length;
	}

	uint8_t* body()
	{
		return m_pData + header_length;
	}

	std::size_t body_length() const
	{
		return m_Body_length;
	}

	void body_size(std::size_t new_size)
	{
		assert(new_size <= max_body_length);
		m_Body_length = new_size;
	}

	bool decode_header()
	{
		char header[header_length + 1] = { 0, };
		strncat_s(header, (char*)m_pData, header_length);
		m_Body_length = std::atoi(header);
		if (m_Body_length > max_body_length)
		{
			m_Body_length = 0;
			return false;
		}

		return true;
	}

	void encode_header()
	{
		char header[header_length + 1] = { 0, };
		sprintf_s(header, "%4d", static_cast<int>(m_Body_length));
		memcpy(m_pData, header, header_length);
	}

private:
	uint8_t m_pData[header_length + max_body_length] = { 0, };
	std::size_t m_Body_length = 0;
};