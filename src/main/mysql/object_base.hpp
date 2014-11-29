// 这个文件是 Poseidon 服务器应用程序框架的一部分。
// Copyleft 2014, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_MYSQL_OBJECT_BASE_HPP_
#define POSEIDON_MYSQL_OBJECT_BASE_HPP_

#include "../cxx_ver.hpp"
#include "../cxx_util.hpp"
#include "callbacks.hpp"
#include <string>
#include <sstream>
#include <cstdio>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/cstdint.hpp>
#include "connection.hpp"
#include "utilities.hpp"
#include "../atomic.hpp"
#include "../log.hpp"
#include "../virtual_shared_from_this.hpp"

namespace Poseidon {

class MySqlObjectBase
	: public virtual VirtualSharedFromThis
{
	friend class MySqlObjectImpl;

protected:
	static void batchAsyncLoad(const char *tableHint, std::string query,
		MySqlObjectFactoryCallback factory, MySqlBatchAsyncLoadCallback callback);

private:
	mutable volatile bool m_autoSaves;
	mutable void *m_context;

protected:
	mutable boost::shared_mutex m_mutex;

protected:
	MySqlObjectBase();
	// 不要不写析构函数，否则 RTTI 将无法在动态库中使用。
	~MySqlObjectBase();

protected:
	void invalidate() const;

public:
	bool isAutoSavingEnabled() const {
		return atomicLoad(m_autoSaves);
	}
	void enableAutoSaving() const {
		atomicStore(m_autoSaves, true);
	}
	void disableAutoSaving() const {
		atomicStore(m_autoSaves, false);
	}

	virtual const char *getTableName() const = 0;

	virtual void syncSave(std::string &sql, MySqlConnection &conn) const = 0;
	virtual void syncFetch(const MySqlConnection &conn) = 0;

	void asyncSave() const;
	void asyncLoad(std::string query, MySqlAsyncLoadCallback callback);
};

}

#endif
