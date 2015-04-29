// 这个文件是 Poseidon 服务器应用程序框架的一部分。
// Copyleft 2014 - 2015, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_EPOLL_HPP_
#define POSEIDON_EPOLL_HPP_

#include "cxx_util.hpp"
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <vector>
#include <cstddef>
#include "raii.hpp"
#include "mutex.hpp"

namespace Poseidon {

class TcpSessionBase;

class Epoll : NONCOPYABLE, public boost::enable_shared_from_this<Epoll> {
	friend TcpSessionBase;

private:
	class SessionMapDelegator;

private:
	mutable Mutex m_mutex;
	UniqueFile m_epoll;
	boost::scoped_ptr<SessionMapDelegator> m_sessions;

public:
	Epoll();
	~Epoll();

private:
	void notifyWriteable(TcpSessionBase *session);
	void notifyUnlinked(TcpSessionBase *session);

public:
	void addSession(const boost::shared_ptr<TcpSessionBase> &session);
	void removeSession(const boost::shared_ptr<TcpSessionBase> &session);
	void snapshot(std::vector<boost::shared_ptr<TcpSessionBase> > &sessions) const;
	void clear();

	// 这三个函数必须位于同一个线程内调用。
	std::size_t wait(unsigned timeout) NOEXCEPT;
	std::size_t pumpReadable();
	std::size_t pumpWriteable();
};

}

#endif