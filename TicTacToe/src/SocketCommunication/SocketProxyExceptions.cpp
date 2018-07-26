#include "SocketProxyExceptions.h"

using namespace SocketProxy::Exceptions;

SocketProxyException::SocketProxyException(const char* const msg) : m_msg(msg)
{
}

const char* SocketProxyException::what() const throw()
{
	return m_msg;
}

const char* EndOfConnection::what() const throw()
{
	return "End of connection";
}
