#include <exception>
#include <string>

namespace SocketCommunication { namespace Exceptions
{
	struct SocketProxyException : public std::exception
	{
	public:
		SocketProxyException(const char* msg = "");
		
		virtual const char* what() const throw();
	
	private:
		const char* const m_msg;
	};
	
	struct EndOfConnection : public SocketProxyException
	{
		const char* what() const throw();
	};
}}
